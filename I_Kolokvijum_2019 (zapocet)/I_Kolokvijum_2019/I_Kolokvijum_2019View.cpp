
// I_Kolokvijum_2019View.cpp : implementation of the CIKolokvijum2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I_Kolokvijum_2019.h"
#endif

#include "I_Kolokvijum_2019Doc.h"
#include "I_Kolokvijum_2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RAD(a) (a * (3.14) / 180)

// CIKolokvijum2019View

IMPLEMENT_DYNCREATE(CIKolokvijum2019View, CView)

BEGIN_MESSAGE_MAP(CIKolokvijum2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CIKolokvijum2019View construction/destruction

CIKolokvijum2019View::CIKolokvijum2019View() noexcept
{
	this->background.Load(L"res\\background.jpg");
	this->arm1.Load(L"res\\arm1.png");
	this->arm2.Load(L"res\\arm2.png");
	this->leg1.Load(L"res\\leg1.png");
	this->leg2.Load(L"res\\leg2.png");
	this->body1.Load(L"res\\body1.png");
}

CIKolokvijum2019View::~CIKolokvijum2019View()
{
}

BOOL CIKolokvijum2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKolokvijum2019View drawing

void CIKolokvijum2019View::OnDraw(CDC* pDC)
{
	CIKolokvijum2019Doc* pDoc = GetDocument();
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

	auto old_mode = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM old_t{};
	memDC.GetWorldTransform(&old_t);
	{
		this->DrawBackground(&memDC, c);
		
		this->Translate(&memDC, 200, 200, false);
		
		this->DrawTransformer(&memDC);
	}
	memDC.SetWorldTransform(&old_t);
	memDC.SetGraphicsMode(old_mode);

	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);
}


// CIKolokvijum2019View printing

BOOL CIKolokvijum2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKolokvijum2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKolokvijum2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKolokvijum2019View diagnostics

#ifdef _DEBUG
void CIKolokvijum2019View::AssertValid() const
{
	CView::AssertValid();
}

void CIKolokvijum2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKolokvijum2019Doc* CIKolokvijum2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKolokvijum2019Doc)));
	return (CIKolokvijum2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKolokvijum2019View message handlers


BOOL CIKolokvijum2019View::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

void CIKolokvijum2019View::DrawBackground(CDC* pDC, CRect rc)
{
	this->background.Draw(pDC, CRect{ POINT{0, 0}, POINT{this->background.Width(), this->background.Height() } }, rc);
}

void CIKolokvijum2019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect c;
	GetClientRect(&c);

	int w = pImage->Width();
	int h = pImage->Height();

	CDC srcDC, dstDC;
	srcDC.CreateCompatibleDC(pDC);
	dstDC.CreateCompatibleDC(pDC);

	CBitmap* bmpImage = pImage->GetBitmap();
	CBitmap bmpMask;

	bmpMask.CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC.SelectObject(bmpImage);
	CBitmap* oldDest = dstDC.SelectObject(&bmpMask);

	srcDC.SetBkColor(srcDC.GetPixel(0,0));
	dstDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &dstDC, 0, 0, SRCAND);

	srcDC.SelectObject(oldSrc);
	dstDC.SelectObject(oldDest);

	srcDC.DeleteDC();
	dstDC.DeleteDC();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	memDC.SelectObject(&bmpMask);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCAND);

	memDC.SelectObject(bmpImage);
	pDC->BitBlt(0, 0, w, h, &memDC, 0, 0, SRCPAINT);
}

void CIKolokvijum2019View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = {
		0, 0,
		0, 0,
		dX, dY
	};

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKolokvijum2019View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t = {
	cos(RAD(angle)), sin(RAD(angle)),
	-sin(RAD(angle)), cos(RAD(angle))
	};

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIKolokvijum2019View::DrawArm1(CDC* pDC)
{
	this->Translate(pDC, 34, 31, false);
	this->Rotate(pDC, this->arm1Angle, false);

	this->Translate(pDC, 210, 102, false);
	this->Rotate(pDC, this->arm2Angle, false);

	this->DrawImgTransparent(pDC, &this->arm1);
}

void CIKolokvijum2019View::DrawArm2(CDC* pDC)
{
}

void CIKolokvijum2019View::DrawLeg1(CDC* pDC)
{
}

void CIKolokvijum2019View::DrawLeg2(CDC* pDC)
{
}

void CIKolokvijum2019View::DrawBody1(CDC* pDC)
{
}

void CIKolokvijum2019View::DrawTransformer(CDC* pDC)
{
	this->DrawArm1(pDC);
	this->DrawArm2(pDC);
	this->DrawLeg1(pDC);
	this->DrawLeg2(pDC);
	this->DrawBody1(pDC);
}
