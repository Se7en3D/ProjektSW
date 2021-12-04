

typedef struct {
    uint8_t *buffer;
    int head;
    int tail;
    int maxlen;
} bufferStructure;


void circularBufferInitBuf(bufferStructure *structure,const uint32_t bufSize);
void circularBufferFreeBuf(bufferStructure *structure);
void circularBufferPush(bufferStructure *structure,const uint8_t data);
uint8_t circularBufferPop(bufferStructure *structure);
uint8_t *circularBufferGetPointerToTail(bufferStructure *structure);
uint32_t circularBufferGetNumberOfElements(bufferStructure *structure);
