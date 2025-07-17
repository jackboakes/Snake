#include "raylib.h"
#include "GameManager.h"
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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);

    gameManager->shouldQuit = false;
    gameManager->currentState = STATE_MAIN_MENU; 
    gameManager->nextState = STATE_MAIN_MENU;

    InitAudio(gameManager->audioSFX);
    LoadGameTextures();
    UpdateWindowIcon();

    // init ui
    InitMainMenuUI(&gameManager->mainMenuUI);
    InitGameOverUI(&gameManager->gameOverUI);
}

void ShutdownGameManager(GameManager* gameManager)
{
    UnloadGameTextures();
    ShutdownAudio(gameManager->audioSFX);
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
    // data
    UpdateUI(&gameManager->mainMenuUI, gameManager->audioSFX[SFX_MENU_BUTTON]);
    
    // logic
    if (WasActiveButtonReleased(&gameManager->mainMenuUI, START_GAME))
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (WasActiveButtonReleased(&gameManager->mainMenuUI, QUIT))
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

    
    if (WasActiveButtonReleased(&gameManager->gameOverUI, MAIN_MENU))
    {
        gameManager->nextState = STATE_MAIN_MENU;
    }

    if (WasActiveButtonReleased(&gameManager->gameOverUI, RESTART))
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (WasActiveButtonReleased(&gameManager->gameOverUI, QUIT))
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