
// IND_18015View.cpp : implementation of the CIND18015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18015.h"
#endif

#include "IND_18015Doc.h"
#include "IND_18015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD(x) (x*M_PI)/180
#define DEG(x) (x*180)/M_PI

constexpr int dim = 500;
constexpr int squareCount = 20;
constexpr int squareLength = dim / squareCount;


// CIND18015View

IMPLEMENT_DYNCREATE(CIND18015View, CView)

BEGIN_MESSAGE_MAP(CIND18015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIND18015View construction/destruction

CIND18015View::CIND18015View() noexcept
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			CString path = CString(std::string("res/monapuzzle" + std::to_string(this->pieces[i][j].index) + ".dib").c_str());
			this->pieces[i][j].image.Load(path);
		}
	}
}

CIND18015View::~CIND18015View()
{
}

// CIND18015View drawing

void CIND18015View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM matrix =
	{
		1, 0,
		0, 1,
		dX, dY
	};

	ModifyWorldTransform(pDC->m_hDC, &matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18015View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM matrix =
	{
		sX, 0,
		0, sY
	};

	ModifyWorldTransform(pDC->m_hDC, &matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18015View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM matrix =
	{
		cos(RAD(angle)), sin((RAD(angle))),
		-sin(RAD(angle)), cos(RAD(angle))
	};

	ModifyWorldTransform(pDC->m_hDC, &matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18015View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM matrix =
	{
		mx ? -1 : 1, 0,
		0, my ? -1 : 1,
	};

	ModifyWorldTransform(pDC->m_hDC, &matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18015View::DrawAxes(CDC* pDC)
{
	POINT curPos = pDC->GetCurrentPosition();
	CPen* prevPen = pDC->GetCurrentPen();

	CPen xAxisPen(PS_SOLID, 5, RGB(255, 0, 0));
	pDC->SelectObject(&xAxisPen);
	pDC->LineTo(curPos.x, curPos.y + 100);
	pDC->MoveTo(curPos.x, curPos.y);
	CPen yAxisPen(PS_SOLID, 5, RGB(0, 0, 255));
	pDC->SelectObject(&yAxisPen);
	pDC->LineTo(curPos.x + 100, curPos.y);
	pDC->MoveTo(curPos.x, curPos.y);

	pDC->SelectObject(prevPen);
}


void CIND18015View::DrawPuzzle(CDC* pDC)
{
	int oldGmode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	XFORM tOld;

	int imageWidth = pieces[0][0].image.Width();

	double factor = imageWidth / squareLength;
	int size = factor * squareLength;

	int i = 0, j = 0;

	int globalRot = -90;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			pDC->GetWorldTransform(&tOld);

			this->Translate(pDC, dim / 2, dim / 2);
			this->Mirror(pDC, false, true);
			this->Translate(pDC, -dim / 2, -dim / 2);

			this->Translate(pDC, pieces[i][j].cxOffset, pieces[i][j].cyOffset);
			this->Translate(pDC, - squareLength + j * 150, -squareLength + i * 150);
			
			//TRT
			this->Translate(pDC, imageWidth / 2.0, imageWidth / 2.0);
			this->Rotate(pDC, (globalRot + pieces[i][j].rotAngle));
			this->Translate(pDC, -imageWidth / 2.0, -imageWidth / 2.0);


			if (i * 3 + j == 1) {
				this->DrawPiece(pDC, this->pieces[i][j].image, { i * size, j * size }, 25, true);
			}
			else {
				this->DrawPiece(pDC, this->pieces[i][j].image, { i * size, j * size }, 25, false);
			}
			pDC->SetWorldTransform(&tOld);

		}
	}


	SetGraphicsMode(pDC->m_hDC, oldGmode);
}

void CIND18015View::DrawPiece(CDC* pDC, DImage& piece, POINT topLeft, int rotAngle, bool colorFilter)
{
	CBitmap bmpImage;
	CBitmap bmpMask;

	CDC srcDC, destDC;
	srcDC.CreateCompatibleDC(pDC);
	destDC.CreateCompatibleDC(pDC);

	bmpImage.CreateCompatibleBitmap(pDC, piece.Width(), piece.Height());

	bmpImage.SetBitmapBits(piece.Width() * piece.Height() * piece.BPP(), piece.GetDIBBits());

	bmpMask.CreateBitmap(piece.Width(), piece.Height(), 1, 1, NULL);

	// Ucitavanje bitmapa u DC
	CBitmap* oldSrcBmp = srcDC.SelectObject(&bmpImage);
	CBitmap* oldDestBmp = destDC.SelectObject(&bmpMask);

	// Popunjavanje monohromatske bitmape
	COLORREF clrTopLeft = srcDC.GetPixel(0, 0);
	COLORREF clrSaveBk = srcDC.SetBkColor(clrTopLeft);

	destDC.BitBlt(0, 0, piece.Width(), piece.Height(), &srcDC, 0, 0, SRCCOPY);

	unsigned char* bitmapBits = piece.GetDIBBits();

	for (int i = 0; i < piece.Width() * piece.Height() * piece.BPP(); i += 4) {
		byte B = bitmapBits[i];
		byte G = bitmapBits[i + 1];
		byte R = bitmapBits[i + 2];
		byte A = bitmapBits[i + 3];

		if (!colorFilter) {
			byte gr = 64 + (R + G + B) / 3;
			if (gr > 255) {
				gr = 255;
			}

			bitmapBits[i] = bitmapBits[i + 1] = bitmapBits[i + 2] = bitmapBits[i + 3] = gr;
		}
		else {
			bitmapBits[i] = min(bitmapBits[i] + 64, 255);
			bitmapBits[i + 1] = bitmapBits[i + 2] = bitmapBits[i + 3] = 0;
		}
	}

	bmpImage.SetBitmapBits(piece.Width() * piece.Height() * piece.BPP(), bitmapBits);


	// Markiranje pozadine u srcDC (u crno)
	COLORREF clrSaveDstText = srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));

	srcDC.BitBlt(0, 0, piece.Width(), piece.Height(), &destDC, 0, 0, SRCAND);


	// Undo
	destDC.SetTextColor(clrSaveDstText);
	srcDC.SetBkColor(clrSaveBk);

	srcDC.SelectObject(oldSrcBmp);
	destDC.SelectObject(oldDestBmp);

	srcDC.DeleteDC();
	destDC.DeleteDC();

	// Priprema prikaza
	CDC tempDC;
	tempDC.CreateCompatibleDC(pDC);

	CBitmap* bmpOldT = tempDC.SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, piece.Width(), piece.Height(), &tempDC, 0, 0, SRCAND);

	// Prikaz bitmape
	tempDC.SelectObject(&bmpImage);

	pDC->BitBlt(0, 0, piece.Width(), piece.Height(), &tempDC, 0, 0, SRCPAINT);
	tempDC.SelectObject(bmpOldT);

}

