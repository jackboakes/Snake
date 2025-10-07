#pragma once

#include <raylib.h>
#include "Game.h"
#include "UI.h"
#include "Audio.h"

#define SCREEN_WIDTH  (GAME_WIDTH + (2 * GAME_OFFSET)) 
#define SCREEN_HEIGHT (GAME_HEIGHT + (2 * GAME_OFFSET))

class GameManager
{
public:

private:
    enum class State
    {
        STATE_MAIN_MENU,
        STATE_PLAYING,
        STATE_GAME_OVER,
        STATE_QUIT
    };

    GameState m_gameState;
    State m_currentState;
    State m_nextState;
    bool m_shouldQuit;
    // UI Data
    UI m_mainMenuUI;
    UI m_gameOverUI;

    // Audio
    Sound m_audioSFX[SFX_COUNT];

public:
    void InitGameManager();
    void ShutdownGameManager();
    void RunGameManager();
private:
    void UpdateMainMenu();
    void UpdateGameplay();
    void UpdateGameOver();
    void SetGameManagerState(State newState);
};







