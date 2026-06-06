#include "Precompiled.h"
#include "RenderObjectComponent.h"
#include "SaveUtil.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "RenderService.h"

using namespace RedSnowEngine;

void RenderObjectComponent::Initialize()
{
    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    if (renderService != nullptr)
    {
        renderService->Register(this);
    }
}

void RenderObjectComponent::Terminate()
{
    RenderService* renderService = GetOwner().GetWorld().GetService<RenderService>();
    if (renderService != nullptr)
    {
        renderService->Unregister(this);
    }
}

void RenderObjectComponent::Deserialize(const rapidjson::Value& value)
{
    SaveUtil::ReadBool("CastShaow", mCastShadow, value);
}

void RenderObjectComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value,
    const rapidjson::Value& originalValue)
{
    if (originalValue.HasMember("CastShadow") && originalValue["CastShadow"].GetBool() != mCastShadow)
    {
        SaveUtil::WriteBool("CastShadow", mCastShadow, doc, value);
    }
}

bool RenderObjectComponent::CanCastShadow() const
{
    return mCastShadow;
}