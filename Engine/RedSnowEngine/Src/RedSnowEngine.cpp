#include "Precompiled.h"
#include "RedSnowEngine.h"

RedSnowEngine::App& RedSnowEngine::MainApp()
{
    static App sApp;
    return sApp;
}