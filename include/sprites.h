#ifndef SPRITES_H
#define SPRITES_H

#include <unordered_map>
#include <raylib.h>

struct SpriteManager
{
    static std::unordered_map<std::string,Texture2D> sprites;

    static void loadSprites(std::string path);
    static Texture2D getSprite(std::string name);
};

#endif // SPRITES_H