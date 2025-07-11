#ifndef AUDIO_H
#define AUDIO_H

void InitAudio(GameState* gameState);
void ShutdownAudio(GameState* gameState);
static void RandomiseSoundPitch(int lowerBound, int upperBound, Sound sound);
void PlayEatSound(GameState* gameState);
void PlayCollisionSound(GameState* gameState);

#endif