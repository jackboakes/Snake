#include "raylib.h"
#include "Score.h"
#include <stdio.h> // sprintf



int LoadHighScore(void)
{
    int highScore = 0;

    if (FileExists(HIGHSCORE_FILE))
    {
        char* fileText = LoadFileText(HIGHSCORE_FILE);

        if (fileText != NULL)
        {
            highScore = TextToInteger(fileText); // replaced stoi
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
    sprintf_s(scoreText, sizeof(scoreText), "%d", score);;

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