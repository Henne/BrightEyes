pp20:	src/pp20.c include/packer.h
	gcc -g -c -o build/pp20.o src/pp20.c
rle:	src/rle.c include/packer.h
	gcc -g -c -o build/rle.o src/rle.c


nvf2tga:	src/nvf2tga.c pp20 rle include/packer.h
	gcc -g -o bin/nvf2tga -Iinclude src/nvf2tga.c build/pp20.o build/rle.o

clean:
	rm -rf build/*.o
	rm -rf bin/[a-z]*
