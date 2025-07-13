#!/bin/bash

TMP=TEMP/

mkdir -p ${TMP}

# build a new GEN.EXE in temp
./tools/build.sh

# extract the parts of the original and the rewrite
g++ ds_check.cpp -o ds_check && ./ds_check orig/GEN104DE.EXE build/GEN104DE.EXE && cmp -l ${TMP}/ORIG_CODE.bin ${TMP}/REWR_CODE.bin |wc -l
read -r -p "PRESS KEY"
 
# disassemble them
ndisasm -b16 -a -p intel ${TMP}/ORIG_CODE.bin >${TMP}/ORIG_CODE.dis
ndisasm -b16 -a -p intel ${TMP}/REWR_CODE.bin >${TMP}/REWR_CODE.dis

# compare them
diff -y -a ${TMP}/ORIG_CODE.dis ${TMP}/REWR_CODE.dis |less
