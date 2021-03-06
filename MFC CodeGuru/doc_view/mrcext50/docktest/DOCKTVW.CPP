// docktvw.cpp : implementation of the CDocktestView class
//
// Docktest demo of MRCEXT functionality
// $Revision:   1.0  $
// $Author:   MRC  $
// $Date:   11 Jul 1996 13:14:34  $

#include "stdafx.h"
#include "docktest.h"

#include "docktdoc.h"
#include "docktvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocktestView

IMPLEMENT_DYNCREATE(CDocktestView, CView)

BEGIN_MESSAGE_MAP(CDocktestView, CView)
	//{{AFX_MSG_MAP(CDocktestView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocktestView construction/destruction

CDocktestView::CDocktestView()
{
	// TODO: add construction code here

}

CDocktestView::~CDocktestView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDocktestView drawing

void CDocktestView::OnDraw(CDC* pDC)
{
	CDocktestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDocktestView printing

BOOL CDocktestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDocktestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDocktestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDocktestView diagnostics

#ifdef _DEBUG
void CDocktestView::AssertValid() const
{
	CView::AssertValid();
}

void CDocktestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDocktestDoc* CDocktestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocktestDoc)));
	return (CDocktestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDocktestView message handlers
