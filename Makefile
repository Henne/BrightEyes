CC=gcc
CFLAGS=-Wall -pedantic -g -Iinclude

all: pp20 rle dump_tga nvf2tga

pp20:	src/pp20.c include/packer.h
	$(CC) $(CFLAGS) -c -o build/pp20.o src/pp20.c
rle:	src/rle.c include/packer.h
	$(CC) $(CFLAGS) -c -o build/rle.o src/rle.c
dump_tga:	src/dump_tga.c include/dump.h
	$(CC) $(CFLAGS) -c -o build/dump_tga.o src/dump_tga.c


nvf2tga:	src/nvf2tga.c pp20 rle dump_tga include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o bin/nvf2tga src/nvf2tga.c build/pp20.o build/rle.o build/dump_tga.o

clean:
	rm -rf build/*.o
	rm -rf bin/[a-z]*
