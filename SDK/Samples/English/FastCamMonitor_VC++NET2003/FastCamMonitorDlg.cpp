// FastCamMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FastCamMonitor.h"
#include "FastCamMonitorDlg.h"
#include ".\fastcammonitordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFastCamMonitorDlg dialog



CFastCamMonitorDlg::CFastCamMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastCamMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_BmpInfo = NULL;
	m_BmpImage = NULL;
	m_nRecFrame = 0;
	m_PlayFrameCount = 0;
	m_nStartFrame = 0;
	m_nEndFrame = 0;
	
}

void CFastCamMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISP_RECFRAME, m_disp_recframe);
	DDX_Control(pDX, IDC_DISP_STATUS1, m_disp_status1);
	DDX_Control(pDX, IDC_DEVICE_NAME, m_device_name);
	DDX_Control(pDX, IDC_SELECT_CAMERA, m_select_camera);
	DDX_Control(pDX, IDC_CHANGE_RATE, m_change_rate);
	DDX_Control(pDX, IDC_DISP_STATUS2, m_disp_status2);
	DDX_Control(pDX, IDC_DISP_TRIG, m_disp_trigmode);
	DDX_Control(pDX, IDC_CHANGE_SHUTTER, m_change_shutter);
	DDX_Control(pDX, IDC_CHANGE_RESOLUTION, m_change_resolution);
}

BEGIN_MESSAGE_MAP(CFastCamMonitorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_CHANGE_RATE, OnCbnSelchangeChangeRate)
	ON_CBN_SELCHANGE(IDC_CHANGE_SHUTTER, OnCbnSelchangeChangeShutter)
	ON_CBN_SELCHANGE(IDC_CHANGE_RESOLUTION, OnCbnSelchangeChangeResolution)
	ON_CBN_SELCHANGE(IDC_SELECT_CAMERA, OnCbnSelchangeSelectCamera)
	ON_BN_CLICKED(IDC_MODE_LIVE, OnBnClickedModeLive)
	ON_BN_CLICKED(IDC_MODE_MEMORY, OnBnClickedModeMemory)
	ON_BN_CLICKED(IDC_RECORD_MEMORY, OnBnClickedRecordMemory)
	ON_BN_CLICKED(IDC_RECORD_SAVE_HD, OnBnClickedRecordSaveHd)
	ON_BN_CLICKED(IDC_PLAY_MEMORY, OnBnClickedPlayMemory)
	ON_BN_CLICKED(IDC_STOP_MEMORY, OnBnClickedStopMemory)
	ON_BN_CLICKED(IDC_VERSION_INF, OnBnClickedVersionInf)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PROGRAM_EXIT, OnBnClickedProgramExit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFastCamMonitorDlg message handlers
//-----------------------------------------------------------------------------------------------
//	OnInitDialog()
//-----------------------------------------------------------------------------------------------
BOOL CFastCamMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//-----------------------------------------------------------------------------------------------
//	OnSysCommand();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//-----------------------------------------------------------------------------------------------
//	OnPaint();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// render a image
		UpdateMovieRangeWindow();
		CDialog::OnPaint();
	}
}

//-----------------------------------------------------------------------------------------------
//	render a image
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::UpdateMovieRangeWindow() 
{
	int			nCopyWidth;
	int			nCopyHeight;
	HBITMAP		myDIB;			
	HBITMAP		oldDIB;
	HDC			hDC;			
	HDC			hCompatiDC;		
	PAINTSTRUCT ps;				
	CString		msg;
	CRect		nRect;
	int			nCompressedWidth;
	int			nCompressedHeight;

	if(!m_BmpInfo)	return;

	CWnd* pMovieRangeWnd = GetDlgItem(IDC_MOVIE_RANGE);


	hDC = ::GetDC(*pMovieRangeWnd);


	::BeginPaint(*pMovieRangeWnd, &ps);


	hCompatiDC = ::CreateCompatibleDC(hDC);


	myDIB = ::CreateDIBitmap(hDC,&m_BmpInfo->bmiHeader,CBM_INIT,m_BmpImage,m_BmpInfo,DIB_RGB_COLORS);

	oldDIB = (HBITMAP)::SelectObject(hCompatiDC,myDIB);

	SetStretchBltMode(hDC, STRETCH_DELETESCANS);

	// the size of rendering image is 512*512 
	nCopyWidth  = m_BmpInfo->bmiHeader.biWidth;
	nCopyHeight = m_BmpInfo->bmiHeader.biHeight;
	if(nCopyWidth <= 512 && nCopyHeight <= 512){
		::StretchBlt(hDC,0,0,nCopyWidth,nCopyHeight,hCompatiDC,0,nCopyHeight,nCopyWidth,-nCopyHeight,SRCCOPY);
	}else{
		if(nCopyWidth>nCopyHeight){
			nCompressedHeight = nCopyHeight * 512 / nCopyWidth;
			::StretchBlt(hDC,0,0,512,nCompressedHeight,hCompatiDC,0,nCopyHeight,nCopyWidth,-nCopyHeight,SRCCOPY);
		}else{
			nCompressedWidth = nCopyWidth * 512 / nCopyHeight;
			::StretchBlt(hDC,0,0,nCompressedWidth,512,hCompatiDC,0,nCopyHeight,nCopyWidth,-nCopyHeight,SRCCOPY);
		}
	}
	::EndPaint(*pMovieRangeWnd,&ps);

	::SelectObject(hCompatiDC, oldDIB);
	::DeleteObject(myDIB);
	::DeleteObject(hCompatiDC);
	::ReleaseDC(*pMovieRangeWnd,hDC);
}

