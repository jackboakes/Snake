#include "raylib.h"
#include "Audio.h"
#include "Assets.h"



void InitAudio(Sound audioSFX[])
{
	InitAudioDevice();

	audioSFX[SFX_EAT] = LoadSound(GetAssetPath("sfx_snake_eat.wav"));
	audioSFX[SFX_COLLISION] = LoadSound(GetAssetPath("sfx_snake_collision.wav"));
	audioSFX[SFX_MENU_BUTTON] = LoadSound(GetAssetPath("sfx_menu_button_pressed.wav"));
}

void ShutdownAudio(Sound audioSFX[])
{
	for (int i = 0; i < SFX_COUNT; i++)
	{
		UnloadSound(audioSFX[i]);
	}
	CloseAudioDevice();
}

// Helper function to reduce audio fatigue on repeated sounds
static void RandomiseSoundPitch(int lowerBound, int upperBound, Sound sound)
{
	float pitch = GetRandomValue(lowerBound, upperBound) / 100.0f;
	SetSoundPitch(sound, pitch);
}

// Plays a sound with a pitch between 95 and 105 to reduce audio fatigue on repeated sounds
void PlaySoundRandomisedPitch(Sound sound)
{
	const int lowerPitch = 95;
	const int higherPitch = 105;
	RandomiseSoundPitch(lowerPitch, higherPitch, sound);
	PlaySound(sound);
}

