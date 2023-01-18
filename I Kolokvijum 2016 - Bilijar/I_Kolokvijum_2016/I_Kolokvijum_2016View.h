
// I_Kolokvijum_2016View.h : interface of the CIKolokvijum2016View class
//

#pragma once
#include "DImage.h"


class CIKolokvijum2016View : public CView
{
protected: // create from serialization only
	CIKolokvijum2016View() noexcept;
	DECLARE_DYNCREATE(CIKolokvijum2016View)

	DImage felt2;
	DImage wood;

	float stapRot = 0;
	float stapTrans = 0;
	bool collided = false;
	float stapRotCollision = 0;
	float kuglaDeltaX = 0;
	float kuglaDeltaY = 0;

// Attributes
public:
	CIKolokvijum2016Doc* GetDocument() const;

// Operations
public:
	void DrawStick(CDC* pDC, int w);
	void DrawBall(CDC* pDC, int w);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawHoles(CDC* pDC, CRect rect, int size);


// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIKolokvijum2016View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in I_Kolokvijum_2016View.cpp
inline CIKolokvijum2016Doc* CIKolokvijum2016View::GetDocument() const
   { return reinterpret_cast<CIKolokvijum2016Doc*>(m_pDocument); }
#endif