//-----------------------------------------------------------------------------------------------
//	clear a image
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::ClearMovieRangeWindow() 
{
	CString		msg;
	CRect		nRect;
	CPoint		nLT;
	CPoint		nRB;
	DWORD		dwCol;

	CWnd* pMovieRangeWnd = GetDlgItem(IDC_MOVIE_RANGE);
	CClientDC	RangeCLDC(pMovieRangeWnd);

	pMovieRangeWnd->GetClientRect(nRect);

	nLT = nRect.TopLeft();
	nRB = nRect.BottomRight();
	nRB.x = nRB.x+1;
	nRect.SetRect(nLT.x, nLT.y, nRB.x, nRB.y);

	dwCol = GetSysColor(COLOR_BTNFACE);		
	RangeCLDC.FillSolidRect(nRect,dwCol);	

}

//-----------------------------------------------------------------------------------------------
//	OnQueryDragIcon();
//-----------------------------------------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFastCamMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------
//	OnTimer(); 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnTimer(UINT nIDEvent)
{

	unsigned long nRet;
	unsigned long nErrorCode;
	CString	msg;

	switch(nIDEvent){
	case TIMER_MODE_PLAY:
			if(!m_IsDraw)
			{
				if(m_nRecFrame <= 0){
					msg = "There is no frame for playback";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
					return;
				}else if( m_nEndFrame < m_PlayFrameCount)
				{
					::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);
					CButton* button1;
					button1 = (CButton*)GetDlgItem(IDC_RECORD_MEMORY);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
					button1->EnableWindow(TRUE);

					m_disp_status1.SetWindowText("Playback has cmopleted");

					msg = "Playback has cmopleted";
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					return;
				}
				// display a memory image
				DspPlayCamera();
				m_PlayFrameCount++;
			}
			break;
	case TIMER_MODE_LIVE:
			if(!m_IsDraw)	DspLiveCamera(); //display a live image	
			break;
	case TIMER_MODE_RECORD:			
			// check the completion of recording
			if(m_Execute_Record==TRUE){
				// get a status 
				nRet = PDC_GetStatus(nDeviceNo,
									&nStatus,
									&nErrorCode);
				if(nRet == PDC_FAILED){
					msg = "error PDC_GetStatus";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
					EndWaitCursor();	
					return;
				}
				// if the status is not recording, recording has completed.
				if(nStatus != PDC_STATUS_REC && nStatus != PDC_STATUS_RECREADY){
				
					// stop the timer
					//ID NO. = TIME_MODE_RECORD 
					::KillTimer(GetSafeHwnd(), TIMER_MODE_RECORD);

					// set the live mode
					nRet = PDC_SetStatus(nDeviceNo,
										 PDC_STATUS_LIVE,
										 &nErrorCode);
					if (nRet == PDC_FAILED) {
						msg = "error PDC_SetStatus";
						AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
						return;
					}

					m_disp_status1.SetWindowText("Recording has completed");

					msg = "Recording has completed";
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					m_PlayFrameCount = 0;
	
					EndWaitCursor();		

					m_Execute_Record = FALSE;

					// Enable controls
					CButton* button1;
					button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
					button1->EnableWindow(TRUE);				
					button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_MODE_LIVE);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_MODE_MEMORY);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_PROGRAM_EXIT);
					button1->EnableWindow(TRUE);

					// set the live mode
					OnBnClickedModeLive();
				}else{
					BeginWaitCursor();		
				}
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

//-----------------------------------------------------------------------------------------------
//	OnShowWindow();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	unsigned long nRet;
	unsigned long nErrorCode;
	unsigned long IPList[PDC_MAX_DEVICE]; 
	unsigned long nCount;   
	unsigned long RateList[PDC_MAX_LIST_NUMBER];  
	PDC_DETECT_NUM_INFO DetectNumInfo;
	unsigned long i;
	char	cWk0[20];	
	CString ss;	
	CString	msg;

	m_IsDraw=FALSE;		

	// set the default value
	m_Width  = 512;
	m_Height = 512;

	m_Execute_Live	 = FALSE;	
	m_Execute_Record = FALSE;	

	m_device_name.SetWindowText("Unknown Device");
	m_disp_status1.SetWindowText("Stop");	

	// initialize PDCLIB
	nRet = PDC_Init(&nErrorCode);
	if (nRet == PDC_FAILED) {
			msg = "eroor PDC_Init";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}
	// device search
	IPList[0] = 0xC0A80000;//192.168.0.1-192.0.254
	nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,   
							IPList,             
							5,                    
							PDC_DETECT_AUTO,  
							&DetectNumInfo,
							&nErrorCode);
#if 0
	//device serch (if you serch the specified IP adress)
	IPList[0] = 0xC0A8000A;//192.168.0.10
	IPList[0] = 0xC0A8000B;//192.168.0.11
	nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,  
							IPList,                 
							2,                      
							PDC_DETECT_NORMAL,  
							&DetectNumInfo,
							&nErrorCode);
	//device serch (if you serch in PCI)
	nRet = PDC_DetectDevice(PDC_INTTYPE_PCI,		// PCI 
							NULL,                  
							0,                      
							0,   
							&DetectNumInfo,
							&nErrorCode);
