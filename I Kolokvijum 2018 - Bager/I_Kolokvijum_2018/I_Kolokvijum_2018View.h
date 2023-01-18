
// I_Kolokvijum_2018View.h : interface of the CIKolokvijum2018View class
//

#pragma once
#include "DImage.h"


class CIKolokvijum2018View : public CView
{
protected: // create from serialization only
	CIKolokvijum2018View() noexcept;
	DECLARE_DYNCREATE(CIKolokvijum2018View)

		DImage arm1, arm2, bager, pozadina;
	HENHMETAFILE viljuska;

	float arm1Rot = 0;
	float arm2Rot = 0;
	float forkRot = 0;
	float bagerTrans = 0;

	// Attributes
public:
	CIKolokvijum2018Doc* GetDocument() const;

	// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);

	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBody(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawExcavator(CDC* pDC);

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
	virtual ~CIKolokvijum2018View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in I_Kolokvijum_2018View.cpp
inline CIKolokvijum2018Doc* CIKolokvijum2018View::GetDocument() const
{
	return reinterpret_cast<CIKolokvijum2018Doc*>(m_pDocument);
}
#endif

