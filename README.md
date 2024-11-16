# D&D Project
## Text-Based RPG Game

### Description
This project is a **text-based RPG game** based on D&D where players create a character and engage in turn-based battles against enemies. The game focuses on character progression, strategic combat, and spellcasting mechanics, all managed through a simple command-line interface.

### Features
- **Character Creation**:
  - Choose from four classes: **Warrior**, **Cleric**, **Mage**, or **Rogue**.
  - Assign attribute points to customize your character's abilities.
  
- **Combat System**:
  - Turn-based battles against various enemies.
  - Each class has unique combat mechanics:
    - **Warrior**: Option to wield a **Two-Handed Sword** (2d6 damage) or a **Sword and Shield** (1d6 + AC bonus).
    - **Rogue**: Dual-wields **daggers** (2d4 damage) and benefits from double dexterity for AC.
    - **Cleric**: Uses a **shield** for defense and casts healing or damage spells.
    - **Mage**: Focuses on offensive spells with elemental effects.
  - Extra attack unlocked for **Warriors** and **Rogues** at level 5.

- **Spellcasting**:
  - **Clerics** and **Mages** can cast spells using spell slots.
  - Spells scale in power when cast with higher-level spell slots.
  - Spell mechanics include **damage**, **healing**, and **stunning** effects.

- **Level Progression**:
  - Gain experience points (XP) by defeating enemies.
  - Level up to improve stats and gain new abilities.
  - Automatically update spell slot availability and unlock new combat features.

- **Inventory and Equipment**:
  - Different weapons and armor for each class.
  - Dynamic armor class (AC) calculation based on class and equipment.

### How to Play
1. **Download the code**
2. **Open your terminal**, navigate to the game folder and type: **gcc main.c character.c enemy.c battle.c utils.c spell.c -o rpg_game** to build the game itself, then type: **./rpg_game**
3. **Create or Load a Character**.
4. **Engage in Battles** by choosing actions each turn:
   - Attack with your weapon.
   - Use a healing potion.
   - Cast a spell.
5. **Defeat enemies**, gain XP, and level up your character.

### Technology Stack
- **C Programming Language**
- **Command-Line Interface (CLI)** for user interaction.
- **File I/O** for saving/loading character progress.

### Future Improvements
- Add more enemy types and battle scenarios.
- Implement movement, and a battle scenario description.
- Add party members and not only the playable character, add different enemies.
- Enhance the inventory system with consumables and gear upgrades.
- Probably a way to normally download and launch the game honestly.
