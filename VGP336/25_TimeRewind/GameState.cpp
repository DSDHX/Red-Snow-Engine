#include "GameState.h"
#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"
#include "TimeRewindComponent.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;
using namespace RedSnowEngine::Physics;

Service* MakeCustomService(const std::string& serviceName, GameWorld& gameWorld)
{
    if (serviceName == "CustomDebugDrawService")
    {
        return gameWorld.AddService<CustomDebugDrawService>();
    }
    return nullptr;
}

Component* MakeCustomComponent(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.AddComponent<CustomDebugDrawComponent>();
    }
    if (componentName == "TimeRewindComponent")
    {
        return gameObject.AddComponent<TimeRewindComponent>();
    }
    return nullptr;
}

Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.GetComponent<CustomDebugDrawComponent>();
    }
    if (componentName == "TimeRewindComponent")
    {
        return gameObject.GetComponent<TimeRewindComponent>();
    }
    return nullptr;
}

void GameState::Initialize()
{
    mLevelFile = L"../../Assets/Templates/Levels/level_time_rewind.json";

    GameWorld::SetCustomService(MakeCustomService);
    GameObjectFactory::SetCustomMake(MakeCustomComponent);
    GameObjectFactory::SetCustomGet(GetCustomComponent);

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