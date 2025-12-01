#pragma once

#include "MeshTypes.h"

namespace RedSnowEngine::Graphics
{
    class Terrain final
    {
    public:
        void Initialize(const std::filesystem::path& fileName, float hightScale);
        float GetHight(const Math::Vector3& position) const;

        Mesh mesh;
        uint32_t rows = 0;
        uint32_t columns = 0;
    };
}