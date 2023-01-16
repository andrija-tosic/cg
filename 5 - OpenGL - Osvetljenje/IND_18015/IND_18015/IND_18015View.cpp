
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

// CIND18015View

IMPLEMENT_DYNCREATE(CIND18015View, CView)

BEGIN_MESSAGE_MAP(CIND18015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
//	ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CIND18015View construction/destruction

CIND18015View::CIND18015View() noexcept
{
	// TODO: add construction code here

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

	m_glRenderer.DrawScene(pDC);
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


int CIND18015View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


BOOL CIND18015View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}


void CIND18015View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CIND18015View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


void CIND18015View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CIND18015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case '1':
		m_glRenderer.light1On = !m_glRenderer.light1On;
		break;
	case '2':
		m_glRenderer.light2On = !m_glRenderer.light2On;
		break;
	case '3':
		m_glRenderer.light3On = !m_glRenderer.light3On;
		break;
	case 'N':
		m_glRenderer.normalsOn = !m_glRenderer.normalsOn;
		break;
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	default:
		break;
	}
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CIND18015View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (rotationActive) {
		int diffx = point.x - btnDown.x;
		int diffy = point.y - btnDown.y;

		double sensitivity = 1 / 4.0;

		m_glRenderer.RotateCamera(diffy * sensitivity, diffx * sensitivity);
		Invalidate();

	}
	btnDown.x = point.x;
	btnDown.y = point.y;

	CView::OnMouseMove(nFlags, point);
}


void CIND18015View::OnLButtonDown(UINT nFlags, CPoint point)
{
	btnDown.x = point.x;
	btnDown.y = point.y;

	rotationActive = true;

	CView::OnLButtonDown(nFlags, point);
}

void CIND18015View::OnLButtonUp(UINT nFlags, CPoint point)
{
	rotationActive = false;

	CView::OnLButtonUp(nFlags, point);
}


BOOL CIND18015View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.ZoomCamera(zDelta > 0 ? false : true);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
