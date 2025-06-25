#include "Score.h"
#include <raylib.h>
#include <stdio.h> // sprintf
#include <string> // std::stoi

#define HIGHSCORE_FILE "highscore.txt"

int LoadHighScore(void)
{
    int highScore = 0;

    if (FileExists(HIGHSCORE_FILE))
    {
        char* fileText = LoadFileText(HIGHSCORE_FILE);

        if (fileText != NULL)
        {
            highScore = std::stoi(fileText);
            UnloadFileText(fileText);
        }
    }
    else
    {
        // Create file with default value if it doesn't exist
        SaveHighScore(0);
    }

    return highScore;
}

bool SaveHighScore(int score)
{
    char scoreText[32];
    sprintf(scoreText, "%d", score);

    return SaveFileText(HIGHSCORE_FILE, scoreText);
}

void CheckAndUpdateHighScore(int currentScore, int* highScore)
{
    if (currentScore > *highScore)
    {
        *highScore = currentScore;
        SaveHighScore(*highScore);
    }
}