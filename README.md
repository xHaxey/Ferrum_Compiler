# Ferrum

Ferrum is a hobby programming language written in C++ and powered by LLVM.

Current Version: v0.1.0-alpha

Current features:

- Variables
- Arithmetic expressions
- Type checking
- Functions
- Function calls
- LLVM code generation
- Native executable generation

Requirements:

- LLVM 18.1.8 best
- Clang++ 18+
- Visual Studio 2022

Ferrum is in early development.

Currently Implemented:
- Variables
- Functions

Near future:
- String operations
- If expressions
- Loops


How to use:

./ferrum_compiler FerrumProject main.fe math.fe

After successful compilation:

FerrumProject/
  build/
    FerrumProject.obj
    FerrumProject.exe



Ferrum uses LLVM and Clang.

LLVM Project is licensed under
Apache License 2.0 with LLVM Exceptions.
