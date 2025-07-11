#ifndef RENDERER_H
#define RENDERER_H

// Menu global constants
#define TITLE_FONT_SIZE 40
#define OPTION_FRONT_SIZE 30
#define INSTRUCTION_FONT_SIZE 20
#define VERTICAL_SPACING 50
#define TITLE_Y 200


// Resource management
void LoadGameTextures();
void UnloadGameTextures();
void UpdateWindowIcon();

// Individual draw functions
static void DrawGameBoard();
static void DrawGameUI(int score, int highScore);
static Rectangle GetHeadSpriteRect(Direction direction);
static void DrawSnake(const Snake* snake);
static void DrawFood(const Food* food);

// Rendering functions
void RenderMainMenu(int selectedOption);
void RenderGameplay(GameManager* gameManager);
void RenderGameOver(int selectedOption, int score, int highScore);

#endif