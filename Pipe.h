#pragma once
class Pipe
{
public:
    Pipe(int x, int gapY);  // Constructor để khởi tạo các giá trị

    void Draw(CDC* pDC);  // Phương thức vẽ
    void Update();  // Phương thức cập nhật vị trí
    bool CheckCollision(CPoint birdPos, int birdRadius);  // Kiểm tra va chạm

    // Các biến của Pipe
    int x;  // Vị trí x của ống
    int gapY;  // Vị trí của khoảng cách giữa ống trên và dưới
    int width;  // Chiều rộng của ống
    int gapHeight;  // Chiều cao của khoảng trống giữa ống trên và dưới
};

