include Makefile.inc

all: sampleCodeModule sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf "This is sample data." >> SampleDataModule.bin && dd if=/dev/zero bs=1 count=1 >> SampleDataModule.bin

clean:
	cd SampleCodeModule; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean
