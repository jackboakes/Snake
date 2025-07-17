#include "raylib.h"
#include "GameManager.h"
#include "Input.h"
#include "Renderer.h"
#include "Audio.h"
#include "Assets.h"
#include "UI.h"


void InitGameManager(GameManager* gameManager)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);
    InitAudio(gameManager->audioSFX);
    LoadGameTextures();
    UpdateWindowIcon(TILE_SIZE);
    gameManager->shouldQuit = false;
    gameManager->currentState = STATE_MAIN_MENU;
    gameManager->nextState = STATE_MAIN_MENU;
    // init main menu since its first thing loaded
    InitMainMenuUI(&gameManager->mainMenuUI);
}

void ShutdownGameManager(GameManager* gameManager)
{
    UnloadGameTextures();
    ShutdownAudio(gameManager->audioSFX);
    CloseWindow();
}



static void UpdateMainMenu(GameManager* gameManager)
{
    // data
    UpdateUI(&gameManager->mainMenuUI, gameManager->audioSFX[SFX_MENU_BUTTON]);
    
    // logic
    if (WasActiveButtonReleased(&gameManager->mainMenuUI, MAIN_MENU_START))
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (WasActiveButtonReleased(&gameManager->mainMenuUI, MAIN_MENU_QUIT))
    {
        gameManager->nextState = STATE_QUIT;
    }

    // Render
    RenderMainMenu(&gameManager->mainMenuUI);
}

static void UpdateGameplay(GameManager* gameManager)
{
    float deltaTime = GetFrameTime();

    // Input
    InputAction input = ReadGameInput();
    HandleSnakeInput(&gameManager->gameState.snake, input);

    // Update
    UpdateGame(&gameManager->gameState, deltaTime, gameManager->audioSFX[SFX_EAT], gameManager->audioSFX[SFX_COLLISION]);

    // Check transitions
    if (gameManager->gameState.isGameOver)
    {
        gameManager->nextState = STATE_GAME_OVER;
    }

    // Render
    RenderGameplay(&gameManager->gameState);
}

static void UpdateGameOver(GameManager* gameManager)
{
    UpdateUI(&gameManager->gameOverUI, gameManager->audioSFX[SFX_MENU_BUTTON]);

    
    if (WasActiveButtonReleased(&gameManager->gameOverUI, GAME_OVER_MAIN_MENU))
    {
        gameManager->nextState = STATE_MAIN_MENU;
    }

    if (WasActiveButtonReleased(&gameManager->gameOverUI, GAME_OVER_RESTART))
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (WasActiveButtonReleased(&gameManager->gameOverUI, GAME_OVER_QUIT))
    {
        gameManager->nextState = STATE_QUIT;
    }

    // Render
    RenderGameOver(&gameManager->gameOverUI, gameManager->gameState.score, gameManager->gameState.highScore);
}

// Any cleanup from old state could go here
static void SetGameManagerState(GameManager* gameManager, GameStateID newStateID)
{
    gameManager->currentState = newStateID;

    // Any initialization for new state could go here
    switch (newStateID)
    {
    case STATE_MAIN_MENU:
        InitMainMenuUI(&gameManager->mainMenuUI);
        break;
    case STATE_PLAYING:
        InitGame(&gameManager->gameState);
        break;
    case STATE_GAME_OVER:
        InitGameOverUI(&gameManager->gameOverUI);
        break;
    case STATE_QUIT:
        gameManager->shouldQuit = true;
        break;
    }
}

void RunGameManager(GameManager* gameManager)
{
    // Main game loop
    while (!gameManager->shouldQuit && !WindowShouldClose())
    {
        // Handle state transitions
        if (gameManager->nextState != gameManager->currentState)
        {
            SetGameManagerState(gameManager, gameManager->nextState);
        }

        // Update the current state
        switch (gameManager->currentState)
        {
        case STATE_MAIN_MENU:
            UpdateMainMenu(gameManager);
            break;
        case STATE_PLAYING:
            UpdateGameplay(gameManager);
            break;
        case STATE_GAME_OVER:
            UpdateGameOver(gameManager);
            break;
        case STATE_QUIT:
            gameManager->shouldQuit = true;
            break;
        }
    }
}