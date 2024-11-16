#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "character.h"
#include "enemy.h"
#include "battle.h"
#include "utils.h"

void shortRest(Character *player);
void longRest(Character *player);
int rollForEnemies();
Enemy createBoss(char *bossName, int playerLevel); // Function to create boss enemies

int main() {
    srand(time(0));  // Initialize random seed

    FILE *file = fopen("character_save.txt", "r");
    Character player;

    if (file != NULL) {
        fclose(file);
        printf("A saved character was found.\n");
        printf("Do you want to load it? (1 = Yes, 2 = No): ");
        int loadChoice;
        scanf("%d", &loadChoice);

        if (loadChoice == 1) {
            player = loadCharacterProgress();
        } else {
            player = createCharacter();
            saveCharacterProgress(&player);
        }
    } else {
        printf("No saved character found. Creating a new character.\n");
        player = createCharacter();
        saveCharacterProgress(&player);
    }

    int battlesWon = 0; // Track battles won by the player

    while (1) {
        Enemy enemy;

        // Spawn special enemies based on battlesWon
        if (battlesWon == 10 && player.level >= 3) {
            enemy = createBoss("Necromancer", player.level);
        } else if (battlesWon == 20 && player.level >= 5) {
            enemy = createBoss("Shadow Lord", player.level);
        } else {
            enemy = createEnemy(player.level);
        }

        printf("\nA wild %s appears! HP: %d, AC: %d\n", enemy.name, enemy.hp, enemy.armorClass);

        startBattle(&player, &enemy);

        if (player.hp <= 0) {
            printf("\nYou have been defeated. Game over.\n");
            break;
        }

        battlesWon++; // Increment battles won
        printf("\nBattles Won: %d\n", battlesWon);

        printf("\nChoose your next action:\n1. Go deeper into the dungeon\n2. Rest\n");
        int choice;
        scanf("%d", &choice);

        if (choice == 2) {
            printf("\nChoose rest type:\n1. Short Rest\n2. Long Rest\n");
            int restType;
            scanf("%d", &restType);

            if (restType == 1) {
                shortRest(&player);
            } else {
                longRest(&player);
            }
        }
    }

    return 0;
}

// Short Rest Function (unchanged)
void shortRest(Character *player) {
    if (player->hitDice <= 0) {
        printf("You have no hit dice left. You must take a long rest.\n");
        return;
    }

    printf("\nShort Rest: You have %dd%d hit dice available.\n", player->hitDice, player->hitDiceValue);
    printf("Enter the number of hit dice to use: ");
    int diceToUse;
    scanf("%d", &diceToUse);

    if (diceToUse > player->hitDice) {
        printf("You don't have enough hit dice. Using maximum available (%d).\n", player->hitDice);
        diceToUse = player->hitDice;
    }

    int totalHeal = 0;
    for (int i = 0; i < diceToUse; i++) {
        int heal = rollDice(player->hitDiceValue);
        if (getModifier(player->constitution) > 0) {
            heal += getModifier(player->constitution);
        }
        totalHeal += heal;
    }

    player->hp += totalHeal;
    if (player->hp > player->maxHp) {
        player->hp = player->maxHp;
    }

    player->hitDice -= diceToUse;
    printf("You healed for %d HP. Current HP: %d/%d\n", totalHeal, player->hp, player->maxHp);
}

// Long Rest Function (unchanged)
void longRest(Character *player) {
    printf("\nLong Rest: Resting for 8 hours...\n");

    int roll = rollDice(100);
    if (roll <= 20) {
        printf("Enemies attack during your rest!\n");
        Enemy enemy = createEnemy(player->level);
        printf("\nA wild %s interrupts your rest! HP: %d, AC: %d\n", enemy.name, enemy.hp, enemy.armorClass);
        startBattle(player, &enemy);

        if (player->hp <= 0) {
            printf("\nYou have been defeated during your rest. Game over.\n");
            exit(0);
        }
    }

    player->hp = player->maxHp;
    player->hitDice = player->level;  // Restore all hit dice
    printf("You feel fully rested. HP: %d/%d, Hit Dice: %dd%d restored.\n", player->hp, player->maxHp, player->hitDice, player->hitDiceValue);
}
int rollForEnemies() {
    return rollDice(20);
}
