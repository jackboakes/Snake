#include <raylib.h>
#include "Renderer.h"
#include "Game.h"

// snake colour CA0004
Color const snakeColour = { 0xC2, 0x32, 0x1D, 0xFF };
Color const borderColor = { 0x5A, 0x54, 0x62, 0xFF };
//Color snakeRed = { 0xC2, 0x32, 0x1D, 0xFF } { 0xCA, 0x00, 0x04, 0xFF }

void DrawGameBoard()
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            int posX = GAME_OFFSET + (x * TILE_SIZE);
            int posY = GAME_OFFSET + (y * TILE_SIZE);

            Color tileColor = boardColors[(x + y) % 2];
            DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, tileColor);
        }
    }
    Rectangle borderRect = {
          GAME_OFFSET - 5,
          GAME_OFFSET - 5,
          GAME_WIDTH + 10,
          GAME_HEIGHT + 10
    };

    DrawRectangleLinesEx(borderRect, 5, borderColor);
}

void DrawUI(GameState* gameState)
{
    const int fontSize = 40;
    const int yPosition = 20;

    const char* currentScoreText = TextFormat("Score: %i", gameState->score);
    const char* highScoreText = TextFormat("High Score: %i", gameState->highScore);
    int highScoreTextWidth = MeasureText(highScoreText, fontSize);

    int leftEdge = GAME_OFFSET - 5;
    int rightEdge = GAME_OFFSET + GAME_WIDTH + 5; // Right edge of game area plus margin
    int rightAlignedX = rightEdge - highScoreTextWidth;

    DrawText(currentScoreText, leftEdge, yPosition, fontSize, RAYWHITE);
    DrawText(highScoreText, rightAlignedX, yPosition, fontSize, RAYWHITE);
}

void DrawSnake(GameState* gameState)
{
    for (int i = 1; i < gameState->snake.length; i++) 
    {
        int pixelX = GAME_OFFSET + (int)(gameState->snake.bodyPart[i].position.x * TILE_SIZE);
        int pixelY = GAME_OFFSET + (int)(gameState->snake.bodyPart[i].position.y * TILE_SIZE);

        //DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, snakeColour);
        Rectangle headPos = { pixelX, pixelY, TILE_SIZE, TILE_SIZE };
        DrawRectangleRounded(headPos, 0.5, 6, snakeColour);
    }

    int headX = GAME_OFFSET + (int)(gameState->snake.bodyPart[0].position.x * TILE_SIZE);
    int headY = GAME_OFFSET + (int)(gameState->snake.bodyPart[0].position.y * TILE_SIZE);

    //DrawRectangle(headX, headY, TILE_SIZE, TILE_SIZE, snakeColour);
    Rectangle headPos = { headX, headY, TILE_SIZE, TILE_SIZE};
    DrawRectangleRounded(headPos, 0.5, 6, snakeColour);
}

void DrawFood(GameState* gameState)
{
    int pixelX = GAME_OFFSET + (int)(gameState->food.position.x * TILE_SIZE);
    int pixelY = GAME_OFFSET + (int)(gameState->food.position.y * TILE_SIZE);

    DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, GREEN);
}