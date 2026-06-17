#include "TimeRewindComponent.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;
using namespace RedSnowEngine::Math;

namespace
{
    KeyCode ToKeyCode(const std::string& keyName, KeyCode fallback)
    {
        if (keyName == "R" || keyName == "r")
        {
            return KeyCode::R;
        }
        if (keyName == "E" || keyName == "e")
        {
            return KeyCode::E;
        }
        if (keyName == "Q" || keyName == "q")
        {
            return KeyCode::Q;
        }
        if (keyName == "F" || keyName == "f")
        {
            return KeyCode::F;
        }
        if (keyName == "SPACE" || keyName == "Space" || keyName == "space")
        {
            return KeyCode::SPACE;
        }
        if (keyName == "LSHIFT" || keyName == "LShift" || keyName == "Shift")
        {
            return KeyCode::LSHIFT;
        }
        return fallback;
    }
}

void TimeRewindComponent::Initialize()
{
    mTransformComponent = GetOwner().GetComponent<TransformComponent>();
    mRigidBodyComponent = GetOwner().GetComponent<RigidBodyComponent>();
    mPlayerControllerComponent = GetOwner().GetComponent<PlayerControllerComponent>();

    ASSERT(mTransformComponent != nullptr, "TimeRewindComponent: owner requires a TransformComponent");
    RecordFrame();
}

void TimeRewindComponent::Terminate()
{
    mHistory.clear();
    mTransformComponent = nullptr;
    mRigidBodyComponent = nullptr;
    mPlayerControllerComponent = nullptr;
    mIsRewinding = false;
}

void TimeRewindComponent::Update(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const bool wantsRewind = input->IsKeyDown(mRewindKey);
    const bool canRewind = wantsRewind && !mHistory.empty() && mEnergy > 0.0f;

    if (canRewind)
    {
        if (!mIsRewinding)
        {
            BeginRewind();
        }
        Rewind(deltaTime);
        return;
    }

    if (mIsRewinding)
    {
        EndRewind();
    }

    AgeHistory(deltaTime);
    mRecordTimer += deltaTime;
    if (mRecordTimer >= mRecordInterval)
    {
        RecordFrame();
        mRecordTimer -= mRecordInterval;
    }

    mEnergy = Clamp(mEnergy + (mEnergyRecoverPerSecond * deltaTime), 0.0f, mMaxEnergy);
}

void TimeRewindComponent::DebugUI()
{
    if (ImGui::CollapsingHeader("TimeRewindComponent"))
    {
        ImGui::Text("Rewinding: %s", mIsRewinding ? "true" : "false");
        ImGui::Text("History Frames: %d", static_cast<int>(mHistory.size()));
        ImGui::Text("Energy: %.1f / %.1f", mEnergy, mMaxEnergy);

        ImGui::DragFloat("RecordDuration", &mRecordDuration, 0.1f, 0.1f, 30.0f);
        ImGui::DragFloat("RewindSpeed", &mRewindSpeed, 0.1f, 0.1f, 20.0f);
        ImGui::DragFloat("RecordInterval", &mRecordInterval, 0.001f, 0.001f, 1.0f);
        ImGui::DragFloat("MaxEnergy", &mMaxEnergy, 1.0f, 1.0f, 1000.0f);
        ImGui::DragFloat("EnergyDrainPerSecond", &mEnergyDrainPerSecond, 1.0f, 0.0f, 1000.0f);
        ImGui::DragFloat("EnergyRecoverPerSecond", &mEnergyRecoverPerSecond, 1.0f, 0.0f, 1000.0f);
        ImGui::Checkbox("RestoreVelocity", &mRestoreVelocity);
        ImGui::Checkbox("DisableControlDuringRewind", &mDisableControlDuringRewind);
        ImGui::Checkbox("DebugDrawPath", &mDebugDrawPath);
        ImGui::ColorEdit4("DebugColor", &mDebugColor.r);
        if (ImGui::Button("Reset Energy"))
        {
            mEnergy = mMaxEnergy;
        }

        NormalizeSettings();
    }

    if (mDebugDrawPath)
    {
        AddDebugDrawPath();
    }
}

void TimeRewindComponent::Deserialize(const rapidjson::Value& value)
{
    std::string keyName;
    if (SaveUtil::ReadString("RewindKey", keyName, value))
    {
        mRewindKey = ToKeyCode(keyName, mRewindKey);
    }

    SaveUtil::ReadFloat("RecordDuration", mRecordDuration, value);
    SaveUtil::ReadFloat("RewindSpeed", mRewindSpeed, value);
    SaveUtil::ReadFloat("RecordInterval", mRecordInterval, value);
    SaveUtil::ReadBool("RestoreVelocity", mRestoreVelocity, value);
    SaveUtil::ReadBool("DisableControlDuringRewind", mDisableControlDuringRewind, value);
    SaveUtil::ReadFloat("MaxEnergy", mMaxEnergy, value);
    SaveUtil::ReadFloat("EnergyDrainPerSecond", mEnergyDrainPerSecond, value);
    SaveUtil::ReadFloat("EnergyRecoverPerSecond", mEnergyRecoverPerSecond, value);
    SaveUtil::ReadBool("DebugDrawPath", mDebugDrawPath, value);
    SaveUtil::ReadColor("DebugColor", mDebugColor, value);

    mEnergy = mMaxEnergy;
    NormalizeSettings();
}

