#include <RedSnowEngine/Inc/RedSnowEngine.h>
#include "ShapesState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    RedSnowEngine::AppConfig config;
    config.appName = L"Hello Mesh Builder";

    RedSnowEngine::App& myApp = RedSnowEngine::MainApp();
    myApp.AddState<ShapeState>("ShapeState");
    myApp.Run(config);

    return 0;
}