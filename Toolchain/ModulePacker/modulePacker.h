#ifndef _MODULE_PACKER_H_
#define _MODULE_PACKER_H_

#define FALSE 0
#define TRUE !FALSE

#define BUFFER_SIZE 128

#define OUTPUT_FILE "packedKernel.bin"


typedef struct {
	char **array;
	int length;
} array_t;




int write_size(FILE *target, char *filename);

int write_file(FILE *target, FILE *source);

int checkFiles(array_t fileArray);


#endif
