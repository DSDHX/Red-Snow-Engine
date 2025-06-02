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
    void RenderObjects(const RedSnowEngine::Graphics::Camera& camera);

    RedSnowEngine::Graphics::Camera mCamera;
    RedSnowEngine::Graphics::Camera mRenderTargetCamera;

    RedSnowEngine::Graphics::ConstantBuffer mTransformBuffer;
    RedSnowEngine::Graphics::VertexShader mVertexShader;
    RedSnowEngine::Graphics::PixelShader mPixelShader;
    RedSnowEngine::Graphics::Sampler mSampler;

    RedSnowEngine::Math::Matrix4 mWorldMat = RedSnowEngine::Math::Matrix4::Identity;
    RedSnowEngine::Graphics::MeshBuffer mObject;
    RedSnowEngine::Graphics::TextureId mTextureId;

    RedSnowEngine::Graphics::RenderTarget mRenderTarget;
};