#include "raylib.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Game.h"
#include "Assets.h"
#include "UI.h"


static Color const snakeColour = { 0xC2, 0x32, 0x1D, 0xFF }; // pikmin red
static Color const borderColour = { 0x5A, 0x54, 0x62, 0xFF }; // A mid-tone gray/purple
static Color const backgroundColour = { 0x2D, 0x27, 0x2F, 0xFF }; // dark purple/black
static Color const boardColors[2] = {
    { 0x49, 0x43, 0x51, 0xFF },  // light
    { 0x44, 0x3E, 0x4C, 0xFF }   // dark
};

// Button colours
static const Color BUTTON_FACE_DEFAULT = borderColour; // A mid-tone gray/purple
static Color const BUTTON_FACE_HOVER = { 0x6C, 0x65, 0x73, 0xFF }; // A slightly lighter version
static Color const BORDER_LIGHT = { 0x8C, 0x81, 0x93, 0xFF }; // Light edge
static Color const BORDER_DARK = { 0x3E, 0x35, 0x42, 0xFF }; // Dark edge

static void DrawGameBoard()
{
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
          GAME_OFFSET - BORDER_THICKNESS,
          GAME_OFFSET - BORDER_THICKNESS,
          GAME_WIDTH + (BORDER_THICKNESS * 2),
          GAME_HEIGHT + (BORDER_THICKNESS * 2)
    };

    DrawBeveledBorder(borderRect, BORDER_THICKNESS, borderColour, BORDER_DARK);
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

    DrawTextWithShadow(currentScoreText, leftEdge, yPosition, fontSize, RAYWHITE);
    DrawTextWithShadow(highScoreText, rightAlignedX, yPosition, fontSize, YELLOW);
}

// Returns the rectangle of the snakes head sprites based on direction of the snake
static Rectangle GetHeadSpriteRect(Direction direction)
{
    switch (direction)
    {
    case DIR_NORTH: return Assets::GetSpriteRect(0,0, TILE_SIZE);  // head facing up
    case DIR_WEST:  return Assets::GetSpriteRect(1, 0, TILE_SIZE);  // head facing left
    case DIR_SOUTH: return Assets::GetSpriteRect(2, 0, TILE_SIZE);  // head facing down
    case DIR_EAST:  return Assets::GetSpriteRect(3, 0, TILE_SIZE);  // head facing right
    default:        return Assets::GetSpriteRect(0, 0, TILE_SIZE);  // default
    }
}

static void DrawSnake(const Snake* snake)
{
    Texture2D snakeAtlas = Assets::GetSnakeAtlas();
    for (int i = 1; i < snake->g_bodyPart.size(); i++) 
    {
        int pixelX = GAME_OFFSET + (snake->g_bodyPart[i].x * TILE_SIZE);
        int pixelY = GAME_OFFSET + (snake->g_bodyPart[i].y * TILE_SIZE);

        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, snakeColour);
    }

    int headX = GAME_OFFSET + (int)(snake->g_bodyPart[0].x * TILE_SIZE);
    int headY = GAME_OFFSET + (int)(snake->g_bodyPart[0].y * TILE_SIZE);

    if (snakeAtlas.id > 0)
    {
        // Get the correct sprite rectangle for the snake's direction
        Rectangle sourceRect = GetHeadSpriteRect(snake->g_currentDirection);

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
    Texture2D snakeAtlas = Assets::GetSnakeAtlas();

    if (snakeAtlas.id > 0)
    {
        Rectangle foodRect = Assets::GetSpriteRect(0, 2, TILE_SIZE);
        Rectangle destRect = { (float)pixelX, (float)pixelY, (float)TILE_SIZE, (float)TILE_SIZE };
        DrawTexturePro(snakeAtlas, foodRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, GREEN);
    }
}

void RenderGameplay(GameState* gameState)
{
    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawGameBoard();
    DrawSnake(&gameState->snake);
    DrawFood(&gameState->food);
    DrawGameUI(gameState->score, gameState->highScore);
    EndDrawing();
}

