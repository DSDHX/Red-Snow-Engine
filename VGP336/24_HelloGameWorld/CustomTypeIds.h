#pragma once

#include <RedSnowEngine/Inc/TypeIds.h>

enum class CustomComponentId
{
    CustomDebugDraw = static_cast<int>(RedSnowEngine::ComponentId::Count)
};

enum class CustomServiceId
{
    CustomDebugDrawDisplay = static_cast<int>(RedSnowEngine::ServiceId::Count)
};