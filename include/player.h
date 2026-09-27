#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include <list>

#include "cards.h"
#include "actions.h"

struct EnemyCard;
struct EffectCard;

struct Player
{
    static constexpr size_t MAX_HAND_SIZE = 10;
    static constexpr int MAX_PLAYER_HEALTH = 3;
    static constexpr int MAX_PLAYER_HUNGER = 3;

    CardList deck;
    CardPtr currentEnemy;
    CardList board;
    CardList hand;

    std::list<int> effects{}; //queued up effects

    int health = MAX_PLAYER_HEALTH;
    int hunger = MAX_PLAYER_HUNGER;
    bool dead = false;

    //add card to hand or board
    void addCard(CardPtr& ptr, bool hand);
    void addCard(Card::CardIdentifier, bool hand);

    void addCardToDeck(Card::CardIdentifier);
    void draw();

    void addEffect(EffectCard& effectCard);
    void removeEffect(EffectCard& effectCard);
    EffectCard* getEffect();

    Resources getBoardResources() const;

    void takeDamage(int damage);

    void gameOver();
};

struct Interface
{

    static constexpr Vector2 MARGINS = {0.05f,0.05f};

    static constexpr Vector2 HAND_SECTION = {0.8f,0.15f};
    static constexpr Vector2 HAND_TOP_LEFT = {0.5f - HAND_SECTION.x/2, 1 - MARGINS.y - HAND_SECTION.y};
    static constexpr Vector2 DECK_SECTION = {0.5f,0.45f};
    static constexpr Vector2 DECK_TOP_LEFT = {0.5f - DECK_SECTION.x/2,MARGINS.y};
    static constexpr Vector2 HEALTH_AND_HUNGER_SECTION = {0.2f,HAND_TOP_LEFT.y - DECK_TOP_LEFT.y - DECK_SECTION.y};
    static constexpr Vector2 HEALTH_AND_HUNGER_TOP_LEFT = {MARGINS.x, 0.5f};
    static constexpr Vector2 BOARD_TOP_LEFT = {HEALTH_AND_HUNGER_TOP_LEFT.x + HEALTH_AND_HUNGER_SECTION.x, 
                                                DECK_TOP_LEFT.y + DECK_SECTION.y};
    static constexpr Vector2 BOARD_SECTION = {DECK_SECTION.x,HAND_TOP_LEFT.y - (DECK_TOP_LEFT.y + DECK_SECTION.y)};                                           

    static constexpr float HAND_CARD_SEPARATION = .01f;

    static constexpr float ENEMY_SCALE = 4;

    CardPtr heldCard; //index of card in hand that we are holding

    Rectangle getRegion(const Vector2& topLeft,const Vector2& section);
    Rectangle getIthHandCardRect(int i, const Rectangle& handRect);

    void handleRegion(CardList& lst, const Rectangle& rect);
    void handleMouse(Player& player);

    void update( Player& player);
    void renderCardList(const CardList& lst, const Rectangle& rect);
};

#endif // PLAYER_H