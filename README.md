# D&D Text Adventure

Gioco da terminale in C con meccaniche ispirate a D&D. Il salvataggio dei personaggi avviene localmente nella cartella `saves/`.

## Avvio rapido

### Linux / macOS

```bash
chmod +x start.sh start_linux.sh start_macos.sh build_windows.sh
./start.sh
```

### Windows

Apri `start_windows.bat` con un doppio clic o esegui da `cmd`.

## Compilazione manuale

```bash
gcc -o dnd_game main.c -Wall -std=c99 -O2
./dnd_game
```

## Pacchetti e release OS-specifiche

Per creare release dedicate per ogni sistema operativo, usa lo script:

```bash
chmod +x package_release.sh
./package_release.sh
```

I pacchetti vengono generati in `dist/` e includono:
- `dnd-game-linux.tar.gz`
- `dnd-game-windows.zip`
- `dnd-game-macos.zip`

Se pubblichi una release su GitHub, il workflow `.github/workflows/release.yml` può costruire e allegare automaticamente gli asset.

## Salvataggi

I personaggi salvati sono file `.txt` nella cartella `saves/`.
Usa la voce del menu "List Saved Characters" per visualizzare tutti i salvataggi.

## File principali

- `main.c` : codice sorgente del gioco
- `README.md` : documentazione principale
- `Dockerfile` : immagine Docker
- `Makefile` : comandi di build e pulizia
- `start.sh` : avvio rapido su Linux/macOS
- `start_linux.sh` / `start_macos.sh` / `start_windows.bat` : launcher specifici
- `build_windows.sh` : helper per compilare un `.exe` Windows con MinGW
- `build_docker.sh` : build dell'immagine Docker
- `run_game.sh` : avvia il gioco in Docker
- `package_release.sh` : crea pacchetti di distribuzione per release OS-specifiche
- `.github/workflows/release.yml` : workflow per creare asset GitHub Release
- `docker-compose.yml` : configurazione Docker Compose facoltativa
- `saves/` : directory dei salvataggi dei personaggi

## Avvio semplice su Windows

Se distribuisci `dnd-game-windows.zip`, il tester può semplicemente aprire la cartella estratta e fare doppio clic su `start_windows.bat`.
Per un avvio "tipo gioco normale", crea un collegamento sul desktop a `start_windows.bat` o a `dnd_game.exe`.

## Note

- I salvataggi vengono salvati e caricati automaticamente da `saves/`.
- Al termine di una battaglia, il gioco richiede di premere Invio prima di tornare al menu.
- Il file `INSTALLATION.txt` è stato rimosso perché ridondante rispetto a questo README.
