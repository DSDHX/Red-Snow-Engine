#pragma once

#include "CustomTypeIds.h"
#include <RedSnowEngine/Inc/RedSnowEngine.h>
#include <deque>

class TimeRewindComponent final : public RedSnowEngine::Component
{
public:
    SET_TYPE_ID(CustomComponentId::TimeRewind);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void DebugUI() override;
    void Deserialize(const rapidjson::Value& value) override;

private:
    struct RewindFrame
    {
        RedSnowEngine::Math::Vector3 position = RedSnowEngine::Math::Vector3::Zero;
        RedSnowEngine::Math::Quaternion rotation = RedSnowEngine::Math::Quaternion::Identity;
        RedSnowEngine::Math::Vector3 velocity = RedSnowEngine::Math::Vector3::Zero;
        float time = 0.0f;
    };

    void BeginRewind();
    void EndRewind();
    void Rewind(float deltaTime);
    void RecordFrame();
    void AgeHistory(float deltaTime);
    void TrimHistory();
    void ApplyFrame(const RewindFrame& frame);
    void AddDebugDrawPath() const;
    void NormalizeSettings();

    std::deque<RewindFrame> mHistory;

    RedSnowEngine::TransformComponent* mTransformComponent = nullptr;
    RedSnowEngine::RigidBodyComponent* mRigidBodyComponent = nullptr;
    RedSnowEngine::PlayerControllerComponent* mPlayerControllerComponent = nullptr;

    RedSnowEngine::Input::KeyCode mRewindKey = RedSnowEngine::Input::KeyCode::R;

    float mRecordDuration = 3.0f;
    float mRewindSpeed = 2.5f;
    float mRecordInterval = 0.02f;
    float mRecordTimer = 0.0f;
    float mRewindTimer = 0.0f;

    float mMaxEnergy = 100.0f;
    float mEnergy = 100.0f;
    float mEnergyDrainPerSecond = 40.0f;
    float mEnergyRecoverPerSecond = 20.0f;

    bool mRestoreVelocity = true;
    bool mDisableControlDuringRewind = true;
    bool mDebugDrawPath = true;
    bool mIsRewinding = false;

    RedSnowEngine::Graphics::Color mDebugColor = RedSnowEngine::Graphics::Colors::Cyan;
};