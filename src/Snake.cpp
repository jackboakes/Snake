#include "Snake.h"
#include <vector>
#include <cstddef>


Snake::Snake()
{
    bodyPart.reserve(GRID_SIZE * GRID_SIZE - 1);
    constexpr int startLength { 4 };
    constexpr int leftX { startLength + 1 };
    constexpr int halfY { GRID_SIZE / 2 };

    for (int i = 0; i < startLength; i++)
    {
        bodyPart.push_back({ leftX - i, halfY });
    }
}

GridPosition Snake::DirectionToGridOffset(Direction dir)
{
    switch (dir)
    {
    case Direction::NORTH: return { 0, -1 };
    case Direction::SOUTH: return { 0, 1 };
    case Direction::EAST:  return { 1, 0 };
    case Direction::WEST:  return { -1, 0 };
    default:        return { 0, 0 };
    }
}

void Snake::Update(Direction nextDirection)
{
    if (nextDirection != Direction::NONE)
    {
        currentDirection = nextDirection;
    }

    // move snake body by shifting each segment to next position
    for (std::size_t i = bodyPart.size() - 1; i > 0; i--)
    {
        bodyPart[i] = bodyPart[i - 1];
    }

    // update head in current direction 
    GridPosition dirOnGrid = DirectionToGridOffset(currentDirection);
    bodyPart[0].x += dirOnGrid.x;
    bodyPart[0].y += dirOnGrid.y;
}

void Snake::Grow()
{
    bodyPart.push_back(bodyPart.back());
}