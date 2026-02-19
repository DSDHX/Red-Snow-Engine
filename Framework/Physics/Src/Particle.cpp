#include "Precompiled.h"
#include "Particle.h"

#include "PhysicsWorld.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Physics;

void Patricle::Initialize()
{
    mLifeTime = 0.0f;
    mCollisionShape.InitializeEmpty();
    mRigidBody.Initialize(mTransform, mCollisionShape, 1.0f, false);
    mRigidBody.SetCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void Patricle::Terminate()
{
    mRigidBody.Terminate();
    mCollisionShape.Terminate();
}

void Patricle::Activate(const ParticleInfo& info)
{
    mInfo = info;
    mLifeTime = info.lifeTime;
    if (info.lifeTime > 0)
    {
        mRigidBody.Activate();
    }
    mRigidBody.SetPosition(info.position);
    mRigidBody.SetVelocity(info.velocity);
}

void Patricle::Update(float deltaTime)
{
    if (IsActive())
    {
        mLifeTime -= deltaTime;
        if (!IsActive())
        {
            mRigidBody.Deactivate();
        }
        else
        {
            const float t = 1.0f - Math::Clamp(mLifeTime / mInfo.lifeTime, 0.0f, 1.0f);
            mTransform.scale = Math::Lerp(mInfo.startScale, mInfo.endScale, t);
        }
    }
}

bool Patricle::IsActive() const
{
    return mLifeTime > 0.0f;
}

const Graphics::Transform& Patricle::GetTransform() const
{
    return mTransform;
}

Graphics::Color Patricle::GetColor() const
{
    const float t = 1.0f - Math::Clamp(mLifeTime / mInfo.lifeTime, 0.0f, 1.0f);
    return Math::Lerp(mInfo.startColor, mInfo.endColor, t);
}

const Math::Vector3& Patricle::GetPosition() const
{
    return mTransform.position;
}