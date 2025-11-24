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

    RedSnowEngine::Graphics::RenderGroup mCharacter;
    RedSnowEngine::Graphics::RenderObject mGround;

    RedSnowEngine::Graphics::StandardEffecr mStandardEffect;
    RedSnowEngine::Graphics::ShadowEffect mShadowEffect;
};