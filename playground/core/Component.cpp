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

// takes a pointer to a data payload and writes it to the internal variable wrapped by this IO
void ComponentIO::SetData(void *payload)
{
    if (type == Float) {
        *((float *)pData) = *((float *)payload);
    } else {
        *((int *)pData) = *((int *)payload);
    }
}

// takes a variable wrapped in ComponentIO in external object and sets the internal pointer so this object can do a 
// pointer write that updates a variable in that external object
void ComponentIO::SetPointer(ComponentIO *out)
{
    if (type == Float) {
        *((float **)pData) = (float *)out->pData;
    } else {
        *((int **)pData) = (int *)out->pData;
    }
}

Component::Component(Scene *_parent, char const *_name)
{
    parentScene = _parent;
    strcpy(name, _name);

    // handy constants
    sampleRate = parentScene->SampleRate;
    sampleTime = 1.0/sampleRate;
}

ComponentIO* Component::GetInput(const char *name)
{
    for (auto &i : inputList)
    {
        if( strcmp(i.name, name) == 0){
            return &i;
        }
    }
    return nullptr;
}

bool Component::SetParameter(const char *name, void *value)
{
    ComponentIO* comp = GetInput(name);
    if (!comp) {
        return false;
    }

    comp->SetData(value);
    return true;
}

// find named output in this comonent and set pointer
bool Component::SetOutput(const char *name, ComponentIO *out)
{
    for (auto &o : outputList)
    {
        if( strcmp(o.name, name) == 0){
            o.SetPointer(out);
            return true;
        }
    }
    return false;
}
