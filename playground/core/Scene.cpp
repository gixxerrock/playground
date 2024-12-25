//
//  Scene.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#include "Scene.hpp"
#include "SoundObject.hpp"

Scene::Scene(int _rate, int _channels)
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
void Scene::Start(void)
{
    for(auto o : soundObjectList) {
        o->Start();
    }
}
void Scene::Stop(void)
{
    for(auto o : soundObjectList) {
        o->Stop();
    }
}

void Scene::ProcessBuffer(float *inData, float *outData, int numFrames, double curTime)
{
    for(auto o : soundObjectList)
    {
        o->ProcessBuffer(inData, outData, numFrames, curTime);
    }
}
