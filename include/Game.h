#pragma once

#include "Input.h"
#include "GameData.h"
#include "Snake.h"
#include <vector>

#define TILE_SIZE 32
#define GRID_SIZE 20
#define BORDER_THICKNESS 5
#define GAME_OFFSET ((TILE_SIZE * 2) + BORDER_THICKNESS) // 2 TILES + OFFSET FOR BEVELED BORDER
#define GAME_WIDTH (GRID_SIZE * TILE_SIZE) // 20 X 32
#define GAME_HEIGHT (GRID_SIZE * TILE_SIZE) // 20 X 32

#define SCORE_INCREMENT 5


struct Food
{
    GridPosition position;
};

struct GameState
{
    Snake snake;
    float g_moveTimer { 0.0f };
    float g_moveInterval;
    Food food;
    int isGameOver { false };
    int score { 0 };
    int highScore { 0 };
    DirectionQueue directionQueue;
};


// Game logic functions
void InitGame(GameState& gameState);
void HandleInput(GameState& gameState, InputAction input);
void UpdateGame(GameState& gameState, float deltaTime, Sound eatSound, Sound collisionSound);


