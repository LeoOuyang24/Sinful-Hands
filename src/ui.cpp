#include "../include/ui.h"
#include "../include/card_repo.h"
#include "../include/player.h"

void CardUI::render(const UIRenderInfo& info)
{
    if (auto ptr = CardsLookup::getCard(id))
    {
        ptr->render({info.rect.x,info.rect.y},0,1);
        if (hovered)
        {
            ptr->render({info.rect.x,info.rect.y},0,4);
        }
    }
}

void CardUI::update(const UIRenderInfo& info)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos,info.rect))
    {
        hovered = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            //Globals::interface
        }
    }
    else
    {
        hovered = false;
    }
}
