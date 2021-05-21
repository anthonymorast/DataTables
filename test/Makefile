CFLAGS=--std=c++11 -Wno-psabi -lDataTable -larmadillo -lcurl 

Test: all 
	./FileReadTest
	./DataSelectionTest


all: FileReadTest DataSelectionTest

FileReadTest: FileReadTest.o
	g++ $(CFLAGS) -o FileReadTest $< 

DataSelectionTest: DataSelectionTest.o
	g++ $(CFLAGS) -o DataSelectionTest $<

clean:
	rm -rf *.o
