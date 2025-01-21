//
//  XmlProcessor.cpp
//  playground
//
//  Created by Shawn Best on 2025-01-21.
//
#include "3rdparty/tinyxml2.h"
#include "XmlProcessor.hpp"

using namespace tinyxml2;

bool XmlTestLoad(const char* filename)
{
    XMLDocument doc;
    XMLError ret = doc.LoadFile(filename);
	if (ret != XML_SUCCESS)
	{
		printf("Error: failed to load:  %s\n", filename);
        return false;
	}


    return true;
}
