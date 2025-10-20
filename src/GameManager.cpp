#include "raylib.h"
#include "GameManager.h"
#include "Input.h"
#include "Renderer.h"
#include "Audio.h"
#include "Assets.h"


GameManager::GameManager()
{
    constexpr int screenWidth { (GAME_WIDTH + (2 * GAME_OFFSET)) };
    constexpr int screenHeight { (GAME_HEIGHT + (2 * GAME_OFFSET)) };
    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);
    Assets::LoadGameTextures();
    Assets::UpdateWindowIcon(TILE_SIZE);

    // Needs to init as main menu
    const int buttonWidth { 32 * 9 };
    const int buttonHeight { 32 * 2 };
    const int buttonPadding { 20 };

    Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

    m_mainMenuUI.AddButton(MAIN_MENU_START, buttonBounds, "Start Game");
    m_mainMenuUI.AddButton(MAIN_MENU_QUIT, buttonBounds, "Quit");

    m_mainMenuUI.CentreButtonsVertically(screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
}

GameManager::~GameManager()
{
    Assets::UnloadGameTextures();
    CloseWindow();
}

void GameManager::UpdateMainMenu()
{
    // Data
    m_mainMenuUI.Update(m_audio);
    
    // Logic
    if (m_mainMenuUI.WasActiveButtonReleased(MAIN_MENU_START))
    {
        m_nextState = State::STATE_PLAYING;
    }

    if (m_mainMenuUI.WasActiveButtonReleased(MAIN_MENU_QUIT))
    {
        m_nextState = State::STATE_QUIT;
    }

    // Render
    Renderer::MainMenu(m_mainMenuUI);
}

void GameManager::UpdateGameplay()
{
    float deltaTime = GetFrameTime();

    // Input
    InputAction input = ReadGameInput();
    m_gameState.HandleInput(input);

    // Update
    m_gameState.UpdateGame(deltaTime, m_audio);

    // If game is over no need to render next frame
    if (m_gameState.isGameOver)
    {
        m_nextState = State::STATE_GAME_OVER;
        return;
    }

    // Render
    Renderer::Gameplay(m_gameState);
}

void GameManager::UpdateGameOver()
{
    m_gameOverUI.Update(m_audio);

    if (m_gameOverUI.WasActiveButtonReleased(GAME_OVER_MAIN_MENU))
    {
        m_nextState = State::STATE_MAIN_MENU;
    }

    if (m_gameOverUI.WasActiveButtonReleased(GAME_OVER_RESTART))
    {
        m_nextState = State::STATE_PLAYING;
    }

    if (m_gameOverUI.WasActiveButtonReleased(GAME_OVER_QUIT))
    {
        m_nextState = State::STATE_QUIT;
    }

    // Render
    Renderer::GameOver(m_gameOverUI, m_gameState.score, m_gameState.highScore);
}

// Any cleanup from old state could go here
void GameManager::SetGameManagerState(State newState)
{
    m_currentState = newState;

    // Any initialization for new state could go here
    switch (newState)
    {
    case State::STATE_MAIN_MENU:
    {
        m_mainMenuUI = UI{};
        const int buttonWidth { 32 * 9 };
        const int buttonHeight { 32 * 2 };
        const int buttonPadding { 20 };
        const int screenWidth { GetScreenWidth() };
        const int screenHeight { GetScreenHeight() };

        Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

        m_mainMenuUI.AddButton(MAIN_MENU_START, buttonBounds, "Start Game");
        m_mainMenuUI.AddButton(MAIN_MENU_QUIT, buttonBounds, "Quit");

        m_mainMenuUI.CentreButtonsVertically(screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
    }
    break;
    case State::STATE_PLAYING:
        m_gameState.Reset();
        break;
    case State::STATE_GAME_OVER:
    {
        m_gameOverUI = UI {};
        const int buttonWidth { 32 * 9 };
        const int buttonHeight { 32 * 2 };
        const int buttonPadding { 20 };
        const int screenWidth { GetScreenWidth() };
        const int screenHeight { GetScreenHeight() };
        Rectangle buttonBounds = { 0, 0, buttonWidth, buttonHeight };

        m_gameOverUI.AddButton(GAME_OVER_MAIN_MENU, buttonBounds, "Main Menu");
        m_gameOverUI.AddButton(GAME_OVER_RESTART, buttonBounds, "Restart");
        m_gameOverUI.AddButton(GAME_OVER_QUIT, buttonBounds, "Quit");

        m_gameOverUI.CentreButtonsVertically(screenWidth, screenHeight, buttonWidth, buttonHeight, buttonPadding);
    }
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