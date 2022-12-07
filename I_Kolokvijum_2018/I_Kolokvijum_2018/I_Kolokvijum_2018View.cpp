
// I_Kolokvijum_2018View.cpp : implementation of the CIKolokvijum2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "I_Kolokvijum_2018.h"
#endif

#include "I_Kolokvijum_2018Doc.h"
#include "I_Kolokvijum_2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RAD(a) (a*3.14/180)

// CIKolokvijum2018View

IMPLEMENT_DYNCREATE(CIKolokvijum2018View, CView)

BEGIN_MESSAGE_MAP(CIKolokvijum2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKolokvijum2018View construction/destruction

CIKolokvijum2018View::CIKolokvijum2018View() noexcept
{
	arm1.Load(L"res\\arm1.png");
	arm2.Load(L"res\\arm2.png");
	bager.Load(L"res\\bager.png");
	pozadina.Load(L"res\\pozadina.png");
	viljuska = GetEnhMetaFile(L"res\\viljuska.emf");
}

CIKolokvijum2018View::~CIKolokvijum2018View()
{
}

BOOL CIKolokvijum2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKolokvijum2018View drawing

void CIKolokvijum2018View::OnDraw(CDC* pDC)
{
	CIKolokvijum2018Doc* pDoc = GetDocument();
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

	int oldmode = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM tOld;
	memDC.GetWorldTransform(&tOld);
	{



		DrawBackground(&memDC);

		Translate(&memDC, c.Width() * 0.6, c.Height() * 0.5, false);
		Translate(&memDC, bagerTrans, 0, false);

		DrawExcavator(&memDC);
	}
	memDC.SetWorldTransform(&tOld);
	memDC.SetGraphicsMode(oldmode);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), &memDC, 0, 0, SRCCOPY);

}


// CIKolokvijum2018View printing

BOOL CIKolokvijum2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKolokvijum2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKolokvijum2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKolokvijum2018View diagnostics

#ifdef _DEBUG
void CIKolokvijum2018View::AssertValid() const
{
	CView::AssertValid();
}

void CIKolokvijum2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKolokvijum2018Doc* CIKolokvijum2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKolokvijum2018Doc)));
	return (CIKolokvijum2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKolokvijum2018View message handlers

void CIKolokvijum2018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t{
		1,0,
		0,1,
		dX,dY
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKolokvijum2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t{
		cos(RAD(angle)), sin(RAD(angle)),
		-sin(RAD(angle)), cos(RAD(angle))
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIKolokvijum2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t{
		sX,0,
		0,sY,
	};
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CIKolokvijum2018View::DrawBackground(CDC* pDC)
{
	CRect c;
	GetClientRect(&c);

	int diffw = abs(c.Width() - pozadina.Width());
	int diffh = abs(c.Height() - pozadina.Height());

	pozadina.Draw(pDC, CRect(diffw / 2, diffh, diffw / 2 + c.Width(), pozadina.Height()),
		CRect(0, 0, c.Width(), c.Height()));
}

void CIKolokvijum2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
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

	srcDC.SetBkColor(srcDC.GetPixel(0, 0));
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
void CIKolokvijum2018View::DrawBody(CDC* pDC)
{
	DrawImgTransparent(pDC, &bager);

	Translate(pDC, 5, 225, false);
}

void CIKolokvijum2018View::DrawArm1(CDC* pDC)
{
	Rotate(pDC, -90, false);

	Translate(pDC, 58, 61, false);
	Rotate(pDC, arm1Rot, false);
	Translate(pDC, -58, -61, false);

	DrawImgTransparent(pDC, &arm1);

	Translate(pDC, 270, 96, false);
}

void CIKolokvijum2018View::DrawArm2(CDC* pDC)
{
	Rotate(pDC, -90, false);

	Translate(pDC, 36, 40, false);
	Rotate(pDC, arm2Rot, false);
	Translate(pDC, -36, -40, false);

	DrawImgTransparent(pDC, &arm2);

	Translate(pDC, 272 - 40, 40 + 25, false);
}

void CIKolokvijum2018View::DrawFork(CDC* pDC)
{
	Scale(pDC, 2.5, 2.5, false);
	Rotate(pDC, -90, false);

	Translate(pDC, 14, 20, false);
	Rotate(pDC, forkRot, false);
	Translate(pDC, -14, -20, false);


	ENHMETAHEADER mfHeader;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &mfHeader);

	PlayEnhMetaFile(pDC->m_hDC, viljuska, CRect(0, 0, mfHeader.rclBounds.right, mfHeader.rclBounds.bottom));
}

void CIKolokvijum2018View::DrawExcavator(CDC* pDC)
{
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}


BOOL CIKolokvijum2018View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}


void CIKolokvijum2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case '1':
		arm1Rot += 10;
		break;
	case '2':
		arm1Rot -= 10;
		break;
	case '3':
		arm2Rot += 10;
		break;
	case '4':
		arm2Rot -= 10;
		break;
	case '5':
		forkRot += 10;
		break;
	case '6':
		forkRot -= 10;
		break;

	case VK_LEFT:
		bagerTrans -= 10;
		break;
	case VK_RIGHT:
		bagerTrans += 10;
		break;



	default:
		break;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
