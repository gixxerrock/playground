//
//  SineGenerator.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-26.
//

#pragma once
#include "Component.hpp"

class SineGenerator : public Component
{
public:
    SineGenerator(Scene *_parent, char const *_name);
    
    void HandleEvent(Event *event, double time);
    void UpdateTick(double time);
    void Recalc(void);
    
private:
    
    // inputs
    float baseFrequency;
    float amplitudeScale;   // amplitude 0..1
    float amplitudeOffset;  // small +- number added to amplitude scale for LFO or FM
    float freqOffset;       // +- ratio to tune oscillator
    float freqRatio;        // 1.0
    
    // outputs
    float *output1;

    // internal state    
    uint8_t state;              // 0 - stopped  1 - playing
    bool respondNoteEvent;      // stand alone oscillator, or freq driven by events
    double noteStartTime;
    float freqScale;

    // sin table
    static constexpr int TABLE_SIZE = 2048;
    float sine[TABLE_SIZE];
};
