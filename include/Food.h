#pragma once
#include "GameData.h"
#include "Snake.h"

class Food
{
public:
	Food();

	void Spawn(const Snake& snake);

	bool CheckCollision(const Snake& snake);

	GridPosition position;
};