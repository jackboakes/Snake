#ifndef UI_H
#define UI_H

#define MAX_BUTTONS 5
#define MAX_BUTTON_TEXT_LENGTH 32

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

	Sound buttonSound;
};

void InitMainMenuUI(UI* ui);
void InitGameOverUI(UI* ui);
void UpdateUI(UI* ui);
void ResetUI(UI* ui);

int AddButton(UI* ui, Rectangle bounds, const char* text, int id);
bool IsButtonActive(const UI* ui, int buttonId);
Button* GetButton(UI* ui, int buttonId);
int GetButtonIndex(const UI* ui, int buttonId);

// Layout Helpers
void CenterButtonsVertically(UI* ui, int screenWidth, int screenHeight, int buttonWidth, int buttonHeight, int padding);
void PositionButton(UI* ui, int buttonIndex, int x, int y, int width, int height);

// Rendering
void RenderUI(const UI* ui);
void RenderButton(const Button* button);

#endif