#include "raylib.h"

#include "../include/frames.h"

float Frames::accumulator = 0;
size_t Frames::currentFrame = 0;

void Frames::accumulate()
{
    accumulator += GetFrameTime();
    currentFrame ++ ;
}

bool Frames::hasFramesLeft()
{
    bool done = accumulator >= 1.0f/Frames::fps;
    if (done)
    {
        accumulator -= 1.0f/Frames::fps;
    }
    return done;
}