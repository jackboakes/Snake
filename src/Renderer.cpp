#include <raylib.h>
#include "Renderer.h"
#include "Game.h"
#include "Types.h"

// snake colour CA0004
Color const snakeColour = { 0xC2, 0x32, 0x1D, 0xFF };
Color const borderColor = { 0x5A, 0x54, 0x62, 0xFF };

static Texture2D snakeAtlas = { 0 };

void LoadSnakeTextures()
{
    snakeAtlas = LoadTexture("assets/snake_atlas.png"); // when run with cmake in root dir

    if (snakeAtlas.id == 0) {
        // Fallback to build directory (when run by clicking exe)
        snakeAtlas = LoadTexture("../../assets/snake_atlas.png");
    }

    if (snakeAtlas.id > 0)
    {
        TraceLog(LOG_INFO, "Snake atlas loaded successfully");
    }
    else
    {
        TraceLog(LOG_WARNING, "Failed to load snake atlas");
    }
}

void UnloadSnakeTextures()
{
    if (snakeAtlas.id > 0)
    {
        UnloadTexture(snakeAtlas);
    }
}

void DrawGameBoard()
{
    const int borderThickness = 5;

    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            int posX = GAME_OFFSET + (x * TILE_SIZE);
            int posY = GAME_OFFSET + (y * TILE_SIZE);

            Color tileColor = boardColors[(x + y) % 2];
            DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, tileColor);
        }
    }
    Rectangle borderRect = {
          GAME_OFFSET - 5,
          GAME_OFFSET - 5,
          GAME_WIDTH + 10,
          GAME_HEIGHT + 10
    };

    DrawRectangleLinesEx(borderRect, borderThickness, borderColor);
}

void DrawGameUI(int score, int highScore)
{
    const int fontSize = 40;
    const int yPosition = 20;

    const char* currentScoreText = TextFormat("Score: %i", score);
    const char* highScoreText = TextFormat("High Score: %i", highScore);
    int highScoreTextWidth = MeasureText(highScoreText, fontSize);

    int leftEdge = GAME_OFFSET - 5;
    int rightEdge = GAME_OFFSET + GAME_WIDTH + 5; // Right edge of game area plus margin
    int rightAlignedX = rightEdge - highScoreTextWidth;

    DrawText(currentScoreText, leftEdge, yPosition, fontSize, RAYWHITE);
    DrawText(highScoreText, rightAlignedX, yPosition, fontSize, YELLOW);
}

Rectangle GetHeadSpriteRect(Direction direction)
{
    int spriteIndex = 0;

    switch (direction)
    {
    case DIR_NORTH: spriteIndex = 0; break;  // Position 0,0
    case DIR_WEST:  spriteIndex = 1; break;  // Position 1,0
    case DIR_SOUTH: spriteIndex = 2; break;  // Position 2,0
    case DIR_EAST:  spriteIndex = 3; break;  // Position 3,0
    default:        spriteIndex = 0; break;  // Default to north
    }

    Rectangle out = { spriteIndex * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };

    return out;
}

void DrawSnake(const Snake& snake)
{
    for (int i = 1; i < snake.length; i++) 
    {
        int pixelX = GAME_OFFSET + (int)(snake.bodyPart[i].position.x * TILE_SIZE);
        int pixelY = GAME_OFFSET + (int)(snake.bodyPart[i].position.y * TILE_SIZE);

        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, snakeColour);
    }

    int headX = GAME_OFFSET + (int)(snake.bodyPart[0].position.x * TILE_SIZE);
    int headY = GAME_OFFSET + (int)(snake.bodyPart[0].position.y * TILE_SIZE);

    if (snakeAtlas.id > 0)
    {
        // Get the correct sprite rectangle for the snake's direction
        Rectangle sourceRect = GetHeadSpriteRect(snake.currentDirection);

        // Destination rectangle (where to draw on screen)
        Rectangle destRect = {headX, headY, TILE_SIZE, TILE_SIZE};

        // Draw the sprite (no rotation needed since we have pre-rotated sprites)
        DrawTexturePro(snakeAtlas, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        // Fallback to rounded rectangle if texture isn't loaded
        Rectangle headPos = { headX, headY, TILE_SIZE, TILE_SIZE };
        DrawRectangleRounded(headPos, 0.5, 6, snakeColour);
    }
}

