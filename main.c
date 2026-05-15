#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

// ============================================================================
// TYPEDEFS AND DATA STRUCTURES
// ============================================================================

typedef enum {
    ITEM_TYPE_POTION,
    ITEM_TYPE_WEAPON,
    ITEM_TYPE_ARMOR,
    ITEM_TYPE_MAGIC_ITEM,
    ITEM_TYPE_MISC,
    ITEM_TYPE_GOLD
} ItemType;

typedef struct {
    char name[50];
    ItemType type;
    int value;
    int effect;
} Item;

typedef struct {
    char name[30];
    int damageDice;
    char effect[20];
    int requiredLevel;
    int isCantrip;
} Spell;

typedef struct {
    char name[50];
    char race[20];
    char sex[10];
    char class[20];
    int level;
    int experience;
    int strength, constitution, dexterity, intelligence, wisdom, charisma;
    int hp, maxHp, armorClass;
    int hitDice, hitDiceValue;
    char currentWeapon[30];
    int weaponDamage;
    int shieldBonus;
    int spellSlots[10];
    int extraAttack;
    int battlesWon;
    int hardcore;
    int actionPoints;
    int ragePoints;
    int buffed;
    int buffDuration;
    int kiPoints;
    int maxKiPoints;
    int isSpellcaster;
    Item inventory[50];
    int inventoryCount;
    int stunned;
} Character;

typedef struct {
    char name[50];
    int hp, maxHp;
    int armorClass;
    int attackBonus;
    int damageDice;
    int strength, dexterity, constitution;
    int intelligence, wisdom, charisma;
    int stunned;
    int xpReward;
    int spellSlots[9];
    int isSpellcaster;
} Enemy;

// ============================================================================
// SPELL DEFINITIONS
// ============================================================================

Spell druidSpells[] = {
    {"Shillelagh", 0, "Buff", 1, 1},
    {"Guidance", 0, "Buff", 1, 1},
    {"Produce Flame", 6, "Damage", 1, 1},
    {"Entangle", 0, "Control", 1, 0},
    {"Cure Wounds", 8, "Heal", 1, 0},
    {"Moonbeam", 10, "Damage", 3, 0},
    {"Flaming Sphere", 10, "Damage", 3, 0},
    {"Call Lightning", 10, "Damage", 5, 0},
    {"Water Breathing", 0, "Utility", 5, 0},
    {"Guardian of Nature", 0, "Buff", 7, 0},
    {"Polymorph", 0, "Control", 7, 0},
    {"Wall of Thorns", 12, "Control", 9, 0},
    {"Heal", 15, "Heal", 9, 0},
    {"Sunbeam", 10, "Damage", 11, 0},
    {"Foresight", 0, "Buff", 17, 0}
};

Spell rangerSpells[] = {
    {"Guided Arrow", 8, "Damage", 1, 1},
    {"Hunter's Trick", 0, "Utility", 1, 1},
    {"Hunter's Mark", 6, "Damage", 1, 0},
    {"Cure Wounds", 8, "Heal", 1, 0},
    {"Spike Growth", 0, "Control", 3, 0},
    {"Pass Without Trace", 0, "Utility", 3, 0},
    {"Conjure Barrage", 8, "Damage", 5, 0},
    {"Freedom of Movement", 0, "Utility", 7, 0},
    {"Conjure Volley", 8, "Damage", 9, 0},
    {"Swift Quiver", 0, "Buff", 11, 0}
};

Spell paladinSpells[] = {
    {"Bless", 0, "Buff", 1, 0},
    {"Divine Smite", 8, "Damage", 2, 0},
    {"Lay on Hands", 10, "Heal", 3, 0},
    {"Protection from Evil", 0, "Buff", 3, 0},
    {"Spirit Guardians", 8, "Damage", 5, 0},
    {"Aura of Vitality", 10, "Heal", 7, 0},
    {"Banishing Smite", 10, "Damage", 11, 0},
    {"Circle of Power", 0, "Buff", 13, 0}
};

Spell bardSpells[] = {
    {"Vicious Mockery", 4, "Damage", 1, 1},
    {"Inspire Courage", 0, "Buff", 1, 1},
    {"Healing Word", 4, "Heal", 1, 0},
    {"Charm Person", 0, "Control", 1, 0},
    {"Silence", 0, "Control", 3, 0},
    {"Heat Metal", 8, "Damage", 3, 0},
    {"Hypnotic Pattern", 0, "Control", 5, 0},
    {"Greater Invisibility", 0, "Buff", 7, 0},
    {"Mass Cure Wounds", 10, "Heal", 9, 0}
};

Spell sorcererSpells[] = {
    {"Fire Bolt", 8, "Damage", 1, 1},
    {"Prestidigitation", 0, "Utility", 1, 1},
    {"Magic Missile", 4, "Damage", 1, 0},
    {"Shield", 0, "Buff", 1, 0},
    {"Burning Hands", 8, "Damage", 3, 0},
    {"Mirror Image", 0, "Buff", 3, 0},
    {"Counterspell", 0, "Utility", 5, 0},
    {"Fireball", 8, "Damage", 5, 0},
    {"Greater Polymorph", 0, "Control", 7, 0},
    {"Wish", 0, "Utility", 17, 0}
};

Spell wizardSpells[] = {
    {"Mage Hand", 0, "Utility", 1, 1},
    {"Ray of Frost", 8, "Damage", 1, 1},
    {"Magic Missile", 4, "Damage", 1, 0},
    {"Mage Armor", 0, "Buff", 1, 0},
    {"Misty Step", 0, "Utility", 3, 0},
    {"Scorching Ray", 6, "Damage", 3, 0},
    {"Fireball", 8, "Damage", 5, 0},
    {"Counterspell", 0, "Utility", 5, 0},
    {"Dimension Door", 0, "Utility", 7, 0},
    {"Meteor Swarm", 20, "Damage", 19, 0}
};

Spell clericSpells[] = {
    {"Sacred Flame", 8, "Damage", 1, 1},
    {"Light", 0, "Utility", 1, 1},
    {"Cure Wounds", 8, "Heal", 1, 0},
    {"Bless", 0, "Buff", 1, 0},
    {"Spiritual Weapon", 8, "Damage", 3, 0},
    {"Hold Person", 0, "Control", 3, 0},
    {"Revivify", 0, "Heal", 5, 0},
    {"Guardian of Faith", 10, "Damage", 7, 0}
};

Spell warlockSpells[] = {
    {"Eldritch Blast", 10, "Damage", 1, 1},
    {"Mage Hand", 0, "Utility", 1, 1},
    {"Hex", 0, "Debuff", 1, 0},
    {"Hellish Rebuke", 8, "Damage", 1, 0},
    {"Darkness", 0, "Control", 3, 0},
    {"Misty Step", 0, "Utility", 3, 0},
    {"Counterspell", 0, "Utility", 5, 0},
    {"Hunger of Hadar", 8, "Damage", 9, 0}
};

Spell monkAbilities[] = {
    {"Flurry of Blows", 0, "Ability", 1, 0},
    {"Stunning Strike", 0, "Ability", 3, 0},
    {"Patient Defense", 0, "Buff", 5, 0},
    {"Step of the Wind", 0, "Utility", 7, 0},
    {"Empty Body", 0, "Buff", 18, 0}
};

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

