CFLAGS=--std=c++11 -Wno-psabi -lDataTable -larmadillo -lcurl

Test: all
	./StringUtilTest
	./ShapeTypeTest
	./DataTableHeadersTest
#	./FileReadTest
	./DataSelectionTest
	./DataTableEcoTest

all: StringUtilTest \
	ShapeTypeTest \
	DataSelectionTest \
	DataTableHeadersTest \
	DataTableEcoTest \
	# FileReadTest
	# DataSelectionTest

DataTableHeadersTest: DataTableHeadersTest.o
	g++ $(CFLAGS) -o DataTableHeadersTest $<

StringUtilTest: StringUtilTest.o
	g++ $(CFLAGS) -o StringUtilTest $<

ShapeTypeTest: ShapeTypeTest.o
	g++ $(CFLAGS) -o ShapeTypeTest $<

# FileReadTest: FileReadTest.o
# 	g++ $(CFLAGS) -o FileReadTest $<

DataTableEcoTest: DataTableEcoTest.o
	g++ $(CFLAGS) -o DataTableEcoTest $<

DataSelectionTest: DataSelectionTest.o
	g++ $(CFLAGS) -o DataSelectionTest $<

clean:
	rm -rf *.o dttest ShapeTypeTest StringUtilTest DataSelectionTest DataTableEcoTest DataTableHeadersTest
