#CC=i586-mingw32msvc-gcc
CC=gcc
#CFLAGS=-Wall -pedantic -g -Iinclude
CFLAGS=-g -std=c99 -Iinclude
LDFLAGS=-lgif
BUILDDIR=./build
BINDIR=./bin

LOADERS=loader_nvf loader_ace loader_aif loader_raw loader_bob loader_gif loader_tga loader_uli
#LOADERS=loader_ace loader_nvf loader_gif

default: any2any
all: packer loader any2any

packer: pp20 rle rl
loader: $(LOADERS)

pp20:	src/pp20.c include/packer.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/pp20.o src/pp20.c
rle:	src/rle.c include/packer.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/rle.o src/rle.c
rl:	src/rl.c include/packer.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/rl.o src/rl.c

loader_%: src/loader_%.c
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/$@.o $<
format.o: src/format.c include/format.h
	$(CC) $(CFLAGS) -c -o $(BUILDDIR)/$@ $<
any2any: mkdirs src/any2any.c packer $(LOADERS) format.o include/packer.h 
	$(CC) $(CFLAGS) -o $(BINDIR)/any2any src/any2any.c $(BUILDDIR)/format.o $(BUILDDIR)/pp20.o $(BUILDDIR)/rle.o $(BUILDDIR)/rl.o $(LOADERS:%=$(BUILDDIR)/%.o) $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)/*.o
	rm -rf $(BINDIR)/[a-z]*


mkdirs:	| bin build
bin:
	mkdir -p bin
build:
	mkdir -p build
