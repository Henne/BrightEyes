@ECHO OFF
@REM These are the files, which produce the same code as the originals.
@REM They shall not change, so the are compared with every commit.
@REM
@REM -- here start the complete
BCC.EXE -mlarge -O- -c -2 SEG001.C
BCC.EXE -mlarge -O  -c -2 -Z -IAIL SEG002.C
BCC.EXE -mlarge -O  -c -2 -Z SEG003.C
TASM.EXE /os /z SEG004.asm SEG004.OBJ
TASM.EXE /os /z SEG005.asm SEG005.OBJ
TASM.EXE /m /w+ /ml /iAIL AIL\AIL.ASM SEG006.OBJ
TASM.EXE /os /z SEG007.asm SEG007.OBJ
TLINK @TLINK.RES
