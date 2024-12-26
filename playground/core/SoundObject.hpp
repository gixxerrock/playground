//
//  SoundObject.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#pragma once

class Scene;
class Event;

class BuildingBlock
{
public:
    BuildingBlock(Scene *_parent, char const *_name);

protected:
    // handy constants
    uint32_t sampleRate;
    float sampleTime;
    
    Scene *parentScene;

private:
    char name[256];
};

class SoundObject : public BuildingBlock
{
public:
    SoundObject(Scene *_parent, char const *_name);
    ~SoundObject() { }
    
    virtual void HandleEvent(Event *event, double time) { }
    
    virtual void ProcessBuffer(float *inData, float *outData, int numFrames, double curTime) = 0;
    
protected:
    
    uint8_t state;              // 0 - stopped  1 - playing
    uint8_t midiPitch;              // midi pitch
    uint8_t midiVelocity;
    
    double noteStartTime;
    double lastBufferTime;
};

class SineGenerator : public SoundObject
{
public:
    SineGenerator(Scene *_parent, char const *_name);
    
    void HandleEvent(Event *event, double time);
    void ProcessBuffer(float *inData, float *outData, int numFrames, double curTime);

private:
    static constexpr int TABLE_SIZE = 2048;
    
    float sine[TABLE_SIZE];
    uint32_t leftPhase;
    uint32_t rightPhase;
    
    float freqScale;
    float ampScale;
};
