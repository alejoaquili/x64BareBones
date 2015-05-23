#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <argp.h>

#include "modulePacker.h"

//Parser elements
const char *argp_program_version =
  "x64BareBones ModulePacker (C) v0.2";
const char *argp_program_bug_address =
  "arq-catedra@googlegroups.com";

/* Program documentation. */
static char doc[] =
  "ModulePacker is an appender of binary files to be loaded all together";

/* A description of the arguments we accept. */
static char args_doc[] = "KernelFile Module1 Module2 ...";

/* The options we understand. */
static struct argp_option options[] = {
  {"output",   'o', "FILE", 0,
   "Output to FILE instead of standard output" },
  { 0 }
};

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


int main(int argc, char *argv[]) {
	
	struct arguments arguments;

	arguments.output_file = OUTPUT_FILE;
	arguments.count = 0;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	array_t fileArray = {arguments.args, arguments.count};

	if(!checkFiles(fileArray)) {
		return 1;
	}	

	return !buildImage(fileArray, arguments.output_file);
}

int buildImage(array_t fileArray, char *output_file) {

	FILE *target;

	if((target = fopen(output_file, "w")) == NULL) {
		printf("Can't create target file\n");
		return FALSE;
	}

	//First, write the kernel
	FILE *source = fopen(fileArray.array[0], "r");
	write_file(target, source);

	//Write how many extra binaries we got.
	int extraBinaries = fileArray.length - 1;
	fwrite(&extraBinaries, sizeof(extraBinaries), 1, target);	
	fclose(source);

	int i;
	for (i = 1 ; i < fileArray.length ; i++) {
		FILE *source = fopen(fileArray.array[i], "r");
		
		//Write the file size;
		write_size(target, fileArray.array[i]);

		//Write the binary
		write_file(target, source);

		fclose(source);

	} 
	fclose(target);
	return TRUE;
}


int checkFiles(array_t fileArray) {

	int i = 0;
	for(; i < fileArray.length ; i++) {
		if(access(fileArray.array[i], R_OK)) {
			printf("Can't open file: %s\n", fileArray.array[i]);
			return FALSE;
		}
	}
	return TRUE;

}

int write_size(FILE *target, char *filename) {
	struct stat st;
	stat(filename, &st);
	uint32_t size = st.st_size;
	fwrite(&size, sizeof(uint32_t), 1, target);
}


int write_file(FILE *target, FILE *source) {
	char buffer[BUFFER_SIZE];
	int read;

	while (!feof(source)) {
		read = fread(buffer, 1, BUFFER_SIZE, source);
		fwrite(buffer, 1, read, target);
	}

	return TRUE;
}


/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'o':
      arguments->output_file = arg;
      break;

    case ARGP_KEY_ARG:
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        argp_usage (state);
      arguments->count = state->arg_num;
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}


