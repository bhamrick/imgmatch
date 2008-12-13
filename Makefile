CXXFLAGS=-I/usr/include/cairo
LDFLAGS=-L/usr/lib -lcairo

all: test match

test: test.cpp
	g++ $(CXXFLAGS) -c -o test.o test.cpp
	g++ $(LDFLAGS) -o test test.o
	rm test.o

match: match.cpp
	g++ $(CXXFLAGS) -c -o match.o match.cpp
	g++ $(LDFLAGS) -o match match.o
	rm match.o

clean:
	rm test
	rm match
