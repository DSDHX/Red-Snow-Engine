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

    struct Vertex
    {
        RedSnowEngine::Math::Vector3 position;
        RedSnowEngine::Graphics::Color color;
    };
    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
};

class TriangleShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShape() override;
};