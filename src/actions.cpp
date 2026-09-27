#include "../include/actions.h"
#include "../include/player.h"
#include "../include/card_repo.h"
#include "../include/sprites.h"

Actions::Action Actions::AddCardsToHand(const std::vector<Card::CardIdentifier>& ids)
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("add_card.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);
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
            Texture2D icon = SpriteManager::getSprite("shuffle_card.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);
        },
        [ids](Player& player){    
            for (int i = 0; i < ids.size(); i ++)
            {
                player.addCardToDeck(ids[i]);
            }
        }
    };
}

Actions::Action Actions::Skip()
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("skip_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [](Player& player){

        }
    };
}

Actions::Action Actions::TakeDamage(int damage)
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("take_damage_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [damage](Player& player){
            player.takeDamage(damage);
        }
    };  
}

Actions::Action Actions::Sin()
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("sin_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [](Player& player){
            player.addCardToDeck("Wrathful Spirit");
            player.addCardToDeck("Famished Spirit");
            player.addCardToDeck("Greedy Spirit");
        }
    };   
}

Actions::Action Actions::Die()
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("death_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [](Player& player){
            player.gameOver();
        }
    };   
}
