// FastCamMonitorDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "FastCamMonitor.h"
#include "FastCamMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h��������܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorDlg �_�C�A���O

CFastCamMonitorDlg::CFastCamMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastCamMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastCamMonitorDlg)
		// ����: ���̈ʒu�� ClassWizard �ɂ���ă����o�̏��������ǉ�����܂��B
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
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
	//{{AFX_DATA_MAP(CFastCamMonitorDlg)
	DDX_Control(pDX, IDC_DISP_RECFRAME, m_disp_recframe);
	DDX_Control(pDX, IDC_DISP_STATUS1, m_disp_status1);
	DDX_Control(pDX, IDC_DEVICE_NAME, m_device_name);
	DDX_Control(pDX, IDC_SELECT_CAMERA, m_select_camera);
	DDX_Control(pDX, IDC_CHANGE_RATE, m_change_rate);
	DDX_Control(pDX, IDC_DISP_STATUS2, m_disp_status2);
	DDX_Control(pDX, IDC_DISP_TRIG, m_disp_trigmode);
	DDX_Control(pDX, IDC_CHANGE_SHUTTER, m_change_shutter);
	DDX_Control(pDX, IDC_CHANGE_RESOLUTION, m_change_resolution);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFastCamMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CFastCamMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_CHANGE_RATE, OnSelchangeChangeRate)
	ON_CBN_SELCHANGE(IDC_CHANGE_SHUTTER, OnSelchangeChangeShutter)
	ON_CBN_SELCHANGE(IDC_CHANGE_RESOLUTION, OnSelchangeChangeResolution)
	ON_CBN_SELCHANGE(IDC_SELECT_CAMERA, OnSelchangeSelectCamera)
	ON_BN_CLICKED(IDC_MODE_LIVE, OnModeLive)
	ON_BN_CLICKED(IDC_MODE_MEMORY, OnModeMemory)
	ON_BN_CLICKED(IDC_RECORD_MEMORY, OnRecordMemory)
	ON_BN_CLICKED(IDC_RECORD_SAVE_HD, OnRecordSaveHd)
	ON_BN_CLICKED(IDC_PLAY_MEMORY, OnPlayMemory)
	ON_BN_CLICKED(IDC_STOP_MEMORY, OnStopMemory)
	ON_BN_CLICKED(IDC_VERSION_INF, OnVersionInf)
	ON_BN_CLICKED(IDC_PROGRAM_EXIT, OnProgramExit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorDlg ���b�Z�[�W �n���h��

//-----------------------------------------------------------------------------------------------
//	OnInitDialog()
//-----------------------------------------------------------------------------------------------
BOOL CFastCamMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
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

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

//-----------------------------------------------------------------------------------------------
//	OnPaint();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//	�摜�\���̈��`��
		UpdateMovieRangeWindow();
		CDialog::OnPaint();
	}
}
//-----------------------------------------------------------------------------------------------
//	�摜�\���̈��`��
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::UpdateMovieRangeWindow() 
{
	int			nCopyWidth;
	int			nCopyHeight;
	HBITMAP		myDIB;			//�r�b�g�}�b�v�̃n���h��
	HBITMAP		oldDIB;
	HDC			hDC;			//�f�o�C�X�R���e�L�X�g�̃n���h��
	HDC			hCompatiDC;		//�������f�o�C�X�R���e�L�X�g
	PAINTSTRUCT ps;				//�N���C�A���g�̈�̕`��Ɏg������ێ�
	CString		msg;
	CRect		nRect;
	int			nCompressedWidth;
	int			nCompressedHeight;

	//�摜�̏������o���Ă��Ȃ���΃��^�[��
	if(!m_BmpInfo)	return;

	CWnd* pMovieRangeWnd = GetDlgItem(IDC_MOVIE_RANGE);

	//m_hWnd�Ŏ������݂̃E�C���h�E�̃f�o�C�X�R���e�L�X�g���擾
	//���̃f�o�C�X�R���e�L�X�g�͕`��I�����ReleaseDC�ŊJ������K�v������
	hDC = ::GetDC(*pMovieRangeWnd);

	//�`��̏���������
	//�`�悪�I��������K��EndPaint�����s����K�v������
	::BeginPaint(*pMovieRangeWnd, &ps);

	//hDC�ƌ݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬����
	hCompatiDC = ::CreateCompatibleDC(hDC);

	//DIB�`���r�b�g�}�b�v����DDB(�f�o�C�X�ˑ�)�`���r�b�g�}�b�v���쐬����
	myDIB = ::CreateDIBitmap(hDC,&m_BmpInfo->bmiHeader,CBM_INIT,m_BmpImage,m_BmpInfo,DIB_RGB_COLORS);

	//hCompatiDC��myDIB�̎����I�u�W�F�N�g���g�p�o����悤�ɂ���
	//�I���W�i���r�b�g�}�b�v��oldDIB�ɕۑ����Ă�����ŕ���������
	oldDIB = (HBITMAP)::SelectObject(hCompatiDC,myDIB);

	//�r�b�g�}�b�v�L�k���[�h���Z�b�g����
	SetStretchBltMode(hDC, STRETCH_DELETESCANS);

	//hCompatiDC�ɂ���摜��hDC�ɕ\������
	//�摜��\������g��512x512
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
	//BeginPaint�Ή�����
	::EndPaint(*pMovieRangeWnd,&ps);

	//�I�u�W�F�N�̊J���Ȃ�
	::SelectObject(hCompatiDC, oldDIB);
	::DeleteObject(myDIB);
	::DeleteObject(hCompatiDC);
	::ReleaseDC(*pMovieRangeWnd,hDC);
}

//-----------------------------------------------------------------------------------------------
//	�摜�\���̈���N���A
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

	//�s�N�`���{�b�N�X��BtnFace�F�ŃN���A
	pMovieRangeWnd->GetClientRect(nRect);

	nLT = nRect.TopLeft();
	nRB = nRect.BottomRight();
	nRB.x = nRB.x+1;
	nRect.SetRect(nLT.x, nLT.y, nRB.x, nRB.y);

	dwCol = GetSysColor(COLOR_BTNFACE);		//ButtomnFace�F���擾 
	RangeCLDC.FillSolidRect(nRect,dwCol);	//ButtonFace�F�Ńs�N�`���{�b�N�X��h��Ԃ�

}

