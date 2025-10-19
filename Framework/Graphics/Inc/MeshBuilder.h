#pragma once

#include "MeshTypes.h"

namespace RedSnowEngine::Graphics
{
    class MeshBuilder
    {
    public:
        static MeshPC CreateCubePC(float size, const Color& color);
        static MeshPC CreateCubePC(float size);
        static MeshPX CreateCubePX(float size);

        static MeshPC CreatePyramidPC(float size);

        static MeshPC CreateRectanglePC(float width, float height, float depth);

        static MeshPC CreatePlanePC(int numRows, int numColums, float spacing, bool horizontal = true);
        static MeshPX CreatePlanePX(int numRows, int numColums, float spacing, bool horizontal = true);
        static Mesh CreatePlane(int numRows, int numColums, float spacing, bool horizontal = true);

        static MeshPC CreateCylinderPC(int slices, int rings);

        static MeshPC CreateSpherePC(int slices, int rings, float radius);
        static MeshPX CreateSpherePX(int slices, int rings, float radius);
        static Mesh CreateSphere(int slices, int rings, float radius);

        static MeshPX CreateSkySpherePX(int slices, int rings, float radius);

        static MeshPX CreateOBJPX(const std::filesystem::path& filePath, float scale);

    private:

    };
}