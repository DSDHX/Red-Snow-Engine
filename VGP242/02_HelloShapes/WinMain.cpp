#include <RedSnowEngine/Inc/RedSnowEngine.h>
#include "ShapesState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    RedSnowEngine::AppConfig config;
    config.appName = L"Hello Shapes";

    RedSnowEngine::App& myApp = RedSnowEngine::MainApp();
    myApp.AddState<ShapeState>("Shapes State");
    myApp.Run(config);

    return 0;
}