#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"

extern Color const snakeColour;


Color const boardColors[2] = {
    { 0x49, 0x43, 0x51, 0xFF },  // light
    { 0x44, 0x3E, 0x4C, 0xFF }   // dark
};

void LoadSnakeTextures();
void UnloadSnakeTextures();


void DrawGameBoard();
void DrawUI(int score, int highScore);
Rectangle GetHeadSpriteRect(Direction direction);
void DrawSnake(const Snake& snake);
void DrawFood(const Food& food);

#endif