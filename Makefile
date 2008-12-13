CXXFLAGS=-I/usr/include/cairo
LDFLAGS=-L/usr/lib -lcairo

all: test

test: test.cpp
	g++ $(CXXFLAGS) -c -o test.o test.cpp
	g++ $(LDFLAGS) -o test test.o
	rm test.o

clean:
	rm test
