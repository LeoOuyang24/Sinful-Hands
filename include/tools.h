#ifndef TOOLS_H
#define TOOLS_H

#include "raylib.h"

/*struct Tool
{
    Texture2D sprite;

};*/

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
        TYPES,
        NONE
    };

    Texture2D getSprite(Type type);


}

#endif // TOOLS_H