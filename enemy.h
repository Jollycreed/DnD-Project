#ifndef ENEMY_H
#define ENEMY_H

typedef struct {
    char name[30];
    int hp, maxHp;
    int armorClass;
    int attackBonus;
    int damageDice;
    int stunned;
    int xpReward;
} Enemy;

Enemy createEnemy(int playerLevel);

#endif