void initializeEnemyStats(Enemy *enemy, int hpDice, int hpMultiplier, int playerLevel);
void setupInventory(Character *player);
int getHitDiceValue(const char *class);
int rollInitiative(Character *player, Enemy *enemy);
Spell chooseEnemySpell(Enemy *enemy);
void castSpellEnemy(Enemy *enemy, Character *player, Spell spell);
void generateLoot(Enemy *enemy, Item *loot[], int *lootCount, int playerLevel);
void displayLoot(Item *loot[], int lootCount);
void initializeWarlockSlots(Character *player);
void initializeKiPoints(Character *player);
void initializeFullCasterSlots(Character *player);
void initializeHalfCasterSlots(Character *player);
void shortRest(Character *player);
void longRest(Character *player);
void removeBuff(Character *player);
void listSpells(Character *player);
void displaySpellSlots(Character *player);
void castSpell(Character *player, Enemy *enemy, int slotLevel);
Spell getSpellByClassAndChoice(Character *player, int spellChoice);
int validateSpellSlot(Character *player, int requestedSlot);
int hasAvailableSpellSlots(Character *player);
Spell *getSpellsForClass(const char *className, int *spellCount, int characterLevel);
void useActionPoint(Character *player, const char *ability);
void useRage(Character *player);
int hasAvailableActionPoints(Character *player);
Character createCharacter();
void saveCharacterProgress(Character *player);
Character loadCharacterByName(const char *fileName);
void addExperience(Character *player, int xp);
int getExperienceForNextLevel(int level);
void levelUp(Character *player);
void initializeSpellSlots(Character *player);
void applyRaceModifiers(Character *player);
void setupClass(Character *player);
void assignBonusStats(Character *player, int count);
void initializeCharacterStats(Character *player);
void addItemToInventory(Character *player, Item *item);
void showInventory(Character *player);
void useItem(Character *player, ItemType type);
void listSavedCharacters();
void deleteSaveFile(const char *fileName);
int rollDice(int sides);
int getModifier(int stat);
int getProficiencyBonus(int level);
int calculateSpellSaveDC(int castingStat, int level);
Enemy createEnemy(int playerLevel);
Enemy createBoss(int playerLevel, const char *bossName);
void enemyTurn(Enemy *enemy, Character *player);
void handleEnemySpellcasting(Enemy *enemy, Character *player);
void startBattle(Character *player, Enemy *enemy);
void playerAttack(Character *player, Enemy *enemy);
void enemyAttack(Enemy *enemy, Character *player);
void useHealingPotion(Character *player);
void handleBattleEnd(Character *player, Enemy *enemy);
void applyBuff(Character *player, const char *buffName);
void manageBuffs(Character *player);
void flurryOfBlows(Character *player, Enemy *enemy);
void stunningStrike(Character *player, Enemy *enemy);
void clearScreen();

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void clearScreen() {
    system("clear || cls");
}

int rollDice(int sides) {
    return (rand() % sides) + 1;
}

int getModifier(int stat) {
    return (stat - 10) / 2;
}

int getProficiencyBonus(int level) {
    if (level <= 4) return 2;
    else if (level <= 8) return 3;
    else if (level <= 12) return 4;
    else if (level <= 16) return 5;
    else return 6;
}

int calculateSpellSaveDC(int castingStat, int level) {
    return 8 + getProficiencyBonus(level) + getModifier(castingStat);
}

