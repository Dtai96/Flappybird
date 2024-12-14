#pragma once
#include "Bird.h"
#include "Pipe.h"
#include <vector>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
class GameManager
{
public:
    void StartBackgroundMusic();
    void PreloadSounds();
    void PlayScoreSound();
    void UnloadSounds();
    void StopBackgroundMusic();
    void InitGame();
    GameManager();
    void Update();
    void IncreaseScore();
    void Draw(CDC* pDC);
    void OnKeyDown(UINT nChar);
    bool isGameOver;
    void StartGame();
    bool isMenu = true;  // Bắt đầu ở trạng thái menu
    void DrawMenu(CDC* pDC);  // Hàm vẽ menu
    void FlySound();
private:
    Bird bird;
    std::vector<Pipe> pipes;
    int score;

    void CheckCollisions();
    void GeneratePipe();
};

