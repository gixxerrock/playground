//
//  SoundObject.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#pragma once

class SoundObject
{
public:
    SoundObject(int _rate, int _channels);
    ~SoundObject() { }
    
    void Start() { state = 1; }
    void Stop() { state = 0; }
    
    void ProcessBuffer(float *inData, float *outData, int numFrames, double curTime);
    
protected:
    int SampleRate;
    int NumChannels;
    int state;              /* 0 - stopped  1 - playing */
    
    static constexpr int TABLE_SIZE = 2048;
    
    float sine[TABLE_SIZE];
    int leftPhase;
    int rightPhase;
    double lastTime;
};
