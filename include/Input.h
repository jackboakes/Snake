#pragma once

constexpr int INPUT_QUEUE_SIZE { 2 };

enum class InputAction
{
	NONE = 0,
	UP,
	LEFT,
	DOWN,
	RIGHT
};

enum class Direction
{
	NONE = 0,
	NORTH,
	WEST,
	SOUTH,
	EAST
};

InputAction ReadGameInput();