void safe_input(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

// ============================================================================
// CHARACTER CREATION AND MANAGEMENT
// ============================================================================

Character createCharacter() {
    Character newChar;
    memset(&newChar, 0, sizeof(Character));
    newChar.level = 1;

    printf("Enter your character's name: ");
    safe_input(newChar.name, sizeof(newChar.name));

    printf("Choose your sex (Male/Female/Other): ");
    safe_input(newChar.sex, sizeof(newChar.sex));

    printf("Choose your race (Human, Elf, Half-Elf, Drow, Dwarf, Tiefling, Halfling, Gnome, Half-Orc, Dragonborn):\n");
    safe_input(newChar.race, sizeof(newChar.race));

    printf("Choose your class (Fighter, Paladin, Barbarian, Ranger, Rogue, Wizard, Sorcerer, Warlock, Cleric, Bard, Druid, Monk):\n");
    safe_input(newChar.class, sizeof(newChar.class));

    printf("Distribute your stats using the standard array (15, 14, 13, 12, 10, 8):\n");

    int standardArray[] = {15, 14, 13, 12, 10, 8};
    const char *statNames[] = {"Strength", "Constitution", "Dexterity", "Intelligence", "Wisdom", "Charisma"};
    int *statPointer[] = { &newChar.strength, &newChar.constitution, &newChar.dexterity,
                           &newChar.intelligence, &newChar.wisdom, &newChar.charisma };
    int used[6] = {0};

    for (int i = 0; i < 6; i++) {
        int statChoice, value;
        do {
            printf("Assign value from the array to %s: ", statNames[i]);
            scanf("%d", &value);
            getchar();
            for (statChoice = 0; statChoice < 6; statChoice++) {
                if (standardArray[statChoice] == value && !used[statChoice]) {
                    used[statChoice] = 1;
                    *statPointer[i] = value;
                    break;
                }
            }
            if (statChoice == 6) {
                printf("Invalid or already used value. Try again.\n");
            }
        } while (statChoice == 6);
    }

    applyRaceModifiers(&newChar);
    setupClass(&newChar);
    newChar.maxHp = newChar.hitDiceValue + getModifier(newChar.constitution);
    newChar.hp = newChar.maxHp;
    setupInventory(&newChar);

    printf("Do you want to enable Hardcore Mode? (1 for Yes, 0 for No): ");
    int hardcoreChoice;
    scanf("%d", &hardcoreChoice);
    getchar();
    newChar.hardcore = (hardcoreChoice == 1);

    printf("Character created successfully!\n");
    return newChar;
}

void applyRaceModifiers(Character *player) {
    if (strcmp(player->race, "Human") == 0) {
        player->strength++; player->constitution++; player->dexterity++;
        player->intelligence++; player->wisdom++; player->charisma++;
    } else if (strcmp(player->race, "Elf") == 0 || strcmp(player->race, "Drow") == 0) {
        player->dexterity += 2;
        player->intelligence++;
    } else if (strcmp(player->race, "Half-Elf") == 0) {
        player->charisma += 2;
        printf("Choose two stats to increase by +1:\n");
        assignBonusStats(player, 2);
    } else if (strcmp(player->race, "Dwarf") == 0) {
        player->constitution += 2;
        player->intelligence++;
    } else if (strcmp(player->race, "Tiefling") == 0) {
        player->charisma += 2;
        player->intelligence++;
    } else if (strcmp(player->race, "Halfling") == 0) {
        player->dexterity += 2;
        player->wisdom++;
    } else if (strcmp(player->race, "Gnome") == 0) {
        player->intelligence += 2;
        player->charisma++;
    } else if (strcmp(player->race, "Half-Orc") == 0) {
        player->strength += 2;
        player->constitution++;
    } else if (strcmp(player->race, "Dragonborn") == 0) {
        printf("Choose stat increases: (a) +2 to one and +1 to another or (b) +1 to three stats.\n");
        char option;
        scanf("%c", &option);
        getchar();
        if (option == 'a') {
            assignBonusStats(player, 2);
            assignBonusStats(player, 1);
        } else {
            assignBonusStats(player, 1);
            assignBonusStats(player, 1);
            assignBonusStats(player, 1);
        }
    }
}

void assignBonusStats(Character *player, int count) {
    for (int i = 0; i < count; i++) {
        char stat[20];
        printf("Choose stat to increase (Strength, Constitution, Dexterity, Intelligence, Wisdom, Charisma): ");
        safe_input(stat, sizeof(stat));
        if (strcmp(stat, "Strength") == 0) player->strength++;
        else if (strcmp(stat, "Constitution") == 0) player->constitution++;
        else if (strcmp(stat, "Dexterity") == 0) player->dexterity++;
        else if (strcmp(stat, "Intelligence") == 0) player->intelligence++;
        else if (strcmp(stat, "Wisdom") == 0) player->wisdom++;
        else if (strcmp(stat, "Charisma") == 0) player->charisma++;
        else {
            printf("Invalid stat. Try again.\n");
            i--;
        }
    }
}

int getHitDiceValue(const char *class) {
    if (strcmp(class, "Barbarian") == 0) return 12;
    if (strcmp(class, "Fighter") == 0 || strcmp(class, "Paladin") == 0 || strcmp(class, "Ranger") == 0) return 10;
    if (strcmp(class, "Rogue") == 0 || strcmp(class, "Cleric") == 0 || strcmp(class, "Warlock") == 0 ||
        strcmp(class, "Bard") == 0 || strcmp(class, "Monk") == 0 || strcmp(class, "Druid") == 0) return 8;
    return 6;
}

void setupClass(Character *player) {
    player->hitDiceValue = getHitDiceValue(player->class);

    if (strcmp(player->class, "Wizard") == 0 ||
        strcmp(player->class, "Cleric") == 0 ||
        strcmp(player->class, "Sorcerer") == 0 ||
        strcmp(player->class, "Bard") == 0 ||
        strcmp(player->class, "Druid") == 0) {
        player->isSpellcaster = 1;
        initializeFullCasterSlots(player);
    }
    else if (strcmp(player->class, "Paladin") == 0 ||
             strcmp(player->class, "Ranger") == 0) {
        player->isSpellcaster = 1;
        initializeHalfCasterSlots(player);
    }
    else if (strcmp(player->class, "Warlock") == 0) {
        player->isSpellcaster = 1;
        initializeWarlockSlots(player);
    }
    else {
        player->isSpellcaster = 0;
    }

    if (strcmp(player->class, "Monk") == 0) {
        initializeKiPoints(player);
    }
}

void initializeCharacterStats(Character *player) {
    player->hp = player->maxHp;
    player->actionPoints = 2;
    player->ragePoints = 0;
    player->kiPoints = player->maxKiPoints;
    player->stunned = 0;
    printf("Character stats initialized for %s.\n", player->name);
}

void setupInventory(Character *player) {
    player->inventoryCount = 0;

    if (strcmp(player->class, "Fighter") == 0) {
        int weaponChoice;
        printf("\nChoose your weapon:\n1. Two-Handed Sword (2d6 damage)\n2. Sword (1d6 damage) and Shield (+2 AC)\n");
        scanf("%d", &weaponChoice);
        getchar();
        player->weaponDamage = (weaponChoice == 1) ? 12 : 6;
        player->shieldBonus = (weaponChoice == 2) ? 2 : 0;
        player->armorClass = 13 + getModifier(player->dexterity) + player->shieldBonus;
    } else if (strcmp(player->class, "Paladin") == 0) {
        printf("\nChoose your weapon:\n1. Mace (1d8 damage)\n2. Sword (1d6 damage) and Shield (+2 AC)\n");
        int weaponChoice;
        scanf("%d", &weaponChoice);
        getchar();
        if (weaponChoice == 1) {
            player->weaponDamage = 8;
            player->shieldBonus = 0;
        } else {
            player->weaponDamage = 6;
            player->shieldBonus = 2;
        }
        player->armorClass = 16 + player->shieldBonus;
    } else if (strcmp(player->class, "Ranger") == 0) {
        printf("\nChoose your weapon:\n1. Two Daggers (1d4 each)\n2. Longsword (1d8) and Bow (1d6)\n");
        int weaponChoice;
        scanf("%d", &weaponChoice);
        getchar();
        player->weaponDamage = (weaponChoice == 1) ? 8 : 8;
        player->armorClass = 12 + getModifier(player->dexterity);
    } else if (strcmp(player->class, "Wizard") == 0 || strcmp(player->class, "Sorcerer") == 0) {
        player->weaponDamage = 6;
        player->armorClass = 10 + getModifier(player->dexterity);
    } else if (strcmp(player->class, "Monk") == 0) {
        printf("\nChoose your weapon:\n1. Quarterstaff (1d8 damage)\n2. Hand to Hand (2d4 damage)\n");
        int weaponChoice;
        scanf("%d", &weaponChoice);
        getchar();
        strcpy(player->currentWeapon, (weaponChoice == 1) ? "Quarterstaff" : "Hand to Hand");
        player->weaponDamage = 8;
        player->armorClass = 10 + getModifier(player->dexterity) + getModifier(player->wisdom);
    } else if (strcmp(player->class, "Warlock") == 0) {
        player->weaponDamage = 6;
        player->armorClass = 10 + getModifier(player->dexterity);
    } else if (strcmp(player->class, "Bard") == 0) {
        player->weaponDamage = 8;
        player->armorClass = 10 + getModifier(player->dexterity);
    } else if (strcmp(player->class, "Cleric") == 0) {
        player->weaponDamage = 8;
        player->shieldBonus = 2;
        player->armorClass = 13 + getModifier(player->dexterity) + player->shieldBonus;
    } else if (strcmp(player->class, "Druid") == 0) {
        printf("\nChoose your weapon:\n1. Quarterstaff (1d6 damage)\n2. Dagger (1d4) and Wooden Shield (+1 AC)\n");
        int weaponChoice;
        scanf("%d", &weaponChoice);
        getchar();
        if (weaponChoice == 1) {
            player->weaponDamage = 6;
            player->shieldBonus = 0;
        } else {
            player->weaponDamage = 4;
            player->shieldBonus = 1;
        }
        player->armorClass = 10 + getModifier(player->dexterity) + player->shieldBonus;
    } else if (strcmp(player->class, "Rogue") == 0) {
        printf("\nChoose your weapon:\n1. Two Daggers (1d4 each)\n2. Bow (1d6 damage)\n");
        int weaponChoice;
        scanf("%d", &weaponChoice);
        getchar();
        player->weaponDamage = (weaponChoice == 1) ? 8 : 6;
        player->armorClass = 10 + (2 * getModifier(player->dexterity));
    } else if (strcmp(player->class, "Barbarian") == 0) {
        printf("\nChoose your weapon:\n1. Greataxe (1d12 damage)\n2. Dual Handaxes (1d6 each)\n");
        int weaponChoice;
        scanf("%d", &weaponChoice);
        getchar();
        player->weaponDamage = (weaponChoice == 1) ? 12 : 6;
        player->armorClass = 10 + getModifier(player->dexterity) + getModifier(player->constitution);
    }

    player->maxHp = rollDice(player->hitDiceValue) + getModifier(player->constitution);
    player->hp = player->maxHp;

    // Add starting potion
    Item potion;
    strcpy(potion.name, "Health Potion");
    potion.type = ITEM_TYPE_POTION;
    potion.value = 50;
    potion.effect = 10;
    addItemToInventory(player, &potion);
}

// ============================================================================
// SPELL AND ABILITY MANAGEMENT
// ============================================================================

Spell *getSpellsForClass(const char *className, int *spellCount, int characterLevel) {
    static Spell availableSpells[50];
    int count = 0;

    Spell *allSpells = NULL;
    int totalSpells = 0;

    if (strcmp(className, "Druid") == 0) {
        allSpells = druidSpells;
        totalSpells = sizeof(druidSpells) / sizeof(druidSpells[0]);
    } else if (strcmp(className, "Ranger") == 0) {
        allSpells = rangerSpells;
        totalSpells = sizeof(rangerSpells) / sizeof(rangerSpells[0]);
    } else if (strcmp(className, "Paladin") == 0) {
        allSpells = paladinSpells;
        totalSpells = sizeof(paladinSpells) / sizeof(paladinSpells[0]);
    } else if (strcmp(className, "Bard") == 0) {
        allSpells = bardSpells;
        totalSpells = sizeof(bardSpells) / sizeof(bardSpells[0]);
    } else if (strcmp(className, "Sorcerer") == 0) {
        allSpells = sorcererSpells;
        totalSpells = sizeof(sorcererSpells) / sizeof(sorcererSpells[0]);
    } else if (strcmp(className, "Wizard") == 0) {
        allSpells = wizardSpells;
        totalSpells = sizeof(wizardSpells) / sizeof(wizardSpells[0]);
    } else if (strcmp(className, "Cleric") == 0) {
        allSpells = clericSpells;
        totalSpells = sizeof(clericSpells) / sizeof(clericSpells[0]);
    } else if (strcmp(className, "Warlock") == 0) {
        allSpells = warlockSpells;
        totalSpells = sizeof(warlockSpells) / sizeof(warlockSpells[0]);
    } else if (strcmp(className, "Monk") == 0) {
        allSpells = monkAbilities;
        totalSpells = sizeof(monkAbilities) / sizeof(monkAbilities[0]);
    }

    for (int i = 0; i < totalSpells; i++) {
        if (allSpells[i].requiredLevel <= characterLevel) {
            availableSpells[count++] = allSpells[i];
        }
    }

    *spellCount = count;
    return availableSpells;
}

void listSpells(Character *player) {
    int spellCount;
    Spell *spells = getSpellsForClass(player->class, &spellCount, player->level);

    if (spellCount == 0) {
        printf("No spells available for %s.\n", player->class);
        return;
    }

    printf("\nAvailable spells for %s:\n", player->class);
    for (int i = 0; i < spellCount; i++) {
        printf("%d. %s (%s)\n", i + 1, spells[i].name, spells[i].effect);
    }
}

Spell getSpellByClassAndChoice(Character *player, int spellChoice) {
    int spellCount;
    Spell *spells = getSpellsForClass(player->class, &spellCount, player->level);

    if (spellChoice >= 1 && spellChoice <= spellCount) {
        return spells[spellChoice - 1];
    }

    Spell invalid;
    strcpy(invalid.name, "Invalid");
    return invalid;
}

int validateSpellSlot(Character *player, int requestedSlot) {
    if (requestedSlot >= 1 && requestedSlot <= 9 && player->spellSlots[requestedSlot - 1] > 0) {
        return requestedSlot;
    }
    return -1;
}

int hasAvailableSpellSlots(Character *player) {
    for (int i = 0; i < 10; i++) {
        if (player->spellSlots[i] > 0) return 1;
    }
    return 0;
}

void displaySpellSlots(Character *player) {
    printf("\nSpell Slots:\n");
    for (int i = 0; i < 9; i++) {
        if (player->spellSlots[i] > 0) {
            printf("Level %d: %d slots\n", i + 1, player->spellSlots[i]);
        }
    }
}

void castSpell(Character *player, Enemy *enemy, int slotLevel) {
    if (slotLevel < 1 || slotLevel > 9) {
        printf("Invalid slot level.\n");
        return;
    }

    if (player->spellSlots[slotLevel - 1] <= 0) {
        printf("No level %d spell slots available.\n", slotLevel);
        return;
    }

    listSpells(player);
    printf("Choose a spell (1-based number): ");
    int choice;
    scanf("%d", &choice);
    getchar();

    Spell chosenSpell = getSpellByClassAndChoice(player, choice);
    if (strcmp(chosenSpell.name, "Invalid") == 0) {
        printf("Invalid spell choice.\n");
        return;
    }

    printf("%s casts %s!\n", player->name, chosenSpell.name);
    player->spellSlots[slotLevel - 1]--;

    if (strcmp(chosenSpell.effect, "Damage") == 0) {
        int damage = rollDice(chosenSpell.damageDice) + getModifier(player->intelligence);
        enemy->hp -= damage;
        if (enemy->hp < 0) enemy->hp = 0;
        printf("Dealt %d damage! Enemy HP: %d/%d\n", damage, enemy->hp, enemy->maxHp);
    } else if (strcmp(chosenSpell.effect, "Heal") == 0) {
        int heal = rollDice(chosenSpell.damageDice) + getModifier(player->wisdom);
        player->hp += heal;
        if (player->hp > player->maxHp) player->hp = player->maxHp;
        printf("Healed for %d HP! Current HP: %d/%d\n", heal, player->hp, player->maxHp);
    } else if (strcmp(chosenSpell.effect, "Buff") == 0) {
        printf("You feel empowered!\n");
        player->buffed = 1;
    } else if (strcmp(chosenSpell.effect, "Debuff") == 0) {
        printf("The enemy is weakened!\n");
        enemy->stunned = 1;
    } else if (strcmp(chosenSpell.effect, "Control") == 0) {
        printf("Battlefield controlled!\n");
        enemy->stunned = 1;
    }
}

void initializeFullCasterSlots(Character *player) {
    for (int i = 0; i < 9; i++) player->spellSlots[i] = 0;

    switch (player->level) {
        case 1: player->spellSlots[0] = 2; break;
        case 2: player->spellSlots[0] = 3; break;
        case 3: player->spellSlots[0] = 4; player->spellSlots[1] = 2; break;
        case 4: player->spellSlots[0] = 4; player->spellSlots[1] = 3; break;
        case 5: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 2; break;
        case 6:
        case 7: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; break;
        case 8:
        case 9: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 1; break;
        case 10:
        case 11: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 2; break;
        case 12:
        case 13: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 1; break;
        case 14:
        case 15: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 2; break;
        case 16:
        case 17: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 3; player->spellSlots[5] = 1; break;
        case 18:
        case 19: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 3; player->spellSlots[5] = 2; player->spellSlots[6] = 1; break;
        case 20: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 3; player->spellSlots[5] = 2; player->spellSlots[6] = 2; player->spellSlots[7] = 1; player->spellSlots[8] = 1; break;
    }
}

void initializeHalfCasterSlots(Character *player) {
    for (int i = 0; i < 9; i++) player->spellSlots[i] = 0;

    switch (player->level) {
        case 1: player->spellSlots[0] = 2; break;
        case 2: player->spellSlots[0] = 3; break;
        case 3:
        case 4: player->spellSlots[0] = 4; break;
        case 5:
        case 6: player->spellSlots[0] = 4; player->spellSlots[1] = 2; break;
        case 7:
        case 8: player->spellSlots[0] = 4; player->spellSlots[1] = 3; break;
        case 9:
        case 10: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 2; break;
        case 11:
        case 12: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; break;
        case 13:
        case 14: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 1; break;
        case 15:
        case 16: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 2; break;
        case 17:
        case 18: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 1; break;
        case 19:
        case 20: player->spellSlots[0] = 4; player->spellSlots[1] = 3; player->spellSlots[2] = 3; player->spellSlots[3] = 3; player->spellSlots[4] = 2; break;
    }
}

void initializeWarlockSlots(Character *player) {
    for (int i = 0; i < 9; i++) player->spellSlots[i] = 0;

    if (player->level >= 1) player->spellSlots[0] = 1;
    if (player->level >= 2) player->spellSlots[0] = 2;
    if (player->level >= 3) player->spellSlots[1] = 2;
    if (player->level >= 5) player->spellSlots[2] = 2;
    if (player->level >= 7) player->spellSlots[3] = 2;
    if (player->level >= 9) player->spellSlots[4] = 2;
}

void initializeKiPoints(Character *player) {
    if (strcmp(player->class, "Monk") == 0) {
        player->maxKiPoints = player->level;
        player->kiPoints = player->maxKiPoints;
    }
}

void flurryOfBlows(Character *player, Enemy *enemy) {
    if (player->kiPoints < 1) {
        printf("Not enough Ki Points for Flurry of Blows.\n");
        return;
    }

    player->kiPoints--;
    printf("Using Flurry of Blows! Remaining Ki: %d\n", player->kiPoints);

    for (int i = 0; i < 2; i++) {
        int attackRoll = rollDice(20) + getModifier(player->dexterity);
        if (attackRoll >= enemy->armorClass) {
            int damage = rollDice(4) + getModifier(player->dexterity);
            enemy->hp -= damage;
            printf("Hit! Dealt %d damage.\n", damage);
        } else {
            printf("Missed!\n");
        }
    }
}

void stunningStrike(Character *player, Enemy *enemy) {
    if (player->kiPoints < 1) {
        printf("Not enough Ki Points.\n");
        return;
    }

    player->kiPoints--;
    int saveRoll = rollDice(20) + getModifier(enemy->constitution);
    int saveDC = 8 + getProficiencyBonus(player->level) + getModifier(player->wisdom);

    if (saveRoll < saveDC) {
        enemy->stunned = 1;
        printf("Enemy is stunned!\n");
    } else {
        printf("Enemy resisted!\n");
    }
}

// ============================================================================
// EXPERIENCE AND LEVELING
// ============================================================================

int getExperienceForNextLevel(int level) {
    int expTable[] = {
        0, 300, 900, 2700, 6500, 14000, 23000, 34000,
        48000, 64000, 85000, 100000, 120000, 140000,
        165000, 195000, 225000, 265000, 305000, 355000
    };
    return (level < 20) ? expTable[level] : expTable[19];
}

void addExperience(Character *player, int xp) {
    player->experience += xp;
    printf("You gained %d XP!\n", xp);

    while (player->level < 20 && player->experience >= getExperienceForNextLevel(player->level + 1)) {
        levelUp(player);
    }
}

void levelUp(Character *player) {
    player->level++;
    printf("\n*** Level Up! You are now level %d! ***\n", player->level);

    int hpGain = rollDice(player->hitDiceValue) + getModifier(player->constitution);
    player->maxHp += (hpGain > 0) ? hpGain : 1;
    player->hp = player->maxHp;

    if (player->level == 5) {
        if (strcmp(player->class, "Fighter") == 0 || strcmp(player->class, "Monk") == 0 ||
            strcmp(player->class, "Paladin") == 0 || strcmp(player->class, "Ranger") == 0 ||
            strcmp(player->class, "Barbarian") == 0) {
            player->extraAttack = 1;
            printf("You gained Extra Attack!\n");
        }
    }

    if (player->isSpellcaster) {
        initializeSpellSlots(player);
    }

    if (strcmp(player->class, "Monk") == 0) {
        initializeKiPoints(player);
    }
}

void initializeSpellSlots(Character *player) {
    if (strcmp(player->class, "Wizard") == 0 || strcmp(player->class, "Cleric") == 0 || strcmp(player->class, "Druid") == 0) {
        initializeFullCasterSlots(player);
    } else if (strcmp(player->class, "Paladin") == 0 || strcmp(player->class, "Ranger") == 0 || strcmp(player->class, "Bard") == 0) {
        initializeHalfCasterSlots(player);
    } else if (strcmp(player->class, "Warlock") == 0) {
        initializeWarlockSlots(player);
    }
}

// ============================================================================
// SAVE/LOAD SYSTEM
// ============================================================================

void saveCharacterProgress(Character *player) {
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "%s.txt", player->name);
    FILE *file = fopen(fileName, "w");
    if (!file) {
        printf("Error saving character.\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|",
            player->name, player->race, player->sex, player->class,
            player->level, player->experience,
            player->strength, player->constitution, player->dexterity,
            player->intelligence, player->wisdom, player->charisma,
            player->hp, player->maxHp, player->armorClass,
            player->weaponDamage, player->hitDice, player->hitDiceValue);

    for (int i = 0; i < 9; i++) {
        fprintf(file, "%d", player->spellSlots[i]);
        if (i < 8) fprintf(file, "|");
    }
    fprintf(file, "|");

    fprintf(file, "%d|%d|%d|%d|", player->kiPoints, player->maxKiPoints, player->hardcore, player->battlesWon);

    fprintf(file, "%d|", player->inventoryCount);
    for (int i = 0; i < player->inventoryCount; i++) {
        fprintf(file, "%s:%d:%d:%d|", player->inventory[i].name, player->inventory[i].type,
                player->inventory[i].value, player->inventory[i].effect);
    }

    fclose(file);
    printf("Character saved.\n");
}

Character loadCharacterByName(const char *fileName) {
    Character c;
    memset(&c, 0, sizeof(Character));

    FILE *file = fopen(fileName, "r");
    if (!file) {
        printf("Error loading character.\n");
        exit(1);
    }

    char buffer[2048];
    if (!fgets(buffer, sizeof(buffer), file)) {
        fclose(file);
        exit(1);
    }
    fclose(file);

    char *token = strtok(buffer, "|");
    if (!token) exit(1);
    strcpy(c.name, token);

    token = strtok(NULL, "|");
    strcpy(c.race, token);
    token = strtok(NULL, "|");
    strcpy(c.sex, token);
    token = strtok(NULL, "|");
    strcpy(c.class, token);

    token = strtok(NULL, "|");
    c.level = atoi(token);
    token = strtok(NULL, "|");
    c.experience = atoi(token);

    token = strtok(NULL, "|");
    c.strength = atoi(token);
    token = strtok(NULL, "|");
    c.constitution = atoi(token);
    token = strtok(NULL, "|");
    c.dexterity = atoi(token);
    token = strtok(NULL, "|");
    c.intelligence = atoi(token);
    token = strtok(NULL, "|");
    c.wisdom = atoi(token);
    token = strtok(NULL, "|");
    c.charisma = atoi(token);

    token = strtok(NULL, "|");
    c.hp = atoi(token);
    token = strtok(NULL, "|");
    c.maxHp = atoi(token);
    token = strtok(NULL, "|");
    c.armorClass = atoi(token);
    token = strtok(NULL, "|");
    c.weaponDamage = atoi(token);
    token = strtok(NULL, "|");
    c.hitDice = atoi(token);
    token = strtok(NULL, "|");
    c.hitDiceValue = atoi(token);

    for (int i = 0; i < 9; i++) {
        token = strtok(NULL, "|");
        c.spellSlots[i] = atoi(token);
    }

    token = strtok(NULL, "|");
    c.kiPoints = atoi(token);
    token = strtok(NULL, "|");
    c.maxKiPoints = atoi(token);
    token = strtok(NULL, "|");
    c.hardcore = atoi(token);
    token = strtok(NULL, "|");
    c.battlesWon = atoi(token);

    token = strtok(NULL, "|");
    c.inventoryCount = atoi(token);

    for (int i = 0; i < c.inventoryCount; i++) {
        token = strtok(NULL, "|");
        if (!token) break;

        char tempName[50], tempStr[20];
        sscanf(token, "%[^:]:%[^:]:%d:%d", tempName, tempStr, &c.inventory[i].value, &c.inventory[i].effect);
        strcpy(c.inventory[i].name, tempName);
        c.inventory[i].type = atoi(tempStr);
    }

    return c;
}

void listSavedCharacters() {
    printf("\n=== Saved Characters ===\n");
    DIR *dp = opendir(".");
    if (!dp) return;

    struct dirent *entry;
    int found = 0;
    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".txt")) {
            found = 1;
            char characterName[50];
            sscanf(entry->d_name, "%[^.]", characterName);
            FILE *file = fopen(entry->d_name, "r");
            if (file) {
                char buffer[500];
                if (fgets(buffer, sizeof(buffer), file)) {
                    char *token = strtok(buffer, "|");
                    strtok(NULL, "|");
                    strtok(NULL, "|");
                    char *classStr = strtok(NULL, "|");
                    token = strtok(NULL, "|");
                    int level = atoi(token);
                    printf("%-20s | Class: %-10s | Level: %d\n", characterName, classStr, level);
                }
                fclose(file);
            }
        }
    }
    closedir(dp);
    if (!found) printf("No saved characters found.\n");
}

