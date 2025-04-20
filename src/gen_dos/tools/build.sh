#!/bin/bash

# This script compiles a binary and makes sure the have not changed

# You need a Borland C++ 3.1 compiler installed in $DRIVE_C

DSTDIR=${PWD}/build
ORIGDIR=${PWD}/orig

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
cp gen105de.c gen.h ${DRIVE_C}/src
cp random.c random.h ${DRIVE_C}/src
cp powerp20.asm powerp20.h ${DRIVE_C}/src
cp vgalib.asm vgalib.h ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src
cp cda_data.asm cda_data.h ${DRIVE_C}/src
cp hero.h ${DRIVE_C}/src

cp gen300en.c ${DRIVE_C}/src

cp GEN*.RES ${DRIVE_C}/src

# copy c_ready.bat as compile.bat
cp build.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf bcc31.conf
popd

# move the EXE and MAP-file to DIR
rm -f ${DSTDIR}/*.EXE ${DSTDIR}/*.MAP
mv ${DRIVE_C}/src/*.EXE ${DSTDIR} 2>/dev/null
mv ${DRIVE_C}/src/*.MAP ${DSTDIR} 2>/dev/null

# cleanup
rm -rf ${DRIVE_C}/src/*


# VERIFICATION
echo $DSTDIR

# Bytewise comparision of original and rewritten file
CHECK=$(cmp -l ${ORIGDIR}/GEN105DE.EXE ${DSTDIR}/GEN105DE.EXE | wc -l)

if [ ${CHECK} -gt 13 ]; then
	echo "ERROR: ${CHECK} bytes difference in the binaries (max. 13)"
	exit 1
elif [ ${CHECK} -eq 13 ]; then
	echo "TEST PASSED"
	exit 0
else
	echo "IMPROVEMENT: Now only ${CHECK} bytes difference"
	exit 0
fi
