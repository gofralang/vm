@echo off

echo Compiling with VM...
vm "bytecode/example.gofbc" -s

echo Compiling with NASM...
nasm -f win64 "bytecode/example.gofbc.asm" -o "bytecode/example.gofbc.o"

echo Linking with LD...
ld "bytecode/example.gofbc.o" -o "bytecode/example.gofbc.exe"

echo Executing final executable...
"bytecode/example.gofbc.exe"

pause