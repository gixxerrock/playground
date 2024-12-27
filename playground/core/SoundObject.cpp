//
//  SoundObject.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#include <math.h>
#include <string.h>
#include <assert.h>
#include "Scene.hpp"
#include "Event.hpp"
#include "SoundObject.hpp"

BuildingBlock::BuildingBlock(Scene *_parent, char const *_name)
{
    parentScene = _parent;
    strcpy(name, _name);

    // handy constants
    sampleRate = parentScene->SampleRate;
    sampleTime = 1.0/sampleRate;
}

SoundObject::SoundObject(Scene *_parent, char const *_name)
    : BuildingBlock(_parent, _name)
{
    noteStartTime = 0;
    lastBufferTime = 0;
}

SineGenerator_x::SineGenerator_x(Scene *_parent, char const *_name) :
    SoundObject(_parent, _name)
{
    /* initialise sinusoidal wavetable */
    for( uint32_t i = 0; i < TABLE_SIZE; i++ )
    {
        sine[i] = (float) sin( 2.0 * ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    leftPhase = 0;
    rightPhase = 0;
    freqScale = 1.0;
}

void SineGenerator_x::HandleEvent(Event *event, double time)
{
    switch(event->type)
    {
        case(Event::NoteOn):
            state = 1;
            noteStartTime = time;
            midiPitch = ((NoteOnEvent *)event)->pitch;

            freqScale = parentScene->MidiNoteToHz(midiPitch) * (float)TABLE_SIZE / sampleRate;
            ampScale = ((NoteOnEvent *)event)->velocity / 255.0;
            break;
        
        case(Event::NoteOff):
            state = 0;
            break;

        default:
            assert(0);
    }
}

void SineGenerator_x::ProcessBuffer(float *inData, float *outData, int numFrames, double curTime)
{
    if(state == 0)
    {
        memset(outData, 0, sizeof(float) * 2 * numFrames );
        return;
    }
    
    for( uint32_t i = 0; i < numFrames; i++ )
    {
        float time = curTime + (i * sampleTime) - noteStartTime;

        leftPhase = (uint32_t)((time * freqScale * 1.0) / sampleTime) % TABLE_SIZE;
        rightPhase = (uint32_t)((time * freqScale * 3.0) / sampleTime) % TABLE_SIZE;

        *outData++ = ampScale * sine[leftPhase];
        *outData++ = ampScale * sine[rightPhase];
    }

    lastBufferTime = curTime;
}