void deleteSaveFile(const char *fileName) {
    char fullName[100];
    snprintf(fullName, sizeof(fullName), "%s.txt", fileName);
    if (remove(fullName) == 0) {
        printf("Save file deleted.\n");
    }
}

// ============================================================================
// INVENTORY MANAGEMENT
// ============================================================================

void addItemToInventory(Character *player, Item *item) {
    if (player->inventoryCount < 50) {
        player->inventory[player->inventoryCount++] = *item;
        printf("Added %s to inventory.\n", item->name);
    } else {
        printf("Inventory full!\n");
    }
}

void showInventory(Character *player) {
    printf("\n=== Inventory ===\n");
    if (player->inventoryCount == 0) {
        printf("Empty inventory.\n");
        return;
    }
    for (int i = 0; i < player->inventoryCount; i++) {
        printf("%d. %s (Value: %d)\n", i + 1, player->inventory[i].name, player->inventory[i].value);
    }
}

void useItem(Character *player, ItemType type) {
    for (int i = 0; i < player->inventoryCount; i++) {
        if (player->inventory[i].type == type) {
            if (type == ITEM_TYPE_POTION) {
                player->hp += player->inventory[i].effect;
                if (player->hp > player->maxHp) player->hp = player->maxHp;
                printf("Used %s. HP: %d/%d\n", player->inventory[i].name, player->hp, player->maxHp);

                for (int j = i; j < player->inventoryCount - 1; j++) {
                    player->inventory[j] = player->inventory[j + 1];
                }
                player->inventoryCount--;
                return;
            }
        }
    }
    printf("No item of that type found!\n");
}

