#pragma once

#include "Camera.h"
#include "DirectionalLight.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Sampler.h"
#include "Material.h"
#include "RenderObject.h"

namespace RedSnowEngine::Graphics
{
    class RenderGroup;

    class HalftoneEffect
    {
    public:
        void Initialize(const std::filesystem::path& fileName);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);
        void Render(const RenderGroup& renderGroup);

        void DebugUI();

        void SetCamera(const Camera& camera);
        void SetDirectionalLight(const DirectionalLight& light);
        void SetLightCamera(const Camera& camera);
        void SetShadowMap(const Texture& shadowMap);

    private:
        struct TransformData
        {
            Math::Matrix4 world;
            Math::Matrix4 wvp;
            Math::Matrix4 lightViewProj;
            Math::Vector3 viewPosition;
            float padding;
        };

        struct HalftoneData
        {
            Math::Vector2 viewportSize;
            float dotDensity;
            float power;
            Color shadowColor;
            Color lightColor;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using LightBuffer = TypedConstantBuffer<DirectionalLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using SettingsBuffer = TypedConstantBuffer<HalftoneData>;

        TransformBuffer mTransformBuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMaterialBuffer;
        SettingsBuffer mSettingsBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;

        Sampler mSampler;
        Sampler mShadowSampler;

        const Camera* mCamera = nullptr;
        const Camera* mLightCamera = nullptr;
        const DirectionalLight* mDirectionalLight = nullptr;
        const Texture* mShadowMap = nullptr;

        HalftoneData mHalftoneData;
    };
}