#endif
	if (nRet == PDC_FAILED) {
			msg = "error PDC_DetectDevice";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}

	// opening the device 
	nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[0]), 
							&nDeviceNo,   
							&nErrorCode);
	if(nRet == PDC_FAILED) {
			msg = "error PDC_OpenDevice";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}

	// get the child device
	nRet = PDC_GetExistChildDeviceList(nDeviceNo,
									   &nCount,
									   ChildNo,
									   &nErrorCode);
	if (nRet == PDC_FAILED) {
			msg = "error PDC_GetExistChildDeviceList";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}
	
	// display the list of child device
	m_select_camera.ResetContent();
	for(i=0; i<nCount; i++){
			sprintf(cWk0,"%d",ChildNo[i]);
			m_select_camera.InsertString(-1,cWk0);
	}

	m_select_camera.GetLBText(0,ss);
	m_select_camera.SetWindowText(ss);
	m_select_camera.SetCurSel(0);	

	// set the child device number as the current child device number
	nChildNo = atoi(ss);

	// get the device name
	char StrName[50];
	nRet = PDC_GetDeviceName(nDeviceNo,
							0,
							StrName,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "error PDC_GetDeviceName";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	// display the device name
	m_device_name.SetWindowText(StrName);

	// get the record rate list
	nRet = PDC_GetRecordRateList(nDeviceNo,
							nChildNo,
							&nCount,
							RateList,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "error PDC_GetRecordRateList";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	// display the list of record rate
	m_change_rate.ResetContent();
	for(i=0; i<nCount; i++){
			sprintf(cWk0,"%d",RateList[i]);
			m_change_rate.InsertString(-1,cWk0);
	}

	m_change_rate.GetLBText(3,ss);
	m_change_rate.SetWindowText(ss);
	m_change_rate.SetCurSel(3);	

	// set the record rate as the current record rate
	m_nRecordRate = atoi(ss);

	// set the record rate
	nRet = PDC_SetRecordRate(nDeviceNo,
							nChildNo,
							m_nRecordRate,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "error PDC_SetRecordRate";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}

	// update parameters(shutter speed, resolution, trigger mode)
	UpdateParam(); 	

	// set live mode
	OnBnClickedModeLive();
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_MODE_LIVE);
	button1->EnableWindow(TRUE);
	button1->SetCheck(1);
	button1 = (CButton*)GetDlgItem(IDC_MODE_MEMORY);
	button1->EnableWindow(TRUE);
}

//-----------------------------------------------------------------------------------------------
//	DspLiveCamera();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::DspLiveCamera()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	char nMode;
	CString	msg;

	m_IsDraw=TRUE;		

	// get a color type
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetColorType";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		
		if(m_BmpInfo) 
			delete []m_BmpInfo;
		m_BmpInfo = (LPBITMAPINFO)new char[sizeof(BITMAPINFO)];
		
		m_BmpInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		m_BmpInfo->bmiHeader.biWidth=m_Width;
		m_BmpInfo->bmiHeader.biHeight=m_Height;
		m_BmpInfo->bmiHeader.biPlanes=1;
		m_BmpInfo->bmiHeader.biBitCount=24;
		m_BmpInfo->bmiHeader.biSizeImage=0;
		m_BmpInfo->bmiHeader.biClrUsed=0;
		m_BmpInfo->bmiHeader.biClrImportant=0;
		m_BmpInfo->bmiHeader.biCompression=BI_RGB;
		m_BmpInfo->bmiHeader.biXPelsPerMeter=0;
		m_BmpInfo->bmiHeader.biYPelsPerMeter=0;

		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight*3];

	}else{				
		if(m_BmpInfo) 
			delete [] m_BmpInfo;
		m_BmpInfo= (LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER)+
								  (256)*sizeof(RGBQUAD)/*sizeof(BITMAPINFO)*/];
		
		m_BmpInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		m_BmpInfo->bmiHeader.biWidth=m_Width;
		m_BmpInfo->bmiHeader.biHeight=m_Height;
		m_BmpInfo->bmiHeader.biPlanes=1;
		m_BmpInfo->bmiHeader.biBitCount=8;
		m_BmpInfo->bmiHeader.biSizeImage=0;
		m_BmpInfo->bmiHeader.biClrUsed=256;
		m_BmpInfo->bmiHeader.biClrImportant=0;
		m_BmpInfo->bmiHeader.biCompression=BI_RGB;
		m_BmpInfo->bmiHeader.biXPelsPerMeter=0;
		m_BmpInfo->bmiHeader.biYPelsPerMeter=0;

		RGBQUAD				*mp_BmpRgbQuad;	//RGBQUAD
		mp_BmpRgbQuad = m_BmpInfo->bmiColors;

		for(int i=0; i<256; i++ ){
			mp_BmpRgbQuad[i].rgbBlue	= i;	
			mp_BmpRgbQuad[i].rgbGreen	= i;	
			mp_BmpRgbQuad[i].rgbRed		= i;	
			mp_BmpRgbQuad[i].rgbReserved	= 0;	
		}

		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight];
	}
	// get a live image
	nRet = PDC_GetLiveImageData(nDeviceNo,
                            nChildNo,
                            8,    
                            m_BmpImage,
                            &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetLiveImageData";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		delete [] m_BmpImage;
		return;
	}

	// render the image
	UpdateMovieRangeWindow(); 

	m_IsDraw=FALSE;		
}

