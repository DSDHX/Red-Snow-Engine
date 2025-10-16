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
    RedSnowEngine::Graphics::Camera mRenderTargetCamera;

    RedSnowEngine::Graphics::SimpleTextureEffect mSimpleTextureEffect;

    RedSnowEngine::Graphics::SimpleTextureEffect::RenderData mObject0;
    RedSnowEngine::Graphics::SimpleTextureEffect::RenderData mObject1;

    RedSnowEngine::Graphics::RenderTarget mRenderTarget;

    RedSnowEngine::Math::Matrix4 mWorldMat = RedSnowEngine::Math::Matrix4::Identity;
};