void CIND18015View::DrawGrid(CDC* pDC, POINT start, COLORREF color)
{
	CPen newPen(PS_SOLID, 0, color);
	CPen* prevPen = pDC->SelectObject(&newPen);

	POINT points[(squareCount + 1) * 2 * 2]{};
	DWORD lpPolyPoints[(squareCount + 1) * 2]{};

	for (int i = 0; i <= squareCount; i++) {
		points[i * 4] = { start.x + i * squareLength, start.y };
		points[i * 4 + 1] = { start.x + i * squareLength, start.y + dim };
		points[i * 4 + 2] = { start.x, start.y + i * squareLength };
		points[i * 4 + 3] = { start.x + dim, start.y + i * squareLength };

		lpPolyPoints[i] = lpPolyPoints[i + squareCount + 1] = 2;
	}

	pDC->PolyPolyline(points, lpPolyPoints, squareCount * 2 + 2);
	pDC->SelectObject(prevPen);
}


void CIND18015View::OnDraw(CDC* pDC)
{
	CIND18015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// pozadina

	CRect c;
	GetClientRect(&c);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, c.Width(), c.Height());

	memDC.SelectObject(&bm);

	constexpr COLORREF BGCOLOR = RGB(255, 255, 255);
	CRect rect(0, 0, dim, dim);
	CBrush background(BGCOLOR);
	memDC.FillRect(&rect, &background);

	constexpr int GRID_GREY = RGB(238, 238, 238);
	this->DrawGrid(&memDC, { 0, 0 }, GRID_GREY);

	this->DrawPuzzle(&memDC);

	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);
}


// CIND18015View diagnostics

#ifdef _DEBUG

CIND18015Doc* CIND18015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18015Doc)));
	return (CIND18015Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18015View message handlers



BOOL CIND18015View::OnEraseBkgnd(CDC* pDC)
{
	return true;

	return CView::OnEraseBkgnd(pDC);
}
