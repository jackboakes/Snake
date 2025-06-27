#ifndef INPUT_H
#define INPUT_H


enum InputAction
{
	INPUT_NONE = 0,
	INPUT_UP,
	INPUT_LEFT,
	INPUT_DOWN,
	INPUT_RIGHT,
	INPUT_SELECT,
	INPUT_QUIT
};


InputAction ReadInputMenu();
InputAction ReadInputGame();

#endif