#include <stdio.h>
#include <string.h>
#include "spell.h"
#include "utils.h"
#include "character.h"
#include "enemy.h"

Spell getSpellByClassAndChoice(Character *player, int spellChoice) {
    if (strcmp(player->class, "Mage") == 0) {
        if (spellChoice == 1) return (Spell){"Firebolt", 10, "Damage"};
        if (spellChoice == 2) return (Spell){"Icebolt", 8, "Damage"};
        if (spellChoice == 3) return (Spell){"Sparkingbolt", 4, "Stun"};
    } else if (strcmp(player->class, "Cleric") == 0) {
        if (spellChoice == 1) return (Spell){"Heal", 6, "Heal"};
        if (spellChoice == 2) return (Spell){"Sacred Fire", 8, "Damage"};
    }
    return (Spell){"Invalid", 0, "None"};
}

void listSpells(Character *player) {
    int magickaMod = getModifier(player->magicka);

    printf("\nAvailable Spells:\n");
    if (strcmp(player->class, "Mage") == 0) {
        printf("1. Firebolt (1d10 + %d damage, 1st-level slot)\n", magickaMod);
        printf("2. Icebolt (1d8 + %d to all enemies, 1st-level slot)\n", magickaMod);
        printf("3. Sparkingbolt (1d4 + %d damage + Stun, 1st-level slot)\n", magickaMod);
    } else if (strcmp(player->class, "Cleric") == 0) {
        printf("1. Heal (Heals 1d6 + %d HP, 1st-level slot)\n", magickaMod);
        printf("2. Sacred Fire (1d8 + %d damage, 1st-level slot)\n", magickaMod);
    }

    displaySpellSlots(player);
}

void displaySpellSlots(Character *player) {
    printf("\nSpell Slots Available:\n");
    for (int i = 0; i < 9; i++) {
        if (player->spellSlots[i] > 0) {
            printf("%d-level: %d slots\n", i + 1, player->spellSlots[i]);
        }
    }
}


void castSpell(Character *player, Enemy *enemy, Spell spell, int slotLevel) {
    if (strcmp(player->class, "Mage") != 0 && strcmp(player->class, "Cleric") != 0) {
        printf("\nYou cannot cast spells!\n");
        return;
    }

    slotLevel = validateSpellSlot(player, slotLevel);
    if (slotLevel == -1) {
        printf("\nYou have no valid spell slots available!\n");
        return;
    }

    player->spellSlots[slotLevel - 1]--;  // Consume a spell slot
    printf("\nCasting %s at level %d...\n", spell.name, slotLevel);

    int diceCount = (slotLevel > 1) ? slotLevel : 1;  // Scale dice based on slot level

    if (strcmp(spell.effect, "Damage") == 0) {
        int attackRoll = rollDice(20) + getModifier(player->magicka);
        printf("Attack Roll: %d (vs AC %d)\n", attackRoll, enemy->armorClass);
        if (attackRoll >= enemy->armorClass) {
            int damage = 0;
            for (int i = 0; i < diceCount; i++) {
                damage += rollDice(spell.damageDice);
            }
            damage += getModifier(player->magicka);
            enemy->hp -= damage;
            printf("The spell hits! %s dealt %d damage. Enemy HP: %d/%d\n", spell.name, damage, enemy->hp, enemy->maxHp);
        } else {
            printf("%s missed!\n", spell.name);
        }
    } else if (strcmp(spell.effect, "Heal") == 0) {
        int heal = 0;
        for (int i = 0; i < diceCount; i++) {
            heal += rollDice(spell.damageDice);
        }
        heal += getModifier(player->magicka);
        player->hp += heal;
        if (player->hp > player->maxHp) player->hp = player->maxHp;
        printf("You healed for %d HP. Current HP: %d/%d\n", heal, player->hp, player->maxHp);
    } else if (strcmp(spell.effect, "Stun") == 0) {
        int attackRoll = rollDice(20) + getModifier(player->magicka);
        printf("Attack Roll: %d (vs AC %d)\n", attackRoll, enemy->armorClass);
        if (attackRoll >= enemy->armorClass) {
            printf("The enemy is stunned and skips its next turn!\n");
            enemy->stunned = 1;
        } else {
            printf("The stun spell failed!\n");
        }
    }
}

int validateSpellSlot(Character *player, int requestedSlot) {
    for (int i = requestedSlot - 1; i >= 0; i--) {
        if (player->spellSlots[i] > 0) {
            return i + 1;
        }
    }
    return -1;  // No available slots
}
