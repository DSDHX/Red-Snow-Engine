#include "Precompiled.h"
#include "AnimationUtil.h"

#include "Color.h"
#include "SimpleDraw.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

namespace
{
    void ComputeBoneTransformsRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
    {
        if (bone != nullptr)
        {
            if (animator == nullptr || !animator->GoToParentTransform(bone, boneTransforms[bone->index]))
            {
                boneTransforms[bone->index] = bone->toParentTransform;
            }
            if (bone->parent != nullptr)
            {
                boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
            }
            for (const Bone* child : bone->children)
            {
                ComputeBoneTransformsRecursive(child, boneTransforms, animator);
            }
        }
    }
}

void AnimationUtil::ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator)
{
    const Model* model = ModelManager::Get()->GetModel(modelId);
    if (model != nullptr && model->skeleton != nullptr)
    {
        boneTransforms.resize(model->skeleton->bones.size());
        ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms, animator);
    }
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(modelId);
    if (model != nullptr && model->skeleton != nullptr)
    {
        for (const auto& bone : model->skeleton->bones)
        {
            if (bone->parent != nullptr)
            {
                const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
                const Math::Vector3 parentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
                SimpleDraw::AddLine(bonePos, parentPos, Colors::HotPink);
                SimpleDraw::AddSphere(10, 10, 0.03f, Colors::Blue, bonePos);
            }
        }
    }
}

void AnimationUtil::ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(modelId);
    if (model != nullptr && model->skeleton != nullptr)
    {
        for (auto& bone : model->skeleton->bones)
        {
            boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
        }
    }
}