#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct {
    char name[50];
    char race[20];
    char sex[10];
    char class[10];
    int level;
    int experience;
    int strength, constitution, dexterity, magicka;
    int hp, maxHp, armorClass;
    int hitDice, hitDiceValue;
    char inventory[5][30];
    char currentWeapon[30];
    int weaponDamage;
    int shieldBonus;
    int spellSlots[9];
    int extraAttack;  // Level 5 Warrior/Rogue
    int battlesWon;   // Tracks battles for boss spawn
    int movement;     // Base movement distance in feet
    int currentDistance; // Current distance from the enemy
} Character;


// Function prototypes
Character createCharacter();
void saveCharacterProgress(Character *player);
Character loadCharacterProgress();
void setupInventory(Character *player);
void addExperience(Character *player, int xp);
int getExperienceForNextLevel(int level);
void levelUp(Character *player);
void initializeSpellSlots(Character *player);  // Sets up spell slots based on class and level

#endif
