//
//  Scene.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#include "Scene.hpp"
#include "SoundObject.hpp"
#include "Event.hpp"

Scene::Scene(uint32_t _rate, uint8_t _channels)
{
    SampleRate = _rate;
    NumChannels = _channels;

    // initialize MidiNote2Hz table
    for(auto i = 0; i < 128; i++)
    {
        MidiNoteTable[i] = 440.0 * pow( 2.0, (i - 69) / 12.0);
    }
}

void Scene::CreateBuildingBlock(char const *type, char const *name)
{
    tempo = 120.0;
    
    BuildingBlock *blk = new SineGenerator(this, name);
    buildingBlockList.push_back(blk);
    soundObjectList.push_back((SoundObject*)blk);
}

void Scene::SetParameter(char const *name, char const *parameter, void *value)
{
    // TODO:
}

void Scene::HandleEvent(Event *event, double time)
{
    for(auto o : soundObjectList) {
        o->HandleEvent(event, time);
    }
}

void Scene::ProcessBuffer(float *inData, float *outData, uint32_t numFrames, double curTime)
{
    for(auto o : soundObjectList) {
        o->ProcessBuffer(inData, outData, numFrames, curTime);
    }
}
