#include "../include/actions.h"
#include "../include/player.h"
#include "../include/card_repo.h"
#include "../include/sequencer.h"
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
        },
        [ids](const Rectangle& rect)
        {
            DrawText("Adds these card(s)\n to your hand: ", rect.x, rect.y, TooltipFont, WHITE);
            for (int i = 0; i < ids.size(); i ++)
            {
                CardPtr temp = CardsLookup::getCard(ids[i]);
                if (temp)
                temp->render({rect.x + rect.width + 10,rect.y + Card::CARD_DIMEN.y*1.1*i*2},0,2);
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
        },
        [ids](const Rectangle& rect)
        {
            DrawText("Adds these card(s)\n to the deck: ", rect.x, rect.y, TooltipFont, WHITE);
            for (int i = 0; i < ids.size(); i ++)
            {
                CardPtr temp = CardsLookup::getCard(ids[i]);
                temp->render({rect.x + rect.width + 10,rect.y + (Card::CARD_DIMEN.y*1.1)*i*2},0,2);
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

        },
        [](const Rectangle& rect)
        {
            DrawText("Remove this card", rect.x, rect.y, TooltipFont, WHITE);
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
        },
        [damage](const Rectangle& rect)
        {
            std::string message = "Take ";
            message += std::to_string(damage) + " damage.";
            DrawText(message.c_str(),rect.x, rect.y, TooltipFont, WHITE);
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
            Sequences::addSequence([start=GetTime()]()
            {
                float lerp = GetTime() - start;
                DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),{120,0,0,255*(std::max(0.0f,1 - lerp/2))});

                return lerp > 2;
            });
        },
        [](const Rectangle& rect){
            DrawText("Commit a vile sin.\n3 angry spirits\nwill haunt you!",rect.x, rect.y, TooltipFont, WHITE);
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
        },
        [](const Rectangle& rect){
            DrawText("Die a horrible death",rect.x, rect.y, TooltipFont, WHITE);
        }        
    };   
}

Actions::Action Actions::LoseHand()
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("lose_hand_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [](Player& player){
            player.loseHandSize();
        },
        [](const Rectangle& rect){
            DrawText("Reduce max hand\nsize by 1",rect.x, rect.y, TooltipFont, WHITE);
        }        
    };   
}

Actions::Action Actions::Cleanse(Card::CardIdentifier type)
{
    return {
        [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("cleanse_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [type](Player& player){
            for (auto it = player.deck.begin(); it != player.deck.end();)
            {
                if (*it && (*it)->name == type)
                {
                    it = player.deck.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        },
        [type](const Rectangle& rect){
            std::string message = "Remove all \n" + type + "\nfrom the deck";
            DrawText(message.c_str(),rect.x, rect.y, TooltipFont, WHITE);
        }   
    };  
}

Actions::Action Actions::Heal(int amount)
{
    return {
         [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("heal_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [amount](Player& player){
            player.takeDamage(-amount);
        },
        [amount](const Rectangle& rect){
            std::string message = "Restore " + std::to_string(amount) + " health";
            DrawText(message.c_str(),rect.x, rect.y, TooltipFont, WHITE);
        }       
    };
}

Actions::Action Actions::ForbiddenArt()
{
    return {
         [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("forbidden_arts.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [](Player& player){
            player.addCardToDeck("Forbidden Arts");
            player.addCardToDeck("Forbidden Arts");
        },
        [](const Rectangle& rect){
            DrawText("Shuffle 2 more\nForbidden Arts\ninto the deck",rect.x, rect.y, TooltipFont, WHITE);
        }       
    };
}

Actions::Action Actions::Win()
{
    return {
         [](const Rectangle& rect){
            Texture2D icon = SpriteManager::getSprite("win_icon.png");
            DrawTexturePro(icon,{0,0,icon.width,icon.height},rect,{0.5,0.5},0,WHITE);           
        },
        [](Player& player){
            player.win();
        },
        [](const Rectangle& rect){
            DrawText("Win the game.\nYour goal is to\nbypass this card.",rect.x, rect.y, TooltipFont, WHITE);
        }       
    };   
}