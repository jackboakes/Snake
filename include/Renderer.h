#ifndef RENDERER_H
#define RENDERER_H

// Menu global constants
#define TITLE_FONT_SIZE 40
#define OPTION_FRONT_SIZE 30
#define INSTRUCTION_FONT_SIZE 20
#define VERTICAL_SPACING 50
#define TITLE_Y 200


extern Color const snakeColour;

Color const boardColors[2] = {
    { 0x49, 0x43, 0x51, 0xFF },  // light
    { 0x44, 0x3E, 0x4C, 0xFF }   // dark
};


// Resource management
void LoadGameTextures();
void UnloadGameTextures();

// Individual draw functions
void DrawGameBoard();
void DrawGameUI(int score, int highScore);
Rectangle GetHeadSpriteRect(Direction direction);
void DrawSnake(const Snake* snake);
void DrawFood(const Food* food);

//rendering functions
void RenderMainMenu(int selectedOption);
void RenderGameplay(GameManager* gameManager);
void RenderGameOver(int selectedOption, int score, int highScore);

#endif