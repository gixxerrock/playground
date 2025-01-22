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

int ProcessComponents(const XMLElement *pComponents, Scene *pScene)
{
    int numComponents = 0;

    const XMLElement *pComp = pComponents->FirstChildElement();
    while(pComp)
    {
        const char* type = pComp->Name();
        assert(type);

        const char *name = nullptr;
        pComp->QueryStringAttribute("Name", &name);

        if (strcmp(type, "SineGenerator") == 0)
        {
            pScene->CreateComponent(type, name );
        } 
        else if (strcmp(type, "Envelope") == 0)
        {
            pScene->CreateComponent(type, name );
        }

        pComp = pComp->NextSiblingElement();
        numComponents++;
    }

    return numComponents;
}

bool XmlTestLoad(const char* filename, Scene **ppScene)
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
    return true;
}
