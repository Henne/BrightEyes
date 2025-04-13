# TODO

This file tries to give a brief overview over the current state of the project.

## What's done
* refactoring of the code
* restructuring of initialized data (DS:INIT)
* correct compiler flags + minimal set of Sync-Points
* have a DOS-Build (not working atm)
* compile with GCC/Clang
* linking with Clang

## What's WIP
We distinguish between two different worlds here:
* BCC-World: DOS / 16-bit => create an identical binary
* GCC/Clang-World: for now Linux / 64-bit => create an identical working binary
The BCC-World takes precedence due to it's age.
The GCC/Clang-world is there for support.

### BCC-World
* restructuring of uninitialized data (DS:BSS)
* seg004: normalize_ptr() calls F_LXLSH (32bit left shift) from CLib and is not linked correctly

### GCC/Clang-World
* linking SDL2 with GCC
* using compiler warnings to make things better

## Overview

| Filenames    | Content       | BCC/DOS-World      | GCC/Clang-World    | State      |
| ------------ | ------------- | ------------------ | ------------------ | ---------- |
| seg000.h     | BCC Clib      | **DONE**           | **DONE**           | **DONE**   |
| seg001.[ch]  | AudioCD code  | should be rebuild  | should be removed  | WIP        |
| seg002.[ch]  | Main          | should be rebuild  | should be rebuild  | WIP        |
| seg003.[ch]  | Random        | **DONE**           | **DONE**           | **DONE**   |
| seg004.[ch]  | PowerPack2.0  | -                  | **DONE**           | **DONE**   |
| seg004.asm/h | PowerPack2.0  | should be rebuild  | not portable       | WIP        |
| seg005.[ch]  | Rasterlib     | -                  | should be rebuild  | WIP        |
| seg005.asm/h | Rasterlib     | should be rebuild  | not portable       | WIP        |
| seg006.[ch]  | AIL (Sound)   | -                  | should be replaced | WIP        |
| seg006.asm/h | AIL (Sound)   | should be rebuild  | not portable       | WIP        |
| seg007.asm/h | AudioCD data  | should be rebuild  | not portable       | WIP        |
| hero.h       | hero struct   | should be complete | should be complete | WIP        |
| port.h       | DOSBox compat | **DONE**           | **DONE**           | **DONE**   |
| symbols.h    | DS adresses   | **obsolete**       | **obsolete**       | **DONE**   |
