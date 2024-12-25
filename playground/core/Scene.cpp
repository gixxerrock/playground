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
}

void Scene::CreateBuildingBlock(char const *type, char const *name)
{
    BuildingBlock *blk = new SoundObject(this, name);
    buildingBlockList.push_back(blk);
    soundObjectList.push_back((SoundObject*)blk);
}
void Scene::SetParameter(char const *name, char const *parameter, void *value)
{
    // TODO:
}

void Scene::HandleEvent(Event *event)
{
    switch(event->type)
    {
        case(Event::NoteOn):
        {
            for(auto o : soundObjectList) {
                o->Start();
            }
            break;
        }
        case(Event::NoteOff):
        {
            for(auto o : soundObjectList) {
                o->Stop();
            }
            break;
        }
            
        
    }
}

void Scene::ProcessBuffer(float *inData, float *outData, uint32_t numFrames, double curTime)
{
    for(auto o : soundObjectList)
    {
        o->ProcessBuffer(inData, outData, numFrames, curTime);
    }
}
