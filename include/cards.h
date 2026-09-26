#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <memory>
#include <array>

#include "raylib.h"

struct Resources : public std::array<int,3>
{
    enum ResourceName : size_t
    {
        FOOD = 0,
        ATTACK,
        MONEY,
        RESOURCE_NAME_SIZE
    };

    static Texture2D getResourceIcon(size_t resource);
};

struct Card
{
    static constexpr Vector2 CARD_DIMEN = {70,110};
    static constexpr Vector2 CARD_MARGIN = {0.1f,0.1f};
    static constexpr Vector2 CARD_FACE_TOP_LEFT = {CARD_MARGIN.x,CARD_MARGIN.y};
    static constexpr Vector2 CARD_FACE_DIMEN = {1 - CARD_MARGIN.x*2,0.2};
    static constexpr Vector2 CARD_BODY_TOP_LEFT = {CARD_MARGIN.x,CARD_MARGIN.y*2 + CARD_FACE_DIMEN.y};
    static constexpr Vector2 CARD_BODY_DIMEN = {CARD_FACE_DIMEN.x, 1 - CARD_FACE_DIMEN.y - CARD_MARGIN.y*3};

    std::string name;
    Resources resources;
    Image cardArt;
    Color borderColor = BLUE;    

    void render(const Vector2& pos, float rotation, float scale);
};

typedef std::unique_ptr<Card> CardPtr;



#endif // CARDS_H