void useHealingPotion(Character *player) {
    useItem(player, ITEM_TYPE_POTION);
}

// ============================================================================
// ENEMY MANAGEMENT
// ============================================================================

void initializeEnemyStats(Enemy *enemy, int hpDice, int hpMultiplier, int playerLevel) {
    enemy->hp = rollDice(hpDice) + getModifier(enemy->constitution) + playerLevel * hpMultiplier;
    enemy->maxHp = enemy->hp;
    enemy->stunned = 0;
    enemy->armorClass = 10 + getModifier(enemy->dexterity);
    enemy->attackBonus = getProficiencyBonus(playerLevel) + getModifier(enemy->strength);
}

Enemy createEnemy(int playerLevel) {
    Enemy enemy;
    memset(&enemy, 0, sizeof(Enemy));

    int roll = rollDice(20);

    if (roll <= 3) {
        strcpy(enemy.name, "Goblin");
        enemy.strength = 10; enemy.dexterity = 14; enemy.constitution = 10;
        enemy.intelligence = 8; enemy.wisdom = 8; enemy.charisma = 8;
        enemy.damageDice = 6; enemy.xpReward = 50; enemy.isSpellcaster = 0;
    } else if (roll <= 6) {
        strcpy(enemy.name, "Skeleton");
        enemy.strength = 10; enemy.dexterity = 12; enemy.constitution = 10;
        enemy.intelligence = 6; enemy.wisdom = 8; enemy.charisma = 5;
        enemy.damageDice = 6; enemy.xpReward = 75; enemy.isSpellcaster = 0;
    } else if (roll <= 9) {
        strcpy(enemy.name, "Zombie");
        enemy.strength = 13; enemy.dexterity = 6; enemy.constitution = 14;
        enemy.intelligence = 3; enemy.wisdom = 6; enemy.charisma = 5;
        enemy.damageDice = 8; enemy.xpReward = 100; enemy.isSpellcaster = 0;
    } else if (roll <= 12) {
        strcpy(enemy.name, "Spider");
        enemy.strength = 10; enemy.dexterity = 16; enemy.constitution = 10;
        enemy.intelligence = 2; enemy.wisdom = 10; enemy.charisma = 4;
        enemy.damageDice = 6; enemy.xpReward = 125; enemy.isSpellcaster = 0;
    } else if (roll <= 15) {
        strcpy(enemy.name, "Bandit");
        enemy.strength = 14; enemy.dexterity = 12; enemy.constitution = 12;
        enemy.intelligence = 10; enemy.wisdom = 8; enemy.charisma = 10;
        enemy.damageDice = 8; enemy.xpReward = 200; enemy.isSpellcaster = 0;
    } else if (roll <= 17) {
        strcpy(enemy.name, "Necromancer");
        enemy.strength = 8; enemy.dexterity = 10; enemy.constitution = 10;
        enemy.intelligence = 16; enemy.wisdom = 12; enemy.charisma = 14;
        enemy.damageDice = 10; enemy.xpReward = 300; enemy.isSpellcaster = 1;
    } else if (roll == 18) {
        strcpy(enemy.name, "Orc");
        enemy.strength = 16; enemy.dexterity = 12; enemy.constitution = 14;
        enemy.intelligence = 7; enemy.wisdom = 9; enemy.charisma = 8;
        enemy.damageDice = 10; enemy.xpReward = 350; enemy.isSpellcaster = 0;
    } else if (roll == 19) {
        strcpy(enemy.name, "Troll");
        enemy.strength = 18; enemy.dexterity = 10; enemy.constitution = 16;
        enemy.intelligence = 6; enemy.wisdom = 9; enemy.charisma = 7;
        enemy.damageDice = 12; enemy.xpReward = 500; enemy.isSpellcaster = 0;
    } else {
        strcpy(enemy.name, "Shadow Mage");
        enemy.strength = 8; enemy.dexterity = 14; enemy.constitution = 12;
        enemy.intelligence = 18; enemy.wisdom = 14; enemy.charisma = 16;
        enemy.damageDice = 8; enemy.xpReward = 600; enemy.isSpellcaster = 1;
    }

    initializeEnemyStats(&enemy, 8, 2, playerLevel);
    return enemy;
}

