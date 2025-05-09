#include <RedSnowEngine/Inc/RedSnowEngine.h>
#include "ShapesState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    RedSnowEngine::AppConfig config;
    config.appName = L"Hello Shapes";

    RedSnowEngine::App& myApp = RedSnowEngine::MainApp();
    //myApp.AddState<ShapeState>("ShapeState");
    myApp.AddState<TriforceShapeState>("TriforceShapeState");
    myApp.AddState<DiamondShapeState>("DiamondShapeState");
    myApp.AddState<HeartShapeState>("HeartShapeState");

    myApp.Run(config);

    return 0;
}