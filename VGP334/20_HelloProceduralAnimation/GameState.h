#pragma once

#include <RedSnowEngine/Inc/RedSnowEngine.h>

class GameState : public RedSnowEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    void UpdateCamera(float deltaTime);

    RedSnowEngine::Graphics::Camera mCamera;
    RedSnowEngine::Graphics::DirectionalLight mDirectionalLight;
    RedSnowEngine::Graphics::StandardEffecr mStandardEffect;

    RedSnowEngine::Graphics::RenderObject mGroundObject;
    RedSnowEngine::Physics::CollisionShape mGroundShape;
    RedSnowEngine::Physics::RigidBody mGroundRigidBody;

    struct Segment
    {
        RedSnowEngine::Graphics::RenderObject segment;
        float range = 0.0f;
    };
    std::vector<Segment> mSegments;
    RedSnowEngine::Physics::CollisionShape mBodyAnchorShape;
    RedSnowEngine::Physics::RigidBody mBodyAnchor;
};