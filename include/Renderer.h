#ifndef RENDERER_H
#define RENDERER_H

// Menu global constants
#define TITLE_FONT_SIZE 40
#define OPTION_FRONT_SIZE 30
#define INSTRUCTION_FONT_SIZE 20
#define VERTICAL_SPACING 50
#define TITLE_Y 200

struct ButtonState 
{
	bool isHovered;
	bool isClicked;
};

// Resource management
static Rectangle GetSpriteRect(int col, int row);
void LoadGameTextures();
void UnloadGameTextures();
void UpdateWindowIcon();

void DrawTextWithShadow(const char* text, int posX, int posY, int fontSize, Color colour);

// Individual draw functions
static void DrawGameBoard();
static void DrawGameUI(int score, int highScore);
static Rectangle GetHeadSpriteRect(Direction direction);
static void DrawSnake(const Snake* snake);
static void DrawFood(const Food* food);
ButtonState UpdateButton(Rectangle buttonBounds);
void DrawBeveledBorder(Rectangle borderRec, int borderThickness, Color lightColour, Color darkColour);
void RenderButton(Rectangle bounds, const char* text, ButtonState buttonState);

// Rendering functions
void RenderMainMenu();
void RenderGameplay(GameManager* gameManager);
void RenderGameOver(int selectedOption, int score, int highScore);

#endif