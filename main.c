#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

// Typedefs for inventory, spells, and characters
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
    int effect;  // e.g., healing amount or bonus provided by the item
} Item;

typedef struct {
    char name[30];
    int damageDice;  // Damage die, 0 if non-damaging
    char effect[20]; // Effect type: "Damage", "Heal", "Buff", etc.
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
    int spellSlots[10];  // Available spell slots for spell levels 1-9
    int extraAttack;
    int battlesWon;
    int hardcore;
    int actionPoints;
    int ragePoints;
    int buffed;          // Whether a buff is active
    int buffDuration;    // Duration of the buff in turns
    int kiPoints;        // Used by Monk class
    int maxKiPoints;
    int isSpellcaster;   // Boolean flag for spellcasting classes
    Item inventory[50];  // Character inventory
    int inventoryCount;  // Number of items in inventory
    int stunned;
} Character;

typedef struct {
    char name[50];
    int hp, maxHp;
    int armorClass;
    int attackBonus;
    int damageDice;
    int strength;     // For melee attacks and saves
    int dexterity;    // For AC and initiative
    int constitution; // For HP and saves
    int intelligence; // For magic-based actions
    int wisdom;       // For saving throws, perception
    int charisma;     // For charm-related effects
    int stunned;      // Indicates if the enemy is stunned
    int xpReward;     // XP given when defeated
    int spellSlots[9]; // Spell slots (for levels 1-9)
    int isSpellcaster; // Boolean flag indicating spellcasting ability
} Enemy;

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

// Spell and combat management
void listSpells(Character *player);
void displaySpellSlots(Character *player);
void castSpell(Character *player, Enemy *enemy, int slotLevel);
Spell getSpellByClassAndChoice(Character *player, int spellChoice);
int validateSpellSlot(Character *player, int requestedSlot);
int hasAvailableSpellSlots(Character *player);
Spell *getSpellsForClass(const char *className, int *spellCount, int characterLevel);


// Class-specific utilities
void useActionPoint(Character *player, const char *ability);
void useRage(Character *player);
int hasAvailableActionPoints(Character *player);
int hasAvailableRages(Character *player);

// Character utilities
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


// Inventory and item management
void addItemToInventory(Character *player, Item *item);
void showInventory(Character *player);
void useItem(Character *player, ItemType type);

// Game utilities
void listSavedCharacters();
void deleteSaveFile(const char *fileName);
int rollDice(int sides);
int getModifier(int stat);
int getProficiencyBonus(int level);
int calculateSpellSaveDC(int castingStat, int level);

// Function prototypes
Enemy createEnemy(int playerLevel);
Enemy createBoss(int playerLevel, const char *bossName);
void enemyTurn(Enemy *enemy, Character *player);
void handleEnemySpellcasting(Enemy *enemy, Character *player);
void applyEnemyBuff(Enemy *enemy, const char *buffName);

// Core battle mechanics
void startBattle(Character *player, Enemy *enemy);
void playerAttack(Character *player, Enemy *enemy);
void enemyAttack(Enemy *enemy, Character *player);

// Player utilities
void useHealingPotion(Character *player);
void handleStatusEffects(Character *player, Enemy *enemy);
void applyCombatEffect(const char *effect, Character *player, Enemy *enemy);


// Battle resolution
void handleBattleEnd(Character *player, Enemy *enemy);

int standardArray[] = {15, 14, 13, 12, 10, 8};
const char *statNames[] = {"Strength", "Constitution", "Dexterity", "Intelligence", "Wisdom", "Charisma"};

