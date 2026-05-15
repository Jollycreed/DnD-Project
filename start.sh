#!/bin/bash

# Start script rapido per D&D Text Adventure
# Usa Docker se disponibile, altrimenti compila ed esegue localmente.

set -e

if command -v docker &> /dev/null; then
    chmod +x build_docker.sh run_game.sh
    ./run_game.sh
    exit 0
fi

if ! command -v gcc &> /dev/null; then
    echo "Errore: né Docker né gcc sono disponibili. Installa Docker o gcc e riprova."
    exit 1
fi

if [ ! -f ./dnd_game ]; then
    echo "Docker non trovato, compilazione locale..."
    gcc -o dnd_game main.c -Wall -std=c99 -O2
fi

./dnd_game
