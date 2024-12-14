#include "pch.h"
#include "SoundCoin.h"

void SoundCoin::PlayShortSoundEffect(const CString& filePath)
{
	PlaySound(filePath, NULL, SND_FILENAME | SND_ASYNC);
}
