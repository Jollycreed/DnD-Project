#ifndef BATTLE_H
#define BATTLE_H

#include "character.h"
#include "enemy.h"

void startBattle(Character *player, Enemy *enemy);
void playerAttack(Character *player, Enemy *enemy);
void enemyAttack(Enemy *enemy, Character *player);
void useHealingPotion(Character *player);
int validateSpellSlot(Character *player, int slotLevel);

#endif
