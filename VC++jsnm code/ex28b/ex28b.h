// ex28b.h : main header file for the EX28B application
//

#if !defined(AFX_EX28B_H__E38A6DBB_6F02_11D0_860F_D6B8FDE4DD70__INCLUDED_)
#define AFX_EX28B_H__E38A6DBB_6F02_11D0_860F_D6B8FDE4DD70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEx28bApp:
// See ex28b.cpp for the implementation of this class
//

class CEx28bApp : public CWinApp
{
public:
	CEx28bApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEx28bApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEx28bApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EX28B_H__E38A6DBB_6F02_11D0_860F_D6B8FDE4DD70__INCLUDED_)
