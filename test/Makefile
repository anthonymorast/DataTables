CFLAGS=--std=c++11 -Wno-psabi -lDataTable -larmadillo -lcurl

all: dt

dt:
	g++ -o dt DataTableTest.cpp $(CFLAGS) 

clean:
	rm -rf *.o dt
