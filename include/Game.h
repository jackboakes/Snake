#ifndef GAME_H
#define GAME_H

#include "Input.h"

#define TILE_SIZE 32
#define GRID_SIZE 20
#define BORDER_THICKNESS 5
#define GAME_OFFSET ((TILE_SIZE * 2) + BORDER_THICKNESS) // 2 TILES + OFFSET FOR BEVELED BORDER
#define GAME_WIDTH (GRID_SIZE * TILE_SIZE) // 20 X 32
#define GAME_HEIGHT (GRID_SIZE * TILE_SIZE) // 20 X 32


#define SNAKE_MAX_LEN 256
#define SCORE_INCREMENT 5

struct GameManager;

// Allows refering to grid position instead of screen space
struct GridPosition
{
    int x;
    int y;
};


struct Food
{
    GridPosition position;
};

struct SnakePart
{
    GridPosition position;
};

struct Snake
{
    int length;
    SnakePart bodyPart[SNAKE_MAX_LEN];
    Direction currentDirection;
    DirectionQueue directionQueue;
    float moveSpeed;
    float moveTimer;
    float moveInterval;
};


struct GameState
{
    Snake snake;
    Food food;
    int isGameOver;
    int score;
    int highScore;
};


// Game logic functions
void InitGame(GameState* gameState);
void UpdateGame(GameState* gameState, float deltaTime, Sound eatSound, Sound collisionSound);

// Snake functions
void HandleSnakeInput(Snake* snake, InputAction input);





#endif
