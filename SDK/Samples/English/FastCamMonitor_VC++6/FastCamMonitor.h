// FastCamMonitor.h : 
//

#if !defined(AFX_FASTCAMMONITOR_H__49F6DED0_17C2_49C1_A7DF_1D0D2E7013F7__INCLUDED_)
#define AFX_FASTCAMMONITOR_H__49F6DED0_17C2_49C1_A7DF_1D0D2E7013F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"	

/////////////////////////////////////////////////////////////////////////////
//

class CFastCamMonitorApp : public CWinApp
{
public:
	CFastCamMonitorApp();

	//{{AFX_VIRTUAL(CFastCamMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CFastCamMonitorApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_FASTCAMMONITOR_H__49F6DED0_17C2_49C1_A7DF_1D0D2E7013F7__INCLUDED_)
