#include "GameState.h"
#include "GameEvents.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;
using namespace RedSnowEngine::Input;
using namespace RedSnowEngine::Physics;
using namespace RedSnowEngine::Audio;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 3.0f, -8.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    ModelManager* mm = ModelManager::Get();

    mCharacter1.Initialize("Character01/Character01.model");
    mCharacter1.animator = &mAnimator1;
    mCharacter1.transform.position = { -1.5f, 0.0f, 0.0f };
    mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/HipHopDancing.animset");
    mAnimator1.Initialize(mCharacter1.modelId);
    mAnimator1.PlayAnimation(0, true);

    mCharacter2.Initialize("Character02/Character02.model");
    mCharacter2.animator = &mAnimator2;
    mCharacter2.transform.position = { 1.5f, 0.0f, 0.0f };
    mm->AddAnimation(mCharacter2.modelId, L"../../Assets/Models/Character02/Animations/WaveHipHopDance.animset");
    mAnimator2.Initialize(mCharacter2.modelId);
    mAnimator2.PlayAnimation(0, true);

    mParticleSystemEffect.Initialize();
    mParticleSystemEffect.SetCamera(mCamera);

    ParticleSystemInfo cornerInfo;
    cornerInfo.textureId = TextureManager::Get()->LoadTexture("Images/bullet1.png");
    cornerInfo.maxParticles = 500;
    cornerInfo.particlesPerEmit = { 2, 5 };
    cornerInfo.delay = 0.0f;
    cornerInfo.lifeTime = FLT_MAX;
    cornerInfo.timeBetweenEmits = { 0.05f, 0.1f };
    cornerInfo.spawnAngle = { -15.0f, 15.0f };
    cornerInfo.spawnSpeed = { 5.0f, 10.0f };
    cornerInfo.particleLifeTime = { 0.5f, 1.5f };
    cornerInfo.spawnDirection = Math::Vector3::YAxis;
    cornerInfo.startScale = { Math::Vector3::One * 0.5, Math::Vector3::One * 0.8f };
    cornerInfo.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
    cornerInfo.startColor = { Graphics::Colors::Yellow, Graphics::Colors::Orange };
    cornerInfo.endColor = { Graphics::Colors::Red, Graphics::Colors::DarkRed };

    Math::Vector3 cornerPos[4] = {
        { -4.0f, 0.0f, -4.0f },
        {  4.0f, 0.0f, -4.0f },
        { -4.0f, 0.0f,  4.0f },
        {  4.0f, 0.0f,  4.0f } 
    };

    for (int i = 0; i < 4; ++i)
    {
        mCornerParticles[i].Initialize(cornerInfo);
        mCornerParticles[i].SetPosition(cornerPos[i]);
    }

    ParticleSystemInfo envelopeInfo;
    envelopeInfo.textureId = TextureManager::Get()->LoadTexture("Images/red_pocket.png");
    envelopeInfo.maxParticles = 1000;
    envelopeInfo.particlesPerEmit = { 30, 50 };
    envelopeInfo.delay = 0.0f;
    envelopeInfo.lifeTime = 0.0f;
    envelopeInfo.spawnAngle = { -60.0f, 60.0f };
    envelopeInfo.spawnSpeed = { 2.0f, 5.0f };
    envelopeInfo.particleLifeTime = { 2.0f, 4.0f };
    envelopeInfo.spawnDirection = -Math::Vector3::YAxis;
    envelopeInfo.startScale = { Math::Vector3::One * 0.8f, Math::Vector3::One * 1.2f };
    envelopeInfo.endScale = { Math::Vector3::One * 0.8f, Math::Vector3::One * 1.2f };
    envelopeInfo.startColor = { Graphics::Colors::White, Graphics::Colors::White };
    envelopeInfo.endColor = { Graphics::Colors::White, Graphics::Colors::White };
    mRedEnvelopeParticles.Initialize(envelopeInfo);

    mEmitterAnimation = AnimationBuilder()
        .AddPositionKey({ -5.0f, 8.0f, 0.0f }, 0.0f)
        .AddPositionKey({ 5.0f, 8.0f, 0.0f }, 2.0f)
        .AddPositionKey({ -5.0f, 8.0f, 0.0f }, 4.0f)
        .Build();

    SoundEffectManager* sm = SoundEffectManager::Get();
    mBackgroundMusicId = sm->Load("SKAIISYOURGOD.wav");
    sm->Play(mBackgroundMusicId, true);

    EventManager* em = EventManager::Get();
    mSpacePressedListenerId = em->AddListener((EventTypeId)GameEventType::PressSpace,
        std::bind(&GameState::OnSpacePressedEvent, this, std::placeholders::_1));

    mEnterPressedListenerId = em->AddListener((EventTypeId)GameEventType::PressEnter,
        std::bind(&GameState::OnEnterPressedEvent, this, std::placeholders::_1));
}