//-----------------------------------------------------------------------------------------------
//	OnQueryDragIcon();
//-----------------------------------------------------------------------------------------------
// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CFastCamMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//-----------------------------------------------------------------------------------------------
//	OnTimer(); 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	CString	msg;

	switch(nIDEvent){
	case TIMER_MODE_PLAY:
			if(!m_IsDraw)
			{
				if(m_nRecFrame <= 0){
					msg = "�Đ��ł���摜�����݂��܂���";
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

					m_disp_status1.SetWindowText("�Đ��I��");

					msg = "�Đ��I�����܂���";
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					return;
				}
				//�������摜�\��
				DspPlayCamera();
				m_PlayFrameCount++;
			}
			break;
	case TIMER_MODE_LIVE:
			if(!m_IsDraw)	DspLiveCamera();	//���C�u�摜�\��
			break;
	case TIMER_MODE_RECORD:			//�^��(������)�ւ̘^��I���`�F�b�N

			if(m_Execute_Record==TRUE){
				// �J�����̏�Ԃ��擾
				nRet = PDC_GetStatus(nDeviceNo,
									&nStatus,
									&nErrorCode);
				if(nRet == PDC_FAILED){
					msg = "��Ԃ̎擾�Ɏ��s���܂���";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
					EndWaitCursor();	// �����v�J�[�\�����������܂��B
					return;
				}
				//�^�撆�łȂ��ꍇ�A�^��I������
				if(nStatus != PDC_STATUS_REC && nStatus != PDC_STATUS_RECREADY){
				
					//�^��m�F�^�C�}�[�X�g�b�v
					//ID NO. = TIME_MODE_RECORD 
					::KillTimer(GetSafeHwnd(), TIMER_MODE_RECORD);

					//�ҋ@��Ԃ�����(���C�u���[�h)
					nRet = PDC_SetStatus(nDeviceNo,
										 PDC_STATUS_LIVE,
										 &nErrorCode);
					if (nRet == PDC_FAILED) {
						msg = "��Ԃ̐ݒ�Ɏ��s���܂���";
						AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
						return;
					}

					m_disp_status1.SetWindowText("�^��I��");

					msg = "�^��I�����܂���";
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					m_PlayFrameCount = 0;
	
					EndWaitCursor();	// �����v�J�[�\�����������܂��B		

					m_Execute_Record = FALSE;

					//�^��ۑ�,�Đ���L���ɂ���

					CButton* button1;
					button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
					button1->EnableWindow(TRUE);				
					button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_MODE_LIVE);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_MODE_PLAYBACK);
					button1->EnableWindow(TRUE);
					button1 = (CButton*)GetDlgItem(IDC_PROGRAM_EXIT);
					button1->EnableWindow(TRUE);

					//	���C�u�摜�\�� 
					OnModeLive();
				}else{
					BeginWaitCursor();	// �����v�J�[�\����\�����܂��B		
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
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	unsigned long IPList[PDC_MAX_DEVICE]; // �����Ώ�IP�A�h���X 
	unsigned long nCount;       // �ڑ�����Ă���q�f�o�C�X�� 
	unsigned long RateList[PDC_MAX_LIST_NUMBER];    // �B�e���x�ꗗ 
	PDC_DETECT_NUM_INFO DetectNumInfo;    // �������� 
	unsigned long i;
	char	cWk0[20];	
	CString ss;	
	CString	msg;

	m_IsDraw=FALSE;		//�摜�\���������łȂ�

	//------------------
	//�����l�Z�b�g
	//�摜�T�C�Y(Width)
	//�摜�T�C�Y(Height)
	//------------------		
	m_Width  = 512;
	m_Height = 512;

	m_Execute_Live	 = FALSE;	//���C�u���łȂ��Ƃ���
	m_Execute_Record = FALSE;	//�^�撆�łȂ��Ƃ���

	m_device_name.SetWindowText("Unknown Device");
	m_disp_status1.SetWindowText("���j�^��~");	
	//--------------------
	//�w��J�����̏���������
	//--------------------

	// �������E�I������ 
	nRet = PDC_Init(&nErrorCode);
	if (nRet == PDC_FAILED) {
			msg = "���C�u�������������ł��܂���ł���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}

	// �f�o�C�X�̌����i��������/Gigabit-Ether I/F�j
	IPList[0] = 0xC0A80000;//192.168.0.1�`192.0.254������
	nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,    // Gigabit-Ether I/F 
							IPList,                 // IP�A�h���X 
							5,                      // �ő匟���䐔 
							PDC_DETECT_AUTO,   //��������
							&DetectNumInfo,
							&nErrorCode);
#if 0
	//�f�o�C�X�̌����i�����Ώێw��/Gigabit-Ether I/F�j
	IPList[0] = 0xC0A8000A;//192.168.0.10
	IPList[1] = 0xC0A8000B;//192.168.0.11
	nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,    // Gigabit-Ether I/F 
							IPList,                 // IP�A�h���X 
							2,                      // �ő匟���䐔 
							PDC_DETECT_NORMAL,   //IP�A�h���X�𖾎��I�Ɏw��
							&DetectNumInfo,
							&nErrorCode);
	//�f�o�C�X�̌����i�����Ώێw��/PCI�j
	nRet = PDC_DetectDevice(PDC_INTTYPE_PCI,		// PCI 
							NULL,                  
							0,                      
							0,   
							&DetectNumInfo,
							&nErrorCode);
