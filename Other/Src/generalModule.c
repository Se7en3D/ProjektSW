
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"

#include "generalModule.h"


void generalModuleInit(controlPeripheralStruct *peripheralStruct,UART_HandleTypeDef *huart){
	if(huart==0){
		return;
	}
		//Inicjalizacja struktury controlPeripheralStruct
	peripheralStruct->huart=huart;
	peripheralStruct->pDataToRxhuart=((uint8_t*) malloc(SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT));
	peripheralStruct->sizepDataToRxHuart=SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT;

		//Inicjalizacja struktury bufferStructure wykorzystywana do wysyłania danych przez UART
	circularBufferInitBuf(&bufStruct, SIZE_BUF_FOR_SENDING_DATA);

	generalModuleUartReceiveIt(peripheralStruct);
	xQueueUartPCRx = xQueueGenericCreate(
					SIZE_XQUEUEUARTPCRX,
					sizeof(uint8_t),
					(uint8_t)0);
	xQueueUartPCTx =xQueueGenericCreate(
					SIZE_XQUEUEUARTPCTX,
					sizeof(uint8_t),
					(uint8_t)0);
	xTaskCreate(
					vTaskDecodeData,       /* Function that implements the task. */
                    "DecoderData",          /* Text name for the task. */
					configMINIMAL_STACK_SIZE+100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                    &xTaskDecodeData );      /* Used to pass out the created task's handle. */
	xTaskCreate(
					vTaskSendRespons,       /* Function that implements the task. */
                    "RespondTask",          /* Text name for the task. */
					configMINIMAL_STACK_SIZE+100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                    &xTaskSendRespond );      /* Used to pass out the created task's handle. */
	xTaskCreate(
						vTaskGPIOController,       /* Function that implements the task. */
	                    "wywolanie",          /* Text name for the task. */
						configMINIMAL_STACK_SIZE+100,      /* Stack size in words, not bytes. */
	                    ( void * ) 1,    /* Parameter passed into the task. */
	                    tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
	                    &xTaskWywolanie );      /* Used to pass out the created task's handle. */
	decoderInitStructure(&decoderGeneralStructure);
}


void generalModuleUartRxInterrupt(controlPeripheralStruct *peripheralStruct){
	xQueueSendFromISR(xQueueUartPCRx, peripheralStruct->pDataToRxhuart, pdFALSE);
	generalModuleUartReceiveIt(peripheralStruct);
}


void generalModuleUartReceiveIt(controlPeripheralStruct *peripheralStruct){
	HAL_UART_Receive_IT(peripheralStruct->huart, peripheralStruct->pDataToRxhuart, peripheralStruct->sizepDataToRxHuart);
}

void vTaskDecodeData(){
	static uint8_t data;
	while(1){
		if(xQueueReceiveFromISR(xQueueUartPCRx, &data, pdFALSE)){
			decoderAddNextSign(&decoderGeneralStructure,&data);
			if(decoderIsEndDecode(&decoderGeneralStructure)){
				uint8_t decodeValue=decoderGetDecodeValue(&decoderGeneralStructure);
				printf("Wartosc do wyslania=%x\n",decodeValue);
				if(decodeValue==COMMAND_AT){
					printf("Wspanialy projekt D.D. M.K.\n");
				}else{
					//TODO dodać kod dla GPIO
				}
			}
		}
		vTaskDelay(1/portTICK_RATE_MS);
	};
}
void vTaskGPIOController(){
while (1){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	vTaskDelay(1000/portTICK_RATE_MS);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	vTaskDelay(1000/portTICK_RATE_MS);
}
}
void vTaskSendRespons(){
	static uint8_t data;
	while(1){
		if(xQueueReceive(xQueueUartPCTx, &data, pdFALSE)){
			HAL_UART_Transmit_IT(peripheralStruct.huart, &data, 1);
		}
		vTaskDelay(1/portTICK_RATE_MS);
	};
}
int __io_putchar(int ch){
  if (ch == '\n') {
    __io_putchar('\r');
  }
  uint8_t castCh=(uint8_t)ch;
  xQueueGenericSend(xQueueUartPCTx,&castCh,pdFALSE,queueSEND_TO_BACK);
  return 1;
}
