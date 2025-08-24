#!/bin/bash

# This script compiles all files maked as finalized and makes sure the have not changed

DIR=${PWD}/TEMP
OBJDIR=${DIR}/OBJ
BINDIR=${DIR}/BIN
DISDIR=${DIR}/disasm

DISORIG=${DIR}/disasm_orig

BUILDDIR=build/

DRIVE_C=${PWD}/../../drive_c

rm -rf $OBJDIR/* $BINDIR/* $DISDIR/*
mkdir -p $OBJDIR $BINDIR $DISDIR

# COMPILATION with DOSBox

# copy all source files to DRIVE_C
cp *.cpp *.h *.asm TLINK.RES ${DRIVE_C}/src
cp -r AIL ${DRIVE_C}/src

# copy c_ready.bat as compile.bat
cp build.bat ${DRIVE_C}/src/compile.bat

# run compile.bat in a DOSBox environment, needs an installed BCC.EXE there
pushd ${DRIVE_C}
dosbox -conf bcc31.conf
popd

# cleanup
rm -rf ${DRIVE_C}/src/*.cpp
rm -rf ${DRIVE_C}/src/*.h
rm -rf ${DRIVE_C}/src/*.asm
rm -rf ${DRIVE_C}/src/TLINK.RES
rm -rf ${DRIVE_C}/src/compile.bat
rm -rf ${DRIVE_C}/src/AIL

# move all OBJ-files to OBJDIR
mv ${DRIVE_C}/src/*.OBJ $OBJDIR 2>/dev/null

# move all OBJ-files to OBJDIR
mv ${DRIVE_C}/src/*.EXE $BUILDDIR 2>/dev/null
mv ${DRIVE_C}/src/*.MAP $BUILDDIR 2>/dev/null

ls ${OBJDIR}/*.OBJ | grep -o -E "SEG[0-9]+" >${OBJDIR}/summary
grep -o -P "SEG[0-9]+(?=\.(OBJ|CPP))" build.bat >build.summary
if cmp --silent ${OBJDIR}/summary build.summary; then
    echo "BCC hat alle OBJ-Dateien erzeugt."
    rm ${OBJDIR}/summary build.summary
else
    echo "Kompilieren der folgenden Segmente fehlgeschlagen:"
    comm -13 ${OBJDIR}/summary build.summary
    rm ${OBJDIR}/summary build.summary
    exit 1
fi

# TODO: VERIFICATION (when ready)
if [ -e ${BUILDDIR}/BLADEM.EXE ]; then
	exit 0
else
	exit -1
fi
