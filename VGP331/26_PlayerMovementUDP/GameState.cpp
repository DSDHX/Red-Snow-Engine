#include "GameState.h"
#include "MazeLayout.h"
#include <algorithm>
#include <cmath>

using namespace RedSnowEngine;
using namespace RedSnowEngine::Math;
using namespace RedSnowEngine::Input;

namespace
{
    void ReadJson(Component* component, const char* json)
    {
        rapidjson::Document document;
        document.Parse(json);
        component->Deserialize(document);
    }
}

void GameState::Initialize()
{
    mWindow = GetActiveWindow();
    NetworkSession::Role role = NetworkSession::Role::Automatic;
    const std::wstring arguments = GetCommandLineW();
    if (arguments.find(L"--server") != std::wstring::npos) role = NetworkSession::Role::Server;
    else if (arguments.find(L"--client") != std::wstring::npos) role = NetworkSession::Role::Client;
    if (!mNetwork.Initialize(role)) return;

    SetWindowTextW(mWindow, mNetwork.IsServer()
        ? L"Red Snow - UDP Maze | SERVER - Player 1"
        : L"Red Snow - UDP Maze | CLIENT - Player 2");
    CreateScene();
    mSceneReady = true;
}

void GameState::CreateScene()
{
    mGameWorld.LoadLevel(L"../../Assets/Templates/Levels/level_udp_maze.json");
    mGameWorld.GetService<PhysicsService>()->SetEnabled(false);

    for (int row = 0; row < MazeLayout::Size; ++row)
    {
        for (int column = 0; column < MazeLayout::Size; ++column)
        {
            const char cell = MazeLayout::Cells[row][column];
            const Vector3 location{ MazeLayout::X(column), 0, MazeLayout::Z(row) };
            if (cell == '#')
            {
                auto object = mGameWorld.CreateGameObject("Wall_" + std::to_string(row) + "_" + std::to_string(column),
                    L"../../Assets/Templates/Objects/maze_wall.json");
                object->GetComponent<TransformComponent>()->position = {location.x, 0.55f, location.z};
                object->Initialize();
            }
            else if (cell == '1' || cell == '2')
            {
                mSpawns[cell - '1'] = {location.x, MazeLayout::PlayerRadius + 0.02f, location.z};
            }
            else if (cell == 'E')
            {
                mExit = location;
                auto object = mGameWorld.CreateGameObject("Exit",
                    L"../../Assets/Templates/Objects/maze_exit.json");
                object->GetComponent<TransformComponent>()->position = {location.x, 0.03f, location.z};
                object->Initialize();
            }
        }
    }

    for (size_t i = 0; i < mPlayers.size(); ++i)
    {
        auto object = mGameWorld.CreateGameObject("Player" + std::to_string(i + 1),
            L"../../Assets/Templates/Objects/player_udp_maze.json");
        mPlayers[i] = object->GetComponent<TransformComponent>();
        mPlayers[i]->position = mSpawns[i];
        ReadJson(object->GetComponent<MeshComponent>(), i == 0
            ? R"({"Material":{"Ambient":[1.0,0.58,0.58,1.0],"Diffuse":[1.0,0.58,0.58,1.0]}})"
            : R"({"Material":{"Ambient":[0.52,0.72,1.0,1.0],"Diffuse":[0.52,0.72,1.0,1.0]}})");
        if (mNetwork.IsServer())
        {
            mBodies[i] = object->AddComponent<RigidBodyComponent>();
            ReadJson(mBodies[i], R"({"Mass":1.0,"ColliderData":{"Shape":"Sphere","Radius":0.45}})");
        }
        object->Initialize();
    }
}

void GameState::Terminate()
{
    mNetwork.Terminate();
    if (mSceneReady) mGameWorld.Terminate();
    mSceneReady = false;
    mPlayers.fill(nullptr);
    mBodies.fill(nullptr);
}

uint32_t GameState::ReadButtons() const
{
    if (GetForegroundWindow() != mWindow || ImGui::GetIO().WantCaptureKeyboard) return 0;
    auto input = InputSystem::Get();
    auto down = [input](KeyCode key)
    {
        return input->IsKeyDown(key) && (GetAsyncKeyState(static_cast<int>(key)) & 0x8000) != 0;
    };
    uint32_t buttons = 0;
    if (down(KeyCode::W)) buttons |= NetworkSession::Forward;
    if (down(KeyCode::S)) buttons |= NetworkSession::Back;
    if (down(KeyCode::A)) buttons |= NetworkSession::Left;
    if (down(KeyCode::D)) buttons |= NetworkSession::Right;
    return buttons;
}

