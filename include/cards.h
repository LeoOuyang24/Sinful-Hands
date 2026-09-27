#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <memory>
#include <array>
#include <vector>

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
    static void renderResource(ResourceName name, size_t amount,const Vector2& start, size_t size, size_t spacing);
};

struct Player;

struct Card
{
    typedef std::string CardIdentifier;

    static constexpr Vector2 CARD_DIMEN = {70,110};
    static constexpr Vector2 CARD_MARGIN = {0.1f,0.1f};
    static constexpr Vector2 CARD_FACE_TOP_LEFT = {CARD_MARGIN.x,CARD_MARGIN.y};
    static constexpr Vector2 CARD_FACE_DIMEN = {1 - CARD_MARGIN.x*2,0.2};
    static constexpr Vector2 CARD_BODY_TOP_LEFT = {CARD_MARGIN.x,CARD_MARGIN.y*2 + CARD_FACE_DIMEN.y};
    static constexpr Vector2 CARD_BODY_DIMEN = {CARD_FACE_DIMEN.x, 1 - CARD_FACE_DIMEN.y - CARD_MARGIN.y*3};

    inline static Rectangle getCardRect(const Vector2& center, float scale)
    {
        float width = CARD_DIMEN.x*scale;
        float height = CARD_DIMEN.y*scale;
        return {center.x - width/2, center.y - height/2, width, height};
    }
    inline static Rectangle getCardFaceRect(const Vector2& center, float scale)
    {
        Rectangle card = getCardRect(center,scale);
        return {
            card.x + card.width * CARD_FACE_TOP_LEFT.x,
            card.y + card.height * CARD_FACE_TOP_LEFT.y,
            card.width * CARD_FACE_DIMEN.x,
            card.height * CARD_FACE_DIMEN.y
        };
    }
    inline static Rectangle getCardBodyRect(const Vector2& center, float scale)
    {
        Rectangle card = getCardRect(center,scale);

        return {
          card.x + card.width * CARD_BODY_TOP_LEFT.x,
          card.y + card.height * CARD_BODY_TOP_LEFT.y,
          card.width * CARD_BODY_DIMEN.x,
          card.height * CARD_BODY_DIMEN.y
        };
    }


    std::string name;
    Resources resources = {0,0,0};
    Texture2D cardArt;
    Color borderColor = BROWN;    
    bool isEnemy = false;

    Card(std::string name_, Resources resources_ = {}, std::string cardArtName = "");

    void render(const Vector2& pos, float rotation, float scale);
    void renderCentered(const Vector2& pos, float rotation, float scale);
    virtual void renderCardBody(const Rectangle& cardBody, float scale);

    virtual void onEnterBoard(Player& player){};
    virtual void onEnterHand(Player& player){};

};


typedef std::unique_ptr<Card> CardPtr;

struct CardList : public std::vector<CardPtr>
{
    void addCard(CardPtr ptr);
    void addCard(Card::CardIdentifier id);
    CardPtr removeCard(int index);
};






#endif // CARDS_H