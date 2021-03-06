#include "stdafx.h"
#include "GDI.h"
#include "MainFrm.h"
#include "GDIDoc.h"
#include "GDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGDIView, CScrollView)

BEGIN_MESSAGE_MAP(CGDIView, CScrollView)
	//{{AFX_MSG_MAP(CGDIView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

CGDIView::CGDIView()
{
	CBitmap bmp;

	m_pPalOld=NULL;
	m_pBmpOld=NULL;
	bmp.LoadBitmap(IDB_BITMAP_GRID);
	ASSERT(bmp.GetSafeHandle());
	m_brGrid.CreatePatternBrush(&bmp);
	ASSERT(m_brGrid.GetSafeHandle());
	m_trackerSel.m_nStyle=
	(
		CRectTracker::dottedLine | CRectTracker::resizeOutside
	);
}

CGDIView::~CGDIView()
{
}

BOOL CGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CGDIView::OnDraw(CDC* pDC)
{
	CPalette *pPal;
	CPalette *pPalOld;
	BITMAP bm;
	int nRatio;
	POINT pt;

	if(m_bmpDraw.GetSafeHandle() == NULL)return;
	
	CGDIDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	pPal=pDoc->GetPalette();
	pPalOld=pDC->SelectPalette(pPal, FALSE);
	pDC->RealizePalette();
	nRatio=pDoc->GetRatio();

	pt=GetScrollPosition();

	m_bmpDraw.GetBitmap(&bm);
	pDC->StretchBlt
	(
		0, 0,
		bm.bmWidth*nRatio, bm.bmHeight*nRatio,
		&m_dcMem,
		0, 0,
		bm.bmWidth,	bm.bmHeight,
		SRCCOPY
	);
	if(pDoc->GetGridOn() == TRUE && nRatio > 2)
	{
		CBrush *pBrOld;
		CSize size;
		int i;

		size.cx=bm.bmWidth*nRatio;
		size.cy=bm.bmHeight*nRatio;

		m_brGrid.UnrealizeObject();
		pDC->SetBrushOrg(pt.x%2 ? 1:0, pt.y%2 ? 1:0);
		pBrOld=pDC->SelectObject(&m_brGrid);
		for(i=0; i<bm.bmWidth; i++)
		{
			pDC->PatBlt(i*nRatio, 0, 1, size.cy, PATCOPY);
		}
		for(i=0; i<bm.bmHeight; i++)
		{
			pDC->PatBlt(0, i*nRatio, size.cx, 1, PATCOPY);
		}
		pDC->SelectObject(pBrOld);
	}
	if(!m_trackerSel.m_rect.IsRectEmpty())
	{
		CPoint ptScroll;

		ptScroll=GetScrollPosition();
		m_trackerSel.m_rect.OffsetRect(-ptScroll);
		m_trackerSel.Draw(pDC);
		m_trackerSel.m_rect.OffsetRect(ptScroll);
	}

	pDC->SelectPalette(pPalOld, FALSE);
}

BOOL CGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void CGDIView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGDIView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGDIDoc* CGDIView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIDoc)));
	return (CGDIDoc*)m_pDocument;
}
#endif

void CGDIView::OnInitialUpdate() 
{
	CGDIDoc *pDoc;
	HGLOBAL hData;

	CScrollView::OnInitialUpdate();

	pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	hData=pDoc->GetHDib();
	if(hData != NULL)LoadBitmap(hData);
	else SetScrollSizes(MM_TEXT, CSize(0, 0));
}

