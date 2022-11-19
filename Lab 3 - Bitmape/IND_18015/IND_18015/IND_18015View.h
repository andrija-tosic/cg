
// IND_18015View.h : interface of the CIND18015View class
//

#pragma once
#include "DImage.h"

struct PUZZLE_PIECE {
	DImage image;
	int cxOffset;
	int cyOffset;
	int rotAngle;
	bool flipped;
	int index;
};


class CIND18015View : public CView
{
protected: // create from serialization only
	CIND18015View() noexcept;
	DECLARE_DYNCREATE(CIND18015View)

		// Attributes
public:
	CIND18015Doc* GetDocument() const;

	PUZZLE_PIECE piece0 = {
		DImage(), -6, 3, -71, false, 3
	}, piece1 = {
		DImage(), -5, -10, 23, false, 5
	}, piece2 = {
		DImage(), 2, -12, 71, false, 7
	}, piece3 = {
		DImage(), -8, -8, 17, false, 8
	}, piece4 = {
		DImage(), -8, -2, -32, false, 1
	}, piece5 = {
		DImage(), -8, -4, -18, false, 2
	}, piece6 = {
		DImage(), 5, -10, 115, false, 0
	}, piece7 = {
		DImage(), 6, -5, 145, false, 6
	}, piece8 = {
		DImage(), 5,0 , -166, false, 4
	};


	PUZZLE_PIECE pieces[3][3] = {
		piece0, piece1, piece2, piece3, piece4, piece5, piece6, piece7, piece8
	};

	// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = false);

	void DrawAxes(CDC* pDC);

	void DrawGrid(CDC* pDC, POINT start, COLORREF borderColor);
	void DrawPiece(CDC* pDC, DImage& piece, bool colorFilter);
	void DrawPuzzle(CDC* memDC);
	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
protected:

	// Implementation
public:
	virtual ~CIND18015View();
#ifdef _DEBUG
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in IND_18015View.cpp
inline CIND18015Doc* CIND18015View::GetDocument() const
{
	return reinterpret_cast<CIND18015Doc*>(m_pDocument);
}
#endif

