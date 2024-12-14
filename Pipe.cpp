#include "pch.h"
#include "Pipe.h"
#include <gdiplus.h>
using namespace Gdiplus;
Pipe::Pipe(int x, int gapY) : x(x), gapY(gapY), width(50), gapHeight(150) {}

void Pipe::Draw(CDC* pDC) {
    // Tạo Brush và Pen để tô màu
    CBrush brushTop(RGB(0, 255, 0));  // Màu xanh cho ống trên
    CBrush brushBottom(RGB(0, 255, 0));  // Màu đỏ cho ống dưới
    CBrush brushCap(RGB(0, 100, 0));    // Màu xanh lá đậm cho miệng ống
    // Lưu trạng thái vẽ trước khi thay đổi Brush và Pen
    CBrush* pOldBrushTop = pDC->SelectObject(&brushTop);
    CPen penTop(PS_SOLID, 1, RGB(0, 128, 0));  // Đường viền ống trên màu xanh đậm
    CPen* pOldPenTop = pDC->SelectObject(&penTop);

    // Vẽ ống trên
    pDC->Rectangle(x, 0, x + width, gapY);

    // Khôi phục trạng thái vẽ sau khi vẽ ống trên
    pDC->SelectObject(pOldBrushTop);
    pDC->SelectObject(pOldPenTop);



    // Vẽ ống dưới
    CBrush* pOldBrushBottom = pDC->SelectObject(&brushBottom);
    CPen penBottom(PS_SOLID, 1, RGB(128, 0, 0));  // Đường viền ống dưới màu đỏ đậm
    CPen* pOldPenBottom = pDC->SelectObject(&penBottom);

    pDC->Rectangle(x, gapY + gapHeight, x + width, 600);

    // Khôi phục trạng thái vẽ sau khi vẽ ống dưới
    pDC->SelectObject(pOldBrushBottom);
    pDC->SelectObject(pOldPenBottom);

    pDC->FillSolidRect(x - 5, gapY - 20, width + 10, 20, RGB(0, 100, 0)); // Miệng ống trên
    pDC->FillSolidRect(x - 5, gapY + gapHeight, width + 10, 20, RGB(0, 100, 0)); // Miệng ống dưới
}

void Pipe::Update() {
    x -= 5; // Dịch chuyển sang trái
}

bool Pipe::CheckCollision(CPoint birdPos, int birdRadius) {
    if (birdPos.x + birdRadius > x && birdPos.x - birdRadius < x + width) {
        if (birdPos.y - birdRadius < gapY || birdPos.y + birdRadius > gapY + gapHeight) {
            return true;
        }
    }
    return false;
}