#endif
	if (nRet == PDC_FAILED) {
			msg = "�f�o�C�X�������ł��܂���ł���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}


	// �f�o�C�X�̃I�[�v�� 
	nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[0]), // �Ώۃf�o�C�X��� 
							&nDeviceNo,    // �f�o�C�X�i���o�[ 
							&nErrorCode);
	if(nRet == PDC_FAILED) {
			msg = "�f�o�C�X���I�[�v���ł��܂���ł���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}


	// �ڑ�����Ă���q�f�o�C�X�i���o�[���擾 
	nRet = PDC_GetExistChildDeviceList(nDeviceNo,
									   &nCount,
									   ChildNo,
									   &nErrorCode);
	if (nRet == PDC_FAILED) {
			msg = "�ڑ�����Ă���q�f�o�C�X�����擾�ł��܂���ł���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}
	// �q�f�o�C�X�i���o�[�����X�g�ɕ\�� 
	m_select_camera.ResetContent();
	for(i=0; i<nCount; i++){
			sprintf(cWk0,"%d",ChildNo[i]);
			m_select_camera.InsertString(-1,cWk0);
	}
	// ���X�g��1�Ԗڂ��̗p���� 
	m_select_camera.GetLBText(0,ss);
	m_select_camera.SetWindowText(ss);
	m_select_camera.SetCurSel(0);	

	// �q�f�o�C�X�i���o�[��ϐ��ɃZ�b�g
	nChildNo = atoi(ss);

	//�f�o�C�X�����擾
	char StrName[50];
	nRet = PDC_GetDeviceName(nDeviceNo,
							0,
							StrName,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "�f�o�C�X���̎擾���ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	// �f�o�C�X����\��
	m_device_name.SetWindowText(StrName);

	// �B�e���x�̃��X�g���擾 
	nRet = PDC_GetRecordRateList(nDeviceNo,
							nChildNo,
							&nCount,
							RateList,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "�B�e���x���X�g�̎擾���ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	// �B�e���x�̃��X�g��\�� 
	m_change_rate.ResetContent();
	for(i=0; i<nCount; i++){
			sprintf(cWk0,"%d",RateList[i]);
			m_change_rate.InsertString(-1,cWk0);
	}

	//���X�g��3�Ԗڂ��̗p���� 
	m_change_rate.GetLBText(3,ss);
	m_change_rate.SetWindowText(ss);
	m_change_rate.SetCurSel(3);	

	// ��荞��ڰĂ�ϐ��ɃZ�b�g
	m_nRecordRate = atoi(ss);

	//�@�B�e���x��ݒ�  
	nRet = PDC_SetRecordRate(nDeviceNo,
							nChildNo,
							m_nRecordRate,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "�t���[�����[�g��ݒ�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	
	//	�V���b�^�[�X�s�[�h�A�𑜓x�A�g���K���[�h��ݒ�
	UpdateParam(); 	

	//	���C�u���[�h�ɐݒ�
	OnModeLive();
	
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_MODE_LIVE);
	button1->EnableWindow(TRUE);
	button1->SetCheck(1);
	button1 = (CButton*)GetDlgItem(IDC_MODE_PLAYBACK);
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

	m_IsDraw=TRUE;		//�摜�\��������

	//�J���[�E���m�N�����̎擾
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�J���[�E���m�N�������擾�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//�J���[
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

	}else{					//���m�N��
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
			mp_BmpRgbQuad[i].rgbBlue	= i;	//�̓x����
			mp_BmpRgbQuad[i].rgbGreen	= i;	//�΂̓x����
			mp_BmpRgbQuad[i].rgbRed		= i;	//�Ԃ̓x����
			mp_BmpRgbQuad[i].rgbReserved	= 0;	//0 �ɂ���
		}

		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight];
	}
	// ���C�u�摜�̎擾
	nRet = PDC_GetLiveImageData(nDeviceNo,
                            nChildNo,
                            8,    /* 8�r�b�g */
                            m_BmpImage,
                            &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "���C�u�摜���擾�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		delete [] m_BmpImage;
		return;
	}
	//----------------------
	//	�摜�\���̈��`��
	//----------------------
	UpdateMovieRangeWindow(); 

	m_IsDraw=FALSE;		//�摜�\���������łȂ�
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
	
	m_IsDraw = TRUE;		//�摜�\��������

	//�J���[�E���m�N�����̎擾
	nRet = PDC_GetColorType(nDeviceNo,
                     nChildNo,
                     &nMode,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�J���[�E���m�N�������擾�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//�J���[
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

	}else{					//���m�N��
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

		RGBQUAD				*mp_BmpRgbQuad;	//RGBQUAD
		mp_BmpRgbQuad = m_BmpInfo->bmiColors;


		for(int i=0; i<256; i++ ){
			mp_BmpRgbQuad[i].rgbBlue	= i;	//�̓x����
			mp_BmpRgbQuad[i].rgbGreen	= i;	//�΂̓x����
			mp_BmpRgbQuad[i].rgbRed		= i;	//�Ԃ̓x����
			mp_BmpRgbQuad[i].rgbReserved	= 0;	//0 �ɂ���
		}
		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight];
	}
	
	//�^�悵���摜���擾
	nRet = PDC_GetMemImageData(nDeviceNo,			
								nChildNo,
							   m_PlayFrameCount,
							   8,
							   m_BmpImage,
							   &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�摜���擾�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		delete [] m_BmpImage;
		return;
	}
	//�Đ��J�E���^�[��\��
	char	cWk0[50];

	sprintf(cWk0,"FrameNo.[%d]",m_PlayFrameCount);
	m_disp_status2.SetWindowText(cWk0);

	//----------------------
	//	�摜�\���̈��`��
	//----------------------
	UpdateMovieRangeWindow(); 
	m_IsDraw = FALSE;	//�摜�\���������łȂ�
}

//-----------------------------------------------------------------------------------------------
//	��荞�݃��[�g�ύX
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnSelchangeChangeRate() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	char	ss[10];
	CString msg;

	//���C�u���H
	if(m_Execute_Live==FALSE){
		msg = "���C�u�摜�\���ɐ؂�ւ��Ă�����s���Ă�������";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	nSel = m_change_rate.GetCurSel();
	m_change_rate.GetLBText(nSel,ss);
	m_nRecordRate = atoi(ss);

	//��荞�݃��[�g��ݒ�
	nRet = PDC_SetRecordRate(nDeviceNo,
							nChildNo,
							m_nRecordRate,
							&nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "��荞�݃��[�g��ݒ�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	//-----------------------
	//	�摜�\���̈���N���A
	//-----------------------
	ClearMovieRangeWindow(); 
	
	//--------------------------------------------------
	//	�V���b�^�[�X�s�[�h�A�𑜓x�A�g���K���[�h��ݒ�
	//--------------------------------------------------
	UpdateParam();
}

//-----------------------------------------------------------------------------------------------
//	�V���b�^�[�X�s�[�h�ύX
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSelchangeChangeShutter() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	CString msg;

	//���C�u���H
	if(m_Execute_Live==FALSE){
		msg = "���C�u�摜�\���ɐ؂�ւ��Ă�����s���Ă�������";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B

	//�I�������V���b�^�[�X�s�[�h���擾
	nSel = m_change_shutter.GetCurSel();

	// �V���b�^�[�X�s�[�h��ϐ��ɃZ�b�g 
	m_nShutterSpeed = nShutterList[nSel];

	// �V���b�^�[�X�s�[�h�̐ݒ� 
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,
									nChildNo,
									m_nShutterSpeed,
									&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�V���b�^�[�X�s�[�h���ݒ�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}

	EndWaitCursor();	// �����v�J�[�\�����������܂��B
}
//-----------------------------------------------------------------------------------------------
//	�𑜓x�ύX
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSelchangeChangeResolution() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	CString msg;
	int		nWidth;
	int		nHeight;
	int		nInterval;
	char nMode;

	//���C�u���H
	if(m_Execute_Live==FALSE){
		msg = "���C�u�摜�\���ɐ؂�ւ��Ă�����s���Ă�������";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B
	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);


	//�I�������𑜓x���擾
	nSel = m_change_resolution.GetCurSel();
	m_dwResolution = nResolutionList[nSel];
	nWidth	= m_dwResolution & 0xffff0000;
	nWidth	= nWidth>>16;
	nHeight = m_dwResolution & 0x0000ffff;

	// �𑜓x��ݒ� 
	nRet = PDC_SetResolution(nDeviceNo,
							nChildNo,
							nWidth,
							nHeight,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�𑜓xނ��ݒ�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}
	m_Width  = nWidth;
	m_Height = nHeight;	

	//�J���[�E���m�N�����̎擾
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�J���[�E���m�N����񂪂ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//�J���[
	//�摜�T�C�Y�ɂ�胉�C�u�\���Ԋu��ύX����
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}else{				//���m�N��
		//�摜�T�C�Y�ɂ�胉�C�u�\���Ԋu��ύX����
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

	//���C�u�\���^�C�}�[�X�^�[�g
	//ID NO.  = TIMER_MODE_LIVE
	//�C���^�[�o�� = nInterval (ms) 
	::SetTimer(GetSafeHwnd(), TIMER_MODE_LIVE, nInterval, NULL);
	//	�摜�\���̈���N���A
	ClearMovieRangeWindow(); 
	
	EndWaitCursor();	// �����v�J�[�\�����������܂��B
}
//-----------------------------------------------------------------------------------------------
//	�q�f�o�C�X�ύX
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSelchangeSelectCamera() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int		nSel;
	char	ss[10];
	CString	msg;

	//���C�u���H
	if(m_Execute_Live==FALSE){
		msg = "���C�u�摜�\���ɐ؂�ւ��Ă�����s���Ă�������";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}
	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B

	//�I�������q�f�o�C�X���擾
	nSel = m_select_camera.GetCurSel();
	m_select_camera.GetLBText(nSel,ss);

	//�ϐ��ɃZ�b�g
	nChildNo = atoi(ss);

	m_IsDraw=FALSE;		//�摜�\���������łȂ�
	EndWaitCursor();	// �����v�J�[�\�����������܂��B
}

//-----------------------------------------------------------------------------------------------
//	���C�u���[�h 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnModeLive() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nInterval;
	CString	msg;
	char nMode;

	//���C�u���H
	if(m_Execute_Live==TRUE){
		return;
	}

	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);

	//���C�u���[�h�Ƃ���	

	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_LIVE,
						 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "���C�u���[�h�ɕύX�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	//	�摜�\���̈���N���A
	ClearMovieRangeWindow(); 

	//�ύX�\�ɂ���
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
	//�ύX�֎~�ɂ���
	CButton* button2;
	button2 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button2->EnableWindow(FALSE);
	button2 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button2->EnableWindow(FALSE);
	button2 = (CButton*)GetDlgItem(IDC_STOP_MEMORY);
	button2->EnableWindow(FALSE);

	//���C�u�摜�̉𑜓x���擾
	nRet = PDC_GetResolution(nDeviceNo,
                     nChildNo,
                     &m_Width,
					 &m_Height,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�𑜓x���擾�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	//----------------------------------------
	//�J�����̓��m�N�� or �J���[�����ʂ���
	//----------------------------------------
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�J���[�E���m�N���ݒ�̎擾���ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//�J���[
	//�摜�T�C�Y�ɂ�胉�C�u�\���Ԋu��ύX����
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}else{				//���m�N��
		//�摜�T�C�Y�ɂ�胉�C�u�\���Ԋu��ύX����
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

	//���C�uޕ\���^�C�}�[�X�^�[�g
	//ID NO.  = TIMER_MODE_LIVE
	//�C���^�[�o�� = nInterval (ms) 
	::SetTimer(GetSafeHwnd(), TIMER_MODE_LIVE, nInterval, NULL);
	//-----------------------
	//	�摜�\���̈���N���A
	//-----------------------
	ClearMovieRangeWindow(); 

	m_disp_status1.SetWindowText("���C�u�摜�\����");
	m_Execute_Live = TRUE;	//ײ�ޒ�

	UpdateWindow();
}

//-----------------------------------------------------------------------------------------------
//	���������[�h 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnModeMemory() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	//���C�u���H
	if(m_Execute_Live==FALSE){
		return;
	}
	//���C�u�\���^�C�}�[�X�g�b�v
	//ID NO. = TIMER_MODE_LIVE 
	::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

	//���������[�h�ɕύX
	unsigned long nRet;
	unsigned long nErrorCode;
	CString	msg;
	m_Execute_Live = FALSE;	//���C�u���ł͂Ȃ�

	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_PLAYBACK,
						 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "���������[�h�ɕύX�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	//�^��t���[�������擾
	PDC_FRAME_INFO info;
	nRet = PDC_GetMemFrameInfo(nDeviceNo,
								nChildNo,
								&info,
								&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "�t���[����񂪎擾�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}

	m_nRecFrame = info.m_nRecordedFrames;
	m_nStartFrame = info.m_nStart;
	m_nEndFrame = info.m_nEnd;
	
	//�t���[�����̕\��
	char	cWk0[10];
	sprintf(cWk0,"[%d]",m_nEndFrame);
	m_disp_recframe.SetWindowText(cWk0);

	//�Đ��t���[�����J�n�t���[���ɃZ�b�g
	m_PlayFrameCount = m_nStartFrame;

	//�ύX�֎~�ɂ���
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
	//�ύX�\�ɂ���
	CButton* button2;
	button2 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button2->EnableWindow(TRUE);
	button2 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button2->EnableWindow(TRUE);
	button2 = (CButton*)GetDlgItem(IDC_STOP_MEMORY);
	button2->EnableWindow(TRUE);

	m_disp_status1.SetWindowText("������");
	//-----------------------
	//	�摜�\���̈���N���A
	//-----------------------
	ClearMovieRangeWindow(); 

	//�^�悵���摜�̉𑜓x���擾
	nRet = PDC_GetMemResolution(nDeviceNo,
                     nChildNo,
                     &m_Width,
					 &m_Height,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "�𑜓x���擾�ł��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	DspPlayCamera();

	UpdateWindow();
}

//-----------------------------------------------------------------------------------------------
//	�^��
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnRecordMemory() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	CString msg;

	//���C�u���H
	if(m_Execute_Live==FALSE){
		msg = "���C�u�摜�\���ɐ؂�ւ��Ă�����s���Ă�������";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	m_Execute_Live = FALSE;	//���C�u���łȂ�

	m_disp_status1.SetWindowText("�^�揀����");

	//���C�u�\���^�C�}�[�X�g�b�v
	//ID NO. = TIMER_MODE_LIVE 
	::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);
	
	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B

	//�^��ҋ@��ݒ� 
	nRet =PDC_SetRecReady(nDeviceNo,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�^�揀���Ɏ��s���܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		OnModeLive();
		return;
	}
	// �^��ҋ@��Ԃ��`�F�b�N 
	while(1){
		nRet = PDC_GetStatus(nDeviceNo,
							&nStatus,
							&nErrorCode);
		if(nRet == PDC_FAILED){
			msg = "��Ԃ̎擾�Ɏ��s���܂���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
			EndWaitCursor();	// �����v�J�[�\�����������܂��B
			OnModeLive();
			return;
		}
		if ((nStatus == PDC_STATUS_RECREADY) ||
			(nStatus == PDC_STATUS_REC))
		{
			break;
		}
	}
	// �^��J�n 
	nRet = PDC_TriggerIn(nDeviceNo,
						&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�^��J�n�Ɏ��s���܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		OnModeLive();
		return;
	}
	// �^��J�n���m�F 
	int wait = GetTickCount();
	while(1){
		nRet = PDC_GetStatus(nDeviceNo,
							 &nStatus,
							 &nErrorCode);
		if (nRet == PDC_FAILED) {
			msg = "��Ԃ̏����Ɏ��s���܂���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			EndWaitCursor();
			OnModeLive();
			return;
		}
		if (nStatus == PDC_STATUS_REC)
		{
			break;
		}
		if((int)(GetTickCount()-wait)>3000){	//3000ms�ȓ��ɘ^��J�n�ł��Ȃ�����?
			msg = "�^��J�n���ł��܂���ł���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
			EndWaitCursor();	// �����v�J�[�\�����������܂��B
			OnModeLive();
			return;
		}
	}

	//�ύX�֎~�ɂ���
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
	button1 = (CButton*)GetDlgItem(IDC_MODE_LIVE);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_MODE_PLAYBACK);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_PROGRAM_EXIT);
	button1->EnableWindow(FALSE);

	UpdateWindow();

	//�^��I���m�F�^�C�}�[�X�^�[�g
	//ID NO.  = TIMER_MODE_RECORD
	//�C���^�[�o�� = 250 (ms) 
	::SetTimer(GetSafeHwnd(), TIMER_MODE_RECORD, 250, NULL);

	m_Execute_Record = TRUE;	//�^�撆
	m_disp_status1.SetWindowText("�^�撆");

	EndWaitCursor();	// �����v�J�[�\�����������܂��B
}

