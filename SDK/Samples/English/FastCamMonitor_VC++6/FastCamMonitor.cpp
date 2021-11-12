// FastCamMonitor.cpp

#include "stdafx.h"
#include "FastCamMonitor.h"
#include "FastCamMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorApp

BEGIN_MESSAGE_MAP(CFastCamMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CFastCamMonitorApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorApp class

CFastCamMonitorApp::CFastCamMonitorApp()
{
}

/////////////////////////////////////////////////////////////////////////////

CFastCamMonitorApp theApp;

/////////////////////////////////////////////////////////////////////////////

BOOL CFastCamMonitorApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			
#else
	Enable3dControlsStatic();	
#endif

	CFastCamMonitorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
