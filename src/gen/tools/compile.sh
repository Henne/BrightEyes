#!/bin/bash

# This script compiles the files from compile.bat

# You need a Borland C++ 3.1 compiler installed in $DRIVE_C

DIR=${PWD}/TEMP

DRIVE_C=${PWD}/../../drive_c

# check all tools are available
for prog in "dosbox"; do
	#echo "checking for $prog"
	$prog --version >/dev/null 2>/dev/null

	if [ $? -ne 0 ]; then
		echo "Problems with $prog => install it"
		exit 1
	fi
done
# all tools are available

# copy all source files to DRIVE_C
cp cda_code.c cda_code.h ${DRIVE_C}/src
cp ngen.c ${DRIVE_C}/src
cp powerp20.asm powerp20.h ${DRIVE_C}/src
cp powerp20.c ${DRIVE_C}/src
cp vgalib.c vgalib.h ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src

cp hero.h ${DRIVE_C}/src
cp TLINK.RES ${DRIVE_C}/src

# copy and use compile.bat
cp compile.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf bcc31.conf
popd

# cleanup
rm -rf ${DRIVE_C}/src/*
