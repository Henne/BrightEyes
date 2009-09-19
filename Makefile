#CC=i586-mingw32msvc-gcc
CC=gcc
CFLAGS=-Wall -pedantic -g -Iinclude

all: packer dumper loader nvf2tga ace2tga

packer: pp20 rle
dumper: dump_tga
loader: loader_nvf loader_ace

pp20:	src/pp20.c include/packer.h
	$(CC) $(CFLAGS) -c -o build/pp20.o src/pp20.c
rle:	src/rle.c include/packer.h
	$(CC) $(CFLAGS) -c -o build/rle.o src/rle.c
dump_tga:	src/dump_tga.c include/dump.h
	$(CC) $(CFLAGS) -c -o build/dump_tga.o src/dump_tga.c
loader_nvf:	src/loader_nvf.c include/loader.h
	$(CC) $(CFLAGS) -c -o build/loader_nvf.o src/loader_nvf.c
loader_ace:	src/loader_ace.c include/loader.h
	$(CC) $(CFLAGS) -c -o build/loader_ace.o src/loader_ace.c


nvf2tga:	src/nvf2tga.c packer dumper loader_nvf include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o bin/nvf2tga src/nvf2tga.c build/pp20.o build/rle.o build/dump_tga.o build/loader_nvf.o

ace2tga:	src/ace2tga.c packer dumper loader_nvf include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o bin/ace2tga src/ace2tga.c build/pp20.o build/rle.o build/dump_tga.o build/loader_ace.o

clean:
	rm -rf build/*.o
	rm -rf bin/[a-z]*
