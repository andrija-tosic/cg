
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
#include <string>
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

	int oldGraphicsMode = SetGraphicsMode(pDC->GetSafeHdc(), GM_ADVANCED);

	this->RotateAroundPoint(pDC, { dim / 2, dim / 2 }, 90);

	// pozadina

	constexpr COLORREF BGCOLOR = RGB(135, 206, 235);

	CRect rect(0, 0, dim, dim);
	CBrush background(BGCOLOR);
	pDC->FillRect(&rect, &background);

	this->DrawFigure(pDC);

	// vaza

	constexpr COLORREF VASE_GOLD = RGB(222, 148, 0);

	this->DrawVase(pDC, RGB(0, 0, 0), VASE_GOLD);

	POINT textLocation = { squareLength * (squareCount - 1), squareLength };

	LOGFONTW logFont{};
	logFont.lfEscapement = -900;
	logFont.lfHeight = 40;
	logFont.lfOrientation = -900;

	this->DrawTextString(pDC, textLocation.x, textLocation.y, CString("18015 Andrija Tosic"), logFont, RGB(0, 0, 0), RGB(255, 255, 0));

	constexpr int GRID_GREY = RGB(238, 238, 238);

	if (this->gridEnabled) {
		this->DrawGrid(pDC, { 0,0 }, GRID_GREY);
	}
	SetGraphicsMode(pDC->GetSafeHdc(), oldGraphicsMode);
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