void TimeRewindComponent::BeginRewind()
{
    mIsRewinding = true;
    mRewindTimer = 0.0f;

    if (mDisableControlDuringRewind && mRigidBodyComponent != nullptr && !mRestoreVelocity)
    {
        mRigidBodyComponent->SetVelocity(Vector3::Zero);
    }
}

void TimeRewindComponent::EndRewind()
{
    mIsRewinding = false;
    mRewindTimer = 0.0f;

    if (mDisableControlDuringRewind && mRigidBodyComponent != nullptr && !mRestoreVelocity)
    {
        mRigidBodyComponent->SetVelocity(Vector3::Zero);
    }
}

void TimeRewindComponent::Rewind(float deltaTime)
{
    mEnergy = Clamp(mEnergy - (mEnergyDrainPerSecond * deltaTime), 0.0f, mMaxEnergy);
    mRewindTimer += deltaTime * mRewindSpeed;

    bool appliedFrame = false;
    while (mRewindTimer >= mRecordInterval && !mHistory.empty())
    {
        ApplyFrame(mHistory.back());
        mHistory.pop_back();
        mRewindTimer -= mRecordInterval;
        appliedFrame = true;
    }

    if (!appliedFrame && !mHistory.empty())
    {
        ApplyFrame(mHistory.back());
    }

    if (mHistory.empty() || mEnergy <= 0.0f)
    {
        EndRewind();
    }
}

void TimeRewindComponent::RecordFrame()
{
    if (mTransformComponent == nullptr)
    {
        return;
    }

    RewindFrame frame;
    frame.position = mTransformComponent->position;
    frame.rotation = mTransformComponent->rotation;
    if (mRigidBodyComponent != nullptr)
    {
        frame.velocity = mRigidBodyComponent->GetVelocity();
    }

    mHistory.push_back(frame);
    TrimHistory();
}

void TimeRewindComponent::AgeHistory(float deltaTime)
{
    for (RewindFrame& frame : mHistory)
    {
        frame.time += deltaTime;
    }
    TrimHistory();
}

void TimeRewindComponent::TrimHistory()
{
    while (!mHistory.empty() && mHistory.front().time > mRecordDuration)
    {
        mHistory.pop_front();
    }
}

void TimeRewindComponent::ApplyFrame(const RewindFrame& frame)
{
    if (mTransformComponent == nullptr)
    {
        return;
    }

    if (mRigidBodyComponent != nullptr)
    {
        mRigidBodyComponent->SetPosition(frame.position);
        if (mRestoreVelocity)
        {
            mRigidBodyComponent->SetVelocity(frame.velocity);
        }
        else if (mDisableControlDuringRewind)
        {
            mRigidBodyComponent->SetVelocity(Vector3::Zero);
        }
    }

    mTransformComponent->position = frame.position;
    mTransformComponent->rotation = frame.rotation;
}

void TimeRewindComponent::AddDebugDrawPath() const
{
    if (mHistory.size() < 2)
    {
        return;
    }

    auto iter = mHistory.begin();
    Vector3 previous = iter->position;
    ++iter;

    int frameIndex = 1;
    for (; iter != mHistory.end(); ++iter, ++frameIndex)
    {
        const Vector3 current = iter->position;
        SimpleDraw::AddLine(previous, current, mDebugColor);
        if ((frameIndex % 10) == 0)
        {
            SimpleDraw::AddSphere(8, 6, 0.08f, mDebugColor, current);
        }
        previous = current;
    }

    if (mTransformComponent != nullptr)
    {
        SimpleDraw::AddLine(mTransformComponent->position, mHistory.back().position, Colors::Yellow);
    }
}

void TimeRewindComponent::NormalizeSettings()
{
    mRecordInterval = Max(0.001f, mRecordInterval);
    mRecordDuration = Max(mRecordInterval, mRecordDuration);
    mRewindSpeed = Max(0.1f, mRewindSpeed);
    mMaxEnergy = Max(1.0f, mMaxEnergy);
    mEnergyDrainPerSecond = Max(0.0f, mEnergyDrainPerSecond);
    mEnergyRecoverPerSecond = Max(0.0f, mEnergyRecoverPerSecond);
    mEnergy = Clamp(mEnergy, 0.0f, mMaxEnergy);
}