#!/bin/bash

# Launcher per macOS

cd "$(dirname "$0")"

if command -v docker >/dev/null 2>&1; then
    ./run_game.sh
    exit 0
fi

if ! command -v gcc >/dev/null 2>&1; then
    echo "Errore: né Docker né gcc sono disponibili. Installa Docker o gcc (Xcode command line tools) e riprova."
    exit 1
fi

if [ ! -f ./dnd_game ]; then
    echo "Docker non trovato, compilazione locale..."
    gcc -o dnd_game main.c -Wall -std=c99 -O2
fi

./dnd_game
