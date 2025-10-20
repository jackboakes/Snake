#include "GameState.h"
#include "raylib.h"


bool GameState::IsOppositeDirection(Direction dir1, Direction dir2)
{
    return (dir1 == Direction::NORTH && dir2 == Direction::SOUTH) ||
        (dir1 == Direction::SOUTH && dir2 == Direction::NORTH) ||
        (dir1 == Direction::WEST && dir2 == Direction::EAST) ||
        (dir1 == Direction::EAST && dir2 == Direction::WEST);
}

bool GameState::CheckWallCollision(const Snake& snake)
{
    const GridPosition head { snake.bodyPart[0] };

    return (head.x == 0 || head.x == GRID_SIZE - 1 ||
        head.y == 0 || head.y == GRID_SIZE - 1);
}

bool GameState::CheckSelfCollision(const Snake& snake)
{
    const GridPosition head { snake.bodyPart[0] };

    for (int i { 1 }; i < snake.bodyPart.size(); i++) {
        if (head.x == snake.bodyPart[i].x &&
            head.y == snake.bodyPart[i].y) {
            return true;
        }
    }
    return false;
}


void GameState::HandleFoodEat(Audio& audio)
{
    audio.PlaySoundRandomisedPitch(Audio::SFXID::EAT);
    snake.Grow();
    score += 5;
    food.Spawn(snake);
}

void GameState::HandleGameOver(Audio& audio)
{
    audio.PlaySoundRandomisedPitch(Audio::SFXID::COLLISION);
    m_score.CheckAndUpdateHighScore(score, highScore);
    isGameOver = true;
}
void GameState::GameLogic(Audio& audio)
{
    if (food.CheckCollision(snake))
    {
        HandleFoodEat(audio);
    }

    // game over if collide with wall or snakes self
    if (CheckWallCollision(snake) ||
        CheckSelfCollision(snake))
    {
        HandleGameOver(audio);
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

    m_moveInterval = 1.0f / snake.moveSpeed;
    m_moveTimer = 0.0f;
    m_directionQueue = {};

    food.Spawn(snake);
}

void GameState::HandleInput(InputAction input)
{
    Direction newDirection { Direction::NONE };

    switch (input)
    {
    case InputAction::UP: 
        newDirection = Direction::NORTH; 
        break;
    case InputAction::LEFT: 
        newDirection = Direction::WEST; 
        break;
    case InputAction::DOWN: 
        newDirection = Direction::SOUTH; 
        break;
    case InputAction::RIGHT: 
        newDirection = Direction::EAST; 
        break;
    case InputAction::NONE: 
        return;
    }

    Direction checkDirection { snake.currentDirection };
    if (!m_directionQueue.empty())
    {
        // If there are queued inputs, check against the last one
        checkDirection = m_directionQueue.back();
    }

    // Only queue the input if it's not opposite to the current/queued direction
    if (!IsOppositeDirection(newDirection, checkDirection))
    {
        // pop if we are max size 
        if (m_directionQueue.size() == INPUT_QUEUE_SIZE)
        {
            m_directionQueue.pop();

        }
        m_directionQueue.push(newDirection);
    }
}

void GameState::UpdateGame(float deltaTime, Audio& audio)
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
        Direction nextDirection { Direction::NONE };
        if (!m_directionQueue.empty())
        {
            nextDirection = m_directionQueue.front(); 
            m_directionQueue.pop();
        }

        // Inject the direction into the snake. The snake will update itself.
        snake.Update(nextDirection);
    }

        GameLogic(audio);
}


