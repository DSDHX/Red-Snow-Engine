#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace RedSnowEngine;
using namespace RedSnowEngine::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] =
        {
            Colors::Red,
            Colors::Yellow,
            Colors::Blue,
            Colors::Green,
            Colors::Purple,
            Colors::Orange,
            Colors::Pink
        };

        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }

    void CreateCubeIndices(std::vector<uint32_t>& indices)
    {
        indices =
        {
            // Front
            0, 1, 2,
            0, 2, 3,
            // Back
            7, 5, 4,
            7, 6, 5,
            // Right
            3, 2, 6,
            3, 6, 7,
            // Left
            4, 5, 1,
            4, 1, 0,
            // Top
            1, 5, 6,
            1, 6, 2,
            // Bottom
            0, 3, 7,
            0, 7, 4,
        };
    }

    void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numColums)
    {
        for (int r = 0; r < numRows; r++)
        {
            for (int c = 0; c < numColums; c++)
            {
                int i = (r * (numColums + 1)) + c;

                indices.push_back(i);
                indices.push_back(i + numColums + 1);
                indices.push_back(i + numColums + 2);

                indices.push_back(i);
                indices.push_back(i + numColums + 2);
                indices.push_back(i + 1);
            }
        }
    }

    void CreateCapIndices(std::vector<uint32_t>& indices, int slices, int topIndex, int bottomIndex)
    {
        for (int s = 0; s < slices; s++)
        {
            indices.push_back(bottomIndex);
            indices.push_back(s);
            indices.push_back(s + 1);

            int topRowIndex = topIndex - slices - 1 + s;
            indices.push_back(topIndex);
            indices.push_back(topRowIndex + 1);
            indices.push_back(topRowIndex);
        }
    }
}

MeshPC MeshBuilder::CreateCubePC(float size, const Color& color)
{
    MeshPC mesh;

    const float hs = size * 0.5f;

    // Front
    mesh.vertices.push_back({ { -hs, -hs, -hs}, color });
    mesh.vertices.push_back({ { -hs,  hs, -hs}, color });
    mesh.vertices.push_back({ {  hs,  hs, -hs}, color });
    mesh.vertices.push_back({ {  hs, -hs, -hs}, color });

    // Back
    mesh.vertices.push_back({ { -hs, -hs,  hs}, color });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, color });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, color });
    mesh.vertices.push_back({ {  hs, -hs,  hs}, color });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = size * 0.5f;

    // Front
    mesh.vertices.push_back({ { -hs, -hs, -hs}, GetNextColor(index)});
    mesh.vertices.push_back({ { -hs,  hs, -hs}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, -hs}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs}, GetNextColor(index) });

    // Back
    mesh.vertices.push_back({ { -hs, -hs,  hs}, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs,  hs}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs,  hs}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs,  hs}, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPX MeshBuilder::CreateCubePX(float size)
{
    MeshPX mesh;


    return mesh;
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = size * 0.5f;

    mesh.vertices.push_back({ { 0.0f, hs, 0.0f }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs, -hs,  hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs,  hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });
    mesh.indices =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 3, 2,
        1, 4, 3
    };

    return mesh;
}

MeshPC MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hd = depth * 0.5f;

    // Front
    mesh.vertices.push_back({ { -hw, -hh, -hd}, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, -hd}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, -hd}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, -hd}, GetNextColor(index) });

    // Back
    mesh.vertices.push_back({ { -hw, -hh,  hd}, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh,  hd}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh,  hd}, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh,  hd}, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(int numRows, int numColums, float spacing, bool horizontal)
{
    MeshPC mesh;
    int index = rand() % 100;
    const float hpw = static_cast<float>(numColums) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float w = -hpw;
    float h = -hph;

    for (int r = 0; r <= numRows; r++)
    {
        for (int c = 0; c <= numColums; c++)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h) : Math::Vector3(w, h, 0.0f);
            mesh.vertices.push_back({ pos, GetNextColor(index) });
            w += spacing;
        }
        w = -hpw;
        h += spacing;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColums);

    return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(int numRows, int numColums, float spacing, bool horizontal)
{
    MeshPX mesh;

    const float hpw = static_cast<float>(numColums) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numColums);
    const float vInc = -1.0f / static_cast<float>(numRows);

    float w = -hpw;
    float h = -hph;
    float u = 0.0f;
    float v = 1.0f;

    for (int r = 0; r <= numRows; r++)
    {
        for (int c = 0; c <= numColums; c++)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h) : Math::Vector3(w, h, 0.0f);
            mesh.vertices.push_back({ pos, { u, v } });
            w += spacing;
            u += uInc;
        }
        w = -hpw;
        h += spacing;
        u = 0.0f;
        v += vInc;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColums);

    return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(int slices, int rings)
{
    MeshPC mesh;
    int index = rand() % 100;

    const float hh = static_cast<float>(rings) * 0.5f;
    const float fSlice = static_cast<float>(slices);

    for (int r = 0; r <= rings; r++)
    {
        float rings = static_cast<float>(r);
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = (slice / fSlice) * Math::Constants::TwoPi;

            mesh.vertices.push_back({ {
                    sin(rotation),
                    rings - hh,
                    -cos(rotation)},
                    GetNextColor(index) });
        }
    }

    mesh.vertices.push_back({ { 0.0f, hh, 0.0f }, GetNextColor(index) });
    mesh.vertices.push_back({ { 0.0f, -hh, 0.0f }, GetNextColor(index) });

    CreatePlaneIndices(mesh.indices, rings, slices);
    CreateCapIndices(mesh.indices, slices, mesh.vertices.size() - 2, mesh.vertices.size() - 1);

    return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
    MeshPC mesh;
    int index = rand() % 100;

    float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
    float horRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;
            mesh.vertices.push_back({ {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi) },
                    GetNextColor(index) });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
    MeshPX mesh;

    float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
    float horRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;

            float u = uStep * slice;
            float v = vStep * ring;
            mesh.vertices.push_back({ {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi) },
                    { u, v } });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX RedSnowEngine::Graphics::MeshBuilder::CreateSkySpherePX(int slices, int rings, float radius)
{
    MeshPX mesh;

    float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
    float horRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;

            float u = uStep * slice;
            float v = vStep * ring;
            mesh.vertices.push_back({ {
                    radius* cos(rotation)* sin(phi),
                    radius * cos(phi),
                    radius* sin(rotation)* sin(phi) },
                    { u, v } });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}