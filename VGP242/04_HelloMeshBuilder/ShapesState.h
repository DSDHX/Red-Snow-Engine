#pragma once

#include <RedSnowEngine/Inc/RedSnowEngine.h>

class ShapeState : public RedSnowEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;

protected:
    virtual void CreateShape();

    RedSnowEngine::Graphics::MeshPC mMesh;
    RedSnowEngine::Graphics::Camera mCamera;
    RedSnowEngine::Graphics::ConstantBuffer mTransformBuffer;
    RedSnowEngine::Graphics::MeshBuffer mMeshBuffer;
    RedSnowEngine::Graphics::VertexShader mVertexShader;
    RedSnowEngine::Graphics::PixelShader mPixelShader;
};