@ECHO OFF
@REM These are the files, which produce the same code as the originals.
@REM They shall not change, so the are compared with every commit.
@REM
@REM -- here start the complete
BCC.EXE -mlarge -O  -c -2 -Z CDA_CODE.C
BCC.EXE -mlarge -O  -c -2 -Z -IAIL NGEN.C
BCC.EXE -mlarge -O  -c -2 -Z VGALIB.C
@REM BCC.EXE -mlarge -O  -c -2 -Z POWERP20.C
TASM.EXE /os /z POWERP20.ASM POWERP20.OBJ
TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM AIL.OBJ
TLINK @TLINK.RES
@REM pause
