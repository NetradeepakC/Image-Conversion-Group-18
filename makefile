

CC = gcc

CFLAGS = -lm

DEPS = RAWtoIco.h TifftoRAW.h 

OBJ = RAWtoICOmake.o RAWtoIco.o TifftoRAW.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


TIFFtoICOmake: $(OBJ)
	$(CC) -o  $@ $^ $(CFLAGS)

all: TIFFtoICOmake
	chmod a+x TIFFtoICOmake


clean:
	rm -rf *o TIFFtoICOmake
