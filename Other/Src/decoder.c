#include <stdio.h>
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include "decoder.h"


void decoderInitStructure(decoderStructure* decoderGeneralStructure) {
	uint8_t sizeTableOdRejectedCommands = (sizeof(decoderArrayOfCommand) /sizeof(decoderArrayOfCommand[0])/sizeof(uint8_t));
	if (decoderGeneralStructure->tableOfRejectedCommands !=0) {
		decoderSendAnswer(DECODE_ANSWER_INIT_MALLOC_ERROR);
		return;
	}
	decoderGeneralStructure->tableOfRejectedCommands =(uint8_t*) malloc(sizeTableOdRejectedCommands);
	decoderGeneralStructure->sizeTableOdRejectedCommands = sizeTableOdRejectedCommands;
	decoderClearStructure(decoderGeneralStructure);
}

void decoderAddNextSign(decoderStructure *decoderGeneralStructure, uint8_t* letter) {
	uint8_t positionOfDecodeCommand = decoderGeneralStructure->positionOfDecodeCommand;
	uint8_t sign = *letter;

	if (positionOfDecodeCommand >= DECODER_MAX_LENGTH_COMMAND) {
		if (sign == '\n') {
			decoderSendAnswer(DECODE_ANSWER_ERROR);
			decoderClearStructure(decoderGeneralStructure);
			printf("ERR\n");
			return;
		}
		else {
			return;
		}
	}

	for (int i = 0; i < DECODER_MAX_LENGTH_COMMAND; i++) {
		if (decoderGeneralStructure->tableOfRejectedCommands[i]) {
			if (decoderArrayOfCommand[i][positionOfDecodeCommand] == 'x') {
				continue;
			}

			if (decoderArrayOfCommand[i][positionOfDecodeCommand] != sign) {
				decoderGeneralStructure->tableOfRejectedCommands[i] = DECODE_REJECT_COMMAND_OFF;
			}
		}
	}
	decoderGeneralStructure->command[positionOfDecodeCommand] = sign;
	if (sign == '\n') {
		uint8_t dataReturn=decoderGetDecodingEffect(decoderGeneralStructure);
		if(dataReturn==COMMAND_INVALID){
			printf("ERR\n");
		}else{
			printf("OK.\n");
			decoderGeneralStructure->isEndDecode=1;
			decoderGeneralStructure->decodeValue=dataReturn;
		}
		decoderClearStructure(decoderGeneralStructure);
		return;
	}
	positionOfDecodeCommand++;
	decoderGeneralStructure->positionOfDecodeCommand=positionOfDecodeCommand;

}

void decoderSendAnswer(uint8_t what) {
	switch (what) {
	case DECODE_ANSWER_ERROR:
		break;
		printf("Nie rozpoznano komendy\n");
	case DECODE_ANSWER_OK:
		printf("Komenda poprawnie rozpoznana\n");
		break;
	case DECODE_ANSWER_INIT_MALLOC_ERROR:
		printf("Nie mozna wykonac malloc na wys�anej strukturze\n");
		break;
	case DECODE_CLEAR_STRUCT_RESET_REJECTED_TABLE:
		printf("Nie mozna zresetowac blokady komend\n");
		break;
	default:
		printf("Nie rozpoznano kodu");
		break;
	};
}

uint8_t decoderIsEndDecode(decoderStructure *decoderGeneralStructure){
	return decoderGeneralStructure->isEndDecode;
}
uint8_t decoderGetDecodeValue(decoderStructure *decoderGeneralStructure){
	decoderGeneralStructure->isEndDecode=0;
	return decoderGeneralStructure->decodeValue;

}

void decoderClearStructure(decoderStructure* decoderGeneralStructure) {
	uint8_t sizeTableOdRejectedCommands = decoderGeneralStructure->sizeTableOdRejectedCommands;
	
	if (decoderGeneralStructure->tableOfRejectedCommands == 0) {
		decoderSendAnswer(DECODE_CLEAR_STRUCT_RESET_REJECTED_TABLE);
		return;

	}
		//Reset komendy
	for (int i = 0; i < DECODER_MAX_LENGTH_COMMAND; i++) {
		decoderGeneralStructure->command[i] = 0;
	}
		//Odblokowanie odrzuconych komend
	for (int i = 0; i < sizeTableOdRejectedCommands; i++) {
		decoderGeneralStructure->tableOfRejectedCommands[i] = DECODE_REJECT_COMMAND_ON;
	}

	decoderGeneralStructure->positionOfDecodeCommand = 0;

}

void decoderShowInfo(decoderStructure* decoderGeneralStructure) {
	uint8_t sizeTableOdRejectedCommands = decoderGeneralStructure->sizeTableOdRejectedCommands;
	printf("tableOfRejectedCommands:\n");
	for (int i = 0; i < sizeTableOdRejectedCommands; i++) {
		printf("	tableOfRejectedCommands[%d]=%d\n", i, decoderGeneralStructure->tableOfRejectedCommands[i]);
	}
	printf("command:\n");

	for (int i = 0; i < DECODER_MAX_LENGTH_COMMAND; i++) {
		printf("	command[%d]=%c\n",(int) i,(int)decoderGeneralStructure->command[i]);
	}

	printf("positionOfDecodeCommand=%d\n", decoderGeneralStructure->positionOfDecodeCommand);

}

uint8_t decoderGetDecodingEffect(decoderStructure* decoderGeneralStructure) {
	uint32_t countRejectCommands = 0;
	uint8_t sizeTableOdRejectedCommands = decoderGeneralStructure->sizeTableOdRejectedCommands;
	for (int i = 0; i < sizeTableOdRejectedCommands; i++) {
		uint8_t statusReject = decoderGeneralStructure->tableOfRejectedCommands[i];
		countRejectCommands += (2 * (i+1)*statusReject);
	}
	//printf("countRejectCommands=%d	", (int)countRejectCommands);

	
	switch (countRejectCommands) {
	case 2: //Komenda ONx
		switch (decoderGeneralStructure->command[2]) {
		case '0':
			return COMMAND_ON0;
			break;
		case '1':
			return COMMAND_ON1;
			break;
		case '2':
			return COMMAND_ON2;
			break;
		case '3':
			return COMMAND_ON3;
			break;
		case '4':
			return COMMAND_ON4;
			break;
		case '5':
			return COMMAND_ON5;
			break;
		case '6':
			return COMMAND_ON6;
			break;
		case '7':
			return COMMAND_ON7;
			break;
		default:
			return COMMAND_INVALID;
			break;
		}
		break;
	case 4: //Komenda OFFx
		switch (decoderGeneralStructure->command[3]) {
		case '0':
			return COMMAND_OFF0;
			break;
		case '1':
			return COMMAND_OFF1;
			break;
		case '2':
			return COMMAND_OFF2;
			break;
		case '3':
			return COMMAND_OFF3;
			break;
		case '4':
			return COMMAND_OFF4;
			break;
		case '5':
			return COMMAND_OFF5;
			break;
		case '6':
			return COMMAND_OFF6;
			break;
		case '7':
			return COMMAND_OFF7;
			break;
		default:
			return COMMAND_INVALID;
			break;
		}
		break;
	case 6: //Komenda ONALL
		return COMMAND_ONALL;
		break;
	case 8: //Komenda OFFALL
		return COMMAND_OFALL;
		break;
	case 10: //Komenda AT
		return COMMAND_AT;
		break;
	default: //Nie rozpoznano komendy
		return COMMAND_INVALID;
		break;

	}

}


