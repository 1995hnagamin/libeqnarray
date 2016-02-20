CXXFLAGS += -Wall -std=c++14

all: main

main: texobject.o texview.o stream.o stringbox.o

.PHONY: clean

clean:
	@rm -rf *.o main
