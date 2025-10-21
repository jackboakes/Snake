#include "raylib.h"
#include "Score.h"
#include <fstream>
#include <filesystem>
#include <string>

int Score::LoadHighScore()
{
    int highScore = 0;

    if (std::filesystem::exists(m_file))
    {
        std::ifstream inFile { m_file };
        if (inFile)
        {
            inFile >> highScore;
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
    std::ofstream outFile(m_file);

    if (outFile)
    {
        outFile << score;
        return true;
    }

    return false;
}

void Score::CheckAndUpdateHighScore(int currentScore, int& highScore)
{
    if (currentScore > highScore)
    {
        highScore = currentScore;
        if (!SaveHighScore(highScore)) 
        {
            TraceLog(LOG_WARNING, "Failed to save high score");
        }
    }
}