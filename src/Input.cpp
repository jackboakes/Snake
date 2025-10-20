#include "raylib.h"
#include "Input.h"


InputAction ReadGameInput()
{
	if (IsKeyPressed(KEY_W))
	{
		return InputAction::UP;
	}

	if (IsKeyPressed(KEY_A))
	{
		return InputAction::LEFT;
	}

	if (IsKeyPressed(KEY_S))
	{
		return InputAction::DOWN;
	}

	if (IsKeyPressed(KEY_D))
	{
		return InputAction::RIGHT;
	}

	return InputAction::NONE;
}
