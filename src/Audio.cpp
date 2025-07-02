#include "raylib.h"
#include "GameManager.h"
#include "Assets.h"

// TODO: if you need more sounds refactor the master struct to store them in a vector
void InitAudio(GameState* gameState)
{
	InitAudioDevice();
	gameState->eatSound = LoadSound(GetAssetPath("sfx_snake_eat.wav"));
	gameState->collisionSound = LoadSound(GetAssetPath("sfx_snake_collision.wav"));
}

void ShutdownAudio(GameState* gameState)
{
	UnloadSound(gameState->collisionSound);
	UnloadSound(gameState->eatSound);
	CloseAudioDevice();
}

// Helper function to reduce audio fatigue on repeated sounds
void RandomiseSoundPitch(int lowerBound, int upperBound, Sound sound)
{
	float pitch = GetRandomValue(lowerBound, upperBound) / 100.0f;
	SetSoundPitch(sound, pitch);
}

void PlayEatSound(GameState* gameState)
{
	const int lowerPitch = 95;
	const int higherPitch = 105;
	RandomiseSoundPitch(lowerPitch, higherPitch, gameState->eatSound);
	PlaySound(gameState->eatSound);
}

void PlayCollisionSound(GameState* gameState)
{
	const int lowerPitch = 95;
	const int higherPitch = 105;
	RandomiseSoundPitch(lowerPitch, higherPitch, gameState->collisionSound);
	PlaySound(gameState->collisionSound);
}

