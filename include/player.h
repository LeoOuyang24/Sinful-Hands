#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include <list>

#include "cards.h"
#include "actions.h"
#include "tools.h"

struct EnemyCard;
struct EffectCard;

struct Player
{
    static constexpr size_t MAX_HAND_SIZE = 11;
    static constexpr int MAX_PLAYER_HEALTH = 3;
    static constexpr int MAX_PLAYER_HUNGER = 5;

    std::vector<Tool::Type> tools;
    int currentTool = -1; //index of the current tool

    CardList deck;
    CardPtr currentEnemy;
    CardList board;
    CardList hand;

    std::list<int> effects{}; //queued up effects

    int health = MAX_PLAYER_HEALTH;
    int maxHealth = MAX_PLAYER_HEALTH;
    int hunger = MAX_PLAYER_HUNGER;
    int maxHandSize = MAX_HAND_SIZE;
    bool dead = false;
    bool won = false;

    //add card to hand or board
    void addCard(CardPtr& ptr, bool hand);
    void addCard(Card::CardIdentifier, bool hand);
    int getTrueHandSize() const { return hand.size() + board.size();}

    void changeHandSize(int amount);

    void changeHealth(int amount);
    void changeMaxHealth(int newVal);

    void resetHunger();
    bool isOverEncumbered();

    //add a card at the given index
    //-1 to insert randomly 
    void addCardToDeck(Card::CardIdentifier, int index = -1);
    void draw();

    void addEffect(EffectCard& effectCard);
    void removeEffect(EffectCard& effectCard);
    EffectCard* getEffect();

    Resources getBoardResources() const;

    void addTool(Tool::Type);
    void setCurrentTool(int index);
    void removeCurrentTool();
    Tool::Type getCurrentTool() const;

    void gameOver();
    void win();
};

struct Interface
{

    static constexpr Vector2 MARGINS = {0.05f,0.05f};

    static constexpr Vector2 HAND_SECTION = {0.8,0.15f};
    static constexpr Vector2 HAND_TOP_LEFT = {0, 1 - MARGINS.y - HAND_SECTION.y};
    static constexpr Vector2 DECK_SECTION = {0.6f,0.4f};
    static constexpr Vector2 DECK_TOP_LEFT = {.1f,MARGINS.y + .01};
    static constexpr Vector4 TOOLBOX = {MARGINS.x,0.5,.1,HAND_TOP_LEFT.y - 0.5};

    static constexpr Vector2 HEALTH_AND_HUNGER_SECTION = {0.2f,HAND_TOP_LEFT.y - DECK_TOP_LEFT.y - DECK_SECTION.y};
    static constexpr Vector2 HEALTH_AND_HUNGER_TOP_LEFT = {TOOLBOX.x + TOOLBOX.z + MARGINS.x, HAND_TOP_LEFT.y - HEALTH_AND_HUNGER_SECTION.y};
    static constexpr Vector2 BOARD_TOP_LEFT = {.5 - .35, 
                                                DECK_TOP_LEFT.y + DECK_SECTION.y};
    static constexpr Vector2 BOARD_SECTION = {.7,HAND_TOP_LEFT.y - (DECK_TOP_LEFT.y + DECK_SECTION.y)};   
    

    static constexpr float HAND_CARD_SEPARATION = .01f;

    static constexpr float ENEMY_SCALE = 3.5;

    CardPtr heldCard; //index of card in hand that we are holding

    int foresight = 0; //number of cards ahead the player is scouting

    Rectangle getRegion(const Vector2& topLeft,const Vector2& section);
    Rectangle getIthHandCardRect(int i, const Rectangle& handRect);

    void handleRegion(CardList& lst, const Rectangle& rect);
    void handleMouse(Player& player);

    void update( Player& player);
    void renderCardList(const CardList& lst, const Rectangle& rect);
};

#endif // PLAYER_H