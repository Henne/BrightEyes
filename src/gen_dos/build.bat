@ECHO OFF
@REM These are the files, which produce the same code as the originals.
@REM They shall not change, so the are compared with every commit.
@REM
@REM -- here start the complete
BCC.EXE -mlarge -O  -c -2 -Z CDA_CODE.C
BCC.EXE -mlarge -O  -c -2 -Z -IAIL GEN105DE.C
BCC.EXE -mlarge -O  -c -2 -Z RANDOM.C
TASM.EXE /os /z POWERP20.ASM POWERP20.OBJ
TASM.EXE /os /z VGALIB.ASM VGALIB.OBJ
TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM AIL.OBJ
TASM.EXE /os /z CDA_DATA.ASM CDA_DATA.OBJ
TLINK @GEN105DE.RES

BCC.EXE -mlarge -O  -c -2 -Z -IAIL GEN300EN.C
TLINK @GEN300EN.RES
@REM pause
