#include "raylib.h"
#include "GameManager.h"
#include "Types.h"
#include "Input.h"
#include "Renderer.h"
#include "Audio.h"

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
    gameManager->selectedMenuOption = 0;

    InitAudio(&gameManager->gameState);
    LoadGameTextures();
    UpdateWindowIcon();
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
        gameManager->selectedMenuOption = 0;
        break;
    case STATE_PLAYING:
        InitGame(&gameManager->gameState);
        break;
    case STATE_GAME_OVER:
        gameManager->selectedMenuOption = 0;
        break;
    case STATE_QUIT:
        gameManager->shouldQuit = true;
        break;
    }
}


static void UpdateMainMenu(GameManager* gameManager)
{
    const int buttonWidth = 32 * 9;
    const int buttonHeight = 32 * 2;
    const int buttonPadding = 32;
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const char* mainMenuOptions[] = { "Start Game", "Quit" };
    const int mainMenuOptionCount = sizeof(mainMenuOptions) / sizeof(mainMenuOptions[0]);

    const int totalButtonsHeight = (mainMenuOptionCount * buttonHeight) + ((mainMenuOptionCount - 1) * buttonPadding);

    const int startY = (screenHeight / 2) - (totalButtonsHeight / 2);

    Rectangle buttonBounds[mainMenuOptionCount];
    ButtonState buttonStates[mainMenuOptionCount];

    for (int i = 0; i < mainMenuOptionCount; i++)
    {
        buttonBounds[i] = {
            (screenWidth / 2.0f) - (buttonWidth / 2.0f),  // Centered horizontally
            (float)startY + (i * (buttonHeight + buttonPadding)), // Distributed vertically
            buttonWidth,
            buttonHeight
        };

        // Update button state
        buttonStates[i] = UpdateButton(buttonBounds[i]);
    }

    // magic numbers use enum if u stick with this
    if (buttonStates[0].isClicked)
    {
        gameManager->nextState = STATE_PLAYING;
    }

    if (buttonStates[1].isClicked)
    {
        gameManager->nextState = STATE_QUIT;
    }

    // Render
    Color backgroundColour = { 0x2D, 0x27, 0x2F, 0xFF };
    const char* title = "SNAKE GAME";
    const int titleFontSize = 40;
    const int titleFontSpacing = titleFontSize / 10;
    const int titleWidth = MeasureTextEx(GetFontDefault(), title, titleFontSize, titleFontSpacing).x;
    const int titlePosX = (screenWidth / 2) - (titleWidth / 2);


    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawText(title, titlePosX, 50, titleFontSize, WHITE);
    for (int i = 0; i < mainMenuOptionCount; i++)
    {
        RenderButton(buttonBounds[i], mainMenuOptions[i], buttonStates[i]);
    }

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
    // Input
    InputAction input = ReadMenuInput();

    // Update
    if (input == INPUT_UP && gameManager->selectedMenuOption > GAME_OVER_MAIN_MENU) 
    {
        gameManager->selectedMenuOption--;
    }
    else if (input == INPUT_DOWN && gameManager->selectedMenuOption < (GAME_OVER_OPTION_COUNT - 1)) 
    {
        gameManager->selectedMenuOption++;
    }

    if (input == INPUT_SELECT && gameManager->selectedMenuOption == GAME_OVER_MAIN_MENU)
    {
        InitGame(&gameManager->gameState); 
        gameManager->nextState = STATE_MAIN_MENU; 
    }
    else if (input == INPUT_SELECT && gameManager->selectedMenuOption == GAME_OVER_RESTART) 
    {
        InitGame(&gameManager->gameState);
        gameManager->nextState = STATE_PLAYING; 
    }
    else if (input == INPUT_QUIT || (input == INPUT_SELECT && gameManager->selectedMenuOption == GAME_OVER_EXIT))
    {
        gameManager->nextState = STATE_QUIT;
    }


    // Render
    RenderGameOver(gameManager->selectedMenuOption, gameManager->gameState.score, gameManager->gameState.highScore);
}