Character createCharacter() {
    Character newChar;
    newChar.level = 1;

    printf("Enter your character's name: ");
    scanf("%49s", newChar.name);  // Prevent buffer overflow

    printf("Choose your race (Human, Elf, Half-Elf, Drow, Dwarf, Tiefling, Halfling, Gnome, Half-Orc, Dragonborn):\n");
    scanf("%19s", newChar.race);

    printf("Choose your class (Fighter, Paladin, Barbarian, Ranger, Rogue, Wizard, Sorcerer, Warlock, Cleric, Bard):\n");
    scanf("%19s", newChar.class);

    printf("Distribute your stats using the standard array (15, 14, 13, 12, 10, 8):\n");

    int *statPointer[] = { &newChar.strength, &newChar.constitution, &newChar.dexterity, 
                           &newChar.intelligence, &newChar.wisdom, &newChar.charisma };
    int used[6] = {0};  // Track which standardArray values are used

    for (int i = 0; i < 6; i++) {
        int statChoice, value;
        do {
            printf("Assign value from the array to %s: ", statNames[i]);
            scanf("%d", &value);
            for (statChoice = 0; statChoice < 6; statChoice++) {
                if (standardArray[statChoice] == value && !used[statChoice]) {
                    used[statChoice] = 1;  // Mark value as used
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
    setupClass(&newChar);  // Set class-specific hit dice and features

    

    // Prompt for Hardcore Mode
    printf("Do you want to enable Hardcore Mode? (1 for Yes, 0 for No): ");
    int hardcoreChoice;
    scanf("%d", &hardcoreChoice);
    newChar.hardcore = (hardcoreChoice == 1);

    // Initial HP and Armor Class
    newChar.maxHp = newChar.hitDiceValue + getModifier(newChar.constitution);
    newChar.hp = newChar.maxHp;

    setupInventory(&newChar);  // Assign default inventory or starting equipment
    printf("Character created successfully!\n");

    return newChar;
}


void initializeCharacterStats(Character *player) {
    player->hp = player->maxHp;
    player->actionPoints = 2;  // Example default action points
    player->ragePoints = 0;
    player->kiPoints = player->maxKiPoints;
    player->stunned = 0;
    printf("Character stats initialized for %s.\n", player->name);
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
        printf("Choose two stats to increase by +1 (other than Charisma):\n");
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
        scanf(" %c", &option);
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
        printf("Choose stat %d to increase (Strength, Constitution, Dexterity, Intelligence, Wisdom, Charisma): ", i + 1);
        scanf("%19s", stat);  // Prevent buffer overflow
        if (strcmp(stat, "Strength") == 0) player->strength++;
        else if (strcmp(stat, "Constitution") == 0) player->constitution++;
        else if (strcmp(stat, "Dexterity") == 0) player->dexterity++;
        else if (strcmp(stat, "Intelligence") == 0) player->intelligence++;
        else if (strcmp(stat, "Wisdom") == 0) player->wisdom++;
        else if (strcmp(stat, "Charisma") == 0) player->charisma++;
        else {
            printf("Invalid stat. Try again.\n");
            i--;  // Retry current stat allocation
        }
    }
}

void setupClass(Character *player) {
    player->hitDiceValue = getHitDiceValue(player->class);

    if (strcmp(player->class, "Wizard") == 0 || 
        strcmp(player->class, "Cleric") == 0 || 
        strcmp(player->class, "Sorcerer") == 0 || 
        strcmp(player->class, "Bard") == 0 ||
        strcmp(player->class, "Druid") == 0) {
        player->isSpellcaster = 1;
        initializeFullCasterSlots(player);  // Full-casters
    } 
    else if (strcmp(player->class, "Paladin") == 0 || 
             strcmp(player->class, "Ranger") == 0) {
        player->isSpellcaster = 1;
        initializeHalfCasterSlots(player);  // Half-casters
    } 
    else if (strcmp(player->class, "Warlock") == 0) {
        player->isSpellcaster = 1;
        initializeWarlockSlots(player);  // Warlock-specific spell slots
    } 
    else {
        player->isSpellcaster = 0;  // Non-spellcasters
    }
}



int getHitDiceValue(const char *class) {
    if (strcmp(class, "Barbarian") == 0) return 12;
    if (strcmp(class, "Fighter") == 0 || strcmp(class, "Paladin") == 0 || strcmp(class, "Ranger") == 0) return 10;
    if (strcmp(class, "Rogue") == 0 || strcmp(class, "Cleric") == 0 || strcmp(class, "Warlock") == 0 || 
        strcmp(class, "Bard") == 0 || strcmp(class, "Monk") == 0) return 8;
    return 6; // Default for Wizard and Sorcerer
}


void setupInventory(Character *player) {
    if (strcmp(player->class, "Fighter") == 0) {
        int weaponChoice;
        printf("\nChoose your weapon:\n1. Two-Handed Sword (2d6 damage)\n2. Sword (1d6 damage) and Shield (+2 AC)\n");
        scanf("%d", &weaponChoice);

        player->weaponDamage = (weaponChoice == 1) ? 12 : 6; // Assign weapon damage
        player->shieldBonus = (weaponChoice == 2) ? 2 : 0;   // Add shield bonus if applicable
        player->armorClass = 13 + getModifier(player->dexterity) + player->shieldBonus; 

    } else if (strcmp(player->class, "Paladin") == 0) {
        printf("\nChoose your weapon:\n1. Mace (1d8 damage)\n2. Sword (1d6 damage) and Shield (+2 AC)\n");
        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                player->weaponDamage = 8;  // Mace
                player->shieldBonus = 0;
            } else if (weaponChoice == 2) {
                player->weaponDamage = 6;  // Sword
                player->shieldBonus = 2;
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 16 + player->shieldBonus; // Chain Mail

    } else if (strcmp(player->class, "Ranger") == 0) {
        printf("\nChoose your weapon:\n1. Two Daggers (1d4 each, dual wield)\n2. Longsword (1d8 damage) and Bow (1d6 damage)\n");
        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                player->weaponDamage = 8;  // Two Daggers (1d4 + 1d4)
            } else if (weaponChoice == 2) {
                player->weaponDamage = 8;  // Longsword (1d8) with Bow (ranged 1d6)
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 12 + getModifier(player->dexterity); // Hide armor

    } else if (strcmp(player->class, "Wizard") == 0 || strcmp(player->class, "Sorcerer") == 0) {
        player->weaponDamage = 6;  // Quarterstaff
        player->armorClass = 10 + getModifier(player->dexterity);

    } else if (strcmp(player->class, "Monk") == 0) {
        printf("\nMonk Starting Equipment:\n");
        printf("Choose your weapon:\n1. Quarterstaff (1d6 damage)\n2. Hand to Hand (2d4 damage)\n");

        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                strcpy(player->currentWeapon, "Quarterstaff");
                player->weaponDamage = 8;  // 1d8 damage
            } else if (weaponChoice == 2) {
                strcpy(player->currentWeapon, "Hand to Hand");
                player->weaponDamage = 8;  // Punches (1d4 + 1d4)
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 10 + getModifier(player->dexterity) + getModifier(player->wisdom);
        player->shieldBonus = 0;  // Monks don't use shields

    } else if (strcmp(player->class, "Warlock") == 0) {
        player->weaponDamage = 6;  // Shortsword
        player->armorClass = 10 + getModifier(player->dexterity);

    } else if (strcmp(player->class, "Bard") == 0) {
        player->weaponDamage = 8;  // Rapier
        player->armorClass = 10 + getModifier(player->dexterity);

    } else if (strcmp(player->class, "Cleric") == 0) {
        player->weaponDamage = 8;  // Mace
        player->shieldBonus = 2;   // Shield
        player->armorClass = 13 + getModifier(player->dexterity) + player->shieldBonus;

    } else if (strcmp(player->class, "Druid") == 0) {
        printf("\nChoose your weapon:\n1. Quarterstaff (1d6 damage)\n2. Dagger (1d4 damage) and Wooden Shield (+1 AC)\n");
        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                player->weaponDamage = 6;  // Quarterstaff
                player->shieldBonus = 0;
            } else if (weaponChoice == 2) {
                player->weaponDamage = 4;  // Dagger
                player->shieldBonus = 1;   // Wooden Shield
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 10 + getModifier(player->dexterity) + player->shieldBonus;

    } else if (strcmp(player->class, "Rogue") == 0) {
        printf("\nChoose your weapon:\n1. Two Daggers (1d4 each, dual wield)\n2. Bow (1d6 damage)\n");
        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                player->weaponDamage = 8;  // Two Daggers (1d4 + 1d4)
            } else if (weaponChoice == 2) {
                player->weaponDamage = 6;  // Bow
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 10 + (2 * getModifier(player->dexterity));  // Double Dex modifier

    } else if (strcmp(player->class, "Barbarian") == 0) {
        printf("\nChoose your weapon:\n1. Greataxe (1d12 damage)\n2. Dual Handaxes (1d6 each)\n");
        int weaponChoice;
        do {
            scanf("%d", &weaponChoice);
            if (weaponChoice == 1) {
                player->weaponDamage = 12;  // Greataxe
            } else if (weaponChoice == 2) {
                player->weaponDamage = 6;   // Handaxe (dual)
            } else {
                printf("Invalid choice. Please select 1 or 2.\n");
            }
        } while (weaponChoice != 1 && weaponChoice != 2);

        player->armorClass = 10 + getModifier(player->dexterity) + getModifier(player->constitution);
    }

    // HP setup
    player->maxHp = rollDice(player->hitDiceValue) + getModifier(player->constitution);
    player->hp = player->maxHp;  // Set current HP to max HP
}

void saveCharacterProgress(Character *player) {
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "%s.txt", player->name);  // Safe filename generation
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error saving character.\n");
        return;
    }

    // Save all character attributes, including spell slots
    fprintf(file, "%s %s %s %s %d %d %d %d %d %d %d %d %d %d %d",
            player->name, player->race, player->sex, player->class,
            player->level, player->experience,
            player->strength, player->constitution, player->dexterity, 
            player->intelligence, player->wisdom, player->charisma, 
            player->hp, player->maxHp, player->armorClass);

    for (int i = 0; i < 9; i++) {
        fprintf(file, " %d", player->spellSlots[i]);
    }

    fclose(file);
}

Character loadCharacterByName(const char *fileName) {
    Character c;
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error loading save file. Exiting.\n");
        exit(1);
    }

    // Load character attributes
    fscanf(file, "%s %s %s %s %d %d %d %d %d %d %d %d %d %d %d",
           c.name, c.race, c.sex, c.class,
           &c.level, &c.experience,
           &c.strength, &c.constitution, &c.dexterity,
           &c.intelligence, &c.wisdom, &c.charisma,
           &c.hp, &c.maxHp, &c.armorClass);

    for (int i = 0; i < 9; i++) {
        fscanf(file, " %d", &c.spellSlots[i]);  // Load spell slots
    }

    fclose(file);
    return c;
}

void startBattle(Character *player, Enemy *enemy) {
    printf("\nBattle Start!\n");

    // Roll for initiative
    int playerTurn = rollInitiative(player, enemy);

    while (player->hp > 0 && enemy->hp > 0) {
        if (playerTurn) {
            printf("\nYour Turn:\n1. Attack\n2. Use Potion\n");

            if (player->isSpellcaster && hasAvailableSpellSlots(player)) {
                printf("3. Cast Spell\n");
            }

            int action;
            scanf("%d", &action);

            switch (action) {
                case 1:
                    playerAttack(player, enemy);
                    break;
                case 2:
                    useHealingPotion(player);
                    break;
                case 3:
                    if (player->isSpellcaster) {
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

                        castSpell(player, enemy, slotLevel);
                    } else {
                        printf("Invalid action! Skipping turn...\n");
                    }
                    break;
                default:
                    printf("Invalid choice! Skipping turn...\n");
                    break;
            }
        } else {
            printf("\nEnemy's Turn:\n");
            if (enemy->isSpellcaster && rollDice(10) > 5) {  // 50% chance to cast a spell
                handleEnemySpellcasting(enemy, player);
            } else {
                enemyAttack(enemy, player);
            }
        }

        handleBattleEnd(player, enemy);

        // Manage turn swapping and stun effects
        playerTurn = enemy->stunned ? 1 : !playerTurn;
        if (enemy->stunned) enemy->stunned = 0;  // Reset stun
    }
}

