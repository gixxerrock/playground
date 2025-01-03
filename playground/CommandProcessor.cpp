//
//  CommandProcessor.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-29.
//
#include <cstring>
#include <assert.h>
#include "core/Scene.hpp"
#include "core/Event.hpp"
#include "core/SineGenerator.hpp"
#include "core/Envelope.hpp"
#include "CommandProcessor.hpp"

// commands:
// create type name
// connect srcName srcOutput dstName dstOutput
// event type val1 val2 val3
// param compName paramName value

//const char *delimiterStr =  " ,./";
CommandProcessor::CommandProcessor(Scene *_scene)
{
    scene = _scene;
}

bool CommandProcessor::ProcessCommandString(const char *cmdStr, double time)
{
    char a1[64];
    char a2[64];
    char a3[64];
    char a4[64];
    char a5[64];
    int numCmds = sscanf(cmdStr,"%s %s %s %s %s", a1, a2, a3, a4, a5);

    if(strncmp(a1, "create", 6) == 0)
    {
        if (numCmds < 3) {
            return false;
        }
        else if (numCmds == 3) {
            return scene->CreateComponent(a2, a3);
        } else {
            return scene->CreateComponent(a2, a3, a4);
        }
        
    }
    else if (strncmp(a1, "connect", 7) == 0)
    {
        if (numCmds != 5) {
            return false;
        }
        return scene->Connect(a2, a3, a4, a5);
    }
    else if (strncmp(a1, "event", 5) == 0)
    {
        if (strncmp(a2, "NoteOn", 6) == 0)
        {
            int channel = 0;
            int pitch = 0;
            int velocity = 64;
            sscanf(a3," %d", &channel);
            sscanf(a4," %d", &pitch);
            sscanf(a5," %d", &velocity);
            NoteOnEvent event((uint8_t)channel, (uint8_t)pitch, (uint8_t)velocity);

            scene->HandleEvent(&event, time);
            return true;
        }
        else if (strncmp(a2, "NoteOff", 7) == 0)
        {
            int channel = 0;
            int pitch = 0;
            sscanf(a3," %d", &channel);
            sscanf(a4," %d", &pitch);
            NoteOffEvent event((uint8_t)channel, (uint8_t)pitch);

            scene->HandleEvent(&event, time);
            return true;
        }
    }
    else if (strncmp(a1, "param", 5) == 0)
    {
        float value;
        sscanf(a4, "%f", &value);

        return scene->SetParameter(a2, a3, &value);
    }
    return false;
}
