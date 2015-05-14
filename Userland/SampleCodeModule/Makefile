include ../Makefile.inc

MODULE=sampleCodeModule.bin
SOURCES=$(wildcard *.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
