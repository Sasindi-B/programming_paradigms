#!/bin/bash
set -e

echo "Building TestLang++..."

flex testlang.l
bison -d testlang.y

echo "Compiling parser..."
gcc lex.yy.c testlang.tab.c -o testlang_compiler

echo "Compiling Java support files..."
javac Samples/src/*.java

echo "Build complete."
