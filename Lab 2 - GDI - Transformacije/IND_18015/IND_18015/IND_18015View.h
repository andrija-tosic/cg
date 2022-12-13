
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
	int smallCactusRotAngle = 0;
	int wholeCactusRotAngle = 0;
	HENHMETAFILE cactusPartMF;
	HENHMETAFILE cactusPartLightMF;
public:
	CIND18015Doc* GetDocument() const;

	// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);

	void RotateAroundPoint(CDC* pDC, POINT point, float angle);

	void DrawFigure(CDC* pDC);
	void DrawGrid(CDC* pDC, POINT start, COLORREF borderColor);

	void DrawCactus(CDC* pDC, SIZE scale, POINT translate, int selfRotationAngle, HENHMETAFILE mf);
	void DrawVase(CDC* pDC, COLORREF color, COLORREF fill);
	void DrawCircle(CDC* pDC, POINT point, int r, COLORREF borderColor, COLORREF fill);
	void DrawTextString(CDC* pDC, int x, int y, const CString& text, LOGFONTW logFont, COLORREF color, COLORREF strokeColor);

	void UndoWorldTransform(CDC* pDC, XFORM& xForm);


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
{
	return reinterpret_cast<CIND18015Doc*>(m_pDocument);
}
#endif

