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

bool CommandProcessor::ProcessCommandString(const char *cmdStr)
{
    char a1[64];
    char a2[64];
    char a3[64];
    char a4[64];
    char a5[64];
    int numCmds = sscanf(cmdStr,"%s %s %s %s %s", a1, a2, a3, a4, a5);

    if(strncmp(a1, "create", 6) == 0)
    {
        if (numCmds != 3) {
            return false;
        }
        return scene->CreateComponent(a2, a3);
    }
    else if (strncmp(a1, "connect", 7) == 0)
    {
        if (numCmds != 5) {
            return false;
        }
        return scene->Connect(a2, a3, a4, a5);
    }
    return false;
}