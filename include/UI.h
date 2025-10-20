#pragma once
#include "Audio.h"
#include <vector>

enum MainMenuOption
{
	MAIN_MENU_START,
	MAIN_MENU_QUIT,
	MAIN_MENU_OPTION_COUNT
};

enum GameOverOption
{
	GAME_OVER_MAIN_MENU,
	GAME_OVER_RESTART,
	GAME_OVER_QUIT,
	GAME_OVER_OPTION_COUNT
};

class Button
{
public:
	enum class State
	{
		DEFAULT,
		HOVERED,
		PRESSED,
		RELEASED
	};

	Button();
	void SetState(State newState);
	Button::State GetState() const;

	int id { -1 };
	Rectangle bounds {};
	const char* text {};
private:
	State m_state { State::DEFAULT };
};

class UI
{
public:
	UI();

	void Update(Audio& audio);
	void AddButton(int id, Rectangle bounds, const char* text);
	std::vector<Button> GetButtons() const;
	bool WasActiveButtonReleased(int buttonID);
	void CentreButtonsVertically(int screenWidth, int screenHeight, int buttonWidth, int buttonHeight, int padding);

private:
	int m_activeButtonID { -1 };
	std::vector<Button> m_buttons {};

	Vector2 m_mousePos { 0, 0 };
	bool m_mouseButtonDown { false };
	bool m_mouseButtonReleased { false };
};

