
// RG.AlgoritmiView.h : interface of the CRGAlgoritmiView class
//

#pragma once


class CRGAlgoritmiView : public CView
{
protected: // create from serialization only
	CRGAlgoritmiView() noexcept;
	DECLARE_DYNCREATE(CRGAlgoritmiView)

// Attributes
public:
	CRGAlgoritmiDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
//	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
//	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRGAlgoritmiView();
#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RG.AlgoritmiView.cpp
inline CRGAlgoritmiDoc* CRGAlgoritmiView::GetDocument() const
   { return reinterpret_cast<CRGAlgoritmiDoc*>(m_pDocument); }
#endif

