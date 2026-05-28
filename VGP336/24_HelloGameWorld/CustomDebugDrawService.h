#pragma once

#include "CustomTypeIds.h"
#include <RedSnowEngine/Inc/RedSnowEngine.h>

class CustomDebugDrawComponent;

class CustomDebugDrawService : public RedSnowEngine::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomDebugDrawDisplay);

    void Render() override;

    void Register(const CustomDebugDrawComponent* debugDrawcComponent);
    void Unregister(const CustomDebugDrawComponent* debugDrawcComponent);

private:
    using CustomDebugDrawComponents = std::vector<const CustomDebugDrawComponent*>;
    CustomDebugDrawComponents mCustomDebugDrawComponents;
};