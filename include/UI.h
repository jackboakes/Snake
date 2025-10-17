#pragma once
#include "Audio.h"

#define MAX_BUTTONS 5
#define MAX_BUTTON_TEXT_LENGTH 32

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

struct Button
{
	int id;
	Rectangle bounds;
	const char* text;
	bool isHovered;
	bool isPressed;
	bool isReleased;
};

struct UI
{
	Button buttons[MAX_BUTTONS];
	int buttonCount;
	Vector2 mousePos;
	bool mouseButtonDown;
	bool mouseButtonReleased;
	int activeButtonID;
};

void InitMainMenuUI(UI* ui);
void InitGameOverUI(UI* ui);
void UpdateUI(UI* ui, Audio& audio);

// Button functions
void AddButton(UI* ui, Rectangle bounds, const char* text, int id);
bool IsButtonActive(const UI* ui, int buttonId);
Button* GetButton(const UI* ui, int buttonId);
bool WasActiveButtonReleased(const UI* ui, int buttonId);

// Layout Helpers
void CenterButtonsVertically(UI* ui, int screenWidth, int screenHeight, int buttonWidth, int buttonHeight, int padding);
void PositionButton(UI* ui, int buttonIndex, int x, int y, int width, int height);
