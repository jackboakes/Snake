#include "raylib.h"
#include "Score.h"


int Score::LoadHighScore()
{
    int highScore = 0;

    if (FileExists(m_File))
    {
        char* fileText = LoadFileText(m_File);

        if (fileText != NULL)
        {
            highScore = TextToInteger(fileText); 
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

bool Score::SaveHighScore(int score)
{
    char scoreText[32];
    TextCopy(scoreText, TextFormat("%d", score));

    return SaveFileText(m_File, scoreText);
}

void Score::CheckAndUpdateHighScore(int currentScore, int& highScore)
{
    if (currentScore > highScore)
    {
        highScore = currentScore;
        if (!SaveHighScore(highScore)) {
            TraceLog(LOG_WARNING, "Failed to save high score");
        }
        SaveHighScore(highScore);
    }
}