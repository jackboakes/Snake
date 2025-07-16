#include "raylib.h"
#include "GameManager.h"
#include "Types.h"
#include "Input.h"
#include "Renderer.h"
#include "Audio.h"
#include "UI.h"

enum MainMenuOption
{
    MAIN_MENU_START,
    MAIN_MENU_EXIT,
    MAIN_MENU_OPTION_COUNT
};

enum GameOverOption
{
    GAME_OVER_MAIN_MENU,
    GAME_OVER_RESTART,
    GAME_OVER_EXIT,
    GAME_OVER_OPTION_COUNT
};


void InitGameManager(GameManager* gameManager)
{
    gameManager->shouldQuit = false;
    gameManager->currentState = STATE_MAIN_MENU; 
    gameManager->nextState = STATE_MAIN_MENU;

    InitAudio(&gameManager->gameState);
    LoadGameTextures();
    UpdateWindowIcon();

    // init ui
    InitMainMenuUI(&gameManager->mainMenuUI);
    InitGameOverUI(&gameManager->gameOverUI);
}

void ShutdownGameManager(GameManager* gameManager)
{
    UnloadGameTextures();
    ShutdownAudio(&gameManager->gameState);
    CloseWindow();
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


static void UpdateMainMenu(GameManager* gameManager)
{
    UpdateUI(&gameManager->mainMenuUI);

    
    if (IsButtonActive(&gameManager->mainMenuUI, 1) && GetButton(&gameManager->mainMenuUI, 1)->isReleased == true)
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (IsButtonActive(&gameManager->mainMenuUI, 2) && GetButton(&gameManager->mainMenuUI, 2)->isReleased == true)
    {
        gameManager->nextState = STATE_QUIT;
    }

    // Render
    Color backgroundColour = { 0x2D, 0x27, 0x2F, 0xFF };
    const char* title = "SNAKE GAME";
    const int titleFontSize = 40;
    const int titleFontSpacing = titleFontSize / 10;
    const int titleWidth = MeasureTextEx(GetFontDefault(), title, titleFontSize, titleFontSpacing).x;
    const int titlePosX = (GetScreenWidth() / 2) - (titleWidth / 2);
    const int titlePosY = 250;

    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawTextWithShadow(title, titlePosX, titlePosY, titleFontSize, RED);
    RenderUI(&gameManager->mainMenuUI);
    EndDrawing();
}

static void UpdateGameplay(GameManager* gameManager)
{
    float deltaTime = GetFrameTime();

    // Input
    InputAction input = ReadGameInput();
    HandleSnakeInput(&gameManager->gameState.snake, input);

    // Update
    UpdateGame(&gameManager->gameState, deltaTime);

    // Check transitions
    if (gameManager->gameState.isGameOver)
    {
        gameManager->nextState = STATE_GAME_OVER;
    }

    // Render
    RenderGameplay(gameManager);
}

static void UpdateGameOver(GameManager* gameManager)
{
    UpdateUI(&gameManager->gameOverUI);


    if (IsButtonActive(&gameManager->gameOverUI, 1) && GetButton(&gameManager->gameOverUI, 1)->isReleased == true)
    {
        gameManager->nextState = STATE_MAIN_MENU;
    }

    if (IsButtonActive(&gameManager->gameOverUI, 2) && GetButton(&gameManager->gameOverUI, 2)->isReleased == true)
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (IsButtonActive(&gameManager->gameOverUI, 3) && GetButton(&gameManager->gameOverUI, 3)->isReleased == true)
    {
        gameManager->nextState = STATE_QUIT;
    }

    // Render
    Color backgroundColour = { 0x2D, 0x27, 0x2F, 0xFF };
    const char* title = "GAME OVER";
    const int scoreFontSize = 25;
    const int screenWidth = GetScreenWidth();
    const int titleWidth = MeasureText(title, 40);
    const int titleX = (screenWidth - titleWidth) / 2;

    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawTextWithShadow(title, titleX, 150, 40, RED);

    // Draw scores
    const char* scoreText = TextFormat("Final Score: %d", gameManager->gameState.score);
    const char* highScoreText = TextFormat("High Score: %d", gameManager->gameState.highScore);

    int scoreWidth = MeasureText(scoreText, scoreFontSize);
    int highScoreWidth = MeasureText(highScoreText, scoreFontSize);

    DrawTextWithShadow(scoreText, (screenWidth - scoreWidth) / 2, 200, scoreFontSize, RAYWHITE);
    DrawTextWithShadow(highScoreText, (screenWidth - highScoreWidth) / 2, 230, scoreFontSize, YELLOW);

    RenderUI(&gameManager->gameOverUI);
    EndDrawing();
}