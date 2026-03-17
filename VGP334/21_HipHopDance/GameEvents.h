#pragma once

#include <RedSnowEngine/Inc/RedSnowEngine.h>

enum class GameEventType
{
    PressSpace = 1,
    PressEnter
};

class PressSpaceEvent : public RedSnowEngine::Core::Event
{
public:
    PressSpaceEvent() {}
    SET_EVENT_TYPE_ID(GameEventType::PressSpace)
};

class PressEnterEvent : public RedSnowEngine::Core::Event
{
public:
    PressEnterEvent() {}
    SET_EVENT_TYPE_ID(GameEventType::PressEnter)
};