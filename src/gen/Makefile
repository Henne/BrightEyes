CC=gcc
CFLAGS=-O0 -g -fno-asm

gen: cda_code.o ngen.o powerp20.o vgalib.o ail_stub.o
	$(CC) $(CFLAGS) -o ngen_$(CC) cda_code.o ngen.o powerp20.o vgalib.o ail_stub.o -lSDL2

clean:
	rm -f ngen_* *.o

cda_code.o: cda_code.c
	$(CC) $(CFLAGS) -c cda_code.c
	
ngen.o: ngen.c
	$(CC) $(CFLAGS) -c ngen.c

powerp20.o: powerp20.c
	$(CC) $(CFLAGS) -c powerp20.c

vgalib.o: vgalib.c
	$(CC) $(CFLAGS) -c vgalib.c

ail_stub.o: ail_stub.c
	$(CC) $(CFLAGS) -c ail_stub.c