//-----------------------------------------------------------------------------------------------
//	DspPlayCamera();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::DspPlayCamera()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	char nMode;
	CString	msg;
	
	m_IsDraw = TRUE;		

	// get color type information 
	nRet = PDC_GetColorType(nDeviceNo,
                     nChildNo,
                     &nMode,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetColorType";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	if(nMode == PDC_COLORTYPE_COLOR){		
			if(m_BmpInfo) 
				delete [] m_BmpInfo;
			m_BmpInfo = (LPBITMAPINFO)new char[sizeof(BITMAPINFO)];
			
			m_BmpInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			m_BmpInfo->bmiHeader.biWidth=m_Width;
			m_BmpInfo->bmiHeader.biHeight=m_Height;
			m_BmpInfo->bmiHeader.biPlanes=1;
			m_BmpInfo->bmiHeader.biBitCount=24;
			m_BmpInfo->bmiHeader.biSizeImage=0;
			m_BmpInfo->bmiHeader.biClrUsed=0;
			m_BmpInfo->bmiHeader.biClrImportant=0;
			m_BmpInfo->bmiHeader.biCompression=BI_RGB;
			m_BmpInfo->bmiHeader.biXPelsPerMeter=0;
			m_BmpInfo->bmiHeader.biYPelsPerMeter=0;
			if(m_BmpImage)	
				delete [] m_BmpImage;
			m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight*3];

	}else{					
		if(m_BmpInfo) 
			delete [] m_BmpInfo;
		m_BmpInfo = (LPBITMAPINFO)new char[sizeof(BITMAPINFOHEADER)+(256*sizeof(RGBQUAD))];

		m_BmpInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		m_BmpInfo->bmiHeader.biWidth=m_Width;
		m_BmpInfo->bmiHeader.biHeight=m_Height;
		m_BmpInfo->bmiHeader.biPlanes=1;
		m_BmpInfo->bmiHeader.biBitCount=8;
		m_BmpInfo->bmiHeader.biSizeImage=0;
		m_BmpInfo->bmiHeader.biClrUsed=0;
		m_BmpInfo->bmiHeader.biClrImportant=0;
		m_BmpInfo->bmiHeader.biCompression=BI_RGB;
		m_BmpInfo->bmiHeader.biXPelsPerMeter=0;
		m_BmpInfo->bmiHeader.biYPelsPerMeter=0;

		RGBQUAD				*mp_BmpRgbQuad;	
		mp_BmpRgbQuad = m_BmpInfo->bmiColors;


		for(int i=0; i<256; i++ ){
			mp_BmpRgbQuad[i].rgbBlue	= i;	
			mp_BmpRgbQuad[i].rgbGreen	= i;	
			mp_BmpRgbQuad[i].rgbRed		= i;	
			mp_BmpRgbQuad[i].rgbReserved	= 0;	
		}
		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight];
	}
	
	// get the recorded image
	nRet = PDC_GetMemImageData(nDeviceNo,			
								nChildNo,
							   m_PlayFrameCount,
							   8,
							   m_BmpImage,
							   &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetMemImageData";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		delete [] m_BmpImage;
		return;
	}
	// update the playback counter
	char	cWk0[50];
	sprintf(cWk0,"FrameNo.[%d]",m_PlayFrameCount);
	m_disp_status2.SetWindowText(cWk0);


	//render the image
	UpdateMovieRangeWindow(); 
	m_IsDraw = FALSE;	
}

//-----------------------------------------------------------------------------------------------
//	change the record rate
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnCbnSelchangeChangeRate()
{

	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	char	ss[10];
	CString msg;

	if(m_Execute_Live==FALSE){
		msg = "Please change to Live Mode";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	nSel = m_change_rate.GetCurSel();
	m_change_rate.GetLBText(nSel,ss);
	m_nRecordRate = atoi(ss);

	// set the selected record rate 
	nRet = PDC_SetRecordRate(nDeviceNo,
							nChildNo,
							m_nRecordRate,
							&nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_SetRecordRate";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	//clear the image
	ClearMovieRangeWindow(); 
	
	//update parameters(shutter speed, resolution, trigger mode)
	UpdateParam();
}

//-----------------------------------------------------------------------------------------------
//	change the shutter speed
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnCbnSelchangeChangeShutter()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	CString msg;

	if(m_Execute_Live==FALSE){
		msg = "Please change to Live Mode";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	BeginWaitCursor();	

	// get the selected shutter speed
	nSel = m_change_shutter.GetCurSel();

	m_nShutterSpeed = nShutterList[nSel];

	// set the selected shutter speed
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,
									nChildNo,
									m_nShutterSpeed,
									&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_SetShutterSpeedFps";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();
		return;
	}

	EndWaitCursor();	
}

//-----------------------------------------------------------------------------------------------
//	change the resolution
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnCbnSelchangeChangeResolution()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	CString msg;
	int		nWidth;
	int		nHeight;
	int		nInterval;
	char nMode;

	if(m_Execute_Live==FALSE){
		msg = "Please change to Live Mode";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	BeginWaitCursor();	
	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);


	// get the selected resolution
	nSel = m_change_resolution.GetCurSel();
	m_dwResolution = nResolutionList[nSel];
	nWidth	= m_dwResolution & 0xffff0000;
	nWidth	= nWidth>>16;
	nHeight = m_dwResolution & 0x0000ffff;

	// set the selected resolution
	nRet = PDC_SetResolution(nDeviceNo,
							nChildNo,
							nWidth,
							nHeight,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_SetResolution";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	
		return;
	}
	m_Width  = nWidth;
	m_Height = nHeight;	

	// get color type information
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetColorType";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}else{				

		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}

	// set the timer to display a live image
	//ID NO.  = TIMER_MODE_LIVE
	::SetTimer(GetSafeHwnd(), TIMER_MODE_LIVE, nInterval, NULL);

	// clear the image
	ClearMovieRangeWindow(); 
	
	EndWaitCursor();	
}