//-----------------------------------------------------------------------------------------------
//	�Đ�(������) 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnPlayMemory() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	CString msg;
	if(m_nRecFrame<=0){
		msg = "�Đ��ł���摜�����݂��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	//��Ԃ��擾����
	nRet = PDC_GetStatus(nDeviceNo,
						&nStatus,
						&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "��Ԃ̎擾�Ɏ��s���܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}
	//�Đ����[�h�ɐݒ肷��
	if(nStatus != PDC_STATUS_PLAYBACK){
		nRet = PDC_SetStatus(nDeviceNo,
							 PDC_STATUS_PLAYBACK,
							 &nErrorCode);
		if(nRet == PDC_FAILED){
			msg = "���s���܂���";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			EndWaitCursor();
			return;
		}
	}

	//���C�u�摜�\�����̏ꍇ�̓^�C�}�[�X�g�b�v
	if(m_Execute_Live==TRUE){	//���C�u��
		//���C�u�\���^�C�}�[�X�g�b�v
		//ID NO. = TIMER_MODE_LIVE 
		::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

		m_disp_status1.SetWindowText("���j�^��~");
		m_Execute_Live = FALSE;	//���C�u���łȂ�
	}
	
	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B

	m_disp_status1.SetWindowText("�Đ���");

	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button1->EnableWindow(FALSE);

	::SetTimer(GetSafeHwnd(), TIMER_MODE_PLAY, 250, NULL);

	EndWaitCursor();	// �����v�J�[�\�����������܂��B
}

//-----------------------------------------------------------------------------------------------
//	��~(������) 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnStopMemory() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString	msg;

	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);
	//�ύX�\�ɂ���
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button1->EnableWindow(TRUE);
	button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button1->EnableWindow(TRUE);

	m_disp_status1.SetWindowText("�Đ��I��");
}

