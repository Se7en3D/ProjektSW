
#include <stdio.h>
#include "stm32f4xx_hal.h"

#include "generalModule.h"


void generalModuleInit(controlPeripheralStruct *peripheralStruct,UART_HandleTypeDef *huart){
	if(huart==0){
		return;
	}
	peripheralStruct->huart=huart;
	peripheralStruct->pDataToRxhuart=((uint8_t*) malloc(SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT));
	peripheralStruct->sizepDataToRxHuart=SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT;
	generalModuleUartReceiveIt(peripheralStruct);
	xQueueUartPCRx = xQueueGenericCreate(SIZE_XQUEUEUARTPCRX, sizeof(uint8_t), (unsigned char)0);
	xTaskCreate(
					vTaskDecodeData,       /* Function that implements the task. */
                    "DecoderData",          /* Text name for the task. */
					configMINIMAL_STACK_SIZE+100,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                    &xTaskDecodeData );      /* Used to pass out the created task's handle. */
	decoderInitStructure(&decoderGeneralStructure);
}


void generalModuleUartRxInterrupt(controlPeripheralStruct *peripheralStruct){
	xQueueSendFromISR(xQueueUartPCRx, peripheralStruct->pDataToRxhuart, pdFALSE);
	generalModuleUartReceiveIt(peripheralStruct);
}


static void generalModuleUartReceiveIt(controlPeripheralStruct *peripheralStruct){
	HAL_UART_Receive_IT(peripheralStruct->huart, peripheralStruct->pDataToRxhuart, peripheralStruct->sizepDataToRxHuart);
}

void vTaskDecodeData(){
	static uint8_t data;
	while(1){
		if(xQueueReceiveFromISR(xQueueUartPCRx, &data, pdFALSE)){
			vTaskAddNextCommandLetter(&decoderGeneralStructure,&data);
		}
		vTaskDelay(10/portTICK_RATE_MS);
	};
}
