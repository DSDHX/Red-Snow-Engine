#pragma once

#include "MeshTypes.h"

namespace RedSnowEngine::Graphics
{
	class MeshBuilder
	{
	public:
        static MeshPC CreateCubePC(float size, const Color& color);
		static MeshPC CreateCubePC(float size);

        static MeshPC CreatePyramid(float size);

        static MeshPC CreateRectanglePC(float width, float height, float depth);

	private:

	};
}