Enemy createBoss(int playerLevel, const char *bossName) {
    Enemy boss;
    memset(&boss, 0, sizeof(Enemy));
    strcpy(boss.name, bossName);

    if (strcmp(bossName, "Necromancer") == 0) {
        boss.strength = 10; boss.dexterity = 12; boss.constitution = 14;
        boss.intelligence = 18; boss.wisdom = 14; boss.charisma = 16;
        boss.damageDice = 10; boss.isSpellcaster = 1; boss.xpReward = 1000;
        initializeEnemyStats(&boss, 10, 10, playerLevel);
        boss.spellSlots[0] = 4; boss.spellSlots[1] = 3; boss.spellSlots[2] = 2;
    } else if (strcmp(bossName, "Shadow Lord") == 0) {
        boss.strength = 16; boss.dexterity = 14; boss.constitution = 18;
        boss.intelligence = 16; boss.wisdom = 12; boss.charisma = 18;
        boss.damageDice = 12; boss.isSpellcaster = 1; boss.xpReward = 2000;
        initializeEnemyStats(&boss, 12, 15, playerLevel);
        boss.spellSlots[0] = 4; boss.spellSlots[1] = 3; boss.spellSlots[2] = 2; boss.spellSlots[3] = 1;
    } else if (strcmp(bossName, "Dragon") == 0) {
        boss.strength = 20; boss.dexterity = 10; boss.constitution = 20;
        boss.intelligence = 16; boss.wisdom = 12; boss.charisma = 18;
        boss.damageDice = 20; boss.isSpellcaster = 0; boss.xpReward = 5000;
        initializeEnemyStats(&boss, 20, 20, playerLevel);
    } else {
        boss.strength = 12; boss.dexterity = 12; boss.constitution = 12;
        boss.intelligence = 12; boss.wisdom = 12; boss.charisma = 12;
        boss.damageDice = 10; boss.isSpellcaster = 0; boss.xpReward = 800;
        initializeEnemyStats(&boss, 8, 5, playerLevel);
    }

    return boss;
}

Spell chooseEnemySpell(Enemy *enemy) {
    Spell spells[] = {
        {"Shadow Bolt", 8, "Damage", 1, 0},
        {"Drain Life", 6, "Heal", 1, 0},
        {"Fear", 0, "Control", 1, 0}
    };
    return spells[rand() % 3];
}

