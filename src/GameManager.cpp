#include <raylib.h>
#include "GameManager.h"
#include "Renderer.h"
#include "Game.h"
#include "Input.h"
#include "Score.h"
#include "Types.h"


void InitGameManager(GameManager* gameManager)
{
    gameManager->shouldQuit = false;
    gameManager->currentStateID = STATE_MAIN_MENU; // Start in playing state for now
    gameManager->nextStateID = STATE_MAIN_MENU;
    gameManager->selectedMenuOption = 0;

    // Initialize game resources
    InitGame(&gameManager->gameState);
    LoadSnakeTextures();
}

void RunGameManager(GameManager* gameManager)
{
    // Main game loop - super simple!
    while (!gameManager->shouldQuit && !WindowShouldClose())
    {
        // Handle state transitions
        if (gameManager->nextStateID != gameManager->currentStateID)
        {
            SetGameManagerState(gameManager, gameManager->nextStateID);
        }

        // Run the current state based on enum
        switch (gameManager->currentStateID)
        {
        case STATE_MAIN_MENU:
            StateMainMenu(gameManager);
            break;
        case STATE_PLAYING:
            StateGameplay(gameManager);
            break;
        case STATE_GAME_OVER:
            StateGameOver(gameManager);
            break;
        case STATE_QUIT:
            gameManager->shouldQuit = true;
            break;
        }
    }
}

void SetGameManagerState(GameManager* gameManager, GameStateID newStateID)
{
    // Any cleanup from old state could go here

    gameManager->currentStateID = newStateID;

    // Any initialization for new state could go here
    switch (newStateID)
    {
    case STATE_MAIN_MENU:
        // Initialize menu if needed
        break;
    case STATE_PLAYING:
        // Game already initialized
        break;
    case STATE_GAME_OVER:
        // Maybe play game over sound
        break;
    case STATE_QUIT:
        gameManager->shouldQuit = true;
        break;
    }
}

void ShutdownGameManager(GameManager* gameManager)
{
    UnloadSnakeTextures();
    CloseWindow();
}

// ============================================================================
// STATE IMPLEMENTATIONS - Each is a complete input/update/render cycle
// ============================================================================

void StateMainMenu(GameManager* gameManager)
{
    // INPUT
    InputAction input = ReadInputMenu();

    // UPDATE
    if (input == INPUT_UP && gameManager->selectedMenuOption > 0) {
        gameManager->selectedMenuOption--;
    }
    else if (input == INPUT_DOWN && gameManager->selectedMenuOption < 1) { // 1 = number of options - 1
        gameManager->selectedMenuOption++;
    }
    else if (input == INPUT_SELECT && gameManager->selectedMenuOption == 0) {
        gameManager->nextStateID = STATE_PLAYING; // Start game if Enter on "Start Game"
    }
    else if (input == INPUT_QUIT || (input == INPUT_SELECT && gameManager->selectedMenuOption == 1)) {
        gameManager->nextStateID = STATE_QUIT; // Quit if Enter on "Exit"
    }

    // RENDER
    BeginDrawing();
    Color backgroundColor = { 0x2D, 0x27, 0x2F, 0xFF };
    ClearBackground(backgroundColor);
    DrawMainMenu(gameManager->selectedMenuOption);
    EndDrawing();
}

void StateGameplay(GameManager* gameManager)
{
    float deltaTime = GetFrameTime();

    // INPUT
    InputAction input = ReadInputGame();
    HandleInputGame(&gameManager->gameState.snake, input);

    // UPDATE
    UpdateGame(&gameManager->gameState, deltaTime);

    // Check for game over transition
    if (gameManager->gameState.isGameOver)
    {
        gameManager->nextStateID = STATE_GAME_OVER;
    }

    // RENDER
    BeginDrawing();
    Color backgroundColor = { 0x2D, 0x27, 0x2F, 0xFF };
    ClearBackground(backgroundColor);

    DrawGameBoard();
    DrawGameUI(gameManager->gameState.score, gameManager->gameState.highScore);
    DrawSnake(gameManager->gameState.snake);
    DrawFood(gameManager->gameState.food);

    EndDrawing();
}

void StateGameOver(GameManager* gameManager)
{
    // INPUT
    InputAction input = ReadInputMenu();

    // UPDATE
    if (input == INPUT_UP && gameManager->selectedMenuOption > 0) 
    {
        gameManager->selectedMenuOption--;
    }
    else if (input == INPUT_DOWN && gameManager->selectedMenuOption < 2) 
    {
        gameManager->selectedMenuOption++;
    }
    else if (input == INPUT_SELECT && gameManager->selectedMenuOption == 0) 
    {
        InitGame(&gameManager->gameState); 
        gameManager->nextStateID = STATE_MAIN_MENU; 
    }
    else if (input == INPUT_SELECT && gameManager->selectedMenuOption == 1) //RESTART
    {
        InitGame(&gameManager->gameState);
        gameManager->nextStateID = STATE_PLAYING; 
    }
    else if (input == INPUT_QUIT || (input == INPUT_SELECT && gameManager->selectedMenuOption == 2))
    {
        gameManager->nextStateID = STATE_QUIT;
    }


    // RENDER
    BeginDrawing();
    Color backgroundColor = { 0x2D, 0x27, 0x2F, 0xFF };
    ClearBackground(backgroundColor);
    DrawGameOverMenu(gameManager->selectedMenuOption, gameManager->gameState.score, gameManager->gameState.highScore);
    EndDrawing();
}