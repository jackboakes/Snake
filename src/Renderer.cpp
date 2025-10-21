#include "raylib.h"
#include "GameManager.h"
#include "Renderer.h"
#include "GameState.h"
#include "Assets.h"
#include "UI.h"
#include <string>


void Renderer::DrawGameBoard()
{
    for (int x { 0 }; x < GRID_SIZE; x++)
    {
        for (int y { 0 }; y < GRID_SIZE; y++)
        {
            int posX { GAME_OFFSET + (x * TILE_SIZE) };
            int posY { GAME_OFFSET + (y * TILE_SIZE) };

            Color tileColor { BOARD_COLOURS[(x + y) % 2] };
            DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, tileColor);
        }
    }

    Rectangle borderRect {
            GAME_OFFSET - BORDER_THICKNESS,
            GAME_OFFSET - BORDER_THICKNESS,
            GAME_WIDTH + (BORDER_THICKNESS * 2),
            GAME_HEIGHT + (BORDER_THICKNESS * 2)
    };

    Renderer::DrawBeveledBorder(borderRect, BORDER_THICKNESS, BORDER_COLOUR, BORDER_DARK);
}

void Renderer::DrawGameUI(int score, int highScore)
{
    {
        const int fontSize { 40 };
        const int yPosition { 20 };

        std::string scoreText { "Score: " + std::to_string(score) };
        std::string highScoreText { "High Score: " + std::to_string(highScore) };
 
        int highScoreTextWidth { MeasureText(highScoreText.c_str(), fontSize)};

        int leftEdge { GAME_OFFSET - 5 };
        int rightEdge { GAME_OFFSET + GAME_WIDTH + 5 }; // Right edge of game area plus margin
        int rightAlignedX { rightEdge - highScoreTextWidth };

        Renderer::DrawTextWithShadow(scoreText.c_str(), leftEdge, yPosition, fontSize, RAYWHITE);
        Renderer::DrawTextWithShadow(highScoreText.c_str(), rightAlignedX, yPosition, fontSize, YELLOW);
    }
}

// Returns the rectangle of the snakes head sprites based on direction of the snake
Rectangle Renderer::GetHeadSpriteRect(Direction direction)
{
    switch (direction)
    {
    case Direction::NORTH: return Assets::GetSpriteRect(0, 0, static_cast<float>(TILE_SIZE));  // head facing up
    case Direction::WEST:  return Assets::GetSpriteRect(1, 0, static_cast<float>(TILE_SIZE));  // head facing left
    case Direction::SOUTH: return Assets::GetSpriteRect(2, 0, static_cast<float>(TILE_SIZE));  // head facing down
    case Direction::EAST:  return Assets::GetSpriteRect(3, 0, static_cast<float>(TILE_SIZE));  // head facing right
    default:        return Assets::GetSpriteRect(0, 0, static_cast<float>(TILE_SIZE));  // default
    }
}
    
void Renderer::DrawSnake(const Snake& snake)
{
    Texture2D snakeAtlas { Assets::GetSnakeAtlas() };
    for (int i { 1 }; i < snake.bodyPart.size(); i++)
    {
        int pixelX { GAME_OFFSET + (snake.bodyPart[i].x * TILE_SIZE) };
        int pixelY { GAME_OFFSET + (snake.bodyPart[i].y * TILE_SIZE) };

        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, SNAKE_COLOUR);
    }

    int headX { GAME_OFFSET + (snake.bodyPart[0].x * TILE_SIZE) };
    int headY { GAME_OFFSET + (snake.bodyPart[0].y * TILE_SIZE) };

    if (snakeAtlas.id > 0)
    {
        // Get the correct sprite rectangle for the snake's direction
        Rectangle sourceRect { GetHeadSpriteRect(snake.currentDirection) };

        // Destination rectangle (where to draw on screen)
        Rectangle destRect { (float)headX, (float)headY, (float)TILE_SIZE, (float)TILE_SIZE };

        // Draw the sprite (no rotation needed since we have pre-rotated sprites)
        DrawTexturePro(snakeAtlas, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        // Fallback to rounded rectangle if texture isn't loaded
        Rectangle headPos { (float)headX, (float)headY, (float)TILE_SIZE, (float)TILE_SIZE };
        DrawRectangleRounded(headPos, 0.5, 6, SNAKE_COLOUR);
    }
}

void Renderer::DrawFood(const Food& food)
{
    int pixelX { GAME_OFFSET + (food.position.x * TILE_SIZE) };
    int pixelY { GAME_OFFSET + (food.position.y * TILE_SIZE) };
    Texture2D snakeAtlas { Assets::GetSnakeAtlas() };

    if (snakeAtlas.id > 0)
    {
        Rectangle foodRect { Assets::GetSpriteRect(0, 2, static_cast<float>(TILE_SIZE)) };
        Rectangle destRect { (float)pixelX, (float)pixelY, (float)TILE_SIZE, (float)TILE_SIZE };
        DrawTexturePro(snakeAtlas, foodRect, destRect, { 0, 0 }, 0.0f, WHITE);
    }
    else
    {
        DrawRectangle(pixelX, pixelY, TILE_SIZE, TILE_SIZE, GREEN);
    }
}

