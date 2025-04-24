@ECHO OFF
@REM These are the files, which produce the same code as the originals.
@REM They shall not change, so the are compared with every commit.
@REM
@REM -- here start the complete
@REM
@REM
@REM
@REM
@REM BUILD THE LIBS < GEN104DE
BCC.EXE -mlarge -O -c -Z RANDOM.C
TASM.EXE /os /z POWERP20.ASM POWERP20.OBJ
TASM.EXE /os /z VGALIB.ASM VGALIB.OBJ
TASM.EXE /os /z SNDLIB.ASM SNDLIB.OBJ
TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM AIL.OBJ

@REM BUILD AND LINK THE BINARY GEN104DE
BCC.EXE -mlarge -O -c -Z -UNEWHERO GEN104DE.C
TLINK @GEN104DE.RES

del RANDOM.OBJ


@REM BUILD THE LIBS GEN105DE and GEN300EN
BCC.EXE -mlarge -O  -c -2 -Z CDA_CODE.C
TASM.EXE /os /z CDA_DATA.ASM CDA_DATA.OBJ
BCC.EXE -mlarge -O  -c -2 -Z RANDOM.C

BCC.EXE -mlarge -O  -c -2 -Z -IAIL -DNEWHERO GEN105DE.C
TLINK @GEN105DE.RES

BCC.EXE -mlarge -O  -c -2 -Z -IAIL -DNEWHERO GEN300EN.C
TLINK @GEN300EN.RES
pause
