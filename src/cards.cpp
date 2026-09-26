#include "../include/cards.h"
#include "../include/sprites.h"
#include "../include/card_repo.h"

Texture2D Resources::getResourceIcon(size_t resourceName)
{
    switch (resourceName)
    {
        case FOOD:
            return SpriteManager::getSprite("food_resource.png");
        case ATTACK:
            return SpriteManager::getSprite("attack_resource.png");
        case MONEY:
            return SpriteManager::getSprite("money_resource.png");
        default:
            return {};
    }
}

Card::Card(std::string name_, Image cardArt_, Color borderColor_, Resources resources_) : name(name_), cardArt(cardArt_), borderColor(borderColor_), resources(resources_)
{

}

void Card::renderCentered(const Vector2& pos, float rotation, float scale)
{
    float width = CARD_DIMEN.x*scale;
    float height = CARD_DIMEN.y*scale;
    
    DrawRectanglePro(getCardRect(pos,scale),{0.5,0.5},rotation,borderColor);

    Rectangle cardFace = getCardFaceRect(pos,scale);

    Rectangle cardBody = getCardBodyRect(pos,scale);

    const Color TAN = {169,175,0,255};

    DrawRectangle(cardFace.x,cardFace.y,cardFace.width,cardFace.height,TAN);
    DrawRectangle(cardBody.x,cardBody.y,cardBody.width,cardBody.height,TAN);
    DrawText(name.c_str(),cardFace.x,cardFace.y + cardFace.height,10*scale,WHITE);

    renderCardBody(cardBody, scale);
}


void Card::render(const Vector2& pos, float rotation, float scale)
{
    renderCentered({pos.x + CARD_DIMEN.x*scale/2, pos.y + CARD_DIMEN.y*scale/2},rotation,scale);
}

void Card::renderCardBody(const Rectangle& cardBody, float scale)
{
    Vector2 cardBodyCenter = {cardBody.x + cardBody.width/2,cardBody.y + cardBody.height/2};
    
    int rendered = 0;

    const int fontSize = 10*scale;
    for (size_t i = 0; i < resources.size(); i ++)
    {
        if (resources[i])
        {
            const int x = cardBodyCenter.x - 1.5*fontSize;
            const int y = cardBody.y + (rendered + 0.5)*CARD_MARGIN.y*CARD_DIMEN.y*scale;
            Texture2D icon = Resources::getResourceIcon(i);
            if (IsTextureValid(icon))
            {
                DrawTexturePro(icon,
                    {0,0,icon.width,icon.height},
                    {x,y,fontSize,fontSize},
                    {}, 0,
                    WHITE);
            }
            DrawText(std::to_string(resources[i]).c_str(),x + 2*fontSize,y,fontSize,BLACK);
            rendered++;
        }
    }
}


void CardList::addCard(CardPtr newCard)
{
    if (newCard)
    {
        emplace_back(std::move(newCard));
    }
}

void CardList::addCard(Card::CardIdentifier id)
{
    addCard(CardsLookup::getCard(id));
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
