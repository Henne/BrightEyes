CC=clang
CFLAGS=-O0 -g -fno-asm

gen: cda_code.o gen105de.o random.o powerp20.o vgalib.o ail_stub.o
	$(CC) $(CFLAGS) -o g105de_$(CC) cda_code.o gen105de.o random.o powerp20.o vgalib.o ail_stub.o -lSDL2

clean:
	rm -f g105de_[gc]* *.o

cda_code.o: cda_code.c
	$(CC) $(CFLAGS) -c cda_code.c
	
gen105de.o: gen105de.c
	$(CC) $(CFLAGS) -c gen105de.c

random.o: random.c
	$(CC) $(CFLAGS) -c random.c

powerp20.o: powerp20.c
	$(CC) $(CFLAGS) -c powerp20.c

vgalib.o: vgalib.c
	$(CC) $(CFLAGS) -c vgalib.c

ail_stub.o: ail_stub.c
	$(CC) $(CFLAGS) -c ail_stub.c
