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
        scanf("%d", &weaponChoice);
        if (weaponChoice == 1) {
            player->weaponDamage = 12;  // 2d6
            player->shieldBonus = 0;    // No shield
        } else {
            player->weaponDamage = 6;   // 1d6
            player->shieldBonus = 2;    // Shield adds 2 AC
        }
        player->armorClass = 15 + getModifier(player->dexterity) + player->shieldBonus;
    } else if (strcmp(player->class, "Mage") == 0) {
        player->weaponDamage = 6;       // 1d6 for basic spells
        player->armorClass = 10 + getModifier(player->dexterity);
    } else if (strcmp(player->class, "Cleric") == 0) {
        player->weaponDamage = 6;       // 1d6 basic melee weapon
        player->shieldBonus = 2;        // Cleric always has a shield
        player->armorClass = 13 + getModifier(player->dexterity) + player->shieldBonus;
    } else if (strcmp(player->class, "Rogue") == 0) {
        player->weaponDamage = 8;       // 2d4 (rogue uses two daggers)
        player->armorClass = 10 + (2 * getModifier(player->dexterity));  // Rogue has double dex modifier
    }

    player->maxHp = rollDice(10) + getModifier(player->constitution);
    player->hp = player->maxHp;
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
                player->spellSlots[0] = 2;
                break;
            case 2:
                player->spellSlots[0] = 3;
                break;
            case 3:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 2;
                break;
            case 4:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                break;
            case 5:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 2;
                break;
            case 20:
                player->spellSlots[0] = 4;
                player->spellSlots[1] = 3;
                player->spellSlots[2] = 3;
                player->spellSlots[3] = 3;
                player->spellSlots[4] = 3;
                player->spellSlots[5] = 2;
                player->spellSlots[6] = 2;
                player->spellSlots[7] = 2;
                player->spellSlots[8] = 1;
                break;
        }
    }
}
