#ifndef OPTIONS_H
#define OPTIONS_H

#include <array>
#include <functional>

#include "actions.h"

struct Player;



struct OptionVerifier
{
    std::function<bool(const Player& player)> verifier;
    std::function<void(Player& player)> boardProcessor = [](Player& player){  };
    std::function<void(const Rectangle& rect)> renderer = [](const Rectangle& rect){ DrawRectangle(rect.x,rect.y,rect.width,rect.height,RED); };

    bool verify(const Player& player){ return !verifier || verifier(player); }
    void render(const Rectangle& rect){ if (renderer) renderer(rect); }
    void postPick(Player& player) { if (boardProcessor) boardProcessor(player); }

    operator bool() const //a verifier is effectively non existent if it has no verification function
    {
        return static_cast<bool>(verifier);
    }
};

OptionVerifier ResourceVerifier(const Resources& resources);
OptionVerifier CardVerifier(Card::CardIdentifier id);

struct Option
{
    Actions::Consequences consequences;
    OptionVerifier verifier;

    Option(const Actions::Consequences& cons_, OptionVerifier verifier = {});
    Option(const Actions::Consequences& cons_, const Resources& resources);

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