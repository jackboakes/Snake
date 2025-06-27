#include <raylib.h>
#include "Input.h"

InputAction ReadInputMenu()
{
	if (IsKeyPressed(KEY_W))
	{
		return INPUT_UP;
	}

	if (IsKeyPressed(KEY_S))
	{
		return INPUT_DOWN;
	}

	if (IsKeyPressed(KEY_ENTER)) {
		return INPUT_SELECT;
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		return INPUT_QUIT;
	}

	return INPUT_NONE;
}

InputAction ReadInputGame()
{
	if (IsKeyPressed(KEY_W))
	{
		return INPUT_UP;
	}

	if (IsKeyPressed(KEY_A))
	{
		return INPUT_LEFT;
	}

	if (IsKeyPressed(KEY_S))
	{
		return INPUT_DOWN;
	}

	if (IsKeyPressed(KEY_D))
	{
		return INPUT_RIGHT;
	}

	return INPUT_NONE;
}