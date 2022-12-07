
// I_Kolokvijum_2019View.h : interface of the CIKolokvijum2019View class
//

#pragma once
#include "DImage.h"


class CIKolokvijum2019View : public CView
{
protected: // create from serialization only
	CIKolokvijum2019View() noexcept;
	DECLARE_DYNCREATE(CIKolokvijum2019View);

	DImage body1, arm1, arm2, leg1, leg2, background;

	int arm1Angle = 0;
	int arm2Angle = 0;
	int leg1Angle = 0;
	int leg2Angle = 0;

	// Attributes
public:
	CIKolokvijum2019Doc* GetDocument() const;

	// Operations
public:
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);

	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);

	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);

	void DrawBody1(CDC* pDC);
	void DrawTransformer(CDC* pDC);



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
	virtual ~CIKolokvijum2019View();
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
};

#ifndef _DEBUG  // debug version in I_Kolokvijum_2019View.cpp
inline CIKolokvijum2019Doc* CIKolokvijum2019View::GetDocument() const
{
	return reinterpret_cast<CIKolokvijum2019Doc*>(m_pDocument);
}
#endif

