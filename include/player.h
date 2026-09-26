#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>

#include "cards.h"

struct CardList : public std::vector<CardPtr>
{
    void addCard(CardPtr& ptr);
    CardPtr removeCard(int index);
};

struct Player
{
    static constexpr size_t MAX_HAND_SIZE = 10;
    static constexpr int MAX_PLAYER_HEALTH = 5;
    static constexpr int MAX_PLAYER_HUNGER = 3;

    CardList hand;
    CardList deck;
    CardList board;

    int health = MAX_PLAYER_HEALTH;
    int hunger = MAX_PLAYER_HUNGER;

};

struct Interface
{

    static constexpr Vector2 MARGINS = {0.1f,0.1f};

    static constexpr Vector2 HAND_SECTION = {0.8f,0.2f};
    static constexpr Vector2 HAND_TOP_LEFT = {0.5f - HAND_SECTION.x/2, 1 - MARGINS.y - HAND_SECTION.y};
    static constexpr Vector2 HEALTH_AND_HUNGER_SECTION = {0.2f,0.4f};
    static constexpr Vector2 HEALTH_AND_HUNGER_TOP_LEFT = {MARGINS.x, 0.5f};
    static constexpr Vector2 DECK_SECTION = {0.5f,0.4f};
    static constexpr Vector2 DECK_TOP_LEFT = {0.5f - DECK_SECTION.x/2,MARGINS.y};
    static constexpr Vector2 BOARD_TOP_LEFT = {HEALTH_AND_HUNGER_TOP_LEFT.x + HEALTH_AND_HUNGER_SECTION.x, 
                                                DECK_TOP_LEFT.y + DECK_SECTION.y};
    static constexpr Vector2 BOARD_SECTION = {DECK_SECTION.x,HAND_TOP_LEFT.y - (DECK_TOP_LEFT.y + DECK_SECTION.y)};                                           

    static constexpr float HAND_CARD_SEPARATION = .01f;

    CardPtr heldCard; //index of card in hand that we are holding

    Rectangle getRegion(const Vector2& topLeft,const Vector2& section);
    Rectangle getIthHandCardRect(int i, const Rectangle& handRect);

    void handleRegion(CardList& lst, const Rectangle& rect);
    void handleMouse(Player& player);

    void update( Player& player);
    void renderCardList(const CardList& lst, const Rectangle& rect);
};

#endif // PLAYER_H