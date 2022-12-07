
// I_Kolokvijum_2016View.cpp : implementation of the CIKolokvijum2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I_Kolokvijum_2016.h"
#endif

#include "I_Kolokvijum_2016Doc.h"
#include "I_Kolokvijum_2016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RAD(a) (a * 3.14/180)

// CIKolokvijum2016View

IMPLEMENT_DYNCREATE(CIKolokvijum2016View, CView)

BEGIN_MESSAGE_MAP(CIKolokvijum2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIKolokvijum2016View construction/destruction

CIKolokvijum2016View::CIKolokvijum2016View() noexcept
{
	this->felt2.Load(L"res\\felt2.jpg");
	this->wood.Load(L"res\\wood.jpg");
}

CIKolokvijum2016View::~CIKolokvijum2016View()
{
}

BOOL CIKolokvijum2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKolokvijum2016View drawing

void CIKolokvijum2016View::OnDraw(CDC* pDC)
{
	CIKolokvijum2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect c;
	GetClientRect(&c);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	int oldm = memDC.SetGraphicsMode(GM_ADVANCED);

	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, c.Width(), c.Height());

	memDC.SelectObject(&bm);

	XFORM told;
	memDC.GetWorldTransform(&told);

	int ww = c.Width() / 20;
	int wh = c.Height() / 20;

	CRect table(ww, wh, c.Width() - ww, c.Height() - wh);
	
	DrawBorder(&memDC, c, 30);
	DrawTable(&memDC, table);
	DrawHoles(&memDC, c, 50);

	int b = 40;

	Translate(&memDC, 600, 400, false);
	
	if (stapTrans < -b/3.0 && !collided) {
		collided = true;
		kuglaDeltaX = -cos(RAD(stapRot)) * 10;
		kuglaDeltaY = -sin(RAD(stapRot)) * 10;
		stapRotCollision = stapRot;
	}

	XFORM tBeforeBall;
	memDC.GetWorldTransform(&tBeforeBall);

	if (collided) {
		// Kretanje kugle nakon udarca stapa.
		kuglaDeltaX += -cos(RAD(stapRotCollision))*10;
		kuglaDeltaY += -sin(RAD(stapRotCollision)) * 10;

		Translate(&memDC, kuglaDeltaX, kuglaDeltaY, false);
	}

	DrawBall(&memDC, b);

	memDC.SetWorldTransform(&tBeforeBall);

	Translate(&memDC, b / 2, b / 2 - 600/100, false);
	Rotate(&memDC, stapRot, false);
	Translate(&memDC, b, 0, false);

	if (!collided) {
		Translate(&memDC, stapTrans, 0, false);
	}

	DrawStick(&memDC, 600);

	memDC.SetWorldTransform(&told);

	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetGraphicsMode(oldm);
	memDC.DeleteDC();

}


// CIKolokvijum2016View printing

BOOL CIKolokvijum2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKolokvijum2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKolokvijum2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKolokvijum2016View diagnostics

#ifdef _DEBUG
void CIKolokvijum2016View::AssertValid() const
{
	CView::AssertValid();
}

void CIKolokvijum2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKolokvijum2016Doc* CIKolokvijum2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKolokvijum2016Doc)));
	return (CIKolokvijum2016Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKolokvijum2016View message handlers

void CIKolokvijum2016View::DrawStick(CDC* pDC, int w)
{
	CBrush oker(RGB(255, 190, 28));
	CPen bpen(PS_SOLID, 1, RGB(0, 0, 0));

	auto oldb = pDC->SelectObject(&oker);
	auto oldp = pDC->SelectObject(&bpen);

	pDC->MoveTo(0, 0);
	pDC->BeginPath();
	{
		pDC->LineTo(2 * w / 3, -w / 75);
		pDC->LineTo(2 * w / 3, w / 100 + w / 75);
		pDC->LineTo(0, w / 100);
		pDC->LineTo(0, 0);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();


	CBrush brown(RGB(64, 32, 0));
	pDC->SelectObject(&brown);

	pDC->MoveTo(2 * w / 3, -w / 75);
	pDC->BeginPath();
	{
		pDC->LineTo(w, -w / 50);
		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(w - w / 50, -w / 50, w + w / 50, w / 100 + w / 50, w, -w / 50, w, w / 100 + w / 50);
		pDC->LineTo(2 * w / 3, w / 100 + w / 75);
		pDC->LineTo(2 * w / 3, -w / 75);
	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();
}

void CIKolokvijum2016View::DrawBall(CDC* pDC, int w)
{
	CBrush brush(RGB(192, 0, 0));
	CBrush* oldb = pDC->SelectObject(&brush);

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldp = pDC->SelectObject(&pen);

	pDC->Ellipse(0, 0, w, w);

	CBrush white(RGB(255, 255, 255));
	CPen whiteP(PS_SOLID, 1, RGB(255, 255, 255));

	pDC->SelectObject(&white);
	pDC->SelectObject(&whiteP);

	pDC->Ellipse(w / 2, w / 4, w / 2 + w / 6, w / 4 + w / 6);

	pDC->SelectObject(oldb);
	pDC->SelectObject(oldp);
}

void CIKolokvijum2016View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = {
		1, 0,
		0, 1,
		dX, dY
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKolokvijum2016View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t = {
		 cos(RAD(angle)), sin(RAD(angle)),
		-sin(RAD(angle)), cos(RAD(angle))
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIKolokvijum2016View::DrawTable(CDC* pDC, CRect rect)
{
	int w = felt2.Width();
	int h = felt2.Height();

	for (int i = 0; i < rect.Width() / w; i++) {
		for (int j = 0; j < rect.Height() / h; j++) {
			felt2.Draw(pDC, CRect(0, 0, w, h), CRect(rect.left + i * w, rect.top + j * h, rect.left + i * w + w, rect.top + j * h + h));
		}
	}

}

void CIKolokvijum2016View::DrawBorder(CDC* pDC, CRect rect, int w)
{
	wood.Draw(pDC, CRect(0, 0, wood.Width(), wood.Height()), rect);
}

void CIKolokvijum2016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	CBrush b(RGB(0, 0, 0));

	CBrush* oldb = pDC->SelectObject(&b);

	//gorelevo
	pDC->Ellipse(size / 2, size / 2, size + size / 2, size + size / 2);

	//gore
	pDC->Ellipse(rect.Width() / 2 - size / 2, size / 2, rect.Width() / 2 + size / 2, size + size / 2);

	//goredesno
	pDC->Ellipse(rect.Width() - size - size / 2, size / 2, rect.Width() - size / 2, size + size / 2);

	//dolelevo
	pDC->Ellipse(size / 2, rect.Height() - size - size / 2, size + size / 2, rect.Height() - size / 2);

	//dole
	pDC->Ellipse(rect.Width() / 2 - size / 2, rect.Height() - size - size / 2, rect.Width() / 2 + size / 2, rect.Height() - size / 2);

	//doledesno
	pDC->Ellipse(rect.Width() - size - size / 2, rect.Height() - size - size / 2, rect.Width() - size / 2, rect.Height() - size / 2);

	pDC->SelectObject(oldb);
}


void CIKolokvijum2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		this->stapRot -= 10;
		break;
	case VK_RIGHT:
		this->stapRot += 10;
		break;
	case VK_UP:
		this->stapTrans -= 10;

		break;
	case VK_DOWN:
		if (collided) {
			stapTrans = 0;
		}
		
		collided = false;
		stapRotCollision = 0;
		kuglaDeltaX = 0;
		kuglaDeltaY = 0;
		this->stapTrans += 10;

	default:
		break;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CIKolokvijum2016View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}
