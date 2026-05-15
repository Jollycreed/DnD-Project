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
# D&D Text Adventure

Terminal-based single-player adventure inspired by D&D, written in C.

This repository contains the game source, helper scripts and a CI workflow
that produces OS-specific release assets.

## Quick start

Linux / macOS:

1. Make scripts executable:

```bash
chmod +x start.sh package_release.sh
```

2. Start the game:

```bash
./start.sh
```

Windows:

Extract `dnd-game-windows.zip` from the Releases page and double-click
`start_windows.bat`, or run it from `cmd`.

## Manual build

To compile locally:

```bash
gcc -o dnd_game main.c -Wall -std=c99 -O2
./dnd_game
```

## Releases and packaging

Run `package_release.sh` to create local release packages in `dist/`, or
publish a GitHub Release and let the workflow `.github/workflows/release.yml`
build and attach official release artifacts:

- `dnd-game-linux.tar.gz`
- `dnd-game-windows.zip` (built on Windows runner; contains `dnd_game.exe`)
- `dnd-game-macos.zip`

Provide `dnd-game-windows.zip` to Windows testers so they can run the game
without compiling. Optionally create a desktop shortcut to `dnd_game.exe` or
`start_windows.bat` for easy launching.

## Saves

Saved characters are plain-text files in the `saves/` directory.

## Project files

- `main.c` — game source
- `README.md` — this file
- `Dockerfile`, `build_docker.sh`, `run_game.sh` — Docker support
- `start.sh`, `start_linux.sh`, `start_macos.sh`, `start_windows.bat` — launchers
- `build_windows.sh` — MinGW helper for Windows `.exe`
- `package_release.sh` — local packaging helper
- `.github/workflows/release.yml` — CI workflow for release assets

## Contributing

Bug reports and pull requests are welcome. Please open issues or PRs against
the `main` branch.

## License

Add your license file to the repository (e.g. `LICENSE`).

---

If you want, I can create a draft release to trigger the Actions workflow and
verify that the Windows `.exe` is produced on the GitHub runner.
