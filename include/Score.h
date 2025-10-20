#pragma once

class Score
{
public:
	int LoadHighScore();
	bool SaveHighScore(int score);
	void CheckAndUpdateHighScore(int currentScore, int& highScore);
private:
	const char* m_File { "highscore.txt" };
};





