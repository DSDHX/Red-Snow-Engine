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

    mesh.indices =
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

    mesh.indices =
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

    return mesh;
}

MeshPC MeshBuilder::CreatePyramid(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = size * 0.5f;

    return mesh;
}

MeshPC MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hd = depth * 0.5f;

    return mesh;
}