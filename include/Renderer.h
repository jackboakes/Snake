#pragma once
#include "GameState.h"
#include "UI.h"

// Menu global constants
inline constexpr int TITLE_FONT_SIZE { 40 };
inline constexpr int OPTION_FRONT_SIZE { 30 };
inline constexpr int INSTRUCTION_FONT_SIZE { 20 };
inline constexpr int VERTICAL_SPACING { 50 };

inline constexpr Color SNAKE_COLOUR = { 0xC2, 0x32, 0x1D, 0xFF }; // pikmin red
inline constexpr Color BORDER_COLOUR = { 0x5A, 0x54, 0x62, 0xFF }; // A mid-tone gray/purple
inline constexpr Color  BACKGROUND_COLOUR = { 0x2D, 0x27, 0x2F, 0xFF }; // dark purple/black
inline constexpr Color  BOARD_COLOURS[2] = {
    { 0x49, 0x43, 0x51, 0xFF },  // light
    { 0x44, 0x3E, 0x4C, 0xFF }   // dark
};

// Button colours
inline constexpr Color BUTTON_FACE_DEFAULT = BORDER_COLOUR; // A mid-tone gray/purple
inline constexpr Color BUTTON_FACE_HOVER = { 0x6C, 0x65, 0x73, 0xFF }; // A slightly lighter version of default
inline constexpr Color BORDER_LIGHT = { 0x8C, 0x81, 0x93, 0xFF }; // Light edge
inline constexpr Color BORDER_DARK = { 0x3E, 0x35, 0x42, 0xFF }; // Dark edge

class Renderer
{
public:
    // render scene functions
    static void Gameplay(GameState& gameState);
    static void MainMenu(const UI& ui);
    static void GameOver(const UI& ui, int score, int highScore);

    // Built upon raylib functions
    static void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour);
    static void DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour);

private:
    static void DrawGameBoard();
    static void DrawGameUI(int score, int highScore);
    static Rectangle GetHeadSpriteRect(Direction direction);
    static void DrawSnake(const Snake& snake);
    static void DrawFood(const Food& food);
    static void DrawCenteredTitle(const char* title, int y, int fontSize, Color color);
    static void RenderButton(const Button& button);
    static void RenderAllButtons(const UI& ui);
};



