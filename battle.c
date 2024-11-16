#include <stdio.h>
#include <string.h>
#include "battle.h"
#include "utils.h"
#include "character.h"
#include "spell.h"


// Start Battle Function
void startBattle(Character *player, Enemy *enemy) {
    printf("\nBattle Start!\n");

    // Roll for initiative
    int playerTurn = rollInitiative(player, enemy);

    while (player->hp > 0 && enemy->hp > 0) {
        if (playerTurn) {
            printf("\nYour Turn:\n1. Attack\n2. Use Potion\n");

            // Allow casting spells only for Mage and Cleric
            if (strcmp(player->class, "Mage") == 0 || strcmp(player->class, "Cleric") == 0) {
                printf("3. Cast Spell\n");
            }

            int action;
            scanf("%d", &action);

            if (action == 1) {
                playerAttack(player, enemy);
            } else if (action == 2) {
                useHealingPotion(player);
            } else if (action == 3 && (strcmp(player->class, "Mage") == 0 || strcmp(player->class, "Cleric") == 0)) {
                if (!hasAvailableSpellSlots(player)) {
                    printf("No available spell slots! Choose another action.\n");
                    continue;
                }

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
        } else {
            printf("\nEnemy's Turn:\n");
            enemyAttack(enemy, player);
        }

        // Check for end conditions
        if (enemy->hp <= 0) {
            printf("Enemy defeated!\n");
            printf("You gained %d XP!\n", enemy->xpReward);
            addExperience(player, enemy->xpReward);
            saveCharacterProgress(player);
            break;
        }

        if (player->hp <= 0) {
            printf("You have been defeated!\n");
            break;
        }

        if (enemy->stunned) {
            printf("\nThe enemy is stunned and skips its turn!\n");
            enemy->stunned = 0;
            playerTurn = 1;  // Player keeps their turn after enemy stun
        } else {
            playerTurn = !playerTurn;  // Toggle turns
        }
    }
}

// Player Attack Function
void playerAttack(Character *player, Enemy *enemy) {
    int attacks = (player->extraAttack && 
                  (strcmp(player->class, "Warrior") == 0 || 
                   strcmp(player->class, "Rogue") == 0)) ? 2 : 1;

    for (int i = 0; i < attacks; i++) {
        printf("\nAttack %d:\n", i + 1);
        int attackRoll = rollDice(20) + getModifier(player->strength);
        printf("Attack Roll: %d (vs AC %d)\n", attackRoll, enemy->armorClass);

        if (attackRoll >= enemy->armorClass) {
            int damage = 0;

            if (strcmp(player->class, "Rogue") == 0) {
                // Rogue attacks with two daggers
                damage = rollDice(4) + rollDice(4) + getModifier(player->dexterity);  
            } 
            else if (strcmp(player->class, "Warrior") == 0) {
                if (player->weaponDamage == 12) { 
                    // Warrior with Two-Handed Sword
                    damage = rollDice(6) + rollDice(6) + getModifier(player->strength);  
                } else { 
                    // Warrior with Sword and Shield (1d6)
                    damage = rollDice(player->weaponDamage) + getModifier(player->strength);
                }
            } 
            else if (strcmp(player->class, "Mage") == 0 || strcmp(player->class, "Cleric") == 0) {
                // Mage or Cleric using melee attacks
                damage = rollDice(player->weaponDamage) + getModifier(player->strength);
            } 
            else {
                // Default case for other classes
                damage = rollDice(player->weaponDamage) + getModifier(player->strength);
            }

            if (attackRoll == 20) {  // Critical hit
                printf("Critical Hit!\n");
                damage += rollDice(player->weaponDamage);
            }

            enemy->hp -= damage;
            printf("You dealt %d damage! Enemy HP: %d/%d\n", damage, enemy->hp, enemy->maxHp);

            if (enemy->hp <= 0) break;  // Stop further attacks if the enemy is defeated
        } else {
            printf("You missed! The enemy dodges swiftly.\n");
        }
    }
}

// Enemy Attack Function
void enemyAttack(Enemy *enemy, Character *player) {
    int attackRoll = rollDice(20) + enemy->attackBonus;
    printf("Enemy Attack Roll: %d (vs AC %d)\n", attackRoll, player->armorClass);

    if (attackRoll >= player->armorClass) {
        int damage = rollDice(enemy->damageDice) + getModifier(enemy->strength);
        player->hp -= damage;
        printf("Enemy hits you for %d damage! Player HP: %d/%d\n", damage, player->hp, player->maxHp);
    } else {
        printf("Enemy missed!\n");
    }
}



// Healing Potion Function
void useHealingPotion(Character *player) {
    printf("\nYou use a healing potion!\n");
    int heal = rollDice(4) + rollDice(4) + 2;  // 2d4 + 2 healing
    player->hp += heal;
    if (player->hp > player->maxHp) {
        player->hp = player->maxHp;
    }
    printf("You healed for %d HP. Current HP: %d/%d\n", heal, player->hp, player->maxHp);
}

// Roll Initiative Function
int rollInitiative(Character *player, Enemy *enemy) {
    int playerRoll, enemyRoll;

    do {
        playerRoll = rollDice(20) + getModifier(player->dexterity);
        enemyRoll = rollDice(20) + getModifier(enemy->dexterity);

        printf("\nInitiative Roll:\n");
        printf("%s rolled %d\n", player->name, playerRoll);
        printf("%s rolled %d\n", enemy->name, enemyRoll);

        if (playerRoll == enemyRoll) {
            printf("It's a tie! Rolling again...\n");
        }
    } while (playerRoll == enemyRoll);  // Reroll until there's a clear winner

    return (playerRoll > enemyRoll) ? 1 : 0;  // 1 if player goes first, 0 otherwise
}


// Check Available Spell Slots Function
int hasAvailableSpellSlots(Character *player) {
    for (int i = 0; i < 9; i++) {
        if (player->spellSlots[i] > 0) return 1;
    }
    return 0;  // No spell slots available
}
