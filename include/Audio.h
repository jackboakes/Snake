#ifndef AUDIO_H
#define AUDIO_H

enum SfxID
{
	SFX_EAT,
	SFX_COLLISION,
	SFX_MENU_BUTTON,
	SFX_COUNT // initiatise the size of audioSFX array in gamemanager master struct
};

void InitAudio(Sound audioSFX[]);
void ShutdownAudio(Sound audioSFX[]);

void PlaySoundRandomisedPitch(Sound sound);


#endif