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
static Color const BUTTON_FACE_UP = { 0x5A, 0x54, 0x62, 0xFF }; // A mid-tone gray/purple
static Color const BUTTON_FACE_HOVER = { 0x6C, 0x65, 0x73, 0xFF }; // A slightly lighter version
static Color const BORDER_LIGHT = { 0x8C, 0x81, 0x93, 0xFF }; // Light edge
static Color const BORDER_DARK = { 0x3E, 0x35, 0x42, 0xFF }; // Dark edge

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
          GAME_OFFSET - borderThickness,
          GAME_OFFSET - borderThickness,
          GAME_WIDTH + (borderThickness * 2.0f),
          GAME_HEIGHT + (borderThickness * 2.0f)
    };

    //DrawRectangleLinesEx(borderRect, borderThickness, borderColour);
    DrawBeveledBorder(borderRect, borderThickness, borderColour, BORDER_DARK);
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


// Refactors raylib.h DrawRectangleLinesEx to take in two colours and create a bevel
// with a light colour in the top and left border, and darker in the bottom and right
void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour)
{
    Rectangle top = { borderRec.x, borderRec.y, borderRec.width, borderThickness };
    Rectangle bottom = { borderRec.x, borderRec.y + borderRec.height - borderThickness, borderRec.width, borderThickness };
    Rectangle left = { borderRec.x, borderRec.y + borderThickness, borderThickness, borderRec.height - (2 * borderThickness) };
    Rectangle right = { borderRec.x + borderRec.width - borderThickness, borderRec.y + borderThickness, borderThickness, borderRec.height - (2 * borderThickness) };

    DrawRectangleRec(top, lightColour);
    DrawRectangleRec(bottom, darkColour);
    DrawRectangleRec(left, lightColour);
    DrawRectangleRec(right, darkColour);
}

void DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour)
{
    Color const shadowColor = { 0, 0, 0, 128 }; // black with half alpha

    DrawText(text, posX + 2, posY + 2, fontSize, shadowColor);
    DrawText(text, posX, posY, fontSize, colour);
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
