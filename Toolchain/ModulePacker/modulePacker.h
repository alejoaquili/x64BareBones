#ifndef _MODULE_PACKER_H_
#define _MODULE_PACKER_H_

#include <argp.h>


#define FALSE 0
#define TRUE !FALSE

#define BUFFER_SIZE 128

#define OUTPUT_FILE "packedKernel.bin"

#define MAX_FILES 128


typedef struct {
	char **array;
	int length;
} array_t;


/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[MAX_FILES];                
  int silent, verbose;
  char *output_file;
  int count;
};


int buildImage(array_t fileArray, char *output_file);

int write_size(FILE *target, char *filename);

int write_file(FILE *target, FILE *source);

int checkFiles(array_t fileArray);

static error_t
parse_opt (int key, char *arg, struct argp_state *state);


#endif
