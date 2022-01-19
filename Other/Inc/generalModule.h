/**
  ******************************************************************************
  * @file    generalModule.h
  * @author  Daniel Dunak, Michał Kuska
  * @brief   Plik naglowkowy modulu zawierajacy taski oraz kolejki
  ******************************************************************************
  * @attention
    * PC1 -> Dioda nr.0 <br>
  * PC3	-> Dioda nr.1 <br>
  * PA1	-> Dioda nr.2 <br>
  * PA3	-> Dioda nr.3 <br>
  * PA5	-> Dioda nr.4 <br>
  * PA7	-> Dioda nr.5 <br>
  * PC5	-> Dioda nr.6 <br>
  * PB1	-> Dioda nr.7 <br>
  * PC12 -> UART5_TX <br>
  * PD2	-> UART5_RX <br>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "decoder.h"
#include "cmsis_os.h"

/* Exported constants --------------------------------------------------------*/
#define SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT 1 /*!< Rozmiar buforu do odbierania danych
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 z przerwania od odbiornika UART poprzez
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 interfejs HAL*/
#define SIZE_XQUEUEUARTPCRX 20 /*!< Rozmiar kolejki do dekodera komend*/
#define SIZE_XQUEUEUARTPCTX 150/*!<* Rozmiar kolejki do wysyłania danych poprzez interfejs UART*/




#if SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT<1
#error "zmienna SIZE_BUFFER_TO_RECEIVE_FROM_UART_INNTERUPT nie może byc równa 0"
#endif

/**
 * @brief Struktura kontrolujaca interfejs UART
 */
typedef struct{
	UART_HandleTypeDef *huart;	/*!< Wzkażnik do struktury UART_HandleTypeDef*/
	uint8_t *pDataToRxhuart; /*!< Wzkażnik do bufora danych wykorzystywanych w przerwaniu odbiornika UART */
	uint16_t sizepDataToRxHuart;/*!<Rozmiar bufora danych którego wzkażnikiem jest pDataToRxhuart*/
}controlPeripheralStruct;

decoderStructure decoderGeneralStructure; /*!<Struktura dekodera*/
controlPeripheralStruct peripheralStruct;/*!<Struktura kontrolera UART*/
xQueueHandle xQueueUartPCRx;/*!<Kolejki do dokodera*/
xTaskHandle xTaskDecodeData;/*!<Task dekodera*/
xTaskHandle xTaskSendRespond;/*!<Task wysyłania odpowiedzi*/
xQueueHandle xQueueUartPCTx;/*!<Kolejka do wysyłania odpowiedzi*/
xQueueHandle xQueueCommand;/*!<Kolejka do przekazywania rozkazów wymagających zmiany Diod*/
xTaskHandle xTaskWywolanie;/*!<Task do konfiguracji wyjści GPIO*/

void vTaskGPIOController();
void vTaskDecodeData();
void vTaskSendRespons();
void generalModuleInit(controlPeripheralStruct *peripheralStruct,UART_HandleTypeDef *huart);
int __io_putchar(int ch);
void generalModuleUartRxInterrupt(controlPeripheralStruct *peripheralStruct);

void generalModuleUartReceiveIt(controlPeripheralStruct *peripheralStruct);
void switchGPIO(uint8_t rozkaz);
