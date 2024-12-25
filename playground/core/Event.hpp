//
//  Event.hpp
//  playground
//
//  Created by Shawn Best on 2024-12-24.
//
#pragma once
#include <stdint.h>

struct Event
{
    enum
    {
        NoteOn,
        NoteOff,
        Stop
    };
    
    uint8_t     type;
    uint8_t     channel;
    
    Event(uint8_t _type, uint8_t _channel)
    {
        type = _type;
        channel = _channel;
    }
};

struct NoteOn : public Event
{
    uint8_t pitch;      // midi pitch mapping
    uint8_t velocity;
    
    NoteOn(uint8_t _channel, uint8_t _pitch, uint8_t _velocity) :
        Event( Event::NoteOn, _channel)
    {
        pitch = _pitch;
        velocity = _velocity;
    }
};

struct NoteOff : public Event
{
    uint8_t pitch;      // midi pitch mapping
    
    NoteOff(uint8_t _channel, uint8_t _pitch) :
        Event( Event::NoteOff, _channel)
    {
        pitch = _pitch;
    }
};
