CXXFLAGS=-DSDL_MAIN_HANDLED
LDFLAGS=-static -s -lSDL2 -limm32 -lgdi32 -liconv -lwinmm -lversion -lole32 -loleaut32

all: openday

openday: util.o main.o
	c++ -o$@ $^ $(LDFLAGS)

%.o: src/%.cpp
	c++ -c $(CXXFLAGS) -o$@ $<

clean:
	rm -f openday
	rm -f *.o
