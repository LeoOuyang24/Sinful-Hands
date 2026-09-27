#include "../include/card_repo.h"
#include "../include/options.h"

std::unordered_map<Card::CardIdentifier,CardPtr> CardsLookup::CardsRepo;

CardPtr CardsLookup::getCard(Card::CardIdentifier id)
{
    if (CardsRepo.find(id) != CardsRepo.end())
    {
        Card* ptr = CardsRepo[id].get();
        if (ptr && ptr->isEnemy)
        {
            return CardPtr(new EnemyCard(*static_cast<EnemyCard*>(ptr)));
        }
        return CardPtr(new Card(*ptr));
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

    addCard(*(new EnemyCard("Kind Grandma",{},{
        Option({Actions::AddCardsToHand({"Apple"})},{0,0,3}),
        Option({Actions::Skip()},{}),
        Option({Actions::Sin()},{})
    })));
    addCard(*(new EnemyCard("Wrathful Spirit",{},
                {Option({},{0,5,0}),
                Option({Actions::Sin(),Actions::TakeDamage(1)},{})}
                )));
    addCard(*(new EnemyCard("Famished Spirit",{},
                {Option({},{5,0,0}),
                Option({Actions::Sin(),Actions::TakeDamage(1)},{})}
                )));
    addCard(*(new EnemyCard("Greedy Spirit",{},
                {Option({},{0,0,5}),
                Option({Actions::Sin(),Actions::TakeDamage(1)},{})}
                )));
}

void CardsLookup::addCard(Card& card)
{
    if (CardsRepo.find(card.name) != CardsRepo.end())
    {
        std::cerr << "Refusing to add card with duplicate id: " << card.name << "\n";
    }
    else
    {
        card.renderCardBody({},0);
        CardsRepo[card.name] = CardPtr(&card);
        CardsRepo[card.name]->renderCardBody({},0);
    }
}