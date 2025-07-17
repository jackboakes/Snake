#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"

// Menu global constants
#define TITLE_FONT_SIZE 40
#define OPTION_FRONT_SIZE 30
#define INSTRUCTION_FONT_SIZE 20
#define VERTICAL_SPACING 50

// Resource management
static Rectangle GetSpriteRect(int col, int row);
void LoadGameTextures();
void UnloadGameTextures();
void UpdateWindowIcon();

// Gameplay draw functions
static void DrawGameBoard();
static void DrawGameUI(int score, int highScore);
static Rectangle GetHeadSpriteRect(Direction direction);
static void DrawSnake(const Snake* snake);
static void DrawFood(const Food* food);
void RenderGameplay(GameState* gameState);

// Built upon raylib functions
void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour);
void DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour);

// Rendering functions

void RenderUI(const UI* ui);
void RenderButton(const Button* button);


#endif