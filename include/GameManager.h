#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <raylib.h>
#include "Types.h"
#include "Game.h"
#include "Input.h"
#include "Score.h" 

//struct GameState;

// Main controller struct
struct GameManager
{
    GameStateID currentStateID;
    GameStateID nextStateID;
    GameState gameState;

    bool shouldQuit;
    int selectedMenuOption;
};

void InitGameManager(GameManager* gameManager);
void RunGameManager(GameManager* gameManager);
void SetGameManagerState(GameManager* gameManager, GameStateID newStateID);
void ShutdownGameManager(GameManager* gameManager);

// State functions
void StateMainMenu(GameManager* gameManager);
void StateGameplay(GameManager* gameManager);
void StateGameOver(GameManager* gameManager);

#endif 
