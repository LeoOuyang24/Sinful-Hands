#ifndef ACTIONS_H
#define ACTIONS_H

#include <functional>
#include <memory>

#include "cards.h"
#include "tools.h"

//actions are the things that can happen after you choose an option

struct Player;

namespace Actions
{

    typedef std::function<void(const Rectangle& rect)> ActionRender;
    typedef std::function<void(Player& player)> ActionEffect;
    struct Action
    {
        ActionRender renderIcon = [](const Rectangle& rect){};
        ActionEffect effect = [](Player&  player){};
        ActionRender tooltip = [](const Rectangle& rect){};
    };

    static constexpr int TooltipFont = 20;

    Action AddCardsToHand(const std::vector<Card::CardIdentifier>& ids);
    Action AddCardToHand(Card::CardIdentifier id);
    Action AddCardsToDeck(const std::vector<Card::CardIdentifier>& ids, bool topdeck = false); 
    Action Random(const std::vector<Action>& actions); //randomly selects an option
    Action Skip();
    Action Die();
    Action ChangeHandSize(int amount);
    Action Heal(int amount);
    Action ChangeMaxHealth(int amount, bool heal=true);
    Action Win();
    Action Cleanse(Card::CardIdentifier type);
    Action ClearBoard();
    Action AddMetaCard();
    Action TakeDamage(int damage = 1);
    Action Clone();
    Action ResetHunger();
    Action AddTool(Tool::Type);

    typedef std::vector<Action> Consequences;
};


#endif // ACTIONS_H