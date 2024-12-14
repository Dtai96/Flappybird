
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "GameManager.h"


// CChildView
GameManager gameManager;
CChildView::CChildView() : isGameOver(false)  // Trò chơi chưa kết thúc khi khởi tạo
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	 CPaintDC dc(this); // device context for painting
    // Vẽ các đối tượng trong trò chơi
    gameManager.Draw(&dc);  // Gọi phương thức Draw với đối số là CDC*
}

void CChildView::OnTimer(UINT_PTR nIDEvent) {
	 if (gameManager.isGameOver)  // Kiểm tra trạng thái trò chơi
	{
		KillTimer(1);  // Dừng Timer
		return;
	}

	// Cập nhật trò chơi nếu chưa kết thúc
	gameManager.Update();
	Invalidate(FALSE);
}
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (gameManager.isGameOver)  // Kiểm tra nếu trò chơi đã kết thúc
	{
		if (nChar == VK_RETURN)  // Kiểm tra phím Enter
		{
			gameManager.StartGame(); // Bắt đầu lại trò chơi
			SetTimer(1, 30, NULL);   // Khởi động lại Timer
			Invalidate(FALSE);       // Yêu cầu vẽ lại giao diện
		}
	}

	gameManager.OnKeyDown(nChar);
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Khởi tạo Timer với ID là 1, chu kỳ 30ms
	SetTimer(1, 30, NULL);  // 60 FPS

	return 0;
}
void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// Hủy Timer khi cửa sổ bị phá hủy
	KillTimer(1);
}