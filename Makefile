CXXFLAGS += -Wall -std=c++14

all: main

main: texobject.o texview.o stream.o stringbox.o reader.hpp

stringbox.o: optional.hpp

.PHONY: clean

clean:
	@rm -rf *.o main
