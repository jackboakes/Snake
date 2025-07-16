#ifndef AUDIO_H
#define AUDIO_H

void InitAudio(GameManager* gameManager);
void ShutdownAudio(GameManager* gameManager);
static void RandomiseSoundPitch(int lowerBound, int upperBound, Sound sound);
void PlayEatSound(GameState* gameState);
void PlayCollisionSound(GameState* gameState);
void PlayButtonSound(Sound buttonSound);

#endif