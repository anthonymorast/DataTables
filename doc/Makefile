CFLAGS=-lDataTable -O2 --std=c++17
EXES=LoadingData WritingData ViewingData

all: $(EXES)

# filenames must be EXE.cpp for this to work
$(EXES):
	g++ $@.cpp $(CFLAGS) -o $@

clean:
	rm -rf $(EXES)