void GameState::MovePlayer(size_t index, uint32_t buttons)
{
    Vector3 direction{
        float((buttons & NetworkSession::Right) != 0) - float((buttons & NetworkSession::Left) != 0),
        0,
        float((buttons & NetworkSession::Forward) != 0) - float((buttons & NetworkSession::Back) != 0)
    };
    const float length = std::sqrt(direction.x * direction.x + direction.z * direction.z);
    if (length > 0) direction *= MazeLayout::MoveSpeed / length;
    direction.y = mBodies[index]->GetVelocity().y;
    mBodies[index]->SetVelocity(direction);
    mBodies[index]->SetAngularVelocity(Vector3::Zero);
    const auto& p = mPlayers[index]->position;
    if (p.y < -3 || std::abs(p.x) > 15 || std::abs(p.z) > 15)
    {
        mBodies[index]->SetPosition(mSpawns[index]);
        mBodies[index]->SetVelocity(Vector3::Zero);
    }
}

NetworkSession::Positions GameState::ReadPositions() const
{
    NetworkSession::Positions positions;
    for (size_t i = 0; i < mPlayers.size(); ++i)
    {
        const auto& p = mPlayers[i]->position;
        positions[i] = {p.x, p.y, p.z};
    }
    return positions;
}

void GameState::ResetPlayers()
{
    for (size_t i = 0; i < mPlayers.size(); ++i)
    {
        mBodies[i]->SetPosition(mSpawns[i]);
        mBodies[i]->SetVelocity(Vector3::Zero);
        mBodies[i]->SetAngularVelocity(Vector3::Zero);
    }
    mNetwork.SendSnapshot(ReadPositions());
}

void GameState::Update(float deltaTime)
{
    if (!mSceneReady) return;
    mNetwork.Receive(deltaTime);
    const uint32_t buttons = ReadButtons();
    if (mNetwork.IsServer())
    {
        MovePlayer(0, buttons);
        MovePlayer(1, mNetwork.RemoteButtons());
    }
    else if (mNetwork.HasSnapshot() && mNetwork.IsConnected())
    {
        const auto& positions = mNetwork.GetPositions();
        const float blend = 1.0f - std::exp(-25.0f * deltaTime);
        for (size_t i = 0; i < mPlayers.size(); ++i)
        {
            const Vector3 target{positions[i].x, positions[i].y, positions[i].z};
            const auto difference = target - mPlayers[i]->position;
            // Snap respawns; interpolate normal movement between 30 Hz snapshots.
            if (!mReceivedFirstSnapshot || MagnitudeSqr(difference) > 16.0f)
                mPlayers[i]->position = target;
            else
                mPlayers[i]->position += difference * blend;
        }
        mReceivedFirstSnapshot = true;
    }
    mSendTime += deltaTime;
    if (mSendTime >= NetworkSession::SendInterval)
    {
        mSendTime = std::fmod(mSendTime, NetworkSession::SendInterval);
        if (mNetwork.IsServer()) mNetwork.SendSnapshot(ReadPositions());
        else mNetwork.SendInput(buttons);
    }
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    if (mSceneReady) mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::SetNextWindowPos({12, 12}, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.93f);
    ImGui::Begin("UDP Maze", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
    if (!mSceneReady)
    {
        ImGui::TextWrapped("Startup failed: %s", mNetwork.Error().c_str());
        ImGui::TextUnformatted("Close this window and restart. Port: 127.0.0.1:27026");
        ImGui::End();
        return;
    }
    const bool server = mNetwork.IsServer();
    ImGui::TextUnformatted(server ? "SERVER / YOU ARE P1" : "CLIENT / YOU ARE P2");
    ImGui::TextColored(mNetwork.IsConnected() ? ImVec4(0.4f, 1, 0.6f, 1) : ImVec4(1, 0.8f, 0.3f, 1),
        "%s", mNetwork.IsConnected() ? "Connected - localhost UDP" : (server
            ? "Waiting for client - run the program again" : "Waiting for server - retrying automatically"));
    ImGui::TextUnformatted("WASD: move your ball | Green tile: exit | Esc: quit");
    ImGui::TextUnformatted("Click a game window to control its player.");
    ImGui::Separator();
    for (size_t i = 0; i < mPlayers.size(); ++i)
    {
        const auto& p = mPlayers[i]->position;
        const bool atExit = std::abs(p.x - mExit.x) < 0.8f && std::abs(p.z - mExit.z) < 0.8f;
        ImGui::TextColored(i == 0 ? ImVec4(1, 0.45f, 0.45f, 1) : ImVec4(0.4f, 0.7f, 1, 1),
            "P%d / Mario ball   x %.2f  z %.2f%s", static_cast<int>(i + 1), p.x, p.z,
            atExit ? "  EXIT REACHED!" : "");
    }
    if (server && ImGui::Button("Reset both players")) ResetPlayers();
    ImGui::Text("UDP snapshots: 30 Hz | Received: %u", mNetwork.ReceivedCount());
    if (!mNetwork.Error().empty()) ImGui::TextWrapped("%s", mNetwork.Error().c_str());
    ImGui::End();
}