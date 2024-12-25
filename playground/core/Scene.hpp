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
class Scene
{
public:
    Scene(uint32_t _rate, uint8_t _channels);
    ~Scene() {}
    
    void CreateBuildingBlock(char const *type, char const *name);
    void SetParameter(char const *name, char const *parameter, void *value);

    void ProcessBuffer(float *inData, float *outData, uint32_t numFrames, double curTime);

    void HandleEvent(Event *event);
    
public:
    uint32_t SampleRate;
    uint8_t NumChannels;
    
private:
    std::vector<BuildingBlock *> buildingBlockList;    
    std::vector<SoundObject*> soundObjectList;
};
