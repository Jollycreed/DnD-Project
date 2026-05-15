.PHONY: help build run clean test compile package docker-clean compose

IMAGE_NAME := dnd-game
IMAGE_TAG := latest
DOCKER_IMAGE := $(IMAGE_NAME):$(IMAGE_TAG)
SAVES_DIR := ./saves

help:
	@echo "╔════════════════════════════════════════╗"
	@echo "║  D&D Text Adventure - Makefile Help    ║"
	@echo "╚════════════════════════════════════════╝"
	@echo ""
	@echo "Available targets:"
	@echo "  make build        - Build l'immagine Docker"
	@echo "  make run          - Esegui il gioco in Docker"
	@echo "  make compile      - Compila il gioco localmente"
	@echo "  make test         - Testa la compilazione"
	@echo "  make package      - Crea pacchetti di distribuzione in dist/"
	@echo "  make clean        - Pulisci gli artefatti locali"
	@echo "  make docker-clean - Rimuovi l'immagine Docker"
	@echo "  make compose      - Esegui con docker-compose"
	@echo "  make help         - Mostra questo messaggio"

build:
	@echo "Building Docker image: $(DOCKER_IMAGE)"
	@docker build -t $(DOCKER_IMAGE) .
	@echo "Build completato!"

run: build
	@mkdir -p $(SAVES_DIR)
	@docker run -it --rm -v $$(pwd)/$(SAVES_DIR):/app/saves $(DOCKER_IMAGE)

compose:
	@mkdir -p $(SAVES_DIR)
	@docker-compose up --build

compile:
	@echo "Compilando il gioco..."
	@gcc -o dnd_game main.c -Wall -std=c99 -O2
	@echo "Compilazione completata: ./dnd_game"
	@echo "Esegui: ./dnd_game"

test: compile
	@echo "Test eseguito con successo!"

package:
	@chmod +x package_release.sh
	@./package_release.sh

clean:
	@rm -f dnd_game dnd_game.exe
	@rm -rf dist
	@echo "Pulizia completata"

docker-clean:
	@echo "Rimozione immagine Docker..."
	@docker rmi -f $(DOCKER_IMAGE) 2>/dev/null || true
	@echo "Pulizia Docker completata"

.DEFAULT_GOAL := help
