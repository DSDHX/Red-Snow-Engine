#pragma once

#include <RedSnowEngine/Inc/RedSnowEngine.h>

class GameState : public RedSnowEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    void UpdateCamera(float deltaTime);

    void OnSpacePressedEvent(const RedSnowEngine::Core::Event& e);

    RedSnowEngine::Graphics::Camera mCamera;
    RedSnowEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
    RedSnowEngine::Physics::ParticleSystem mParticleSystem;
    RedSnowEngine::Physics::ParticleSystem mFireworkParticles;

    RedSnowEngine::Core::EventListenerId mSpacePressedListenerId = 0;
    RedSnowEngine::Core::EventListenerId mEnterPressedListenerId = 0;

    RedSnowEngine::Graphics::Animation mFireworkAnimation;
    float mFireworkAnimationTime = 0.0f;

    RedSnowEngine::Audio::SoundId mLaunchSoundId = 0;
    RedSnowEngine::Audio::SoundId mExplosionSoundId = 0;
};