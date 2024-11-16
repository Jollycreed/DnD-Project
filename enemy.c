#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemy.h"
#include "utils.h"

Enemy createEnemy(int playerLevel) {
    Enemy enemy;
    int roll = rollDice(20);

    if (roll <= 5) {
        strcpy(enemy.name, "Goblin");
        enemy.strength = 13;
        enemy.dexterity = 10;
        enemy.constitution = 8;
        enemy.magicka = 5;
        enemy.hp = rollDice(8) + getModifier(enemy.constitution);
        enemy.damageDice = 6;
        enemy.xpReward = 50;
    } else if (roll <= 10) {
        strcpy(enemy.name, "Skeleton");
        enemy.strength = 8;
        enemy.dexterity = 13;
        enemy.constitution = 10;
        enemy.magicka = 5;
        enemy.hp = rollDice(8) + getModifier(enemy.constitution);
        enemy.damageDice = 6;
        enemy.xpReward = 50;
    } else if (roll <= 14) {
        strcpy(enemy.name, "Zombie");
        enemy.strength = 10;
        enemy.dexterity = 8;
        enemy.constitution = 13;
        enemy.magicka = 5;
        enemy.hp = rollDice(12) + getModifier(enemy.constitution);
        enemy.damageDice = 6;
        enemy.xpReward = 150;
    } else if (roll <= 18) {
        strcpy(enemy.name, "Spider");
        enemy.strength = 8;
        enemy.dexterity = 14;
        enemy.constitution = 10;
        enemy.magicka = 5;
        enemy.hp = rollDice(10) + getModifier(enemy.constitution);
        enemy.damageDice = 6;
        enemy.xpReward = 200;
    } else if (roll == 19) {
        strcpy(enemy.name, "Bandit");
        enemy.strength = 14;
        enemy.dexterity = 12;
        enemy.constitution = 12;
        enemy.magicka = 5;
        enemy.hp = rollDice(12) + getModifier(enemy.constitution);
        enemy.damageDice = 8;
        enemy.xpReward = 500;
    } else {
        strcpy(enemy.name, "Necromancer");
        enemy.strength = 8;
        enemy.dexterity = 10;
        enemy.constitution = 10;
        enemy.magicka = 16;
        enemy.hp = rollDice(10) + getModifier(enemy.constitution);
        enemy.damageDice = 10;  // For spell damage
        enemy.xpReward = 1200;
    }

    enemy.maxHp = enemy.hp;
    enemy.armorClass = 10 + getModifier(enemy.dexterity);
    enemy.attackBonus = getModifier(enemy.strength);
    enemy.stunned = 0;

    return enemy;
}

Enemy createBoss(int playerLevel, const char *bossName) {
    Enemy boss;
    strcpy(boss.name, bossName);

    if (strcmp(bossName, "Necromancer") == 0) {
        boss.maxHp = 100 + (playerLevel * 10);  // Base HP + scaling per player level
        boss.hp = boss.maxHp;
        boss.armorClass = 15;
        boss.attackBonus = 5 + (playerLevel / 2);  // Scaling attack bonus
        boss.damageDice = 10;  // 1d10 spell damage
        boss.dexterity = 12;
        boss.strength = 8;
        boss.constitution = 14;
        boss.xpReward = 1200;
    } else if (strcmp(bossName, "Shadow Lord") == 0) {
        boss.hp = 150 + playerLevel * 10;
        boss.armorClass = 15 + getModifier(16);  // Stronger AC
        boss.strength = 18;
        boss.dexterity = 16;
        boss.constitution = 20;
        boss.magicka = 18;
        boss.damageDice = 10;  // 2d10 for Void Blast
        boss.xpReward = 2500;
    }

    boss.stunned = 0;  // Initialize stunned flag
    return boss;
}
