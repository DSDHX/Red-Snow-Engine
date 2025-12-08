#include "GameState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    mCharacter.Initialize("Character01/Character01.model");

    Mesh groundMesh = MeshBuilder::CreatePlane(10, 10, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/concrete.jpg");

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    std::filesystem::path standardShader = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(standardShader);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDeptMap());

    std::filesystem::path halftoneShader = L"../../Assets/Shaders/Halftone.fx";
    mHalftoneEffect.Initialize(halftoneShader);
    mHalftoneEffect.SetCamera(mCamera);
    mHalftoneEffect.SetDirectionalLight(mDirectionalLight);
    mHalftoneEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mHalftoneEffect.SetShadowMap(mShadowEffect.GetDeptMap());
}

void GameState::Terminate()
{
    mHalftoneEffect.Terminate();
    mShadowEffect.Terminate();
    mGround.Terminate();
    mCharacter.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    mShadowEffect.Begin();
        mShadowEffect.Render(mCharacter);
    mShadowEffect.End();

    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
    mStandardEffect.End();

    mHalftoneEffect.Begin();
        mHalftoneEffect.Render(mCharacter);
    mHalftoneEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction#Light", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
    }
    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (uint32_t i = 0; i < mCharacter.renderObjects.size(); ++i)
        {
            Material& material = mCharacter.renderObjects[i].material;
            std::string renderObjectId = "RenderObject " + std::to_string(i);
            ImGui::PushID(renderObjectId.c_str());
            if (ImGui::CollapsingHeader("RenderObject"))
            {
                ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
                ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
                ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
                ImGui::ColorEdit4("Specular#Material", &material.specular.r);
                ImGui::DragFloat("Shininess#Material", &material.shininess, 0.1f, 0.1f, 1000.0f);
            }
            ImGui::PopID();
        }
    }
    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();
    mHalftoneEffect.DebugUI();
    ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}