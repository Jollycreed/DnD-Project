#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"
#include "utils.h"

Enemy createEnemy(int playerLevel) {
    Enemy enemy;
    sprintf(enemy.name, "Goblin Lv %d", playerLevel);
    enemy.hp = enemy.maxHp = rollDice(8) + playerLevel * 2;
    enemy.armorClass = 12 + playerLevel / 2;
    enemy.attackBonus = 2 + playerLevel / 2;
    enemy.damageDice = 6;  // 1d6 damage
    enemy.stunned = 0;     // Initialize stunned flag
    enemy.xpReward = 50;   // Goblins give 50 XP
    return enemy;
}
