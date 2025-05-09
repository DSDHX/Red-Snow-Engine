#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Core;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;

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

        float deltaTime = TimeUtil::GetDeltaTime();
#if defined(_DEBUG)
        if (deltaTime < 0.5f)
#endif
        {
            mCurrentState->Update(deltaTime);
        }

        GraphicsSystem* gs = GraphicsSystem::Get();
        gs->BeginRender();
        mCurrentState->Render();
        gs->EndRender();
    }

    LOG("App Quit");
    mCurrentState->Terminate();

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