void playerAttack(Character *player, Enemy *enemy) {
    int attacks = (player->extraAttack && 
                  (strcmp(player->class, "Fighter") == 0 || 
                   strcmp(player->class, "Rogue") == 0)) ? 2 : 1;

    for (int i = 0; i < attacks; i++) {
        printf("\nAttack %d:\n", i + 1);
        int attackRoll = rollDice(20) + getModifier(player->strength);
        printf("Attack Roll: %d (vs AC %d)\n", attackRoll, enemy->armorClass);

        if (attackRoll >= enemy->armorClass) {
            int damage = rollDice(player->weaponDamage) + getModifier(player->strength);

            if (attackRoll == 20) {  // Critical hit
                printf("Critical Hit!\n");
                damage += rollDice(player->weaponDamage);
            }

            enemy->hp -= damage;
            enemy->hp = (enemy->hp < 0) ? 0 : enemy->hp; // Ensure HP doesn't drop below zero

            printf("You dealt %d damage! Enemy HP: %d/%d\n", damage, enemy->hp, enemy->maxHp);

            if (enemy->hp == 0) {
                printf("Enemy defeated!\n");
                break; // Stop further attacks if the enemy is defeated
            }
        } else {
            printf("You missed! The enemy dodges swiftly.\n");
        }
    }
}

void initializeEnemy(Enemy *enemy) {
    if (strcmp(enemy->name, "Necromancer") == 0 || 
        strcmp(enemy->name, "Shadow Lord") == 0 || 
        strcmp(enemy->name, "Dragon") == 0) {
        enemy->isSpellcaster = 1;  // Only bosses are spellcasters
    } else {
        enemy->isSpellcaster = 0;  // Regular enemies cannot cast spells
    }
}

// Enemy Attack Function
void enemyAttack(Enemy *enemy, Character *player) {
    int attackRoll = rollDice(20) + enemy->attackBonus;
    printf("Enemy Attack Roll: %d (vs AC %d)\n", attackRoll, player->armorClass);
    
    if (attackRoll >= player->armorClass) {
        int damage = rollDice(enemy->damageDice) + getModifier(enemy->strength);
        player->hp -= damage;
        printf("%s hits you for %d damage! Player HP: %d/%d\n", enemy->name, damage, player->hp, player->maxHp);

        // Ensure enemy HP is adjusted properly (in case of backlash logic)
        enemy->hp = (enemy->hp < 0) ? 0 : enemy->hp;
    } else {
        printf("%s missed!\n", enemy->name);
    }
}


// Enemy Spellcasting Logic
void handleEnemySpellcasting(Enemy *enemy, Character *player) {
    if (enemy->isSpellcaster) {
        int slotLevel = 1; // Example slot level usage
        if (enemy->spellSlots[slotLevel - 1] > 0) {
            printf("%s is casting a spell!\n", enemy->name);
            Spell enemySpell = chooseEnemySpell(enemy);
            castSpellEnemy(enemy, player, enemySpell);
            enemy->spellSlots[slotLevel - 1]--;  // Decrement spell slot
        } else {
            printf("%s tries to cast a spell but has no available spell slots!\n", enemy->name);
        }
    } else {
        printf("%s tries to cast a spell but fails! (Not a spellcaster)\n", enemy->name);
    }
}


Spell chooseEnemySpell(Enemy *enemy) {
    if (strcmp(enemy->name, "Necromancer") == 0) {
        Spell necromancerSpells[] = {
            {"Shadow Bolt", 8, "Damage"},
            {"Drain Life", 6, "Heal"},
            {"Fear", 0, "Control"}
        };
        int spellCount = sizeof(necromancerSpells) / sizeof(necromancerSpells[0]);
        return necromancerSpells[rand() % spellCount];
    }
    // Default spell pool
    Spell defaultSpells[] = {
        {"Magic Missile", 4, "Damage"},
        {"Cure Wounds", 8, "Heal"}
    };
    int spellCount = sizeof(defaultSpells) / sizeof(defaultSpells[0]);
    return defaultSpells[rand() % spellCount];
}

void castSpellEnemy(Enemy *enemy, Character *player, Spell spell) {
    if (strcmp(spell.effect, "Damage") == 0) {
        int damage = rollDice(spell.damageDice) + getModifier(enemy->intelligence);
        player->hp -= damage;
        if (player->hp < 0) player->hp = 0;  // Ensure HP doesn't go below zero
        printf("%s casts %s, dealing %d damage! Player HP: %d/%d\n", enemy->name, spell.name, damage, player->hp, player->maxHp);
    } else if (strcmp(spell.effect, "Heal") == 0) {
        int heal = rollDice(spell.damageDice) + getModifier(enemy->intelligence);
        enemy->hp += heal;
        if (enemy->hp > enemy->maxHp) enemy->hp = enemy->maxHp;
        printf("%s casts %s, healing for %d HP. Enemy HP: %d/%d\n", enemy->name, spell.name, heal, enemy->hp, enemy->maxHp);
    } else if (strcmp(spell.effect, "Control") == 0) {
        printf("%s casts %s. %s is now feared!\n", enemy->name, spell.name, player->name);
        player->stunned = 1;
    }
}

void applyControlEffect(Character *player, const char *effect, int duration) {
    if (strcmp(effect, "Fear") == 0) {
        printf("%s is feared for %d turns!\n", player->name, duration);
        player->stunned = duration;  // Track turns remaining
    }
}


// Healing Potion Function
void useHealingPotion(Character *player) {
    for (int i = 0; i < player->inventoryCount; i++) {
        if (player->inventory[i].type == ITEM_TYPE_POTION) {
            printf("\nYou use %s!\n", player->inventory[i].name);
            int heal = rollDice(4) + rollDice(4) + 2;  // 2d4 + 2 healing
            player->hp += heal;
            if (player->hp > player->maxHp) player->hp = player->maxHp;
            printf("You healed for %d HP. Current HP: %d/%d\n", heal, player->hp, player->maxHp);

            // Remove the potion from the inventory
            for (int j = i; j < player->inventoryCount - 1; j++) {
                player->inventory[j] = player->inventory[j + 1];
            }
            player->inventoryCount--;
            return;
        }
    }

    printf("You have no potions left!\n");
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
    } while (playerRoll == enemyRoll);

    return (playerRoll > enemyRoll) ? 1 : 0;  // 1 if player goes first
}

// Battle Outcome Function
void handleBattleEnd(Character *player, Enemy *enemy) {
    if (enemy->hp <= 0) {
        printf("\n%s defeated! You earned %d XP.\n", enemy->name, enemy->xpReward);
        addExperience(player, enemy->xpReward);
        saveCharacterProgress(player);

        // After enemy defeat
        printf("Enemy defeated! Rolling for loot...\n");
        Item *loot[10];
        int lootCount = 0;

        generateLoot(enemy, loot, &lootCount, player->level);  // Generate loot based on player level
        displayLoot(loot, lootCount);

        for (int i = 0; i < lootCount; i++) {
            addItemToInventory(player, loot[i]);
            free(loot[i]);  // Free dynamically allocated loot
        }
    }

    if (player->hp <= 0) {
        printf("\nYou have been defeated in battle.\n");
    }
}

