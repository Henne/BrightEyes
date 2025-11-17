#!/bin/bash
#
# Requirements	: DOSBox, nasm, diff
# Returns	: <0 = error, otherwise the number off differences in code (0 = OK)

FNAME="BLADEM.EXE"
TMP=TEMP

mkdir -p ${TMP}

#clear bin files and old builds
rm -f ${TMP}/*.bin build/BLADEM.*

# build a new BINARY in temp
./tools/build.sh

# extract the parts of the original and the rewrite
# Note: The file SCHICKM.EXE was built as BLADEM.EXE
../tools/exe_comp orig/SCHICKM.EXE build/${FNAME}
if [ $? -lt 0 ]; then
	echo "ERROR: Failed to compare ${FNAME}"
	exit -1
else
	mv *.bin ${TMP}/
	RETVAL=$(cmp -l ${TMP}/ORIG_CODE.bin ${TMP}/REWR_CODE.bin |wc -l)
	RETVAL_OVR=$(cmp -l ${TMP}/ORIG_OVR.bin ${TMP}/REWR_OVR.bin |wc -l)
	echo "Differing Bytes in CODE = ${RETVAL}"
	echo "Differing Bytes in OVR = ${RETVAL_OVR}"

	if [ $RETVAL -gt 0 ]; then
		read -r -p "PRESS KEY TO SEE CODE DIFF"

		# disassemble them
		ndisasm -b16 -a -p intel ${TMP}/ORIG_CODE.bin >${TMP}/ORIG_CODE.dis
		ndisasm -b16 -a -p intel ${TMP}/REWR_CODE.bin >${TMP}/REWR_CODE.dis

		# compare them
		diff -y -a ${TMP}/ORIG_CODE.dis ${TMP}/REWR_CODE.dis |less
	fi

	if [ $RETVAL_OVR -gt 0 ]; then
		read -r -p "PRESS KEY TO SEE OVERLAY CODE DIFF"
		# disassemble them
		ndisasm -b16 -a -p intel ${TMP}/ORIG_OVR.bin >${TMP}/ORIG_OVR.dis
		ndisasm -b16 -a -p intel ${TMP}/REWR_OVR.bin >${TMP}/REWR_OVR.dis

		# compare them
		diff -y -a ${TMP}/ORIG_OVR.dis ${TMP}/REWR_OVR.dis |less
	fi

	exit ${RETVAL}
fi