void CGDIView::LoadBitmap(HGLOBAL hData)
{
	CPalette *pPalDraw;
	CPalette *pPalOld;
	CClientDC dc(this);
	HBITMAP hBmp;
	LPBYTE pBits;
	LPBITMAPINFO lpBi;
	LPLOGPALETTE lpLogPal;
	int nSizeCT;
	int i;
	int nRatio;
	CGDIDoc *pDoc;

	pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	AfxGetApp()->DoWaitCursor(TRUE);
	nRatio=pDoc->GetRatio();
	lpBi=(LPBITMAPINFO)::GlobalLock(hData);
	ASSERT(lpBi);
	nSizeCT=pDoc->GetColorTableSize(lpBi->bmiHeader.biBitCount);
	pPalDraw=pDoc->GetPalette();
	if(nSizeCT != 0)
	{
		lpLogPal=(LPLOGPALETTE) new BYTE
		[
			sizeof(LOGPALETTE)+(nSizeCT-1)*sizeof(PALETTEENTRY)
		];
		for(i=0; i<nSizeCT; i++)
		{
			lpLogPal->palPalEntry[i].peRed=lpBi->bmiColors[i].rgbRed;
			lpLogPal->palPalEntry[i].peGreen=lpBi->bmiColors[i].rgbGreen;
			lpLogPal->palPalEntry[i].peBlue=lpBi->bmiColors[i].rgbBlue;
			lpLogPal->palPalEntry[i].peFlags=NULL;
		}
		pPalDraw->SetPaletteEntries(0, nSizeCT, lpLogPal->palPalEntry);
		((CMainFrame *)(AfxGetApp()->m_pMainWnd))->UpdateColorBar();
		delete [](BYTE *)lpLogPal;
		pPalOld=dc.SelectPalette(pPalDraw, FALSE);
		dc.RealizePalette();
	}

	CleanUp();
	hBmp=::CreateDIBSection
	(
		dc.GetSafeHdc(),
		lpBi,
		DIB_RGB_COLORS,
		(void **)&pBits,
		NULL,
		0
	);
	memcpy
	(
		(LPSTR)pBits,
		(LPSTR)lpBi+sizeof(BITMAPINFOHEADER)+nSizeCT*sizeof(RGBQUAD), 
		lpBi->bmiHeader.biSizeImage
	);
	ASSERT(hBmp);
	m_bmpDraw.Attach(hBmp);
	m_dcMem.CreateCompatibleDC(&dc);
	ASSERT(m_dcMem.GetSafeHdc());

	if(nSizeCT != 0)
	{
		m_pPalOld=m_dcMem.SelectPalette(pPalDraw, FALSE);
	}
	m_pBmpOld=m_dcMem.SelectObject(&m_bmpDraw);

	UpdateScrollSizes();
	::GlobalUnlock(hData);
	if(nSizeCT != 0)dc.SelectPalette(pPalOld, FALSE);

	Invalidate();
	AfxGetApp()->DoWaitCursor(FALSE);
}

void CGDIView::CleanUp()
{
	if(m_pPalOld != NULL)
	{
		m_dcMem.SelectPalette(m_pPalOld, FALSE);
		m_pPalOld=NULL;
	}
	if(m_pBmpOld != NULL)
	{
		m_dcMem.SelectObject(m_pBmpOld);
		m_pBmpOld=NULL;
	}
	if(m_dcMem.GetSafeHdc() != NULL)m_dcMem.DeleteDC();
	if(m_bmpDraw.GetSafeHandle() != NULL)m_bmpDraw.DeleteObject();
}

void CGDIView::OnDestroy() 
{
	CleanUp();
	CScrollView::OnDestroy();
}

void CGDIView::UpdateScrollSizes()
{
	BITMAP bm;
	int nRatio;
	CGDIDoc *pDoc;

	pDoc=(CGDIDoc *)GetDocument();
	nRatio=pDoc->GetRatio();

	m_bmpDraw.GetBitmap(&bm);
	SetScrollSizes
	(
		MM_TEXT,
		CSize
		(
			bm.bmWidth*nRatio,
			bm.bmHeight*nRatio
		)
	);
}

