#include "ShapesState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Math;

void ShapeState::Initialize()
{
    CreateShape();
    mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), mVertices.size());

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoColor.fx";
    mVertexShader.Initialize<VertexPC>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);
}

void ShapeState::Terminate()
{
    mVertices.clear();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{
}

void ShapeState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
    mVertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Red });
    mVertices.push_back({ { 0.0f, 0.75f, 0.0f}, Colors::Blue });
    mVertices.push_back({ { 0.5f, 0.0f, 0.0f}, Colors::Green });

    mVertices.push_back({ {-0.5f, 0.0f, 0.0f}, Colors::Red });
    mVertices.push_back({ { 0.5f, 0.0f, 0.0f}, Colors::Blue });
    mVertices.push_back({ { 0.0f, -0.75f, 0.0f}, Colors::Green });
}