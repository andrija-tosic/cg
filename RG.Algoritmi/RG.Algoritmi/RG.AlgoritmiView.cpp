
// RG.AlgoritmiView.cpp : implementation of the CRGAlgoritmiView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RG.Algoritmi.h"
#endif

#include "RG.AlgoritmiDoc.h"
#include "RG.AlgoritmiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGAlgoritmiView

IMPLEMENT_DYNCREATE(CRGAlgoritmiView, CView)

BEGIN_MESSAGE_MAP(CRGAlgoritmiView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CRGAlgoritmiView construction/destruction

CRGAlgoritmiView::CRGAlgoritmiView() noexcept
{
	// TODO: add construction code here

}

CRGAlgoritmiView::~CRGAlgoritmiView()
{
}

// CRGAlgoritmiView drawing

void swap(int& a, int& b) {
	int pom;
	pom = a;
	a = b;
	b = pom;
}

void BresenhamLine(CDC* pDC, int x0, int y0, int x1, int y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;

	float m = dx == 0 ? INT_MAX : dy / dx;

	int d = 2 * dy - dx;

	int incr1 = 2 * abs(dy - dx);
	int incr2 = 2 * abs(dy);

	int y = y0;

	for (int x = min(x0, x1); x <= max(x0, x1); x++) {
		if (d > 0) {
			m > 0 ? y++ : y--;
			d += incr1;
		}
		else {
			d += incr2;
		}

		pDC->SetPixel(x, y, RGB(0, 0, 255));
	}
}

void NagibniAlgoritam(CDC* pDC, int x0, int y0, int x1, int y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;
	float m = dy / dx;


	float b = y0 - m * x0;

	for (int x = min(x0, x1); x <= max(x0, x1); x++) {

		float y = m * x + b;

		pDC->SetPixel(x, y, RGB(0, 255, 0));
	}

}

void PolyEllipse(CDC* pDC, int a, int b, COLORREF value)
{
	float x;
	float y = b;
	float a2 = a * a, b2 = b * b;
	x = 0;

	float plotBoundary = a2 / sqrt(a2 + b2);

	for (x = 0; x <= plotBoundary; x += 1) {
		y = (int)(b * sqrt(1 - x * x / a2) + 0.5);
		pDC->SetPixel(x, y, value);
		pDC->SetPixel(x, -y, value);
		pDC->SetPixel(-x, y, value);
		pDC->SetPixel(-x, -y, value);
	}

	for (; y >= 0; y -= 1) {
		x = (int)(a * sqrt(1 - y * y / b2) + 0.5);
		pDC->SetPixel(x, y, value);
		pDC->SetPixel(x, -y, value);
		pDC->SetPixel(-x, y, value);
		pDC->SetPixel(-x, -y, value);
	}
}

void TrigEllipse(CDC* pDC, int x1, int y1, int x2, int y2) {
	int a = (x2 - x1) / 2;
	int b = (y2 - y1) / 2;

	int x_offset = (x2 + x1) / 2;
	int y_offset = (y2 + y1) / 2;

	float step = (2 * 3.14) / (7 * max(a, b));

	for (float alfa = 0; alfa <= 3.14 / 2; alfa += step) {
		int x = a * cos(alfa);
		int y = b * sin(alfa);

		COLORREF col = RGB(255, 0, 0);

		pDC->SetPixel(x + x_offset, y + y_offset, col);
		pDC->SetPixel(-x + x_offset, y + y_offset, col);
		pDC->SetPixel(x + x_offset, -y + y_offset, col);
		pDC->SetPixel(-x + x_offset, -y + y_offset, col);
	}
}

void Translate(CDC* pDC, int dx, int dy) {
	XFORM t = {
		1, 0,
		0, 1,
		dx, dy
	};

	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CRGAlgoritmiView::OnDraw(CDC* pDC)
{
	CRGAlgoritmiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->SetGraphicsMode(GM_ADVANCED);

	CRect c;
	GetClientRect(&c);

	Translate(pDC, c.Width() / 2, c.Height() / 2);

	PolyEllipse(pDC, 400, 200, RGB(128, 0, 0));
	PolyEllipse(pDC, 100, 100, RGB(0, 128, 0));
	PolyEllipse(pDC, 100, 300, RGB(0, 0, 128));

	NagibniAlgoritam(pDC, 20, 20, 300, 300);
	NagibniAlgoritam(pDC, 20, 240, 300, 20);

	BresenhamLine(pDC, 50, 30, 230, 100);
	BresenhamLine(pDC, 0, 250, 400, 20);
	BresenhamLine(pDC, 20, 20, 300, 20);
	BresenhamLine(pDC, 30, 200, 30, 20);
	
	TrigEllipse(pDC, 50, 50, 500, 300);
}



#ifdef _DEBUG

CRGAlgoritmiDoc* CRGAlgoritmiView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGAlgoritmiDoc)));
	return (CRGAlgoritmiDoc*)m_pDocument;
}
#endif //_DEBUG


// CRGAlgoritmiView message handlers