void CGDIView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nCurrentTool;
	CGDIDoc *pDoc;

	if(m_bmpDraw.GetSafeHandle() != NULL)
	{
		pDoc=(CGDIDoc *)GetDocument();
		nCurrentTool=pDoc->GetCurrentTool();
		SetCapture();
		m_ptMouseDown=NormalizePtPosition(point);
		switch(nCurrentTool)
		{
			case TOOL_RECTSEL:
			{
				int nHitTest;

				nHitTest=m_trackerSel.HitTest(point);
				if(nHitTest == CRectTracker::hitNothing)
				{
					m_trackerSel.m_rect.left=m_trackerSel.m_rect.right=point.x+GetScrollPosition().x;
					m_trackerSel.m_rect.top=m_trackerSel.m_rect.bottom=point.y+GetScrollPosition().y;
				}
				else
				{
					::ReleaseCapture();
					m_trackerSel.Track(this, point);
					ResumeBackupBmp();
					StretchCopySelection();
					Invalidate();
				}
				break;
			}
			case TOOL_PEN:
			{
				DrawPoint(m_ptMouseDown);
				pDoc->SetModifiedFlag(TRUE);
				break;
			}
			case TOOL_LINE:
			{
				BackupCurrentBmp();
				DrawPoint(m_ptMouseDown);
				pDoc->SetModifiedFlag(TRUE);
				break;
			}
		}
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CGDIView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int nCurrentTool;
	CGDIDoc *pDoc;
	CPoint pt;

	if(m_bmpDraw.GetSafeHandle() != NULL)
	{
		pDoc=(CGDIDoc *)GetDocument();
		nCurrentTool=pDoc->GetCurrentTool();
		::ReleaseCapture();
		pt=NormalizePtPosition(point);
		switch(nCurrentTool)
		{
			case TOOL_RECTSEL:
			{
				CClientDC dc(this);

				if(!m_trackerSel.m_rect.IsRectEmpty())
				{
					dc.DrawFocusRect(m_trackerSel.m_rect);
				}
				m_trackerSel.m_rect.right=point.x+GetScrollPosition().x;
				m_trackerSel.m_rect.bottom=point.y+GetScrollPosition().y;
				BackupCurrentBmp();
				BackupSelection();
				Invalidate(FALSE);
				break;
			}
			case TOOL_PEN:
			{
				if(pt != m_ptMouseDown)
				{
					DrawPoint(point);
					pDoc->SetModifiedFlag(TRUE);
				}
				break;
			}
			case TOOL_LINE:
			{
				ResumeBackupBmp();
				DrawLine(m_ptMouseDown, pt);
				break;
			}
		}
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

void CGDIView::OnMouseMove(UINT nFlags, CPoint point) 
{
	int nCurrentTool;
	CGDIDoc *pDoc;
	CPoint pt;

	if(m_bmpDraw.GetSafeHandle() != NULL)
	{
		pDoc=(CGDIDoc *)GetDocument();
		nCurrentTool=pDoc->GetCurrentTool();
		pt=NormalizePtPosition(point);
		switch(nCurrentTool)
		{
			case TOOL_RECTSEL:
			{
				if(nFlags & MK_LBUTTON)
				{
					CClientDC dc(this);

					if(!m_trackerSel.m_rect.IsRectEmpty())
					{
						dc.DrawFocusRect(m_trackerSel.m_rect);
					}
					m_trackerSel.m_rect.right=point.x;
					m_trackerSel.m_rect.bottom=point.y;
					if(!m_trackerSel.m_rect.IsRectEmpty())
					{
						dc.DrawFocusRect(m_trackerSel.m_rect);
					}
				}
				break;
			}
			case TOOL_PEN:
			{
				if(nFlags & MK_LBUTTON)
				{
					if(pt != m_ptMouseDown)
					{
						DrawPoint(point);
						pDoc->SetModifiedFlag(TRUE);
					}
				}
				break;
			}
			case TOOL_LINE:
			{
				if(nFlags & MK_LBUTTON)
				{
					ResumeBackupBmp();
					DrawLine(m_ptMouseDown, pt);
				}
				break;
			}
		}
	}
	CScrollView::OnMouseMove(nFlags, point);
}

CPoint CGDIView::NormalizePtPosition(CPoint pt)
{
	int nRatio;
	CGDIDoc *pDoc;

	pDoc=(CGDIDoc *)GetDocument();
	nRatio=pDoc->GetRatio();
	pt.Offset(GetScrollPosition());

	return CPoint(pt.x/nRatio, pt.y/nRatio);
}

void CGDIView::DrawPoint(CPoint pt)
{
	int nPalIndex;
	CGDIDoc *pDoc;

	pDoc=(CGDIDoc *)GetDocument();
	nPalIndex=pDoc->GetFgdIndex();
	m_dcMem.SetPixel(pt, PALETTEINDEX(nPalIndex));
	Invalidate(FALSE);
}

void CGDIView::DrawLine(CPoint ptStart, CPoint ptEnd)
{
	int nPalIndex;
	CGDIDoc *pDoc;
	CPen pen;
	CPen *pPenOld;

	pDoc=(CGDIDoc *)GetDocument();
	nPalIndex=pDoc->GetFgdIndex();
	pen.CreatePen(PS_SOLID, 1, PALETTEINDEX(nPalIndex));
	pPenOld=m_dcMem.SelectObject(&pen);
	m_dcMem.MoveTo(ptStart);
	m_dcMem.LineTo(ptEnd);
	m_dcMem.SelectObject(pPenOld);
	Invalidate(FALSE);
}

void CGDIView::BackupSelection()
{
	CDC dcMem;
	CPalette *pPalOld;
	CPalette *pPal;
	CGDIDoc *pDoc;
	BITMAP bm;
	CBitmap *pBmpOld;
	CRect rect;
	int nBgdIndex;

	if(m_bmpSelBackup.GetSafeHandle() != NULL)m_bmpSelBackup.DeleteObject();
	pDoc=(CGDIDoc *)GetDocument();
	pPal=pDoc->GetPalette();
	ASSERT(pPal->GetSafeHandle());
	nBgdIndex=pDoc->GetBgdIndex();
	dcMem.CreateCompatibleDC(&m_dcMem);
	pPalOld=dcMem.SelectPalette(pPal, FALSE);

	rect=NormalizeTrackerRect(m_trackerSel.m_rect);
	m_bmpDraw.GetBitmap(&bm);
	m_bmpSelBackup.CreateCompatibleBitmap(&m_dcMem, rect.Width(), rect.Height());
	pBmpOld=dcMem.SelectObject(&m_bmpSelBackup);
	dcMem.FillSolidRect(0, 0, rect.Width(), rect.Height(), PALETTEINDEX(nBgdIndex));
	if(rect.left < 0)rect.left=0;
	if(rect.top < 0)rect.top=0;
	if(rect.right >= bm.bmWidth)rect.right=bm.bmWidth-1;
	if(rect.bottom >= bm.bmHeight)rect.bottom=bm.bmHeight-1;
	dcMem.BitBlt
	(
		0, 0,
		rect.Width(), rect.Height(),
		&m_dcMem,
		rect.left, rect.top,
		SRCCOPY
	);

	dcMem.SelectObject(pBmpOld);
	dcMem.SelectPalette(pPal, FALSE);
}

void CGDIView::StretchCopySelection()
{
	CDC dcMem;
	CPalette *pPalOld;
	CPalette *pPal;
	CGDIDoc *pDoc;
	BITMAP bm;
	CBitmap *pBmpOld;
	CRect rect;

	if(m_bmpSelBackup.GetSafeHandle() == NULL)return;

	pDoc=(CGDIDoc *)GetDocument();
	pPal=pDoc->GetPalette();
	ASSERT(pPal->GetSafeHandle());
	dcMem.CreateCompatibleDC(&m_dcMem);
	pPalOld=dcMem.SelectPalette(pPal, FALSE);

	rect=NormalizeTrackerRect(m_trackerSel.m_rect);
	m_bmpSelBackup.GetBitmap(&bm);
	pBmpOld=dcMem.SelectObject(&m_bmpSelBackup);
	m_dcMem.StretchBlt
	(
		rect.left, rect.top,
		rect.Width(), rect.Height(),
		&dcMem,
		0, 0,
		bm.bmWidth, bm.bmHeight,
		SRCCOPY
	);

	dcMem.SelectObject(pBmpOld);
	dcMem.SelectPalette(pPal, FALSE);
}

void CGDIView::BackupCurrentBmp()
{
	CDC dcMem;
	CPalette *pPalOld;
	CPalette *pPal;
	CGDIDoc *pDoc;
	BITMAP bm;
	CBitmap *pBmpOld;

	if(m_bmpBackup.GetSafeHandle() != NULL)m_bmpBackup.DeleteObject();
	pDoc=(CGDIDoc *)GetDocument();
	pPal=pDoc->GetPalette();
	ASSERT(pPal->GetSafeHandle());
	dcMem.CreateCompatibleDC(&m_dcMem);
	pPalOld=dcMem.SelectPalette(pPal, FALSE);

	m_bmpDraw.GetBitmap(&bm);
	m_bmpBackup.CreateCompatibleBitmap(&m_dcMem, bm.bmWidth, bm.bmHeight);
	pBmpOld=dcMem.SelectObject(&m_bmpBackup);
	dcMem.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &m_dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pBmpOld);
	dcMem.SelectPalette(pPal, FALSE);
}

void CGDIView::ResumeBackupBmp()
{
	CDC dcMem;
	CPalette *pPalOld;
	CPalette *pPal;
	CGDIDoc *pDoc;
	BITMAP bm;
	CBitmap *pBmpOld;

	ASSERT(m_bmpBackup.GetSafeHandle() != NULL);
	pDoc=(CGDIDoc *)GetDocument();
	pPal=pDoc->GetPalette();
	ASSERT(pPal->GetSafeHandle());
	dcMem.CreateCompatibleDC(&m_dcMem);
	pPalOld=dcMem.SelectPalette(pPal, FALSE);

	m_bmpDraw.GetBitmap(&bm);
	pBmpOld=dcMem.SelectObject(&m_bmpBackup);
	m_dcMem.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pBmpOld);
	dcMem.SelectPalette(pPal, FALSE);
}

