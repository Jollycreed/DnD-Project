# Multi-stage build per ridurre la dimensione dell'immagine
FROM gcc:12 AS builder

WORKDIR /app

# Copia il codice sorgente
COPY main_fixed.c .

# Compila il gioco
RUN gcc -o dnd_game main_fixed.c -Wall -std=c99 -O2 && \
    strip dnd_game

# Stage finale
FROM debian:bookworm-slim

WORKDIR /app

# Installa solo runtime dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    libc6 && \
    rm -rf /var/lib/apt/lists/*

# Copia l'eseguibile dal builder
COPY --from=builder /app/dnd_game .

# Volume per salvare i progressi dei personaggi
VOLUME ["/app/saves"]

# Comando di avvio
CMD ["./dnd_game"]
