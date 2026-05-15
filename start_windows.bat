@echo off
REM Launcher per Windows
cd /d %~dp0

if exist dnd_game.exe (
    dnd_game.exe
    goto end
)

echo dnd_game.exe non trovato.
where gcc >nul 2>nul
if errorlevel 1 (
    echo gcc non trovato. Installa MinGW o copia un dnd_game.exe nella cartella.
    goto end
)

echo Compilazione locale con gcc...
gcc -o dnd_game.exe main.c -Wall -std=c99 -O2
if errorlevel 1 (
    echo Compilazione fallita.
    goto end
)

dnd_game.exe
:end
