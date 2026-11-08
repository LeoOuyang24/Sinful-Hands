#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

#include "cards.h"
#include "player.h"

struct Globals
{
    static Player player;
    static Interface interface;

    static void reset();

    static void addMetaCard(Card::CardIdentifier);

private:
    static std::vector<Card::CardIdentifier> metaCards; //ADDITIONAL cards to add at the start
};

#endif // GLOBALS_H