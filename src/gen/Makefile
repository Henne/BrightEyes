CC='clang'

gen: g105de_seg001.o g105de_seg002.o g105de_seg003.o g105de_seg004.o g105de_seg005.o g105de_seg006.o
	$(CC) -o g105de_$(CC) g105de_seg001.o g105de_seg002.o g105de_seg003.o g105de_seg004.o g105de_seg005.o g105de_seg006.o

clean: rm g105de_$(CC) g105de_seg00*.o

g105de_seg001.o: g105de_seg001.c
	$(CC) -c g105de_seg001.c
	
g105de_seg002.o: g105de_seg002.c
	$(CC) -c g105de_seg002.c

g105de_seg003.o: g105de_seg003.c
	$(CC) -c g105de_seg003.c

g105de_seg004.o: g105de_seg004.c
	$(CC) -c g105de_seg004.c

g105de_seg005.o: g105de_seg005.c
	$(CC) -c g105de_seg005.c

g105de_seg006.o: g105de_seg006.c
	$(CC) -c g105de_seg006.c
