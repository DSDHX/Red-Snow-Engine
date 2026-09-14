#pragma once

#include <RedSnowEngine/Inc/RedSnowEngine.h>
#include "NetworkSession.h"

class GameState : public RedSnowEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    void CreateScene();
    void ResetPlayers();
    uint32_t ReadButtons() const;
    void MovePlayer(size_t index, uint32_t buttons);
    NetworkSession::Positions ReadPositions() const;
    RedSnowEngine::GameWorld mGameWorld;
    NetworkSession mNetwork;
    std::array<RedSnowEngine::TransformComponent*, 2> mPlayers{};
    std::array<RedSnowEngine::RigidBodyComponent*, 2> mBodies{};
    std::array<RedSnowEngine::Math::Vector3, 2> mSpawns{};
    RedSnowEngine::Math::Vector3 mExit;
    HWND mWindow = nullptr;
    float mSendTime = 0;
    bool mSceneReady = false;
    bool mReceivedFirstSnapshot = false;
};