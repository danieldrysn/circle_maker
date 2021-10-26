CC=g++
IDIR=/usr/local/include/opencv4
LDIR=/usr/local/lib
CFLAGS=-I$(IDIR) -I.
OPENCV=`pkg-config --cflags --libs opencv4`
LIBS=-L$(LDIR) $(OPENCV)
DEPS=circle_algortihms.h
OBJ=circle_maker.o circle_algorithms.o

%.o: %.cpp 
	$(CC) -c -o $@ $< $(CFLAGS)

circle_maker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean: 
	rm -f $(OBJ)

