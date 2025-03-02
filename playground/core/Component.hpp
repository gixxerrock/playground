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
class Component;

// ComponentIO : a mapping of a variable inside a component class, so an external entity can modify its value via a ptr
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
    bool bSetTriggersRecalc;
    Component *parent;
    
    ComponentIO(const char* _name, void* _pData, const char* _type, bool _bSetTriggersRecalc = false, Component *_parent = nullptr);
    void SetData(void *payload);
    void SetPointer(ComponentIO *out);
};

class Component
{
public:
    Component(Scene *_parent, char const *_name);
    ~Component() {}

    ComponentIO* GetInput(const char *name);
    bool SetParameter(const char *name, void *value);
    bool SetOutput(const char *name, ComponentIO *out);

    virtual void Recalc() { }
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
