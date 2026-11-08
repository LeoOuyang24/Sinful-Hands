#include "../include/globals.h"
#include "../include/card_repo.h"

Player Globals::player;
Interface Globals::interface;

std::vector<Card::CardIdentifier> Globals::metaCards;

void Globals::addMetaCard(Card::CardIdentifier id)
{
    metaCards.push_back(id);
}

void Globals::reset()
{
    player.reset();
    for (int i = 0; i < 2; i++)
    {
        player.addCard("Potato", true);
        player.addCard("Dagger",true);
        player.addCard("Allowance",true);
    }

    for (int i = 0; i < metaCards.size(); i ++)
    {
        player.addCard(metaCards[i],true);
    }

    metaCards.clear();

    for (int i = 0; i < CardsLookup::startingDeck.size(); i ++)
    {
        player.addCardToDeck(CardsLookup::startingDeck[i]);
    }

    player.currentEnemy = CardsLookup::getCard("Kind Vendor"); 
    player.deck.insert(player.deck.begin(),CardsLookup::getCard("Castle Gates"));
    player.deck.insert(player.deck.begin() + 1, CardsLookup::getCard("Fork in the Road"));
}