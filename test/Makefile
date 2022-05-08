CFLAGS=--std=c++11 -Wno-psabi -lDataTable -larmadillo -lcurl

Test: all 
	./StringUtilTest
	./ShapeTypeTest
#	./FileReadTest
	./DataSelectionTest


all: StringUtilTest ShapeTypeTest DataSelectionTest #FileReadTest DataSelectionTest

StringUtilTest: StringUtilTest.o
	g++ $(CFLAGS) -o StringUtilTest $<

ShapeTypeTest: ShapeTypeTest.o
	g++ $(CFLAGS) -o ShapeTypeTest $<

# FileReadTest: FileReadTest.o
# 	g++ $(CFLAGS) -o FileReadTest $< 

DataSelectionTest: DataSelectionTest.o
	g++ $(CFLAGS) -o DataSelectionTest $<

clean:
	rm -rf *.o dttest ShapeTypeTest StringUtilTest DataSelectionTest
