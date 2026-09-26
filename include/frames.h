#ifndef FRAMES_H
#define FRAMES_H

#include <cstddef>

//very simple class that implements an accumulation loop
struct Frames
{
    static size_t currentFrame;
    static constexpr size_t fps = 60;
    static float accumulator;

    /**
     * @brief Increments currentFrame by 1 and accumulator by GetFrameTime()
     * 
     */
    static void accumulate();

    /**
     * @brief Decrements "accumulator" by 1.0f/fps, then returns true if accumulator had frames to run BEFORE the subtraction
     * 
     * @return true 
     * @return false 
     */
    static bool hasFramesLeft();

};
#endif // FRAMES_H