void CIND18015View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM t = {
		1, 0,
		0, 1,
		dX, dY
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIND18015View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false)
{
	XFORM t = {
		sX, 0,
		0, sY
	};

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18015View::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	XFORM t = {
		cos(RAD(angle)), -sin(RAD(angle)),
		sin(RAD(angle)), cos(RAD(angle))
	};

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18015View::RotateAroundPoint(CDC* pDC, POINT point, float angle)
{
	this->Translate(pDC, point.x, point.y);
	this->Rotate(pDC, angle);
	this->Translate(pDC, -point.x, -point.y);
}

void CIND18015View::DrawCactus(CDC* pDC, SIZE scale, POINT translate, int selfRotationAngle, HENHMETAFILE mf) {
	XFORM prevTransform;
	GetWorldTransform(pDC->GetSafeHdc(), &prevTransform);
	
	this->Translate(pDC, translate.x, translate.y);
	this->Rotate(pDC, selfRotationAngle);
	this->Scale(pDC, scale.cx, scale.cy);

	PlayEnhMetaFile(pDC->GetSafeHdc(), mf, CRect(POINT{ -squareLength / 2, -squareLength }, SIZE{ squareLength, squareLength }));

	this->UndoWorldTransform(pDC, prevTransform);
}

void CIND18015View::UndoWorldTransform(CDC* pDC, XFORM& xForm) {
	SetWorldTransform(pDC->GetSafeHdc(), &xForm);
}

void CIND18015View::DrawFigure(CDC* pDC)
{
	constexpr POINT bottomCircleInVase = { squareLength * 10, squareLength * (squareCount - 3) };
	constexpr POINT circleAbove = { bottomCircleInVase.x + 0, bottomCircleInVase.y - squareLength * 3 };
	constexpr POINT firstCircleOnRight = { bottomCircleInVase.x + squareLength * 2.125, bottomCircleInVase.y - squareLength * 5 };
	constexpr POINT secondCircleOnRight = { bottomCircleInVase.x + squareLength * 5, bottomCircleInVase.y - squareLength * 5 };
	constexpr POINT bottomCircleOnLeft = { bottomCircleInVase.x + -2 * squareLength, bottomCircleInVase.y - 5 * squareLength };
	constexpr POINT topCircleOnLeft = { bottomCircleInVase.x + -2 * squareLength, bottomCircleInVase.y - 8 * squareLength };

	constexpr COLORREF CIRCLE_GREEN = RGB(0, 204, 0);

	constexpr long thinCactusWidthFactor = 1;
	constexpr long thinCactusHeightFactor = 4;

	constexpr SIZE thinCactusScale = { thinCactusWidthFactor, thinCactusHeightFactor };

	constexpr long mediumCactusWidthFactor = 2;
	constexpr long mediumCactusHeightFactor = 3;

	constexpr SIZE mediumCactusScale = { mediumCactusWidthFactor, mediumCactusHeightFactor };

	constexpr double fatCactusWidthFactor = 2.5;
	constexpr long fatCactusHeightFactor = 3;

	constexpr SIZE fatCactusScale = { fatCactusWidthFactor, fatCactusHeightFactor };

	XFORM xFormOld;
	GetWorldTransform(pDC->GetSafeHdc(), &xFormOld);

	this->RotateAroundPoint(pDC, bottomCircleInVase, wholeCactusRotAngle);

	// bottom svetli kaktus
	this->DrawCactus(pDC, fatCactusScale, bottomCircleInVase, 0, this->cactusPartLightMF);

	// kaktus iznad bottom svetlog
	this->DrawCactus(pDC, thinCactusScale, circleAbove, 0, this->cactusPartMF);

	// oba kaktusa rotirana za 45
	this->DrawCactus(pDC, thinCactusScale, { circleAbove.x, circleAbove.y + squareLength / 4 }, 45, this->cactusPartMF);
	this->DrawCactus(pDC, thinCactusScale, { circleAbove.x, circleAbove.y + squareLength / 4 }, -45, this->cactusPartMF);

	// prvi kaktus desno
	this->DrawCactus(pDC, mediumCactusScale, firstCircleOnRight, -90, this->cactusPartMF);

	// rotirajuci kaktus
	this->DrawCactus(pDC, mediumCactusScale, firstCircleOnRight, smallCactusRotAngle, this->cactusPartLightMF);

	// krajnja desna 2 kaktusa

	// 1.
	this->DrawCactus(pDC, mediumCactusScale, secondCircleOnRight, -45, this->cactusPartMF);

	// 2.
	this->DrawCactus(pDC, mediumCactusScale, secondCircleOnRight, -45 - 90, this->cactusPartMF);

	// levi kaktus
	this->DrawCactus(pDC, mediumCactusScale, bottomCircleOnLeft, 90, this->cactusPartMF);

	// kaktus iznad levog
	this->DrawCactus(pDC, mediumCactusScale, bottomCircleOnLeft, 0, this->cactusPartMF);

	// kaktus gore levo
	this->DrawCactus(pDC, fatCactusScale, topCircleOnLeft, 0, this->cactusPartMF);

	// staticni kruzic
	this->DrawCircle(pDC, bottomCircleInVase, squareLength, RGB(0, 0, 0), CIRCLE_GREEN);

	// kruzici koji se rotiraju sa celom slikom

	this->DrawCircle(pDC, circleAbove, squareLength, RGB(0, 0, 0), CIRCLE_GREEN);

	this->DrawCircle(pDC, firstCircleOnRight, squareLength, RGB(0, 0, 0), CIRCLE_GREEN);
	this->DrawCircle(pDC, secondCircleOnRight, squareLength, RGB(0, 0, 0), CIRCLE_GREEN);

	this->DrawCircle(pDC, bottomCircleOnLeft, squareLength, RGB(0, 0, 0), CIRCLE_GREEN);
	this->DrawCircle(pDC, topCircleOnLeft, squareLength, RGB(0, 0, 0), CIRCLE_GREEN);

	this->UndoWorldTransform(pDC, xFormOld);
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

void CIND18015View::DrawCircle(CDC* pDC, POINT point, int r, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 1, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	pDC->Ellipse(point.x - r / 2, point.y + r / 2, point.x + r / 2, point.y - r / 2);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void CIND18015View::DrawTextString(CDC* pDC, int x, int y, const CString& text, LOGFONTW logFont, COLORREF color, COLORREF strokeColor)
{
	CFont font;
	font.CreateFontIndirectW(&logFont);

	CFont* pOldFont = pDC->SelectObject(&font);

	COLORREF oldColor = pDC->SetTextColor(color);
	int nOldMode = pDC->SetBkMode(TRANSPARENT);

	CPen pen(PS_SOLID | PS_GEOMETRIC, 1, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	pDC->TextOutW(x + 3, y + 3, CString("18015 Andrija Tosic"));
	pDC->SetTextColor(strokeColor);
	pDC->TextOutW(x, y, text);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(oldPen);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(nOldMode);

	font.DeleteObject();
}


// CIND18015View message handlers

void CIND18015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString rotAngle = CString(std::to_string(this->smallCactusRotAngle).c_str());

	switch (nChar)
	{
	case 'G':
		this->gridEnabled = !this->gridEnabled;
		break;
	case VK_LEFT:
		this->smallCactusRotAngle += 10;
		break;
	case VK_RIGHT:
		this->smallCactusRotAngle -= 10;
		break;
	case 'A':
		this->wholeCactusRotAngle += 10;
		break;
	case 'D':
		this->wholeCactusRotAngle -= 10;
		break;
	default:
		break;
	}

	InvalidateRect(NULL);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
