#!/bin/bash

# Build Windows executable su Linux/Mac usando MinGW cross compiler
cd "$(dirname "$0")"

if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    x86_64-w64-mingw32-gcc -o dnd_game.exe main.c -Wall -std=c99 -O2
    exit $?
fi

if command -v i686-w64-mingw32-gcc >/dev/null 2>&1; then
    i686-w64-mingw32-gcc -o dnd_game.exe main.c -Wall -std=c99 -O2
    exit $?
fi

echo "Nessun cross-compiler Windows trovato. Installa x86_64-w64-mingw32-gcc o i686-w64-mingw32-gcc."
exit 1
