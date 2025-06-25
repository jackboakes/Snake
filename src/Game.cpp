#include <raylib.h>
#include "Game.h"
#include "Input.h"
#include "Score.h"


void InitDirectionQueue(DirectionQueue* queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
}

bool DirectionQueueEmpty(DirectionQueue* queue)
{
    return (queue->count == 0);
}

bool DirectionQueueFull(DirectionQueue* queue)
{

    return (queue->count >= INPUT_QUEUE_SIZE);
}

bool EnqueueDirection(DirectionQueue* queue, Direction dir)
{
    if (DirectionQueueFull(queue))
    {
        return 0;
    }
    queue->dirValues[queue->tail] = dir;
    queue->count++;
    queue->tail = (queue->tail + 1) % INPUT_QUEUE_SIZE; // wrap

    return 1;
}

Direction DequeueDirection(DirectionQueue* queue)
{
    Direction result;

    if (DirectionQueueEmpty(queue))
    {
        return DIR_NONE;
    }

    result = queue->dirValues[queue->head];
    queue->head = (queue->head + 1) % INPUT_QUEUE_SIZE; // wrap
    queue->count--;

    return result;
}

Direction GetNextDirection(DirectionQueue* queue)
{
    if (DirectionQueueEmpty(queue))
    {
        return DIR_NONE;
    }

    return queue->dirValues[queue->head];
}

Vector2 DirectionToVector(Direction dir)
{
    switch (dir)
    {
        Vector2 direction;
        case DIR_NORTH: return direction = { 0, -1 };
        case DIR_SOUTH: return direction = { 0, 1 };
        case DIR_EAST: return direction = { 1, 0 };
        case DIR_WEST: return direction = { -1, 0 };
        default: return direction = { 0, 0 };
    }
}

bool IsOppositeDirection(Direction dir1, Direction dir2)
{
    return (dir1 == DIR_NORTH && dir2 == DIR_SOUTH) ||
        (dir1 == DIR_SOUTH && dir2 == DIR_NORTH) ||
        (dir1 == DIR_WEST && dir2 == DIR_EAST) ||
        (dir1 == DIR_EAST && dir2 == DIR_WEST);
}

void InitGame(GameState* gameState)
{
    gameState->isGameOver = 0;
    gameState->score = 0;
    gameState->highScore = LoadHighScore();
    InitSnake(&gameState->snake);
    UpdateFood(gameState);
}

void UpdateGame(GameState* gameState, float deltaTime)
{
    if (!gameState->isGameOver)
    {
        UpdateSnake(&gameState->snake, deltaTime);
        GameLogic(gameState);
    }
}

void InitSnake(Snake* snake)
{
    const int initSnakeSize = 4;

    // Snake spawns left and centre of game area
    const int leftX = initSnakeSize + 1;
    const int halfY = GRID_SIZE / 2;
    Vector2 startPos = { leftX,  halfY };

    snake->length = initSnakeSize;
    snake->currentDirection = DIR_EAST;
    snake->moveSpeed = 10.0f;  
    snake->moveTimer = 0.0f;

    InitDirectionQueue(&snake->directionQueue);

    snake->bodyPart[0].position.x = startPos.x;
    snake->bodyPart[0].position.y = startPos.y;

    for (int i = 0; i < initSnakeSize; i++)
    {
        snake->bodyPart[i].position.x = leftX - i;
        snake->bodyPart[i].position.y = halfY;
    }
}

void UpdateSnake(Snake* snake, float deltaTime)
{

    snake->moveTimer += deltaTime; // Accumulate time

    float moveInterval = 1.0f / snake->moveSpeed;
    if (snake->moveTimer >= moveInterval)
    {
        // Reset timer (keep any excess time for precision)
        snake->moveTimer -= moveInterval;

        Direction nextDirection = DequeueDirection(&snake->directionQueue);

        if (nextDirection != DIR_NONE)
        {
            snake->currentDirection = nextDirection;
        }

        for (int i = snake->length - 1; i > 0; i--)
        {
            snake->bodyPart[i] = snake->bodyPart[i - 1];
        }

        Vector2 directionVector = DirectionToVector(snake->currentDirection);
        snake->bodyPart[0].position.x += directionVector.x;
        snake->bodyPart[0].position.y += directionVector.y;
    }
}

void UpdateFood(GameState* gameState)
{
    int collision = 0;

    do
    {
        Vector2 newFoodPos;

        // raylibs getrandom value
        newFoodPos.x = GetRandomValue(1, GRID_SIZE - 2);  
        newFoodPos.y = GetRandomValue(1, GRID_SIZE - 2);

        collision = 0;
        //check for open space to spawn food
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

void GameLogic(GameState* gameState)
{
    // when eat food grow snake
    if (gameState->food.position.x == gameState->snake.bodyPart[0].position.x &&
        gameState->food.position.y == gameState->snake.bodyPart[0].position.y)
    {
        Vector2 directionVector = DirectionToVector(gameState->snake.currentDirection);


        // initialise position of newly created tail based on direction
        gameState->snake.bodyPart[gameState->snake.length].position.x =
            gameState->snake.bodyPart[gameState->snake.length - 1].position.x - directionVector.x;
        gameState->snake.bodyPart[gameState->snake.length].position.y =
            gameState->snake.bodyPart[gameState->snake.length - 1].position.y - directionVector.y;

        gameState->snake.length++;
        gameState->score += 5;
        UpdateFood(gameState);
    }

    // Check wall collision
    if (gameState->snake.bodyPart[0].position.x == 0 || gameState->snake.bodyPart[0].position.x == GRID_SIZE - 1
        || gameState->snake.bodyPart[0].position.y == 0 || gameState->snake.bodyPart[0].position.y == GRID_SIZE - 1)
    {
        CheckAndUpdateHighScore(gameState->score, &gameState->highScore);
        gameState->isGameOver = 1;
        return;
    }

    // Check snake collision
    for (int i = 1; i < gameState->snake.length; i++)
    {
        if (gameState->snake.bodyPart[0].position.x == gameState->snake.bodyPart[i].position.x &&
            gameState->snake.bodyPart[0].position.y == gameState->snake.bodyPart[i].position.y)
        {
            CheckAndUpdateHighScore(gameState->score, &gameState->highScore);
            gameState->isGameOver = 1;
            return;
        }
    }

}

void HandleInput(Snake* snake, InputAction input)
{
    Direction newDirection = DIR_NONE;

    switch (input)
    {
        case INPUT_UP: newDirection = DIR_NORTH; break;
        case INPUT_LEFT: newDirection = DIR_WEST; break;
        case INPUT_DOWN: newDirection = DIR_SOUTH; break;
        case INPUT_RIGHT: newDirection = DIR_EAST; break;
        //case INPUT_QUIT: ; break;
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
