#pragma once
#include "raylib.h"
#include <array>

class Audio
{
public:
	enum class SFXID
	{
		EAT,
		COLLISION,
		MENU_BUTTON,
		COUNT
	};

	Audio();
	~Audio();

	void PlaySoundRandomisedPitch(SFXID id);

private:
	std::array<Sound, static_cast<int>(SFXID::COUNT)> m_audioSFX;
	void RandomiseSoundPitch(int lowerBound, int upperBound, Sound& sound);
};







