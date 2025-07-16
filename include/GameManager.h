#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <raylib.h>
#include "Types.h"
#include "Game.h"
#include "UI.h"

// Used to control states
struct GameManager
{
    GameStateID currentState;
    GameStateID nextState;
    bool shouldQuit;

    // Game Data
    GameState gameState;

    // UI Data
    UI mainMenuUI;
    UI gameOverUI;
};


void InitGameManager(GameManager* gameManager);
void ShutdownGameManager(GameManager* gameManager);
void RunGameManager(GameManager* gameManager);

// State management
static void SetGameManagerState(GameManager* gameManager, GameStateID newStateID);

// State update functions - each is a complete input, update, render cycle
static void UpdateMainMenu(GameManager* gameManager);
static void UpdateGameplay(GameManager* gameManager);
static void UpdateGameOver(GameManager* gameManager);

#endif 
