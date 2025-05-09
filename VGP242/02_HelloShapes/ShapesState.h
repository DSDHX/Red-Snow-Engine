#pragma once

#include <RedSnowEngine/Inc/RedSnowEngine.h>

class TriforceShapeState;
class DiamondShapeState;
class HeartShapeState;

class ShapeState : public RedSnowEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    virtual void Update(float deltaTime) override = 0;
    void Render() override;

protected:
    virtual void CreateShape() = 0;

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

    void RecreateVertexBuffer();
};

class TriforceShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShape() override;
};

class DiamondShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShape() override;
};

class HeartShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;

protected:
    void CreateShape() override;
};