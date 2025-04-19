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
for i in g105de_*.c; do
	#remove prefix from filenames
	cp ${i} ${DRIVE_C}/src/${i##g105de_}
	#remove prefix in c-files
	sed -i 's/g105de_//g' ${DRIVE_C}/src/${i##g105de_}
done
for i in g105de_*.h; do
	#remove prefix from filenames
	cp ${i} ${DRIVE_C}/src/${i##g105de_}
	#remove prefix in h -files
	sed -i 's/g105de_//g' ${DRIVE_C}/src/${i##g105de_}
done

for i in g105de_*.asm; do
	#remove prefix from filenames
	cp ${i} ${DRIVE_C}/src/${i##g105de_}
done

cp hero.h ${DRIVE_C}/src
cp TLINK.RES ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src

# copy c_ready.bat as compile.bat
cp build.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf bcc31.conf
popd

# move the EXE and MAP-file to DIR
mv ${DRIVE_C}/src/*.EXE ${DSTDIR} 2>/dev/null
mv ${DRIVE_C}/src/*.MAP ${DSTDIR} 2>/dev/null

# cleanup
rm -rf ${DRIVE_C}/src/*


# VERIFICATION
echo $DSTDIR

# Bytewise comparision of original and rewritten file
CHECK=$(cmp -l ${DSTDIR}/GEN.EXE ${DSTDIR}/NG_105DE.EXE | wc -l)

if [ ${CHECK} -gt 78 ]; then
	echo "ERROR: ${CHECK} bytes difference in the binaries (max. 78)"
	exit 1
elif [ ${CHECK} -eq 78 ]; then
	echo "TEST PASSED"
	exit 0
else
	echo "IMPROVEMENT: Now only ${CHECK} bytes difference"
	exit 0
fi
