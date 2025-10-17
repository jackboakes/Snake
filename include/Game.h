#pragma once


#include "Input.h"
#include "GameData.h"
#include "Score.h"
#include "Snake.h"
#include "Audio.h"
#include "raylib.h"
#include <vector>



struct Food
{
    GridPosition position;
};

class GameState
{
public:
    Snake snake;
    Food food;
    int score { 0 };
    int highScore { 0 };
    bool isGameOver { false };

private:
    DirectionQueue m_directionQueue;
    float m_moveTimer { 0.0f };
    float m_moveInterval;
    Score m_score;
public:
    GameState();

    void Reset();
    void HandleInput(InputAction input);
    void UpdateGame(float deltaTime, Sound eatSound, Sound collisionSound);
private:

    bool IsOppositeDirection(Direction dir1, Direction dir2);

    bool CheckWallCollision(const Snake& snake);
    bool CheckSelfCollision(const Snake& snake);

    // TODO: Move this to a Food class?
    void GameState::UpdateFood();
    bool CheckFoodCollision(const Snake& snake, const Food& food);
    void HandleFoodEat(Sound eatSound);

    void HandleGameOver(Sound collisionSound);
    void GameLogic(Sound eatSound, Sound collisionSound);

};


