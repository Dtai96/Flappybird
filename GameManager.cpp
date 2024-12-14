#include "pch.h"
#include "GameManager.h"
#include "PlaySound.h"
#include "SoundCoin.h"
#include <windows.h> // Để sử dụng SetPriorityClass
#include <mmsystem.h>  // Thư viện âm thanh Windows
#pragma comment(lib, "winmm.lib")  // Liên kết thư viện winmm.lib


//void GameManager::StartBackgroundMusic() {
//    PlaySound(_T(".\\res\\background.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
//}
void GameManager::PreloadSounds() {
    // Mở file âm thanh trước và gán alias
    mciSendString(_T("open .\\res\\coin.wav type waveaudio alias coinEffect"), NULL, 0, NULL);
}

void GameManager::PlayScoreSound() {
    // Chỉ phát lại âm thanh đã mở
    mciSendString(_T("play coinEffect from 0"), NULL, 0, NULL);
}

void GameManager::UnloadSounds() {
    // Đóng âm thanh khi không cần nữa
    mciSendString(_T("close coinEffect"), NULL, 0, NULL);
}
void GameManager::StopBackgroundMusic() {
    PlaySound(NULL, NULL, 0);  // Dừng phát nhạc
}
void GameManager::InitGame() {
    // Thiết lập độ ưu tiên cao cho quá trình
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    PreloadSounds();
    // Khởi tạo các giá trị ban đầu
    score = 0;
    isGameOver = false;
    pipes.clear();
    pipes.push_back(Pipe(400, rand() % 300 + 100));
    pipes.push_back(Pipe(700, rand() % 300 + 100));
    pipes.push_back(Pipe(1000, rand() % 300 + 100));
    
}
GameManager::GameManager()  {
    isMenu = true;  // Bắt đầu ở giao diện menu
    isGameOver = false;  // Trò chơi chưa kết thúc
    InitGame();
}

void GameManager::Update() {
    if (isGameOver)
        return;
      
    bird.Update();
    for (auto& pipe : pipes) {
        pipe.Update();
    }

    // Nếu ống đầu tiên đã đi ra ngoài màn hình, xóa nó và tạo thêm ống mới
    if (pipes.front().x + pipes.front().width < 0) {
        pipes.erase(pipes.begin());  // Xóa ống đầu tiên
        GeneratePipe();  // Tạo ống mới
        IncreaseScore();
    }

    CheckCollisions();
}
void GameManager::IncreaseScore() {
    score++;  // Tăng điểm số
    PlayScoreSound();
    
}

