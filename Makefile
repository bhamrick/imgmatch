CXXFLAGS=-I/usr/include/cairo -fopenmp -O2 -I. -g
LDFLAGS=-L/usr/lib -lcairo -fopenmp -O2 -g

all: test match imaging_test

test: test.cpp
	g++ $(CXXFLAGS) -c -o test.o test.cpp
	g++ $(LDFLAGS) -o test test.o
	rm test.o

match: match.cpp
	g++ $(CXXFLAGS) -c -o match.o match.cpp
	g++ $(LDFLAGS) -o match match.o
	rm match.o

imaging_test: imaging_test.cpp imaging.cpp
	g++ $(CXXFLAGS) -c -o imaging_test.o imaging_test.cpp
	g++ $(CXXFLAGS) -c -o imaging.o imaging.cpp
	g++ $(LDFLAGS) -o imaging_test imaging_test.o imaging.o
	rm imaging_test.o imaging.o

clean:
	rm test
	rm match
	rm imaging_test
