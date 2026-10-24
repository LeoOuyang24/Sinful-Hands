#include "../include/tools.h"
#include "../include/sprites.h"
#include "../include/player.h"

Texture2D Tool::getSprite(Type type)
{
    switch (type)
    {
        case HAMMER:
            return SpriteManager::getSprite("hammer.png");
        case AXE:
            return SpriteManager::getSprite("axe.png");
        case WAND:
            return SpriteManager::getSprite("wand.png");
        case GLOVE:
            return SpriteManager::getSprite("glove.png");
        case GLASSES:
            return SpriteManager::getSprite("magnifying_glass.png");
        case BINOCULARS:
            return SpriteManager::getSprite("binoculars.png");
        case PORTAL:
            return SpriteManager::getSprite("portal_tool.png");
        case JOKER:
            return SpriteManager::getSprite("joker.png");
    }
    return {};
}

void Tool::JokerEffect(Player& player)
{
    if (player.currentEnemy && player.deck.size() > 0)
    {
        Card::CardIdentifier name = player.currentEnemy->name;
        player.draw();
        //TODO: this may cause problems if enemy cards can have special metadata as name alone wont be enough
        player.addCardToDeck(name);
    }
}