#ifndef CARD_REPO_H
#define CARD_REPO_H

#include <unordered_map>

#include "cards.h"

struct CardsLookup
{
    static std::unordered_map<Card::CardIdentifier,CardPtr> CardsRepo;

    static CardPtr getCard(Card::CardIdentifier);
    static void loadCards();
private:
    static void addCard(Card& card);
};

#endif // CARD_REPO_H