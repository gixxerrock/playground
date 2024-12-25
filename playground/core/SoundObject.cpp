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
}

SoundObject::SoundObject(Scene *_parent, char const *_name)
    : BuildingBlock(_parent, _name)
{
    /* initialise sinusoidal wavetable */
    for( uint32_t i = 0; i < TABLE_SIZE; i++ )
    {
        sine[i] = (float) sin( 2.0 * ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    leftPhase = 0;
    rightPhase = 0;
    lastTime = 0;
    state = 0;
}

void SoundObject::HandleEvent(Event *event)
{
    switch(event->type)
    {
        case(Event::NoteOn):
            state = 1;
            break;
        
        case(Event::NoteOff):
            state = 0;
            break;

        default:
            assert(0);
    }
}

void SoundObject::ProcessBuffer(float *inData, float *outData, int numFrames, double curTime)
{
    if(state == 0)
    {
        memset(outData, 0, sizeof(float) * 2 * numFrames );
        return;
    }
    
    for( uint32_t i = 0; i < numFrames; i++ )
    {
        *outData++ = sine[leftPhase];
        *outData++ = sine[rightPhase];
        leftPhase += 1;
        if( leftPhase >= TABLE_SIZE ) leftPhase -= TABLE_SIZE;
        rightPhase += 3;
        if( rightPhase >= TABLE_SIZE ) rightPhase -= TABLE_SIZE;
    }
    lastTime = curTime;
}
