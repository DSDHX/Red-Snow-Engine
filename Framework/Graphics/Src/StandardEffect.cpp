#include "Precompiled.h"
#include "StandardEffect.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

void StandardEffecr::Initialize(const std::filesystem::path& path)
{
    mTransformBuffer.Initialize();
    mLightBuffer.Initialize();
    mMaterialBuffer.Initialize();
    mSettingsBuffer.Initialize();

    mVertexShader.Initialize<Vertex>(path);
    mPixelShader.Initialize(path);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffecr::Terminate()
{
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mSettingsBuffer.Terminate();
    mMaterialBuffer.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
}

void StandardEffecr::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindVS(0);
    mSampler.BindPS(0);

    mTransformBuffer.BindVS(0);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);
    mMaterialBuffer.BindPS(2);
    mSettingsBuffer.BindVS(3);
    mSettingsBuffer.BindPS(3);
}

void StandardEffecr::End()
{
}

void StandardEffecr::Render(const RenderObject& renderObject)
{
    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    data.world = Math::Transpose(matWorld);
    data.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(data);

    SettingsData settings;
    settings.useDiffuseMap = (renderObject.diffuseMapId > 0 && mSettingsData.useDiffuseMap > 0) ? 1 : 0;
    settings.useSpecMap = (renderObject.specMapId > 0 && mSettingsData.useSpecMap > 0) ? 1 : 0;
    settings.useNormalMap = (renderObject.normalMapId > 0 && mSettingsData.useNormalMap > 0) ? 1 : 0;
    settings.useBumpMap = (renderObject.bumpMapId > 0 && mSettingsData.useBumpMap > 0) ? 1 : 0;
    settings.bumpWeight = mSettingsData.bumpWeight;
    mSettingsBuffer.Update(settings);

    mLightBuffer.Update(*mDirectionalLight);
    mMaterialBuffer.Update(renderObject.material);

    TextureManager* tm = TextureManager::Get();
    tm->BindPS(renderObject.diffuseMapId, 0);
    tm->BindPS(renderObject.specMapId, 1);
    tm->BindPS(renderObject.normalMapId, 2);
    tm->BindVS(renderObject.bumpMapId, 3);

    renderObject.meshBuffer.Render();
}

void StandardEffecr::Render(const RenderGroup& renderGroup)
{
    const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData data;
    data.wvp = Math::Transpose(matFinal);
    data.world = Math::Transpose(matWorld);
    data.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(data);

    mLightBuffer.Update(*mDirectionalLight);

    TextureManager* tm = TextureManager::Get();
    SettingsData settings;
    for (const RenderObject& renderObject : renderGroup.renderObjects)
    {
        settings.useDiffuseMap = (renderObject.diffuseMapId > 0 && mSettingsData.useDiffuseMap > 0) ? 1 : 0;
        settings.useSpecMap = (renderObject.specMapId > 0 && mSettingsData.useSpecMap > 0) ? 1 : 0;
        settings.useNormalMap = (renderObject.normalMapId > 0 && mSettingsData.useNormalMap > 0) ? 1 : 0;
        settings.useBumpMap = (renderObject.bumpMapId > 0 && mSettingsData.useBumpMap > 0) ? 1 : 0;
        settings.bumpWeight = mSettingsData.bumpWeight;
        mSettingsBuffer.Update(settings);
        mMaterialBuffer.Update(renderObject.material);

        tm->BindPS(renderObject.diffuseMapId, 0);
        tm->BindPS(renderObject.specMapId, 1);
        tm->BindPS(renderObject.normalMapId, 2);
        tm->BindVS(renderObject.bumpMapId, 3);

        renderObject.meshBuffer.Render();
    }

    mLightBuffer.Update(*mDirectionalLight);
}

void StandardEffecr::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}
void StandardEffecr::SetDirectionalLight(const DirectionalLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}
void StandardEffecr::DebugUI()
{
    if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
        if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
        {
            mSettingsData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;
        }
        bool useSpecMap = mSettingsData.useSpecMap > 0;
        if (ImGui::Checkbox("UseSpecMap", &useSpecMap))
        {
            mSettingsData.useSpecMap = (useSpecMap) ? 1 : 0;
        }
        bool useNormalMap = mSettingsData.useNormalMap > 0;
        if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
        {
            mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;
        }
        bool useBumpMap = mSettingsData.useBumpMap > 0;
        if (ImGui::Checkbox("UseBumpMap", &useBumpMap))
        {
            mSettingsData.useBumpMap = (useBumpMap) ? 1 : 0;
        }
        ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 100.0f);
    }
}