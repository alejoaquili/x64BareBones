#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <list>

using namespace std;

struct Parameters {
public:
	char * output;
	list<char*> input;

	Parameters(int argc, char ** argv);
};

list<ifstream*> fileNamesToFiles(list<char*> fileNames) {
	list<ifstream*> result;

	list<char*>::iterator i = fileNames.begin();
	list<char*>::iterator end = fileNames.end();

	for (; i != end; i++) {
		ifstream * file = new ifstream(*i, ios_base::binary | ios::ate);
		result.push_back(file);
	};

	return result;
}

bool checkFiles(list<ifstream*> files, list<char*> names) {
	bool result;

	list<ifstream*>::iterator i = files.begin();
	list<ifstream*>::iterator end = files.end();
	list<char*>::iterator j = names.begin();
	
	for (; i != end; i++, j++)
		if (!(*i)->is_open()) {
			cerr << "Couldn't open file " << *j << endl;
			return false;
		}

	return true;
}

int buildImage(list<ifstream*> files, char * output) {
	ofstream result(output, ios_base::binary);
	
	if (!result.is_open()) {
		cerr << "Couldn't open file " << output << endl;
		return 1;
	}

	ifstream * kernel = *(files.begin());
	list<ifstream*>::iterator i = ++(files.begin());
	list<ifstream*>::iterator end = files.end();

	//The kernel followed by the number of binaries that will follow the kernel
	kernel->seekg(0);
	int count = files.size() - 1;
	result << kernel->rdbuf();
	result.write((char*)&count, sizeof(count)); 

	for (; i != end; i++) {
		//Size of the binary
		int size = (*i)->tellg();
		result.write((char*)&size, sizeof(size)); 
		(*i)->seekg(0);
 		
 		//The binary itself
 		result << (*i)->rdbuf();

 		//Cleanup
 		(*i)->close();
		delete *i;
	}

	//Cleanup
	result.close();

	return 0;
}

int main(int argc, char ** argv) {
	Parameters parameters(argc, argv);

	if (parameters.input.size() < 1) {
		cout << "Wyrm Kernel Packer v0.1" << endl;
		cout << "usage: <kernel binary> <user binary0> ... <user binaryN> [-o output]" << endl;
		return 1;
	}

	list<ifstream*> files = fileNamesToFiles(parameters.input);
	if (!checkFiles(files, parameters.input)) {
		return 1;
	}

	return buildImage(files, parameters.output);
}

Parameters::Parameters(int argc, char ** argv) {
	output = (char*)"InitialImage.bin";

	for(int i = 1; i < argc; i++) {
		bool out = (strcmp(argv[i], "-o") == 0);

		if (out && i + 1 < argc) {
			output = argv[++i];
		} else if (!out){
			input.push_back(argv[i]);
		}
	}
}
