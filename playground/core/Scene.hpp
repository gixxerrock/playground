//
//  Scene.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#pragma once
#include <stdint.h>
#include <vector>

class BuildingBlock;
class SoundObject;
class Event;
class Component;

class Scene
{
public:
        
    Scene(uint32_t _rate, uint8_t _channels);
    ~Scene() {}
    
    float GetTempo(void) { return tempo; }              // current tempo in bpm
    uint32_t GetMaxFrames(void) { return 512; }         // max number of samples in buffer
    
    void CreateComponent(char const *type, char const *name);

    void ConnectOutput(char const *compName, char const *outName, uint8_t channel);

    void CreateBuildingBlock(char const *type, char const *name);
    void SetParameter(char const *name, char const *parameter, void *value);

    void ProcessBuffer(float *inData, float *outData, uint32_t numFrames, double curTime);

    void HandleEvent(Event *event, double time);

    // usefull utility functions
    float MidiNoteTable[128];
    float MidiNoteToHz(uint8_t n) { return MidiNoteTable[n]; }

public:
    uint32_t SampleRate;
    float SampleTime;
    uint8_t NumChannels;
    
    float tempo;
    
private:
    std::vector<BuildingBlock *> buildingBlockList;    
    std::vector<SoundObject*> soundObjectList;
    
    std::vector<Component*> componentList;

    // outputs (per tick) from a Component
    float outputCh0;
    float outputCh1;
};
