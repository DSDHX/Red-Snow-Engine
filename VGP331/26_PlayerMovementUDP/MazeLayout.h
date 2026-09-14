#pragma once

#include <array>
#include <string_view>

namespace MazeLayout
{
    constexpr float CellSize = 2.0f;
    constexpr float PlayerRadius = 0.45f;
    constexpr float MoveSpeed = 3.5f;
    constexpr int Size = 11;
    constexpr std::array<std::string_view, Size> Cells = {
        "###########",
        "#12...#...#",
        "#.###.#.#.#",
        "#...#...#.#",
        "###.#####.#",
        "#...#.....#",
        "#.###.###.#",
        "#.....#...#",
        "#.#####.#.#",
        "#.......#E#",
        "###########"
    };
    constexpr float X(int column) { return (column - Size / 2) * CellSize; }
    constexpr float Z(int row) { return (Size / 2 - row) * CellSize; }
}