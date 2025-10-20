#pragma once
#include <raylib.h>
#include "GameState.h"
#include "Audio.h"
#include "UI.h"

class GameManager
{
public:
    GameManager();
    ~GameManager();
    void Run();
private:
    enum class State
    {
        STATE_MAIN_MENU,
        STATE_PLAYING,
        STATE_GAME_OVER,
        STATE_QUIT
    };

    GameState m_gameState;
    State m_currentState { State::STATE_MAIN_MENU };
    State m_nextState { State::STATE_MAIN_MENU };
    bool m_shouldQuit { false };

    UI m_mainMenuUI {};
    UI m_gameOverUI {};

    Audio m_audio;

    void UpdateMainMenu();
    void UpdateGameplay();
    void UpdateGameOver();
    void SetGameManagerState(State newState);
};







