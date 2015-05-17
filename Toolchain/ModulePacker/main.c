#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "modulePacker.h"

int main(int argc, char *argv[]) {

	printf("x64BareBones Module Packer (C) v0.1\n");

	if(argc < 2) {
		printf("usage: \n");
		printf("	$> mp.bin <kernel binary> <user binary0> ... <user binaryN>");
		
		return 1;
	}

	array_t fileArray = {&argv[1], argc -1};

	if(!checkFiles(fileArray)) {
		return 1;
	}	


	return !buildImage(fileArray);

}

int buildImage(array_t fileArray) {

	FILE *target;

	if((target = fopen(OUTPUT_FILE, "w")) == NULL) {
		LOG("Can't create target file\n", "");
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
			LOG("Can't open file: %s\n", fileArray.array[i]);
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