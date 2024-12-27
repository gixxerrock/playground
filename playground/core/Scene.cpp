//
//  Scene.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-23.
//
#include "Scene.hpp"
#include "Component.hpp"
#include "SineGenerator.hpp"
#include "SoundObject.hpp"
#include "Event.hpp"

Scene::Scene(uint32_t _rate, uint8_t _channels)
{
    SampleRate = _rate;
    SampleTime =  1.0 / SampleRate;
    NumChannels = _channels;
    tempo = 120.0;

    // initialize MidiNote2Hz table
    for(auto i = 0; i < 128; i++)
    {
        MidiNoteTable[i] = 440.0 * pow( 2.0, (i - 69) / 12.0);
    }
    
    outputCh0 = 0.0;
    outputCh1 = 0.0;
}

void Scene::CreateBuildingBlock(char const *type, char const *name)
{
    BuildingBlock *blk = new SineGenerator_x(this, name);
    buildingBlockList.push_back(blk);
    soundObjectList.push_back((SoundObject*)blk);
}

void Scene::CreateComponent(char const *type, char const *name)
{
    if(strcmp(type, "SineGenerator") == 0)
    {
        Component *comp = new SineGenerator(this, name);
        componentList.push_back(comp);
    }
}

void Scene::ConnectOutput(char const *compName, char const *outName, uint8_t channel)
{
    for (auto c : componentList) 
    {
        // found matching component block
        if (strcmp(compName, c->name) == 0) 
        {
            if (channel == 0)
            {
                ComponentIO compIo("outputCh0", &outputCh0, "Float");

                c->SetOutput(outName, &compIo);
            }
            else if (channel == 1)
            {
                ComponentIO compIo("outputCh1", &outputCh1, "Float");

                c->SetOutput(outName, &compIo);
            }
            break;
        }
    }
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

    for(auto c : componentList) {
        c->HandleEvent(event, time);
    }
}

void Scene::ProcessBuffer(float *inData, float *outData, uint32_t numFrames, double curTime)
{
    if(0)
    {
        for(auto o : soundObjectList) {
            o->ProcessBuffer(inData, outData, numFrames, curTime);
        }
    } else 
    {
        float *pOut = outData;
        for(auto i = 0; i < numFrames; i++)
        {
            double time = curTime + (i * SampleTime);

            for(auto c : componentList){
                c->UpdateTick(time);
            }

            // all components have run, move output sample into buffer
            *pOut++ = outputCh0;
            *pOut++ = outputCh1;
        }
    }
}
