#ifndef TOOLS_H
#define TOOLS_H

#include "raylib.h"

/*struct Tool
{
    Texture2D sprite;

};*/

struct Player;

namespace Tool
{
    enum Type : int
    {
        HAMMER = 0,
        AXE,
        GLOVE,
        WAND,
        GLASSES,
        BINOCULARS,
        PORTAL,
        JOKER,
        TYPES,
        NONE
    };

    Texture2D getSprite(Type type);
    void JokerEffect(Player& player);


}

#endif // TOOLS_H