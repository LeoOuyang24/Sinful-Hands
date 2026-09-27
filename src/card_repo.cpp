
#include "../include/card_repo.h"
#include "../include/options.h"

std::unordered_map<Card::CardIdentifier,CardPtr> CardsLookup::CardsRepo;
std::vector<Card::CardIdentifier> CardsLookup::startingDeck;

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
    addCard(*(new EnemyCard("Castle Gates",{
        {
            Option({Actions::Win()},{0,20,0}),
            Option({Actions::Win()},{0,0,20}),
            Option({Actions::AddCardsToDeck({"Castle Gates"})},{})       
        }
    })),true);

    addCard(*(new EnemyCard("Kind Vendor",
        {
            Option({Actions::AddCardsToHand({"Apple"})},{}),
            Option({Actions::AddCardsToHand({"Apple","Potato"})},{0,0,3}),
            Option({Actions::Sin(),Actions::AddCardsToHand({"Apple","Potato"})},{})
        })));

    
    addCard(*(new EnemyCard("Wrathful Spirit",
                {Option({Actions::Skip()},{0,5,0}),
                Option({Actions::Sin(),Actions::TakeDamage(1)},{})}
                )));
    addCard(*(new EnemyCard("Famished Spirit",
                {Option({Actions::Skip()},{5,0,0}),
                Option({Actions::Sin(),Actions::TakeDamage(1)},{})}
                )));
    addCard(*(new EnemyCard("Greedy Spirit",
                {Option({Actions::Skip()},{0,0,5}),
                Option({Actions::Sin(),Actions::TakeDamage(1)},{})}
                )));

    addCard(*(new EnemyCard("Distant Dragon",
            {
                Option({Actions::Skip()},{0,5,0}),
                Option({Actions::AddCardsToDeck({"Looming Dragon"})},{})    
            }
            )),true);
    addCard(*(new EnemyCard("Looming Dragon",
            {
                Option({Actions::Skip()},{0,10,0}),
                Option({Actions::AddCardsToDeck({"Hungry Dragon"})},{})    
            }
            )));

    addCard(*(new EnemyCard("Hungry Dragon",
            {
                Option({Actions::Skip()},{0,15,0}),
                Option({Actions::Die()},{})    
            }
            )));
    addCard(*(new EnemyCard("Border Guard",
            {
                Option({Actions::Skip()},{0,0,10}),
                Option({Actions::Skip()},{0,8,0}),
                Option({Actions::Skip(),Actions::TakeDamage(1)},{})
            }
    )),true);

    addCard(*(new EnemyCard("Exorcist",
        {
            Option({Actions::Cleanse("Wrathful Spirit")},{}),
            Option({Actions::Cleanse("Famished Spirit")},{}),
            Option({Actions::Cleanse("Greedy Spirit")},{}),
        }
    )),true);
    addCard(*(new EnemyCard("Poorly Drawn Bandit",
            {
                Option({Actions::Skip()},{0,4,0}),
                Option({Actions::AddCardsToDeck({"Poorly Drawn Bandit"})},{0,0,1}),
                Option({Actions::Skip(),Actions::TakeDamage(1)},{})
            }
    )),true);
    addCard(*(new EnemyCard("Religious Cow",
        {
            Option({Actions::AddCardsToHand({"Milk"})},{}),
            Option({Actions::AddCardsToHand({"Steak"})},{0,3,0}),
            Option({Actions::AddCardsToDeck({"Holy Cow"})},{})
        } 
    )),true);
    addCard(*(new EnemyCard("Holy Cow",
        {
            Option({Actions::AddCardsToHand({"Golden Apple"})},{}),
            Option({Actions::AddCardsToHand({"Sharp Cheddar"})},{}),
            Option({Actions::Sin(),Actions::AddCardsToHand({"Golden Apple","Sharp Cheddar"})})
        }
    )));
    addCard(*(new EnemyCard("Starving Wolf",
        {
            Option({Actions::TakeDamage(1)},{}),
            Option({Actions::AddCardsToHand({"Allowance"})},{0,3,0}),
            Option({Actions::AddCardsToDeck({"Grateful Wolf"})},{3,0,0})
        })),true);
    addCard(*(new EnemyCard("Grateful Wolf",
        {
            Option({Actions::AddCardsToHand({"Sword"})},{}),
            Option({Actions::AddCardsToHand({"Steak"})},{}),
            Option({Actions::Heal(1)}),
        }
    )));

    addCard(*(new EnemyCard("Orphans",
        {
            Option({Actions::Skip()},{0,0,2}),
            Option({Actions::Skip()},{2,0,0}),
            Option({Actions::Sin(),Actions::AddCardsToHand({"Mystery Meat","Mystery Meat"})},{})            
        }
    )),true);
    
    addCard(*(new EnemyCard("Blacksmith",
        {
            Option({Actions::AddCardsToHand({"Dagger"})},{}),
            Option({Actions::AddCardsToHand({"Sword"})},{0,0,5}),
            Option({Actions::Sin(),Actions::AddCardsToHand({"Dagger","Sword"})},{})
        }
    )),true);

    addCard(*(new EnemyCard("Joyful Genie",
        {
            Option({Actions::AddCardsToHand({"Dagger","Dagger","Dagger"})},{}),
            Option({Actions::AddCardsToHand({"Potato","Potato","Potato"})},{}),
            Option({Actions::AddCardsToHand({"Allowance","Allowance","Allowance"})},{})
        }
    )),true);
    addCard(*(new EnemyCard("Giving Tree",
        {
            Option({Actions::AddCardsToHand({"Apple"})},{}),
            Option({Actions::AddCardsToHand({"Apple","Apple","Apple"})},{0,3,0}),
            Option({Actions::Heal(1)},{})
        }
    )),true);
    addCard(*(new EnemyCard("Locked Chest",
        {
            Option({Actions::AddCardsToHand({"Allowance","Allowance"})},{0,3,0}),
            Option({Actions::AddCardsToDeck({"Treasure Map"})},{0,3,0}),
            Option({Actions::Skip()},{})
        }
    )),true);
    addCard(*(new EnemyCard("Trapped Chest",
            {
            Option({Actions::AddCardsToHand({"Allowance","Allowance"})},{0,3,0}),
            Option({Actions::AddCardsToDeck({"Treasure Map"})},{0,3,0}),
            Option({Actions::Skip()},{})
        }
    )),true);
    addCard(*(new EnemyCard("Treasure Map",
        {
            Option({Actions::AddCardsToHand({"Emerald","Emerald"})},{}),
            Option({Actions::Heal(1)},{}),
            Option({Actions::AddCardsToDeck({"True Treasure"})},{})
        }
    )));
    addCard(*(new EnemyCard("True Treasure",
        {
            Option({Actions::AddCardsToHand({"Diamond","Diamond","Diamond"})},{}),
            Option({Actions::Heal(3)},{})
        })));
    addCard(*(new EnemyCard("The Elements",
        {
            Option({Actions::AddCardsToDeck({"The Elements"}),Actions::TakeDamage(1)},{}),
            Option({Actions::AddCardsToDeck({"The Elements"})},{1,0,0}),
            Option({Actions::AddCardsToDeck({"The Elements"})},{0,0,1})
        })),true);
    addCard(*(new EnemyCard("Job Offering",
        {
            Option({Actions::AddCardsToHand({"Allowance"})},{}),
            Option({Actions::AddCardsToDeck({"Starving Wolf","Starving Wolf"})},{}),
            Option({Actions::AddCardsToDeck({"Cultist"})},{})
        }
    )),true);
    addCard(*(new EnemyCard("Cultist",
        {
            Option({Actions::AddCardsToHand({"Allowance","Emerald"})},{0,5,0}),
            Option({Actions::AddCardsToDeck({"Demon"})},{0,5,0}),
            Option({Actions::Sin()},{})
        }
    )));
    addCard(*(new EnemyCard("Demon",
        {
            Option({Actions::TakeDamage(2)},{}),
            Option({Actions::Sin(),Actions::Sin(),Actions::Sin()},{}),
            Option({Actions::AddCardsToHand({"Demon Heart","Diamond","Sword"})},{0,15,0})
        }
    )));
    addCard(*(new EnemyCard("Lush Garden",
        {
            Option({Actions::AddCardsToHand({"Apple","Potato"})},{}),
            Option({Actions::AddCardsToDeck({"Potato Tree"})},{}),
            Option({Actions::Heal(1)})
        }
    )),true);
    addCard(*(new EnemyCard("Potato Tree",
        {
            Option({Actions::AddCardsToHand({"Potato", "Potato"})},{}),
            Option({Actions::AddCardsToHand({"Allowance", "Allowance", "Allowance"})},{}),
            Option({Actions::AddCardsToDeck({"Potato Tree","Potato Tree"})},{2,0,0})
        }
    )));
    addCard(*(new EnemyCard("Evil Book",
        {
            Option({Actions::AddCardsToHand({"Allowance"})},{0,1,0}),
            Option({Actions::AddCardsToDeck({"Forbidden Arts"})},{}),
            Option({Actions::TakeDamage(1)},{})
        }
    )),true);
    addCard(*(new EnemyCard("Forbidden Arts",
        {
            Option({Actions::ForbiddenArt(), Actions::AddCardsToHand({"Emerald","Emerald"}),Actions::TakeDamage(1)}),
            Option({Actions::ForbiddenArt(), Actions::AddCardsToHand({"Sharp Claws","Sharp Claws"}),Actions::TakeDamage(1)}),
            Option({Actions::Sin()})        
        }
    )));

    addCard(*(new EnemyCard("Holy Fountain",
        {
            Option({Actions::Heal(3)}),
            Option({Actions::AddCardsToHand({"Allowance","Potato","Dagger"})}),
            Option({Actions::AddCardsToDeck({"Exorcist","Exorcist"})})        
        }        
    )),true);
    addCard(*(new EnemyCard("Roadside Corpse",
        {
            Option({Actions::AddCardsToHand({"Single Coin","Single Coin"})}),
            Option({Actions::AddCardsToHand({"Apple", "Apple"})}),
            Option({Actions::AddCardsToHand({"Dogshit Dagger","Dogshit Dagger"})})
        }
    )),true);

    addCard(*(new EnemyCard("Mysterious Egg",
        {
            Option({Actions::AddCardsToHand({"Omelette"})}),
            Option({Actions::Sin(),Actions::Heal(2)}),
            Option({Actions::AddCardsToDeck({"Phoenix"})})
        }
    )),true);

    addCard(*(new EnemyCard("Phoenix",
        {
            Option({Actions::Cleanse("Greedy Spirit")}),
            Option({Actions::Heal(3)}),
            Option({Actions::AddCardsToHand({"Sharp Claws","Sharp Claws"})})
        }
    )));

    addCard(*(new EnemyCard("Cursed Idol",
        {
            Option({Actions::Heal(1),Actions::LoseHand()}),
            Option({Actions::Skip()},{0,1,0}),
            Option({Actions::Sin()})
        }
    )),true);

    addCard(*(new EnemyCard("Malicious Idol",
        {
            Option({Actions::Heal(1),Actions::LoseHand()}),
            Option({Actions::Sin()},{0,5,0})
        }
    )));

    addCard(*(new EnemyCard("Consuming Idol",
        {
            Option({Actions::Heal(1),Actions::LoseHand()}),
            Option({Actions::Sin()},{0,10,0}),
            Option({Actions::Die()})
        }
    )));

    addCard(*(new Card("Potato",{3,0,0})));
    addCard(*(new Card("Apple",{1,0,0})));
    addCard(*(new Card("Dagger",{0,3,0})));
    addCard(*(new Card("Dogshit Dagger",{0,1,0})));
    addCard(*(new Card("Sharp Claws",{0,5,0})));
    addCard(*(new Card("Sword",{0,8,0})));
    addCard(*(new Card("Allowance",{0,0,3},"money_resource.png")));
    addCard(*(new Card("Single Coin",{0,0,1},"gold_coin.png")));
    addCard(*(new Card("Golden Apple",{5,0,5})));
    addCard(*(new Card("Sharp Cheddar",{5,5,0})));
    addCard(*(new Card("Steak",{10,0,0})));
    addCard(*(new Card("Milk",{3,0,0})));
    addCard(*(new Card("Mystery Meat",{5,0,0},"steak.png")));
    addCard(*(new Card("Emerald",{0,0,5})));
    addCard(*(new Card("Diamond",{0,0,10})));
    addCard(*(new Card("Demon Heart",{666,666,666})));
    addCard(*(new Card("Omelette",{8,0,5})));

}

void CardsLookup::addCard(Card& card, bool addToDeck)
{
    if (CardsRepo.find(card.name) != CardsRepo.end())
    {
        std::cerr << "Refusing to add card with duplicate id: " << card.name << "\n";
    }
    else
    {
        CardsRepo[card.name] = CardPtr(&card);
        if (card.isEnemy && addToDeck)
        {
            startingDeck.push_back(card.name);
        }
    }
}