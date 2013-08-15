#CC=i586-mingw32msvc-gcc
CC=gcc
#CFLAGS=-Wall -pedantic -g -Iinclude
CFLAGS=-g -Iinclude
LDFLAGS=-lgd
BUILDDIR=./build
BINDIR=./bin

all: packer dumper loader nvf2tga ace2tga aif2tga raw2tga nvf2gif

packer: pp20 rle rl
dumper: dump_tga dump_gif
loader: loader_nvf loader_nvf2 loader_ace loader_aif

pp20:	src/pp20.c include/packer.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/pp20.o src/pp20.c
rle:	src/rle.c include/packer.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/rle.o src/rle.c
rl:	src/rl.c include/packer.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/rl.o src/rl.c
dump_tga:	src/dump_tga.c include/dump.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/dump_tga.o src/dump_tga.c
dump_gif:	src/dump_gif.c include/dump.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/dump_gif.o src/dump_gif.c
loader_nvf:	src/loader_nvf.c include/loader.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/loader_nvf.o src/loader_nvf.c
loader_nvf2:	src/loader_nvf2.c include/loader.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/loader_nvf2.o src/loader_nvf2.c
loader_ace:	src/loader_ace.c include/loader.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/loader_ace.o src/loader_ace.c
loader_aif:	src/loader_aif.c include/loader.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/loader_aif.o src/loader_aif.c
loader_raw:	src/loader_raw.c include/loader.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/loader_raw.o src/loader_raw.c


nvf2tga:	src/nvf2tga.c packer dumper loader_nvf include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o $(BINDIR)/nvf2tga src/nvf2tga.c $(BUILDDIR)/pp20.o $(BUILDDIR)/rle.o $(BUILDDIR)/dump_tga.o $(BUILDDIR)/loader_nvf.o
nvf2gif:	src/nvf2tga.c packer dumper loader_nvf2 include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o $(BINDIR)/nvf2gif src/nvf2gif.c $(BUILDDIR)/pp20.o $(BUILDDIR)/rle.o $(BUILDDIR)/dump_gif.o $(BUILDDIR)/loader_nvf2.o $(LDFLAGS)
ace2tga:	src/ace2tga.c packer dumper loader_ace include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o $(BINDIR)/ace2tga src/ace2tga.c $(BUILDDIR)/pp20.o $(BUILDDIR)/rle.o $(BUILDDIR)/dump_tga.o $(BUILDDIR)/loader_ace.o $(BUILDDIR)/rl.o

aif2tga:	src/aif2tga.c packer dumper loader_aif include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o $(BINDIR)/aif2tga src/aif2tga.c $(BUILDDIR)/pp20.o $(BUILDDIR)/rle.o $(BUILDDIR)/rl.o $(BUILDDIR)/dump_tga.o $(BUILDDIR)/loader_aif.o

raw2tga:	src/raw2tga.c packer dumper loader_raw include/packer.h include/dump.h
	$(CC) $(CFLAGS) -o $(BINDIR)/raw2tga src/raw2tga.c $(BUILDDIR)/pp20.o $(BUILDDIR)/rle.o $(BUILDDIR)/dump_tga.o $(BUILDDIR)/loader_raw.o

clean:
	rm -rf $(BUILDDIR)/*.o
	rm -rf $(BINDIR)/[a-z]*
