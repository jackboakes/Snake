#ifndef SCORE_H
#define SCORE_H

#define STORAGE_DATA_FILE "highscore.txt"


int LoadHighScore(void);
bool SaveHighScore(int score);
void CheckAndUpdateHighScore(int currentScore, int* highScore);

#endif