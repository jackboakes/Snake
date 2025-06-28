#include <raylib.h>
#include "GameManager.h"

#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 768


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60); 

    GameManager gameManager;
    InitGameManager(&gameManager);
    RunGameManager(&gameManager);
    ShutdownGameManager(&gameManager);

    return 0;
}