//-----------------------------------------------------------------------------------------------
//	�^��ۑ�(HD)
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnRecordSaveHd() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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
		msg = "�ۑ��ł���摜�����݂��܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	
	//���������[�h�ɕύX
	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_PLAYBACK,
							 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "��Ԃ̐ݒ�Ɏ��s���܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		return;
	}
	//���C�u�摜�\�����̏ꍇ�̓^�C�}�[�X�g�b�v
	if(m_Execute_Live==TRUE){	//���C�u��
		//���C�u�\���^�C�}�[�X�g�b�v
		//ID NO. = TIMER_MODE_LIVE 
		::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

		m_disp_status1.SetWindowText("���j�^��~");
		m_Execute_Live = FALSE;	//���C�u���ł͂Ȃ�
	}

	//�ύX�֎~�ɂ���
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
	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B

	m_disp_status1.SetWindowText("�t�@�C���ۑ���");

	CFileDialog myDLG(FALSE,NULL,"FastCam",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�r�b�g�}�b�v(*.bmp)|*.bmp|�S��(*.*)|*.*||");
	if(myDLG.DoModal() == IDOK)
	{
		//�摜�ۑ��t�H���_���쐬
		::GetLocalTime(&tm);				//�N����,�����擾
		strcpy(cPath,myDLG.GetPathName());
		sprintf(cTime,"%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d\\",tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
		strcat(cPath,cTime);			//�^�C���X�^���v��ǉ�
		strcpy(cPathTemp,cPath);			//�t�H���_�̃f�B���N�g�����ꎞ�ۑ�
		CreateDirectory(cPath,NULL);		//�t�H���_�쐬
	
		for(i=m_nStartFrame; i<=m_nEndFrame; i++){
			nRet = PDC_GetMemImageData(nDeviceNo,
										nChildNo,
										i,
										8,
										m_BmpImage,
										&nErrorCode);
			//-----------------------------
			//	BMP�t�@�C�����쐬
			//-----------------------------
			strcpy(cPath,cPathTemp);
			sprintf(cWk0,"Temp%4.4d.bmp",i);
			strcat(cPath,cWk0);
			//�J���[�E���m�N�����̎擾
			nRet = PDC_GetColorType(nDeviceNo,
						 nChildNo,
						 &nMode,
						 &nErrorCode);
			if (nRet == PDC_FAILED) {
				msg = "�J���[�E���m�N�����̎擾�Ɏ��s���܂���";
				AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
				EndWaitCursor();
				return;
			}
			if(nMode == PDC_COLORTYPE_COLOR){		//�J���[
				nSts = WriteBmp24File(cPath, m_Width, m_Height);
				if(nSts<0){
					msg = "�t�@�C���ɕۑ��ł��܂���ł���";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
					EndWaitCursor();	// �����v�J�[�\�����������܂��B
					return;
				}
			}else{					//��ɃN��
				nSts = WriteBmpFile(cPath, m_Width, m_Height);
				if(nSts<0){
					msg = "�t�@�C���ɕۑ��ł��܂���ł���";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
					EndWaitCursor();	// �����v�J�[�\�����������܂��B
					return;
				}
			}
			//�^�文����\��
			sprintf(cWk0,"FrameNo.[%d]",i);
			m_disp_status2.SetWindowText(cWk0);
		}

		m_disp_status1.SetWindowText("�t�@�C���ۑ��I��");

		msg = "�t�@�C���ɕۑ����܂���";
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
	}

	EndWaitCursor();	// �����v�J�[�\���������܂��B
}

