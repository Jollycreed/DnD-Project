# 🎲 D&D Text Adventure ⚔️

[![C CI/CD Pipeline](https://github.com/Jollycreed/DnD-Project/actions/workflows/release.yml/badge.svg)](https://github.com/Jollycreed/DnD-Project/actions)
![Language](https://img.shields.io/badge/Language-C99-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)

Un gioco di ruolo testuale single-player scritto in C (C99), ispirato alle regole di Dungeons & Dragons 5e.

Il progetto include un sistema di creazione del personaggio (basato su Standard Array), gestione delle risorse di classe (slot incantesimo, ira, punti Ki), combattimento a turni con tiri di dado ($d20$) e salvataggi locali.

---

## Prerequisiti

Per compilare ed eseguire il progetto in locale è necessario un compilatore C (`gcc` o `clang`).

Su sistemi Windows, i runner di build utilizzano la toolchain MSYS2/MinGW. È presente anche il supporto a Docker per l'esecuzione in ambiente containerizzato.

---

## Avvio Rapido
## Linux / macOS

Assegna i permessi di esecuzione allo script di avvio ed eseguilo:
```bash
chmod +x start.sh
./start.sh
```

(Dopo aver dato i permessi la prima volta, è possibile avviare il gioco anche facendo doppio clic su start.sh dal file manager).

---

## Windows

Scarica l'archivio dnd-game-windows.zip dall'area Releases, estrailo e lancia:
```DOS
start_windows.bat
```

# Compilazione
gcc -o dnd_game main.c -Wall -std=c99 -O2

# Esecuzione
```bash
./dnd_game
```
# Struttura del Repository

*  main.c: Codice sorgente principale del gioco.
*  saves/: Directory locale in cui vengono scritti i file di salvataggio.
*  start.sh / start_windows.bat: Script di runtime per l'avvio rapido sui diversi OS.
*  Dockerfile / build_docker.sh: Configurazione per la containerizzazione.
*  .github/workflows/release.yml: Pipeline di GitHub Actions per la cross-compilazione automatica dei binari ad ogni release.

# Contributi

Se riscontri bug nel calcolo dei modificatori o crash di segmentazione durante i combattimenti, apri una Issue o una Pull Request direttamente sul branch main.
