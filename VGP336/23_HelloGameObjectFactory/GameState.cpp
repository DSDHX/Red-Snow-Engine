#include "GameState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;
using namespace RedSnowEngine::Physics;

void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.AddService<RenderService>();
    mGameWorld.Initialize();

    GameObject* transformGO = mGameWorld.CreateGameObject("Transform", 
        L"../../Assets/Templates/Objects/transform_obj.json");
    transformGO->Initialize();

    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera", 
        L"../../Assets/Templates/Objects/fps_camera.json");
    cameraGO->Initialize();

    GameObject* playerGO = mGameWorld.CreateGameObject("Player", 
        L"../../Assets/Templates/Objects/transform_obj.json");
    TransformComponent* playerTransform = playerGO->GetComponent<TransformComponent>();
    playerTransform->position.x = 2.0f;
    playerGO->Initialize();
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();
}