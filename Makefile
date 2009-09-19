CC=gcc
CFLAGS=-Wall -pedantic -g -Iinclude

all: packer dumper loader nvf2tga

packer: pp20 rle
dumper: dump_tga
loader: loader_nvf

pp20:	src/pp20.c include/packer.h
	$(CC) $(CFLAGS) -c -o build/pp20.o src/pp20.c
rle:	src/rle.c include/packer.h
	$(CC) $(CFLAGS) -c -o build/rle.o src/rle.c
dump_tga:	src/dump_tga.c include/dump.h
	$(CC) $(CFLAGS) -c -o build/dump_tga.o src/dump_tga.c
loader_nvf:	src/loader_nvf.c include/loader.h
	$(CC) $(CFLAGS) -c -o build/loader_nvf.o src/loader_nvf.c


nvf2tga:	src/nvf2tga.c packer dumper loader_nvf include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o bin/nvf2tga src/nvf2tga.c build/pp20.o build/rle.o build/dump_tga.o build/loader_nvf.o

clean:
	rm -rf build/*.o
	rm -rf bin/[a-z]*
