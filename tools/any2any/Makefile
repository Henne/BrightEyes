#CC=i586-mingw32msvc-gcc
CC=gcc
#CFLAGS=-Wall -pedantic -g -Iinclude
CFLAGS=-g -std=c99 -Iinclude
LDFLAGS=-lgif
BUILDDIR=./build
BINDIR=./bin

PACKERS=pp20 rle rl
LOADERS=loader_nvf loader_ace loader_aif loader_raw loader_bob loader_gif loader_tga loader_uli

default: mkdirs $(BINDIR)/any2any
all: packer loader $(BINDIR)/any2any

packer: $(PACKERS:%=$(BUILDDIR)/%.o)
loader: $(LOADERS:%=$(BUILDDIR)/%.o)

$(BUILDDIR)/pp20.o:	src/pp20.c include/packer.h
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILDDIR)/rle.o:	src/rle.c include/packer.h
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILDDIR)/rl.o:	src/rl.c include/packer.h
	$(CC) $(CFLAGS) -c -o $@ $<
$(BUILDDIR)/format.o:   src/format.c include/format.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/loader_%.o: src/loader_%.c include/format.h
	$(CC) $(CFLAGS) -c -o $@ $<
$(BINDIR)/any2any: src/any2any.c packer loader $(BUILDDIR)/format.o
	$(CC) $(CFLAGS) -o $@ $< $(PACKERS:%=$(BUILDDIR)/%.o) $(LOADERS:%=$(BUILDDIR)/%.o) $(BUILDDIR)/format.o $(LDFLAGS)

clean:
	rm -rf $(BUILDDIR)/*.o
	rm -rf $(BINDIR)/[a-z]*

mkdirs:
	test -d $(BINDIR) || mkdir -p $(BINDIR)
	test -d $(BUILDDIR) || mkdir -p $(BUILDDIR)