//-----------------------------------------------------------------------------------------------
//	Select the child device
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnCbnSelchangeSelectCamera()
{
	int		nSel;
	char	ss[10];
	CString	msg;

	if(m_Execute_Live==FALSE){
		msg = "Please change to Live Mode";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	
		return;
	}
	BeginWaitCursor();

	// get the selected child device
	nSel = m_select_camera.GetCurSel();
	m_select_camera.GetLBText(nSel,ss);

	// set the child device number as the current child device numbaer
	nChildNo = atoi(ss);

	m_IsDraw=FALSE;		
	EndWaitCursor();	
}
//-----------------------------------------------------------------------------------------------
//	Live Mode 
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnBnClickedModeLive()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nInterval;
	CString	msg;
	char nMode;

	if(m_Execute_Live==TRUE){
		return;
	}

	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);

	// set live mode
	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_LIVE,
						 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_SetStatus";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	// clear the image
	ClearMovieRangeWindow(); 

	// enable controls
	CComboBox* combo1;
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RATE);
	combo1->EnableWindow(TRUE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_SELECT_CAMERA);
	combo1->EnableWindow(TRUE);			
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RESOLUTION);
	combo1->EnableWindow(TRUE);			
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_SHUTTER);
	combo1->EnableWindow(TRUE);	
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_MEMORY);
	button1->EnableWindow(TRUE);
	//disable controls
	CButton* button2;
	button2 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button2->EnableWindow(FALSE);
	button2 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button2->EnableWindow(FALSE);
	button2 = (CButton*)GetDlgItem(IDC_STOP_MEMORY);
	button2->EnableWindow(FALSE);

	// get the resolution of live image
	nRet = PDC_GetResolution(nDeviceNo,
                     nChildNo,
                     &m_Width,
					 &m_Height,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetResolution";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}

	// get color type information
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetColorType";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}else{				
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}

	// set the timer to display a live image
	//ID NO.  = TIMER_MODE_LIVE
	::SetTimer(GetSafeHwnd(), TIMER_MODE_LIVE, nInterval, NULL);
	// clear the image
	ClearMovieRangeWindow(); 

	m_disp_status1.SetWindowText("Live");
	m_Execute_Live = TRUE;

	UpdateWindow();
}

//-----------------------------------------------------------------------------------------------
//	Memory Mode
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnBnClickedModeMemory()
{

	if(m_Execute_Live==FALSE){
		return;
	}
	// stop the timer
	//ID NO. = TIMER_MODE_LIVE 
	::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

	// set play back mode
	unsigned long nRet;
	unsigned long nErrorCode;
	CString	msg;
	m_Execute_Live = FALSE;	

	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_PLAYBACK,
						 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_SetStatus";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	// get the recorded frame information
	PDC_FRAME_INFO info;
	nRet = PDC_GetMemFrameInfo(nDeviceNo,
								nChildNo,
								&info,
								&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "error PDC_GetMemFrameInfo";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	m_nRecFrame = info.m_nRecordedFrames;
	m_nStartFrame = info.m_nStart;
	m_nEndFrame = info.m_nEnd;

	//display the frame number
	char	cWk0[10];
	sprintf(cWk0,"[%d]",m_nEndFrame);
	m_disp_recframe.SetWindowText(cWk0);

	// set the playback counter
	m_PlayFrameCount = m_nStartFrame;

	// disable the controls
	CComboBox* combo1;
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RATE);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_SELECT_CAMERA);
	combo1->EnableWindow(FALSE);
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RESOLUTION);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_SHUTTER);
	combo1->EnableWindow(FALSE);
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_MEMORY);
	button1->EnableWindow(FALSE);
	// enable the controls
	CButton* button2;
	button2 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button2->EnableWindow(TRUE);
	button2 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button2->EnableWindow(TRUE);
	button2 = (CButton*)GetDlgItem(IDC_STOP_MEMORY);
	button2->EnableWindow(TRUE);

	m_disp_status1.SetWindowText("Memory");

	// clear the display region
	ClearMovieRangeWindow(); 

	// get the recorded image resolution
	nRet = PDC_GetMemResolution(nDeviceNo,
                     nChildNo,
                     &m_Width,
					 &m_Height,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_GetMemResolution";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	DspPlayCamera();

	UpdateWindow();
}

