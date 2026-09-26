#include "../include/player.h"

#include "raymath.h"

void CardList::addCard(CardPtr& newCard)
{
    if (newCard)
    {
        emplace_back(std::move(newCard));
    }
}

CardPtr CardList::removeCard(int index)
{
    if (index >= 0 && index < size())
    {
       CardPtr ptr =  std::move((*this)[index]);
       erase(begin() + index);
       return ptr;
    }
    return CardPtr();
}

Rectangle Interface::getRegion(const Vector2& topLeft, const Vector2& section)
{
    Vector2 screenDimen = {GetScreenWidth(),GetScreenHeight()};
    return {screenDimen.x*topLeft.x,screenDimen.y*topLeft.y,screenDimen.x*section.x,screenDimen.y*section.y};

}

Rectangle Interface::getIthHandCardRect(int i, const Rectangle& handRect)
{
    return {handRect.x + i*(Card::CARD_DIMEN.x + HAND_CARD_SEPARATION*handRect.width), handRect.y,Card::CARD_DIMEN.x,Card::CARD_DIMEN.y};
}

void Interface::handleRegion(CardList& lst, const Rectangle& rect)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < lst.size(); i ++)
        {
            Rectangle cardRect = getIthHandCardRect(i,rect);

            if (lst[i].get() && CheckCollisionPointRec(GetMousePosition(),cardRect))
            {
                heldCard = lst.removeCard(i);
                break;
            }
        }
    }  
}


void Interface::handleMouse(Player& player)
{
    Rectangle handRect = getRegion(HAND_TOP_LEFT,HAND_SECTION);
    Rectangle boardRect = getRegion(BOARD_TOP_LEFT,BOARD_SECTION);

    handleRegion(player.hand,handRect);
    handleRegion(player.board,boardRect);

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && heldCard)
    {
        if (CheckCollisionPointRec(GetMousePosition(),handRect))
        {
            player.hand.addCard(heldCard);
        }
        else
        {        
            player.board.addCard(heldCard);
        }
    }

}

void Interface::renderCardList(const CardList& lst, const Rectangle& rect)
{
    for (int i = 0; i < lst.size(); i ++)
    {
        if (lst[i].get())
        {
            Rectangle cardRect = getIthHandCardRect(i,rect);
            if (CheckCollisionPointRec(GetMousePosition(),cardRect))
            {
                lst[i]->render({cardRect.x - 2*Card::CARD_DIMEN.x,cardRect.y- 2*Card::CARD_DIMEN.y},0,4);
            }
            else
            {
                lst[i]->render({cardRect.x,cardRect.y},0,1);
            }
        }
    }
}

void Interface::update( Player& player)
{
    Vector2 screenDimen = {GetScreenWidth(),GetScreenHeight()};

    Vector2 screenCenter = screenDimen*0.5;

    Rectangle playerHandRect = getRegion(HAND_TOP_LEFT,HAND_SECTION);
    
    Rectangle playerBoardRect = getRegion(BOARD_TOP_LEFT,BOARD_SECTION);

    DrawRectangleLines(DECK_TOP_LEFT.x*screenDimen.x,
                        DECK_TOP_LEFT.y*screenDimen.y,
                        DECK_SECTION.x*screenDimen.x,
                        DECK_SECTION.y*screenDimen.y, 
                        RED);

    DrawRectangleLines(HEALTH_AND_HUNGER_TOP_LEFT.x*screenDimen.x,
                        HEALTH_AND_HUNGER_TOP_LEFT.y*screenCenter.y,
                        HEALTH_AND_HUNGER_SECTION.x*screenDimen.x,
                        HEALTH_AND_HUNGER_SECTION.y*screenDimen.y,
                        BLUE);

    DrawRectangleLines(playerHandRect.x,playerHandRect.y,playerHandRect.width,playerHandRect.height,BLUE); 

    DrawRectangleLines(playerBoardRect.x, playerBoardRect.y, playerBoardRect.width, playerBoardRect.height,PURPLE);

    handleMouse(player);

    renderCardList(player.hand,playerHandRect);
    renderCardList(player.board,playerBoardRect);

    if (heldCard)
    {
        heldCard->render(GetMousePosition(),0,1);
    }
    
}