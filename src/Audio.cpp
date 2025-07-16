#include "raylib.h"
#include "GameManager.h"
#include "Assets.h"

// TODO: if you need more sounds refactor the master struct to store them in a vector
void InitAudio(GameManager* gameManager)
{
	InitAudioDevice();
	gameManager->gameState.eatSound = LoadSound(GetAssetPath("sfx_snake_eat.wav"));
	gameManager->gameState.collisionSound = LoadSound(GetAssetPath("sfx_snake_collision.wav"));
	gameManager->mainMenuUI.buttonSound = LoadSound(GetAssetPath("sfx_menu_button_pressed.wav"));
	gameManager->gameOverUI.buttonSound = LoadSound(GetAssetPath("sfx_menu_button_pressed.wav"));
}

void ShutdownAudio(GameManager* gameManager)
{
	UnloadSound(gameManager->gameState.eatSound);
	UnloadSound(gameManager->gameState.collisionSound);
	UnloadSound(gameManager->mainMenuUI.buttonSound);
	UnloadSound(gameManager->gameOverUI.buttonSound);
	CloseAudioDevice();
}

// Helper function to reduce audio fatigue on repeated sounds
static void RandomiseSoundPitch(int lowerBound, int upperBound, Sound sound)
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

void PlayButtonSound(Sound buttonSound)
{
	const int lowerPitch = 95;
	const int higherPitch = 105;
	RandomiseSoundPitch(lowerPitch, higherPitch, buttonSound);
	PlaySound(buttonSound);
}

