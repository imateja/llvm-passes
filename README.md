# LLVM Passes
## Passes
Implementation of:
- Dead Store Elimination - deletes useless assignments (store instructions in LLVM IR)
- Dead Global Elimination - deletes unused global variables
- Post Dominator Tree Analysis - finds immediate and semi dominators for basic blocks, bottom to top

## Setup
- Copy lines from the root CMakeLists.txt into llvm/lib/Transforms CMakeLists.txt
- Copy the pass folders into llvm/lib/Transforms
- Build the project from root llvm directory:
```sh
./make_llvm.sh
```
## Pass usage
Not everything is applicable when you clone the newest version from the repository (i.e. the pass manager option is currently deprecated). 
Use the LLVM version from the course or clone the equivalent.
Testing dead global elimination on your C file and writing the changes to a new, optimized file:
```sh
cd build
./bin/clang -emit-llvm -S source.c
./bin/opt -load lib/LLVMOurDGEPass.so -enable-new-pm=0 -our-dge -S source.ll > optimized.ll
```
Since Post Dominator Tree analysis isn't a transform pass redirect the output to /dev/null, read results from the generated .dot file.
