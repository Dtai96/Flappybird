#pragma once
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

 
  
class SoundCoin
{
public:
	static void PlayShortSoundEffect(const CString& filePath);
};

