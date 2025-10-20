#pragma once
#include "GameData.h"
#include "Input.h"
#include <vector>

class Snake
{
public:
    std::vector<GridPosition> bodyPart;
    Direction currentDirection { Direction::EAST };
    float moveSpeed { 10.0f };

    Snake();
    void Update(Direction nextDirection);
    void Grow();

private:
    GridPosition DirectionToGridOffset(Direction dir);
};