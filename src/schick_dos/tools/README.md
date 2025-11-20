# tools folder (game specific)

In this directory are small helpers to prepare manual and automated testing.

## Usage
### Preparation (disassemble.sh)
This needs only to be done once, but it needs to be done the right way.
1. make sure you have installed: Nasm, Python3, DOSBox
2. place an executable into ../orig/, e.g. GAME.EXE
3. edit disassemble.sh (GAMEFILE, offsets/lengths of EACH segment inside GAME.EXE) MAGIC!!!
4. run it from the tools drectory, check in ../TEMP/disasm_orig for the disassembly files

### Manual check (compile.sh)
This should be used while working on a specific segment.
Requires a working compilation environment!
1. go into the rewrite directory of GAME.EXE
2. edit compile.bat to select the segment to investigate
3. run ./tools/bc.sh from there sucessfully
4. investigate output

### Automatic test (build.sh)
#### preparation 
Requires a working compilation environment!
1. go into the rewrite directory of GAME.EXE
2. have build.bat to compile all segments
3. edit Bright-Eyes/.git/hooks/pre-commit to run build.sh
4. make a commit
