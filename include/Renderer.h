#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"

extern Color const snakeColour;


Color const boardColors[2] = {
    { 0x49, 0x43, 0x51, 0xFF },  // light
    { 0x44, 0x3E, 0x4C, 0xFF }   // dark
};

void DrawGameBoard();
void DrawUI(GameState* gameState);
void DrawSnake(GameState* gameState);
void DrawFood(GameState* gameState);

#endif