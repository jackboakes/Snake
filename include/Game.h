#ifndef GAME_H
#define GAME_H

#include "Input.h"
#include "Types.h"

#define TILE_SIZE 32
#define GRID_SIZE 20
#define GAME_OFFSET ((TILE_SIZE * 2) + 5)
#define GAME_WIDTH (GRID_SIZE * TILE_SIZE)
#define GAME_HEIGHT (GRID_SIZE * TILE_SIZE)

#define SNAKE_MAX_LEN 256
#define INPUT_QUEUE_SIZE 2
#define SCORE_INCREMENT 5

struct GameManager;

// Allows refering to grid position instead of screen space
struct GridPosition
{
    int x;
    int y;
};

struct DirectionQueue
{
    Direction dirValues[INPUT_QUEUE_SIZE];
    int head;
    int tail;
    int count;
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

    // Audio resources | needs to be vector to be scaleable
    Sound eatSound;
    Sound collisionSound;
};


void InitDirectionQueue(DirectionQueue* queue);
bool DirectionQueueEmpty(DirectionQueue* queue);
bool DirectionQueueFull(DirectionQueue* queue);
bool EnqueueDirection(DirectionQueue* queue, Direction dir);
Direction DequeueDirection(DirectionQueue* queue);
Direction GetNextDirection(DirectionQueue* queue);

// Helper functions
static GridPosition DirectionToGridOffset(Direction dir);
static bool IsOppositeDirection(Direction dir1, Direction dir2);

// Game logic functions
void InitGame(GameState* gameState);
void UpdateGame(GameState* gameState, float deltaTime);
static void GameLogic(GameState* gameState);
static void HandleGameOver(GameState* gameState);

// Snake functions
static void InitSnake(Snake* snake);
static void UpdateSnake(Snake* snake, float deltaTime);
static void GrowSnake(Snake* snake);
void HandleSnakeInput(Snake* snake, InputAction input);
static bool CheckWallCollison(const Snake* snake);
static bool CheckSelfCollision(const Snake* snake);

// Food functions
static void UpdateFood(GameState* gameState);
static bool CheckFoodCollision(const Snake* snake, const Food* food);
static void HandleFoodEat(GameState* gameState);



#endif