// Dice Roll Function
int rollDice(int sides) {
    return (rand() % sides) + 1;  // Roll a dice with given sides
}

// Get Stat Modifier Function
int getModifier(int stat) {
    return (stat - 10) / 2;  // D&D modifier calculation
}

// Proficiency Bonus Function
int getProficiencyBonus(int level) {
    if (level <= 4) return 2;
    else if (level <= 8) return 3;
    else if (level <= 12) return 4;
    else if (level <= 16) return 5;
    else return 6;  // Levels 17-20
}

// Calculate Spell Save DC Function
int calculateSpellSaveDC(int castingStat, int level) {
    return 8 + getProficiencyBonus(level) + getModifier(castingStat);
}


void initializeEnemyStats(Enemy *enemy, int hpDice, int hpMultiplier, int playerLevel) {
    enemy->hp = rollDice(hpDice) + getModifier(enemy->constitution) + playerLevel * hpMultiplier;
    enemy->maxHp = enemy->hp;  // Set maxHp equal to hp
    enemy->stunned = 0;  // Initialize stunned state
    enemy->armorClass = 10 + getModifier(enemy->dexterity);
    enemy->attackBonus = getProficiencyBonus(playerLevel) + getModifier(enemy->strength);
}

Enemy createEnemy(int playerLevel) {
    Enemy enemy;
    int roll = rollDice(20);

    if (roll <= 3) {
        strcpy(enemy.name, "Goblin");
        enemy.strength = 10; enemy.dexterity = 14; enemy.constitution = 10;
        enemy.intelligence = 8; enemy.wisdom = 8; enemy.charisma = 8;
        enemy.damageDice = 6; enemy.xpReward = 50; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 6, 2, playerLevel);
    } else if (roll <= 6) {
        strcpy(enemy.name, "Skeleton");
        enemy.strength = 10; enemy.dexterity = 12; enemy.constitution = 10;
        enemy.intelligence = 6; enemy.wisdom = 8; enemy.charisma = 5;
        enemy.damageDice = 6; enemy.xpReward = 75; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 8, 2, playerLevel);
    } else if (roll <= 9) {
        strcpy(enemy.name, "Zombie");
        enemy.strength = 13; enemy.dexterity = 6; enemy.constitution = 14;
        enemy.intelligence = 3; enemy.wisdom = 6; enemy.charisma = 5;
        enemy.damageDice = 8; enemy.xpReward = 100; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 10, 3, playerLevel);
    } else if (roll <= 12) {
        strcpy(enemy.name, "Spider");
        enemy.strength = 10; enemy.dexterity = 16; enemy.constitution = 10;
        enemy.intelligence = 2; enemy.wisdom = 10; enemy.charisma = 4;
        enemy.damageDice = 6; enemy.xpReward = 125; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 8, 2, playerLevel);
    } else if (roll <= 15) {
        strcpy(enemy.name, "Bandit");
        enemy.strength = 14; enemy.dexterity = 12; enemy.constitution = 12;
        enemy.intelligence = 10; enemy.wisdom = 8; enemy.charisma = 10;
        enemy.damageDice = 8; enemy.xpReward = 200; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 10, 3, playerLevel);
    } else if (roll <= 17) {
        strcpy(enemy.name, "Necromancer");
        enemy.strength = 8; enemy.dexterity = 10; enemy.constitution = 10;
        enemy.intelligence = 16; enemy.wisdom = 12; enemy.charisma = 14;
        enemy.damageDice = 10; enemy.xpReward = 300; enemy.isSpellcaster = 1;
        initializeEnemyStats(&enemy, 8, 4, playerLevel);
    } else if (roll == 18) {
        strcpy(enemy.name, "Orc");
        enemy.strength = 16; enemy.dexterity = 12; enemy.constitution = 14;
        enemy.intelligence = 7; enemy.wisdom = 9; enemy.charisma = 8;
        enemy.damageDice = 10; enemy.xpReward = 350; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 12, 5, playerLevel);
    } else if (roll == 19) {
        strcpy(enemy.name, "Troll");
        enemy.strength = 18; enemy.dexterity = 10; enemy.constitution = 16;
        enemy.intelligence = 6; enemy.wisdom = 9; enemy.charisma = 7;
        enemy.damageDice = 12; enemy.xpReward = 500; enemy.isSpellcaster = 0;
        initializeEnemyStats(&enemy, 20, 7, playerLevel);
    } else {
        strcpy(enemy.name, "Shadow Mage");
        enemy.strength = 8; enemy.dexterity = 14; enemy.constitution = 12;
        enemy.intelligence = 18; enemy.wisdom = 14; enemy.charisma = 16;
        enemy.damageDice = 8; enemy.xpReward = 600; enemy.isSpellcaster = 1;
        initializeEnemyStats(&enemy, 12, 6, playerLevel);
    }

    return enemy;
}

Enemy createBoss(int playerLevel, const char *bossName) {
    Enemy boss;
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
        memset(boss.spellSlots, 0, sizeof(boss.spellSlots));
    } else {
        boss.strength = 12; boss.dexterity = 12; boss.constitution = 12;
        boss.intelligence = 12; boss.wisdom = 12; boss.charisma = 12;
        boss.damageDice = 10; boss.isSpellcaster = 0; boss.xpReward = 800;
        initializeEnemyStats(&boss, 8, 5, playerLevel);
        memset(boss.spellSlots, 0, sizeof(boss.spellSlots));
    }

    return boss;
}


int getExperienceForNextLevel(int level) {
    int expTable[] = {
        0,       // Level 1
        300,     // Level 2
        900,     // Level 3
        2700,    // Level 4
        6500,    // Level 5
        14000,   // Level 6
        23000,   // Level 7
        34000,   // Level 8
        48000,   // Level 9
        64000,   // Level 10
        85000,   // Level 11
        100000,  // Level 12
        120000,  // Level 13
        140000,  // Level 14
        165000,  // Level 15
        195000,  // Level 16
        225000,  // Level 17
        265000,  // Level 18
        305000,  // Level 19
        355000   // Level 20
    };

    return (level <= 20) ? expTable[level - 1] : expTable[19];  // Cap at level 20
}

void addExperience(Character *player, int xp) {
    player->experience += xp;
    printf("You gained %d experience points!\n", xp);

    while (player->experience >= getExperienceForNextLevel(player->level + 1)) {
        levelUp(player);
    }
}

void levelUp(Character *player) {
    player->level++;
    printf("\nCongratulations! You reached level %d!\n", player->level);

    // Gain HP based on class-specific hit dice
    int hpGain = rollDice(player->hitDiceValue) + getModifier(player->constitution);
    player->maxHp += (hpGain > 0) ? hpGain : 1;  // Ensure at least 1 HP gain
    player->hp = player->maxHp;

    if (player->level == 5) {
        if (strcmp(player->class, "Fighter") == 0 || strcmp(player->class, "Monk") == 0 || 
            strcmp(player->class, "Paladin") == 0 || strcmp(player->class, "Ranger") == 0 || 
            strcmp(player->class, "Barbarian") == 0) {
            player->extraAttack = 1;
            printf("You gained Extra Attack! You can now attack twice per turn.\n");
        }
    }

    // Check if Warlock for special slot recovery
    if (strcmp(player->class, "Warlock") == 0) {
        printf("Warlock spell slots refreshed.\n");
        initializeWarlockSlots(player);  // Short rest spell slot recovery
    } else if (player->isSpellcaster) {
        initializeSpellSlots(player);  // Update spell slots for other spellcasting classes
    }

    if (strcmp(player->class, "Monk") == 0) {
        initializeKiPoints(player);  // Update ki points for Monk
    }

    // Update action points for any classes using them
    if (hasAvailableActionPoints(player)) {
        player->actionPoints += getProficiencyBonus(player->level); // Add proficiency bonus for action points
    }
}

