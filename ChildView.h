
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window
#include "GameManager.h"
class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:
private:
	bool isGameOver;  // Trạng thái của trò chơi
	GameManager gameManager;  // Đối tượng quản lý trò chơi

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	void OnTimer(UINT_PTR nIDEvent);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