//-----------------------------------------------------------------------------------------------
//	�o�[�W������� 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnVersionInf() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CAboutDlg DlgAbout;
	DlgAbout.DoModal();	
}

//-----------------------------------------------------------------------------------------------
//	�v���O�����I�� 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnProgramExit() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	CString msg;

	msg = "�v���O�������I�����܂�";
	nRet = AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION);
	if(nRet==IDYES){
		CDialog::DestroyWindow();		//�v���O�����I��	
	}
}

//--------------------------------------------------------------------------------------------
//	�V���b�^�[�X�s�[�h�A�𑜓x�A�g���K���[�h��ݒ�
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

	BeginWaitCursor();	// �����v�J�[�\����\�����܂��B
	//-----------------------------------------------------
	//	�e��̃p�����[�^��ݒ� 
	//-----------------------------------------------------
	// �V���b�^�[�X�s�[�h���X�g���擾 
	nRet = PDC_GetShutterSpeedFpsList(nDeviceNo,
										nChildNo,
										&nSize,
										nShutterList,
										&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�V���b�^�[�X�s�[�h���X�g���擾�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}
	// �V���b�^�[���x�̃��X�g��\�� 
	m_change_shutter.ResetContent();
	for(i=0; i<nSize; i++){
			sprintf(cWk0,"1/%d",nShutterList[i]);
			m_change_shutter.InsertString(-1,cWk0);
	}

	// ���X�g��0�Ԗڂ��̗p���� 
	m_change_shutter.SetCurSel(0);	


	//�V���b�^�[�X�s�[�h��ϐ��ɃZ�b�g
	//int		m_nShutterSpeed;	//�V���b�^�[�X�s�[�h
	m_nShutterSpeed = nShutterList[0];


	//�V���b�^�[�X�s�[�h�̐ݒ� 
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,
									nChildNo,
									m_nShutterSpeed,
									&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�V���b�^�[�X�s�[�h���ݒ�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}

	// �𑜓x�̃��X�g���擾 
	nRet = PDC_GetResolutionList(nDeviceNo,
										nChildNo,
										&nSize,
										nResolutionList,
										&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�𑜓x�̃��X�g���擾�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}	

	// �𑜓x�̃��X�g��\�� 
	m_change_resolution.ResetContent();
	for(i=0; i<nSize; i++){
		nWidth	= nResolutionList[i] & 0xffff0000;
		nWidth	= nWidth>>16;
		nHeight = nResolutionList[i] & 0x0000ffff;
		sprintf(cWk0,"%dx%d",nWidth,nHeight);
		m_change_resolution.InsertString(-1,cWk0);
	}

	//���X�g��0�Ԗڂ��̗p���� 
	m_change_resolution.SetCurSel(0);	

	//�𑜓x��ϐ��ɾ�� 
	m_dwResolution = nResolutionList[0];
		
	// �𑜓x��ݒ� 
	nWidth	= m_dwResolution & 0xffff0000;
	nWidth	= nWidth>>16;
	nHeight = m_dwResolution & 0x0000ffff;

	nRet = PDC_SetResolution(nDeviceNo,
							nChildNo,
							nWidth,
							nHeight,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�𑜓x���ݒ�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}
	// �𑜓x��ݒ� 
	m_Width  = nWidth;
	m_Height = nHeight;

	
	// �g���K���[�h��ݒ� 	
	nRet = PDC_SetTriggerMode(nDeviceNo,
								PDC_TRIGGER_START,
								0,
								0,
								0,
								&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "�g���K���[�h���ݒ�ł��܂���ł���";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// �����v�J�[�\�����������܂��B
		return;
	}
	
	//�g���K���[�h����ʂɕ\��
	m_disp_trigmode.SetWindowText("START");


	EndWaitCursor();	// �����v�J�[�\�����������܂��B
}

//-----------------------------------------------------------------------------------------------
//	CFastCamMonitorDlg �j��
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	unsigned long nRet;
	unsigned long nErrorCode;
	nRet = PDC_CloseDevice(nDeviceNo, &nErrorCode);

	// �g�p���Ă���ΊJ������ 
	if(m_BmpInfo)	
		delete[] m_BmpInfo;
	if(m_BmpImage)	
		delete[] m_BmpImage;
}

//-----------------------------------------------------------------------------------------------
//	BMP�t�@�C������������	8�r�b�g���m�N��
//-----------------------------------------------------------------------------------------------
int	CFastCamMonitorDlg::WriteBmpFile(char *szFName,int nWidth, int nHeight)
{
	int		i;
	int		x,y;
	int		nCnt;
	int		nPos;
	CFile	file;

	//�t�@�C�����I�[�v������
	if(!file.Open(szFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		return(-1);
	}

	//----------------------------------------------------------------------------
	// BMP�w�b�_�� 	
	//----------------------------------------------------------------------------
	//WORD  : BmpFileHeader.bfType;				BM �Ƃ�������
	//DWORD : BmpFileHeader.bfSize;				�t�@�C���̑S�̃T�C�Y
	//WORD  : BmpFileHeader.bfReserved1;		0 �ɂ���
	//WORD  : BmpFileHeader.bfReserved2;		0 �ɂ���	
	//DWORD : BmpFileHeader.bfOffBits;			�r�b�g�}�b�v�r�b�g�̃t�@�C���擪����̃I�t�Z�b�g
	m_gnSizeFileHdr = sizeof( BITMAPFILEHEADER );	//BMP�w�b�_���T�C�Y(14�o�C�g)
	m_gnSizeInfoHdr = sizeof( BITMAPINFOHEADER );	//BMP��񕔃T�C�Y(40�o�C�g)
	m_gnSizeRgbQuad = sizeof( RGBQUAD )*256;		//�p���b�g�T�C�Y(1024�o�C�g)
	m_gnSizeTop	 = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad;
												//BM �Ƃ�������
	memcpy( (char *)&m_BmpFileHdr.bfType, "BM", 2 );

	m_gnSizeBmpImage = nWidth * nHeight;		//�r�b�g�}�b�v�̉���(Width) x ����(Height)
												//�t�@�C���̑S�̃T�C�Y
	m_BmpFileHdr.bfSize = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad + m_gnSizeBmpImage;
	m_BmpFileHdr.bfReserved1 = 0;				//0 �ɂ���
	m_BmpFileHdr.bfReserved2 = 0;				//0 �ɂ���
	m_BmpFileHdr.bfOffBits = m_gnSizeTop;		//�r�b�g�}�b�v�r�b�g�̃t�@�C���擪����̃I�t�Z�b�g

	file.Write(&m_BmpFileHdr, sizeof(BITMAPFILEHEADER));

	//----------------------------------------------------------------------------
	//	BMP���
	//----------------------------------------------------------------------------
	//DWORD : BmpInfoHeader.biSize;				BITMAPINFOHEADER�^�\���̂̃T�C�Y
	//											(�P�ʂ̓o�C�g)
	//LONG  : BmpInfoHeader.biWidth;			�r�b�g�}�b�v�̕�  (�P�ʂ̓s�N�Z��)
	//LONG  : BmpInfoHeader.biHeight;			�r�b�g�}�b�v�̍���(�P�ʂ̓s�N�Z��)
	//WORD  : BmpInfoHeader.biPlanes;			1 �ɂ���
	//WORD  : BmpInfoHeader.biBitCount;			�s�N�Z��������̃J���[�r�b�g��(1,4,8,24)
	//DWORD : BmpInfoHeader.biCompression;		���k����(���k���Ȃ����� 0)
	//DWORD : BmpInfoHeader.biSizeImage;		�r�b�g�}�b�v�r�b�g�̃T�C�Y	
	//											(�P�ʂ̓o�C�g) ���k����Ƃ��̂ݕK�v
	//LONG  : BmpInfoHeader.biXPelsPerMeter;	�����𑜓x(1���[�g��������̃s�N�Z����)
	//LONG  : BmpInfoHeader.biYPelsPerMeter;	�����𑜓x(1���[�g��������̃s�N�Z����)
	//DWORD : BmpInfoHeader.biClrUsed;			�C���[�W�Ŏg���Ă���F�̐�
	//DWORD : BmpInfoHeader.biClrImportant;		�C���[�W�Ŏg���Ă���d�v�ȐF�̐�

	m_BmpInfoHdr.biSize			= 40L;			//BITMAPINFOHEADER�^�\���̂̃T�C�Y
	m_BmpInfoHdr.biWidth		= nWidth;		//�r�b�g�}�b�v�̕�  (�P�ʂ̓s�N�Z��)
	m_BmpInfoHdr.biHeight		= nHeight;		//�r�b�g�}�b�v�̍���(�P�ʂ̓s�N�Z��)
	m_BmpInfoHdr.biPlanes		= 1;			//1 �ɂ���
	m_BmpInfoHdr.biBitCount 	= 8;			//�s�N�Z��������̃J���[�r�b�g��(1,4,8,24)
	m_BmpInfoHdr.biCompression	= 0;			//���k����(���k���Ȃ����� 0)
	m_BmpInfoHdr.biSizeImage = m_gnSizeBmpImage;//�r�b�g�}�b�v�r�b�g�̃T�C�Y
												//���k����Ƃ��̂ݕK�v
	m_BmpInfoHdr.biXPelsPerMeter	= 19344;	//�����𑜓x(1���[�g��������̃s�N�Z����)
	m_BmpInfoHdr.biYPelsPerMeter	= 19344;	//�����𑜓x(1���[�g��������̃s�N�Z����)
	m_BmpInfoHdr.biClrUsed			= 256;		//�C���[�W�Ŏg���Ă���F�̐�
	m_BmpInfoHdr.biClrImportant	=   0;			//�C���[�W�Ŏg���Ă���d�v�ȐF�̐�

	file.Write(&m_BmpInfoHdr,sizeof(BITMAPINFOHEADER));
	
	//-----------------
	//	�p���b�g����������
	//-----------------
	//BYTE  :	BmpRgbQuad.rgbBlue;		�̓x����
	//BYTE  :	BmpRgbQuad.rgbGreen;	�΂̓x����
	//BYTE  :	BmpRgbQuad.rgbRed;		�Ԃ̓x����
	//BYTE  :	BmpRgbQuad.rgbReserved;	0 �ɂ���
	for( i=0; i<256; i++ ){
		m_BmpRgbQuad[i].rgbBlue		= i;	//�̓x����
		m_BmpRgbQuad[i].rgbGreen	= i;	//�΂̓x����
		m_BmpRgbQuad[i].rgbRed		= i;	//�Ԃ̓x����
		m_BmpRgbQuad[i].rgbReserved	= 0;	//0 �ɂ���
	}

	nCnt = sizeof(RGBQUAD)*256;
	file.Write(&m_BmpRgbQuad[0], nCnt);

	//--------------------------
	//	�r�b�g�}�b�v�C���[�W����������
	//--------------------------
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
	//�t�@�C�����N���[�Y����
	file.Close();

	return(0);
}

//-----------------------------------------------------------------------------------------------
//	BMP�t�@�C������������	24�r�b�g�t���J���[
//-----------------------------------------------------------------------------------------------
int	CFastCamMonitorDlg::WriteBmp24File(char *szFName,int nWidth, int nHeight)
{
	int		x,y;
	int		nPos0;
	int		nPos1;
	CFile	file;

	//�t�@�C�����I�[�v������
	if(!file.Open(szFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		return(-1);
	}

	//----------------------------------------------------------------------------
	// BMP�w�b�_�� 	
	//----------------------------------------------------------------------------
	//WORD  : BmpFileHeader.bfType;				BM �Ƃ�������
	//DWORD : BmpFileHeader.bfSize;				�t�@�C���̑S�̃T�C�Y
	//WORD  : BmpFileHeader.bfReserved1;		0 �ɂ���
	//WORD  : BmpFileHeader.bfReserved2;		0 �ɂ���	
	//DWORD : BmpFileHeader.bfOffBits;			�r�b�g�}�b�v�r�b�g�̃t�@�C���擪����̃I�t�Z�b�g
	m_gnSizeFileHdr = sizeof( BITMAPFILEHEADER );	//BMP�w�b�_���T�C�Y(14�o�C�g)
	m_gnSizeInfoHdr = sizeof( BITMAPINFOHEADER );	//BMP��񕔃T�C�Y(40�o�C�g)
	m_gnSizeRgbQuad = 0;							//�p���b�g�T�C�Y(0�o�C�g)
	m_gnSizeTop	 = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad;
												//BM �Ƃ�������
	memcpy( (char *)&m_BmpFileHdr.bfType, "BM", 2 );

	m_gnSizeBmpImage = nWidth * nHeight * 3;	//�r�b�g�}�b�v�r�b�g�̉���(Width) x ����(Height)
												//�t�@�Cr�̑S�̃T�C�Y
	m_BmpFileHdr.bfSize = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad + m_gnSizeBmpImage;
	m_BmpFileHdr.bfReserved1 = 0;				//0 �ɂ���
	m_BmpFileHdr.bfReserved2 = 0;				//0 �ɂ���
	m_BmpFileHdr.bfOffBits = m_gnSizeTop;		//�r�b�g�}�b�v�r�b�g�̃t�@�C���擪����̃I�t�Z�b�g

	file.Write(&m_BmpFileHdr, sizeof(BITMAPFILEHEADER));

	//----------------------------------------------------------------------------
	//	BMP���
	//----------------------------------------------------------------------------
	//DWORD : BmpInfoHeader.biSize;				BITMAPINFOHEADER�^�\���̂̃T�C�Y
	//											(�P�ʂ̓o�C�g)
	//LONG  : BmpInfoHeader.biWidth;			�r�b�g�}�b�v�̕�  (�P�ʂ̓s�N�Z��)
	//LONG  : BmpInfoHeader.biHeight;			�r�b�g�}�b�v�̍���(�P�ʂ̓s�N�Z��)
	//WORD  : BmpInfoHeader.biPlanes;			1 �ɂ���
	//WORD  : BmpInfoHeader.biBitCount;			�s�N�Z��������̃J���[�r�b�g��(1,4,8,24)
	//DWORD : BmpInfoHeader.biCompression;		���k����(���k���Ȃ����� 0)
	//DWORD : BmpInfoHeader.biSizeImage;		�r�b�g�}�b�v�r�b�g�̃T�C�Y	
	//											(�P�ʂ̓o�C�g) ���k����Ƃ��̂ݕK�v
	//LONG  : BmpInfoHeader.biXPelsPerMeter;	�����𑜓x(1���[�g��������̃s�N�Z����)
	//LONG  : BmpInfoHeader.biYPelsPerMeter;	�����𑜓x(1���[�g��������̃s�N�Z����)
	//DWORD : BmpInfoHeader.biClrUsed;			�C���[�W�Ŏg���Ă���F�̐�
	//DWORD : BmpInfoHeader.biClrImportant;		�C���[�W�Ŏg���Ă���d�v�ȐF�̐�

	m_BmpInfoHdr.biSize			= 40L;			//BITMAPINFOHEADER�^�\���̂̃T�C�Y
	m_BmpInfoHdr.biWidth		= nWidth;		//�r�b�g�}�b�v�̕�  (�P�ʂ̓s�N�Z��)
	m_BmpInfoHdr.biHeight		= nHeight;		//�r�b�g�}�b�v�̍���(�P�ʂ̓s�N�Z��)
	m_BmpInfoHdr.biPlanes		= 1;			//1 �ɂ���
	m_BmpInfoHdr.biBitCount 	= 24;			//�s�N�Z��������̃J���[�r�b�g��(1,4,8,24)
	m_BmpInfoHdr.biCompression	= 0;			//���k����(���k���Ȃ����� 0)
	m_BmpInfoHdr.biSizeImage = m_gnSizeBmpImage;//�r�b�g�}�b�v�r�b�g�̃T�C�Y
												//���k����Ƃ��̂ݕK�v
	m_BmpInfoHdr.biXPelsPerMeter	= 0;		//�����𑜓x(1���[�g��������̃s�N�Z����)
	m_BmpInfoHdr.biYPelsPerMeter	= 0;		//�����𑜓x(1���[�g��������̃s�N�Z����)
	m_BmpInfoHdr.biClrUsed			= 0;		//�C���[�W�Ŏg���Ă���F�̐�
	m_BmpInfoHdr.biClrImportant		= 0;		//�C���[�W�Ŏg���Ă���d�v�ȐF�̐�

	file.Write(&m_BmpInfoHdr,sizeof(BITMAPINFOHEADER));

	//-----------------
	//	�p���b�g��������
	//-----------------
	//�J���[�̈׃p���b�g�͂Ȃ�

	//--------------------------
	//	�r�b�g�}�b�v�C���[�W����������
	//--------------------------
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

	//�t�@�C�����N���[�Y����
	file.Close();

	return(0);
}

