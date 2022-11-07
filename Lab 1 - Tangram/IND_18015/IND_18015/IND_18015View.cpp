
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
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIND18015View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18015View construction/destruction

CIND18015View::CIND18015View() noexcept
{
	this->gridEnabled = false;
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

	// roze trougao

	constexpr COLORREF PINK = RGB(255, 153, 204);
	constexpr COLORREF BORDER_GREEN = RGB(13, 255, 13);

	POINT rozeTrougaoA = { squareLength, squareLength };
	POINT rozeTrougaoB = { 13 * squareLength, squareLength };
	POINT rozeTrougaoC = { squareLength, 13 * squareLength };

	this->DrawTriangle(pDC, rozeTrougaoA, rozeTrougaoB, rozeTrougaoC, BORDER_GREEN, PINK);

	// plave linije u belom kvadratu

	constexpr COLORREF BLUE = RGB(102, 153, 255);
	constexpr POINT hatchedSquareA = { squareLength * 13, squareLength }; 
	constexpr POINT hatchedSquareB = { squareLength * (squareCount - 1), squareLength }; 
	constexpr POINT hatchedSquareC = { squareLength * (squareCount - 1), 7 * squareLength };
	constexpr POINT hatchedSquareD = { squareLength * 13, 7 * squareLength }; 

	POINT hatchedSquareVertices[] = {hatchedSquareA , hatchedSquareB, hatchedSquareC, hatchedSquareD};

	constexpr int hatchedSquareLength = squareLength * 6;

	this->DrawHatchedPolygon(pDC, hatchedSquareVertices, 4, BORDER_GREEN, HS_HORIZONTAL, BLUE);

	// mnogougao u roze trouglu

	this->DrawInscribedPolygon(pDC, rozeTrougaoA, rozeTrougaoB, rozeTrougaoC, 8, PINK, BORDER_GREEN);

	// crveni trougao

	POINT crveniTrougaoA = { 7 * squareLength, 7 * squareLength };
	POINT crveniTrougaoB = { (squareCount - 1) * squareLength, 7 * squareLength };
	POINT crveniTrougaoC = { (squareCount - 1) * squareLength, (squareCount - 1) * squareLength };

	this->DrawTriangle(pDC, crveniTrougaoA, crveniTrougaoB, crveniTrougaoC, BORDER_GREEN, RGB(255, 0, 0));
	this->DrawInscribedPolygon(pDC, crveniTrougaoA, crveniTrougaoB, crveniTrougaoC, 5, RGB(255, 0, 0), BORDER_GREEN);

	// zuti trougao

	POINT zutiTrougaoA = { 13 * squareLength, 13 * squareLength };
	POINT zutiTrougaoB = { (squareCount - 1) * squareLength, (squareCount - 1) * squareLength };
	POINT zutiTrougaoC = { 13 * squareLength, (squareCount - 1) * squareLength };

	this->DrawTriangle(pDC, zutiTrougaoA, zutiTrougaoB, zutiTrougaoC, BORDER_GREEN, RGB(255, 255, 0));
	this->DrawInscribedPolygon(pDC, zutiTrougaoA, zutiTrougaoB, zutiTrougaoC, 6, RGB(255, 255, 0), BORDER_GREEN);

	// zeleni trougao

	constexpr COLORREF TRIANGLE_GREEN = RGB(50, 205, 50);

	POINT zeleniTrougaoA = { squareLength, 13 * squareLength };
	POINT zeleniTrougaoB = { 7 * squareLength, 13 * squareLength };
	POINT zeleniTrougaoC = { squareLength, (squareCount - 1) * squareLength };

	this->DrawTriangle(pDC, zeleniTrougaoA, zeleniTrougaoB, zeleniTrougaoC, BORDER_GREEN, TRIANGLE_GREEN);
	this->DrawInscribedPolygon(pDC, zeleniTrougaoA, zeleniTrougaoB, zeleniTrougaoC, 4, TRIANGLE_GREEN, BORDER_GREEN);

	// narandzasti trougao

	constexpr COLORREF TRIANGLE_ORANGE = RGB(255, 153, 51);

	POINT narandzastiTrougaoA = { squareLength, (squareCount - 1) * squareLength };
	POINT narandzastiTrougaoB = { 7 * squareLength, 13 * squareLength };
	POINT narandzastiTrougaoC = { 13 * squareLength , (squareCount - 1) * squareLength };

	this->DrawTriangle(pDC, narandzastiTrougaoA, narandzastiTrougaoB, narandzastiTrougaoC, BORDER_GREEN, TRIANGLE_ORANGE);
	this->DrawInscribedPolygon(pDC, narandzastiTrougaoA, narandzastiTrougaoB, narandzastiTrougaoC, 7, TRIANGLE_ORANGE, BORDER_GREEN);

	// ljubicasti romb

	constexpr COLORREF PURPLE = RGB(153, 0, 204);

	POINT qA = { squareLength * 7, squareLength * 7 };
	POINT qB = { squareLength * 7, squareLength * 13 };
	POINT qC = { squareLength * 13, squareLength * (squareCount - 1) };
	POINT qD = { squareLength * 13, squareLength * 13 };

	this->DrawQuadrilateral(pDC, qA, qB, qC, qD, BORDER_GREEN, PURPLE);

	constexpr int GRID_GREY = RGB(238, 238, 238);

	if (this->gridEnabled) {
		this->DrawGrid(pDC, { 0, 0 }, dim, squareLength, GRID_GREY);
	}
}


// CIND18015View printing


void CIND18015View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

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

void CIND18015View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIND18015View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
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


void CIND18015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G') {
		this->gridEnabled = !this->gridEnabled;
		InvalidateRect(NULL);
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
