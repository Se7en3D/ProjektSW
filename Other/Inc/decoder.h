/**
  ******************************************************************************
  * @file    decoder.h
  * @author  Daniel Dunak
  * @brief   Plik naglowkowy dla modulu dekodera
  *
  ******************************************************************************
  */

#define DECODER_MAX_LENGTH_COMMAND 7 /*!<Maksymalny rozmiar rozkazu ktory moze zostac wyslany*/
#define DECODE_REJECT_COMMAND_ON 1/*!<stan dla komendy ktora ma byc sprawdzana*/
#define DECODE_REJECT_COMMAND_OFF 0/*!<stan dla komendy która ma nie byc sprawdzana*/
#define DECODE_ANSWER_ERROR 0x01/*!<Wartosc oznaczajaca bladana komende po odkodowania*/
#define DECODE_ANSWER_OK 0x02/*!<Wartosc oznaczajaca ze komenda zostala poprawnie rozpoznana*/
#define DECODE_ANSWER_INIT_MALLOC_ERROR 0x03/*!<Blad inicjalizacji przy uzyciu komendy malloc*/
#define DECODE_CLEAR_STRUCT_RESET_REJECTED_TABLE 0x04/*!<Wartosc oznaczjaca wymuszenie resetu tablicy dla dekodera*/
#define COMMAND_ON0 0x10/*!<Kod komendy wlaczajacej diode nr.0*/
#define COMMAND_ON1 0x11/*!<Kod komendy wlaczajacej diode nr.1*/
#define COMMAND_ON2 0x12/*!<Kod komendy wlaczajacej diode nr.2*/
#define COMMAND_ON3 0x13/*!<Kod komendy wlaczajacej diode nr.3*/
#define COMMAND_ON4 0x14/*!<Kod komendy wlaczajacej diode nr.4*/
#define COMMAND_ON5 0x15/*!<Kod komendy wlaczajacej diode nr.5*/
#define COMMAND_ON6 0x16/*!<Kod komendy wlaczajacej diode nr.6*/
#define COMMAND_ON7 0x17/*!<Kod komendy wlaczajacej diode nr.7*/
#define COMMAND_OFF0 0x20/*!<Kod komendy wylaczajacej diode nr.0*/
#define COMMAND_OFF1 0x21/*!<Kod komendy wylaczajacej diode nr.1*/
#define COMMAND_OFF2 0x22/*!<Kod komendy wylaczajacej diode nr.2*/
#define COMMAND_OFF3 0x23/*!<Kod komendy wylaczajacej diode nr.3*/
#define COMMAND_OFF4 0x24/*!<Kod komendy wylaczajacej diode nr.4*/
#define COMMAND_OFF5 0x25/*!<Kod komendy wylaczajacej diode nr.5*/
#define COMMAND_OFF6 0x26/*!<Kod komendy wylaczajacej diode nr.6*/
#define COMMAND_OFF7 0x27/*!<Kod komendy wylaczajacej diode nr.7*/
#define COMMAND_ONALL 0x30/*!<Kod komendy wylaczajacej wszystkie diody*/
#define COMMAND_OFALL 0x40/*!<Kod komendy wlaczajacej wszystkie diody*/
#define COMMAND_AT 0x50/*!<Kod komendy AT*/
#define COMMAND_INVALID 0x00/*!<Kod nieznanej komendy*/








  /**
   * @brief decoderArrayOfCommand tablica zawierająca wszystkie komendy
   */
static const volatile uint8_t decoderArrayOfCommand[][DECODER_MAX_LENGTH_COMMAND] = {
	"ONx\n",
	"OFFx\n",
	"ONALL\n",
	"OFFALL\n",
	"AT\n"
};

/**
 * @brief decoderStructure glowna struktura dekodera
 */
typedef struct{
	uint8_t* tableOfRejectedCommands;/*!< Wzkaznik do tablicy ktora zawiera akutalnie odrzucone komendy */
	uint8_t sizeTableOdRejectedCommands;/*!<Rozmiar tablicy odrzucanych komend jest sizeof(decoderArrayOfCommand)*/
	uint32_t command[DECODER_MAX_LENGTH_COMMAND];/*!<Tablica przechowujaca komende do odkowdowania*/
	uint8_t positionOfDecodeCommand;/*!<Pozycja aktualnie odkodowywanej wartosci*/
	uint8_t isEndDecode;/*!<Zmienna informujaca o zakonczeniu dekodowania*/
	uint8_t decodeValue;/*!<Zdekodowana wartosc*/
}decoderStructure;


void decoderInitStructure(decoderStructure* decoderGeneralStructure);
void decoderAddNextSign(decoderStructure *decoderGeneralStructure,uint8_t* letter);
void decoderSendAnswer(uint8_t what);
uint8_t decoderIsEndDecode(decoderStructure *decoderGeneralStructure);
uint8_t decoderGetDecodeValue(decoderStructure *decoderGeneralStructure);
void decoderClearStructure(decoderStructure* decoderGeneralStructure);
void decoderShowInfo(decoderStructure* decoderGeneralStructure);
uint8_t decoderGetDecodingEffect(decoderStructure* decoderGeneralStructure);



