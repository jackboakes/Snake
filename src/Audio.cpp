
#include "Audio.h"
#include "Assets.h" // get asset path helper



Audio::Audio()
{
	InitAudioDevice();

	m_audioSFX[static_cast<int>(SFXID::EAT)] = LoadSound(GetAssetPath("sfx_snake_eat.wav"));
	m_audioSFX[static_cast<int>(SFXID::COLLISION)] = LoadSound(GetAssetPath("sfx_snake_collision.wav"));
	m_audioSFX[static_cast<int>(SFXID::MENU_BUTTON)] = LoadSound(GetAssetPath("sfx_menu_button_pressed.wav"));
}

Audio::~Audio()
{
	for (int i = 0; i < m_audioSFX.size(); i++)
	{
		UnloadSound(m_audioSFX[i]);
	}
	CloseAudioDevice();
}

// Helper function to reduce audio fatigue on repeated sounds
void Audio::RandomiseSoundPitch(int lowerBound, int upperBound, Sound& sound)
{
	float pitch = GetRandomValue(lowerBound, upperBound) / 100.0f;
	SetSoundPitch(sound, pitch);
}

// Plays a sound with a pitch between 95 and 105 to reduce audio fatigue on repeated sounds
void Audio::PlaySoundRandomisedPitch(SFXID id)
{
	constexpr int lowerPitch = 95;
	constexpr int higherPitch = 105;
	Sound& sound = m_audioSFX[static_cast<int>(id)];
	RandomiseSoundPitch(lowerPitch, higherPitch, sound);
	PlaySound(sound);
}