void initializeSpellSlots(Character *player) {
    for (int i = 0; i < 9; i++) {
        player->spellSlots[i] = 0;
    }

    if (strcmp(player->class, "Wizard") == 0 || strcmp(player->class, "Cleric") == 0) {
        initializeFullCasterSlots(player);
    } else if (strcmp(player->class, "Paladin") == 0 || strcmp(player->class, "Ranger") == 0 || strcmp(player->class, "Bard") == 0) {
        initializeHalfCasterSlots(player);
    } else if (strcmp(player->class, "Warlock") == 0) {
        initializeWarlockSlots(player);
    }
}

int hasAvailableSpellSlots(Character *player) {
    for (int i = 0; i < 10; i++) {
        if (player->spellSlots[i] > 0) return 1;  // Available slot
    }
    return 0;  // No slots available
}

void listSpells(Character *player) {
    int spellCount;
    Spell *spells = getSpellsForClass(player->class, &spellCount, player->level);


    if (spellCount == 0) {
        printf("No spells available for %s.\n", player->class);
        return;
    }

    printf("Available spells for %s:\n", player->class);
    for (int i = 0; i < spellCount; i++) {
        printf("%d. %s (%s)\n", i + 1, spells[i].name, spells[i].effect);
    }
}


Spell getSpellByClassAndChoice(Character *player, int spellChoice) {
    Spell spell;
    // Example: Retrieve spell based on class and choice
    printf("Retrieved spell choice %d for class %s.\n", spellChoice, player->class);
    return spell;
}

int validateSpellSlot(Character *player, int requestedSlot) {
    if (requestedSlot >= 0 && requestedSlot < 10 && player->spellSlots[requestedSlot] > 0) {
        return 1;  // Valid slot
    }
    return 0;  // Invalid slot
}

void castSpell(Character *player, Enemy *enemy, int slotLevel) {
    int spellCount;
    Spell *spells = getSpellsForClass(player->class, &spellCount, player->level);

    if (spellCount == 0) {
        printf("You have no spells available to cast.\n");
        return;
    }

    listSpells(player);  // Display available spells
    printf("Choose a spell to cast (enter the number): ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > spellCount) {
        printf("Invalid spell choice.\n");
        return;
    }

    Spell chosenSpell = spells[choice - 1];  // Get selected spell

    // Validate spell slot
    if (slotLevel < 1 || slotLevel > 9) {
        printf("Invalid slot level. Spell slots range from 1 to 9.\n");
        return;
    }

    if (player->spellSlots[slotLevel - 1] <= 0) {
        printf("You do not have any level %d spell slots available.\n", slotLevel);
        return;
    }

    // Cast the spell
    printf("%s casts %s on %s using a level %d slot.\n", player->name, chosenSpell.name, enemy->name, slotLevel);
    player->spellSlots[slotLevel - 1]--;

    // Apply spell effects
    if (strcmp(chosenSpell.effect, "Damage") == 0) {
        int damage = rollDice(chosenSpell.damageDice) + getModifier(player->intelligence);
        enemy->hp -= damage;
        enemy->hp = (enemy->hp < 0) ? 0 : enemy->hp;  // Clamp HP to 0
        printf("Dealt %d damage! Enemy HP: %d/%d\n", damage, enemy->hp, enemy->maxHp);
    } else if (strcmp(chosenSpell.effect, "Heal") == 0) {
        int heal = rollDice(chosenSpell.damageDice) + getModifier(player->wisdom);
        player->hp += heal;
        player->hp = (player->hp > player->maxHp) ? player->maxHp : player->hp;  // Clamp to max HP
        printf("Healed for %d HP! Current HP: %d/%d\n", heal, player->hp, player->maxHp);
    } else if (strcmp(chosenSpell.effect, "Buff") == 0) {
        printf("You feel empowered by %s.\n", chosenSpell.name);
        player->buffed = 1;  // Example effect
    } else if (strcmp(chosenSpell.effect, "Debuff") == 0) {
        printf("The enemy is weakened by %s.\n", chosenSpell.name);
        enemy->stunned = 1;  // Example effect
    } else if (strcmp(chosenSpell.effect, "Control") == 0) {
        printf("You control the battlefield with %s.\n", chosenSpell.name);
        enemy->stunned = 1;  // Example for fear or stun
    } else {
        printf("Spell effect applied: %s.\n", chosenSpell.effect);
    }
}


int hasAvailableActionPoints(Character *player) {
    return player->actionPoints > 0;
}


void initializeKiPoints(Character *player) {
    if (strcmp(player->class, "Monk") == 0) {
        player->maxKiPoints = player->level;  // Monks get ki points equal to their level
        player->kiPoints = player->maxKiPoints;
    }
}

void useKiPoint(Character *player, const char *abilityName) {
    if (player->kiPoints > 0) {
        player->kiPoints--;
        printf("Used %s! Remaining Ki Points: %d\n", abilityName, player->kiPoints);
    } else {
        printf("No Ki Points left!\n");
    }
}

void initializeFullCasterSlots(Character *player) {
    switch (player->level) {
        case 1:
            player->spellSlots[0] = 2;  // 2 Level 1 slots
            break;
        case 2:
            player->spellSlots[0] = 3;  // 3 Level 1 slots
            break;
        case 3:
            player->spellSlots[0] = 4;  // 4 Level 1 slots
            player->spellSlots[1] = 2;  // 2 Level 2 slots
            break;
        case 4:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;  // 3 Level 2 slots
            break;
        case 5:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 2;  // 2 Level 3 slots
            break;
        case 6:
        case 7:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;  // 3 Level 3 slots
            break;
        case 8:
        case 9:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 1;  // 1 Level 4 slot
            break;
        case 10:
        case 11:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 2;  // 2 Level 4 slots
            break;
        case 12:
        case 13:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;  // 3 Level 4 slots
            player->spellSlots[4] = 1;  // 1 Level 5 slot
            break;
        case 14:
        case 15:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;
            player->spellSlots[4] = 2;  // 2 Level 5 slots
            break;
        case 16:
        case 17:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;
            player->spellSlots[4] = 3;  // 3 Level 5 slots
            player->spellSlots[5] = 1;  // 1 Level 6 slot
            break;
        case 18:
        case 19:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;
            player->spellSlots[4] = 3;
            player->spellSlots[5] = 2;  // 2 Level 6 slots
            player->spellSlots[6] = 1;  // 1 Level 7 slot
            break;
        case 20:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;
            player->spellSlots[4] = 3;
            player->spellSlots[5] = 2;
            player->spellSlots[6] = 2;  // 2 Level 7 slots
            player->spellSlots[7] = 1;  // 1 Level 8 slot
            player->spellSlots[8] = 1;  // 1 Level 9 slot
            break;
    }
}


void initializeHalfCasterSlots(Character *player) {
    switch (player->level) {
        case 1:
            player->spellSlots[0] = 2;  // 2 Level 1 slots
            break;
        case 2:
            player->spellSlots[0] = 3;  // 3 Level 1 slots
            break;
        case 3:
        case 4:
            player->spellSlots[0] = 4;  // 4 Level 1 slots
            break;
        case 5:
        case 6:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 2;  // 2 Level 2 slots
            break;
        case 7:
        case 8:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;  // 3 Level 2 slots
            break;
        case 9:
        case 10:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 2;  // 2 Level 3 slots
            break;
        case 11:
        case 12:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;  // 3 Level 3 slots
            break;
        case 13:
        case 14:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 1;  // 1 Level 4 slot
            break;
        case 15:
        case 16:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 2;  // 2 Level 4 slots
            break;
        case 17:
        case 18:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;  // 3 Level 4 slots
            player->spellSlots[4] = 1;  // 1 Level 5 slot
            break;
        case 19:
        case 20:
            player->spellSlots[0] = 4;
            player->spellSlots[1] = 3;
            player->spellSlots[2] = 3;
            player->spellSlots[3] = 3;
            player->spellSlots[4] = 2;  // 2 Level 5 slots
            break;
    }
}


