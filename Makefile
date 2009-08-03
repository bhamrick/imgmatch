CXXFLAGS=-O2 -I. -g
LDFLAGS=-O2 -g

all: match drawing drawing_test

match: drawing match.cpp
	g++ $(CXXFLAGS) -c -o match.o match.cpp
	g++ $(LDFLAGS) -o match match.o drawing.o

drawing: drawing.cpp
	g++ $(CXXFLAGS) -c -o drawing.o drawing.cpp

drawing_test: drawing drawing_test.cpp
	g++ $(CXXFLAGS) -c -o drawing_test.o drawing_test.cpp
	g++ -o drawing_test drawing_test.o drawing.o

clean:
	rm drawing_test
	rm match
	rm *.o
