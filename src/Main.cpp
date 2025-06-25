#include <raylib.h>
#include"Game.h"
#include "Input.h"
#include "Renderer.h"


//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 768

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    GameState gameState;
    InitGame(&gameState);

    LoadSnakeTextures(); //snake head atlas
    SetTargetFPS(60); 


    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // input
        InputAction input = ReadInput();
        HandleInput(&gameState.snake, input);

        // update gamestate
        UpdateGame(&gameState, deltaTime);

        // render
        BeginDrawing();
        Color backgroundColor = { 0x2D, 0x27, 0x2F, 0xFF }; // Dark purple background
        ClearBackground(backgroundColor);

        DrawGameBoard();
        DrawUI(gameState.score, gameState.highScore);
        DrawSnake(gameState.snake);
        DrawFood(gameState.food);


        int fps = GetFPS();
        const char* windowTitle = TextFormat("Snake! FPS: %i", fps);

        EndDrawing();
        SetWindowTitle(windowTitle);
    }



    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadSnakeTextures();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