//-----------------------------------------------------------------------------------------------
//	Record
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnBnClickedRecordMemory()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	CString msg;

	if(m_Execute_Live==FALSE){
		msg = "Please change to Live Mode";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	m_Execute_Live = FALSE;	

	m_disp_status1.SetWindowText("Rec ready");

	// stop the timer
	//ID NO. = TIMER_MODE_LIVE 
	::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);
	
	BeginWaitCursor();

	// get ready for recording
	nRet =PDC_SetRecReady(nDeviceNo,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_SetRecReady";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		OnBnClickedModeLive();
		return;
	}
	// check the status
	while(1){
		nRet = PDC_GetStatus(nDeviceNo,
							&nStatus,
							&nErrorCode);
		if(nRet == PDC_FAILED){
			msg = "error PDC_GetStatus";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
			EndWaitCursor();
			OnBnClickedModeLive();
			return;
		}
		if ((nStatus == PDC_STATUS_RECREADY) ||
			(nStatus == PDC_STATUS_REC))
		{
			break;
		}
	}
	// start recording
	nRet = PDC_TriggerIn(nDeviceNo,
						&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_TriggerIn";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		OnBnClickedModeLive();
		return;
	}
	// check the status
	int wait = GetTickCount();
	while(1){
		nRet = PDC_GetStatus(nDeviceNo,
							 &nStatus,
							 &nErrorCode);
		if (nRet == PDC_FAILED) {
			msg = "erro PDC_GetStatus";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			EndWaitCursor();
			OnBnClickedModeLive();
			return;
		}
		if (nStatus == PDC_STATUS_REC)
		{
			break;
		}
		if((int)(GetTickCount()-wait)>3000){	
			msg = "cannot start recording";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
			EndWaitCursor();	
			OnBnClickedModeLive();
			return;
		}
	}

	// enable controls
	CComboBox* combo1;
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RATE);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_SELECT_CAMERA);
	combo1->EnableWindow(FALSE);
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RESOLUTION);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_SHUTTER);
	combo1->EnableWindow(FALSE);
	// disable controls	
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_MEMORY);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_MODE_LIVE);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_MODE_MEMORY);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_PROGRAM_EXIT);
	button1->EnableWindow(FALSE);

	UpdateWindow();

	// set the timer to record
	//ID NO.  = TIMER_MODE_RECORD
	::SetTimer(GetSafeHwnd(), TIMER_MODE_RECORD, 250, NULL);

	m_Execute_Record = TRUE;	
	m_disp_status1.SetWindowText("Recording");

	EndWaitCursor();	
}

//-----------------------------------------------------------------------------------------------
//	Playback
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnBnClickedPlayMemory()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	CString msg;
	if(m_nRecFrame<=0){
		msg = "There is no frame for playback";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	// get a camera status
	nRet = PDC_GetStatus(nDeviceNo,
						&nStatus,
						&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_GetStatus";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();
		return;
	}
	// set playback status
	if(nStatus != PDC_STATUS_PLAYBACK){
		nRet = PDC_SetStatus(nDeviceNo,
							 PDC_STATUS_PLAYBACK,
							 &nErrorCode);
		if(nRet == PDC_FAILED){
			msg = "error PDC_SetStatus";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			EndWaitCursor();
			return;
		}
	}

	if(m_Execute_Live==TRUE){
		// stop the timer
		//ID NO. = TIMER_MODE_LIVE 
		::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

		m_disp_status1.SetWindowText("Stop");
		m_Execute_Live = FALSE;	
	}
	
	BeginWaitCursor();
	m_disp_status1.SetWindowText("Play");

	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button1->EnableWindow(FALSE);

	::SetTimer(GetSafeHwnd(), TIMER_MODE_PLAY, 250, NULL);

	EndWaitCursor();
}

//-----------------------------------------------------------------------------------------------
//	stop playback
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnBnClickedStopMemory()
{
	CString	msg;
	// stop the timer
	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);
	// enable controls
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button1->EnableWindow(TRUE);
	button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button1->EnableWindow(TRUE);

	m_disp_status1.SetWindowText("Stop");
}

//-----------------------------------------------------------------------------------------------
//	Download
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnBnClickedRecordSaveHd()
{
	unsigned long nRet;
	unsigned long nErrorCode;
	char nMode;
	unsigned long	i;
	int		nSts;
	CString	msg;
	char	cWk0[50];
	char	cPath[100];
	char	cPathTemp[100];
	char	cTime[100];
	SYSTEMTIME	tm;

	if(m_nRecFrame<=0){
		msg = "There is no frame for playback";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	
	// set playback status
	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_PLAYBACK,
							 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "error PDC_SetStatus";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		return;
	}
	if(m_Execute_Live==TRUE){	
		//stop the timer
		//ID NO. = TIMER_MODE_LIVE 
		::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

		m_disp_status1.SetWindowText("Stop");
		m_Execute_Live = FALSE;	
	}

	// disable controls
	CComboBox* combo1;
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RATE);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_SELECT_CAMERA);
	combo1->EnableWindow(FALSE);
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_RESOLUTION);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_CHANGE_SHUTTER);
	combo1->EnableWindow(FALSE);				
	combo1 = (CComboBox*)GetDlgItem(IDC_RECORD_MEMORY);
	combo1->EnableWindow(FALSE);
	UpdateWindow();
	BeginWaitCursor();	

	m_disp_status1.SetWindowText("Download");

	CFileDialog myDLG(FALSE,NULL,"FastCam",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"bitmap(*.bmp)|*.bmp|all(*.*)|*.*||");
	if(myDLG.DoModal() == IDOK)
	{
		// create folder to save images
		::GetLocalTime(&tm);				
		strcpy(cPath,myDLG.GetPathName());
		sprintf(cTime,"%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d\\",tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
		strcat(cPath,cTime);			
		strcpy(cPathTemp,cPath);			
		CreateDirectory(cPath,NULL);		

		for(i=m_nStartFrame; i<=m_nEndFrame; i++){
			nRet = PDC_GetMemImageData(nDeviceNo,
										nChildNo,
										i,
										8,
										m_BmpImage,
										&nErrorCode);
		
			// create BMP file
			strcpy(cPath,cPathTemp);
			sprintf(cWk0,"Temp%4.4d.bmp",i+1);
			strcat(cPath,cWk0);

			nRet = PDC_GetColorType(nDeviceNo,
						 nChildNo,
						 &nMode,
						 &nErrorCode);
			if (nRet == PDC_FAILED) {
				msg = "error PDC_GetColorType";
				AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
				EndWaitCursor();
				return;
			}
			if(nMode == PDC_COLORTYPE_COLOR){		
				nSts = WriteBmp24File(cPath, m_Width, m_Height);
				if(nSts<0){
					msg = "Fail to download";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
					EndWaitCursor();
					return;
				}
			}else{					
				nSts = WriteBmpFile(cPath, m_Width, m_Height);
				if(nSts<0){
					msg = "Fail to download";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
					EndWaitCursor();	
					return;
				}
			}
			sprintf(cWk0,"FrameNo.[%d]",i+1);
			m_disp_status2.SetWindowText(cWk0);

		}

		m_disp_status1.SetWindowText("Download End");

		msg = "Download End";
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
	}

	EndWaitCursor();
}

