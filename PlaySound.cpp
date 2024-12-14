#include "pch.h"
#include "PlaySound.h"

void PlaySound::Play(const CString& filePath, bool loop)
{
    // Tạo alias "music" cho file
    CString command;
    command.Format(_T("open \"%s\" type mpegvideo alias music"), filePath);
    mciSendString(command, NULL, 0, NULL);

    // Phát nhạc
    if (loop) {
        mciSendString(_T("play music repeat"), NULL, 0, NULL);  // Lặp
    }
    else {
        mciSendString(_T("play music"), NULL, 0, NULL);  // Không lặp
    }
}

void PlaySound::Stop()
{
    mciSendString(_T("stop music"), NULL, 0, NULL);
    mciSendString(_T("close music"), NULL, 0, NULL);
}

void PlaySound::Coin(const CString& filePath) {
    CString command;
    command.Format(_T("play \"%s\" from 0"), filePath);
    mciSendString(command, NULL, 0, NULL);

    // Đóng ngay sau khi phát xong
    command.Format(_T("close \"%s\""), filePath);
    mciSendString(command, NULL, 0, NULL);
}
void PlaySound::PreloadSoundEffect(const CString& filePath, const CString& alias) {
    CString command;
    command.Format(_T("open \"%s\" type waveaudio alias %s"), filePath, alias);
    mciSendString(command, NULL, 0, NULL); // Chỉ mở file một lần
}

void  PlaySound::PlaySoundEffectWithAlias(const CString& alias) {
    CString command;
    command.Format(_T("play %s from 0"), alias);
    mciSendString(command, NULL, 0, NULL); // Phát âm thanh
}

void  PlaySound::UnloadSoundEffect(const CString& alias) {
    CString command;
    command.Format(_T("close %s"), alias);
    mciSendString(command, NULL, 0, NULL); // Đóng file khi không cần nữa
}