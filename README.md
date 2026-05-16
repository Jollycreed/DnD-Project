# 🎲 D&D Text Adventure — Terminal RPG ⚔️

[![C CI/CD Pipeline](https://github.com/Jollycreed/DnD-Project/actions/workflows/release.yml/badge.svg)](https://github.com/Jollycreed/DnD-Project/actions)
![Language](https://img.shields.io/badge/Language-C99-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)

Una ricca avventura testuale single-player da terminale ispirata alle regole di **Dungeons & Dragons 5e**, scritta interamente in **C**. Crea il tuo eroe distribuendo le statistiche con lo *Standard Array*, seleziona razza e classe, gestisci l'inventario e affronta mostri e boss in un sistema di combattimento a turni guidato dai classici tiri di dado ($d20$).

---

## 🚀 Caratteristiche Principali

* **Creazione Personaggio Completa:** Scelta di Razza, Classe e allocazione delle caratteristiche (15, 14, 13, 12, 10, 8) con calcolo automatico dei modificatori e dei Punti Ferita ($HP$).
* **Meccaniche di Classe Avanzate:** Gestione degli slot incantesimo per i maghi, punti *Ira* (Rage) per i Barbari, punti *Ki* per i Monaci e Punti Azione per i Guerrieri.
* **Sistema di Combattimento Dinamico:** Iniziativa basata sulla Destrezza, tiri per colpire contro la Classe Armatura ($AC$), gestione degli stati alterati (es. Stordimento) e Loot casuale.
* **Salvataggio Persistente:** Riprendi l'avventura quando vuoi. I salvataggi vengono gestiti automaticamente in formato flat-file all'interno della cartella `saves/` (con pieno supporto ai nomi con spazi).
* **Cross-Platform CI/CD:** Pipeline automatizzata tramite GitHub Actions che compila ed esporta nativamente gli eseguibili per **Linux**, **Windows** (`.exe`) e **macOS**.

---

## 🎮 Avvio Rapido

### 🐧 Linux & 🍏 macOS
Il repository include un launcher universale che automatizza i permessi e gestisce l'avvio:
```bash
chmod +x start.sh
./start.sh
