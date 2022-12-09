#include "DImage.h"
// I_Kolokvijum_2015View.h : interface of the CIKolokvijum2015View class
//

#pragma once


class CIKolokvijum2015View : public CView
{
protected: // create from serialization only
	CIKolokvijum2015View() noexcept;
	DECLARE_DYNCREATE(CIKolokvijum2015View);

	DImage pozadina;
	bool collided = false;
	int rot = 0;
	int dx = 0;
	int dy = 0;
	bool mirrored = false;
	bool mouthOpen = true;
	bool previousLeft = false;


	// Attributes
public:
	CIKolokvijum2015Doc* GetDocument() const;

	// Operations
public:
	void DrawPacman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);


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
	virtual ~CIKolokvijum2015View();
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

#ifndef _DEBUG  // debug version in I_Kolokvijum_2015View.cpp
inline CIKolokvijum2015Doc* CIKolokvijum2015View::GetDocument() const
{
	return reinterpret_cast<CIKolokvijum2015Doc*>(m_pDocument);
}
#endif

