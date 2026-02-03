#pragma once

#include "ModelManager.h"

namespace RedSnowEngine::Graphics::AnimationUtil
{
    using BoneTransforms = std::vector<Math::Matrix4>;

    void ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms);
    void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
    void ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms);
}