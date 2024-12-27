//
//  Component.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-26.
//

#pragma once
#include <vector>

class Scene;
class Event;

// ComponentIO : a mapping to a variable inside a component class, so an external entity can modify its value via a ptr
struct ComponentIO
{
    enum Type
    {
        Float,
        Int
    };
    
    char name[256];
    Type type;
    void* pData;
    
    ComponentIO(const char* _name, void* _pData, const char* _type);
};

class Component
{
public:
    Component(Scene *_parent, char const *_name);
    ~Component() {}

    ComponentIO* GetInput(char *name);
    bool SetOutput(const char *name, ComponentIO *out);

    virtual void HandleEvent(Event *event, double time) { }
    virtual void UpdateTick(double time) = 0;

protected:
    // handy constants
    uint32_t    sampleRate;
    float       sampleTime;
    Scene*      parentScene;

    // derrived classes responsible to add exposed parameters to this list
    std::vector<ComponentIO> inputList;
    std::vector<ComponentIO> outputList;

public:
    char name[256];
};