void castSpellEnemy(Enemy *enemy, Character *player, Spell spell) {
    if (strcmp(spell.effect, "Damage") == 0) {
        int damage = rollDice(spell.damageDice) + getModifier(enemy->intelligence);
        player->hp -= damage;
        if (player->hp < 0) player->hp = 0;
        printf("%s casts %s, dealing %d damage! Player HP: %d/%d\n", enemy->name, spell.name, damage, player->hp, player->maxHp);
    } else if (strcmp(spell.effect, "Heal") == 0) {
        int heal = rollDice(spell.damageDice) + getModifier(enemy->intelligence);
        enemy->hp += heal;
        if (enemy->hp > enemy->maxHp) enemy->hp = enemy->maxHp;
        printf("%s casts %s, healing for %d. Enemy HP: %d/%d\n", enemy->name, spell.name, heal, enemy->hp, enemy->maxHp);
    } else if (strcmp(spell.effect, "Control") == 0) {
        printf("%s casts %s!\n", enemy->name, spell.name);
        player->stunned = 1;
    }
}

void handleEnemySpellcasting(Enemy *enemy, Character *player) {
    if (enemy->isSpellcaster && rollDice(10) > 5) {
        int slotLevel = 1;
        if (enemy->spellSlots[slotLevel - 1] > 0) {
            printf("%s casts a spell!\n", enemy->name);
            Spell spell = chooseEnemySpell(enemy);
            castSpellEnemy(enemy, player, spell);
            enemy->spellSlots[slotLevel - 1]--;
        }
    }
}

// ============================================================================
// LOOT SYSTEM
// ============================================================================

void generateLoot(Enemy *enemy, Item *loot[], int *lootCount, int playerLevel) {
    int rollD20 = rollDice(20);
    int coins = rollD20 * playerLevel * 5;

    *lootCount = 0;

    loot[*lootCount] = malloc(sizeof(Item));
    strcpy(loot[*lootCount]->name, "Gold Coins");
    loot[*lootCount]->type = ITEM_TYPE_GOLD;
    loot[*lootCount]->value = coins;
    (*lootCount)++;

    int rollD100 = rollDice(100);
    if (rollD100 <= 90) {
        loot[*lootCount] = malloc(sizeof(Item));
        strcpy(loot[*lootCount]->name, "Health Potion");
        loot[*lootCount]->type = ITEM_TYPE_POTION;
        loot[*lootCount]->effect = 10;
        loot[*lootCount]->value = 50;
        (*lootCount)++;
    }
}

void displayLoot(Item *loot[], int lootCount) {
    printf("\nLoot Found:\n");
    for (int i = 0; i < lootCount; i++) {
        printf("%d. %s (Value: %d)\n", i + 1, loot[i]->name, loot[i]->value);
    }
}

// ============================================================================
// BUFF AND STATUS MANAGEMENT
// ============================================================================

void applyBuff(Character *player, const char *buffName) {
    printf("Buff applied: %s\n", buffName);
    player->buffed = 1;
    player->buffDuration = 3;
}

void manageBuffs(Character *player) {
    if (player->buffed && player->buffDuration > 0) {
        player->buffDuration--;
        if (player->buffDuration == 0) {
            removeBuff(player);
        }
    }
}

void removeBuff(Character *player) {
    printf("Buff expired.\n");
    player->buffed = 0;
}

// ============================================================================
// CLASS ABILITIES
// ============================================================================

int hasAvailableActionPoints(Character *player) {
    return player->actionPoints > 0;
}

void useActionPoint(Character *player, const char *ability) {
    if (player->actionPoints > 0) {
        player->actionPoints--;
        printf("%s used. Points left: %d\n", ability, player->actionPoints);
    } else {
        printf("No action points!\n");
    }
}

void useRage(Character *player) {
    if (player->ragePoints > 0) {
        player->ragePoints--;
        printf("Rage activated! Rages left: %d\n", player->ragePoints);
    } else {
        printf("No rage points!\n");
    }
}

// ============================================================================
// REST SYSTEM
// ============================================================================

void shortRest(Character *player) {
    if (player->hitDice <= 0) {
        printf("No hit dice left! Take a long rest.\n");
        return;
    }

    printf("\nShort Rest: You have %dd%d hit dice.\n", player->hitDice, player->hitDiceValue);
    printf("Dice to use: ");
    int diceToUse;
    scanf("%d", &diceToUse);
    getchar();

    if (diceToUse > player->hitDice) {
        diceToUse = player->hitDice;
    }

    int totalHeal = 0;
    for (int i = 0; i < diceToUse; i++) {
        int heal = rollDice(player->hitDiceValue) + getModifier(player->constitution);
        totalHeal += (heal > 0) ? heal : 1;
    }

    player->hp += totalHeal;
    if (player->hp > player->maxHp) player->hp = player->maxHp;
    player->hitDice -= diceToUse;

    if (strcmp(player->class, "Warlock") == 0) {
        initializeWarlockSlots(player);
    }

    if (strcmp(player->class, "Monk") == 0) {
        player->kiPoints = player->maxKiPoints;
        printf("Ki Points restored!\n");
    }

    printf("Healed %d HP. Current HP: %d/%d\n", totalHeal, player->hp, player->maxHp);
}

void longRest(Character *player) {
    printf("\nLong Rest: 8 hours of sleep...\n");

    int roll = rollDice(100);
    if (roll <= 20) {
        printf("Enemies attack during your rest!\n");
        Enemy enemy = createEnemy(player->level);
        printf("A %s appears!\n", enemy.name);
        startBattle(player, &enemy);

        if (player->hp <= 0) {
            printf("Defeated during rest. Game Over.\n");
            exit(0);
        }
    }

    player->hp = player->maxHp;
    player->hitDice = player->level;

    if (player->isSpellcaster) {
        initializeSpellSlots(player);
    }

    if (strcmp(player->class, "Fighter") == 0) {
        player->actionPoints = 2;
    }

    if (strcmp(player->class, "Barbarian") == 0) {
        player->ragePoints = player->level >= 5 ? 3 : 2;
    }

    if (strcmp(player->class, "Monk") == 0) {
        player->kiPoints = player->maxKiPoints;
    }

    printf("Fully rested. HP: %d/%d\n", player->hp, player->maxHp);
}

// ============================================================================
// BATTLE SYSTEM
// ============================================================================

int rollInitiative(Character *player, Enemy *enemy) {
    int playerRoll = rollDice(20) + getModifier(player->dexterity);
    int enemyRoll = rollDice(20) + getModifier(enemy->dexterity);

    printf("\nInitiative:\n%s: %d\n%s: %d\n", player->name, playerRoll, enemy->name, enemyRoll);

    return (playerRoll > enemyRoll) ? 1 : 0;
}

void playerAttack(Character *player, Enemy *enemy) {
    int attacks = (player->extraAttack && (strcmp(player->class, "Fighter") == 0 || strcmp(player->class, "Rogue") == 0)) ? 2 : 1;

    for (int i = 0; i < attacks; i++) {
        int attackRoll = rollDice(20) + getModifier(player->strength);
        printf("\nAttack %d: Roll %d vs AC %d\n", i + 1, attackRoll, enemy->armorClass);

        if (attackRoll >= enemy->armorClass) {
            int damage = rollDice(player->weaponDamage) + getModifier(player->strength);

            if (attackRoll == 20) {
                printf("CRITICAL HIT!\n");
                damage += rollDice(player->weaponDamage);
            }

            enemy->hp -= damage;
            if (enemy->hp < 0) enemy->hp = 0;

            printf("HIT! Dealt %d damage. Enemy HP: %d/%d\n", damage, enemy->hp, enemy->maxHp);

            if (enemy->hp == 0) break;
        } else {
            printf("MISS!\n");
        }
    }
}

void enemyAttack(Enemy *enemy, Character *player) {
    int attackRoll = rollDice(20) + enemy->attackBonus;
    printf("%s attacks: Roll %d vs AC %d\n", enemy->name, attackRoll, player->armorClass);

    if (attackRoll >= player->armorClass) {
        int damage = rollDice(enemy->damageDice) + getModifier(enemy->strength);
        player->hp -= damage;
        printf("HIT! You took %d damage. HP: %d/%d\n", damage, player->hp, player->maxHp);
    } else {
        printf("MISS!\n");
    }
}

