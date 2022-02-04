@echo off

echo Compiling with VM...
vm "bytecode/example.gofbc" -s
cd bytecode/

echo Compiling with NASM...
nasm -f win64 "example.gofbc.asm" -o "example.gofbc.o"

echo Linking with LD...
ld "example.gofbc.o" -o "example.gofbc.exe"

echo Executing final executable...
example.gofbc.exe

pause