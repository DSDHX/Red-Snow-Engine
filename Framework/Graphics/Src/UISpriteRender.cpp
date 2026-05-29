#include "Precompiled.h"
#include "UISpriteRender.h"
#include "UISprite.h"

#include "GraphicsSystem.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

namespace
{
    std::unique_ptr<UISpriteRender> sUISpriteRender;
}

void UISpriteRender::StaticInitialize()
{
    ASSERT(sUISpriteRender == nullptr, "UISpriteRender: is already initialized");
    sUISpriteRender = std::make_unique<UISpriteRender>();
    sUISpriteRender->Initialize();
}

void UISpriteRender::StaticTerminate()
{
    if (sUISpriteRender != nullptr)
    {
        sUISpriteRender->Terminate();
        sUISpriteRender.reset();
    }
}

UISpriteRender* UISpriteRender::Get()
{
    ASSERT(sUISpriteRender != nullptr, "UISpriteRender: is not initialized");
    return sUISpriteRender.get();
}

UISpriteRender::~UISpriteRender()
{
    ASSERT(mCommonStates == nullptr && mSpriteBatch == nullptr, "UISpriteRender: terminate must be called");
}

void UISpriteRender::Initialize()
{
    GraphicsSystem* gs = GraphicsSystem::Get();
    mCommonStates = new DirectX::CommonStates(gs->GetDevice());
    mSpriteBatch = new DirectX::SpriteBatch(gs->GetContext());
}

void UISpriteRender::Terminate()
{
    if (mCommonStates != nullptr)
    {
        delete mCommonStates;
        mCommonStates = nullptr;
    }
    if (mSpriteBatch != nullptr)
    {
        mSpriteBatch = nullptr;
    }
}

void UISpriteRender::BeginRender()
{
    mSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, mCommonStates->NonPremultiplied());
}

void UISpriteRender::EndRender()
{
    mSpriteBatch->End();

    auto blendState = mCommonStates->Opaque();
    auto depthStencilState = mCommonStates->DepthDefault();
    auto rasterizerState = mCommonStates->CullCounterClockwise();

    auto context = GraphicsSystem::Get()->GetContext();
    context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(depthStencilState, 0);
    context->RSSetState(rasterizerState);
}

void UISpriteRender::Render(const UISprite& uiSprite)
{
    const Texture* texture = TextureManager::Get()->GetTexture(uiSprite.mTextureId);
    if (texture != nullptr)
    {
        mSpriteBatch->Draw(
            (ID3D11ShaderResourceView*)texture->GetRawData(),
            uiSprite.mPosition,
            &uiSprite.mRect,
            uiSprite.mColor,
            uiSprite.mRotation,
            uiSprite.mOrigin,
            uiSprite.mScale,
            uiSprite.mFlip
        );
    }
}