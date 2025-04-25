#!/bin/bash

# This script compiles a binary and makes sure the have not changed

# You need a Borland C++ 3.1 compiler installed in $DRIVE_C

DSTDIR=${PWD}/tools

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
cp ngen.c ngen.h ${DRIVE_C}/src
cp powerp20.asm powerp20.h ${DRIVE_C}/src
cp vgalib.asm vgalib.h ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src
cp cda_data.asm cda_data.h ${DRIVE_C}/src

cp hero.h ${DRIVE_C}/src
cp TLINK.RES ${DRIVE_C}/src

# copy c_ready.bat as compile.bat
cp build.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf bcc31.conf
popd

# move the EXE and MAP-file to DIR
rm -f ${DSTDIR}/NGEN.EXE ${DSTDIR}/NGEN.MAP
mv ${DRIVE_C}/src/*.EXE ${DSTDIR} 2>/dev/null
mv ${DRIVE_C}/src/*.MAP ${DSTDIR} 2>/dev/null

# cleanup
rm -rf ${DRIVE_C}/src/*

# VERIFICATION: Check if rewritten file exists

if [ -f ${DSTDIR}/NGEN.EXE ]; then
	echo "TEST PASSED"
	exit 0
else
	echo "ERROR: Build must have been failed"
	exit 1
fi
