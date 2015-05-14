#include <stdint.h>
#include <string.h>
#include "lib.h"

extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel		//End of kernel
		+ PageSize					//Dead zone used to page fault if there is an stack overflow
		+ PageSize * 4				//The size of the stack itself
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main() {
	((char*)(0xB8000))[0] = 'X';
	return 0;
}
