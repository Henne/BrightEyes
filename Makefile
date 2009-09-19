nvf2tga:	src/nvf2tga.c src/pp20.c src/rle.c include/packer.h
	gcc -g -c -o build/pp20.o src/pp20.c
	gcc -g -c -o build/rle.o src/rle.c
	gcc -g -o bin/nvf2tga -Iinclude src/nvf2tga.c build/pp20.o build/rle.o