void handleBattleEnd(Character *player, Enemy *enemy) {
    if (enemy->hp <= 0) {
        printf("\n*** %s DEFEATED! ***\n", enemy->name);
        printf("Gained %d XP!\n", enemy->xpReward);
        addExperience(player, enemy->xpReward);

        Item *loot[10];
        int lootCount = 0;
        generateLoot(enemy, loot, &lootCount, player->level);
        displayLoot(loot, lootCount);

        for (int i = 0; i < lootCount; i++) {
            addItemToInventory(player, loot[i]);
            free(loot[i]);
        }
    }

    if (player->hp <= 0) {
        printf("\n*** YOU HAVE BEEN DEFEATED! ***\n");
    }
}

void startBattle(Character *player, Enemy *enemy) {
    printf("\n========== BATTLE START! ==========\n");
    printf("%s (HP: %d) vs %s (HP: %d)\n", player->name, player->hp, enemy->name, enemy->hp);

    int playerTurn = rollInitiative(player, enemy);

    while (player->hp > 0 && enemy->hp > 0) {
        if (playerTurn) {
            printf("\n[YOUR TURN]\n");
            printf("1. Attack\n2. Use Potion\n");
            if (player->isSpellcaster && hasAvailableSpellSlots(player)) {
                printf("3. Cast Spell\n");
            }
            if (strcmp(player->class, "Monk") == 0 && player->kiPoints > 0) {
                printf("4. Flurry of Blows (1 Ki)\n5. Stunning Strike (1 Ki)\n");
            }
            printf("Choice: ");

            int action;
            scanf("%d", &action);
            getchar();

            switch (action) {
                case 1:
                    playerAttack(player, enemy);
                    break;
                case 2:
                    useHealingPotion(player);
                    break;
                case 3:
                    if (player->isSpellcaster) {
                        printf("Spell level (1-9): ");
                        int slotLevel;
                        scanf("%d", &slotLevel);
                        getchar();
                        castSpell(player, enemy, slotLevel);
                    }
                    break;
                case 4:
                    if (strcmp(player->class, "Monk") == 0) {
                        flurryOfBlows(player, enemy);
                    }
                    break;
                case 5:
                    if (strcmp(player->class, "Monk") == 0) {
                        stunningStrike(player, enemy);
                    }
                    break;
                default:
                    printf("Invalid action. Skipped turn.\n");
            }
        } else {
            printf("\n[ENEMY TURN]\n");
            if (enemy->isSpellcaster && rollDice(10) > 5) {
                handleEnemySpellcasting(enemy, player);
            } else {
                enemyAttack(enemy, player);
            }
        }

        handleBattleEnd(player, enemy);
        if (player->hp <= 0 || enemy->hp <= 0) break;

        playerTurn = enemy->stunned ? 1 : !playerTurn;
        if (enemy->stunned) enemy->stunned = 0;
    }

    printf("\n===================================\n");
}

// ============================================================================
// MAIN GAME LOOP
// ============================================================================

void displayMainMenu() {
    clearScreen();
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║      D&D TEXT ADVENTURE GAME v2.0      ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n1. Create New Character\n");
    printf("2. Load Existing Character\n");
    printf("3. List Saved Characters\n");
    printf("4. Exit\n");
    printf("\nChoice: ");
}

void displayGameMenu(Character *player) {
    clearScreen();
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║  %s (Level %d)\n", player->name, player->level);
    printf("║  HP: %d/%d | XP: %d\n", player->hp, player->maxHp, player->experience);
    printf("╚════════════════════════════════════════╝\n");
    printf("\n1. Go Deeper into Dungeon\n");
    printf("2. Rest\n");
    printf("3. View Inventory\n");
    printf("4. View Stats\n");
    printf("5. View Spells\n");
    printf("6. Save and Exit\n");
    printf("\nChoice: ");
}

void displayStats(Character *player) {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║           CHARACTER STATS              ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\nName: %s\n", player->name);
    printf("Race: %s | Class: %s\n", player->race, player->class);
    printf("Level: %d | XP: %d\n", player->level, player->experience);
    printf("\nHP: %d/%d\n", player->hp, player->maxHp);
    printf("AC: %d\n", player->armorClass);
    printf("\nStrength: %d\nConstitution: %d\nDexterity: %d\n", player->strength, player->constitution, player->dexterity);
    printf("Intelligence: %d\nWisdom: %d\nCharisma: %d\n", player->intelligence, player->wisdom, player->charisma);
    printf("\nBattles Won: %d\n", player->battlesWon);
    printf("Hardcore Mode: %s\n", player->hardcore ? "Enabled" : "Disabled");
    printf("\nPress Enter to continue...\n");
    getchar();
}

int main() {
    srand(time(0));

    Character player;
    int running = 1;
    int inGame = 0;

    while (running) {
        if (!inGame) {
            displayMainMenu();
            int choice;
            scanf("%d", &choice);
            getchar();

            switch (choice) {
                case 1:
                    player = createCharacter();
                    saveCharacterProgress(&player);
                    inGame = 1;
                    break;
                case 2:
                    listSavedCharacters();
                    printf("\nEnter character name to load: ");
                    char charName[50];
                    safe_input(charName, sizeof(charName));
                    char fileName[100];
                    snprintf(fileName, sizeof(fileName), "%s.txt", charName);
                    player = loadCharacterByName(fileName);
                    inGame = 1;
                    break;
                case 3:
                    listSavedCharacters();
                    printf("\nPress Enter...\n");
                    getchar();
                    break;
                case 4:
                    running = 0;
                    break;
                default:
                    printf("Invalid choice.\n");
                    getchar();
            }
        } else {
            displayGameMenu(&player);
            int choice;
            scanf("%d", &choice);
            getchar();

            switch (choice) {
                case 1: {
                    Enemy enemy;
                    if (player.battlesWon == 10 && player.level >= 3) {
                        enemy = createBoss(player.level, "Necromancer");
                    } else if (player.battlesWon == 20 && player.level >= 5) {
                        enemy = createBoss(player.level, "Shadow Lord");
                    } else {
                        enemy = createEnemy(player.level);
                    }

                    printf("\nA %s appears! HP: %d, AC: %d\n", enemy.name, enemy.hp, enemy.armorClass);
                    getchar();
                    startBattle(&player, &enemy);

                    if (player.hp <= 0) {
                        if (player.hardcore) {
                            printf("\nHardcore mode! Deleting save...\n");
                            deleteSaveFile(player.name);
                            inGame = 0;
                        } else {
                            printf("\nContinue from checkpoint? (1=Yes, 0=No): ");
                            int cont;
                            scanf("%d", &cont);
                            getchar();
                            if (cont) {
                                player.hp = player.maxHp;
                            } else {
                                inGame = 0;
                            }
                        }
                    } else {
                        player.battlesWon++;
                        saveCharacterProgress(&player);
                    }
                    break;
                }
                case 2: {
                    printf("\n1. Short Rest\n2. Long Rest\nChoice: ");
                    int restChoice;
                    scanf("%d", &restChoice);
                    getchar();
                    if (restChoice == 1) {
                        shortRest(&player);
                    } else {
                        longRest(&player);
                    }
                    saveCharacterProgress(&player);
                    printf("\nPress Enter...\n");
                    getchar();
                    break;
                }
                case 3:
                    showInventory(&player);
                    printf("\nPress Enter...\n");
                    getchar();
                    break;
                case 4:
                    displayStats(&player);
                    break;
                case 5:
                    listSpells(&player);
                    displaySpellSlots(&player);
                    printf("\nPress Enter...\n");
                    getchar();
                    break;
                case 6:
                    saveCharacterProgress(&player);
                    inGame = 0;
                    break;
                default:
                    printf("Invalid choice.\n");
                    getchar();
            }
        }
    }

    printf("\nThanks for playing!\n");
    return 0;
}
