#include "../include/cards.h"
#include "../include/sprites.h"

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

void Card::render(const Vector2& pos, float rotation, float scale)
{
    float width = CARD_DIMEN.x*scale;
    float height = CARD_DIMEN.y*scale;
    
    DrawRectanglePro({pos.x,pos.y, width, height},{0.5,0.5},rotation,borderColor);


    Rectangle cardFace = {pos.x + CARD_MARGIN.x*width,
                        pos.y + CARD_MARGIN.y*height,
                        width*CARD_FACE_DIMEN.x,
                        height*CARD_FACE_DIMEN.y };

    Rectangle cardBody = {pos.x + CARD_MARGIN.x*width,
                          pos.y + (CARD_MARGIN.y*2+CARD_FACE_DIMEN.y)*height,
                          width*CARD_BODY_DIMEN.x,
                          height*CARD_BODY_DIMEN.y};

    const Color TAN = {169,175,0,255};

    DrawRectangle(cardFace.x,cardFace.y,cardFace.width,cardFace.height,TAN);
    DrawRectangle(cardBody.x,cardBody.y,cardBody.width,cardBody.height,TAN);
    DrawText(name.c_str(),cardFace.x,cardFace.y + cardFace.height,10*scale,WHITE);

    Vector2 cardBodyCenter = {cardBody.x + cardBody.width/2,cardBody.y + cardBody.height/2};
    
    int rendered = 0;

    const int fontSize = 10*scale;
    for (size_t i = 0; i < resources.size(); i ++)
    {
        if (resources[i])
        {
            const int x = cardBodyCenter.x - 1.5*fontSize;
            const int y = cardBody.y + (rendered + 0.5)*CARD_MARGIN.y*height;
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