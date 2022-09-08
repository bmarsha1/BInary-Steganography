CXX=g++
CXXFLAGS=-Wall -g

all: conceal extract

conceal: conceal.o BinarySteganography.o
	$(CXX) $(CXXFLAGS) conceal.o BinarySteganography.o -o conceal

extract: extract.o BinarySteganography.o
	$(CXX) $(CXXFLAGS) extract.o BinarySteganography.o -o extract

extract.o: extract.cpp BinarySteganography.h
	$(CXX) $(CXXFLAGS) -c extract.cpp

conceal.o: conceal.cpp BinarySteganography.h 
	$(CXX) $(CXXFLAGS) -c conceal.cpp

BinarySteganography.o: BinarySteganography.cpp BinarySteganography.h BinarySteganographyKey.h BinarySteganographyInstruction.h
	$(CXX) $(CXXFLAGS) -c BinarySteganography.cpp

clean:
	rm -rf conceal extract *.o *.dSYM

