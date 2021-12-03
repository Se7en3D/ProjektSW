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
#define DECODE_COMMAND_ON0 0x01
#define DECODE_COMMAND_ON1 0x02
#define DECODE_COMMAND_ON2 0x03
#define DECODE_COMMAND_ON3 0x04
#define DECODE_COMMAND_ON4 0x05
#define DECODE_COMMAND_ON5 0x06
#define DECODE_COMMAND_ON6 0x07
#define DECODE_COMMAND_ON7 0x08
#define DECODE_COMMAND_OFF0 0x09
#define DECODE_COMMAND_OFF1 0x0A
#define DECODE_COMMAND_OFF2 0x0B
#define DECODE_COMMAND_OFF3 0x0C
#define DECODE_COMMAND_OFF4 0x0D
#define DECODE_COMMAND_OFF5 0x0E
#define DECODE_COMMAND_OFF6 0x0F
#define DECODE_COMMAND_OFF7 0x10
#define DECODE_COMMAND_ONALL 0x11
#define DECODE_COMMAND_OFALL 0x12
#define DECODE_COMMAND_AT 0x13
#define DECODE_COMMAND_INVALID 0x00








  /**
   * @brief decoderArrayOfCommand tablica zawieraj�ca wszystkie komendy 
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
}decoderStructure;


void decoderInitStructure(decoderStructure* decoderGeneralStructure);
void vTaskAddNextCommandLetter(decoderStructure *decoderGeneralStructure,uint8_t* letter);
static void decoderSendAnswer(uint8_t what);
static void decoderClearStructure(decoderStructure* decoderGeneralStructure);
static void decoderShowInfo(decoderStructure* decoderGeneralStructure);
static uint8_t decoderGetDecodingEffect(decoderStructure* decoderGeneralStructure);



