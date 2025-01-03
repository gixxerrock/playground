//
//  Envelope.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-27.
//

#pragma once
#include "Component.hpp"

class Event;

class Envelope : public Component
{
public:
    Envelope(Scene *_parent, char const *_name);
    
    void HandleEvent(Event *event, double time);
    void UpdateTick(double time);
    
private:
    
    // inputs
    float input1;

    // note these are specified in seconds
    float attackTime;           // 0.001 .. 1.0
    float decayTime;            // 0.001 .. 1.0
    float sustainLevel;         // 0 .. 1.0 
    float releaseTime;          // 0.001 .. 1.0
    
    // outputs
    float *output1;
    
    uint32_t state; // 0:inactive 1:initial(AD) 2:sustain  3: release
    double noteStartTime;
    double noteDecayTime;
    double noteSustainTime;
    double noteReleaseTime;
    double noteStopTime;
};
