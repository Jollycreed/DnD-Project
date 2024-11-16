#ifndef SPELL_H
#define SPELL_H

#include "character.h"
#include "enemy.h"

typedef struct {
    char name[30];
    int damageDice;
    char effect[20];  // Damage, Heal, Stun, etc.
} Spell;

// Function prototypes
void listSpells(Character *player);
void displaySpellSlots(Character *player);
void castSpell(Character *player, Enemy *enemy, Spell spell, int slotLevel);
Spell getSpellByClassAndChoice(Character *player, int spellChoice);
int validateSpellSlot(Character *player, int requestedSlot);
int hasAvailableSpellSlots(Character *player);

#endif
