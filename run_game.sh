#!/bin/bash

# Script per eseguire il gioco D&D in Docker

echo "╔════════════════════════════════════════╗"
echo "║    D&D Text Adventure - Game Launcher  ║"
echo "╚════════════════════════════════════════╝"

# Variabili
IMAGE_NAME="dnd-game"
IMAGE_TAG="latest"
DOCKER_IMAGE="${IMAGE_NAME}:${IMAGE_TAG}"
SAVES_DIR="./saves"

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Controlla se Docker è installato
if ! command -v docker &> /dev/null; then
    echo -e "${RED}Docker non trovato. Installa Docker prima di continuare.${NC}"
    exit 1
fi

# Controlla se l'immagine esiste
if ! docker image inspect "${DOCKER_IMAGE}" &> /dev/null; then
    echo -e "${YELLOW}Immagine Docker non trovata. Building...${NC}"
    bash build_docker.sh
    if [ $? -ne 0 ]; then
        echo -e "${RED}Build fallito!${NC}"
        exit 1
    fi
fi

# Crea la directory saves se non esiste
mkdir -p "${SAVES_DIR}"

# Esegui il gioco
echo -e "${GREEN}Avvio del gioco...${NC}"
echo ""

docker run -it --rm \
    -v "${SAVES_DIR}:/app/saves" \
    "${DOCKER_IMAGE}"

echo ""
echo -e "${GREEN}Grazie per aver giocato!${NC}"