void GameState::Terminate()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    sm->Stop(mBackgroundMusicId);

    EventManager* em = EventManager::Get();
    em->RemoveListener((EventTypeId)GameEventType::PressSpace, mSpacePressedListenerId);
    em->RemoveListener((EventTypeId)GameEventType::PressEnter, mEnterPressedListenerId);

    mRedEnvelopeParticles.Terminate();
    for (int i = 0; i < 4; ++i)
    {
        mCornerParticles[i].Terminate();
    }
    mParticleSystemEffect.Terminate();

    mCharacter1.Terminate();
    mCharacter2.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    mTimer += deltaTime;

    UpdateCamera(deltaTime);

    mAnimator1.Update(deltaTime * mAnimationSpeed);
    mAnimator2.Update(deltaTime * mAnimationSpeed);

    mRedEnvelopeParticles.Update(deltaTime);
    for (int i = 0; i < 4; ++i)
    {
        mCornerParticles[i].Update(deltaTime);
    }

    float prevTime = mEmitterAnimationTime;
    mEmitterAnimationTime += deltaTime;
    if (mEmitterAnimation.GetDuration() > 0)
    {
        mEmitterAnimation.PlayEvents(prevTime, mEmitterAnimationTime);
        while (mEmitterAnimationTime >= mEmitterAnimation.GetDuration())
        {
            mEmitterAnimationTime -= mEmitterAnimation.GetDuration();
        }
    }
}

void GameState::Render()
{
    SimpleDraw::AddGroundPlane(20.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter1);
        mStandardEffect.Render(mCharacter2);
    mStandardEffect.End();

    Transform emitterTransform = mEmitterAnimation.GetTransform(mEmitterAnimationTime);
    mRedEnvelopeParticles.SetPosition(emitterTransform.position);

    mParticleSystemEffect.Begin();
        mRedEnvelopeParticles.Render(mParticleSystemEffect);
        for (int i = 0; i < 4; ++i)
        {
            mCornerParticles[i].Render(mParticleSystemEffect);
        }
    mParticleSystemEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Animation Timer: %.2f", mTimer);

    ImGui::Separator();

    ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
    ImGui::DragFloat("AnimSpeed", &mAnimationSpeed, 0.1f, 0.0f, 10.0f);

    ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }

    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        PressSpaceEvent event;
        EventManager::Broadcast(event);
    }
    if (input->IsKeyPressed(KeyCode::ENTER))
    {
        PressEnterEvent event;
        EventManager::Broadcast(event);
    }
}

void GameState::OnSpacePressedEvent(const RedSnowEngine::Core::Event& e)
{
    LOG("SPACE WAS PRESSED - Dropping Red Envelopes!");
    mRedEnvelopeParticles.SpawnParticles();
}

void GameState::OnEnterPressedEvent(const RedSnowEngine::Core::Event& e)
{
    LOG("ENTER WAS PRESSED - Dropping Red Envelopes!");
    mRedEnvelopeParticles.SpawnParticles();
}