//-----------------------------------------------------------------------------------------------
//	version information
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnBnClickedVersionInf()
{
	CAboutDlg DlgAbout;
	DlgAbout.DoModal();	
}

//-----------------------------------------------------------------------------------------------
//	exit 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnBnClickedProgramExit()
{
	unsigned long nRet;
	CString msg;

	msg = "Do you want to exit program?";
	nRet = AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION);
	if(nRet==IDYES){
		CDialog::DestroyWindow();		
	}
}

//--------------------------------------------------------------------------------------------
// UpdateParameters (shutter speed, resolution, trigger mode)
//--------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::UpdateParam() 
{
	unsigned long nRet;
	unsigned long nErrorCode;
	unsigned long nSize;
	CString	msg;
	int		nWidth;
	int		nHeight;
	char	cWk0[50];
	unsigned long	i;
	CString ss;	

	BeginWaitCursor();	

	// get a list of shutterspeed
	nRet = PDC_GetShutterSpeedFpsList(nDeviceNo,
										nChildNo,
										&nSize,
										nShutterList,
										&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_GetShutterSpeedFpsList";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	
		return;
	}
	// display the list of shutterspeed
	m_change_shutter.ResetContent();
	for(i=0; i<nSize; i++){
			sprintf(cWk0,"1/%d",nShutterList[i]);
			m_change_shutter.InsertString(-1,cWk0);
	}

	m_change_shutter.SetCurSel(0);	
	// set the shutter speed as the current shutter speed
	m_nShutterSpeed = nShutterList[0];

	// set the shutterspeed
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,
									nChildNo,
									m_nShutterSpeed,
									&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_SetShutterSpeedFps";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();
		return;
	}

	// get a list of resolution
	nRet = PDC_GetResolutionList(nDeviceNo,
										nChildNo,
										&nSize,
										nResolutionList,
										&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_GetResolutionList";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();
		return;
	}	

	// display the list
	m_change_resolution.ResetContent();
	for(i=0; i<nSize; i++){
		nWidth	= nResolutionList[i] & 0xffff0000;
		nWidth	= nWidth>>16;
		nHeight = nResolutionList[i] & 0x0000ffff;
		sprintf(cWk0,"%dx%d",nWidth,nHeight);
		m_change_resolution.InsertString(-1,cWk0);
	}

	m_change_resolution.SetCurSel(0);	

	// set the resolution as the current resolution
	m_dwResolution = nResolutionList[0];
		
	// set the resolution
	nWidth	= m_dwResolution & 0xffff0000;
	nWidth	= nWidth>>16;
	nHeight = m_dwResolution & 0x0000ffff;

	nRet = PDC_SetResolution(nDeviceNo,
							nChildNo,
							nWidth,
							nHeight,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_SetResolution";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();
		return;
	}
	m_Width  = nWidth;
	m_Height = nHeight;

	// set the trigger mode
	nRet = PDC_SetTriggerMode(nDeviceNo,
								PDC_TRIGGER_START,
								0,
								0,
								0,
								&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "error PDC_SetTriggerMode";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();
		return;
	}
	
	// display the trigger mode
	m_disp_trigmode.SetWindowText("START");

	EndWaitCursor();	
}

