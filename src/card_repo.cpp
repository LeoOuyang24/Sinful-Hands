#include <algorithm>
#include <random>

#include "../include/card_repo.h"
#include "../include/options.h"
#include "../include/player.h"

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
            Option({Actions::Win()},Resources{0,20,0}),
            Option({Actions::Win()},Resources{0,0,20}),
            Option({Actions::AddCardsToDeck({"Castle Gates"})})       
        }
    })));

    addCard(*(new EnemyCard("Kind Vendor",
        {
            Option({Actions::AddCardsToHand({"Potato"})}),
            Option({Actions::AddCardsToHand({"Dagger"})}),
            Option({Actions::AddCardsToHand({"Allowance"}),Actions::AddTool(Tool::AXE),Actions::AddTool(Tool::GLOVE),Actions::AddTool(Tool::WAND)})
        })));

    addCard(*(new EnemyCard("Fork in the Road",
    {
        Option({Actions::AddCardsToDeck({"Starving Wolf", "Roadside Corpse","Pestilence","Sleeping Bear","Crumbling Cave","A little lost..."})}),
        Option({Actions::AddCardsToDeck({"Border Guard", "Distant Dragon", "Tax Collector", "Poorly Drawn Bandit"})})
    }
    )));

    addCard(*(new EnemyCard("Sleeping Bear",
    {
        Option({Actions::Skip()},[](const Player& player){ return player.hand.size() <= 5;}),
        Option({Actions::TakeDamage(1)},[](const Player& player){ return player.hand.size() <= 7;}),
        Option({Actions::TakeDamage(2)})
    }
    )));

    addCard(*(new EnemyCard("Crumbling Cave",
    {
        Option({Actions::AddCardsToHand({"Useless Rubble","Useless Rubble"})}),
        Option({Actions::Skip()},CardVerifier("Shovel")),
        Option({Actions::Skip()},Resources{0,8,0})
    }
    )));

    addCard(*(new EnemyCard("A little lost...",
    {
        Option({Actions::AddCardsToDeck({"Starving Wolf","Poorly Drawn Bandit","A little lost..."})}),
        Option({Actions::AddCardsToDeck({"Crumbling Cave", "Sleeping Bear", "A little lost..."})}),
        Option({Actions::Skip()},Resources{3,3,3})
    }
    )));

    addCard(*(new EnemyCard("Trash Goblin",
    {
        Option({{
            .effect = [](Player& player){    
            player.board.clear();
        }}}),
        Option({Actions::Skip()}),
        Option({Actions::AddCardsToDeck({"Trash Goblin"})})
    }
    )),true);

    addCard(*(new EnemyCard("Weeping Angel",
    {
        Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Dagger","Dagger"})}),
        Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Sword"})}),
        Option({Actions::Skip()})
    }
    )),true);

    addCard(*(new EnemyCard("Pestilence",
    {
        Option({Actions::TakeDamage(1),Actions::AddCardsToDeck({"Pestilence"})}),
        Option({Actions::Skip()},Resources{3,0,0}),
        Option({Actions::Skip()},Resources{0,0,3})
    }
    )));

    addCard(*(new EnemyCard("Tax Collector",
    {
        Option({Actions::Skip()},Resources{0,0,15}),
        Option({Actions::AddCardsToDeck({"Vengeful Knight","Vengeful Knight"})},Resources{0,5,0}),
        Option({Actions::AddCardsToDeck({"Vengeful Knight","Tax Collector"})}),
    }
    )));

    addCard(*(new EnemyCard("Vengeful Knight",
    {
        Option({Actions::TakeDamage(2)}),
        Option({Actions::TakeDamage(1)},Resources{0,7,0}),
        Option({Actions::Skip()},Resources{0,10,0})
    }
    )));

    addCard(*(new EnemyCard("Lady of the Ferns",
    {
        Option({Actions::Clone()},CardAmountVerifier(1)),
        Option({Actions::Skip()})
    }
    )),true);

    addCard(*(new EnemyCard("A Place To Rest",
    {
        Option({Actions::Heal(1)}),
        Option({Actions::AddCardsToHand({"Apple","Potato","Single Coin"})}),
        Option({Actions::AddCardsToHand({"Golden Idol"})},CardVerifier("Shovel"))
    }
    )), true);

    addCard(*(new EnemyCard("Retired Gravedigger",
    {
        Option({Actions::AddCardsToHand({"Shovel"})})
    }
    )),true);

    addCard(*(new EnemyCard("Deep Well",
        {
            Option({Actions::AddCardToHand("Bucket of Water")}),
            Option({Actions::AddCardsToDeck({"Deep Spider","Creature of the Well"},true)})
        }
    )),true);

    addCard(*(new EnemyCard("Deep Spider",
        {
            Option({Actions::TakeDamage(1),Actions::TakeDamage(1)}),
            Option({Actions::TakeDamage(1)},Resources{0,3,0}),
            Option({Actions::Skip()},Resources{0,6,0})
        }
    )));

    addCard(*(new EnemyCard("Creature of the Well",
        {
            Option({Actions::AddCardsToDeck({"Gift of Food","Gift of Food", "Gift of Food"})},Resources{0,0,3}),
            Option({Actions::AddCardsToDeck({"Gift of Health", "Gift of Health", "Gift of Health"})},Resources{0,0,4}),
            Option({Actions::AddCardsToDeck({"Blessing of the Well"})})
        }
    )));

    addCard(*(new EnemyCard("Blessing of the Well",
        {
            Option({Actions::ChangeHandSize(1)})
        }
    )));

    addCard(*(new EnemyCard("Gift of Health",
    {
        Option({Actions::Heal(1)})
    }
    )));

    addCard(*(new EnemyCard("Gift of Food",
        {
            Option({Actions::AddCardToHand("Potato")})
        }
    )));

    addCard(*(new EnemyCard("Roadside Corpse",
        {
            Option({Actions::AddCardsToHand({"Single Coin", "Single Coin"})}),
            Option({Actions::AddCardsToHand({"Apple", "Apple"})}),
            Option({Actions::AddCardsToHand({"Dogshit Dagger", "Dogshit Dagger"})})
        })));

    addCard(*(new EnemyCard("Cultist",
        {
            Option({Actions::TakeDamage(2)}),
            Option({Actions::AddCardsToHand({"Emerald"})},Resources{0,5,0}),
            Option({Actions::AddCardsToDeck({"Demon"})},Resources{0,5,0})
        }
    )));

    addCard(*(new EnemyCard("Demon",
    {
        Option({Actions::Die()}),
        Option({Actions::AddCardsToDeck({"Demonic Power","Demonic Power","Demonic Power"})},Resources{0,0,10}),
        Option({Actions::AddCardsToHand({"Demon Heart"})},Resources{0,10,0})
    }
    )));

    addCard(*(new EnemyCard("Demonic Power",
        {
            Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Sharp Claws","Sharp Claws"})})
        })));

    addCard(*(new EnemyCard("Burning Village",
        {
            Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Allowance","Dagger"})}),
            Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Crying Orphan"})}),
            Option({Actions::Skip()})
        }
    )),true);

    addCard(*(new EnemyCard("Distant Dragon",
            {
                Option({Actions::Skip()},Resources{0,5,0}),
                Option({Actions::AddCardsToDeck({"Looming Dragon"})},Resources{})    
            }
            )));

    addCard(*(new EnemyCard("Looming Dragon",
            {
                Option({Actions::Skip()},Resources{0,10,0}),
                Option({Actions::AddCardsToDeck({"Hungry Dragon"})},Resources{})    
            }
            )));

    addCard(*(new EnemyCard("Hungry Dragon",
            {
                Option({Actions::Skip()},Resources{0,15,0}),
                Option({Actions::Die()},Resources{})    
            }
            )));
    addCard(*(new EnemyCard("Border Guard",
            {
                Option({Actions::Skip()},Resources{0,0,10}),
                Option({Actions::Skip()},Resources{0,8,0}),
                Option({Actions::Skip(),Actions::TakeDamage(1)},Resources{})
            }
    )));
    addCard(*(new EnemyCard("Poorly Drawn Bandit",
            {
                Option({Actions::Skip()},{0,4,0}),
                Option({Actions::AddCardsToDeck({"Poorly Drawn Bandit"})},Resources{0,0,1}),
                Option({Actions::AddCardsToDeck({"Poorly Drawn Bandit"}),Actions::TakeDamage(1)},Resources{})
            }
    )));
    addCard(*(new EnemyCard("Starving Wolf",
        {
            Option({Actions::TakeDamage(1)}),
            Option({Actions::AddCardsToHand({"Allowance"})},Resources{0,3,0}),
            Option({Actions::AddCardsToDeck({"Grateful Wolf"})},Resources{3,0,0})
        })));
    addCard(*(new EnemyCard("Grateful Wolf",
        {
            Option({Actions::AddCardsToHand({"Sword"})}),
            Option({Actions::AddCardsToHand({"Steak"})}),
            Option({Actions::Heal(1)}),
        }
    )));

    addCard(*(new EnemyCard("Blacksmith",
        {
            Option({Actions::AddCardsToHand({"Dagger"})}),
            Option({Actions::AddCardsToHand({"Sword"})},Resources{0,0,5}),
            Option({Actions::AddCardsToHand({"Sword","Sword"})},CardVerifier("Crying Orphan"))
        }
    )),true);

    addCard(*(new EnemyCard("Giving Tree",
        {
            Option({Actions::AddCardsToHand({"Apple"})}),
            Option({Actions::AddCardsToHand({"Apple","Apple","Apple"})},Resources{0,3,0}),
            Option({Actions::Heal(1)})
        }
    )),true);
    addCard(*(new EnemyCard("The Elements",
        {
            Option({Actions::AddCardsToDeck({"The Elements"}),Actions::TakeDamage(1)}),
            Option({Actions::AddCardsToDeck({"The Elements"})},Resources{1,0,0}),
            Option({Actions::AddCardsToDeck({"The Elements"})},Resources{0,0,1})
        })),true);
    addCard(*(new EnemyCard("Job Offering",
        {
            Option({Actions::AddCardsToHand({"Allowance"})}),
            Option({Actions::AddCardsToDeck({"Starving Wolf","Starving Wolf"})}),
            Option({Actions::AddCardsToDeck({"Cultist"})})
        }
    )),true);

    addCard(*(new EnemyCard("Bear",
        {
            Option({Actions::TakeDamage(2)}),
            Option({Actions::Skip()},Resources{0,7,0}),
            Option({Actions::Skip()},Resources{10,0,0})
        }
    )));

    addCard(*(new EnemyCard("Sheep",
        {
            Option({Actions::Skip()},Resources{1,0,0}),
            Option({Actions::AddCardsToHand({"Steak"})},Resources{0,1,0}),
            Option({Actions::Skip()})
        }
    )));

    //TODO: Princess Option??
    addCard(*(new EnemyCard("Frog",
        {
            Option({Actions::Skip()})
        }
    )));

    addCard(*(new EnemyCard("Sentient Corn",
        {
            Option({Actions::AddCardToHand("Cooked Corn")}),
            Option({Actions::AddCardToHand("Sentient Corn Child")}),
            Option({Actions::Skip()})
        }
    )));

    addCard(*(new Card("Potato",{3,0,0})));
    addCard(*(new Card("Apple",{1,0,0},"food_resource.png")));
    addCard(*(new Card("Cooked Corn",{4,0,0})));
    addCard(*(new Card("Sentient Corn Child",{2,0,0})));
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
    addCard(*(new Card("Black Goat",{666,0,0})));
    addCard(*(new Card("Boulderfist",{0,6,7})));
    addCard(*(new Card("Pot of Greed",{0,0,15})));
    addCard(*(new Card("Crying Orphan",{})));
    addCard(*(new Card("Golden Idol",{0,0,10})));
    addCard(*(new Card("Shovel",{0,1,0})));
    addCard(*(new Card("Bucket of Water",{1,0,0})));
    addCard(*(new Card("Useless Rubble",{})));

    std::shuffle(startingDeck.begin(),startingDeck.end(), std::default_random_engine(time(NULL)));
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