#pragma once
#pragma once
#include <mmsystem.h>
#include <string>
#pragma comment(lib, "winmm.lib")

class PlaySound {
public:
    static void Play(const CString& filePath, bool loop = false);
    static void Stop();
    static void Coin(const CString& filePath);

    static void PreloadSoundEffect(const CString& filePath, const CString& alias);


    static void PlaySoundEffectWithAlias(const CString& alias);

    static void UnloadSoundEffect(const CString& alias);
   
};