void initializeWarlockSlots(Character *player) {
    switch (player->level) {
        case 1:  player->spellSlots[0] = 1; break;
        case 2:  player->spellSlots[0] = 2; break;
        case 3:  player->spellSlots[1] = 2; break;
        case 5:  player->spellSlots[2] = 2; break;
        case 7:  player->spellSlots[3] = 2; break;
        case 9:  player->spellSlots[4] = 2; break;
        case 11: player->spellSlots[5] = 2; break;
        case 13: player->spellSlots[6] = 2; break;
        case 15: player->spellSlots[7] = 2; break;
        case 17: player->spellSlots[8] = 2; break;
        case 20: player->spellSlots[8] = 2; break;
    }
}

#define MAX_SPELLS 10


// Spell data structure for each class
Spell druidSpells[] = {
    // Cantrips
    {"Shillelagh", 0, "Buff", 1},
    {"Guidance", 0, "Buff", 1},
    {"Produce Flame", 6, "Damage", 1},
    
    // Level 1
    {"Entangle", 0, "Control", 1},
    {"Cure Wounds", 8, "Heal", 1},
    
    // Level 2
    {"Moonbeam", 10, "Damage", 3},
    {"Flaming Sphere", 10, "Damage", 3},

    // Level 3
    {"Call Lightning", 10, "Damage", 5},
    {"Water Breathing", 0, "Utility", 5},
    
    // Level 4
    {"Guardian of Nature", 0, "Buff", 7},
    {"Polymorph", 0, "Control", 7},
    
    // Level 5
    {"Wall of Thorns", 12, "Control", 9},
    {"Heal", 15, "Heal", 9},

    // Level 6-9
    {"Sunbeam", 10, "Damage", 11},
    {"Foresight", 0, "Buff", 17}
};


Spell rangerSpells[] = {
    // Cantrips
    {"Guided Arrow", 8, "Damage", 1},
    {"Hunter’s Trick", 0, "Utility", 1},

    // Level 1
    {"Hunter's Mark", 6, "Damage", 1},
    {"Cure Wounds", 8, "Heal", 1},

    // Level 2
    {"Spike Growth", 0, "Control", 3},
    {"Pass Without Trace", 0, "Utility", 3},

    // Level 3
    {"Conjure Barrage", 8, "Damage", 5},

    // Level 4
    {"Freedom of Movement", 0, "Utility", 7},
    
    // Level 5+
    {"Conjure Volley", 8, "Damage", 9},
    {"Swift Quiver", 0, "Buff", 11}
};


Spell paladinSpells[] = {
    // Level 1
    {"Bless", 0, "Buff", 1},
    {"Divine Smite", 8, "Damage", 2},

    // Level 2
    {"Lay on Hands", 10, "Heal", 3},
    {"Protection from Evil", 0, "Buff", 3},

    // Level 3
    {"Spirit Guardians", 8, "Damage", 5},

    // Level 4
    {"Aura of Vitality", 10, "Heal", 7},

    // Level 5+
    {"Banishing Smite", 10, "Damage", 11},
    {"Circle of Power", 0, "Buff", 13},
};

Spell bardSpells[] = {
    // Cantrips
    {"Vicious Mockery", 4, "Damage", 1},
    {"Inspire Courage", 0, "Buff", 1},

    // Level 1
    {"Healing Word", 4, "Heal", 1},
    {"Charm Person", 0, "Control", 1},

    // Level 2
    {"Silence", 0, "Control", 3},
    {"Heat Metal", 8, "Damage", 3},

    // Level 3
    {"Hypnotic Pattern", 0, "Control", 5},

    // Level 4+
    {"Greater Invisibility", 0, "Buff", 7},
    {"Mass Cure Wounds", 10, "Heal", 9},
};


Spell sorcererSpells[] = {
    // Cantrips
    {"Fire Bolt", 8, "Damage", 1},
    {"Prestidigitation", 0, "Utility", 1},

    // Level 1
    {"Magic Missile", 4, "Damage", 1},
    {"Shield", 0, "Buff", 1},

    // Level 2
    {"Burning Hands", 8, "Damage", 3},
    {"Mirror Image", 0, "Buff", 3},

    // Level 3
    {"Counterspell", 0, "Utility", 5},
    {"Fireball", 8, "Damage", 5},

    // Level 4+
    {"Greater Polymorph", 0, "Control", 7},
    {"Wish", 0, "Utility", 17},
};


Spell wizardSpells[] = {
    // Cantrips
    {"Mage Hand", 0, "Utility", 1},
    {"Ray of Frost", 8, "Damage", 1},

    // Level 1
    {"Magic Missile", 4, "Damage", 1},
    {"Mage Armor", 0, "Buff", 1},

    // Level 2
    {"Misty Step", 0, "Utility", 3},
    {"Scorching Ray", 6, "Damage", 3},

    // Level 3
    {"Fireball", 8, "Damage", 5},
    {"Counterspell", 0, "Utility", 5},

    // Level 4+
    {"Dimension Door", 0, "Utility", 7},
    {"Meteor Swarm", 20, "Damage", 19},
};


Spell clericSpells[] = {
    // Cantrips
    {"Sacred Flame", 8, "Damage", 1},
    {"Light", 0, "Utility", 1},

    // Level 1
    {"Cure Wounds", 8, "Heal", 1},
    {"Bless", 0, "Buff", 1},

    // Level 2
    {"Spiritual Weapon", 8, "Damage", 3},
    {"Hold Person", 0, "Control", 3},

    // Level 3+
    {"Revivify", 0, "Heal", 5},
    {"Guardian of Faith", 10, "Damage", 7},
};


Spell warlockSpells[] = {
    // Cantrips
    {"Eldritch Blast", 10, "Damage", 1},
    {"Mage Hand", 0, "Utility", 1},

    // Level 1
    {"Hex", 0, "Debuff", 1},
    {"Hellish Rebuke", 8, "Damage", 1},

    // Level 2
    {"Darkness", 0, "Control", 3},
    {"Misty Step", 0, "Utility", 3},

    // Level 3+
    {"Counterspell", 0, "Utility", 5},
    {"Hunger of Hadar", 8, "Damage", 9},
};


Spell monkAbilities[] = {
    {"Flurry of Blows", 0, "Ability", 1},
    {"Stunning Strike", 0, "Ability", 3},
    {"Patient Defense", 0, "Buff", 5},
    {"Step of the Wind", 0, "Utility", 7},
    {"Empty Body", 0, "Buff", 18},
};


// Retrieve spells/abilities based on class

Spell *getSpellsForClass(const char *className, int *spellCount, int characterLevel) {
    static Spell availableSpells[50];  // Temporary array for filtered spells
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

    // Filter spells based on character level
    for (int i = 0; i < totalSpells; i++) {
        if (allSpells[i].requiredLevel <= characterLevel) {
            availableSpells[count++] = allSpells[i];
        }
    }

    *spellCount = count;
    return availableSpells;
}



