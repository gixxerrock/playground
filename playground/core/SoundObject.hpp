//
//  SoundObject.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#pragma once

class Scene;

class BuildingBlock
{
public:
    BuildingBlock(Scene *_parent, char const *_name);

protected:
    Scene *parentScene;

private:
    char name[256];
};

class SoundObject : public BuildingBlock
{
public:
    SoundObject(Scene *_parent, char const *_name);
    ~SoundObject() { }
    
    void Start() { state = 1; }
    void Stop() { state = 0; }
    
    void ProcessBuffer(float *inData, float *outData, int numFrames, double curTime);
    
protected:
    uint8_t state;              /* 0 - stopped  1 - playing */
    
    static constexpr int TABLE_SIZE = 2048;
    
    float sine[TABLE_SIZE];
    uint32_t leftPhase;
    uint32_t rightPhase;
    double lastTime;
};
