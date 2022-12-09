
// I_Kolokvijum_2015View.cpp : implementation of the CIKolokvijum2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I_Kolokvijum_2015.h"
#endif

#include "I_Kolokvijum_2015Doc.h"
#include "I_Kolokvijum_2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIKolokvijum2015View

IMPLEMENT_DYNCREATE(CIKolokvijum2015View, CView)

BEGIN_MESSAGE_MAP(CIKolokvijum2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIKolokvijum2015View construction/destruction

CIKolokvijum2015View::CIKolokvijum2015View() noexcept
{
	pozadina.Load(L"res\\blue.png");
}

CIKolokvijum2015View::~CIKolokvijum2015View()
{
}

BOOL CIKolokvijum2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Rotate(CDC* pDC, float rotAngle) {
	XFORM t = {
		cos(rotAngle * 3.14 / 180), sin(rotAngle * 3.14 / 180),
		-sin(rotAngle * 3.14 / 180), cos(rotAngle * 3.14 / 180)
	};
	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void Translate(CDC* pDC, int dx, int dy) {
	XFORM t = {
		1, 0,
		0, 1,
		dx, dy
	};
	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void Mirror(CDC* pDC, bool x, bool y) {
	XFORM t = {
		x ? -1 : 1, 0,
		0, y ? -1 : 1
	};
	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}
// CIKolokvijum2015View drawing

void CIKolokvijum2015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	CBrush b(RGB(255, 255, 0));
	CPen p(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* oldb = pDC->SelectObject(&b);
	CPen* oldp = pDC->SelectObject(&p);

	angle = angle / 2;

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;
	POINT middle = { (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 };

	XFORM tOld;
	pDC->GetWorldTransform(&tOld);

	Translate(pDC, middle.x, middle.y);
	Rotate(pDC, rot);
	Translate(pDC, -middle.x, -middle.y);

	pDC->MoveTo(middle);
	pDC->BeginPath();
	{
		if (!mouthOpen) {
			angle = 0;
		}

		POINT mouthTop = { middle.x + w / 2 * cos(angle * 3.14 / 180), middle.y - h / 2 * sin(angle * 3.14 / 180) };
		POINT mouthBottom = { middle.x + w / 2 * cos(angle * 3.14 / 180), middle.y + h / 2 * sin(angle * 3.14 / 180) };

		pDC->LineTo(mouthTop);
		pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		pDC->ArcTo(rect.left, rect.top, rect.right, rect.bottom, mouthTop.x, mouthTop.y, mouthBottom.x, mouthBottom.y);
		pDC->LineTo(middle);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	CBrush black(RGB(0, 0, 0));
	pDC->SelectObject(&black);

	int eyesize = 10;

	pDC->Ellipse(int(rect.left + w * 3.0 / 4.0), int(rect.top + h * 1 / 4.0), int(rect.left + w * 3.0 / 4.0 + eyesize), int(rect.top + h * 1 / 4.0 + eyesize));

	pDC->SelectObject(oldb);
	pDC->SelectObject(oldp);

	pDC->SetWorldTransform(&tOld);
}

void CIKolokvijum2015View::DrawGhost(CDC* pDC, CRect rect)
{
	CBrush b(RGB(255, 0, 0));
	CBrush* oldb = pDC->SelectObject(&b);
	pDC->Ellipse(rect);

	pDC->SelectObject(oldb);
}

void CIKolokvijum2015View::OnDraw(CDC* pDC)
{
	CIKolokvijum2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect c;
	GetClientRect(&c);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, c.Width(), c.Height());
	memDC.SelectObject(&bm);
	auto oldmode = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM tOld{};
	memDC.GetWorldTransform(&tOld);
	{

		pozadina.Draw(&memDC, CRect(0, 0, pozadina.Width(), pozadina.Height()), CRect(0, 0, c.Width(), c.Height()));
		
		POINT ghostMid = { (100 + 196) / 2, (120 + 216) / 2 };
		POINT pacmanMid = { (400 + 100 + 400 + dx + dx) / 2, (400 + 100 + 400 + dy +dy) / 2 };

		CPen white(PS_SOLID, 2, RGB(255, 255, 255));
		memDC.SelectObject(&white);
		memDC.MoveTo(ghostMid);
		memDC.LineTo(pacmanMid);


		int initialX = 400, initialY = 400;

		CRect pacman(initialX + dx, initialY + dy, initialX + dx + 100, initialY + dy + 100);

		if (mirrored) {
			Mirror(&memDC, true, false);
		}
		DrawPacman(&memDC, pacman, 20);
		memDC.SetWorldTransform(&tOld);

		DrawGhost(&memDC, CRect(100, 120, 196, 216));

		if (collided) {
			CFont font;
			font.CreateFontW(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");

			memDC.TextOutW(0, 0, L"The End");
		}

	}
	memDC.SetWorldTransform(&tOld);
	memDC.SetGraphicsMode(oldmode);

	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);


	// TODO: add draw code for native data here
}


// CIKolokvijum2015View printing

BOOL CIKolokvijum2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKolokvijum2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKolokvijum2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKolokvijum2015View diagnostics

#ifdef _DEBUG
void CIKolokvijum2015View::AssertValid() const
{
	CView::AssertValid();
}

void CIKolokvijum2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKolokvijum2015Doc* CIKolokvijum2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKolokvijum2015Doc)));
	return (CIKolokvijum2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKolokvijum2015View message handlers


void CIKolokvijum2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	POINT ghostMid = { (100 + 196) / 2, (120 + 216) / 2 };
	POINT pacmanMid = { (400 + 100 + 400 + dx + dx) / 2, (400 + 100 + 400 + dy + dy) / 2 };

	double r1 = sqrt((96/2) * (96/2) + (96/2) * (96/2));
	double r2 = sqrt(50 * 50 + 50 * 50);

	double radiusSum = r1 + r2;

	double xdiff = ghostMid.x - pacmanMid.x;
	double ydiff = ghostMid.y - pacmanMid.y;

	double distance = sqrt(xdiff*xdiff + ydiff*ydiff);

	if (distance <= radiusSum) {
		collided = true;
	}
	else {
		collided = false;
	}

	switch (nChar)
	{
	case VK_UP:
		rot = -90;
		dy -= 10;
		mirrored = false;
		mouthOpen = !mouthOpen;
		break;
	case VK_DOWN:
		rot = 90;
		dy += 10;
		mirrored = false;
		mouthOpen = !mouthOpen;
		break;
	case VK_LEFT:
		rot = 0;
		dx -= 10;
		mouthOpen = !mouthOpen;
		if (!previousLeft) {
			mirrored = true;
		}
		break;
	case VK_RIGHT:
		rot = 0;
		dx += 10;
		mirrored = false;
		mouthOpen = !mouthOpen;
		if (previousLeft) {
			mirrored = false;
		}
		break;
	default:
		break;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CIKolokvijum2015View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}
