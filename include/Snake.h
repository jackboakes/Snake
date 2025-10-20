#pragma once
#include "GameData.h"
#include "Input.h"
#include <vector>

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