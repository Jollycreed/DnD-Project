#!/bin/bash

# Script per compilare e buildare il gioco D&D in Docker

echo "╔════════════════════════════════════════╗"
echo "║  D&D Text Adventure - Docker Builder   ║"
echo "╚════════════════════════════════════════╝"

# Variabili
IMAGE_NAME="dnd-game"
IMAGE_TAG="latest"
DOCKER_IMAGE="${IMAGE_NAME}:${IMAGE_TAG}"

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Controlla se Docker è installato
if ! command -v docker &> /dev/null; then
    echo -e "${RED}Docker non trovato. Installa Docker prima di continuare.${NC}"
    exit 1
fi

# Build dell'immagine
echo -e "${YELLOW}Building Docker image: ${DOCKER_IMAGE}${NC}"
docker build -t "${DOCKER_IMAGE}" .

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Build completato con successo!${NC}"
    echo ""
    echo -e "${GREEN}Per giocare, esegui:${NC}"
    echo "  ./run_game.sh"
    echo ""
    echo -e "${YELLOW}Oppure manualmente:${NC}"
    echo "  docker run -it --rm -v \$(pwd)/saves:/app/saves ${DOCKER_IMAGE}"
else
    echo -e "${RED}✗ Build fallito!${NC}"
    exit 1
fi
