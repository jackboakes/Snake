#pragma once
#include <vector>
#include "GameData.h"
#include "Input.h"

class Snake
{
public:
    std::vector<GridPosition> g_bodyPart;
    Direction g_currentDirection { Direction::EAST };
    float g_moveSpeed { 10.0f };
private:

public:
    Snake();
    void Update(Direction nextDirection);
    void Grow();

private:
    GridPosition DirectionToGridOffset(Direction dir);
};