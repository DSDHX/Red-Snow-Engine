#include "GameState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mSimpleTextureEffect.Initialize();

    MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
    mObject0.mesh.Initialize(sphere);
    mObject1.mesh.Initialize(sphere);

    mObject0.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");
    mObject1.textureId = TextureManager::Get()->LoadTexture(L"earth.jpg");
    mObject1.matWorld = Math::Matrix4::Translation({ 0.0f, 1.0f, 0.0f });

    constexpr uint32_t size = 512;
    mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);
}

void GameState::Terminate()
{
    mRenderTarget.Terminate();
    TextureManager::Get()->RelaseTexture(mObject0.textureId);
    TextureManager::Get()->RelaseTexture(mObject1.textureId);
    mObject0.mesh.Terminate();
    mObject1.mesh.Terminate();
    mSimpleTextureEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(20.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mSimpleTextureEffect.SetCamera(mRenderTargetCamera);
    mRenderTarget.BeginRender();
        mSimpleTextureEffect.Begin();
            mSimpleTextureEffect.Render(mObject0);
            mSimpleTextureEffect.Render(mObject1);
        mSimpleTextureEffect.End();
    mRenderTarget.EndRender();

    mSimpleTextureEffect.SetCamera(mCamera);
    mSimpleTextureEffect.Begin();
        mSimpleTextureEffect.Render(mObject0);
        mSimpleTextureEffect.Render(mObject1);
    mSimpleTextureEffect.End();
}

bool gCheckValue = false;
float gFloatVal = 0.0f;
Math::Vector3 gV0 = Math::Vector3::Zero;
Math::Vector3 gV1 = Math::Vector3::One;
Math::Vector3 gV2 = Math::Vector3::XAxis;
Color gColor = Colors::White;
void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Hello Debug UI!");
    ImGui::Checkbox("IsChecked", &gCheckValue);
    ImGui::DragFloat("FloatVal", &gFloatVal);
    ImGui::DragFloat3("V0", &gV0.x, 0.1f);
    ImGui::DragFloat3("V1", &gV1.x, 0.1f);
    ImGui::DragFloat3("V2", &gV2.x, 0.1f);
    ImGui::ColorEdit4("Color", &gColor.r);

    Math::Vector3 pos = Math::GetTranslation(mWorldMat);
    if (ImGui::DragFloat3("ObjectPos", &pos.x, 0.1f))
    {
        mWorldMat._41 = pos.x;
        mWorldMat._42 = pos.y;
        mWorldMat._43 = pos.z;
    }
    ImGui::Separator();
    ImGui::Text("RenderTarget");
    ImGui::Image(mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });

    ImGui::End();

    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);
}

void GameState::UpdateCamera(float deltaTime)
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