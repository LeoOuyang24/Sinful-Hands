#include <algorithm>

#include "raylib_helper.h"

#include "../include/options.h"
#include "../include/player.h"
#include "../include/sprites.h"
#include "../include/globals.h"
#include "../include/card_repo.h"

OptionVerifier ResourceVerifier(const Resources& resources)
{
    //blank resources yields a non existent OptionVerifier
    if (resources == Resources{0,0,0}) return {};

    return {
            [resources](const Player& player)
            {
                Resources playerResources = player.getBoardResources();
                for (int i = 0; i < Resources::RESOURCE_NAME_SIZE; i ++)
                {
                    if (playerResources[i] < resources[i])
                    {
                        return false;
                    }
                }
                return true;
            },
            [res=resources](Player& player) mutable
            {
                for (int j = 0; j < player.board.size() && res;)
                {
                    if (res != res - player.board[j]->resources)
                    {
                        res -= player.board[j]->resources;
                        player.board.removeCard(j);
                    } 
                    else
                    {
                        j++;
                    }
                }
            },
            [resources](const Rectangle& rect)
            {
                int i = 0;
                for (int r = 0; r < Resources::RESOURCE_NAME_SIZE; r++)
                {
                    if (resources[r])
                    {
                        float height = rect.height*(1.0f/Resources::RESOURCE_NAME_SIZE);
                        DrawText(std::to_string(resources[r]).c_str(),rect.x, rect.y + height*i, height, WHITE);
                        DrawSprite(Resources::getResourceIcon(r),{
                            rect.x + height*1.2,
                            rect.y + height*i, 
                            height, 
                            height});
                        i++;
                    }
                }
            }
    };
}

OptionVerifier CardVerifier(Card::CardIdentifier id)
{
    return {
            [id](const Player& player)
            {
                for (auto it = player.board.begin(); it != player.board.end(); ++it)
                {
                    if ((*it)->name == id)
                    {
                        return true;
                    }
                }
                return false;
            },
            [id](Player& player) mutable
            {
                for (int j = 0; j < player.board.size(); j ++)
                {
                    if (player.board[j]->name == id)
                    {
                        player.board.removeCard(j);
                        break;
                    }
                }
            },
            [id](const Rectangle& rect)
            {
                CardPtr ptr = CardsLookup::getCard(id);
                if (ptr)
                {
                    if (IsTextureValid(ptr->cardArt))
                    {
                        DrawTexturePro(ptr->cardArt,
                            {0,0,ptr->cardArt.width,ptr->cardArt.height},
                            {rect.x,rect.y,rect.width,rect.height},
                            {0.5,0.5},0,WHITE);
                    }
                }
            }
    };    
}

OptionVerifier CardAmountVerifier(int amount)
{
    return 
    {
        [amount](const Player& player){return player.board.size() == amount;}
    };
}

void Option::render(const Rectangle& rect)
{
    Rectangle resultRect = rect;

    const float ratio = 0.1f;
    const float margin = ratio*resultRect.height;
    const float arrow_spacing = ratio/2*resultRect.width;
    const float spacing = ratio*resultRect.width;
    const float iconDimen = resultRect.height*(1 - 2*ratio);

    resultRect.x += margin;
    resultRect.y += margin;

    if (name.size() > 0)
    {
        const float fontSize = resultRect.height/5;
        int textDimen = MeasureText(name.c_str(),fontSize);

        DrawRectangle(resultRect.x,resultRect.y,textDimen + 2*margin,fontSize + 2*margin,RED);
        DrawText(name.c_str(),resultRect.x + margin,resultRect.y + margin,fontSize,BLACK);
        resultRect.x += MeasureText(name.c_str(),fontSize) + margin*3;

    }

    //this increment code SUUUUUUUUUUUCKSSSSSS
    if (verifier)
    {
        verifier.render({resultRect.x, resultRect.y, iconDimen, iconDimen});
        resultRect.x += iconDimen + arrow_spacing;
        Texture2D arrow = SpriteManager::getSprite("trade_arrow.png");
        DrawTexturePro(arrow,
            {0,0,arrow.width,arrow.height},
            {resultRect.x, resultRect.y, iconDimen, iconDimen},
            {0.5,0.5},0,WHITE);
        //DrawRectangle(resultRect.x,resultRect.y,iconDimen,iconDimen,GREEN);

        resultRect.x += iconDimen + arrow_spacing;

    }

    for (int i = 0; i < consequences.size(); i ++)
    {
        Rectangle consRect = {resultRect.x + arrow_spacing*(i) + iconDimen*i,resultRect.y,iconDimen,iconDimen };
        consequences[i].renderIcon(consRect);
        //DrawRectangle(resultRect.x + width*(i + 1) + iconDimen*i,resultRect.y + margin*resultRect.height, (1 - 2*margin)*resultRect.height, (1 - 2*margin)*resultRect.height,RED);
        if (CheckCollisionPointRec(GetMousePosition(),consRect) && consequences[i].tooltip)
        {
            Rectangle tooltipRect = {rect.x + rect.width, rect.y + (rect.height + 10)*i, rect.width, rect.height};
            DrawRectangle(tooltipRect.x,tooltipRect.y,tooltipRect.width,tooltipRect.height,BLACK);
            DrawRectangleLines(tooltipRect.x,tooltipRect.y,tooltipRect.width,tooltipRect.height,GRAY);
            consequences[i].tooltip(tooltipRect);
        }
    }
    
}

