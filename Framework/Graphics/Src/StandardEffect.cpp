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

    mVertexShader.Initialize<Vertex>(path);
    mPixelShader.Initialize(path);
}

void StandardEffecr::Terminate()
{
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mLightBuffer.Terminate();
    mTransformBuffer.Terminate();
}

void StandardEffecr::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);
    mLightBuffer.BindVS(1);
    mLightBuffer.BindPS(1);
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

    mLightBuffer.Update(*mDirectionalLight);

    renderObject.meshBuffer.Render();
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
}