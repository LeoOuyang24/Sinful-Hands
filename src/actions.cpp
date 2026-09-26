#include "../include/actions.h"
#include "../include/player.h"
#include "../include/card_repo.h"

Actions::Action Actions::AddCardsToHand(const std::vector<Card::CardIdentifier>& ids)
{
    return {
        [](const Rectangle& rect){
            DrawRectangle(rect.x,rect.y,rect.width,rect.height,RED);
        },
        [ids](Player& player){    
            for (int i = 0; i < ids.size(); i ++)
            {
                player.addCard(ids[i],true);
            }
        }
    };
}

Actions::Action Actions::AddCardsToDeck(const std::vector<Card::CardIdentifier>& ids)
{
    return {
        [](const Rectangle& rect){
            DrawRectangle(rect.x,rect.y,rect.width,rect.height,BLUE);
        },
        [ids](Player& player){    
            for (int i = 0; i < ids.size(); i ++)
            {
                player.addCardToDeck(ids[i]);
            }
        }
    };
}