// Fighters, Barbarians and Monk Resource System
void useActionPoint(Character *player, const char *ability) {
    if (player->actionPoints > 0) {
        player->actionPoints--;
        printf("%s used. Remaining Action Points: %d\n", ability, player->actionPoints);
    } else {
        printf("No Action Points left.\n");
    }
}
void useRage(Character *player) {
    if (player->ragePoints > 0) {
        player->ragePoints--;
        printf("Rage activated! Remaining Rage Points: %d\n", player->ragePoints);
    } else {
        printf("No Rage Points left.\n");
    }
}

// Manage Buffs
void applyBuff(Character *player, const char *buffName) {
    printf("Applying %s buff...\n", buffName);
    if (strcmp(buffName, "Bless") == 0) {
        player->strength += 2;
        player->buffed = 1;
        player->buffDuration = 3;  // Buff lasts 3 turns
        printf("Bless applied! Strength increased by 2 for 3 turns.\n");
    } else if (strcmp(buffName, "Inspire Courage") == 0) {
        player->armorClass += 1;
        player->buffed = 1;
        player->buffDuration = 3;  // Buff lasts 3 turns
        printf("Inspire Courage applied! AC increased by 1 for 3 turns.\n");
    } else if (strcmp(buffName, "Patient Defense") == 0) {
        printf("You take the Dodge action as a bonus action!\n");
    } else if (strcmp(buffName, "Step of the Wind") == 0) {
        printf("Your jump distance is doubled, and you can Dash as a bonus action.\n");
    } else {
        printf("Unknown buff effect.\n");
    }
}

// Manage active buffs
void manageBuffs(Character *player) {
    if (player->buffed && player->buffDuration > 0) {
        player->buffDuration--;
        if (player->buffDuration == 0) {
            removeBuff(player);
        }
    }
}

void removeBuff(Character *player) {
    printf("Buff expired! Reverting stats to normal.\n");
    if (player->strength > 10) {  // Safeguard against negative stats
        player->strength -= 2;
    }
    if (player->armorClass > 10) {  // Safeguard against negative AC
        player->armorClass -= 1;
    }
    player->buffed = 0;
}

// Monk's Flurry of Blows
void flurryOfBlows(Character *player, Enemy *enemy) {
    if (player->kiPoints < 1) {
        printf("Not enough Ki Points for Flurry of Blows.\n");
        return;
    }

    player->kiPoints--;  // Deduct Ki Point
    printf("\nUsing Flurry of Blows! Remaining Ki Points: %d\n", player->kiPoints);

    for (int i = 0; i < 2; i++) {
        int attackRoll = rollDice(20) + getModifier(player->dexterity);
        printf("Unarmed Strike %d Attack Roll: %d (vs AC %d)\n", i + 1, attackRoll, enemy->armorClass);
        
        if (attackRoll >= enemy->armorClass) {
            int damage = rollDice(4) + getModifier(player->dexterity);  // 1d4 + Dex for unarmed strike
            enemy->hp -= damage;
            printf("Unarmed Strike %d: Hit! Dealt %d damage. Enemy HP: %d/%d\n", i + 1, damage, enemy->hp, enemy->maxHp);
        } else {
            printf("Unarmed Strike %d: Missed!\n", i + 1);
        }
    }
}

// Monk's Stunning Strike
void stunningStrike(Character *player, Enemy *enemy) {
    if (player->kiPoints < 1) {
        printf("Not enough Ki Points for Stunning Strike.\n");
        return;
    }

    player->kiPoints--;
    printf("\nUsing Stunning Strike! Remaining Ki Points: %d\n", player->kiPoints);

    int saveRoll = rollDice(20) + getModifier(enemy->constitution);
    int saveDC = 8 + getProficiencyBonus(player->level) + getModifier(player->wisdom);  // Save DC = 8 + Prof + Wis

    printf("Enemy's Constitution Save Roll: %d (vs DC %d)\n", saveRoll, saveDC);

    if (saveRoll < saveDC) {
        enemy->stunned = 1;
        printf("Enemy is stunned until the end of your next turn!\n");
    } else {
        printf("Enemy resisted the Stunning Strike.\n");
    }
}



// Generate loot based on enemy and player level
void generateLoot(Enemy *enemy, Item *loot[], int *lootCount, int playerLevel) {
    int rollD20 = rollDice(20);
    int coins = rollD20 * playerLevel;  // Base coins scaled by player level

    int copper = coins * 100;  // Convert to copper base
    int silver = copper / 100;
    copper %= 100;
    int gold = silver / 10;
    silver %= 10;

    *lootCount = 0;

    // Add gold, silver, copper loot
    loot[*lootCount] = malloc(sizeof(Item));
    strcpy(loot[*lootCount]->name, "Gold Coins");
    loot[*lootCount]->type = ITEM_TYPE_GOLD;
    loot[*lootCount]->value = gold;
    (*lootCount)++;

    loot[*lootCount] = malloc(sizeof(Item));
    strcpy(loot[*lootCount]->name, "Silver Coins");
    loot[*lootCount]->type = ITEM_TYPE_GOLD;
    loot[*lootCount]->value = silver;
    (*lootCount)++;

    loot[*lootCount] = malloc(sizeof(Item));
    strcpy(loot[*lootCount]->name, "Copper Coins");
    loot[*lootCount]->type = ITEM_TYPE_GOLD;
    loot[*lootCount]->value = copper;
    (*lootCount)++;

    // Roll a d100 for additional loot
    int rollD100 = rollDice(100);
    if (rollD100 <= 30) {
        // Drop additional coins
        printf("The enemy drops extra coins!\n");
    } else if (rollD100 <= 90) {
        // Drop miscellaneous items
        loot[*lootCount] = malloc(sizeof(Item));
        strcpy(loot[*lootCount]->name, "Health Potion");
        loot[*lootCount]->type = ITEM_TYPE_POTION;
        loot[*lootCount]->effect = 10;  // Heals 10 HP
        (*lootCount)++;
    } else {
        // Drop magic items
        loot[*lootCount] = malloc(sizeof(Item));
        strcpy(loot[*lootCount]->name, "Magic Sword");
        loot[*lootCount]->type = ITEM_TYPE_MAGIC_ITEM;
        loot[*lootCount]->value = 100;  // Worth in gold
        (*lootCount)++;
    }
}

// Display loot collected
void displayLoot(Item *loot[], int lootCount) {
    printf("\nLoot Found:\n");
    for (int i = 0; i < lootCount; i++) {
        printf("%d. %s (%s) Value: %d\n", i + 1, loot[i]->name, 
               loot[i]->type == ITEM_TYPE_GOLD ? "Coins" : "Item", loot[i]->value);
    }
}

// Add an item to the player's inventory
void addItemToInventory(Character *player, Item *item) {
    if (player->inventoryCount < 50) {
        player->inventory[player->inventoryCount++] = *item;
        printf("Added %s to inventory.\n", item->name);
    } else {
        printf("Inventory is full!\n");
    }
}

// Show the player's current inventory
void showInventory(Character *player) {
    printf("\nInventory:\n");
    for (int i = 0; i < player->inventoryCount; i++) {
        printf("%d. %s (%s) Value: %d\n", i + 1, 
               player->inventory[i].name, 
               player->inventory[i].type == ITEM_TYPE_GOLD ? "Coins" : "Item", 
               player->inventory[i].value);
    }
}

// Use a potion from the inventory
void usePotion(Character *player) {
    for (int i = 0; i < player->inventoryCount; i++) {
        if (player->inventory[i].type == ITEM_TYPE_POTION) {
            printf("You used %s.\n", player->inventory[i].name);
            player->hp += player->inventory[i].effect;
            if (player->hp > player->maxHp) {
                player->hp = player->maxHp;
            }

            // Remove potion from inventory
            for (int j = i; j < player->inventoryCount - 1; j++) {
                player->inventory[j] = player->inventory[j + 1];
            }
            player->inventoryCount--;
            return;
        }
    }
    printf("No potions available!\n");
}

