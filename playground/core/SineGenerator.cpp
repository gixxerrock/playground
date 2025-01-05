//
//  SineOscillator.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-26.
//
#include <assert.h>
#include "Scene.hpp"
#include "Event.hpp"
#include "SineGenerator.hpp"

SineGenerator::SineGenerator(Scene *_parent, char const *_name, float baseFrequency) : Component(_parent, _name)
{
    /* initialise sinusoidal wavetable */
    for( uint32_t i = 0; i < TABLE_SIZE; i++ )
    {
        sine[i] = (float) sin( 2.0 * ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    
    freqScale = 1.0;
    amplitudeScale = 1.0f;
    amplitudeOffset = 0.0f;
    freqOffset = 0.0;
    freqRatio = 1.0;
    respondNoteEvent = true;
    state = 0;

    // if a base frequency is specified calculate appropriate scale and disable midi notes
    if (baseFrequency > 0 )
    {
        respondNoteEvent = false;
        state = 1;
        freqScale = baseFrequency * (float)TABLE_SIZE / sampleRate;
    }
    output1 = nullptr;
    
    // expose parameters
    inputList.push_back( ComponentIO("amplitudeScale", &amplitudeScale, "Float" ) );
    inputList.push_back( ComponentIO("amplitudeOffset", &amplitudeOffset, "Float" ) );
    inputList.push_back( ComponentIO("freqOffset", &freqOffset, "Float" ) );
    inputList.push_back( ComponentIO("freqRatio", &freqRatio, "Float" ) );
    
    outputList.push_back( ComponentIO("output1", &output1, "Float") );
}

void SineGenerator::HandleEvent(Event *event, double time)
{
    if (!respondNoteEvent) {
        return;
    }
    
    switch(event->type)
    {
        case(Event::NoteOn):
            state = 1;
            noteStartTime = time;

            freqScale = parentScene->MidiNoteToHz(((NoteOnEvent *)event)->pitch) * (float)TABLE_SIZE / sampleRate;
            amplitudeScale = ((NoteOnEvent *)event)->velocity / 255.0;
            break;
        
        case(Event::NoteOff):
            //state = 0;
            break;

        default:
            assert(0);
    }
}

void SineGenerator::UpdateTick(double time)
{
    assert(output1);

    if(state == 0)
    {
        *output1 = 0.0;
        return;
    }
    
    // time from start of note
    float offsTime = time - noteStartTime;

    uint32_t phase = (uint32_t)((offsTime * freqScale * (freqRatio + freqOffset)) / sampleTime) % TABLE_SIZE;

    *output1 = (amplitudeScale + amplitudeOffset) * sine[phase];
}