void Renderer::DrawCenteredTitle(const char* title, int y, int fontSize, Color color)
{
    const float fontSpacing { (float)fontSize / 10.0f };
    const int titleWidth { static_cast<int>(MeasureTextEx(GetFontDefault(), title, (float)fontSize, fontSpacing).x) };
    const int titlePosX { static_cast<int>((GetScreenWidth() / 2) - (titleWidth / 2)) };

    Renderer::DrawTextWithShadow(title, titlePosX, y, fontSize, color);
}

void Renderer::RenderButton(const Button & button)
{
    constexpr int borderWidth { 5 };
    constexpr float fontSize { 25.0f };
    constexpr float fontSpacing { fontSize / 10.0f };

    const int textWidth { static_cast<int>(MeasureTextEx(GetFontDefault(), button.text, fontSize, fontSpacing).x) };
    const int textHeight { static_cast<int>(MeasureTextEx(GetFontDefault(), button.text, fontSize, fontSpacing).y) };
    const int textPosX { static_cast<int>(button.bounds.x + (button.bounds.width - textWidth) / 2) };
    const int textPosY { static_cast<int>(button.bounds.y + (button.bounds.height - textHeight) / 2) };

    Color buttonColour { BUTTON_FACE_DEFAULT };
    Color textColour { WHITE };

    // Determines border and text styling
    bool drawPressed { false };

    switch (button.GetState())
    {
    case Button::State::HOVERED:
        buttonColour = BUTTON_FACE_HOVER;
        break;
    case Button::State::PRESSED:
        buttonColour = BUTTON_FACE_HOVER;
        drawPressed = true;
        break;
    case Button::State::RELEASED:
        buttonColour = BUTTON_FACE_HOVER;
        break;
    case Button::State::DEFAULT:
        buttonColour = BUTTON_FACE_DEFAULT;
        break;
    }

    DrawRectangleRec(button.bounds, buttonColour);

    if (drawPressed)
    {
        // Invert the light source, remove shadow and move text for a pressed in look
        Renderer::DrawBeveledBorder(button.bounds, borderWidth, BORDER_DARK, BORDER_LIGHT);
        DrawText(button.text, textPosX + 2, textPosY + 2, static_cast<int>(fontSize), textColour);
    }
    else
    {
        // Standard top left light source look with a shadow
        Renderer::DrawBeveledBorder(button.bounds, borderWidth, BORDER_LIGHT, BORDER_DARK);
        Renderer::DrawTextWithShadow(button.text, textPosX, textPosY, static_cast<int>(fontSize), textColour);

    }
}

void Renderer::RenderAllButtons(const UI & ui)
{
    const std::vector<Button>& buttons = ui.GetButtons();

    for (const Button& button : buttons)
    {
        RenderButton(button);
    }
}


void Renderer::Gameplay(GameState& gameState)
{
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOUR);
    DrawGameBoard();
    DrawSnake(gameState.snake);
    DrawFood(gameState.food);
    DrawGameUI(gameState.score, gameState.highScore);
    EndDrawing();
}

void Renderer::MainMenu(const UI& ui)
{
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOUR);
    DrawCenteredTitle("SNAKE GAME", 250, 40, RED);
    RenderAllButtons(ui);
    EndDrawing();
}

void Renderer::GameOver(const UI& ui, int score, int highScore)
{
    constexpr int scoreFontSize { 25 };
    const int screenWidth { GetScreenWidth() };

    BeginDrawing();
    ClearBackground(BACKGROUND_COLOUR);
    DrawCenteredTitle("GAME OVER", 150, 40, RED);

    // Draw scores
    std::string scoreText { "Score: " + std::to_string(score) };
    std::string highScoreText { "High Score: " + std::to_string(highScore) };
    int scoreWidth { MeasureText(scoreText.c_str(), scoreFontSize) };
    int highScoreWidth { MeasureText(highScoreText.c_str(), scoreFontSize) };

    DrawTextWithShadow(scoreText.c_str(), (screenWidth - scoreWidth) / 2, 200, scoreFontSize, RAYWHITE);
    DrawTextWithShadow(highScoreText.c_str(), (screenWidth - highScoreWidth) / 2, 230, scoreFontSize, YELLOW);

    RenderAllButtons(ui);
    EndDrawing();
}

// Refactors raylib.h DrawRectangleLinesEx to take in two colours and create a bevel
// with a light colour in the top and left border, and darker in the bottom and right
void Renderer::DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour)
{
    Rectangle top { borderRec.x, borderRec.y, borderRec.width, (float)borderThickness };
    Rectangle bottom { borderRec.x, borderRec.y + borderRec.height - (float)borderThickness, borderRec.width, (float)borderThickness };
    Rectangle left { borderRec.x, borderRec.y + (float)borderThickness, (float)borderThickness, borderRec.height - (2 * (float)borderThickness) };
    Rectangle right { borderRec.x + borderRec.width - (float)borderThickness, borderRec.y + (float)borderThickness, (float)borderThickness, borderRec.height - (2 * (float)borderThickness) };

    DrawRectangleRec(top, lightColour);
    DrawRectangleRec(bottom, darkColour);
    DrawRectangleRec(left, lightColour);
    DrawRectangleRec(right, darkColour);
}

void Renderer::DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour)
{
    Color const shadowColor = { 0, 0, 0, 128 }; // black with half alpha

    DrawText(text, posX + 2, posY + 2, fontSize, shadowColor);
    DrawText(text, posX, posY, fontSize, colour);
}