void GameManager::Draw(CDC* pDC) {

    if (isMenu) {
        //StartBackgroundMusic();  // Bắt đầu nhạc nền
        PlaySound::Play(_T(".\\res\\background.wav"), true);  // Phát lặp
        DrawMenu(pDC);  // Nếu đang ở menu, vẽ menu
    }
    else {
       
        PlaySound::Stop();
       //StopBackgroundMusic();
        // Tạo đối tượng CImage để chứa hình ảnh nền
        CImage backgroundImage;

        // Tải hình ảnh vào CImage (Đảm bảo đường dẫn đúng)
        HRESULT hr = backgroundImage.Load(_T(".\\res\\background.png"));

        // Nếu tải hình ảnh thành công, vẽ nó vào bối cảnh vẽ
        if (SUCCEEDED(hr)) {
            // Vẽ hình ảnh nền vào vùng cửa sổ (kích thước bằng kích thước client)
            int width = pDC->GetDeviceCaps(HORZRES);
            int height = pDC->GetDeviceCaps(VERTRES);

            // Giới hạn kích thước của hình nền nếu cần
            int maxWidth = 1920;
            int maxHeight = 650;
            width = min(width, maxWidth);
            height = min(height, maxHeight);

            backgroundImage.Draw(pDC->m_hDC, 0, 0, width, height);
        }
        else {
            // Nếu không tải được hình ảnh, có thể in ra thông báo lỗi hoặc thay thế bằng màu nền
            TRACE("Không thể tải hình ảnh nền\n");
        }

        // Vẽ các đối tượng trong trò chơi
        bird.Draw(pDC);  // Vẽ chim

        // Kiểm tra và giới hạn vị trí của các ống
        for (auto& pipe : pipes) {
            pipe.Draw(pDC);  // Vẽ các ống
        }

        // Vẽ điểm số
        CString scoreText;
        scoreText.Format(_T("Score: %d"), score);
        pDC->TextOutW(10, 10, scoreText);
        if (isGameOver) {
            UnloadSounds();
            CString gameOverText = _T("GAME OVER");
            CString ScoreText;
            ScoreText.Format(_T("Your Score : %d"), score);
            CString ReplayText = _T("Nhấn Enter để chơi lại");
            CFont font;
            font.CreateFont(
                48,                        // Chiều cao font
                0,                         // Chiều rộng font
                0,                         // Góc xoay
                0,                         // Góc chữ
                FW_BOLD,                   // Độ đậm
                FALSE,                     // Chữ in nghiêng
                FALSE,                     // Gạch chân
                FALSE,                     // Gạch ngang
                ANSI_CHARSET,              // Bộ ký tự
                OUT_DEFAULT_PRECIS,        // Độ chính xác đầu ra
                CLIP_DEFAULT_PRECIS,       // Độ chính xác cắt
                DEFAULT_QUALITY,           // Chất lượng
                DEFAULT_PITCH | FF_SWISS,  // Phong cách
                _T("Arial"));              // Font chữ

            CFont* pOldFont = pDC->SelectObject(&font);

            // Tô màu nền
            CRect Rect;
            pDC->GetClipBox(&Rect);

            // Vẽ chữ "GAME OVER"
            pDC->SetTextColor(RGB(255, 0, 0));  // Màu đỏ
            pDC->SetBkMode(TRANSPARENT);        // Nền chữ trong suốt
            pDC->DrawText(gameOverText, &Rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Vẽ điểm số
            Rect.top += 100; // Dịch phần điểm số xuống dưới một chút
            pDC->SetTextColor(RGB(255, 0, 0)); // Màu trắng
            pDC->DrawText(ScoreText, &Rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Vẽ dòng chữ "Nhấn Enter để chơi lại"
            Rect.top += 150; // Dịch dòng này xuống dưới phần điểm số
            pDC->SetTextColor(RGB(200, 200, 200)); // Màu xám nhạt
            pDC->DrawText(ReplayText, &Rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            pDC->SelectObject(pOldFont);

        }
    }

}

void GameManager::OnKeyDown(UINT nChar) { 
if (nChar == VK_SPACE)
{
    
    bird.Flap();  // Chim nhảy (nếu có logic)
}
else if (isMenu && nChar == VK_RETURN)
{
    StartGame();  // Bắt đầu trò chơi
}
else if (isMenu && nChar == VK_ESCAPE)
{
    AfxGetMainWnd()->PostMessage(WM_CLOSE);  // Thoát game
}


}

void GameManager::CheckCollisions() {
    for (auto& pipe : pipes) {
        if (pipe.CheckCollision(bird.position, bird.radius)) {
            isGameOver = true;
            break;
        }
    }
}
void GameManager::StartGame()
{
    isMenu = false;      // Thoát khỏi menu
    isGameOver = false;    // Đặt trạng thái trò chơi thành chưa kết thúc
    score = 0;             // Đặt lại điểm số
    bird.Reset();          // Đặt lại vị trí và trạng thái của chim
    pipes.clear();         // Xóa danh sách các ống
    InitGame(); // Khởi tạo ống ban đầu
}
void GameManager::DrawMenu(CDC* pDC)
{    // Tạo đối tượng CImage để chứa hình ảnh nền
    CImage backgroundImage;

    // Tải hình ảnh vào CImage (Đảm bảo đường dẫn đúng)
    HRESULT hr = backgroundImage.Load(_T(".\\res\\background.png"));

    if (SUCCEEDED(hr))
    {
        // Lấy kích thước vùng client để vẽ nền
        CRect clientRect;
        pDC->GetClipBox(&clientRect);

        // Giới hạn kích thước của hình nền nếu cần
        int width = clientRect.Width();
        int height = clientRect.Height();
        int maxWidth = 1920;
        int maxHeight = 650;
        width = min(width, maxWidth);
        height = min(height, maxHeight);

        // Vẽ hình ảnh nền
        backgroundImage.Draw(pDC->m_hDC, 0, 0, width, height);
    }
    else
    {
        // Nếu không tải được hình nền, tô nền màu xanh da trời
        CBrush blueBrush(RGB(135, 206, 250));  // Màu xanh da trời
        CRect clientRect;
        pDC->GetClipBox(&clientRect);
        pDC->FillRect(&clientRect, &blueBrush);
    }

    // Vẽ các thành phần giao diện menu
    CString title = _T("FLAPPY BIRD ");
    // Tạo đối tượng CImage để chứa hình ảnh biểu tượng
    CImage birdImage;
    HRESULT hrBird = birdImage.Load(_T(".\\res\\Menu.png"));
    if (SUCCEEDED(hrBird)) {
        // Vẽ biểu tượng vào vị trí menu
        birdImage.Draw(pDC->m_hDC, 630, 20, 150, 150);  // Tọa độ và kích thước của biểu tượng
    }
    else {
        TRACE("Không thể tải hình ảnh biểu tượng.\n");
    }
    CString startText = _T("Press ENTER to Start");
    CString quitText = _T("Press ESC to Quit");

    // Thiết lập font chữ
    CFont font;
    font.CreateFont(
        64,                        // Chiều cao font
        0,                         // Chiều rộng font
        0,                         // Góc xoay chữ
        0,                         // Góc cơ bản
        FW_BOLD,                   // Độ đậm
        FALSE,                     // Không in nghiêng
        FALSE,                     // Không gạch chân
        FALSE,                     // Không gạch ngang
        ANSI_CHARSET,              // Bộ ký tự
        OUT_DEFAULT_PRECIS,        // Độ chính xác đầu ra
        CLIP_DEFAULT_PRECIS,       // Độ chính xác cắt
        DEFAULT_QUALITY,           // Chất lượng
        DEFAULT_PITCH | FF_SWISS,  // Kiểu font
        _T("Arial"));              // Tên font

    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->SetTextColor(RGB(255, 255, 255));  // Màu chữ trắng
    pDC->SetBkMode(TRANSPARENT);            // Nền chữ trong suốt

    // Lấy kích thước vùng vẽ
    CRect clientRect;
    pDC->GetClipBox(&clientRect);

    // Vẽ tiêu đề
    CRect titleRect = clientRect;
    titleRect.top += 180;
    pDC->DrawText(title, &titleRect, DT_CENTER | DT_SINGLELINE);

    // Vẽ hướng dẫn bắt đầu
    CRect startRect = clientRect;
    startRect.top += 350;
    pDC->DrawText(startText, &startRect, DT_CENTER | DT_SINGLELINE);

    // Vẽ hướng dẫn thoát
    CRect quitRect = clientRect;
    quitRect.top += 450;
    pDC->DrawText(quitText, &quitRect, DT_CENTER | DT_SINGLELINE);

    // Hoàn tác font
    pDC->SelectObject(pOldFont);
}
void GameManager::FlySound()
{
   
  
}
void GameManager::GeneratePipe() {
    // Tạo ống mới cách các ống cũ một khoảng nhất định
    int gapY = rand() % 300 + 100;
    pipes.push_back(Pipe(1000, gapY));  // Tạo ống ở vị trí xa hơn
}