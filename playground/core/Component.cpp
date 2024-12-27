//
//  Component.cpp
//  playground
//
//  Created by Shawn Best on 2024-12-26.
//
#include "Scene.hpp"
#include "Component.hpp"

ComponentIO::ComponentIO(const char* _name, void* _pData, const char* _type)
{
    strcpy(name, _name);
    if (strcmp(_type, "Float") == 0)
    {
        type = Float;
    }
    pData = _pData;
}

Component::Component(Scene *_parent, char const *_name)
{
    parentScene = _parent;
    strcpy(name, _name);

    // handy constants
    sampleRate = parentScene->SampleRate;
    sampleTime = 1.0/sampleRate;
}

ComponentIO* Component::GetInput(char *name)
{
    for (auto &i : inputList)
    {
        if( strcmp(i.name, name) == 0){
            return &i;
        }
    }
    return nullptr;
}

// find named output in this comonent and set pointer
bool Component::SetOutput(const char *name, ComponentIO *out)
{
    for (auto &o : outputList)
    {
        if( strcmp(o.name, name) == 0){
            *((float **)o.pData) = (float *)out->pData;
            return true;
        }
    }
    return false;
}
