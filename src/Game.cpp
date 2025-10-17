#include "Game.h"
#include "Audio.h"
#include "raylib.h"

bool GameState::IsOppositeDirection(Direction dir1, Direction dir2)
{
    return (dir1 == DIR_NORTH && dir2 == DIR_SOUTH) ||
        (dir1 == DIR_SOUTH && dir2 == DIR_NORTH) ||
        (dir1 == DIR_WEST && dir2 == DIR_EAST) ||
        (dir1 == DIR_EAST && dir2 == DIR_WEST);
}

bool GameState::CheckWallCollision(const Snake& snake)
{
    const GridPosition head = snake.g_bodyPart[0];

    return (head.x == 0 || head.x == GRID_SIZE - 1 ||
        head.y == 0 || head.y == GRID_SIZE - 1);
}

bool GameState::CheckSelfCollision(const Snake& snake)
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

void GameState::UpdateFood()
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
        for (int i = 0; i < snake.g_bodyPart.size(); i++)
        {
            if (snake.g_bodyPart[i].x == newFoodPos.x &&
                snake.g_bodyPart[i].y == newFoodPos.y)
            {
                collision = 1;
                break;
            }
        }

        if (!collision)
        {
            food.position.x = newFoodPos.x;
            food.position.y = newFoodPos.y;
        }
    } while (collision);
}

bool GameState::CheckFoodCollision(const Snake& snake, const Food& food)
{
    return (food.position.x == snake.g_bodyPart[0].x &&
        food.position.y == snake.g_bodyPart[0].y);
}

void GameState::HandleFoodEat(Sound eatSound) 
{
    PlaySoundRandomisedPitch(eatSound);
    snake.Grow();
    score += 5;
    UpdateFood();
}

void GameState::HandleGameOver(Sound collisionSound)
{
    PlaySoundRandomisedPitch(collisionSound);
    m_score.CheckAndUpdateHighScore(score, &highScore);
    isGameOver = true;
}
void GameState::GameLogic(Sound eatSound, Sound collisionSound)
{
    if (CheckFoodCollision(snake, food))
    {
        HandleFoodEat(eatSound);
    }

    // game over if collide with wall or snakes self
    if (CheckWallCollision(snake) ||
        CheckSelfCollision(snake))
    {
        HandleGameOver(collisionSound);
    }
}

GameState::GameState()
{
    highScore = m_score.LoadHighScore();
    Reset();
}

void GameState::Reset()
{
    isGameOver = false;
    score = 0;
    snake = Snake();

    m_moveInterval = 1.0f / snake.g_moveSpeed;
    m_moveTimer = 0.0f;
    InitDirectionQueue(&m_directionQueue);

    UpdateFood();
}

void GameState::HandleInput(InputAction input)
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

    Direction checkDirection = snake.g_currentDirection;
    if (!DirectionQueueEmpty(&m_directionQueue))
    {
        // If there are queued inputs, check against the last one
        int lastIndex = (m_directionQueue.tail - 1 + INPUT_QUEUE_SIZE) % INPUT_QUEUE_SIZE;
        checkDirection = m_directionQueue.dirValues[lastIndex];
    }

    // Only queue the input if it's not opposite to the current/queued direction
    if (!IsOppositeDirection(newDirection, checkDirection))
    {
        EnqueueDirection(&m_directionQueue, newDirection);
    }
}


void GameState::UpdateGame(float deltaTime, Sound eatSound, Sound collisonSound)
{
    if (isGameOver)
    {
        return;
    }

    m_moveTimer += deltaTime;

    if (m_moveTimer >= m_moveInterval)
    {
        // Reset timer
        m_moveTimer -= m_moveInterval;

        // Get the next direction from the queue managed by GameState
        Direction nextDirection = DequeueDirection(&m_directionQueue);

        // Inject the direction into the snake. The snake will update itself.
        snake.Update(nextDirection);
    }

        GameLogic(eatSound, collisonSound);
}


