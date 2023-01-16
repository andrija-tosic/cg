
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
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18015View construction/destruction

CIND18015View::CIND18015View() noexcept
{
	this->gridEnabled = true;
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

void CIND18015View::OnDraw(CDC* pDC)
{
	CIND18015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// pozadina

	constexpr COLORREF BGCOLOR = RGB(221, 221, 221);

	CRect rect(0, 0, dim, dim);
	CBrush background(BGCOLOR);
	pDC->FillRect(&rect, &background);

	// narandzasti trougao

	POINT trianglePoints[] = { 
		{squareLength, squareLength}, 
		{ squareLength, 13 * squareLength},
		{13*squareLength, 13*squareLength }
	};

	constexpr COLORREF BLUE = RGB(0, 0, 255);
	constexpr COLORREF ORANGE = RGB(255, 153, 51);

	this->DrawPolygon(pDC, trianglePoints, 3, BLUE, ORANGE);
	this->DrawInscribedPolygon(pDC, trianglePoints[0], trianglePoints[1], trianglePoints[2], 4, ORANGE, BLUE);
	
	// zeleni T

	constexpr COLORREF GREEN = RGB(50, 205, 50);

	POINT greenTPoints[] = {
		{squareLength, squareLength},
		{13 * squareLength, squareLength},
		{13 * squareLength, 13 * squareLength }
	};

	this->DrawPolygon(pDC, greenTPoints, 3, BLUE, GREEN);
	this->DrawInscribedPolygon(pDC, greenTPoints[0], greenTPoints[1], greenTPoints[2], 5, GREEN, BLUE);

	// kvadrat

	constexpr COLORREF PURPLE = RGB(153, 0, 204);

	POINT squarePoints[] = {
		{13 * squareLength, squareLength},
		{squareLength * (squareCount - 1), squareLength },
		{squareLength * (squareCount - 1), 7 * squareLength},
		{13* squareLength, 7*squareLength}
	};

	this->DrawPolygon(pDC, squarePoints, 4, BLUE, PURPLE);

	// hatched trougao

	constexpr COLORREF LIGHT_BLUE = RGB(115, 161, 255);

	POINT hatchedTPoints[] = {
		{squareLength, 13 * squareLength },
		{squareLength, squareLength * (squareCount - 1)},
		{squareLength * 7, squareLength * (squareCount - 7)}
	};

	this->DrawHatchedPolygon(pDC, hatchedTPoints, 3, BLUE, HS_VERTICAL, BLUE);
	this->DrawInscribedPolygon(pDC, hatchedTPoints[0], hatchedTPoints[1], hatchedTPoints[2], 7, RGB(255, 255, 255), HS_VERTICAL, BLUE);

	// roze romb

	constexpr COLORREF PINK = RGB(255, 153, 204);

	POINT rombPoints[] = {
		{ 7 * squareLength, 13 * squareLength },
		{13 * squareLength, 13 * squareLength},
		{squareLength * 7, squareLength * (squareCount - 1)},
		{squareLength, squareLength * (squareCount - 1)}
	};

	this->DrawPolygon(pDC, rombPoints, 4, BLUE, PINK);

	// zuti trougao

	POINT yellowTPoints[] = {
		{13 * squareLength, 13 * squareLength},
		{7 * squareLength, squareLength * (squareCount - 1)},
		{squareLength * (squareCount - 1),squareLength * (squareCount - 1)}
	};

	this->DrawPolygon(pDC, yellowTPoints, 3, BLUE, RGB(255, 255, 0));
	this->DrawInscribedPolygon(pDC, yellowTPoints[0], yellowTPoints[1], yellowTPoints[2], 6, RGB(255, 255, 0), BLUE);

	// crveni trougao

	POINT redTPoints[] = {
		{13 * squareLength, 13 * squareLength},
		{squareLength * (squareCount - 1), 13 * squareLength},
		{squareLength * (squareCount - 1),squareLength * (squareCount - 1)}
	};

	this->DrawPolygon(pDC, redTPoints, 3, BLUE, RGB(255, 0, 0));
	this->DrawInscribedPolygon(pDC, redTPoints[0], redTPoints[1], redTPoints[2], 8, RGB(255, 0, 0), BLUE);

	constexpr int GRID_GREY = RGB(238, 238, 238);

	if (this->gridEnabled) {
		this->DrawGrid(pDC, { 0, 0 }, dim, squareLength, GRID_GREY);
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

double CIND18015View::GetDistance(POINT a, POINT b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void CIND18015View::DrawInscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n, COLORREF fillColor, COLORREF borderColor) {
	double cLen = GetDistance(a, b);
	double aLen = GetDistance(c, b);
	double bLen = GetDistance(c, a);
	double sum = cLen + bLen + aLen;
	double s = sum / 2;
	double r = sqrt(s * (s - aLen) * (s - bLen) * (s - cLen)) / s;

	double cx = (cLen * c.x + bLen * b.x + aLen * a.x) / sum + 0.5;
	double cy = (cLen * c.y + bLen * b.y + aLen * a.y) / sum + 0.5;

	CPen pen(PS_SOLID | PS_GEOMETRIC, 3, borderColor);
	CBrush brush(fillColor);

	CPen* prevPen = pDC->SelectObject(&pen);
	CBrush* prevBrush = pDC->SelectObject(&brush);

	DrawRegularPolygon(pDC, cx, cy, r / 2, n, 0);

	pDC->SelectObject(prevPen);
	pDC->SelectObject(prevBrush);
}

void CIND18015View::DrawInscribedPolygon(CDC* pDC, POINT a, POINT b, POINT c, int n, COLORREF fillColor, int hsStyle, COLORREF borderColor) {
	double cLen = GetDistance(a, b);
	double aLen = GetDistance(c, b);
	double bLen = GetDistance(c, a);
	double sum = cLen + bLen + aLen;
	double s = sum / 2;
	double r = sqrt(s * (s - aLen) * (s - bLen) * (s - cLen)) / s;

	double cx = (cLen * c.x + bLen * b.x + aLen * a.x) / sum + 0.5;
	double cy = (cLen * c.y + bLen * b.y + aLen * a.y) / sum + 0.5;

	CPen pen(PS_SOLID | PS_GEOMETRIC, 3, borderColor);
	CBrush brush(hsStyle, borderColor);

	CPen* prevPen = pDC->SelectObject(&pen);
	CBrush* prevBrush = pDC->SelectObject(&brush);

	DrawRegularPolygon(pDC, cx, cy, r / 2, n, 0);

	pDC->SelectObject(prevPen);
	pDC->SelectObject(prevBrush);
}


void CIND18015View::DrawTriangle(CDC* pDC, POINT a, POINT b, POINT c, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	POINT vertices[] = { a, b, c };
	pDC->Polygon(vertices, 3);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void CIND18015View::DrawPolygon(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	pDC->Polygon(points, numOfPoints);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);

}

void CIND18015View::DrawHatchedPolygon(CDC* pDC, POINT points[], int numOfPoints, COLORREF color, int hatchStyle, COLORREF fill)
{
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CBrush hatchBrush(hatchStyle, fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&hatchBrush);

	pDC->Polygon(points, numOfPoints);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}
void CIND18015View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	double rotAngleRad = RAD(rotAngle);
	double interceptAngleChange = 2 * M_PI / n;
	
	POINT* vertices = new POINT[n];

	for (auto i = 0; i < n; i++) {
		vertices[i].x = cx + r * cos(rotAngleRad);
		vertices[i].y = cy + r * sin(rotAngleRad);
		rotAngleRad += interceptAngleChange;
	}

	pDC->Polygon(vertices, n);
	delete[] vertices;

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

void CIND18015View::DrawQuadrilateral(CDC* pDC, POINT a, POINT b, POINT c, POINT d, COLORREF color, COLORREF fill)
{
	CPen pen(PS_SOLID | PS_GEOMETRIC, 5, color);
	CPen* prevPen = pDC->SelectObject(&pen);

	CBrush brush(fill);
	CBrush* prevBrush = pDC->SelectObject(&brush);

	POINT vertices[] = { a, b, c, d };
	pDC->Polygon(vertices, 4);

	pDC->SelectObject(prevPen);
	pDC->SelectObject(prevBrush);
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
