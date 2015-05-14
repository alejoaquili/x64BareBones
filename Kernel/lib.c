#include <stdint.h>

void * memset(void * destiny, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiny;

	while(length--)
		dst[length] = chr;

	return destiny;
}
