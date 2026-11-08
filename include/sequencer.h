#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <functional>
#include <list>

typedef std::function<bool()> SequenceUnit;
typedef std::list<SequenceUnit> Sequencer;

// Code Here
struct Sequences
{
    static std::list<Sequencer> sequences;

    static void addSequence(SequenceUnit);
    static void addSequence(Sequencer);
    static void update();
};
#endif // SEQUENCER_H