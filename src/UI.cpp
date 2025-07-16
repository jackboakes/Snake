#include "raylib.h"
#include "UI.h"
#include <stdio.h>

static const Color BUTTON_FACE_DEFAULT = { 0x40, 0x3A, 0x48, 0xFF };
static const Color BUTTON_FACE_HOVER = { 0x6C, 0x65, 0x73, 0xFF };
static const Color BORDER_LIGHT = { 0x8C, 0x81, 0x93, 0xFF };
static const Color BORDER_DARK = { 0x3E, 0x35, 0x42, 0xFF };


void InitMainMenuUI(UI* ui)
{
    *ui = { 0 };

    const int buttonWidth = 32 * 9;
    const int buttonHeight = 32 * 2;
    const int buttonPadding = 20;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

    AddButton(ui, buttonBounds, "Start Game", 1);
    AddButton(ui, buttonBounds, "Quit", 2);

    CenterButtonsVertically(ui, screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
}

void InitGameOverUI(UI* ui)
{
    *ui = { 0 };

    const int buttonWidth = 32 * 9;
    const int buttonHeight = 32 * 2;
    const int buttonPadding = 20;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

    AddButton(ui, buttonBounds, "Main Menu", 1);
    AddButton(ui, buttonBounds, "Restart", 2);
    AddButton(ui, buttonBounds, "Quit", 3);

    CenterButtonsVertically(ui, screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
}


void UpdateUI(UI* ui)
{
	ui->mousePos = GetMousePosition();
    ui->mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	ui->mouseButtonReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

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
                button->isReleased = true;
            }
        }
    }
}

int AddButton(UI* ui, Rectangle bounds, const char* text, int id)
{
    if (ui->buttonCount >= MAX_BUTTONS) return -1;
    if (GetButton(ui, id) != NULL) return -1;
  

    int index = ui->buttonCount++;
    Button* button = &ui->buttons[index];

    button->id = id;
    button->bounds = bounds;
    button->text = text;

    return index;
}

bool IsButtonActive(const UI* ui, int buttonId)
{
    return ui->activeButtonID == buttonId;
}

Button* GetButton(UI* ui, int buttonId)
{
    for (int i = 0; i < ui->buttonCount; i++)
    {
        if (ui->buttons[i].id == buttonId)
        {
            return &ui->buttons[i];
        }
    }
    return NULL;
}

int GetButtonIndex(const UI* ui, int buttonId)
{
    for (int i = 0; i < ui->buttonCount; i++)
    {
        if (ui->buttons[i].id == buttonId)
        {
            return i;
        }
    }
    return -1;
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

static void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour)
{
    Rectangle top = { borderRec.x, borderRec.y, borderRec.width, borderThickness };
    Rectangle bottom = { borderRec.x, borderRec.y + borderRec.height - borderThickness, borderRec.width, borderThickness };
    Rectangle left = { borderRec.x, borderRec.y + borderThickness, borderThickness, borderRec.height - (2 * borderThickness) };
    Rectangle right = { borderRec.x + borderRec.width - borderThickness, borderRec.y + borderThickness, borderThickness, borderRec.height - (2 * borderThickness) };

    DrawRectangleRec(top, lightColour);
    DrawRectangleRec(bottom, darkColour);
    DrawRectangleRec(left, lightColour);
    DrawRectangleRec(right, darkColour);
}

static void DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour)
{
    Color const shadowColor = { 0, 0, 0, 128 };
    DrawText(text, posX + 2, posY + 2, fontSize, shadowColor);
    DrawText(text, posX, posY, fontSize, colour);
}

void RenderButton(const Button* button)
{
    const int borderWidth = 5;
    const int fontSize = 25;
    const int fontSpacing = fontSize / 10;

    const int textWidth = MeasureTextEx(GetFontDefault(), button->text, fontSize, fontSpacing).x;
    const int textHeight = MeasureTextEx(GetFontDefault(), button->text, fontSize, fontSpacing).y;
    const int textPosX = button->bounds.x + (button->bounds.width - textWidth) / 2;
    const int textPosY = button->bounds.y + (button->bounds.height - textHeight) / 2;

    Color buttonColour;
    Color textColour = WHITE;


    if (button->isHovered)
    {
        buttonColour = BUTTON_FACE_HOVER;
    }
    else
    {
        buttonColour = BUTTON_FACE_DEFAULT;
    }

    DrawRectangleRec(button->bounds, buttonColour);

    if (button->isPressed)
    {
        // Invert the light source, remove shadow and move text for a pressed in look
        DrawBeveledBorder(button->bounds, borderWidth, BORDER_DARK, BORDER_LIGHT);
        DrawText(button->text, textPosX + 2, textPosY + 2, fontSize, textColour);
    }
    else
    {
        // Standard top left light source look with a shadow
        DrawBeveledBorder(button->bounds, borderWidth, BORDER_LIGHT, BORDER_DARK);
        DrawTextWithShadow(button->text, textPosX, textPosY, fontSize, textColour);

    }
}

void RenderUI(const UI* ui)
{
    for (int i = 0; i < ui->buttonCount; i++)
    {
        RenderButton(&ui->buttons[i]);
    }
}


