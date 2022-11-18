
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
END_MESSAGE_MAP()

// CIND18015View construction/destruction

CIND18015View::CIND18015View() noexcept
{
	for (int i = 0; i < 9; i++) {
		CString path = CString(std::string("res/monapuzzle" + std::to_string(i) + ".dib").c_str());
		this->pieces[i].Load(path);
	}
}

CIND18015View::~CIND18015View()
{
}

BOOL CIND18015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
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
		-mx, 0,
		0, -my,
	};

	ModifyWorldTransform(pDC->m_hDC, &matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18015View::DrawPuzzle(CDC* pDC)
{
	int oldGmode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	for (int i = 0; i < 9; i++) {
		this->DrawPiece(pDC, pieces[i]);
	}


	SetGraphicsMode(pDC->m_hDC, oldGmode);
}

void CIND18015View::DrawPiece(CDC* pDC, DImage& piece)
{
	CBitmap bmpImage;
	CBitmap bmpMask;

	CDC srcDC, destDC;
	srcDC.CreateCompatibleDC(pDC);
	destDC.CreateCompatibleDC(pDC);

	unsigned char* bitmapBits = piece.GetDIBBits();
	bmpImage.CreateBitmap(piece.Width(), piece.Height(), 1, piece.BPP(), bitmapBits);

	bmpMask.CreateBitmap(piece.Width(), piece.Height(), 1, 1, NULL);

	COLORREF clrTopLeft = srcDC.GetPixel(0, 0); // test

	CBitmap* oldSrcBmp = srcDC.SelectObject(&bmpImage);
	CBitmap* oldDestBmp = destDC.SelectObject(&bmpMask);

	clrTopLeft = srcDC.GetPixel(0, 0); 
	COLORREF clrSaveBk = srcDC.SetBkColor(clrTopLeft);

	destDC.BitBlt(0, 0, piece.Width(), piece.Height(), &srcDC, 0, 0, SRCCOPY);

	COLORREF clrSaveDstText = srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));

	srcDC.BitBlt(0, 0, piece.Width(), piece.Height(), &destDC, 0, 0, SRCAND);

	// undo
	destDC.SetTextColor(clrSaveDstText);
	srcDC.SetBkColor(clrSaveBk);

	srcDC.SelectObject(oldSrcBmp);
	destDC.SelectObject(oldDestBmp);

	srcDC.DeleteDC();
	destDC.DeleteDC();

	// priprema prikaza
	CDC tempDC;
	tempDC.CreateCompatibleDC(pDC);
	CBitmap* bmpOldT = tempDC.SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, piece.Width(), piece.Height(), &tempDC, 0, 0, SRCAND);

	// prikaz bitmape
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


	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, c.Width(), c.Height());

	memDC.SelectObject(&bm);

	constexpr COLORREF BGCOLOR = RGB(221, 221, 221);
	CRect rect(0, 0, dim, dim);
	CBrush background(BGCOLOR);
	memDC.FillRect(&rect, &background);

	this->DrawPuzzle(&memDC);

	constexpr int GRID_GREY = RGB(238, 238, 238);
	this->DrawGrid(&memDC, { 0, 0 }, GRID_GREY);

	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);
}


// CIND18015View printing

BOOL CIND18015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18015View diagnostics

#ifdef _DEBUG
void CIND18015View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18015Doc* CIND18015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18015Doc)));
	return (CIND18015Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18015View message handlers


void CIND18015View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	this->memDC.CreateCompatibleDC(pDC);

	CView::OnPrepareDC(pDC, pInfo);
}