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
};


struct GameState
{
    Snake snake;
    Food food;
    int isGameOver;
    int score;
    int highScore;
};



// Helper functions
static GridPosition DirectionToGridOffset(Direction dir);
static bool IsOppositeDirection(Direction dir1, Direction dir2);

// Game logic functions
void InitGame(GameState* gameState);
void UpdateGame(GameState* gameState, float deltaTime, Sound eatSound, Sound collisonSound);
static void GameLogic(GameState* gameState, Sound eatSound, Sound collisionSound);
static void HandleGameOver(GameState* gameState, Sound collisionSound);

// Snake functions
static void InitSnake(Snake* snake);
static void UpdateSnake(Snake* snake, float deltaTime);
static void GrowSnake(Snake* snake);
void HandleSnakeInput(Snake* snake, InputAction input);
static bool CheckWallCollision(const Snake* snake);
static bool CheckSelfCollision(const Snake* snake);

// Food functions
static void UpdateFood(GameState* gameState);
static bool CheckFoodCollision(const Snake* snake, const Food* food);
static void HandleFoodEat(GameState* gameState, Sound eatSound);



#endif
