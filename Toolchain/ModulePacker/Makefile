MP=mp.bin
SOURCES=$(wildcard *.cpp)

all: $(MP)

$(MP): $(SOURCES)
	g++ $(SOURCES) -o $(MP)

clean:
	rm -rf $(MP)

.PHONY: all clean
