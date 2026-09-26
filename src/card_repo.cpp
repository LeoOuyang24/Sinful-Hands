#include "../include/card_repo.h"

std::unordered_map<Card::CardIdentifier,CardPtr> CardsLookup::CardsRepo;

CardPtr CardsLookup::getCard(Card::CardIdentifier id)
{
    if (CardsRepo.find(id) != CardsRepo.end())
    {
        return CardPtr(new Card(*CardsRepo[id].get()));
    }
    else
    {
        std::cerr << "Unable to fetch card: " << id << "\n";
        return nullptr;
    }
}

void CardsLookup::loadCards()
{
    addCard(*(new Card("Apple",{},GRAY,{10,5,1})));
}

void CardsLookup::addCard(Card& card)
{
    if (CardsRepo.find(card.name) != CardsRepo.end())
    {
        std::cerr << "Refusing to add card with duplicate id: " << card.name << "\n";
    }
    else
    {
        CardsRepo[card.name] = CardPtr(&card);
    }
}