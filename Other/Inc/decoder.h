/**
  ******************************************************************************
  * @file    decoder.h
  * @author  Daniel Dunak
  * @brief   Plik nag��wkowy dla modu�u dekodera
  *
  ******************************************************************************
  */

#define DECODER_MAX_LENGTH_COMMAND 7 /*!<Maksymalny rozmiar rozkazu kt�ry mo�e zostac wys�any*/
#define DECODE_REJECT_COMMAND_ON 1
#define DECODE_REJECT_COMMAND_OFF 0
#define DECODE_ANSWER_ERROR 0x01
#define DECODE_ANSWER_OK 0x02
#define DECODE_ANSWER_INIT_MALLOC_ERROR 0x03
#define DECODE_CLEAR_STRUCT_RESET_REJECTED_TABLE 0x04
#define COMMAND_ON0 0x10
#define COMMAND_ON1 0x11
#define COMMAND_ON2 0x12
#define COMMAND_ON3 0x13
#define COMMAND_ON4 0x14
#define COMMAND_ON5 0x15
#define COMMAND_ON6 0x16
#define COMMAND_ON7 0x17
#define COMMAND_OFF0 0x20
#define COMMAND_OFF1 0x21
#define COMMAND_OFF2 0x22
#define COMMAND_OFF3 0x23
#define COMMAND_OFF4 0x24
#define COMMAND_OFF5 0x25
#define COMMAND_OFF6 0x26
#define COMMAND_OFF7 0x27
#define COMMAND_ONALL 0x30
#define COMMAND_OFALL 0x40
#define COMMAND_AT 0x50
#define COMMAND_INVALID 0x00








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
 * @brief decoderStructure g��wna struktura dekodera
 */
typedef struct{
	uint8_t* tableOfRejectedCommands;
	uint8_t sizeTableOdRejectedCommands;
	uint32_t command[DECODER_MAX_LENGTH_COMMAND];
	uint8_t positionOfDecodeCommand;
	uint8_t isEndDecode;
	uint8_t decodeValue;
}decoderStructure;


void decoderInitStructure(decoderStructure* decoderGeneralStructure);
void decoderAddNextSign(decoderStructure *decoderGeneralStructure,uint8_t* letter);
void decoderSendAnswer(uint8_t what);
uint8_t decoderIsEndDecode(decoderStructure *decoderGeneralStructure);
uint8_t decoderGetDecodeValue(decoderStructure *decoderGeneralStructure);
void decoderClearStructure(decoderStructure* decoderGeneralStructure);
void decoderShowInfo(decoderStructure* decoderGeneralStructure);
uint8_t decoderGetDecodingEffect(decoderStructure* decoderGeneralStructure);



