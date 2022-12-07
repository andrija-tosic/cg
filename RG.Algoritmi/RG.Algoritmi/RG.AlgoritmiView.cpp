
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

// Pokriva sve slucajeve.
void BresenhamLine(CDC* pDC, int x0, int y0, int x1, int y1) {
	int dx = x1 - x0;
	int dy = y1 - y0;

	float m = 1.0 * dy / dx;

	int d;

	int incr1;

	// Inkrement kad je di < 0 je isti bez obzira da li je plot po x ili y.
	int incr2 = 2 * abs(dy);

	int start, end, j;

	if (m >= 1 || m <= -1) {
		// Plot po y.
		start = min(y0, y1);
		end = max(y0, y1);
		j = y0 < y1 ? x0 : x1;

		// Iz izvodjenja.
		d = 2 * abs(dy);
		incr1 = abs(dy);
	}
	else {
		// Plot po x.
		start = min(x0, x1);
		end = max(x0, x1);
		j = x0 < x1 ? y0 : y1;

		// Iz izvodjenja.
		d = 2 * abs(dy) - abs(dx);
		incr1 = 2 * abs(dy - dx);
	}

	for (int i = start; i <= end; i++) {
		if (d >= 0) {
			if (m > 0 && !isinf(m)) {
				j++;
			}
			else if (m < 0 && !isinf(m)) {
				j--;
			}

			d += incr1;
		}
		else {
			d += incr2;
		}

		if (m >= 1 || m <= -1) {
			pDC->SetPixel(j, i, RGB(0, 0, 255));
		}
		else {
			pDC->SetPixel(i, j, RGB(0, 0, 255));
		}
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

void swap(int& a, int& b) {
	int pom = a;
	a = b;
	b = pom;
}

void DrawPixel4(CDC* pDC, int x, int y, int x_off, int y_off, COLORREF col) {
	pDC->SetPixel(x + x_off, y + y_off, col);
	pDC->SetPixel(-x + x_off, y + y_off, col);
	pDC->SetPixel(x + x_off, -y + y_off, col);
	pDC->SetPixel(-x + x_off, -y + y_off, col);
}

void Diff2Ellipse(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF col) {
	int a = abs(x1 + x2) / 2;
	int b = abs(y1 + y2) / 2;

	int x_offset = x1 + abs(x2 - x1) / 2;
	int y_offset = y1 + abs(y2 - y1) / 2;

	float h = 7 * max(a, b);

	float step = (2 * 3.14) / h;

	float _x0 = a;
	float _x1 = a * cos(step);

	float _y0 = 0;
	float _y1 = b * sin(step);

	DrawPixel4(pDC, _x0, _y0, x_offset, y_offset, col);
	DrawPixel4(pDC, _x1, _y1, x_offset, y_offset, col);

	for (int i = 0; i <= h / 2; i++) {
		float _x2 = (2 - step * step) * _x1 - _x0;
		float _y2 = (2 - step * step) * _y1 - _y0;

		DrawPixel4(pDC, _x2, _y2, x_offset, y_offset, col);

		_x0 = _x1;
		_y0 = _y1;
		_x1 = _x2;
		_y1 = _y2;
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

	//PolyEllipse(pDC, 400, 200, RGB(128, 0, 0));
	//PolyEllipse(pDC, 100, 100, RGB(0, 128, 0));
	//PolyEllipse(pDC, 100, 300, RGB(0, 0, 128));

	//// m > 0
	//BresenhamLine(pDC, 0, 0, 200, 200);
	//
	//// m < 0
	//BresenhamLine(pDC, 0, 200, 200, 0);
	//
	//// Horizontalna duz
	//BresenhamLine(pDC, 0, 0, 200, 0);

	//// Vertikalna duz
	//BresenhamLine(pDC, 0, 200, 0, 0);

	///*********** Negativne vrednosti ************/

	//// m > 0
	//BresenhamLine(pDC, 0, 0, -200, -200);

	//// m < 0
	//BresenhamLine(pDC, 0, -200, -200, 0);

	//// Horizontalna duz
	//BresenhamLine(pDC, 0, 0, -200, 0);

	//// Vertikalna duz
	//BresenhamLine(pDC, 0, -200, 0, 0);

	//
	//TrigEllipse(pDC, 300, 0, 500, 300);

	Diff2Ellipse(pDC, 50, 50, 300, 200, RGB(0, 0, 0));
}



#ifdef _DEBUG

CRGAlgoritmiDoc* CRGAlgoritmiView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGAlgoritmiDoc)));
	return (CRGAlgoritmiDoc*)m_pDocument;
}
#endif //_DEBUG


// CRGAlgoritmiView message handlers
