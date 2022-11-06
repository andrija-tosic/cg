
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

	double GetDistance(POINT a, POINT b);
	void DrawInscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n, COLORREF fillColor, COLORREF borderColor);
	void DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, COLORREF fill = NULL);
	void DrawPolygon(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, COLORREF fill);
	void DrawSquare(CDC* pDC, POINT a, int width, COLORREF color, COLORREF fill, float rotAngle = 0);
	void DrawSquareHatched(CDC* pDC, POINT a, int length, COLORREF color, COLORREF fill, float rotAngle = 0);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC, POINT start, int size, int base, COLORREF color);
	void DrawQuadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d, COLORREF color, COLORREF fill = NULL);
public:
	CIND18015Doc* GetDocument() const;

// Operations
public:

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
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18015View.cpp
inline CIND18015Doc* CIND18015View::GetDocument() const
   { return reinterpret_cast<CIND18015Doc*>(m_pDocument); }
#endif

