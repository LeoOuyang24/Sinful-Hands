#ifndef ACTIONS_H
#define ACTIONS_H

#include <functional>
#include <memory>

#include "cards.h"

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
    Action AddCardsToDeck(const std::vector<Card::CardIdentifier>& ids); 
    Action Skip();
    Action Sin();
    Action Die();
    Action LoseHand();
    Action Heal(int amount);
    Action ForbiddenArt();
    Action Win();
    Action Cleanse(Card::CardIdentifier type);
    Action TakeDamage(int damage = 1);

    typedef std::vector<Action> Consequences;
};


#endif // ACTIONS_H