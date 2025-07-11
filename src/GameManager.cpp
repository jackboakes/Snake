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
    // Input
    InputAction input = ReadMenuInput();

    // Update
    if (input == INPUT_UP && gameManager->selectedMenuOption > MAIN_MENU_START) 
    {
        gameManager->selectedMenuOption--;
    }
    else if (input == INPUT_DOWN && gameManager->selectedMenuOption < (MAIN_MENU_OPTION_COUNT -1 )) 
    { 
        gameManager->selectedMenuOption++;
    }

    if (input == INPUT_SELECT && gameManager->selectedMenuOption == MAIN_MENU_START)
    {
        gameManager->nextState = STATE_PLAYING; // Start game if Enter on "Start Game"
    }
    else if (input == INPUT_QUIT || (input == INPUT_SELECT && gameManager->selectedMenuOption == MAIN_MENU_EXIT))
    {
        gameManager->nextState = STATE_QUIT; // Quit if Enter on "Exit"
    }

    // Render
    RenderMainMenu(gameManager->selectedMenuOption);
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