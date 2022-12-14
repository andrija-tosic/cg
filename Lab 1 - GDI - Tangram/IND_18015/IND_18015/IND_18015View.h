
// IND_18015View.h : interface of the CIND18015View class
//

#pragma once


class CIND18015View : public CView
{
protected: // create from serialization only
	CIND18015View() noexcept;
	DECLARE_DYNCREATE(CIND18015View)

// Attributes
protected:
	bool gridEnabled;

public:
	CIND18015Doc* GetDocument() const;

// Operations
public:
	double GetDistance(POINT a, POINT b);
	void DrawInscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n, COLORREF fillColor, COLORREF borderColor);
	void DrawInscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n, COLORREF fillColor, int hsStyle, COLORREF borderColor);
	void DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, COLORREF fill = NULL);
	void DrawPolygon(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, COLORREF fill);
	void DrawHatchedPolygon(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, int hatchStyle, COLORREF fill);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC, POINT start, int size, int base, COLORREF color);
	void DrawQuadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d, COLORREF color, COLORREF fill = NULL);

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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18015View.cpp
inline CIND18015Doc* CIND18015View::GetDocument() const
   { return reinterpret_cast<CIND18015Doc*>(m_pDocument); }
#endif