// Refactors raylib.h DrawRectangleLinesEx to take in two colours and create a bevel
// with a light colour in the top and left border, and darker in the bottom and right
void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour)
{
    Rectangle top = { borderRec.x, borderRec.y, borderRec.width, (float)borderThickness };
    Rectangle bottom = { borderRec.x, borderRec.y + borderRec.height - (float)borderThickness, borderRec.width, (float)borderThickness };
    Rectangle left = { borderRec.x, borderRec.y + (float)borderThickness, (float)borderThickness, borderRec.height - (2 * (float)borderThickness) };
    Rectangle right = { borderRec.x + borderRec.width - (float)borderThickness, borderRec.y + (float)borderThickness, (float)borderThickness, borderRec.height - (2 * (float)borderThickness) };

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



static void DrawCenteredTitle(const char* title, int y, int fontSize, Color color)
{
    const float fontSpacing = (float)fontSize / 10.0f;
    const int titleWidth = MeasureTextEx(GetFontDefault(), title, (float)fontSize, fontSpacing).x;
    const int titlePosX = (GetScreenWidth() / 2) - (titleWidth / 2);

    DrawTextWithShadow(title, titlePosX, y, fontSize, color);
}

static void RenderButton(const Button* button)
{
    const int borderWidth = 5;
    const float fontSize = 25.0f;
    const float fontSpacing = fontSize / 10.0f;

    const int textWidth = MeasureTextEx(GetFontDefault(), button->text, fontSize, fontSpacing).x;
    const int textHeight = MeasureTextEx(GetFontDefault(), button->text, fontSize, fontSpacing).y;
    const int textPosX = button->bounds.x + (button->bounds.width - textWidth) / 2;
    const int textPosY = button->bounds.y + (button->bounds.height - textHeight) / 2;

    Color buttonColour;
    Color textColour = WHITE;


    if (button->isHovered)
    {
        buttonColour = BUTTON_FACE_HOVER;
    }
    else
    {
        buttonColour = BUTTON_FACE_DEFAULT;
    }

    DrawRectangleRec(button->bounds, buttonColour);

    if (button->isPressed)
    {
        // Invert the light source, remove shadow and move text for a pressed in look
        DrawBeveledBorder(button->bounds, borderWidth, BORDER_DARK, BORDER_LIGHT);
        DrawText(button->text, textPosX + 2, textPosY + 2, fontSize, textColour);
    }
    else
    {
        // Standard top left light source look with a shadow
        DrawBeveledBorder(button->bounds, borderWidth, BORDER_LIGHT, BORDER_DARK);
        DrawTextWithShadow(button->text, textPosX, textPosY, fontSize, textColour);

    }
}

static void RenderAllButtons(const UI* ui)
{
    for (int i = 0; i < ui->buttonCount; i++)
    {
        RenderButton(&ui->buttons[i]);
    }
}


void RenderMainMenu(const UI* ui)
{
    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawCenteredTitle("SNAKE GAME", 250, 40, RED);
    RenderAllButtons(ui);
    EndDrawing();
}

void RenderGameOver(const UI* ui, int score, int highScore)
{
    const int scoreFontSize = 25;
    const int screenWidth = GetScreenWidth();

    BeginDrawing();
    ClearBackground(backgroundColour);
    DrawCenteredTitle("GAME OVER", 150, 40, RED);

    // Draw scores
    const char* scoreText = TextFormat("Final Score: %d", score);
    const char* highScoreText = TextFormat("High Score: %d", highScore);
    int scoreWidth = MeasureText(scoreText, scoreFontSize);
    int highScoreWidth = MeasureText(highScoreText, scoreFontSize);

    DrawTextWithShadow(scoreText, (screenWidth - scoreWidth) / 2, 200, scoreFontSize, RAYWHITE);
    DrawTextWithShadow(highScoreText, (screenWidth - highScoreWidth) / 2, 230, scoreFontSize, YELLOW);

    RenderAllButtons(ui);
    EndDrawing();
}