BOOL CGDIView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_bmpDraw.GetSafeHandle() != NULL)
	{
		if
		(
			!(
				m_trackerSel.m_rect.IsRectEmpty() != TRUE && 
				m_trackerSel.SetCursor(this, nHitTest) == TRUE
			)
		)
		{
			if
			(
				MouseWithinBitmap() == TRUE && 
				HTVSCROLL != nHitTest && HTHSCROLL != nHitTest
			)
			{
				
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
				return TRUE;
			}
		}
		else return TRUE;
	}
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CGDIView::MouseWithinBitmap()
{
	CPoint point;
	CPoint ptScroll;
	CRect rect;
	CRect rectBmp;
	BITMAP bm;
	int nRatio;
	CGDIDoc *pDoc;

	pDoc=(CGDIDoc *)GetDocument();
	nRatio=pDoc->GetRatio();
	ptScroll=GetScrollPosition();
	::GetCursorPos(&point);
	GetWindowRect(rect);
	point.Offset(-rect.left, -rect.top);
	point.Offset(ptScroll);

	ASSERT(m_bmpDraw.GetSafeHandle());
	m_bmpDraw.GetBitmap(&bm);
	rectBmp=CRect(0, 0, bm.bmWidth*nRatio, bm.bmHeight*nRatio);

	return rectBmp.PtInRect(point);
}

void CGDIView::ResetTracker()
{
	m_trackerSel.m_rect=CRect(0, 0, 0, 0);
}

CRect CGDIView::NormalizeTrackerRect(CRect rect)
{
	CRect rectClient;
	int nRatio;
	CGDIDoc *pDoc;

	pDoc=(CGDIDoc *)GetDocument();
	nRatio=pDoc->GetRatio();

	rect.NormalizeRect();
	rect.left/=nRatio;
	rect.top/=nRatio;
	rect.right/=nRatio;
	rect.bottom/=nRatio;
	return rect;
}
