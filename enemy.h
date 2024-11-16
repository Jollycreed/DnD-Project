#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"

typedef struct {
    char name[30];
    int hp, maxHp;
    int armorClass;
    int attackBonus;
    int damageDice;   // Damage dice (e.g., 6 for 1d6)
    int dexterity;    // For initiative and AC calculation
    int strength;     // For melee attacks
    int constitution; // For HP calculations
    int magicka;      // For spellcasting enemies like necromancers
    int stunned;      // Indicates if the enemy is stunned
    int xpReward;     // Experience points awarded on defeat
} Enemy;

// Function prototypes
Enemy createEnemy(int playerLevel);
int rollInitiative(Character *player, Enemy *enemy);  // Use updated enemy stats for initiative

#endif
