#pragma once

#include "CustomTypeIds.h"
#include <RedSnowEngine/Inc/RedSnowEngine.h>

class CustomDebugDrawComponent : public RedSnowEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void DebugUI() override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw() const;

private:
    const RedSnowEngine::TransformComponent* mTransformComponent = nullptr;
    RedSnowEngine::Math::Vector3 mPosition = RedSnowEngine::Math::Vector3::Zero;
    RedSnowEngine::Graphics::Color mColor = RedSnowEngine::Graphics::Colors::White;

    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0;
};