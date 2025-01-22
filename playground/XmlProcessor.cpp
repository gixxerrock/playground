//
//  XmlProcessor.cpp
//  playground
//
//  Created by Shawn Best on 2025-01-21.
//
#include "core/Scene.hpp"

#include "3rdparty/tinyxml2.h"
#include "XmlProcessor.hpp"

using namespace tinyxml2;

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
    return true;
}
