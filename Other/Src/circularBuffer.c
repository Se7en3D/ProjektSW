/*
 * circularBuffer.c
 *
 *  Created on: 4 gru 2021
 *      Author: Daniel
 */

#include <stdio.h>
#include <stdlib.h>
#include "circularBuffer.h"

void circularBufferInitBuf(bufferStructure *structure,const uint32_t bufSize){
	if(structure->buffer==0){
		free(structure->buffer);
	}
	structure->head=0;
	structure->tail=0;
	structure->buffer=(uint8_t) malloc(bufSize*sizeof(uint8_t));
	structure->maxlen=bufSize;
}

void circularBufferFreeBuf(bufferStructure *structure){
	free(structure->buffer);
	structure->head=0;
	structure->tail=0;
	structure->maxlen=0;
}

void circularBufferPush(bufferStructure *structure,const uint8_t data){
	int nextPosition;
	nextPosition = structure->head + 1;
	if (nextPosition >= (structure->maxlen)){
			nextPosition = 0;
	}

	if (nextPosition == structure->tail){
		return ;
	}

	structure->buffer[structure->head] = data;
	structure->head = nextPosition;
}

uint8_t circularBufferPop(bufferStructure *structure){
	uint8_t data;
	int next;
	if (structure->head == structure->tail){
		return 0;
	}
	next = structure->tail + 1;
	if (next >= structure->maxlen)
		next = 0;

	data = structure->buffer[structure->tail];
	structure->tail = next;
	return data;
}

uint8_t *circularBufferGetPointerToTail(bufferStructure *structure){
	return &structure->buffer[structure->tail];
}

uint32_t circularBufferGetNumberOfElements(bufferStructure *structure){
	if(structure->head==structure->tail){
		return 0;
	}

	if(structure->head<structure->tail){
		uint32_t length=structure->maxlen-structure->tail;
		structure->tail=0;
		return length;
	}else{
		uint32_t length=structure->head-structure->tail;
		structure->tail=structure->head;
		return length;
	}
}