//-----------------------------------------------------------------------------------------------
//	OnDestroy
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	unsigned long nRet;
	unsigned long nErrorCode;
	nRet = PDC_CloseDevice(nDeviceNo, &nErrorCode);

	// delete the buffer
	if(m_BmpInfo)	
		delete[] m_BmpInfo;
	if(m_BmpImage)	
		delete[] m_BmpImage;
}
//-----------------------------------------------------------------------------------------------
//	write BM file (8bit mono)
//-----------------------------------------------------------------------------------------------
int	CFastCamMonitorDlg::WriteBmpFile(char *szFName,int nWidth, int nHeight)
{
	int		i;
	int		x,y;
	int		nCnt;
	int		nPos;
	CFile	file;

	// open the file
	if(!file.Open(szFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		return(-1);
	}

	m_gnSizeFileHdr = sizeof( BITMAPFILEHEADER );
	m_gnSizeInfoHdr = sizeof( BITMAPINFOHEADER );	
	m_gnSizeRgbQuad = sizeof( RGBQUAD )*256;		
	m_gnSizeTop	 = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad;
	memcpy( (char *)&m_BmpFileHdr.bfType, "BM", 2 );

	m_gnSizeBmpImage = nWidth * nHeight;		
	m_BmpFileHdr.bfSize = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad + m_gnSizeBmpImage;
	m_BmpFileHdr.bfReserved1 = 0;				
	m_BmpFileHdr.bfReserved2 = 0;				
	m_BmpFileHdr.bfOffBits = m_gnSizeTop;		

	file.Write(&m_BmpFileHdr, sizeof(BITMAPFILEHEADER));


	m_BmpInfoHdr.biSize			= 40L;			
	m_BmpInfoHdr.biWidth		= nWidth;		
	m_BmpInfoHdr.biHeight		= nHeight;		
	m_BmpInfoHdr.biPlanes		= 1;			
	m_BmpInfoHdr.biBitCount 	= 8;			
	m_BmpInfoHdr.biCompression	= 0;			
	m_BmpInfoHdr.biSizeImage = m_gnSizeBmpImage;
	m_BmpInfoHdr.biXPelsPerMeter	= 19344;	
	m_BmpInfoHdr.biYPelsPerMeter	= 19344;	
	m_BmpInfoHdr.biClrUsed			= 256;		
	m_BmpInfoHdr.biClrImportant	=   0;			
	file.Write(&m_BmpInfoHdr,sizeof(BITMAPINFOHEADER));
	
	for( i=0; i<256; i++ ){
		m_BmpRgbQuad[i].rgbBlue		= i;
		m_BmpRgbQuad[i].rgbGreen	= i;	
		m_BmpRgbQuad[i].rgbRed		= i;	
		m_BmpRgbQuad[i].rgbReserved	= 0;
	}

	nCnt = sizeof(RGBQUAD)*256;
	file.Write(&m_BmpRgbQuad[0], nCnt);

	// write BMP image
	char* buf =  new char[m_BmpInfoHdr.biWidth];
	for(y=nHeight-1; y>=0; y--){

		nPos = nWidth * y;
		for(x=0; x<nWidth; x++){
			buf[x] = m_BmpImage[nPos];
			nPos++;
		}
	    file.Write(buf, nWidth);
	}
	delete[] buf;
	// close the file
	file.Close();

	return(0);
}

//-----------------------------------------------------------------------------------------------
//	write BM file (24bit color)
//-----------------------------------------------------------------------------------------------
int	CFastCamMonitorDlg::WriteBmp24File(char *szFName,int nWidth, int nHeight)
{
	int		x,y;
	int		nPos0;
	int		nPos1;
	CFile	file;

	// open the file
	if(!file.Open(szFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		return(-1);
	}

	m_gnSizeFileHdr = sizeof( BITMAPFILEHEADER );	
	m_gnSizeInfoHdr = sizeof( BITMAPINFOHEADER );	
	m_gnSizeRgbQuad = 0;							
	m_gnSizeTop	 = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad;
	memcpy( (char *)&m_BmpFileHdr.bfType, "BM", 2 );

	m_gnSizeBmpImage = nWidth * nHeight * 3;	
	m_BmpFileHdr.bfSize = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad + m_gnSizeBmpImage;
	m_BmpFileHdr.bfReserved1 = 0;			
	m_BmpFileHdr.bfReserved2 = 0;				
	m_BmpFileHdr.bfOffBits = m_gnSizeTop;		

	file.Write(&m_BmpFileHdr, sizeof(BITMAPFILEHEADER));

	m_BmpInfoHdr.biSize			= 40L;			
	m_BmpInfoHdr.biWidth		= nWidth;		
	m_BmpInfoHdr.biHeight		= nHeight;		
	m_BmpInfoHdr.biPlanes		= 1;			
	m_BmpInfoHdr.biBitCount 	= 24;		
	m_BmpInfoHdr.biCompression	= 0;			
	m_BmpInfoHdr.biSizeImage = m_gnSizeBmpImage;
	m_BmpInfoHdr.biXPelsPerMeter	= 0;	
	m_BmpInfoHdr.biYPelsPerMeter	= 0;		
	m_BmpInfoHdr.biClrUsed			= 0;		
	m_BmpInfoHdr.biClrImportant		= 0;		

	file.Write(&m_BmpInfoHdr,sizeof(BITMAPINFOHEADER));

	// write BMP image
	char* buf =  new char[nWidth * 3];
	for(y=nHeight-1; y>=0; y--){

		nPos0 = 0;
		nPos1 = nWidth*3*y;
		for(x=0; x<nWidth; x++){
			buf[nPos0]   = m_BmpImage[nPos1];
			buf[nPos0+1] = m_BmpImage[nPos1+1];
			buf[nPos0+2] = m_BmpImage[nPos1+2];
			nPos0 = nPos0+3;
			nPos1 = nPos1+3;
		}
		file.Write(buf, nWidth*3);
	}
	delete [] buf;

	// close the file
	file.Close();

	return(0);
}

