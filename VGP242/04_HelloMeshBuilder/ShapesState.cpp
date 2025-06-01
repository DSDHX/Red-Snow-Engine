#include "ShapesState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Math;
using namespace RedSnowEngine::Input;

void ShapeState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mTransformBuffer.Initialize(sizeof(Math::Matrix4));

    CreateShape();
    mMeshBuffer.Initialize(mMesh);

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTransformColor.fx";
    mVertexShader.Initialize<VertexPC>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);
}

void ShapeState::Terminate()
{
    mMesh.vertices.clear();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
}

void ShapeState::Update(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W)) mCamera.Walk(moveSpeed * deltaTime);
    else if (input->IsKeyDown(KeyCode::S)) mCamera.Walk(-moveSpeed * deltaTime);
    if (input->IsKeyDown(KeyCode::D)) mCamera.Strafe(moveSpeed * deltaTime);
    else if (input->IsKeyDown(KeyCode::A)) mCamera.Strafe(-moveSpeed * deltaTime);
    if (input->IsKeyDown(KeyCode::E)) mCamera.Rise(moveSpeed * deltaTime);
    else if (input->IsKeyDown(KeyCode::Q)) mCamera.Rise(-moveSpeed * deltaTime);

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}

void ShapeState::Render()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);

    Math::Matrix4 matWorld = Math::Matrix4::Identity;
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 matFianl = matWorld * matView * matProj;
    Math::Matrix4 wvp = Math::Transpose(matFianl);
    mTransformBuffer.Update(&wvp);

    mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
    //mMesh = MeshBuilder::CreateCubePC(1.0f);
    //mMesh = MeshBuilder::CreatePyramidPC(1.0f);
    //mMesh = MeshBuilder::CreateRectanglePC(1.0f, 2.0f, 4.0f);
    //mMesh = MeshBuilder::CreatePlanePC(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreateCylinderPC(20, 3);
    mMesh = MeshBuilder::CreateSpherePC(30, 30, 1.0f);
}