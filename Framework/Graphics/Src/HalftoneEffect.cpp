#include "Precompiled.h"
#include "HalftoneEffect.h"

#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

void HalftoneEffect::Initialize(const std::filesystem::path& fileName)
{
    mVertexShader.Initialize<Vertex>(fileName);
    mPixelShader.Initialize(fileName);

    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();

    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

    mShadowSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);

    auto gs = GraphicsSystem::Get();
    mHalftoneData.viewportSize.x = (float)gs->GetBackBufferWidth();
    mHalftoneData.viewportSize.y = (float)gs->GetBackBufferHeight();
    mHalftoneData.dotDensity = 890.0f;
    mHalftoneData.power = 1.0f;
    mHalftoneData.shadowColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    mHalftoneData.lightColor = Colors::White;
}

void HalftoneEffect::Terminate()
{
    mShadowSampler.Terminate();
    mSampler.Terminate();
    mSettingsBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void HalftoneEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);
    mLightBuffer.BindPS(1);
    mMaterialBuffer.BindPS(2);
    mSettingsBuffer.BindPS(3);

    mSampler.BindPS(0);
    mShadowSampler.BindPS(1);

    if (mShadowMap)
    {
        mShadowMap->BindPS(1);
    }
}

void HalftoneEffect::End()
{
    if (mShadowMap)
    {
        Texture::UnbindPS(1);
    }
}

void HalftoneEffect::Render(const RenderObject& renderObject)
{
    Math::Matrix4 world = renderObject.transform.GetMatrix4();
    Math::Matrix4 view = mCamera->GetViewMatrix();
    Math::Matrix4 proj = mCamera->GetProjectionMatrix();

    Math::Matrix4 lightView = mLightCamera->GetViewMatrix();
    Math::Matrix4 lightProj = mLightCamera->GetProjectionMatrix();

    TransformData data;
    data.world = Math::Transpose(world);
    data.wvp = Math::Transpose(world * view * proj);
    data.lightViewProj = Math::Transpose(world * lightView * lightProj);
    data.viewPosition = mCamera->GetPosition();
    data.padding = 0.0f;

    mTransformBuffer.Update(data);

    if (mDirectionalLight)
    {
        mLightBuffer.Update(*mDirectionalLight);
    }
    mMaterialBuffer.Update(renderObject.material);

    auto gs = GraphicsSystem::Get();
    mHalftoneData.viewportSize = { (float)gs->GetBackBufferWidth(), (float)gs->GetBackBufferHeight() };
    mSettingsBuffer.Update(mHalftoneData);

    auto tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapId, 0);

    renderObject.meshBuffer.Render();
}

void HalftoneEffect::Render(const RenderGroup& renderGroup)
{
    Math::Matrix4 world = renderGroup.transform.GetMatrix4();
    Math::Matrix4 view = mCamera->GetViewMatrix();
    Math::Matrix4 proj = mCamera->GetProjectionMatrix();
    Math::Matrix4 lightView = mLightCamera->GetViewMatrix();
    Math::Matrix4 lightProj = mLightCamera->GetProjectionMatrix();

    TransformData data;
    data.world = Math::Transpose(world);
    data.wvp = Math::Transpose(world * view * proj);
    data.lightViewProj = Math::Transpose(world * lightView * lightProj);
    data.viewPosition = mCamera->GetPosition();
    data.padding = 0.0f;
    mTransformBuffer.Update(data);

    if (mDirectionalLight)
    {
        mLightBuffer.Update(*mDirectionalLight);
    }

    auto gs = GraphicsSystem::Get();
    mHalftoneData.viewportSize = { (float)gs->GetBackBufferWidth(), (float)gs->GetBackBufferHeight() };
    mSettingsBuffer.Update(mHalftoneData);

    for (const auto& obj : renderGroup.renderObjects)
    {
        mMaterialBuffer.Update(obj.material);
        auto tm = TextureManager::Get();
        tm->BindPS(obj.diffuseMapId, 0);
        obj.meshBuffer.Render();
    }
}

void HalftoneEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("Halftone Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Dot Density", &mHalftoneData.dotDensity, 1.0f, 10.0f, 2000.0f);
        ImGui::DragFloat("Power", &mHalftoneData.power, 0.1f, 0.1f, 5.0f);
        ImGui::ColorEdit4("Light Color", &mHalftoneData.lightColor.r);
        ImGui::ColorEdit4("Shadow Color", &mHalftoneData.shadowColor.r);
    }
}

void HalftoneEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}

void HalftoneEffect::SetDirectionalLight(const DirectionalLight& light)
{
    mDirectionalLight = &light;
}

void HalftoneEffect::SetLightCamera(const Camera& camera)
{
    mLightCamera = &camera;
}

void HalftoneEffect::SetShadowMap(const Texture& shadowMap)
{
    mShadowMap = &shadowMap;
}