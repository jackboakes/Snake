#pragma once

constexpr int TILE_SIZE { 32 };
constexpr int GRID_SIZE { 20 };
constexpr int BORDER_THICKNESS { 5 };
constexpr int GAME_OFFSET { ((TILE_SIZE * 2) + BORDER_THICKNESS) };
constexpr int GAME_WIDTH { (GRID_SIZE * TILE_SIZE) };
constexpr int GAME_HEIGHT { (GRID_SIZE * TILE_SIZE) };
constexpr int SCORE_INCREMENT { 5 };

struct GridPosition
{
    int x;
    int y;
};