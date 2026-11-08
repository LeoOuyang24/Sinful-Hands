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
            Option({Actions::Win()},Resources{0,30,0}, "Fight Your Way In"),
            Option({Actions::Win()},Resources{0,0,20}, "Bribe Your Way In"),
            Option({Actions::AddCardsToDeck({"Castle Gates"})}, "Can't Get In Yet")       
        }
    })));

    addCard(*(new EnemyCard("Kind Vendor",
        {
            Option({Actions::AddCardsToHand({"Potato","Potato"})},"Ask for Food"),
            Option({Actions::AddCardsToHand({"Dagger","Dagger"})}, "Ask for Power"),
            Option({Actions::AddCardsToHand({"Allowance","Allowance"})},"Ask for Money")
        })));

    addCard(*(new EnemyCard("Fork in the Road",
    {
        Option({Actions::AddCardsToDeck({"Starving Wolf", "Roadside Corpse","Pestilence","Sleeping Bear","Crumbling Cave","A little lost...","Undelivered Package"})}, "Head Left"),
        Option({Actions::AddCardsToDeck({"Border Guard", "Distant Dragon", "Tax Collector", "Poorly Drawn Bandit","Traveling Salesman", "Wizard of the East", "The Banker"})}, "Head Right"),
        Option({Actions::AddTool(Tool::GLOVE)},CardVerifier("Shovel"), "Dig")
    }
    )));

    addCard(*(new EnemyCard("Sleeping Bear",
    {
        Option({Actions::Skip()},[](const Player& player){ return player.getTrueHandSize() <= 7;},"Sneak Past"),
        Option({Actions::Skip()},Resources{0,6,0},"Fight"),
        Option({Actions::TakeDamage(1)}, "Flee")
    }
    )));

    addCard(*(new EnemyCard("Traveling Salesman",
    {
        Option({Actions::AddCardsToHand({"Mystery Meat"}),Actions::AddCardsToDeck({"Traveling Salesman"})},Resources{0,0,3}, "Buy the Lunch Special"),
        Option({Actions::AddCardsToHand({"Sharp Claws"}),Actions::AddCardsToDeck({"Traveling Salesman"})},Resources{0,0,3}, "Buy Some Weapons"),
        Option({Actions::Skip()}, "Can't Afford It")
    }
    )));

    addCard(*(new EnemyCard("Mysterious Temple",
    {
        Option({Actions::Random({
            Actions::AddCardsToDeck({"Skeletal Guards"},true),
            Actions::AddCardsToDeck({"Stone Golem"},true),
            Actions::AddCardsToDeck({"Angel Statue"},true)
            })}, "Enter"),
        Option({Actions::AddCardsToDeck({"Mysterious Temple"})}, "Maybe Later..."),
        Option({Actions::Skip()}, "Seems Risky")
    }
    )),true);

    addCard(*(new EnemyCard("Skeletal Guards",
    {
        Option({Actions::TakeDamage(1)}, "Flee"),
        Option({Actions::AddCardsToDeck({"Sacrificial Altar"},true)},Resources{0,7,0}, "Fight"),
        Option({Actions::AddCardsToDeck({"Sacrificial Altar"},true),Actions::AddCardToHand("Golden Idol")},CardVerifier("Golden Idol"), "Turn the Undead")
    }
    )));

    addCard(*(new EnemyCard("Sacrificial Altar",
    {
        Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Sharp Claws","Sharp Claws","Sharp Claws"})}, "Make a Minor Sacrifice"),
        Option({Actions::TakeDamage(2),Actions::ChangeMaxHealth(3,false)}, "Make a Major Sacrifice"),
        Option({Actions::ChangeMaxHealth(3),Actions::AddCardsToHand({"Heavy Guilt","Heavy Guilt"})},CardVerifier("Crying Orphan"), "Make an Unspeakable Sacrifice")
    }
    )));

    addCard(*(new EnemyCard("Stone Golem",
    {
        Option({Actions::Skip()}, "Entrance is Blocked"),
        Option({Actions::AddCardsToDeck({"Portal Room"},true)},[](const Player& player){ return player.getTrueHandSize() <= 7;},"Sneak Past"),
        Option({Actions::AddCardsToDeck({"Portal Room"},true)},[](const Player& player){ return player.hand.countResources() >= Resources{5,5,5};}, "Prove Your Worth"),
    }
    )));

    addCard(*(new EnemyCard("Portal Room",
    {
        Option({Actions::AddTool(Tool::PORTAL)}, "Claim the Portal"),
        Option({Actions::AddCardsToDeck({"Creature of the Well"},true)}, "Go to a Wet Location"),
        Option({Actions::AddCardsToDeck({"A Place To Rest"},true)}, "Go to a Dry Location"),
    }
    )));

    addCard(*(new EnemyCard("Angel Statue",
    {
        Option({Actions::AddCardsToDeck({"Desecrated Altar"},true)},Resources{0,0,6}, "Pay a Tithe"),
        Option({Actions::AddCardsToDeck({"Desecrated Altar"},true)},CardVerifier("Golden Idol"), "Present a Holy Symbol"),
        Option({Actions::Skip()}, "No Way Through")
    }
    )));

    addCard(*(new EnemyCard("Desecrated Altar",
    {
        Option({Actions::ClearBoard()}, "Pray for Forgiveness"),
        Option({Actions::ChangeMaxHealth(1)}, "Pray for Health"),
        Option({Actions::AddCardsToHand({"Ambrosia","Ambrosia"})}, "Pray for Blessings")
    }
    )));

    addCard(*(new EnemyCard("Toolsmith",
        {
            Option({Actions::AddTool(Tool::BINOCULARS)}, "Take a Free Tool"),
            Option({Actions::AddTool(Tool::BINOCULARS),Actions::AddTool(Tool::BINOCULARS)},Resources{0,0,1}, "Buy His Stock"),
            Option({Actions::AddTool(Tool::JOKER),Actions::AddTool(Tool::BINOCULARS)},CardVerifier("Crying Orphan"), "New Apprentice")
        }
    )),true);

    addCard(*(new EnemyCard("Crumbling Cave",
    {
        Option({Actions::AddCardsToHand({"Useless Rubble","Useless Rubble"})}, "Claw Your Way Out"),
        Option({Actions::Skip()},CardVerifier("Shovel"), "Dig Your Way Out"),
        Option({Actions::Skip()},Resources{0,8,0}, "Makeshift Shovel")
    }
    )));

    addCard(*(new EnemyCard("A little lost...",
    {
        Option({Actions::AddCardsToDeck({"Fork in the Road"},true)}, "Backtrack"),
        Option({Actions::Skip()},Resources{3,3,3}, "Concentrate")
    }
    )));

    addCard(*(new EnemyCard("The Banker",
    {
        Option({Actions::AddCardsToHand({"Single Coin", "Single Coin", "Single Coin"})},"Take a Donation"),
        Option({Actions::AddCardsToDeck({"Return on Investment", "Return on Investment", "Return on Investment"})},Resources{0,0,3},"Make an Investment")
    }
    )));

    addCard(*(new EnemyCard("Return on Investment",
    {
        Option({Actions::AddCardsToHand({"Allowance", "Allowance", "Allowance"})}, "Cash Out"),
        Option({Actions::AddCardsToDeck({"Return on Investment", "Return on Investment"})}, "Invest Further")
    }
    )));

    addCard(*(new EnemyCard("Creature of the Bridge",
    {
        Option({Actions::Skip()},Resources{0,0,2}, "Pay the Toll"),
        Option({Actions::Skip()},Resources{10,0,0}, "Feed the Beast"),
        Option({Actions::TakeDamage(1),Actions::AddCardsToDeck({"Creature of the Bridge"})}, "Can't Cross")
    }
    )),true);

    addCard(*(new EnemyCard("Exotic Vendor",
    {
        Option({Actions::AddCardsToHand({"Sharp Cheddar"})},Resources{0,0,3}, "Buy the Cheese"),
        Option({Actions::AddCardsToHand({"Golden Apple"})},Resources{0,0,3}, "Buy the Fruit"),
        Option({Actions::Skip()}, "Not Today")
    }
    )),true);

    addCard(*(new EnemyCard("Undelivered Package",
    {
        Option({Actions::AddCardsToHand({"Allowance","Allowance"})}, "What's Inside?"),
        Option({Actions::AddCardsToHand({"Unopened Package"}),Actions::AddCardsToDeck({"Bandits"})}, "Deliver the Package")
    }
    )));

    addCard(*(new EnemyCard("Bandits",
    {
        Option({Actions::Skip()},CardVerifier("Unopened Package"), "Give Up the Goods"),
        Option({Actions::AddCardsToDeck({"Wizard of the West"})},Resources{0,8,0}, "Fight"),
        Option({Actions::TakeDamage(2), Actions::AddCardsToDeck({"Wizard of the West"})}, "Lie")
    }
    )));

    addCard(*(new EnemyCard("Wizard of the West",
    {
        Option({Actions::AddTool(Tool::WAND)},CardVerifier("Unopened Package"), "Complete the Delivery"),
        Option({Actions::ChangeHandSize(2)},CardVerifier("Unopened Package"), "Complete the Delivery"),
        Option({Actions::Skip()}, "Skip")
    }
    )));

    addCard(*(new EnemyCard("Wizard of the East",
    {
        Option({Actions::AddCardToHand("Transmuter's Stone")},Resources{0,0,6}, "Buy an Item"),
        Option({Actions::Heal(1)}, "Take a Potion"),
        Option({Actions::AddTool(Tool::HAMMER)},CardVerifier("Sentient Corn Child"), "New Apprentice"),
    }
    )));

    addCard(*(new EnemyCard("Our Memorial of Our Sins",
        {
            Option({Actions::AddCardsToHand({"Heavy Guilt","Emerald","Emerald"})}, "Pray for Wealth"),
            Option({Actions::AddCardsToHand({"Heavy Guilt","Greateaxe"})}, "Pray for Power"),
            Option({Actions::Skip()}, "Praying is for the Weak")
        }
    )),true);

    addCard(*(new EnemyCard("Hostel",
    {
        Option({Actions::Heal(1),Actions::ResetHunger()},Resources{0,0,3}, "Spend the Night"),
        Option({Actions::AddCardsToHand({"Dagger","Dogshit Dagger"})},Resources{0,0,3}, "Restock"),
        Option({Actions::Skip()}, "Not Now...")
    }
    )),true);

    addCard(*new EnemyCard("Goblin Services",
    {
        Option({Actions::AddCardsToHand({"Goblin Mercenary"})},Resources{0,0,5}, "Hire a Fighter"),
        Option({Actions::AddCardsToDeck({"Trash Goblin"})},Resources{0,0,6}, "Hire a Trashman"),
        Option({Actions::AddCardsToHand({"Apple"})}, "Complimentary Apple")
    }
    ),true);

    addCard(*(new EnemyCard("Goldsmith",
    {
        Option({Actions::AddCardsToHand({"Single Coin", "Single Coin", "Single Coin", "Single Coin"})}),
        Option({Actions::AddCardsToHand({"Half Dollar", "Half Dollar"})}),
        Option({Actions::AddCardsToHand({"Full Dollar"})})
    }
    )),true);

    addCard(*(new EnemyCard("Statue of a Fire Spirit",
    {
        Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Sharp Claws"})}, "Pray"),
        Option({Actions::TakeDamage(2),Actions::AddCardsToHand({"Sharp Claws","Sharp Claws"})}, "Pray Hard"),
        Option({Actions::AddCardsToHand({"Allowance","Allowance"}),Actions::AddCardsToDeck({"Wrathful Spirit"})},Resources{0,3,0}, "Desecrate")
    }
    )),true);

    addCard(*(new EnemyCard("Wrathful Spirit",
    {
        Option({Actions::AddCardsToHand({"Useless Rubble", "Useless Rubble", "Useless Rubble", "Useless Rubble"})}, "Beg For Mercy"),
        Option({Actions::AddCardsToHand({"Fire Essence","Fire Essence"})},Resources{0,8,0}, "Fight"),
        Option({Actions::AddCardsToHand({"Fire Essence","Fire Essence"})},CardVerifier("Bucket of Water"), "Fight")
    }
    )));

    addCard(*(new EnemyCard("Trash Goblin",
    {
        Option({Actions::ClearBoard()}, "Take My Trash!"),
        Option({Actions::Skip()}, "Buzz Off!"),
        Option({Actions::AddCardsToDeck({"Trash Goblin"})}, "Maybe Later?")
    }
    )));

    addCard(*(new EnemyCard("Weeping Angel",
    {
        Option({Actions::Heal(2),Actions::AddCardsToHand({"Heavy Guilt"})}, "Beg for Forgiveness"),
        Option({Actions::AddCardsToHand({"Heavy Guilt","Sword","Emerald"})}, "Promise Vengeance"),
        Option({Actions::Skip()}, "Ignore")
    }
    )),true);

    addCard(*(new EnemyCard("Pestilence",
    {
        Option({Actions::AddCardsToHand({"Rotten Food"}),Actions::AddCardsToDeck({"Pestilence"})}, "Ignore it"),
        Option({Actions::Skip()},Resources{3,0,0}, "Throw Away the Bad"),
        Option({Actions::Skip()},Resources{0,0,3}, "Hire a Doctor")
    }
    )));

    addCard(*(new EnemyCard("Tax Collector",
    {
        Option({Actions::Skip()},Resources{0,0,3}, "Pay"),
        Option({Actions::AddCardsToDeck({"Vengeful Knight","Vengeful Knight"})},Resources{0,3,0}, "No One Will Miss Him..."),
        Option({Actions::AddCardsToDeck({"Vengeful Knight","Tax Collector"})}, "Not Enough..."),
    }
    )));

    addCard(*(new EnemyCard("Vengeful Knight",
    {
        Option({Actions::TakeDamage(2)},"Accept Your Fate"),
        Option({Actions::TakeDamage(1)},Resources{0,3,0}, "Fight"),
        Option({Actions::Skip()},Resources{0,6,0}, "Fight Hard")
    }
    )));

    addCard(*(new EnemyCard("Lady of the Ferns",
    {
        Option({Actions::Clone()},CardAmountVerifier(1), "Regret"),
        Option({Actions::AddMetaCard(),Actions::Heal(1)},CardAmountVerifier(1), "Forget"),
        Option({Actions::Skip()}, "Skip")
    }
    )),true);

    addCard(*(new EnemyCard("A Place To Rest",
    {
        Option({Actions::Heal(1)}, "Rest Up"),
        Option({Actions::AddCardsToHand({"Apple","Potato","Single Coin"})}, "Scavenge the Area"),
        Option({Actions::AddCardsToHand({"Golden Idol"})},CardVerifier("Shovel"), "Dig Around")
    }
    )), true);

    addCard(*(new EnemyCard("Retired Gravedigger",
    {
        Option({Actions::AddCardsToHand({"Shovel"})}, "I'll Take It")
    }
    )),true);

    addCard(*(new EnemyCard("Deep Well",
        {
            Option({Actions::AddCardToHand("Bucket of Water")}, "Take Some Water"),
            Option({Actions::AddCardsToDeck({"Deep Spider","Creature of the Well"},true)}, "Something Down There...")
        }
    )),true);

    addCard(*(new EnemyCard("Deep Spider",
        {
            Option({Actions::TakeDamage(1),Actions::TakeDamage(1)}, "Run Past"),
            Option({Actions::TakeDamage(1)},Resources{0,3,0}, "Fight"),
            Option({Actions::Skip()},Resources{0,6,0}, "Fight Hard")
        }
    )));

    addCard(*(new EnemyCard("Creature of the Well",
        {
            Option({Actions::AddCardsToDeck({"Gift of Food","Gift of Food", "Gift of Food"})},Resources{0,0,3}, "Ask for Food"),
            Option({Actions::AddCardsToDeck({"Gift of Health", "Gift of Health", "Gift of Health"})},Resources{0,0,4}, "Ask for Health"),
            Option({Actions::AddCardsToDeck({"Blessing of the Well"})}, "Ask for its Blessing")
        }
    )));

    addCard(*(new EnemyCard("Blessing of the Well",
        {
            Option({Actions::ChangeHandSize(2)}, "Accept")
        }
    )));

    addCard(*(new EnemyCard("Gift of Health",
    {
        Option({Actions::Heal(1)}, "Accept")
    }
    )));

    addCard(*(new EnemyCard("Gift of Food",
        {
            Option({Actions::AddCardToHand("Potato")}, "Accept")
        }
    )));

    addCard(*(new EnemyCard("Roadside Corpse",
        {
            Option({Actions::AddCardsToHand({"Single Coin", "Single Coin"})}, "Take Money"),
            Option({Actions::AddCardsToHand({"Apple", "Apple"})}, "Take Rations"),
            Option({Actions::AddCardsToHand({"Dogshit Dagger", "Dogshit Dagger"})}, "Take Weapons")
        })));

    addCard(*(new EnemyCard("Cultist",
        {
            Option({Actions::TakeDamage(2)}, "Flee"),
            Option({Actions::AddCardsToHand({"Emerald"})},Resources{0,6,0}, "Slay for a Bounty"),
            Option({Actions::AddCardsToDeck({"Demon"})},Resources{0,6,0}, "The Cult Goes Deeper...")
        }
    )));

    addCard(*(new EnemyCard("Demon",
    {
        Option({Actions::Die()}, "Hope is Lost"),
        Option({Actions::AddCardsToDeck({"Demonic Power","Demonic Power","Demonic Power"})},Resources{0,0,6}, "Make a Bargain"),
        Option({Actions::AddCardsToHand({"Demon Heart"})},Resources{0,15,0}, "Slay the Demon")
    }
    )));

    addCard(*(new EnemyCard("Demonic Power",
        {
            Option({Actions::AddCardsToHand({"Heavy Guilt","Sharp Claws","Sharp Claws"})}, "Accept")
        })));

    addCard(*(new EnemyCard("Burning Village",
        {
            Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Allowance","Dagger"})}, "Loot the Place"),
            Option({Actions::TakeDamage(1),Actions::AddCardsToHand({"Crying Orphan"})}, "Look for Survivors"),
            Option({Actions::Skip()}, "Someone Else's Problem")
        }
    )),true);

    addCard(*(new EnemyCard("Distant Dragon",
            {
                Option({Actions::Skip()},Resources{0,5,0}, "Slay it"),
                Option({Actions::AddCardsToDeck({"Looming Dragon"})},Resources{}, "Ignore It")    
            }
            )));

    addCard(*(new EnemyCard("Looming Dragon",
            {
                Option({Actions::Skip()},Resources{0,10,0}, "Slay it Now"),
                Option({Actions::AddCardsToDeck({"Hungry Dragon"})},Resources{}, "It's Still Far Off... ")    
            }
            )));

    addCard(*(new EnemyCard("Hungry Dragon",
            {
                Option({Actions::Skip()},Resources{0,15,0}, "Slay it at Last"),
                Option({Actions::Die()},Resources{}, "There's Nothing To Be Done...")    
            }
            )));
    addCard(*(new EnemyCard("Border Guard",
            {
                Option({Actions::Skip()},Resources{0,0,10}, "Pay a Bribe"),
                Option({Actions::Skip()},Resources{0,8,0}, "Fight Him"),
                Option({Actions::Skip(),Actions::TakeDamage(1)},Resources{}, "Try to Sneak Past")
            }
    )));
    addCard(*(new EnemyCard("Poorly Drawn Bandit",
            {
                Option({Actions::Skip()},Resources{0,4,0}, "Defend Yourself"),
                Option({Actions::AddCardsToDeck({"Poorly Drawn Bandit"})},Resources{0,0,1}, "Pay Up"),
                Option({Actions::AddCardsToDeck({"Poorly Drawn Bandit"}),Actions::TakeDamage(1)},"Grit Your Teeth")
            }
    )));
    addCard(*(new EnemyCard("Starving Wolf",
        {
            Option({Actions::TakeDamage(1)}, "Run"),
            Option({Actions::AddCardsToHand({"Allowance"})},Resources{0,3,0}, "Hunt for a Bounty"),
            Option({Actions::AddCardsToDeck({"Grateful Wolf"})},Resources{3,0,0}, "Tame It")
        })));
    addCard(*(new EnemyCard("Grateful Wolf",
        {
            Option({Actions::AddCardsToHand({"Sword"})}, "It found a weapon!"),
            Option({Actions::AddCardsToHand({"Steak"})}, "Hunt with it"),
            Option({Actions::Heal(1)}, "Pet it"),
        }
    )));

    addCard(*(new EnemyCard("Blacksmith",
        {
            Option({Actions::AddCardsToHand({"Dagger"})}, "Take a Donation"),
            Option({Actions::AddCardsToHand({"Sword"})},Resources{0,0,5}, "Buy some Quality"),
            Option({Actions::AddTool(Tool::AXE)},CardVerifier("Crying Orphan"), "New Apprentice")
        }
    )),true);

    addCard(*(new EnemyCard("Giving Tree",
        {
            Option({Actions::AddCardsToHand({"Apple"})}, "Low Hanging Fruit"),
            Option({Actions::AddCardsToHand({"Apple","Apple","Apple"})},Resources{0,3,0}, "Cut the Tree"),
            Option({Actions::Heal(1)}, "Take a Nap")
        }
    )),true);
    /*addCard(*(new EnemyCard("The Elements",
        {
            Option({Actions::AddCardsToDeck({"The Elements"}),Actions::TakeDamage(1)}),
            Option({Actions::AddCardsToDeck({"The Elements"})},Resources{1,0,0}),
            Option({Actions::AddCardsToDeck({"The Elements"})},Resources{0,0,1})
        })),true);*/
    addCard(*(new EnemyCard("Job Offering",
        {
            Option({Actions::AddCardsToHand({"Allowance"})}, "Take a Donation"),
            Option({Actions::AddCardsToDeck({"Starving Wolf","Starving Wolf"})}, "Hunt Some Wolves"),
            Option({Actions::AddCardsToDeck({"Cultist"})}, "Track Down a Cult")
        }
    )),true);

    addCard(*(new EnemyCard("Bear",
        {
            Option({Actions::TakeDamage(1)}, "Flee"),
            Option({Actions::Skip()},Resources{0,7,0}, "Fight"),
            Option({Actions::Skip()},Resources{10,0,0}, "Feed")
        }
    )));

    addCard(*(new EnemyCard("Sheep",
        {
            Option({Actions::Skip()},Resources{1,0,0}, "Feed him (why not?)"),
            Option({Actions::AddCardsToHand({"Steak"})},Resources{0,1,0}, "You wouldn't!"),
            Option({Actions::Skip()}, "Wolf Mindset")
        }
    )));

    //TODO: Princess Option??
    addCard(*(new EnemyCard("Frog",
        {
            Option({Actions::Skip()},"Yeah that's a frog")
        }
    )));

    addCard(*(new EnemyCard("Sentient Corn",
        {
            Option({Actions::AddCardToHand("Cooked Corn")},"Desparate Times..."),
            Option({Actions::AddCardToHand("Sentient Corn Child")}, "Adopt Him"),
            Option({Actions::Skip()}, "Ignore Him")
        }
    )));

    addCard(*(new EnemyCard("Starvation",
    {
        Option({Actions::ResetHunger()},Resources{1,0,0}, "Consume Rations"),
        Option({Actions::TakeDamage(1), Actions::ResetHunger()}, "Ignore the Hunger")
    }
    )));

    addCard(*(new Card("Potato",{3,0,0})));
    addCard(*(new Card("Apple",{1,0,0},"food_resource.png")));
    addCard(*(new Card("Greateaxe",{0,12,0})));
    addCard(*(new Card("Cooked Corn",{4,0,0})));
    addCard(*(new Card("Sentient Corn Child",{2,1,5})));
    addCard(*(new Card("Dagger",{0,3,0})));
    addCard(*(new Card("Dogshit Dagger",{0,1,0})));
    addCard(*(new Card("Sharp Claws",{0,5,0})));
    addCard(*(new Card("Sword",{0,8,0})));
    addCard(*(new Card("Allowance",{0,0,3},"money_resource.png")));
    addCard(*(new Card("Single Coin",{0,0,1},"gold_coin.png")));
    addCard(*(new Card("Half Dollar",{0,0,2},"gold_coin.png")));
    addCard(*(new Card("Full Dollar",{0,0,4},"gold_coin.png")));
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
    addCard(*(new Card("Heavy Guilt",{})));
    addCard(*(new Card("Unopened Package",{})));
    addCard(*(new Card("Transmuter's Stone",{6,6,6})));
    addCard(*(new Card("Fire Essence",{0,5,7})));
    addCard(*(new Card("Goblin Mercenary",{0,6,0})));
    addCard(*(new Card("Ambrosia",{8})));
    addCard(*(new Card("Rotten Food",{})));

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