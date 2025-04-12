#!/bin/bash

# This script compiles all files marked as finalized and makes sure the have not changed

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

cp symbols.h ${DRIVE_C}/src
cp port.h ${DRIVE_C}/src
cp hero.h ${DRIVE_C}/src
cp TLINK.RES ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src

# copy c_ready.bat as compile.bat
cp bc_ready.bat ${DRIVE_C}/src/compile.bat

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

# move all OBJ-files to OBJDIR
mv ${DRIVE_C}/src/*.EXE $DIR 2>/dev/null
mv ${DRIVE_C}/src/*.MAP $DIR 2>/dev/null

# VERIFICATION

N=0
GOOD=0
FAIL=0

# disassemble all *.OBJ files
for i in ${OBJDIR}/*.OBJ; do

	# extract the filename
	PREFIX=${i%\.OBJ}
	PREFIX=${PREFIX##*/}

	# TODO: SEG007 (CD-DATA) is not extracted properly
	if [ ${PREFIX} = "SEG007" ]; then
		GOOD=$(($GOOD+1))
		N=$(($N+1))
		continue;
	fi

	# extract instructions
	../tools/dump_obj $i >/dev/null
	# move the BIN-files to BINDIR
	mv ${OBJDIR}/${PREFIX}.BIN $BINDIR

	# diassemble BIN-file to DISDIR
	ndisasm -b16 ${BINDIR}/${PREFIX}.BIN >${DISDIR}/${PREFIX}.dis
#	ndisasm -b16 -e4 ${PREFIX}.BIN >${PREFIX}.dis

	# count lines of the original disassembly
	ORIGLINES=$(wc -l ${DISORIG}/${PREFIX}.dis | cut -d " " -f 1)

	# make the fresh file have the same length
	head -n $ORIGLINES ${DISDIR}/${PREFIX}.dis >${DISDIR}/${PREFIX}.tmp
	mv ${DISDIR}/${PREFIX}.tmp ${DISDIR}/${PREFIX}.dis


	RETVAL=0
	# count the lines containing '|' => difference
	case "${PREFIX}" in
		"SEG001")
			# exact 544 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)

			if [ $DIFFLINES -gt 544 ]; then RETVAL=1; fi
			echo "SEG001" $DIFFLINES
			;;
		"SEG002")
			# exact 2864 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -gt 2864 ]; then RETVAL=1; fi
			echo "SEG002" $DIFFLINES
			;;
		"SEG003")
			# exact 5 differing lines are allowed
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -ne 5 ]; then RETVAL=1; fi
			echo "SEG003" $DIFFLINES
			;;
		"SEG004")
			# exact 94 differing lines are allowed
			# adresses in unalinged codesegment
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -gt 94 ]; then RETVAL=1; fi
			echo "SEG004" $DIFFLINES
			;;
		"SEG005")
			# exact 15 differing lines are allowed
			# adresses in unalinged codesegment
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -gt 15 ]; then RETVAL=1; fi
			echo "SEG005" $DIFFLINES
			;;
		"SEG006")
			# AIL: dump_obj produces uncomparable files due to BSS
			# exact 129 differing lines are allowed
			# adresses in unalinged codesegment
			DIFFLINES=$(diff -y -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis | grep '|' |wc -l)
			if [ $DIFFLINES -gt 129 ]; then RETVAL=1; fi
			echo "SEG006" $DIFFLINES
			;;
		"SEG007")
			# AIL: dump_obj produces uncomparable files due to BSS
			;;
		"DATSEG")
			;;
		*)
			diff -q -a ${DISORIG}/${PREFIX}.dis ${DISDIR}/${PREFIX}.dis
			RETVAL=$?
			;;
	esac

	if [ $RETVAL -eq 0 ]; then
		GOOD=$(($GOOD+1))
	else
		echo "Fehler: ${PREFIX} ist verschieden"
		FAIL=$(($FAIL+1))
	fi

	N=$(($N+1))
done

echo "REPORT ${N} Files: Good = ${GOOD} Fail = ${FAIL}"

# count the compile commands in bc_ready.bat, to make sure there is none missing
COMP1=$(grep "BCC.EXE" bc_ready.bat | wc -l);
COMP2=$(grep "TASM.EXE" bc_ready.bat | wc -l);
COMP=$((${COMP1} + ${COMP2}))

if [ ${COMP} -ne ${N} ]; then
	echo "Fehler: ${N} Dateien wurden geprueft, aber es sollten ${COMP} sein"
	exit 1;
fi

exit $FAIL
