#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "character.h"
#include "utils.h"

Character createCharacter() {
    Character newChar;
    printf("Enter your character's name: ");
    scanf("%s", newChar.name);

    printf("Choose your race (Human, Elf, Half-Elf, Drow, Dwarf, Tiefling, Halfling): ");
    scanf("%s", newChar.race);

    printf("Choose your sex (Male/Female): ");
    scanf("%s", newChar.sex);

    printf("Choose your class (Warrior, Cleric, Mage, Rogue): ");
    scanf("%s", newChar.class);

    printf("\nDistribute your stats:\nAvailable scores: 15, 12, 10, 8\n");
    printf("Assign to Strength: ");
    scanf("%d", &newChar.strength);
    printf("Assign to Constitution: ");
    scanf("%d", &newChar.constitution);
    printf("Assign to Dexterity: ");
    scanf("%d", &newChar.dexterity);
    printf("Assign to Magicka: ");
    scanf("%d", &newChar.magicka);

    newChar.level = 1;
    newChar.experience = 0;
    setupInventory(&newChar);
    newChar.hp = newChar.maxHp;
    return newChar;
}

void setupInventory(Character *player) {
    if (strcmp(player->class, "Warrior") == 0) {
        printf("\nChoose your weapon:\n1. Two-Handed Sword (2d6 damage)\n2. Sword (1d6 damage) and Shield\n");
        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                player->weaponDamage = 12;  // Two-Handed Sword
                player->shieldBonus = 0;    // No shield
            } else if (weaponChoice == 2) {
                player->weaponDamage = 6;   // Sword
                player->shieldBonus = 2;    // Shield adds +2 AC
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 15 + getModifier(player->dexterity) + player->shieldBonus;

    } else if (strcmp(player->class, "Mage") == 0) {
        player->weaponDamage = 6;  // 1d6 for basic spells
        player->armorClass = 10 + getModifier(player->dexterity);

    } else if (strcmp(player->class, "Cleric") == 0) {
        player->weaponDamage = 6;  // 1d6 basic melee weapon
        player->shieldBonus = 2;   // Cleric always has a shield
        player->armorClass = 13 + getModifier(player->dexterity) + player->shieldBonus;

    } else if (strcmp(player->class, "Rogue") == 0) {
        player->weaponDamage = 8;  // 2d4 (dual daggers)
        player->armorClass = 10 + (2 * getModifier(player->dexterity));  // Double Dex modifier
    }

    // HP setup
    player->maxHp = rollDice(10) + getModifier(player->constitution);
    player->hp = player->maxHp;  // Set current HP to max HP
}




void saveCharacterProgress(Character *player) {
    FILE *file = fopen("character_save.txt", "w");
    if (file == NULL) {
        printf("Error saving character.\n");
        return;
    }
    fprintf(file, "%s %s %s %s %d %d %d %d %d %d %d %d %d",
            player->name, player->race, player->sex, player->class,
            player->level, player->experience,
            player->strength, player->constitution, player->dexterity, player->magicka,
            player->hp, player->maxHp, player->armorClass);
    
    for (int i = 0; i < 9; i++) {
        fprintf(file, " %d", player->spellSlots[i]);  // Save spell slots
    }
    fprintf(file, "\n");
    fclose(file);
    printf("Character progress saved successfully.\n");
}

Character loadCharacterProgress() {
    Character c;
    FILE *file = fopen("character_save.txt", "r");
    if (file == NULL) {
        printf("No saved character found. Exiting.\n");
        exit(1);
    }
    fscanf(file, "%s %s %s %s %d %d %d %d %d %d %d %d %d",
           c.name, c.race, c.sex, c.class,
           &c.level, &c.experience,
           &c.strength, &c.constitution, &c.dexterity, &c.magicka,
           &c.hp, &c.maxHp, &c.armorClass);

    for (int i = 0; i < 9; i++) {
        fscanf(file, " %d", &c.spellSlots[i]);  // Load spell slots
    }
    fclose(file);
    return c;
}

int getExperienceForNextLevel(int level) {
    int expTable[] = {0, 300, 900, 2700, 6500, 14000, 23000, 34000, 48000, 64000};
    return (level < 10) ? expTable[level] : expTable[9];
}

void addExperience(Character *player, int xp) {
    player->experience += xp;
    while (player->experience >= getExperienceForNextLevel(player->level)) {
        levelUp(player);
    }
}

void levelUp(Character *player) {
    player->level++;
    printf("\nCongratulations! You reached level %d!\n", player->level);

    int hpGain = rollDice(10) + getModifier(player->constitution);
    player->maxHp += hpGain;
    player->hp = player->maxHp;

    if (player->level == 5 &&
        (strcmp(player->class, "Warrior") == 0 || strcmp(player->class, "Rogue") == 0)) {
        player->extraAttack = 1;
        printf("You gained Extra Attack! You can now attack twice in your turn.\n");
    }
}

void initializeSpellSlots(Character *player) {
    for (int i = 0; i < 9; i++) {
        player->spellSlots[i] = 0;
    }

    if (strcmp(player->class, "Mage") == 0 || strcmp(player->class, "Cleric") == 0) {
        switch (player->level) {
            case 1:
                player->spellSlots[0] = 2;  // 2 slots for 1st level
                break;
            case 2:
                player->spellSlots[0] = 3;
                break;
            case 3:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 2;  // 2 slots for 2nd level
                break;
            case 4:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                break;
            case 5:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 2;  // 2 slots for 3rd level
                break;
            case 6:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                break;
            case 7:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 1;  // 1 slot for 4th level
                break;
            case 8:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 2;
                break;
            case 9:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 1;  // 1 slot for 5th level
                break;
            case 10:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                break;
            case 11:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                player->spellSlots[5] = 1;  // 1 slot for 6th level
                break;
            case 12:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                player->spellSlots[5] = 1;
                break;
            case 13:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                player->spellSlots[5] = 1;
                player->spellSlots[6] = 1;  // 1 slot for 7th level
                break;
            case 14:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                player->spellSlots[5] = 1;
                player->spellSlots[6] = 1;
                break;
            case 15:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                player->spellSlots[5] = 1;
                player->spellSlots[6] = 1;
                player->spellSlots[7] = 1;  // 1 slot for 8th level
                break;
            case 16:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 2;
                player->spellSlots[5] = 1;
                player->spellSlots[6] = 1;
                player->spellSlots[7] = 1;
                break;
            case 17:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 3;  // 3 slots for 5th level
                player->spellSlots[5] = 1;
                player->spellSlots[6] = 1;
                player->spellSlots[7] = 1;
                player->spellSlots[8] = 1;  // 1 slot for 9th level
                break;
            case 18:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 3;
                player->spellSlots[5] = 1;
                player->spellSlots[6] = 1;
                player->spellSlots[7] = 1;
                player->spellSlots[8] = 1;
                break;
            case 19:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 3;
                player->spellSlots[5] = 2;  // 2 slots for 6th level
                player->spellSlots[6] = 2;  // 2 slots for 7th level
                player->spellSlots[7] = 1;
                player->spellSlots[8] = 1;
                break;
            case 20:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 3;
                player->spellSlots[5] = 2;
                player->spellSlots[6] = 2;
                player->spellSlots[7] = 2;  // 2 slots for 8th level
                player->spellSlots[8] = 1;  // 1 slot for 9th level
                break;
        }
    }
}