void DrawFood(const Food& food)
{
    int pixelX = GAME_OFFSET + (int)(food.position.x * TILE_SIZE);
    int pixelY = GAME_OFFSET + (int)(food.position.y * TILE_SIZE);


    if (snakeAtlas.id > 0)
    {
        Rectangle foodRect = { 0, 2 * TILE_SIZE, TILE_SIZE,TILE_SIZE };

        Rectangle destRect = {pixelX, pixelY, TILE_SIZE, TILE_SIZE};

        DrawTexturePro(snakeAtlas, foodRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, GREEN);
    }
}

void DrawGamePlay(GameManager* gameManager)
{
    BeginDrawing();

    Color backgroundColor = { 0x2D, 0x27, 0x2F, 0xFF }; // Dark purple background
    ClearBackground(backgroundColor);

    DrawGameBoard();
    DrawGameUI(gameManager->gameState.score, gameManager->gameState.highScore);
    DrawSnake(gameManager->gameState.snake);
    DrawFood(gameManager->gameState.food);

    EndDrawing();
}

void DrawMainMenu(int selectedOption)
{
    const char* title = "SNAKE GAME";
    const char* options[] = { "Start Game", "Exit" };
    const int optionCount = sizeof(options) / sizeof(options[0]);

    // Calculate positions
    int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    int titleX = (GetScreenWidth() - titleWidth) / 2;

    DrawText(title, titleX, TITLE_Y, TITLE_FONT_SIZE, RAYWHITE);

    for (int i = 0; i < optionCount; i++)
    {
        int optionWidth = MeasureText(options[i], OPTION_FRONT_SIZE);
        int optionX = (GetScreenWidth() - optionWidth) / 2;
        int optionY = TITLE_Y + VERTICAL_SPACING + (i * VERTICAL_SPACING);

        Color color;
        if (i == selectedOption) {
            color = YELLOW;
        }
        else {
            color = RAYWHITE;
        }
        DrawText(options[i], optionX, optionY, OPTION_FRONT_SIZE, color);
    }

    // Draw instructions
    const char* instructions = "Use UP/DOWN to navigate, ENTER to select";
    int instrWidth = MeasureText(instructions, INSTRUCTION_FONT_SIZE);
    int instrX = (GetScreenWidth() - instrWidth) / 2;
    DrawText(instructions, instrX, TITLE_Y + VERTICAL_SPACING * 3, INSTRUCTION_FONT_SIZE, GRAY);
}

void DrawGameOverMenu(int selectedOption, int score, int highScore)
{
    const int scoreFontSize = 25;

    const char* title = "GAME OVER";
    const char* options[] = { "Main Menu", "Restart", "Quit" };
    const int optionCount = sizeof(options) / sizeof(options[0]);

    // Calculate positions
    int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    int titleX = (GetScreenWidth() - titleWidth) / 2;

    // Draw title
    DrawText(title, titleX, TITLE_Y, TITLE_FONT_SIZE, RED);

    // Draw scores
    const char* scoreText = TextFormat("Final Score: %d", score);
    const char* highScoreText = TextFormat("High Score: %d", highScore);

    int scoreWidth = MeasureText(scoreText, scoreFontSize);
    int highScoreWidth = MeasureText(highScoreText, scoreFontSize);

    DrawText(scoreText, (GetScreenWidth() - scoreWidth) / 2, TITLE_Y + 50, scoreFontSize, RAYWHITE);
    DrawText(highScoreText, (GetScreenWidth() - highScoreWidth) / 2, TITLE_Y + 80, scoreFontSize, YELLOW);

    // Draw menu options
    for (int i = 0; i < optionCount; i++)
    {
        int optionWidth = MeasureText(options[i], OPTION_FRONT_SIZE);
        int optionX = (GetScreenWidth() - optionWidth) / 2;
        int optionY = TITLE_Y + 150 + (i * VERTICAL_SPACING);

        Color color;
        if (i == selectedOption) {
            color = YELLOW;
        }
        else {
            color = RAYWHITE;
        }
        DrawText(options[i], optionX, optionY, OPTION_FRONT_SIZE, color);
    }

    // Draw instructions
    const char* instructions = "Use UP/DOWN to navigate, ENTER to select";
    int instrWidth = MeasureText(instructions, INSTRUCTION_FONT_SIZE);
    int instrX = (GetScreenWidth() - instrWidth) / 2;
    DrawText(instructions, instrX, TITLE_Y + 150 + (optionCount * VERTICAL_SPACING), INSTRUCTION_FONT_SIZE, GRAY);
}