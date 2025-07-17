#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <raylib.h>
#include "Game.h"
#include "UI.h"
#include "Audio.h"


#define SCREEN_WIDTH  (GAME_WIDTH + (2 * GAME_OFFSET)) 
#define SCREEN_HEIGHT (GAME_HEIGHT + (2 * GAME_OFFSET))

enum GameStateID
{
    STATE_MAIN_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_QUIT
};

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

    // Audio
    Sound audioSFX[SFX_COUNT];
};


void InitGameManager(GameManager* gameManager);
void ShutdownGameManager(GameManager* gameManager);
void RunGameManager(GameManager* gameManager);

#endif 
