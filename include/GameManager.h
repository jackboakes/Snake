#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <raylib.h>
#include "Types.h"
#include "Game.h"



// Used to control states
struct GameManager
{
    GameStateID currentState;
    GameStateID nextState;
    bool shouldQuit;
    int selectedMenuOption;

    // Game Data
    GameState gameState;
};


void InitGameManager(GameManager* gameManager);
void ShutdownGameManager(GameManager* gameManager);
void RunGameManager(GameManager* gameManager);

// State management
void SetGameManagerState(GameManager* gameManager, GameStateID newStateID);

// State update functions - each is a complete input, update, render cycle
void UpdateMainMenu(GameManager* gameManager);
void UpdateGameplay(GameManager* gameManager);
void UpdateGameOver(GameManager* gameManager);

#endif 
