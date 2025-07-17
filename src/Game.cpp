#include "raylib.h"
#include "Game.h"
#include "Input.h"
#include "Score.h"
#include "Audio.h"



static GridPosition DirectionToGridOffset(Direction dir)
{
    switch (dir)
    {
    case DIR_NORTH: return { 0, -1 };
    case DIR_SOUTH: return { 0, 1 };
    case DIR_EAST:  return { 1, 0 };
    case DIR_WEST:  return { -1, 0 };
    default:        return { 0, 0 };
    }
}

static bool IsOppositeDirection(Direction dir1, Direction dir2)
{
    return (dir1 == DIR_NORTH && dir2 == DIR_SOUTH) ||
        (dir1 == DIR_SOUTH && dir2 == DIR_NORTH) ||
        (dir1 == DIR_WEST && dir2 == DIR_EAST) ||
        (dir1 == DIR_EAST && dir2 == DIR_WEST);
}

// Snake spawns left and centre of game grid at 5, 10
static void InitSnake(Snake* snake)
{
    const int initSnakeSize = 4;
    snake->currentDirection = DIR_EAST;
    const int leftX = initSnakeSize + 1;
    const int halfY = GRID_SIZE / 2;
    snake->moveSpeed = 10.0f; // sets the speed of the snake to 10ms 
    snake->moveTimer = 0.0f;
    snake->moveInterval = 1.0f / snake->moveSpeed; // calc the time between moves based on snakes speed
    snake->length = initSnakeSize;

    InitDirectionQueue(&snake->directionQueue);


    for (int i = 0; i < initSnakeSize; i++)
    {
        snake->bodyPart[i].position.x = leftX - i;
        snake->bodyPart[i].position.y = halfY;
    }
}

// Moves the snake independent of frame rate
static void UpdateSnake(Snake* snake, float deltaTime)
{
    // Accumulate time since last move
    snake->moveTimer += deltaTime; 

    if (snake->moveTimer >= snake->moveInterval)
    {
        // Reset timer (carry any time over to the next frame for precesion)
        snake->moveTimer -= snake->moveInterval;

        Direction nextDirection = DequeueDirection(&snake->directionQueue);

        if (nextDirection != DIR_NONE)
        {
            snake->currentDirection = nextDirection;
        }

        // move snake body by shifting each segment to next position
        for (int i = snake->length - 1; i > 0; i--)
        {
            snake->bodyPart[i] = snake->bodyPart[i - 1];
        }

        // update head in current direction 
        GridPosition dirOnGrid = DirectionToGridOffset(snake->currentDirection);
        snake->bodyPart[0].position.x += dirOnGrid.x;
        snake->bodyPart[0].position.y += dirOnGrid.y;
    }
}

static void GrowSnake(Snake* snake) 
{
    if (snake->length >= SNAKE_MAX_LEN) return;
    int currentTailIndex = snake->length - 1;
    int newTailIndex = snake->length;

    // duplicate the current tail position
    snake->bodyPart[newTailIndex].position.x = snake->bodyPart[currentTailIndex].position.x;
    snake->bodyPart[newTailIndex].position.y = snake->bodyPart[currentTailIndex].position.y;

    snake->length++;
}

void HandleSnakeInput(Snake* snake, InputAction input)
{
    Direction newDirection = DIR_NONE;

    switch (input)
    {
    case INPUT_UP: newDirection = DIR_NORTH; break;
    case INPUT_LEFT: newDirection = DIR_WEST; break;
    case INPUT_DOWN: newDirection = DIR_SOUTH; break;
    case INPUT_RIGHT: newDirection = DIR_EAST; break;
    case INPUT_NONE: return;
    }

    Direction checkDirection = snake->currentDirection;
    if (!DirectionQueueEmpty(&snake->directionQueue))
    {
        // If there are queued inputs, check against the last one
        int lastIndex = (snake->directionQueue.tail - 1 + INPUT_QUEUE_SIZE) % INPUT_QUEUE_SIZE;
        checkDirection = snake->directionQueue.dirValues[lastIndex];
    }

    // Only queue the input if it's not opposite to the current/queued direction
    if (!IsOppositeDirection(newDirection, checkDirection))
    {
        EnqueueDirection(&snake->directionQueue, newDirection);
    }
}

static bool CheckWallCollision(const Snake* snake)
{
    const GridPosition head = snake->bodyPart[0].position;

    return (head.x == 0 || head.x == GRID_SIZE - 1 ||
        head.y == 0 || head.y == GRID_SIZE - 1);
}

static bool CheckSelfCollision(const Snake* snake)
{
    const GridPosition head = snake->bodyPart[0].position;

    for (int i = 1; i < snake->length; i++) {
        if (head.x == snake->bodyPart[i].position.x &&
            head.y == snake->bodyPart[i].position.y) {
            return true;
        }
    }
    return false;
}

static void UpdateFood(GameState* gameState)
{
    int collision = 0;

    do
    {
        GridPosition newFoodPos = { 0 };

        // Raylibs getrandom value
        newFoodPos.x = GetRandomValue(1, GRID_SIZE - 2);  
        newFoodPos.y = GetRandomValue(1, GRID_SIZE - 2);

        collision = 0;
        // Check for open space to spawn food
        for (int i = 0; i < gameState->snake.length; i++)
        {
            if (gameState->snake.bodyPart[i].position.x == newFoodPos.x &&
                gameState->snake.bodyPart[i].position.y == newFoodPos.y)
            {
                collision = 1;
                break;
            }
        }

        if (!collision)
        {
            gameState->food.position.x = newFoodPos.x;
            gameState->food.position.y = newFoodPos.y;
        }
    } while (collision);
}

static bool CheckFoodCollision(const Snake* snake, const Food* food)
{
    return (food->position.x == snake->bodyPart[0].position.x &&
        food->position.y == snake->bodyPart[0].position.y);
}

static void HandleFoodEat(GameState* gameState, Sound eatSound) 
{
    PlaySoundRandomisedPitch(eatSound);
    GrowSnake(&gameState->snake);
    gameState->score += 5;
    UpdateFood(gameState);
}

static void HandleGameOver(GameState* gameState, Sound collisionSound)
{
    PlaySoundRandomisedPitch(collisionSound);
    CheckAndUpdateHighScore(gameState->score, &gameState->highScore);
    gameState->isGameOver = true;
}

static void GameLogic(GameState* gameState, Sound eatSound, Sound collisionSound)
{
    if (CheckFoodCollision(&gameState->snake, &gameState->food)) {
        HandleFoodEat(gameState, eatSound);
    }

    // game over if collide with wall or snakes self
    if (CheckWallCollision(&gameState->snake) ||
        CheckSelfCollision(&gameState->snake))
    {
        HandleGameOver(gameState, collisionSound);
    }
}

void InitGame(GameState* gameState)
{
    gameState->isGameOver = 0;
    gameState->score = 0;
    gameState->highScore = LoadHighScore();
    InitSnake(&gameState->snake);
    UpdateFood(gameState);
}

void UpdateGame(GameState* gameState, float deltaTime, Sound eatSound, Sound collisonSound)
{
    if (!gameState->isGameOver)
    {
        UpdateSnake(&gameState->snake, deltaTime);
        GameLogic(gameState, eatSound, collisonSound);
    }
}


