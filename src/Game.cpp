#include "raylib.h"
#include "Game.h"
#include "Input.h"
#include "Score.h"
#include "Audio.h"


static bool IsOppositeDirection(Direction dir1, Direction dir2)
{
    return (dir1 == DIR_NORTH && dir2 == DIR_SOUTH) ||
        (dir1 == DIR_SOUTH && dir2 == DIR_NORTH) ||
        (dir1 == DIR_WEST && dir2 == DIR_EAST) ||
        (dir1 == DIR_EAST && dir2 == DIR_WEST);
}

void HandleInput(GameState& gameState, InputAction input)
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

    Direction checkDirection = gameState.snake.g_currentDirection;
    if (!DirectionQueueEmpty(&gameState.directionQueue))
    {
        // If there are queued inputs, check against the last one
        int lastIndex = (gameState.directionQueue.tail - 1 + INPUT_QUEUE_SIZE) % INPUT_QUEUE_SIZE;
        checkDirection = gameState.directionQueue.dirValues[lastIndex];
    }

    // Only queue the input if it's not opposite to the current/queued direction
    if (!IsOppositeDirection(newDirection, checkDirection))
    {
        EnqueueDirection(&gameState.directionQueue, newDirection);
    }
}

static bool CheckWallCollision(const Snake& snake)
{
    const GridPosition head = snake.g_bodyPart[0];

    return (head.x == 0 || head.x == GRID_SIZE - 1 ||
        head.y == 0 || head.y == GRID_SIZE - 1);
}

static bool CheckSelfCollision(const Snake& snake)
{
    const GridPosition head = snake.g_bodyPart[0];

    for (int i = 1; i < snake.g_bodyPart.size(); i++) {
        if (head.x == snake.g_bodyPart[i].x &&
            head.y == snake.g_bodyPart[i].y) {
            return true;
        }
    }
    return false;
}

static void UpdateFood(GameState& gameState)
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
        for (int i = 0; i < gameState.snake.g_bodyPart.size(); i++)
        {
            if (gameState.snake.g_bodyPart[i].x == newFoodPos.x &&
                gameState.snake.g_bodyPart[i].y == newFoodPos.y)
            {
                collision = 1;
                break;
            }
        }

        if (!collision)
        {
            gameState.food.position.x = newFoodPos.x;
            gameState.food.position.y = newFoodPos.y;
        }
    } while (collision);
}

static bool CheckFoodCollision(const Snake& snake, const Food& food)
{
    return (food.position.x == snake.g_bodyPart[0].x &&
        food.position.y == snake.g_bodyPart[0].y);
}

static void HandleFoodEat(GameState& gameState, Sound eatSound) 
{
    PlaySoundRandomisedPitch(eatSound);
    gameState.snake.Grow();
    gameState.score += 5;
    UpdateFood(gameState);
}

static void HandleGameOver(GameState& gameState, Sound collisionSound)
{
    PlaySoundRandomisedPitch(collisionSound);
    CheckAndUpdateHighScore(gameState.score, &gameState.highScore);
    gameState.isGameOver = true;
}

static void GameLogic(GameState& gameState, Sound eatSound, Sound collisionSound)
{
    if (CheckFoodCollision(gameState.snake, gameState.food)) {
        HandleFoodEat(gameState, eatSound);
    }

    // game over if collide with wall or snakes self
    if (CheckWallCollision(gameState.snake) ||
        CheckSelfCollision(gameState.snake))
    {
        HandleGameOver(gameState, collisionSound);
    }
}

void InitGame(GameState& gameState)
{
    gameState.isGameOver = false;
    gameState.score = 0;
    gameState.highScore = LoadHighScore();
    gameState.snake = Snake();
    gameState.g_moveInterval = 1.0f / gameState.snake.g_moveSpeed;
    InitDirectionQueue(&gameState.directionQueue);
    UpdateFood(gameState);
}

void UpdateGame(GameState& gameState, float deltaTime, Sound eatSound, Sound collisonSound)
{
    if (gameState.isGameOver)
    {
        return;
    }

    gameState.g_moveTimer += deltaTime;

    if (gameState.g_moveTimer >= gameState.g_moveInterval)
    {
        // Reset timer
        gameState.g_moveTimer -= gameState.g_moveInterval;

        // Get the next direction from the queue managed by GameState
        Direction nextDirection = DequeueDirection(&gameState.directionQueue);

        // Inject the direction into the snake. The snake will update itself.
        gameState.snake.Update(nextDirection);
    }

        GameLogic(gameState, eatSound, collisonSound);
}


