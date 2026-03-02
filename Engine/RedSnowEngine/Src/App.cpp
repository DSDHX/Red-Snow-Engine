#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Core;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;
using namespace RedSnowEngine::Physics;
using namespace RedSnowEngine::Audio;

void App::Run(const AppConfig& config)
{
    LOG("App Started");

    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr), 
        config.appName,
        config.winWidth, 
        config.winHeight);
    auto handle = myWindow.GetWindowHandle();
    GraphicsSystem::StaticInitialize(handle, false);
    InputSystem::StaticInitialize(handle);
    DebugUI::StaticInitialize(handle, false, true);
    SimpleDraw::StaticInitialize(config.maxVertexCount);
    TextureManager::StaticInitialize(L"../../Assets/Textures");
    ModelManager::StaticInitialize(L"../../Assets/Models");

    PhysicsWorld::Settings settings;
    PhysicsWorld::StaticInitialize(settings);
    EventManager::StaticInitialize();
    AudioSystem::StaticInitialize();
    SoundEffectManager::StaticInitialize(L"../../Assets/Audio");

    ASSERT(mCurrentState != nullptr, "App: need an app state to run");
    mCurrentState->Initialize();

    InputSystem* input = InputSystem::Get();
    mRunning = true;
    while (mRunning)
    {
        myWindow.ProcessMessages();
        input->Update();

        if (!myWindow.IsActive() || input->IsKeyPressed(KeyCode::ESCAPE))
        {
            Quit();
            continue;
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }

        AudioSystem::Get()->Update();

        float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
        if (deltaTime < 0.5f)
#endif
        {
            mCurrentState->Update(deltaTime);
            PhysicsWorld::Get()->Update(deltaTime);
        }

        GraphicsSystem* gs = GraphicsSystem::Get();
        gs->BeginRender();
            mCurrentState->Render();
            DebugUI::BeginRender();
                mCurrentState->DebugUI();
            DebugUI::EndRender();
        gs->EndRender();
    }

    LOG("App Quit");
    mCurrentState->Terminate();

    SoundEffectManager::StaticTerminate();
    AudioSystem::StaticTerminate();
    EventManager::StaticTerminate();
    PhysicsWorld::StaticTerminate();
    ModelManager::StaticTerminate();
    TextureManager::StaticTerminate();
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    InputSystem::StaticTerminate();
    GraphicsSystem::StaticTerminate();
    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}

void App::ChangeState(const std::string& stateName)
{
    auto iter = mAppStates.find(stateName);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}