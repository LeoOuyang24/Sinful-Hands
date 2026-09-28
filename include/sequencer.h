#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <functional>
#include <list>

typedef std::function<bool()> SequenceUnit;

// Code Here
struct Sequences
{
    static std::list<SequenceUnit> sequences;

    static void addSequence(SequenceUnit);
    static void update();
};
#endif // SEQUENCER_H