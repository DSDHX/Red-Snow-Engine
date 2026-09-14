#include "GameState.h"

using namespace RedSnowEngine;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    wchar_t executable[MAX_PATH]{};
    GetModuleFileNameW(nullptr, executable, MAX_PATH);
    auto directory = std::filesystem::path(executable).parent_path();
    while (!directory.empty())
    {
        auto sample = directory / "VGP331" / "26_PlayerMovementUDP";
        if (std::filesystem::exists(sample) && std::filesystem::exists(directory / "Assets"))
        {
            std::filesystem::current_path(sample);
            break;
        }
        auto parent = directory.parent_path();
        if (parent == directory) break;
        directory = parent;
    }
    if (!std::filesystem::exists("../../Assets/Textures/Images/mario.png"))
    {
        MessageBoxW(nullptr, L"Cannot find Assets. Run this sample inside the Red-Snow-Engine repository.",
            L"UDP Maze", MB_OK | MB_ICONERROR);
        return 1;
    }

    AppConfig config;
    config.appName = L"Player Movement UDP";
    config.winWidth = 1000;
    config.winHeight = 760;
    App& app = MainApp();
    app.AddState<GameState>("GameState");
    app.Run(config);
    return 0;
}