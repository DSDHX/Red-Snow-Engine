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

    using Vertices = std::vector<RedSnowEngine::Graphics::VertexPC>;
    Vertices mVertices;

    RedSnowEngine::Graphics::MeshBuffer mMeshBuffer;
    RedSnowEngine::Graphics::VertexShader mVertexShader;
    RedSnowEngine::Graphics::PixelShader mPixelShader;
};