#include "GameState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;
using namespace RedSnowEngine::Physics;

void GameState::Initialize()
{
    mLevelFile = L"../../Assets/Levels/level.json";
    mGameWorld.LoadLevel(mLevelFile);
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
    if (ImGui::Button("ReloadLevel"))
    {
        mGameWorld.Terminate();
        mGameWorld.LoadLevel(mLevelFile);
    }
    ImGui::End();
}