// Use an item based on its type
void useItem(Character *player, ItemType type) {
    for (int i = 0; i < player->inventoryCount; i++) {
        if (player->inventory[i].type == type) {
            if (type == ITEM_TYPE_POTION) {
                printf("You used %s.\n", player->inventory[i].name);
                player->hp += player->inventory[i].effect;
                if (player->hp > player->maxHp) {
                    player->hp = player->maxHp;
                }
                // Remove item from inventory
                for (int j = i; j < player->inventoryCount - 1; j++) {
                    player->inventory[j] = player->inventory[j + 1];
                }
                player->inventoryCount--;
                return;
            }
        }
    }
    printf("No item of the specified type found!\n");
}

// Main game loop and character management
int main() {
    srand(time(0));  // Initialize random seed

    printf("Do you want to load an existing character or create a new one?\n");
    printf("1. Load Character\n2. Create New Character\n");
    int choice;
    scanf("%d", &choice);

    Character player;
    if (choice == 1) {
        listSavedCharacters();
        printf("Enter the name of the character to load: ");
        char fileName[50];
        scanf("%s", fileName);
        strcat(fileName, ".txt");
        player = loadCharacterByName(fileName);
    } else {
        player = createCharacter();
        saveCharacterProgress(&player);
    }

    int battlesWon = 0; // Track battles won by the player

    while (1) {
        Enemy enemy;

        // Spawn special enemies based on battlesWon
        if (battlesWon == 10 && player.level >= 3) {
            enemy = createBoss(player.level, "Necromancer");
        } else if (battlesWon == 20 && player.level >= 5) {
            enemy = createBoss(player.level, "Shadow Lord");
        } else {
            enemy = createEnemy(player.level);
        }

        printf("\nWhile delving deeper into the dungeon, a %s appears! HP: %d, AC: %d\n", enemy.name, enemy.hp, enemy.armorClass);

        startBattle(&player, &enemy);

        if (player.hp <= 0) {
            if (player.hardcore) {
                printf("\nYou have been defeated. Hardcore mode active. Deleting save file...\n");
                deleteSaveFile(player.name);
                printf("Game Over. Save file deleted.\n");
                break;
            } else {
                printf("\nYou have been defeated.\n");
                printf("Do you want to:\n1. Restart from Level 1\n2. Retry from the previous level\n");
                int deathChoice;
                scanf("%d", &deathChoice);

                if (deathChoice == 1) {
                    printf("Restarting from Level 1...\n");
                    player.level = 1;
                    player.experience = 0;
                    initializeCharacterStats(&player);
                } else {
                    printf("Retrying from Level %d...\n", player.level - 1);
                    player.level--;
                    initializeCharacterStats(&player);
                }
            }
        } else {
            battlesWon++; // Increment battles won
            printf("\nBattles Won: %d\n", battlesWon);

            printf("\nChoose your next action:\n1. Go deeper into the dungeon\n2. Rest\n");
            int nextChoice;
            scanf("%d", &nextChoice);

            if (nextChoice == 2) {
                printf("\nChoose rest type:\n1. Short Rest\n2. Long Rest\n");
                int restType;
                scanf("%d", &restType);

                if (restType == 1) {
                    shortRest(&player);
                } else {
                    longRest(&player);
                }
            }
        }
        saveCharacterProgress(&player);  // Save progress after each action
    }

    return 0;
}

// Function to delete a save file
void deleteSaveFile(const char *fileName) {
    char fullName[60];
    snprintf(fullName, sizeof(fullName), "%s.txt", fileName);  // Add .txt
    if (remove(fullName) == 0) {
        printf("Save file %s deleted successfully.\n", fullName);
    } else {
        printf("Error deleting save file %s.\n", fullName);
    }
}


// Function to list saved characters with their details
void listSavedCharacters() {
    printf("\nAvailable saved characters:\n");
    struct dirent *entry;
    DIR *dp = opendir(".");
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".txt")) {
            char characterName[50];
            sscanf(entry->d_name, "%[^.]", characterName);  // Extract name without .txt
            FILE *file = fopen(entry->d_name, "r");
            if (file != NULL) {
                Character temp;
                fscanf(file, "%s %s %s %s %d",
                       temp.name, temp.race, temp.sex, temp.class, &temp.level);
                printf("%-20s | Class: %-8s | Level: %d\n", characterName, temp.class, temp.level);
                fclose(file);
            }
        }
    }
    closedir(dp);
}

// Function for short rest
void shortRest(Character *player) {
    if (player->hitDice <= 0) {
        printf("You have no hit dice left. You must take a long rest.\n");
        return;
    }

    printf("\nShort Rest: You have %dd%d hit dice available.\n", player->hitDice, player->hitDiceValue);
    printf("Enter the number of hit dice to use: ");
    int diceToUse;
    scanf("%d", &diceToUse);

    if (diceToUse > player->hitDice) {
        printf("You don't have enough hit dice. Using maximum available (%d).\n", player->hitDice);
        diceToUse = player->hitDice;
    }

    int totalHeal = 0;
    for (int i = 0; i < diceToUse; i++) {
        int heal = rollDice(player->hitDiceValue) + getModifier(player->constitution);
        totalHeal += (heal > 0) ? heal : 1;  // Minimum 1 HP per die
    }

    player->hp += totalHeal;
    if (player->hp > player->maxHp) {
        player->hp = player->maxHp;
    }

    player->hitDice -= diceToUse;

    if (strcmp(player->class, "Warlock") == 0) {
        printf("Restoring Warlock spell slots...\n");
        player->spellSlots[player->level / 2] = 2;  // Highest available slots
    }

    if (strcmp(player->class, "Fighter") == 0 && player->actionPoints < 1) {
        player->actionPoints = 1;
        printf("Action Points restored to 1.\n");
    }

    if (strcmp(player->class, "Barbarian") == 0 && player->ragePoints < 2) {
        player->ragePoints = 2;
        printf("Rage Points restored to 2.\n");
    }

    if (strcmp(player->class, "Monk") == 0) {
        player->kiPoints = player->maxKiPoints;
        printf("Ki Points fully restored! Current Ki Points: %d\n", player->kiPoints);
    }

    printf("You healed for %d HP. Current HP: %d/%d\n", totalHeal, player->hp, player->maxHp);
}

// Function for long rest
void longRest(Character *player) {
    printf("\nLong Rest: Resting for 8 hours...\n");

    int roll = rollDice(100);
    if (roll <= 20) {
        printf("Enemies attack during your rest!\n");
        Enemy enemy = createEnemy(player->level);
        printf("\nA wild %s interrupts your rest! HP: %d, AC: %d\n", enemy.name, enemy.hp, enemy.armorClass);
        startBattle(player, &enemy);

        if (player->hp <= 0) {
            printf("\nYou have been defeated during your rest. Game over.\n");
            exit(0);
        }
    }

    player->hp = player->maxHp;
    player->hitDice = player->level;

    if (strcmp(player->class, "Fighter") == 0) {
        player->actionPoints = 2;
        printf("Action Points fully restored.\n");
    }

    if (strcmp(player->class, "Barbarian") == 0) {
        player->ragePoints = player->level >= 5 ? 3 : 2;
        printf("Rage Points fully restored.\n");
    }

    printf("Restoring spell slots...\n");
    if (strcmp(player->class, "Warlock") != 0) {
        initializeSpellSlots(player);
    }

    printf("You feel fully rested. HP: %d/%d, Hit Dice: %dd%d restored.\n",
           player->hp, player->maxHp, player->hitDice, player->hitDiceValue);
}