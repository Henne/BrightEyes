#TODO

This file tries to give a brief overview over the current state of the project.

## What's done
* refactoring of the code
* restructuring of initialized data (DS:INIT)
* correct compiler flags + minimal set of Sync-Points
* have a DOS-Build (not working)
* compile with GCC/Clang
* linking with GCC/Clang

## What's WIP
* restructuring of uninitialized data (DS:BSS)

## Overview

| Filenames    | Content       | BCC/DOS-World      | GCC/Clang-World    | State  |
| ------------ | ------------- | ------------------ | ------------------ | ------ |
| seg000.h     | BCC Clib      | should be removed  | should be removed  | DONE   |
| seg001.[ch]  | AudioCD code  | should be rebuild  | should be removed  | WIP    |
| seg002.[ch]  | Main          | should be rebuld   | should be rebuild  | WIP    |
| seg003.[ch]  | Random        | should be rebuild  | should be rebuild  | WIP    |
| seg004.[ch]  | PowerPack2.0  | -                  | should be rebuild  | WIP    |
| seg004.asm/h | PowerPack2.0  | should be rebuild  | not portable       | WIP    |
| seg005.[ch]  | Rasterlib     | -                  | should be rebuild  | WIP    |
| seg005.asm/h | Rasterlib     | should be rebuild  | not portable       | WIP    |
| seg006.[ch]  | AIL (Sound)   | -                  | should be replaced | WIP    |
| seg006.asm/h | AIL (Sound)   | should be rebuild  | not portable       | WIP    |
| seg007.asm/h | AudioCD data  | should be rebuild  | not portable       | WIP    |
| hero.h       | hero struct   | should be complete | should be complete | WIP    |
| port.h       | DOSBox compat | should be removed  | should be removed  | WIP    |
| symbols.h    | DS adresses   | obsolete           | obsolete           | DONE   |
