#include "Snake.h"
#include <vector>
#include <cstddef>

Snake::Snake()
{
    g_bodyPart.reserve(GRID_SIZE * GRID_SIZE - 1);
    constexpr int startLength { 4 };
    constexpr int leftX { startLength + 1 };
    constexpr int halfY { GRID_SIZE / 2 };

    for (int i = 0; i < startLength; i++)
    {
        g_bodyPart.push_back({ leftX - i, halfY });
    }
}

GridPosition Snake::DirectionToGridOffset(Direction dir)
{
    switch (dir)
    {
    case DIR_NORTH: return { 0, -1 };
    case DIR_SOUTH: return { 0, 1 };
    case DIR_EAST:  return { 1, 0 };
    case DIR_WEST:  return { -1, 0 };
    default:        return { 0, 0 };
    }
}

void Snake::Update(Direction nextDirection)
{
    if (nextDirection != DIR_NONE)
    {
        g_currentDirection = nextDirection;
    }

    // move snake body by shifting each segment to next position
    for (std::size_t i = g_bodyPart.size() - 1; i > 0; i--)
    {
        g_bodyPart[i] = g_bodyPart[i - 1];
    }

    // update head in current direction 
    GridPosition dirOnGrid = DirectionToGridOffset(g_currentDirection);
    g_bodyPart[0].x += dirOnGrid.x;
    g_bodyPart[0].y += dirOnGrid.y;
}

void Snake::Grow()
{
    g_bodyPart.push_back(g_bodyPart.back());
}