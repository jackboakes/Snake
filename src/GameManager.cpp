#include "raylib.h"
#include "GameManager.h"
#include "Input.h"
#include "Renderer.h"
#include "Audio.h"
#include "Assets.h"
#include "UI.h"


GameManager::GameManager()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);
    InitAudio(m_audioSFX);
    LoadGameTextures();
    UpdateWindowIcon(TILE_SIZE);
    // init main menu since its first thing loaded
    InitMainMenuUI(&m_mainMenuUI);
}

GameManager::~GameManager()
{
    UnloadGameTextures();
    ShutdownAudio(m_audioSFX);
    CloseWindow();
}

void GameManager::UpdateMainMenu()
{
    // data
    UpdateUI(&m_mainMenuUI, m_audioSFX[SFX_MENU_BUTTON]);
    
    // logic
    if (WasActiveButtonReleased(&m_mainMenuUI, MAIN_MENU_START))
    {
        m_nextState = State::STATE_PLAYING;
    }

    if (WasActiveButtonReleased(&m_mainMenuUI, MAIN_MENU_QUIT))
    {
        m_nextState = State::STATE_QUIT;
    }

    // Render
    RenderMainMenu(&m_mainMenuUI);
}

void GameManager::UpdateGameplay()
{
    float deltaTime = GetFrameTime();

    // Input
    InputAction input = ReadGameInput();
    m_gameState.HandleInput(input);

    // Update
    m_gameState.UpdateGame(deltaTime, m_audioSFX[SFX_EAT], m_audioSFX[SFX_COLLISION]);

    // Check transitions
    if (m_gameState.isGameOver)
    {
        m_nextState = State::STATE_GAME_OVER;
    }

    // Render
    RenderGameplay(&m_gameState);
}

void GameManager::UpdateGameOver()
{
    UpdateUI(&m_gameOverUI, m_audioSFX[SFX_MENU_BUTTON]);

    if (WasActiveButtonReleased(&m_gameOverUI, GAME_OVER_MAIN_MENU))
    {
        m_nextState = State::STATE_MAIN_MENU;
    }

    if (WasActiveButtonReleased(&m_gameOverUI, GAME_OVER_RESTART))
    {
        m_nextState = State::STATE_PLAYING;
    }

    if (WasActiveButtonReleased(&m_gameOverUI, GAME_OVER_QUIT))
    {
        m_nextState = State::STATE_QUIT;
    }

    // Render
    RenderGameOver(&m_gameOverUI, m_gameState.score, m_gameState.highScore);
}

// Any cleanup from old state could go here
void GameManager::SetGameManagerState(State newState)
{
    m_currentState = newState;

    // Any initialization for new state could go here
    switch (newState)
    {
    case State::STATE_MAIN_MENU:
        InitMainMenuUI(&m_mainMenuUI);
        break;
    case State::STATE_PLAYING:
        m_gameState.Reset();
        break;
    case State::STATE_GAME_OVER:
        InitGameOverUI(&m_gameOverUI);
        break;
    case State::STATE_QUIT:
        m_shouldQuit = true;
        break;
    }
}

void GameManager::Run()
{
    // Main game loop
    while (!m_shouldQuit && !WindowShouldClose())
    {
        // Handle state transitions
        if (m_nextState != m_currentState)
        {
            SetGameManagerState(m_nextState);
        }

        // Update the current state
        switch (m_currentState)
        {
        case State::STATE_MAIN_MENU:
            UpdateMainMenu();
            break;
        case State::STATE_PLAYING:
            UpdateGameplay();
            break;
        case State::STATE_GAME_OVER:
            UpdateGameOver();
            break;
        case State::STATE_QUIT:
            m_shouldQuit = true;
            break;
        }
    }
}