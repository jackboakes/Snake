#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"

// Menu global constants
#define TITLE_FONT_SIZE 40
#define OPTION_FRONT_SIZE 30
#define INSTRUCTION_FONT_SIZE 20
#define VERTICAL_SPACING 50


// Gameplay functions
void RenderGameplay(GameState* gameState);

// Built upon raylib functions
void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour);
void DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour);

// ui functions
void RenderMainMenu(const UI* ui);
void RenderGameOver(const UI* ui, int score, int highScore);



#endif