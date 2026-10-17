#include <algorithm>

#include "raylib_helper.h"

#include "../include/options.h"
#include "../include/player.h"
#include "../include/sprites.h"
#include "../include/globals.h"
#include "../include/card_repo.h"

OptionVerifier ResourceVerifier(const Resources& resources)
{
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
                for (int j = 0; j < player.board.size() && res; j ++)
                {
                    if (res != res - player.board[j]->resources)
                    {
                        res -= player.board[j]->resources;
                        player.board.removeCard(j);
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
                    //ptr->render({rect.x,rect.y},0,0.75);
                    DrawTexturePro(ptr->cardArt,
                        {0,0,ptr->cardArt.width,ptr->cardArt.height},
                        {rect.x,rect.y,rect.width,rect.height},
                        {0.5,0.5},0,WHITE);
                }
            }
    };    
}

void Option::render(const Rectangle& rect)
{
    /*for (int i = 0; i < Resources::RESOURCE_NAME_SIZE; i ++)
    {
        //only handles one resource atm, any more and we run out of space
        if (resources[i] > 0)
        {
            Resources::renderResource(static_cast<Resources::ResourceName>(i),resources[i],{rect.x,rect.y + rect.height/2 - height/2},height,height*0.3f);

            Texture2D arrow = SpriteManager::getSprite("trade_arrow.png");
            DrawTexturePro(arrow,{0,0,arrow.width,arrow.height},{rect.x + height*0.3f + height*2, rect.y + rect.height/2 - rect.height*.125,rect.height*0.5,rect.height*0.25},{0.5,0.5},0,WHITE);

            hasResources = true;
            break;
        }
    }*/

    Rectangle resultRect = rect;

    const float ratio = 0.1f;
    const float margin = ratio*resultRect.height;
    const float arrow_spacing = ratio/2*resultRect.width;
    const float spacing = ratio*resultRect.width;
    const float iconDimen = resultRect.height*(1 - 2*ratio);

    resultRect.x += margin;
    resultRect.y += margin;

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
        Rectangle consRect = {resultRect.x + spacing*(i) + iconDimen*i,resultRect.y + margin, (1 - 2*ratio)*resultRect.height, (1 - 2*ratio)*resultRect.height};
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

Option::Option(const Actions::Consequences& cons, const Resources& resources_ ) : consequences(cons), verifier{ResourceVerifier(resources_)}
{

}

Option::Option(const Actions::Consequences& cons, OptionVerifier verifier_ ) : consequences(cons), verifier(verifier_)
{

}


void Option::effect(Player& player)
{
    for (auto& action : consequences)
        {
            action.effect(player);
        }
    verifier.postPick(player);
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

EffectCard::EffectCard(const Actions::Consequences& effect_) : effect(effect_), Card("",{})
{

}

void EffectCard::onEnterBoard(Player& player)
{
    player.addEffect(*this);
}
    
void EffectCard::onEnterHand(Player& player)
{
    player.removeEffect(*this);
}