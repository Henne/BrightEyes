# TODO

This file tries to give a brief overview over the current state of the project.

## What's done
* refactoring of the code
* restructuring of initialized data (DS:INIT)
* correct compiler flags + minimal set of Sync-Points
* have a working DOS-Build (only 78 bytes difference with identical behaviour)
* compile 32/64-bit binaries with GCC/Clang
* give the files meaningful names


## What's WIP
We distinguish between two different worlds here:
* BCC-World: DOS / 16-bit => create an identical binary
* GCC/Clang-World: for now Linux / 64-bit => create an *identical working* binary

The BCC-World takes precedence due to it's age.
The GCC/Clang-World is there for support.

### BCC-World
* comparing differences in the rewritten GEN.EXE on binary level

### GCC/Clang-World
* using modern software development tools to improve things

## Overview

| Filenames    | Content       | BCC/DOS-World      | GCC/Clang-World    | State      |
| ------------ | ------------- | ------------------ | ------------------ | ---------- |
| seg000.h     | BCC Clib      | **DONE**           | **DONE**           | **DONE**   |
| seg001.[ch]  | AudioCD code  | **DONE**           | -                  | **DONE**   |
| seg002.[ch]  | Main          | **DONE**           | should be rebuild  | WIP        |
| seg003.[ch]  | Random        | **DONE**           | **DONE**           | **DONE**   |
| seg004.[ch]  | PowerPack2.0  | -                  | **DONE**           | **DONE**   |
| seg004.asm/h | PowerPack2.0  | **DONE**           | **not portable**   | **DONE**   |
| seg005.[ch]  | Rasterlib     | -                  | should be rebuild  | WIP        |
| seg005.asm/h | Rasterlib     | **DONE**           | **not portable**   | **DONE**   |
| seg006.[ch]  | AIL (Sound)   | -                  | should be replaced | WIP        |
| seg006.asm/h | AIL (Sound)   | **DONE**           | **not portable**   | **DONE**   |
| seg007.asm/h | AudioCD data  | **DONE**           | **not portable**   | **DONE**   |
| hero.h       | hero struct   | **DONE**           | **DONE**           | **DONE**   |
| port.h       | DOSBox compat | **DONE**           | **DONE**           | **DONE**   |
| symbols.h    | DS adresses   | **obsolete**       | **obsolete**       | **DONE**   |
