#include "decoder.h"
#include "circularBuffer.h"
#include "cmsis_os.h"
//size buffer to receive from uart interrupt
#define SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT 1
#define SIZE_XQUEUEUARTPCRX 20
#define SIZE_XQUEUEUARTPCTX 150
#define SIZE_BUF_FOR_SENDING_DATA 150



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
bufferStructure bufStruct;
xQueueHandle xQueueUartPCRx;
xTaskHandle xTaskDecodeData;
xTaskHandle xTaskSendRespond;
xQueueHandle xQueueUartPCTx;


void vTaskDecodeData();
void vTaskSendRespons();
void generalModuleInit(controlPeripheralStruct *peripheralStruct,UART_HandleTypeDef *huart);
int __io_putchar(int ch);
void generalModuleUartRxInterrupt(controlPeripheralStruct *peripheralStruct);

void generalModuleUartReceiveIt(controlPeripheralStruct *peripheralStruct);
