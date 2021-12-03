#include "decoder.h"
#include "cmsis_os.h"
//size buffer to receive from uart interrupt
#define SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT 1
#define SIZE_XQUEUEUARTPCRX 20


#if SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT<1
#error "zmienna SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT nie może byc równa 0"
#endif

typedef struct{
	UART_HandleTypeDef *huart;
	uint8_t *pDataToRxhuart;
	uint16_t sizepDataToRxHuart;
}controlPeripheralStruct;

decoderStructure decoderGeneralStructure;
controlPeripheralStruct peripheralStruct;
xQueueHandle xQueueUartPCRx;
xTaskHandle xTaskDecodeData;


void vTaskDecodeData();
void generalModuleInit(controlPeripheralStruct *peripheralStruct,UART_HandleTypeDef *huart);

void generalModuleUartRxInterrupt(controlPeripheralStruct *peripheralStruct);

static void generalModuleUartReceiveIt(controlPeripheralStruct *peripheralStruct);
