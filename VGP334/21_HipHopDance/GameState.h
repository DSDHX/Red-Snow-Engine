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
    void OnEnterPressedEvent(const RedSnowEngine::Core::Event& e);

    RedSnowEngine::Graphics::Camera mCamera;
    RedSnowEngine::Graphics::DirectionalLight mDirectionalLight;
    RedSnowEngine::Graphics::StandardEffecr mStandardEffect;

    RedSnowEngine::Graphics::RenderGroup mCharacter1;
    RedSnowEngine::Graphics::Animator mAnimator1;

    RedSnowEngine::Graphics::RenderGroup mCharacter2;
    RedSnowEngine::Graphics::Animator mAnimator2;

    RedSnowEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;
    RedSnowEngine::Physics::ParticleSystem mCornerParticles[4];
    RedSnowEngine::Physics::ParticleSystem mRedEnvelopeParticles;

    RedSnowEngine::Graphics::Animation mEmitterAnimation;
    float mEmitterAnimationTime = 0.0f;

    float mTimer = 0.0f;
    float mAnimationSpeed = 1.0f;
    bool mDrawSkeleton = false;

    RedSnowEngine::Audio::SoundId mBackgroundMusicId;

    RedSnowEngine::Core::EventListenerId mSpacePressedListenerId;
    RedSnowEngine::Core::EventListenerId mEnterPressedListenerId;
};