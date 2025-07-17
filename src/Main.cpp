#include "GameManager.h"


int main()
{
    GameManager gameManager;
    InitGameManager(&gameManager);
    RunGameManager(&gameManager);
    ShutdownGameManager(&gameManager);
}

