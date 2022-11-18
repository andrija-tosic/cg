
// IND_18015View.h : interface of the CIND18015View class
//

#pragma once
#include "DImage.h"


class CIND18015View : public CView
{
protected: // create from serialization only
	CIND18015View() noexcept;
	DECLARE_DYNCREATE(CIND18015View)

// Attributes
public:
	CIND18015Doc* GetDocument() const;
	CDC memDC;

	DImage piece0, piece1, piece2, piece3, piece4, piece5, piece6, piece7, piece8;

	DImage pieces[9] = { piece0, piece1, piece2, piece3, piece4, piece5, piece6, piece7, piece8 };

// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);
	
	void DrawGrid(CDC* pDC, POINT start, COLORREF borderColor);
	void DrawPiece(CDC* pDC, DImage& piece);
	void DrawPuzzle(CDC* memDC);
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
	virtual ~CIND18015View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
//	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
//	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in IND_18015View.cpp
inline CIND18015Doc* CIND18015View::GetDocument() const
   { return reinterpret_cast<CIND18015Doc*>(m_pDocument); }
#endif

