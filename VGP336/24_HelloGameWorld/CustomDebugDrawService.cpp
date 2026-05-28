#include "CustomDebugDrawService.h"
#include "CustomDebugDrawComponent.h"

void CustomDebugDrawService::Render()
{
    for (const CustomDebugDrawComponent* component : mCustomDebugDrawComponents)
    {
        component->AddDebugDraw();
    }
}

void CustomDebugDrawService::Register(const CustomDebugDrawComponent* debugDrawcComponent)
{
    auto iter = 
        std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), debugDrawcComponent);
    if (iter == mCustomDebugDrawComponents.end())
    {
        mCustomDebugDrawComponents.push_back(debugDrawcComponent);
    }
}

void CustomDebugDrawService::Unregister(const CustomDebugDrawComponent* debugDrawcComponent)
{
    auto iter = 
        std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), debugDrawcComponent);
    if (iter != mCustomDebugDrawComponents.end())
    {
        mCustomDebugDrawComponents.erase(iter);
    }
}