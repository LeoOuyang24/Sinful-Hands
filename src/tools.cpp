#include "../include/tools.h"
#include "../include/sprites.h"

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
    }
    return {};
}