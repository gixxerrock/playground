//
//  XmlProcessor.cpp
//  playground
//
//  Created by Shawn Best on 2025-01-21.
//
#include "assert.h"
#include "core/Scene.hpp"

#include "3rdparty/tinyxml2.h"
#include "XmlProcessor.hpp"

using namespace tinyxml2;

void ProcessComponent(const XMLElement *pComp, Scene *pScene)
{
    // get component type
    const char* type = pComp->Name();
    assert(type);

    // get component name
    const char *name = nullptr;
    pComp->QueryStringAttribute("Name", &name);
    
    if (!pScene->CreateComponent(type, name ))
    {
        printf("Error creating %s : %s\n", type, name );
        return;
    }

    // process element's attributes
    const XMLElement *pElem = pComp->FirstChildElement();
    while(pElem)
    {
        const char* param = pElem->Name();
        assert(param);

        float value;
        pElem->QueryFloatText(&value);

        pScene->SetParameter(name, param, &value);

        pElem = pElem->NextSiblingElement();
    }
}

void ProcessConnection(const XMLElement *pElem, Scene *pScene)
{
    const char *srcName = nullptr;
    pElem->QueryStringAttribute("srcName", &srcName);
    assert(srcName);

    const char *srcOut = nullptr;
    pElem->QueryStringAttribute("srcOut", &srcOut);
    assert(srcOut);

    const char *dstName = nullptr;
    pElem->QueryStringAttribute("dstName", &dstName);
    assert(dstName);
 
    const char *dstIn = nullptr;
    pElem->QueryStringAttribute("dstIn", &dstIn);
    assert(dstIn);

    if (strcmp(dstName, "mainScene") == 0)
    {
        int chan = 0;
        if (dstIn[0] == '1') {
            chan = 1;
        }
        pScene->ConnectOutput(srcName, srcOut, chan);
    } 
    else
    {
        pScene->Connect(srcName, srcOut, dstName, dstIn);
    }
}


int ProcessComponents(const XMLElement *pComponents, Scene *pScene)
{
    int numComponents = 0;

    const XMLElement *pComp = pComponents->FirstChildElement();
    while(pComp)
    {
        ProcessComponent(pComp, pScene);

        pComp = pComp->NextSiblingElement();
        numComponents++;
    }

    return numComponents;
}

bool XmlLoadSetupFile(const char* filename, Scene **ppScene)
{
    XMLDocument xmlDoc;
    XMLError ret = xmlDoc.LoadFile(filename);
	if (ret != XML_SUCCESS)
	{
		printf("Error: failed to load:  %s\n", filename);
        return false;
	}

    const XMLElement* sceneElem = xmlDoc.FirstChildElement( "Scene" );
    if(!sceneElem) return false;

    int sampleRate = 44100;
    const XMLElement * pElement = sceneElem->FirstChildElement("SampleRate");
    if(pElement) {
        pElement->QueryIntText(&sampleRate);
    }

    int numChannels = 2;
    pElement = sceneElem->FirstChildElement("NumChannels");
    if(pElement) {
        pElement->QueryIntText(&numChannels);
    }
    
    *ppScene = new Scene(sampleRate, numChannels);

    pElement = sceneElem->FirstChildElement("Components");
    if(pElement) {
        ProcessComponents(pElement, *ppScene);
    }
    
    // process connections
    pElement = sceneElem->FirstChildElement("Connections");
    pElement = pElement->FirstChildElement("Connect");
    while(pElement)
    {
        ProcessConnection(pElement, *ppScene);
        pElement = pElement->NextSiblingElement();
    }

    return true;
}
