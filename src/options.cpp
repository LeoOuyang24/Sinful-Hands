#include "../include/options.h"
#include "../include/player.h"
#include "../include/sprites.h"
#include "../include/globals.h"

void Option::render(const Rectangle& rect)
{
    bool hasResources = false;
    const int height = rect.height/2;
    int rendered = 0;
    for (int i = 0; i < Resources::RESOURCE_NAME_SIZE; i ++)
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
    }
    Rectangle resultRect = rect;
    if (hasResources)
    {
        resultRect.x += 0.5*rect.width;
    }

    const float margin = 0.1f;
    const float width = margin*resultRect.width;
    const float iconDimen = 2*width;

    for (int i = 0; i < consequences.size(); i ++)
    {
        consequences[i].renderIcon({resultRect.x + width*(i + 1) + iconDimen*i,resultRect.y + margin*resultRect.height, iconDimen, iconDimen});
    }
    
}

Option::Option(const Actions::Consequences& cons, const Resources resources_ ) : consequences(cons), resources(resources_)
{

}

void Option::effect(Player& player)
{
    for (auto& action : consequences)
        {
            action.effect(player);
        }
}

bool Option::valid(const Player& player)
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
}

EnemyCard::EnemyCard(std::string name_, Image image_, const std::vector<Option>& options_) : Card(name_, image_, BLACK), options(options_)
{
    isEnemy = true;
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
            options[i].effect(player);
                        
            int total = options[i].resources[Resources::MONEY] + options[i].resources[Resources::ATTACK] + options[i].resources[Resources::FOOD];
            for (int j = 0; j < player.board.size() && total > 0;)
            {
                options[i].resources[Resources::MONEY] = std::max(0,options[i].resources[Resources::MONEY] - player.board[j]->resources[Resources::MONEY]);
                options[i].resources[Resources::ATTACK] = std::max(0,options[i].resources[Resources::ATTACK] - player.board[j]->resources[Resources::ATTACK]);
                options[i].resources[Resources::FOOD] = std::max(0,options[i].resources[Resources::FOOD] - player.board[j]->resources[Resources::FOOD]);
                player.board.removeCard(j);
                
                total = options[i].resources[Resources::MONEY] + options[i].resources[Resources::ATTACK] + options[i].resources[Resources::FOOD];
            }
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