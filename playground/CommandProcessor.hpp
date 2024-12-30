//
//  CommandProcessor.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-29.
//
#pragma once

class Scene;

class CommandProcessor
{
public:
    CommandProcessor(Scene *_scene);

    bool ProcessCommandString(const char *cmdStr);

private:
    Scene *scene;
};