Option::Option(const Actions::Consequences& cons_, std::string name_) : Option(cons_,OptionVerifier{},name_)
{

}


Option::Option(const Actions::Consequences& cons, const Resources& resources_, std::string name_  ) : consequences(cons), verifier{ResourceVerifier(resources_)}, name(name_)
{

}

Option::Option(const Actions::Consequences& cons, OptionVerifier verifier_ , std::string name_ ) : consequences(cons), verifier(verifier_), name(name_)
{

}

Option::Option(const Actions::Consequences& cons_, std::function<bool(const Player& player)> verifier_, std::string name_ ) : consequences(cons_), verifier({verifier_}), name(name_)
{

}

void Option::effect(Player& player)
{
    verifier.postPick(player);
    for (auto& action : consequences)
        {
            action.effect(player);
        }
}

bool Option::valid(const Player& player)
{
    return verifier.verify(player);
}

EnemyCard::EnemyCard(std::string name_, const std::vector<Option>& options_) : Card(name_), options(options_)
{
    isEnemy = true;
    borderColor = DARKGRAY;
}

void EnemyCard::renderCardBody(const Rectangle& cardBody, float scale)
{
    for (int i = 0; i < MAX_OPTIONS; i ++)
    {
        Rectangle space = getIthOptionRect(cardBody,i);
        if (i < options.size())
        {

            if (!options[i].valid(Globals::player))
            {
                DrawRectangle(space.x,space.y,space.width,space.height,GRAY);
            }
            else if (CheckCollisionPointRec(GetMousePosition(),space))
            {
                DrawRectangle(space.x,space.y,space.width,space.height,{255,0,0,100});
            }
                    
            options[i].render(space);
        }
        else
        {
            DrawRectangle(space.x,space.y,space.width,space.height,DARKGRAY);
        }
            
        DrawRectangleLines(space.x,space.y,space.width,space.height,BLACK);

    }
}

bool EnemyCard::handleInput(const Vector2& mousePos, Player& player, const Vector2& cardPos, float scale)
{
    for (int i = 0; i < MAX_OPTIONS; i ++)
    {
        Rectangle space = getIthOptionRect(cardPos,scale,i);

        if (i < options.size() && CheckCollisionPointRec(mousePos,space) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && options[i].valid(player))
        {                        
            /*int total = options[i].resources[Resources::MONEY] + options[i].resources[Resources::ATTACK] + options[i].resources[Resources::FOOD];
            for (int j = 0; j < player.board.size() && total > 0;)
            {
                options[i].resources[Resources::MONEY] = std::max(0,options[i].resources[Resources::MONEY] - player.board[j]->resources[Resources::MONEY]);
                options[i].resources[Resources::ATTACK] = std::max(0,options[i].resources[Resources::ATTACK] - player.board[j]->resources[Resources::ATTACK]);
                options[i].resources[Resources::FOOD] = std::max(0,options[i].resources[Resources::FOOD] - player.board[j]->resources[Resources::FOOD]);
                player.board.removeCard(j);
                
                total = options[i].resources[Resources::MONEY] + options[i].resources[Resources::ATTACK] + options[i].resources[Resources::FOOD];
            }*/
            options[i].effect(player);

            return true;
        }

    }
    return false;
}
