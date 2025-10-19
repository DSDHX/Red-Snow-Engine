#pragma once

#include "MeshBuffer.h"
#include "Transform.h"

namespace RedSnowEngine::Graphics
{
    class RenderObject
    {
    public:
        void Terminate();

        Transform transform;
        MeshBuffer meshBuffer;
    };
}