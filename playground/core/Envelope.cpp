//
//  Envelope.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-27.
//
#include <assert.h>
#include "Event.cpp"
#include "Envelope.hpp"

Envelope::Envelope(Scene *_parent, char const *_name) : Component(_parent, _name)
{
    inputLeft = 0.0;
    inputRight = 0.0;

    outputLeft = nullptr;
    outputRight = nullptr;
    
    state = 0;
    noteStartTime = 0.0;
    noteDecayTime = 0.0;
    noteSustainTime = 0.0;
    noteReleaseTime = 0.0;
    noteStopTime = 0.0;

    // ADSR setup
    attackTime = 0.1;
    decayTime = 0.2;
    sustainLevel = 0.5;
    releaseTime = 0.25;

    // expose parameters
    inputList.push_back( ComponentIO("inputLeft", &inputLeft, "Float" ) );
    inputList.push_back( ComponentIO("inputRight", &inputRight, "Float" ) );
    
    outputList.push_back( ComponentIO("outputLeft", &outputLeft, "Float") );
    outputList.push_back( ComponentIO("outputRight", &outputRight, "Float") );
}

void Envelope::HandleEvent(Event *event, double time)
{
    switch(event->type)
    {
        case(Event::NoteOn):
            state = 1;
            noteStartTime = time;
            noteDecayTime = time + attackTime;
            noteSustainTime = noteDecayTime +decayTime;
            noteReleaseTime = noteStartTime;
            break;
        
        case(Event::NoteOff):
            state = 3;
            noteReleaseTime = time;
            noteStopTime = time + releaseTime;
            break;

        default:
            assert(0);
    }
}

void Envelope::UpdateTick(double time)
{
    float level = 0.0;

    if (state == 1)
    {
        if ( time < noteDecayTime)  // in attack
        {
            level = (time - noteStartTime) /attackTime;
        }
        else if (time < noteSustainTime) // in decay
        {
            assert(decayTime > 0.0);
            level = 1.0 - (1.0 - sustainLevel)*(time - noteDecayTime)/decayTime;
        }
        else
        {
            state = 2;
            level = sustainLevel;
        }
    }
    else if (state == 2) // in sustain
    { 
        level = sustainLevel;
    } 
    else if (state == 3)  // in release
    {
        if (time >= noteStopTime) 
        {
            state = 0;
        }
        else
        {
            assert(time < noteStopTime);
            assert(noteReleaseTime > 0.0);
            level = sustainLevel * (1.0 - (time - noteReleaseTime) / releaseTime);
        }
    }
    else 
    {
        // no note
        assert(state == 0);
    }

    *outputLeft = level * inputLeft;
    *outputRight = level * inputRight;
}
