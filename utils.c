#include <stdlib.h>

int rollDice(int sides) {
    return (rand() % sides) + 1;
}

int getModifier(int stat) {
    return (stat - 10) / 2;
}
