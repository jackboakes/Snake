#include "raylib.h"
#include "UI.h"
#include "Renderer.h"
#include "Audio.h"


void InitMainMenuUI(UI* ui)
{
    ui->buttonCount = 0;
    ui->mousePos = { 0, 0 };
    ui->mouseButtonDown = false;
    ui->mouseButtonReleased = false;
    ui->activeButtonID = 0;

    const int buttonWidth = 32 * 9;
    const int buttonHeight = 32 * 2;
    const int buttonPadding = 20;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

    AddButton(ui, buttonBounds, "Start Game", MAIN_MENU_START);
    AddButton(ui, buttonBounds, "Quit", MAIN_MENU_QUIT);

    CenterButtonsVertically(ui, screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
}

void InitGameOverUI(UI* ui)
{
    ui->buttonCount = 0;
    ui->mousePos = { 0, 0 };
    ui->mouseButtonDown = false;
    ui->mouseButtonReleased = false;
    ui->activeButtonID = 0;

    const int buttonWidth = 32 * 9;
    const int buttonHeight = 32 * 2;
    const int buttonPadding = 20;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

    AddButton(ui, buttonBounds, "Main Menu", GAME_OVER_MAIN_MENU);
    AddButton(ui, buttonBounds, "Restart", GAME_OVER_RESTART);
    AddButton(ui, buttonBounds, "Quit", GAME_OVER_QUIT);

    CenterButtonsVertically(ui, screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
}


void UpdateUI(UI* ui, Sound buttonSound)
{
	ui->mousePos = GetMousePosition();
    ui->mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	ui->mouseButtonReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    ui->activeButtonID = -1;

    for (int i = 0; i < ui->buttonCount; i++)
    {
        Button* button = &ui->buttons[i];

        // reset state
        button->isHovered = false;
        button->isPressed = false;
        button->isReleased = false;

        // Check if mouse is over button
        if (CheckCollisionPointRec(ui->mousePos, button->bounds))
        {
            button->isHovered = true;
            ui->activeButtonID = button->id;

            if (ui->mouseButtonDown)
            {

                button->isPressed = true;
            }

            if (ui->mouseButtonReleased)
            {
                PlaySoundRandomisedPitch(buttonSound);
                button->isReleased = true;
            }

        }
    }
}

void AddButton(UI* ui, Rectangle bounds, const char* text, int id)
{
    if (ui->buttonCount >= MAX_BUTTONS) return;
    if (GetButton(ui, id) != NULL) return;
  

    int index = ui->buttonCount++;
    Button* button = &ui->buttons[index];

    button->id = id;
    button->bounds = bounds;
    button->text = text;
    button->isHovered = false;
    button->isPressed = false;
}

bool IsButtonActive(const UI* ui, int buttonId)
{
    return ui->activeButtonID == buttonId;
}


Button* GetButton(const UI* ui, int buttonId)
{
    for (int i = 0; i < ui->buttonCount; i++)
    {
        if (ui->buttons[i].id == buttonId)
        {
            return (Button*)&ui->buttons[i];
        }
    }
    return NULL;
}

// needs to be hovered and released on the button
bool WasActiveButtonReleased(const UI* ui, int buttonId)
{
    if (!IsButtonActive(ui, buttonId))
        return false;

    Button* button = GetButton(ui, buttonId);
    if (button == NULL)
    {
        return false;
    }

    return button->isReleased;
}

void CenterButtonsVertically(UI* ui, int screenWidth, int screenHeight, int buttonWidth, int buttonHeight, int padding)
{
    if (ui->buttonCount == 0) return;

    const int totalButtonsHeight = (ui->buttonCount * buttonHeight) +
        ((ui->buttonCount - 1) * padding);
    const int startY = (screenHeight / 2) - (totalButtonsHeight / 2);

    for (int i = 0; i < ui->buttonCount; i++)
    {
        ui->buttons[i].bounds = {
            (screenWidth / 2.0f) - (buttonWidth / 2.0f),
            (float) startY + (i * (buttonHeight + padding)),
            (float) buttonWidth,
            (float) buttonHeight
        };
    }
}

void PositionButton(UI* ui, int buttonId, int x, int y, int width, int height)
{
    Button* button = GetButton(ui, buttonId);
    if (button != NULL)
    {
        button->bounds = {
            (float)x, (float)y, (float)width, (float)height
        };
    }
}




