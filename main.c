#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "character.h"
#include "enemy.h"
#include "battle.h"

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
        initializeSpellSlots(&player);  // Properly initialize slots for new character
        saveCharacterProgress(&player);
    }
} else {
    printf("No saved character found. Creating a new character.\n");
    player = createCharacter();
    initializeSpellSlots(&player);  // Initialize spell slots for new character
    saveCharacterProgress(&player);
}


    Enemy goblin = createEnemy(player.level);
    printf("\nA wild %s appears! HP: %d, AC: %d\n", goblin.name, goblin.hp, goblin.armorClass);

    startBattle(&player, &goblin);

    return 0;
}
