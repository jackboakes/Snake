#include "raylib.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Game.h"
#include "Types.h"
#include "Assets.h"


static Color const snakeColour = { 0xC2, 0x32, 0x1D, 0xFF };
static Color const borderColour = { 0x5A, 0x54, 0x62, 0xFF };
static Color const backgroundColour = { 0x2D, 0x27, 0x2F, 0xFF };
static Color const boardColors[2] = {
    { 0x49, 0x43, 0x51, 0xFF },  // light
    { 0x44, 0x3E, 0x4C, 0xFF }   // dark
};

static Texture2D snakeAtlas = { 0 };

// Helper function to get a 32x32 sprite from the atlas by index
static Rectangle GetSpriteRect(int col, int row) 
{
    Rectangle spriteRect;

    spriteRect = {
            (float) (col * TILE_SIZE),
            (float) (row * TILE_SIZE),
            TILE_SIZE,
            TILE_SIZE
    };

    return  spriteRect;
}

void LoadGameTextures()
{
    snakeAtlas = LoadTexture(GetAssetPath("snake_atlas.png"));

    if (snakeAtlas.id > 0)
    {
        TraceLog(LOG_INFO, "Snake atlas loaded successfully");
    }
    else
    {
        TraceLog(LOG_WARNING, "Failed to load snake atlas");
    }
}

void UnloadGameTextures()
{
    if (snakeAtlas.id > 0)
    {
        UnloadTexture(snakeAtlas);
    }
}

// Sets the icon for the window using image from our atlas
void UpdateWindowIcon()
{
    Image fullSnakeAtlas = LoadImageFromTexture(snakeAtlas);
    Rectangle iconSpriteRect = GetSpriteRect(2, 0); // South facing head
    Image windowIcon = ImageFromImage(fullSnakeAtlas, iconSpriteRect);
    UnloadImage(fullSnakeAtlas);

    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);
}

static void DrawGameBoard()
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

    DrawRectangleLinesEx(borderRect, borderThickness, borderColour);
}

static void DrawGameUI(int score, int highScore)
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

// Returns the rectangle of the snakes head sprites based on direction of the snake
static Rectangle GetHeadSpriteRect(Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH: return GetSpriteRect(0,0);  // head facing up
    case DIR_WEST:  return GetSpriteRect(1, 0);  // head facing left
    case DIR_SOUTH: return GetSpriteRect(2, 0);  // head facing down
    case DIR_EAST:  return GetSpriteRect(3, 0);  // head facing right
    default:        return GetSpriteRect(0, 0);  // default
    }
}

static void DrawSnake(const Snake* snake)
{
    for (int i = 1; i < snake->length; i++) 
    {
        int pixelX = GAME_OFFSET + (snake->bodyPart[i].position.x * TILE_SIZE);
        int pixelY = GAME_OFFSET + (snake->bodyPart[i].position.y * TILE_SIZE);

        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, snakeColour);
    }

    int headX = GAME_OFFSET + (int)(snake->bodyPart[0].position.x * TILE_SIZE);
    int headY = GAME_OFFSET + (int)(snake->bodyPart[0].position.y * TILE_SIZE);

    if (snakeAtlas.id > 0)
    {
        // Get the correct sprite rectangle for the snake's direction
        Rectangle sourceRect = GetHeadSpriteRect(snake->currentDirection);

        // Destination rectangle (where to draw on screen)
        Rectangle destRect = { (float)headX, (float)headY, (float)TILE_SIZE, (float)TILE_SIZE};

        // Draw the sprite (no rotation needed since we have pre-rotated sprites)
        DrawTexturePro(snakeAtlas, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        // Fallback to rounded rectangle if texture isn't loaded
        Rectangle headPos = { (float)headX, (float)headY, (float)TILE_SIZE, (float)TILE_SIZE };
        DrawRectangleRounded(headPos, 0.5, 6, snakeColour);
    }
}

static void DrawFood(const Food* food)
{
    int pixelX = GAME_OFFSET + (food->position.x * TILE_SIZE);
    int pixelY = GAME_OFFSET + (food->position.y * TILE_SIZE);


    if (snakeAtlas.id > 0)
    {
        Rectangle foodRect = GetSpriteRect(0, 2);
        Rectangle destRect = { (float)pixelX, (float)pixelY, (float)TILE_SIZE, (float)TILE_SIZE };
        DrawTexturePro(snakeAtlas, foodRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, GREEN);
    }
}

void RenderMainMenu(int selectedOption)
{
    const char* title = "SNAKE GAME";
    const char* options[] = { "Start Game", "Exit" };
    const int optionCount = sizeof(options) / sizeof(options[0]);

    // Calculate positions
    const int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    const int titleX = (GetScreenWidth() - titleWidth) / 2;

    BeginDrawing();

    ClearBackground(backgroundColour);

    DrawText(title, titleX, TITLE_Y, TITLE_FONT_SIZE, RAYWHITE);

    for (int i = 0; i < optionCount; i++)
    {
        int optionWidth = MeasureText(options[i], OPTION_FRONT_SIZE);
        int optionX = (GetScreenWidth() - optionWidth) / 2;
        int optionY = TITLE_Y + VERTICAL_SPACING + (i * VERTICAL_SPACING);

        Color color;
        if (i == selectedOption) 
        {
            color = YELLOW;
        }
        else 
        {
            color = RAYWHITE;
        }

        DrawText(options[i], optionX, optionY, OPTION_FRONT_SIZE, color);
    }

    // Draw instructions
    const char* instructions = "Use UP/DOWN to navigate, ENTER to select";
    int instrWidth = MeasureText(instructions, INSTRUCTION_FONT_SIZE);
    int instrX = (GetScreenWidth() - instrWidth) / 2;
    DrawText(instructions, instrX, TITLE_Y + VERTICAL_SPACING * 3, INSTRUCTION_FONT_SIZE, GRAY);

    EndDrawing();
}

void RenderGameplay(GameManager* gameManager)
{
    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawGameBoard();
    DrawSnake(&gameManager->gameState.snake);
    DrawFood(&gameManager->gameState.food);
    DrawGameUI(gameManager->gameState.score, gameManager->gameState.highScore);
    EndDrawing();
}



void RenderGameOver(int selectedOption, int score, int highScore)
{
    const int scoreFontSize = 25;
    const char* title = "GAME OVER";
    const char* options[] = { "Main Menu", "Restart", "Quit" };
    const int optionCount = sizeof(options) / sizeof(options[0]);
    // Calculate positions
    const int titleWidth = MeasureText(title, TITLE_FONT_SIZE);
    const int titleX = (GetScreenWidth() - titleWidth) / 2;

    BeginDrawing();

    ClearBackground(backgroundColour);
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

    EndDrawing();
}