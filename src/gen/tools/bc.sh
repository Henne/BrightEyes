#!/bin/bash

# This script shows a diff between two disassembly files, to find differences

# You need a Borland C++ compiler installed in $DRIVE_C

DIR=${PWD}/temp
OBJDIR=${DIR}/OBJ
BINDIR=${DIR}/BIN
DISDIR=${DIR}/disasm

DISORIG=${DIR}/disasm_orig

DRIVE_C=${PWD}/../../drive_c

# remove all directories for a clean build
rm -rf $OBJDIR $BINDIR $DISDIR
mkdir -p $OBJDIR $BINDIR $DISDIR

# check all tools are available
for prog in "head diff dosbox less"; do
	#echo "checking for $prog"
	$prog --version >/dev/null 2>/dev/null

	if [ $? -ne 0 ]; then
		echo "Problems with $prog => install it"
		exit 1
	fi
done

ndisasm >/dev/null 2>/dev/null
if [ $? -ne 0 ]; then
	echo "Problems with ndisasm => install it"
	exit 1
fi

../tools/dump_obj >/dev/null 2>/dev/null
if [ $? -ne 255 ]; then
	echo "Problems with dump_obj => Recompile Bright-Eyes"
	exit 1
fi
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
cp compile.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf bcc31.conf
popd

# cleanup
rm -rf ${DRIVE_C}/src/*.c
rm -rf ${DRIVE_C}/src/*.h
rm -rf ${DRIVE_C}/src/*.asm
rm -rf ${DRIVE_C}/src/*.bat
rm -rf ${DRIVE_C}/src/TLINK.RES
rm -rf ${DRIVE_C}/src/AIL

# move all OBJ-files to OBJDIR
mv ${DRIVE_C}/src/*.OBJ $OBJDIR 2>/dev/null

if [ $? -ne 0 ]; then
	echo "No OBJ-files created => check compilation windows for errors or compile somthing"
	exit 1
fi

# disassemble all *.OBJ files and show the diffs
for i in ${OBJDIR}/*.OBJ; do

	# extract the filename
	PREFIX=${i%\.OBJ}
	PREFIX=${PREFIX##*/}

	echo "$i -- $PREFIX"
	# extract instructions
	../tools/dump_obj $i >/dev/null
	# move the BIN-files to BINDIR
        mv ${OBJDIR}/${PREFIX}.BIN $BINDIR

	# diassemble BIN-file into DISDIR
        ndisasm -b16 ${BINDIR}/${PREFIX}.BIN >${DISDIR}/${PREFIX}.dis
#       ndisasm -b16 -e4 ${PREFIX}.BIN >${PREFIX}.dis

        # count lines of the original disassembly
        LINES=$(wc -l ${DISORIG}/${PREFIX}.dis |cut -d " " -f 1);

        # make the fresh file have the same length
        head -n $LINES ${DISDIR}/${PREFIX}.dis >${DISDIR}/${PREFIX}.tmp
        mv ${DISDIR}/${PREFIX}.tmp ${DISDIR}/${PREFIX}.dis

	# compare the original disassembly with the working one
	diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis |less
done
