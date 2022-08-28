
#include <stdio.h>
#include "pico/stdlib.h"

#include "circ_buffer.h"

int CircBufferPut(CIRC_BUFF* cf, char data)
{ 
    uint8_t head_temp = cf->head + 1;
	if ( head_temp == BUF_SIZE ) head_temp = 0;
	if ( head_temp == cf->tail ) return -1;
 
	cf->buffer[head_temp] = data;	
	cf->head = head_temp;			
 
	return 0;	// OK

}
int CircBufferGet(CIRC_BUFF* cf, char* data) 
{ 
	if (cf->head == cf->tail) return -1;
	cf->tail++; 
	if (cf->tail == BUF_SIZE) cf->tail = 0;
    *data = cf->buffer[cf->tail];		
 
	return 0;	// OK
}