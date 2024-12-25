//
//  Scene.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#pragma once
#include <vector>

class BuildingBlock;
class SoundObject;
class Event;
class Scene
{
public:
    Scene(int _rate, int _channels);
    ~Scene() {}
    
    void CreateBuildingBlock(char const *type, char const *name);
    void SetParameter(char const *name, char const *parameter, void *value);

    void ProcessBuffer(float *inData, float *outData, int numFrames, double curTime);

    void HandleEvent(Event *event);
    
    //void Start(void);
    //void Stop(void);
    
public:
    int SampleRate;
    int NumChannels;
    
private:
    std::vector<BuildingBlock *> buildingBlockList;    
    std::vector<SoundObject*> soundObjectList;
};
