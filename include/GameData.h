#pragma once

#define TILE_SIZE 32
#define GRID_SIZE 20
#define BORDER_THICKNESS 5
#define GAME_OFFSET ((TILE_SIZE * 2) + BORDER_THICKNESS)
#define GAME_WIDTH (GRID_SIZE * TILE_SIZE)
#define GAME_HEIGHT (GRID_SIZE * TILE_SIZE)
#define SCORE_INCREMENT 5

struct GridPosition
{
    int x;
    int y;
};