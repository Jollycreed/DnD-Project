#!/bin/bash

set -e
cd "$(dirname "$0")"

mkdir -p dist
rm -rf dist/*

echo "=== Creazione dei pacchetti di distribuzione ==="

if command -v gcc >/dev/null 2>&1; then
    echo "Compilazione del binario Linux..."
    gcc -o dnd_game main.c -Wall -std=c99 -O2
fi

WINDOWS_EXE=""
if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    echo "Compilazione del binario Windows 64-bit..."
    x86_64-w64-mingw32-gcc -o dnd_game.exe main.c -Wall -std=c99 -O2
    WINDOWS_EXE="dnd_game.exe"
elif command -v i686-w64-mingw32-gcc >/dev/null 2>&1; then
    echo "Compilazione del binario Windows 32-bit..."
    i686-w64-mingw32-gcc -o dnd_game.exe main.c -Wall -std=c99 -O2
    WINDOWS_EXE="dnd_game.exe"
else
    echo "Nessun cross-compiler Windows trovato. Il pacchetto Windows conterrà il launcher, ma non il .exe." 
fi

# Linux package
mkdir -p dist/linux
cp main.c README.md start.sh start_linux.sh Dockerfile build_docker.sh run_game.sh .dockerignore dist/linux/ 2>/dev/null || true
if [ -f dnd_game ]; then
    cp dnd_game dist/linux/
fi

tar -czf dist/dnd-game-linux.tar.gz -C dist/linux .

# Windows package
mkdir -p dist/windows
cp main.c README.md start_windows.bat build_windows.sh run_game.sh .dockerignore dist/windows/ 2>/dev/null || true
if [ -n "$WINDOWS_EXE" ] && [ -f dnd_game.exe ]; then
    cp dnd_game.exe dist/windows/
else
    echo "Nessun eseguibile Windows incluso. Usa build_windows.sh con MinGW o carica il .exe da una build CI." > dist/windows/README_WINDOWS.txt
fi
pushd dist/windows >/dev/null
zip -r ../dnd-game-windows.zip ./* >/dev/null
popd >/dev/null

# macOS package
mkdir -p dist/macos
cp main.c README.md start_macos.sh Dockerfile build_docker.sh run_game.sh .dockerignore dist/macos/ 2>/dev/null || true
pushd dist/macos >/dev/null
zip -r ../dnd-game-macos.zip ./* >/dev/null
popd >/dev/null

rm -rf dist/linux dist/windows dist/macos

echo "\nPacchetti creati in dist/:"
ls -1 dist
