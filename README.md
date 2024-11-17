# **D&D PROJECT**
*Text-Based RPG Game*

This project is a text-based RPG game inspired by Dungeons & Dragons. Players create a character, progress through levels, and engage in turn-based battles. The game offers strategic combat, character customization, and a dynamic spellcasting system—all accessible via a simple command-line interface.
### Features
##### Character Creation

Choose from a variety of classes:
- Paladin
- Cleric
- Wizard
- Rogue
- Warlock
- Bard
- Fighter
- Barbarian
- Ranger
- Sorcerer
- Druid
- Monk
##### Assign attributes using the standard array systemAssign attributes using the standard array system (15, 14, 13, 12, 10, 8).
   Select equipment and weaponry unique to each class.

#### Combat System

- Turn-based battles against procedurally generated enemies, including challenging bosses.
- Each class has unique combat abilities:
- Fighter, Paladin, Barbarian: Unlock Extra Attack at level 5.
- Rogue: Dual-wields daggers with double dexterity AC bonus.
- Spellcasters: Cast scaling spells with diverse effects, including damage, healing, and control.

#### Spellcasting System

- Cantrips: No spell slot required; always available.
- Spells: Scale in power when cast using higher-level spell slots.
- Diverse effects include damage, healing, buffing, control, and utility.
- Spell slots and spell lists dynamically update as your character levels up.

#### Level Progression

- Earn experience points (XP) by defeating enemies.
- Level up to unlock new features:
- Gain HP and stat improvements.
- Access higher-level spells and abilities.
- Increase spell slot count.

#### Inventory and Equipment System
*work in progress*
- Manage dynamic inventory, including potions, weapons, and armor.
- Armor Class (AC) adjusts based on equipped gear.
- Loot system for acquiring resources and consumables after battles.

#### Hardcore Mode
If selected: 
**    Permadeath mechanics: Characters deleted upon death, offering a high-risk, high-reward gameplay experience.**

## How to Play

  Download the code from the repository.
  Open your terminal and navigate to the game folder.
  Compile the game by running:

`gcc main.c -o rpg_game`

##### Start the game:

`./rpg_game`


## Technology Stack

    C Programming Language
    Command-Line Interface (CLI) for user input and interaction.
    File I/O for saving/loading character progress.

### Future Improvements

    Expanded enemy types: Introduce new battle scenarios with diverse foes.
    Story integration: Add descriptive narratives for battle locations and enemies.
    Party system: Include additional party members and cooperative mechanics.
    Expanded inventory system: Introduce gear upgrades and consumables beyond health potions.
    Distribution improvements: Develop an easier installation and launch method for players.
