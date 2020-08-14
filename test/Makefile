CFLAGS=--std=c++11 -Wno-psabi -lDataTable 

all: dt

dt:
	g++ $(CFLAGS) -o dt DataTableTest.cpp

clean:
	rm -rf *.o stats linreg nn armadillo_test mr dt

