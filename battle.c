#include <stdio.h>
#include <string.h>
#include "battle.h"
#include "utils.h"
#include "character.h"
#include "spell.h"

void startBattle(Character *player, Enemy *enemy) {
    printf("\nBattle Start!\n");

    while (player->hp > 0 && enemy->hp > 0) {
        printf("\nYour Turn:\n1. Attack\n2. Use Potion\n3. Cast Spell\n");
        int action;
        scanf("%d", &action);

        if (action == 1) {
            playerAttack(player, enemy);
        } else if (action == 2) {
            useHealingPotion(player);
        } else if (action == 3) {
            listSpells(player);
            printf("Choose a spell to cast: ");
            int spellChoice;
            scanf("%d", &spellChoice);

            Spell chosenSpell = getSpellByClassAndChoice(player, spellChoice);
            if (strcmp(chosenSpell.name, "Invalid") == 0) {
                printf("Invalid spell choice!\n");
                continue;
            }

            printf("Choose a spell slot level to cast with (1-9): ");
            int slotLevel;
            scanf("%d", &slotLevel);

            slotLevel = validateSpellSlot(player, slotLevel);
            if (slotLevel == -1) {
                printf("No valid spell slot available.\n");
                continue;
            }

            castSpell(player, enemy, chosenSpell, slotLevel);
        } else {
            printf("Invalid choice! Skipping your turn...\n");
        }

        if (enemy->hp <= 0) {
            printf("Enemy defeated!\n");
            printf("You gained %d XP!\n", enemy->xpReward);
            addExperience(player, enemy->xpReward);
            saveCharacterProgress(player);
            break;
        }

        if (enemy->stunned) {
            printf("\nThe enemy is stunned and skips its turn!\n");
            enemy->stunned = 0;
        } else {
            printf("\nEnemy's Turn:\n");
            enemyAttack(enemy, player);
        }

        if (player->hp <= 0) {
            printf("You have been defeated!\n");
            break;
        }
    }
}

void playerAttack(Character *player, Enemy *enemy) {
    int attacks = player->extraAttack ? 2 : 1;  // Extra Attack at level 5 for Warrior/Rogue
    for (int i = 0; i < attacks; i++) {
        printf("\nAttack %d:\n", i + 1);
        int attackRoll = rollDice(20) + getModifier(player->strength);
        printf("Attack Roll: %d (vs AC %d)\n", attackRoll, enemy->armorClass);

        if (attackRoll >= enemy->armorClass) {
            int damage = 0;
            if (strcmp(player->class, "Rogue") == 0) {
                damage = rollDice(4) + rollDice(4) + getModifier(player->dexterity);  // Two daggers and uses Dex
            } else if (strcmp(player->class, "Warrior") == 0 && player->weaponDamage == 12) {
                damage = rollDice(6) + rollDice(6) + getModifier(player->strength);  // 2d6 for Two-Handed Sword
            } else {
                damage = rollDice(player->weaponDamage) + getModifier(player->strength);
            }

            if (attackRoll == 20) {  // Critical hit
                printf("Critical Hit!\n");
                damage += rollDice(player->weaponDamage);
            }

            enemy->hp -= damage;
            printf("You dealt %d damage! Enemy HP: %d/%d\n", damage, enemy->hp, enemy->maxHp);

            if (enemy->hp <= 0) break;  // Exit loop if enemy is defeated
        } else {
            printf("You missed!\n");
        }
    }
}

void enemyAttack(Enemy *enemy, Character *player) {
    int attackRoll = rollDice(20) + enemy->attackBonus;
    printf("Enemy Attack Roll: %d (vs AC %d)\n", attackRoll, player->armorClass);

    if (attackRoll >= player->armorClass) {
        int damage = rollDice(enemy->damageDice);
        player->hp -= damage;
        printf("Enemy hits you for %d damage! Player HP: %d/%d\n", damage, player->hp, player->maxHp);
    } else {
        printf("Enemy missed!\n");
    }
}

void useHealingPotion(Character *player) {
    printf("\nYou use a healing potion!\n");
    int heal = rollDice(4) + rollDice(4) + 2;  // 2d4 + 2 healing
    player->hp += heal;
    if (player->hp > player->maxHp) {
        player->hp = player->maxHp;
    }
    printf("You healed for %d HP. Current HP: %d/%d\n", heal, player->hp, player->maxHp);
}
