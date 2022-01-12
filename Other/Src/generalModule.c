
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


	generalModuleUartReceiveIt(peripheralStruct);
	xQueueUartPCRx = xQueueGenericCreate(
					SIZE_XQUEUEUARTPCRX,
					sizeof(uint8_t),
					(uint8_t)0);
	xQueueUartPCTx =xQueueGenericCreate(
					SIZE_XQUEUEUARTPCTX,
					sizeof(uint8_t),
					(uint8_t)0);
	xQueueCommand =xQueueGenericCreate(
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
				//printf("Wartosc do wyslania=%x\n",decodeValue);
				if(decodeValue==COMMAND_AT){
					printf("Wspanialy projekt D.D. M.K.\n");
				}else{
					 xQueueGenericSend(xQueueCommand,&decodeValue,pdFALSE,queueSEND_TO_BACK);
				}
			}
		}
		vTaskDelay(1/portTICK_RATE_MS);
	};
}
void vTaskGPIOController(){
	static uint8_t data;
	while (1){
		if(xQueueReceive(xQueueCommand, &data, pdFALSE)){
			switchGPIO(data);
			vTaskDelay(100/portTICK_RATE_MS);
		}
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
/**
  * @brief
  * @param  uint8_t Komenda dekodera
  * @retval None
  */
void switchGPIO(uint8_t rozkaz){ //funkcja oczekuje na wartość int rozkazu pochodzącą z zewnętrznego źródła
	//funkcja switchGPIO wykorzystuje wbudowaną funkcję HAL
	switch (rozkaz)	{
	case 0x10: //zapalenie diody 0 ->PC1
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		break;
	case 0x11: //zapalenie diody 1 ->PC3
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		break;
	case 0x12: //zapalenie diody 2 ->PA1
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
		break;
	case 0x13: //zapalenie diody 3 ->PA3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
		break;
	case 0x14: //zapalenie diody 4 ->PA5
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case 0x15: //zapalenie diody 5 ->PA7
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		break;
	case 0x16: //zapalenie diody 6 ->PC5
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case 0x17: //zapalenie diody 7 ->PB1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		break;
	case 0x20: //zgaszenie diody 0 ->PC1
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		break;
	case 0x21: //zgaszenie diody 1 ->PC3
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		break;
	case 0x22: //zgaszenie diody 2 ->PA1
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		break;
	case 0x23: //zgaszenie diody 3 ->PA3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		break;
	case 0x24: //zgaszenie diody 4 ->PA5
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case 0x25: //zgaszenie diody 5 ->PA7
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		break;
	case 0x26: //zgaszenie diody 6 ->PC5
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
		break;
	case 0x27: //zgaszenie diody 7 ->PB1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		break;

	case 0x30: //zapalenie wszystkich wyjść
		switchGPIO(0x10); //zapalenie diody 0 ->PC1
		switchGPIO(0x11); //zapalenie diody 1 ->PC3
		switchGPIO(0x12); //zapalenie diody 2 ->PA1
		switchGPIO(0x13); //zapalenie diody 3 ->PA3
		switchGPIO(0x14); //zapalenie diody 4 ->PA5
		switchGPIO(0x15); //zapalenie diody 5 ->PA7
		switchGPIO(0x16); //zapalenie diody 6 ->PC5
		switchGPIO(0x17); //zapalenie diody 7 ->PB1
		break;

	case 0x31: //zgaszenie wszystkich wyjść
		switchGPIO(0x20); //zgaszenie diody 0 ->PC1
		switchGPIO(0x21); //zgaszenie diody 1 ->PC3
		switchGPIO(0x22); //zgaszenie diody 2 ->PA1
		switchGPIO(0x23); //zgaszenie diody 3 ->PA3
		switchGPIO(0x24); //zgaszenie diody 4 ->PA5
		switchGPIO(0x25); //zgaszenie diody 5 ->PA7
		switchGPIO(0x26); //zgaszenie diody 6 ->PC5
		switchGPIO(0x27); //zgaszenie diody 7 ->PB1
		break;


	}
}

