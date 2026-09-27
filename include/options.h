#ifndef OPTIONS_H
#define OPTIONS_H

#include <array>

#include "actions.h"

struct Player;

struct Option
{

    Actions::Consequences consequences;
    Resources resources = {0,0,0};

    Option(const Actions::Consequences& cons_, const Resources resources_ = {});

    //true if this option can be selected
    virtual bool valid(const Player& player);
    virtual void effect(Player& player);
    virtual void render(const Rectangle& rect);
};

struct EnemyCard  : public Card
{
    static constexpr size_t MAX_OPTIONS = 3;
    static constexpr size_t PLAYER_OPTION = MAX_OPTIONS - 1;

    std::vector<Option> options;

    inline static Rectangle getIthOptionRect(const Vector2& cardCenter, float scale, int i)
    {
        Rectangle cardBody = Card::getCardBodyRect(cardCenter,scale);
        return getIthOptionRect(cardBody,i);
    }

    inline static Rectangle getIthOptionRect(const Rectangle& cardBody, int i)
    {
        const float ratio = 1.0f/MAX_OPTIONS;
        return {cardBody.x,cardBody.y + i*ratio*cardBody.height,cardBody.width,cardBody.height*ratio};
    }

    EnemyCard(std::string name_, const std::vector<Option>& options_);
    void renderCardBody(const Rectangle& cardBody, float scale);

    //handle user input, returns trues if an option has been successfully selected
    bool handleInput(const Vector2& mousePos, Player& player, const Vector2& cardPos, float scale);
};

struct EffectCard : public Card
{
    Actions::Consequences effect;

    EffectCard(const Actions::Consequences& effect_);

    void onEnterBoard(Player& player);
    void onEnterHand(Player& player);
};

#endif // OPTIONS_H