#include "../include/sequencer.h"

std::list<SequenceUnit> Sequences::sequences;

void Sequences::addSequence(SequenceUnit unit)
{
    sequences.push_back(unit);
}

void Sequences::update()
{
    for (auto it = sequences.begin(); it != sequences.end();)
    {
        if ((*it)())
        {
            it = sequences.erase(it);
        }
        else
        {
            ++it;
        }
    }
}