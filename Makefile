all: main

.PHONY: clean release debug

CC = $(CXX)
CXXFLAGS  = -std=c++17 -Wall -Wextra -fpermissive

release: CXXFLAGS += -O2
release: main

debug: CXXFLAGS += -g -O0
debug: main

main: main.o list.o

main.o: main.cpp tree.h list.h

list.o: list.cpp list.h

clean:
	rm -f *.o