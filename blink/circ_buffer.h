
#define BUF_SIZE 128

typedef struct {
    uint8_t head;
    uint8_t tail;
    char buffer[BUF_SIZE];
} CIRC_BUFF;

int CircBufferPut(CIRC_BUFF *cf, char data);
int CircBufferGet(CIRC_BUFF *cf, char* data);
