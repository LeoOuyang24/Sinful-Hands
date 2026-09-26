#include "../include/options.h"
#include "../include/player.h"

void Option::render(const Rectangle& rect)
{
    const float margin = 0.1f;
    const float width = margin*rect.width;
    const float iconDimen = 2*width;

    for (int i = 0; i < consequences.size(); i ++)
    {
        consequences[i].renderIcon({rect.x + width*(i + 1) + iconDimen*i,rect.y + margin*rect.height, iconDimen, iconDimen});
    }
}

void Option::effect(Player& player)
{
    for (auto& action : consequences)
        {
            action.effect(player);
        }
}

EnemyCard::EnemyCard(std::string name_, Image image_, const std::vector<Option>& options_) : Card(name_, image_, BLACK), options(options_)
{

}

void EnemyCard::renderCardBody(const Rectangle& cardBody, float scale)
{
    for (int i = 0; i < MAX_OPTIONS; i ++)
    {
        Rectangle space = getIthOptionRect(cardBody,i);
        DrawRectangleLines(space.x,space.y,space.width,space.height,BLACK);
        if (i < options.size())
        if (options[i].consequences.size() > 0)
        {
            options[i].render(space);
            if (CheckCollisionPointRec(GetMousePosition(),space))
            {
                DrawRectangle(space.x,space.y,space.width,space.height,{255,0,0,100});
            }
        }
        else
        {
            DrawRectangle(space.x,space.y,space.width,space.height,GRAY);
        }
    }
}

bool EnemyCard::handleInput(const Vector2& mousePos, Player& player, const Vector2& cardPos, float scale)
{
    for (int i = 0; i < MAX_OPTIONS; i ++)
    {
        Rectangle space = getIthOptionRect(cardPos,scale,i);

        if (i < options.size() && CheckCollisionPointRec(mousePos,space) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
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