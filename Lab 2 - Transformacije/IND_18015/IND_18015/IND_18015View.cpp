
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

#define _USE_MATH_DEFINES
#include <math.h>
#define RAD(degrees) ((degrees) * M_PI / 180) 
#define DEG(radians) ((radians) * 180 / M_PI)

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
//	ON_WM_KEYUP()
ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18015View construction/destruction

CIND18015View::CIND18015View() noexcept
{
	this->gridEnabled = true;

	CString mfName("..\\..\\..\\..\\cactus_part.emf");
	CString mfNameLight("..\\..\\..\\..\\cactus_part_light.emf");


	this->cactusPartMF = GetEnhMetaFile(mfName);
	this->cactusPartLightMF = GetEnhMetaFile(mfNameLight);

}

CIND18015View::~CIND18015View()
{
	DeleteEnhMetaFile(this->cactusPartMF);
	DeleteEnhMetaFile(this->cactusPartLightMF);
}

BOOL CIND18015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18015View drawing

void CIND18015View::OnDraw(CDC* pDC)
{
	CIND18015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// pozadina

	constexpr COLORREF BGCOLOR = RGB(135, 206, 235);

	CRect rect(0, 0, dim, dim);
	CBrush background(BGCOLOR);
	pDC->FillRect(&rect, &background);

	this->DrawFigure(pDC);
	
	constexpr int GRID_GREY = RGB(238, 238, 238);

	if (this->gridEnabled) {
		this->DrawGrid(pDC, { 0,0 }, dim, squareLength, GRID_GREY);
	}
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

void CIND18015View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = { 
		1, 0, 
		0, 1, 
		dX, dY 
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18015View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t = {
		sX, 0,
		0, sY
	};

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18015View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t {
		cos(RAD(angle)), -sin(RAD(angle)),
		sin(RAD(angle)), cos(RAD(angle))
	};

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18015View::DrawFigure(CDC* pDC)
{
	int oldGraphicsMode = SetGraphicsMode(pDC->GetSafeHdc(), GM_ADVANCED);

	constexpr COLORREF CIRCLE_GREEN = RGB(0, 204, 0);

	POINT topLeft = { 8.75 * squareLength, squareLength * (squareCount - 6) };
	SIZE cactusSize = { 2.5 * squareLength, 3 * squareLength };
	PlayEnhMetaFile(pDC->GetSafeHdc(), this->cactusPartLightMF, CRect(topLeft, cactusSize));

	topLeft = { (long)(9.5 * squareLength), squareLength * (squareCount - 9) };
	cactusSize = { (long)(squareLength), 3 * squareLength };
	PlayEnhMetaFile(pDC->GetSafeHdc(), this->cactusPartMF, CRect(topLeft, cactusSize));


	XFORM xFormOld;
	GetWorldTransform(pDC->GetSafeHdc(), &xFormOld);
	SetViewportOrgEx(pDC->GetSafeHdc(), squareLength * 10, squareLength * (squareCount - 6), NULL);

	this->Rotate(pDC, 45, true);

	topLeft = { -squareLength/2, 0 };
	cactusSize = { (long)(squareLength), -3 * squareLength };
	PlayEnhMetaFile(pDC->GetSafeHdc(), this->cactusPartMF, CRect(topLeft, cactusSize));
	SetWorldTransform(pDC->GetSafeHdc(), &xFormOld);

	this->Rotate(pDC, -45, true);

	topLeft = { -squareLength / 2, 0 };
	cactusSize = { (long)(squareLength), -3 * squareLength };
	PlayEnhMetaFile(pDC->GetSafeHdc(), this->cactusPartMF, CRect(topLeft, cactusSize));
	SetWorldTransform(pDC->GetSafeHdc(), &xFormOld);


	SetGraphicsMode(pDC->GetSafeHdc(), oldGraphicsMode);
	SetViewportOrgEx(pDC->GetSafeHdc(), 0, 0, NULL);

	this->DrawCircle(pDC, squareLength * 10, squareLength * (squareCount - 3), squareLength, RGB(0, 0, 0), CIRCLE_GREEN);
	this->DrawCircle(pDC, squareLength * 10, squareLength * (squareCount - 6), squareLength, RGB(0, 0, 0), CIRCLE_GREEN);
	

	// vaza

	constexpr COLORREF VASE_GOLD = RGB(222, 148, 0);

	this->DrawVase(pDC, RGB(0, 0, 0), VASE_GOLD);


}

void CIND18015View::DrawGrid(CDC* pDC, POINT start, int size, int base, COLORREF color)
{
	CPen newPen(PS_SOLID, 0, color);
	CPen* prevPen = pDC->SelectObject(&newPen);

	int gridSize = size / base;
	POINT* points = new POINT[gridSize * 4 + 4];
	DWORD* lpPolyPoints = new DWORD[gridSize * 2 + 2];

	for (int i = 0; i <= gridSize; i++) {
		points[i * 4] = { start.x + i * base, start.y };
		points[i * 4 + 1] = { start.x + i * base, start.y + size };
		points[i * 4 + 2] = { start.x, start.y + i * base };
		points[i * 4 + 3] = { start.x + size, start.y + i * base };

		lpPolyPoints[i] = lpPolyPoints[gridSize + i + 1] = 2;
	}

	pDC->PolyPolyline(points, lpPolyPoints, gridSize * 2 + 2);
	pDC->SelectObject(prevPen);

	delete[] points;
	delete[] lpPolyPoints;
}

void CIND18015View::DrawVase(CDC* pDC, COLORREF color, COLORREF fill)
{
	POINT point1 = { squareLength * 8.5, dim };
	POINT point2 = { squareLength * 8, squareLength * (squareCount - 2) };
	POINT point3 = { squareLength * 7.5, squareLength * (squareCount - 2) };
	POINT point4 = { squareLength * 7.5, squareLength * (squareCount - 2.8) };

	POINT point5 = { squareLength * 12.5, squareLength * (squareCount - 2.8) };
	POINT point6 = { squareLength * 12.5, squareLength * (squareCount - 2) };
	POINT point7 = { squareLength * 12, squareLength * (squareCount - 2) };
	POINT point8 = { squareLength * 11.5, dim };

	POINT vertices[] = { point1, point2, point3, point4, point5, point6, point7, point8 };

	CPen newPen(PS_SOLID | PS_GEOMETRIC, 1, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	pDC->Polygon(vertices, 8);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void CIND18015View::DrawCircle(CDC* pDC, int cx, int cy, int r, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 1, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	pDC->Ellipse(cx - r / 2, cy + r / 2, cx + r / 2, cy - r / 2);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}



// CIND18015View message handlers

void CIND18015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G') {
		this->gridEnabled = !this->gridEnabled;
		InvalidateRect(NULL);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
