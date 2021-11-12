// FastCamMonitorDlg.cpp : インプリメンテーション ファイル
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
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
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
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorDlg ダイアログ

CFastCamMonitorDlg::CFastCamMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastCamMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastCamMonitorDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
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
// CFastCamMonitorDlg メッセージ ハンドラ

//-----------------------------------------------------------------------------------------------
//	OnInitDialog()
//-----------------------------------------------------------------------------------------------
BOOL CFastCamMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
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

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

//-----------------------------------------------------------------------------------------------
//	OnPaint();
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//	画像表示領域を描画
		UpdateMovieRangeWindow();
		CDialog::OnPaint();
	}
}
//-----------------------------------------------------------------------------------------------
//	画像表示領域を描画
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::UpdateMovieRangeWindow() 
{
	int			nCopyWidth;
	int			nCopyHeight;
	HBITMAP		myDIB;			//ビットマップのハンドル
	HBITMAP		oldDIB;
	HDC			hDC;			//デバイスコンテキストのハンドル
	HDC			hCompatiDC;		//メモリデバイスコンテキスト
	PAINTSTRUCT ps;				//クライアント領域の描画に使う情報を保持
	CString		msg;
	CRect		nRect;
	int			nCompressedWidth;
	int			nCompressedHeight;

	//画像の準備が出来ていなければリターン
	if(!m_BmpInfo)	return;

	CWnd* pMovieRangeWnd = GetDlgItem(IDC_MOVIE_RANGE);

	//m_hWndで示す現在のウインドウのデバイスコンテキストを取得
	//このデバイスコンテキストは描画終了後にReleaseDCで開放する必要がある
	hDC = ::GetDC(*pMovieRangeWnd);

	//描画の準備をする
	//描画が終了したら必ずEndPaintを実行する必要がある
	::BeginPaint(*pMovieRangeWnd, &ps);

	//hDCと互換性のあるメモリデバイスコンテキストを作成する
	hCompatiDC = ::CreateCompatibleDC(hDC);

	//DIB形式ビットマップからDDB(デバイス依存)形式ビットマップを作成する
	myDIB = ::CreateDIBitmap(hDC,&m_BmpInfo->bmiHeader,CBM_INIT,m_BmpImage,m_BmpInfo,DIB_RGB_COLORS);

	//hCompatiDCでmyDIBの示すオブジェクトを使用出来るようにする
	//オリジナルビットマップをoldDIBに保存しておき後で復活させる
	oldDIB = (HBITMAP)::SelectObject(hCompatiDC,myDIB);

	//ビットマップ伸縮モードをセットする
	SetStretchBltMode(hDC, STRETCH_DELETESCANS);

	//hCompatiDCにある画像をhDCに表示する
	//画像を表示する枠は512x512
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
	//BeginPaint対応処理
	::EndPaint(*pMovieRangeWnd,&ps);

	//オブジェクの開放など
	::SelectObject(hCompatiDC, oldDIB);
	::DeleteObject(myDIB);
	::DeleteObject(hCompatiDC);
	::ReleaseDC(*pMovieRangeWnd,hDC);
}

//-----------------------------------------------------------------------------------------------
//	画像表示領域をクリア
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

	//ピクチャボックスをBtnFace色でクリア
	pMovieRangeWnd->GetClientRect(nRect);

	nLT = nRect.TopLeft();
	nRB = nRect.BottomRight();
	nRB.x = nRB.x+1;
	nRect.SetRect(nLT.x, nLT.y, nRB.x, nRB.y);

	dwCol = GetSysColor(COLOR_BTNFACE);		//ButtomnFace色を取得 
	RangeCLDC.FillSolidRect(nRect,dwCol);	//ButtonFace色でピクチャボックスを塗りつぶす

}

//-----------------------------------------------------------------------------------------------
//	OnQueryDragIcon();
//-----------------------------------------------------------------------------------------------
// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CFastCamMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//-----------------------------------------------------------------------------------------------
//	OnTimer(); 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	unsigned long nRet;
	unsigned long nErrorCode;
	CString	msg;

	switch(nIDEvent){
	case TIMER_MODE_PLAY:
			if(!m_IsDraw)
			{
				if(m_nRecFrame <= 0){
					msg = "再生できる画像が存在しません";
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

					m_disp_status1.SetWindowText("再生終了");

					msg = "再生終了しました";
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					return;
				}
				//メモリ画像表示
				DspPlayCamera();
				m_PlayFrameCount++;
			}
			break;
	case TIMER_MODE_LIVE:
			if(!m_IsDraw)	DspLiveCamera();	//ライブ画像表示
			break;
	case TIMER_MODE_RECORD:			//録画(メモリ)への録画終了チェック

			if(m_Execute_Record==TRUE){
				// カメラの状態を取得
				nRet = PDC_GetStatus(nDeviceNo,
									&nStatus,
									&nErrorCode);
				if(nRet == PDC_FAILED){
					msg = "状態の取得に失敗しました";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
					EndWaitCursor();	// 砂時計カーソルを消去します。
					return;
				}
				//録画中でない場合、録画終了処理
				if(nStatus != PDC_STATUS_REC && nStatus != PDC_STATUS_RECREADY){
				
					//録画確認タイマーストップ
					//ID NO. = TIME_MODE_RECORD 
					::KillTimer(GetSafeHwnd(), TIMER_MODE_RECORD);

					//待機状態を解除(ライブモード)
					nRet = PDC_SetStatus(nDeviceNo,
										 PDC_STATUS_LIVE,
										 &nErrorCode);
					if (nRet == PDC_FAILED) {
						msg = "状態の設定に失敗しました";
						AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
						return;
					}

					m_disp_status1.SetWindowText("録画終了");

					msg = "録画終了しました";
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					m_PlayFrameCount = 0;
	
					EndWaitCursor();	// 砂時計カーソルを消去します。		

					m_Execute_Record = FALSE;

					//録画保存,再生を有効にする

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

					//	ライブ画像表示 
					OnModeLive();
				}else{
					BeginWaitCursor();	// 砂時計カーソルを表示します。		
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
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	unsigned long IPList[PDC_MAX_DEVICE]; // 検索対象IPアドレス 
	unsigned long nCount;       // 接続されている子デバイス数 
	unsigned long RateList[PDC_MAX_LIST_NUMBER];    // 撮影速度一覧 
	PDC_DETECT_NUM_INFO DetectNumInfo;    // 検索結果 
	unsigned long i;
	char	cWk0[20];	
	CString ss;	
	CString	msg;

	m_IsDraw=FALSE;		//画像表示処理中でない

	//------------------
	//初期値セット
	//画像サイズ(Width)
	//画像サイズ(Height)
	//------------------		
	m_Width  = 512;
	m_Height = 512;

	m_Execute_Live	 = FALSE;	//ライブ中でないとする
	m_Execute_Record = FALSE;	//録画中でないとする

	m_device_name.SetWindowText("Unknown Device");
	m_disp_status1.SetWindowText("モニタ停止");	
	//--------------------
	//指定カメラの初期化処理
	//--------------------

	// 初期化・終了処理 
	nRet = PDC_Init(&nErrorCode);
	if (nRet == PDC_FAILED) {
			msg = "ライブラリを初期化できませんでした";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}

	// デバイスの検索（自動検索/Gigabit-Ether I/F）
	IPList[0] = 0xC0A80000;//192.168.0.1～192.0.254を検索
	nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,    // Gigabit-Ether I/F 
							IPList,                 // IPアドレス 
							5,                      // 最大検索台数 
							PDC_DETECT_AUTO,   //自動検索
							&DetectNumInfo,
							&nErrorCode);
#if 0
	//デバイスの検索（検索対象指定/Gigabit-Ether I/F）
	IPList[0] = 0xC0A8000A;//192.168.0.10
	IPList[1] = 0xC0A8000B;//192.168.0.11
	nRet = PDC_DetectDevice(PDC_INTTYPE_G_ETHER,    // Gigabit-Ether I/F 
							IPList,                 // IPアドレス 
							2,                      // 最大検索台数 
							PDC_DETECT_NORMAL,   //IPアドレスを明示的に指定
							&DetectNumInfo,
							&nErrorCode);
	//デバイスの検索（検索対象指定/PCI）
	nRet = PDC_DetectDevice(PDC_INTTYPE_PCI,		// PCI 
							NULL,                  
							0,                      
							0,   
							&DetectNumInfo,
							&nErrorCode);
#endif
	if (nRet == PDC_FAILED) {
			msg = "デバイスを検索できませんでした";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}


	// デバイスのオープン 
	nRet = PDC_OpenDevice(&(DetectNumInfo.m_DetectInfo[0]), // 対象デバイス情報 
							&nDeviceNo,    // デバイスナンバー 
							&nErrorCode);
	if(nRet == PDC_FAILED) {
			msg = "デバイスをオープンできませんでした";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}


	// 接続されている子デバイスナンバーを取得 
	nRet = PDC_GetExistChildDeviceList(nDeviceNo,
									   &nCount,
									   ChildNo,
									   &nErrorCode);
	if (nRet == PDC_FAILED) {
			msg = "接続されている子デバイス数を取得できませんでした";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			return;
	}
	// 子デバイスナンバーをリストに表示 
	m_select_camera.ResetContent();
	for(i=0; i<nCount; i++){
			sprintf(cWk0,"%d",ChildNo[i]);
			m_select_camera.InsertString(-1,cWk0);
	}
	// リストの1番目を採用する 
	m_select_camera.GetLBText(0,ss);
	m_select_camera.SetWindowText(ss);
	m_select_camera.SetCurSel(0);	

	// 子デバイスナンバーを変数にセット
	nChildNo = atoi(ss);

	//デバイス名を取得
	char StrName[50];
	nRet = PDC_GetDeviceName(nDeviceNo,
							0,
							StrName,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "デバイス名の取得ができませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	// デバイス名を表示
	m_device_name.SetWindowText(StrName);

	// 撮影速度のリストを取得 
	nRet = PDC_GetRecordRateList(nDeviceNo,
							nChildNo,
							&nCount,
							RateList,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "撮影速度リストの取得ができませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	// 撮影速度のリストを表示 
	m_change_rate.ResetContent();
	for(i=0; i<nCount; i++){
			sprintf(cWk0,"%d",RateList[i]);
			m_change_rate.InsertString(-1,cWk0);
	}

	//リストの3番目を採用する 
	m_change_rate.GetLBText(3,ss);
	m_change_rate.SetWindowText(ss);
	m_change_rate.SetCurSel(3);	

	// 取り込みﾚｰﾄを変数にセット
	m_nRecordRate = atoi(ss);

	//　撮影速度を設定  
	nRet = PDC_SetRecordRate(nDeviceNo,
							nChildNo,
							m_nRecordRate,
							&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "フレームレートを設定できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	
	//	シャッタースピード、解像度、トリガモードを設定
	UpdateParam(); 	

	//	ライブモードに設定
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

	m_IsDraw=TRUE;		//画像表示処理中

	//カラー・モノクロ情報の取得
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "カラー・モノクロ情報を取得できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//カラー
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

	}else{					//モノクロ
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
			mp_BmpRgbQuad[i].rgbBlue	= i;	//青の度合い
			mp_BmpRgbQuad[i].rgbGreen	= i;	//緑の度合い
			mp_BmpRgbQuad[i].rgbRed		= i;	//赤の度合い
			mp_BmpRgbQuad[i].rgbReserved	= 0;	//0 にする
		}

		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight];
	}
	// ライブ画像の取得
	nRet = PDC_GetLiveImageData(nDeviceNo,
                            nChildNo,
                            8,    /* 8ビット */
                            m_BmpImage,
                            &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "ライブ画像を取得できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		delete [] m_BmpImage;
		return;
	}
	//----------------------
	//	画像表示領域を描画
	//----------------------
	UpdateMovieRangeWindow(); 

	m_IsDraw=FALSE;		//画像表示処理中でない
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
	
	m_IsDraw = TRUE;		//画像表示処理中

	//カラー・モノクロ情報の取得
	nRet = PDC_GetColorType(nDeviceNo,
                     nChildNo,
                     &nMode,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "カラー・モノクロ情報を取得できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//カラー
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

	}else{					//モノクロ
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
			mp_BmpRgbQuad[i].rgbBlue	= i;	//青の度合い
			mp_BmpRgbQuad[i].rgbGreen	= i;	//緑の度合い
			mp_BmpRgbQuad[i].rgbRed		= i;	//赤の度合い
			mp_BmpRgbQuad[i].rgbReserved	= 0;	//0 にする
		}
		if(m_BmpImage)	
			delete [] m_BmpImage;
		m_BmpImage = new char[m_BmpInfo->bmiHeader.biWidth*m_BmpInfo->bmiHeader.biHeight];
	}
	
	//録画した画像を取得
	nRet = PDC_GetMemImageData(nDeviceNo,			
								nChildNo,
							   m_PlayFrameCount,
							   8,
							   m_BmpImage,
							   &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "画像を取得できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		delete [] m_BmpImage;
		return;
	}
	//再生カウンターを表示
	char	cWk0[50];

	sprintf(cWk0,"FrameNo.[%d]",m_PlayFrameCount);
	m_disp_status2.SetWindowText(cWk0);

	//----------------------
	//	画像表示領域を描画
	//----------------------
	UpdateMovieRangeWindow(); 
	m_IsDraw = FALSE;	//画像表示処理中でない
}

//-----------------------------------------------------------------------------------------------
//	取り込みレート変更
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnSelchangeChangeRate() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	char	ss[10];
	CString msg;

	//ライブ中？
	if(m_Execute_Live==FALSE){
		msg = "ライブ画像表示に切り替えてから実行してください";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	nSel = m_change_rate.GetCurSel();
	m_change_rate.GetLBText(nSel,ss);
	m_nRecordRate = atoi(ss);

	//取り込みレートを設定
	nRet = PDC_SetRecordRate(nDeviceNo,
							nChildNo,
							m_nRecordRate,
							&nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "取り込みレートを設定できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	//-----------------------
	//	画像表示領域をクリア
	//-----------------------
	ClearMovieRangeWindow(); 
	
	//--------------------------------------------------
	//	シャッタースピード、解像度、トリガモードを設定
	//--------------------------------------------------
	UpdateParam();
}

//-----------------------------------------------------------------------------------------------
//	シャッタースピード変更
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSelchangeChangeShutter() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	CString msg;

	//ライブ中？
	if(m_Execute_Live==FALSE){
		msg = "ライブ画像表示に切り替えてから実行してください";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	BeginWaitCursor();	// 砂時計カーソルを表示します。

	//選択したシャッタースピードを取得
	nSel = m_change_shutter.GetCurSel();

	// シャッタースピードを変数にセット 
	m_nShutterSpeed = nShutterList[nSel];

	// シャッタースピードの設定 
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,
									nChildNo,
									m_nShutterSpeed,
									&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "シャッタースピードが設定できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}

	EndWaitCursor();	// 砂時計カーソルを消去します。
}
//-----------------------------------------------------------------------------------------------
//	解像度変更
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSelchangeChangeResolution() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nSel;
	CString msg;
	int		nWidth;
	int		nHeight;
	int		nInterval;
	char nMode;

	//ライブ中？
	if(m_Execute_Live==FALSE){
		msg = "ライブ画像表示に切り替えてから実行してください";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	BeginWaitCursor();	// 砂時計カーソルを表示します。
	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);


	//選択した解像度を取得
	nSel = m_change_resolution.GetCurSel();
	m_dwResolution = nResolutionList[nSel];
	nWidth	= m_dwResolution & 0xffff0000;
	nWidth	= nWidth>>16;
	nHeight = m_dwResolution & 0x0000ffff;

	// 解像度を設定 
	nRet = PDC_SetResolution(nDeviceNo,
							nChildNo,
							nWidth,
							nHeight,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "解像度ﾞが設定できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}
	m_Width  = nWidth;
	m_Height = nHeight;	

	//カラー・モノクロ情報の取得
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "カラー・モノクロ情報ができませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//カラー
	//画像サイズによりライブ表示間隔を変更する
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}else{				//モノクロ
		//画像サイズによりライブ表示間隔を変更する
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

	//ライブ表示タイマースタート
	//ID NO.  = TIMER_MODE_LIVE
	//インターバル = nInterval (ms) 
	::SetTimer(GetSafeHwnd(), TIMER_MODE_LIVE, nInterval, NULL);
	//	画像表示領域をクリア
	ClearMovieRangeWindow(); 
	
	EndWaitCursor();	// 砂時計カーソルを消去します。
}
//-----------------------------------------------------------------------------------------------
//	子デバイス変更
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnSelchangeSelectCamera() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int		nSel;
	char	ss[10];
	CString	msg;

	//ライブ中？
	if(m_Execute_Live==FALSE){
		msg = "ライブ画像表示に切り替えてから実行してください";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}
	BeginWaitCursor();	// 砂時計カーソルを表示します。

	//選択した子デバイスを取得
	nSel = m_select_camera.GetCurSel();
	m_select_camera.GetLBText(nSel,ss);

	//変数にセット
	nChildNo = atoi(ss);

	m_IsDraw=FALSE;		//画像表示処理中でない
	EndWaitCursor();	// 砂時計カーソルを消去します。
}

//-----------------------------------------------------------------------------------------------
//	ライブモード 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnModeLive() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	int		nInterval;
	CString	msg;
	char nMode;

	//ライブ中？
	if(m_Execute_Live==TRUE){
		return;
	}

	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);

	//ライブモードとする	

	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_LIVE,
						 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "ライブモードに変更できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	//	画像表示領域をクリア
	ClearMovieRangeWindow(); 

	//変更可能にする
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
	//変更禁止にする
	CButton* button2;
	button2 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button2->EnableWindow(FALSE);
	button2 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button2->EnableWindow(FALSE);
	button2 = (CButton*)GetDlgItem(IDC_STOP_MEMORY);
	button2->EnableWindow(FALSE);

	//ライブ画像の解像度を取得
	nRet = PDC_GetResolution(nDeviceNo,
                     nChildNo,
                     &m_Width,
					 &m_Height,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "解像度を取得できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	//----------------------------------------
	//カメラはモノクロ or カラーか判別する
	//----------------------------------------
	nRet = PDC_GetColorType(nDeviceNo,
                         nChildNo,
                         &nMode,
                         &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "カラー・モノクロ設定の取得ができませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	if(nMode == PDC_COLORTYPE_COLOR){		//カラー
	//画像サイズによりライブ表示間隔を変更する
		if(m_Width<=512){
			nInterval = 100;	//100ms	
		}else if(m_Width<=640){
			nInterval = 200;	//200ms	
		}else if(m_Width<=1280){
			nInterval = 500;	//500ms	
		}else{
			nInterval = 1000;	//1000ms	
		}
	}else{				//モノクロ
		//画像サイズによりライブ表示間隔を変更する
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

	//ライブﾞ表示タイマースタート
	//ID NO.  = TIMER_MODE_LIVE
	//インターバル = nInterval (ms) 
	::SetTimer(GetSafeHwnd(), TIMER_MODE_LIVE, nInterval, NULL);
	//-----------------------
	//	画像表示領域をクリア
	//-----------------------
	ClearMovieRangeWindow(); 

	m_disp_status1.SetWindowText("ライブ画像表示中");
	m_Execute_Live = TRUE;	//ﾗｲﾌﾞ中

	UpdateWindow();
}

//-----------------------------------------------------------------------------------------------
//	メモリモード 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnModeMemory() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	//ライブ中？
	if(m_Execute_Live==FALSE){
		return;
	}
	//ライブ表示タイマーストップ
	//ID NO. = TIMER_MODE_LIVE 
	::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

	//メモリモードに変更
	unsigned long nRet;
	unsigned long nErrorCode;
	CString	msg;
	m_Execute_Live = FALSE;	//ライブ中ではない

	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_PLAYBACK,
						 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "メモリモードに変更できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}

	//録画フレーム情報を取得
	PDC_FRAME_INFO info;
	nRet = PDC_GetMemFrameInfo(nDeviceNo,
								nChildNo,
								&info,
								&nErrorCode);
	if(nRet == PDC_FAILED) {
		msg = "フレーム情報が取得できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		return;
	}

	m_nRecFrame = info.m_nRecordedFrames;
	m_nStartFrame = info.m_nStart;
	m_nEndFrame = info.m_nEnd;
	
	//フレーム数の表示
	char	cWk0[10];
	sprintf(cWk0,"[%d]",m_nEndFrame);
	m_disp_recframe.SetWindowText(cWk0);

	//再生フレームを開始フレームにセット
	m_PlayFrameCount = m_nStartFrame;

	//変更禁止にする
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
	//変更可能にする
	CButton* button2;
	button2 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button2->EnableWindow(TRUE);
	button2 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button2->EnableWindow(TRUE);
	button2 = (CButton*)GetDlgItem(IDC_STOP_MEMORY);
	button2->EnableWindow(TRUE);

	m_disp_status1.SetWindowText("メモリ");
	//-----------------------
	//	画像表示領域をクリア
	//-----------------------
	ClearMovieRangeWindow(); 

	//録画した画像の解像度を取得
	nRet = PDC_GetMemResolution(nDeviceNo,
                     nChildNo,
                     &m_Width,
					 &m_Height,
                     &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "解像度を取得できません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
	}
	DspPlayCamera();

	UpdateWindow();
}

//-----------------------------------------------------------------------------------------------
//	録画
//-----------------------------------------------------------------------------------------------

void CFastCamMonitorDlg::OnRecordMemory() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	CString msg;

	//ライブ中？
	if(m_Execute_Live==FALSE){
		msg = "ライブ画像表示に切り替えてから実行してください";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	m_Execute_Live = FALSE;	//ライブ中でない

	m_disp_status1.SetWindowText("録画準備中");

	//ライブ表示タイマーストップ
	//ID NO. = TIMER_MODE_LIVE 
	::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);
	
	BeginWaitCursor();	// 砂時計カーソルを表示します。

	//録画待機を設定 
	nRet =PDC_SetRecReady(nDeviceNo,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "録画準備に失敗しました";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		OnModeLive();
		return;
	}
	// 録画待機状態かチェック 
	while(1){
		nRet = PDC_GetStatus(nDeviceNo,
							&nStatus,
							&nErrorCode);
		if(nRet == PDC_FAILED){
			msg = "状態の取得に失敗しました";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
			EndWaitCursor();	// 砂時計カーソルを消去します。
			OnModeLive();
			return;
		}
		if ((nStatus == PDC_STATUS_RECREADY) ||
			(nStatus == PDC_STATUS_REC))
		{
			break;
		}
	}
	// 録画開始 
	nRet = PDC_TriggerIn(nDeviceNo,
						&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "録画開始に失敗しました";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		OnModeLive();
		return;
	}
	// 録画開始を確認 
	int wait = GetTickCount();
	while(1){
		nRet = PDC_GetStatus(nDeviceNo,
							 &nStatus,
							 &nErrorCode);
		if (nRet == PDC_FAILED) {
			msg = "状態の所得に失敗しました";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			EndWaitCursor();
			OnModeLive();
			return;
		}
		if (nStatus == PDC_STATUS_REC)
		{
			break;
		}
		if((int)(GetTickCount()-wait)>3000){	//3000ms以内に録画開始できなかった?
			msg = "録画開始ができませんでした";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
			EndWaitCursor();	// 砂時計カーソルを消去します。
			OnModeLive();
			return;
		}
	}

	//変更禁止にする
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

	//録画終了確認タイマースタート
	//ID NO.  = TIMER_MODE_RECORD
	//インターバル = 250 (ms) 
	::SetTimer(GetSafeHwnd(), TIMER_MODE_RECORD, 250, NULL);

	m_Execute_Record = TRUE;	//録画中
	m_disp_status1.SetWindowText("録画中");

	EndWaitCursor();	// 砂時計カーソルを消去します。
}

//-----------------------------------------------------------------------------------------------
//	再生(メモリ) 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnPlayMemory() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	CString msg;
	if(m_nRecFrame<=0){
		msg = "再生できる画像が存在しません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	//状態を取得する
	nRet = PDC_GetStatus(nDeviceNo,
						&nStatus,
						&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "状態の取得に失敗しました";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}
	//再生モードに設定する
	if(nStatus != PDC_STATUS_PLAYBACK){
		nRet = PDC_SetStatus(nDeviceNo,
							 PDC_STATUS_PLAYBACK,
							 &nErrorCode);
		if(nRet == PDC_FAILED){
			msg = "失敗しました";
			AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
			EndWaitCursor();
			return;
		}
	}

	//ライブ画像表示中の場合はタイマーストップ
	if(m_Execute_Live==TRUE){	//ライブ中
		//ライブ表示タイマーストップ
		//ID NO. = TIMER_MODE_LIVE 
		::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

		m_disp_status1.SetWindowText("モニタ停止");
		m_Execute_Live = FALSE;	//ライブ中でない
	}
	
	BeginWaitCursor();	// 砂時計カーソルを表示します。

	m_disp_status1.SetWindowText("再生中");

	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button1->EnableWindow(FALSE);
	button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button1->EnableWindow(FALSE);

	::SetTimer(GetSafeHwnd(), TIMER_MODE_PLAY, 250, NULL);

	EndWaitCursor();	// 砂時計カーソルを消去します。
}

//-----------------------------------------------------------------------------------------------
//	停止(メモリ) 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnStopMemory() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString	msg;

	::KillTimer(GetSafeHwnd(), TIMER_MODE_PLAY);
	//変更可能にする
	CButton* button1;
	button1 = (CButton*)GetDlgItem(IDC_RECORD_SAVE_HD);
	button1->EnableWindow(TRUE);
	button1 = (CButton*)GetDlgItem(IDC_PLAY_MEMORY);
	button1->EnableWindow(TRUE);

	m_disp_status1.SetWindowText("再生終了");
}

//-----------------------------------------------------------------------------------------------
//	録画保存(HD)
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnRecordSaveHd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
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
		msg = "保存できる画像が存在しません";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		return;
	}
	
	//メモリモードに変更
	nRet = PDC_SetStatus(nDeviceNo,
						 PDC_STATUS_PLAYBACK,
							 &nErrorCode);
	if (nRet == PDC_FAILED) {
		msg = "状態の設定に失敗しました";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
		EndWaitCursor();
		return;
	}
	//ライブ画像表示中の場合はタイマーストップ
	if(m_Execute_Live==TRUE){	//ライブ中
		//ライブ表示タイマーストップ
		//ID NO. = TIMER_MODE_LIVE 
		::KillTimer(GetSafeHwnd(), TIMER_MODE_LIVE);

		m_disp_status1.SetWindowText("モニタ停止");
		m_Execute_Live = FALSE;	//ライブ中ではない
	}

	//変更禁止にする
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
	BeginWaitCursor();	// 砂時計カーソルを表示します。

	m_disp_status1.SetWindowText("ファイル保存中");

	CFileDialog myDLG(FALSE,NULL,"FastCam",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"ビットマップ(*.bmp)|*.bmp|全て(*.*)|*.*||");
	if(myDLG.DoModal() == IDOK)
	{
		//画像保存フォルダを作成
		::GetLocalTime(&tm);				//年月日,時刻取得
		strcpy(cPath,myDLG.GetPathName());
		sprintf(cTime,"%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d\\",tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
		strcat(cPath,cTime);			//タイムスタンプを追加
		strcpy(cPathTemp,cPath);			//フォルダのディレクトリを一時保存
		CreateDirectory(cPath,NULL);		//フォルダ作成
	
		for(i=m_nStartFrame; i<=m_nEndFrame; i++){
			nRet = PDC_GetMemImageData(nDeviceNo,
										nChildNo,
										i,
										8,
										m_BmpImage,
										&nErrorCode);
			//-----------------------------
			//	BMPファイルを作成
			//-----------------------------
			strcpy(cPath,cPathTemp);
			sprintf(cWk0,"Temp%4.4d.bmp",i);
			strcat(cPath,cWk0);
			//カラー・モノクロ情報の取得
			nRet = PDC_GetColorType(nDeviceNo,
						 nChildNo,
						 &nMode,
						 &nErrorCode);
			if (nRet == PDC_FAILED) {
				msg = "カラー・モノクロ情報の取得に失敗しました";
				AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
				EndWaitCursor();
				return;
			}
			if(nMode == PDC_COLORTYPE_COLOR){		//カラー
				nSts = WriteBmp24File(cPath, m_Width, m_Height);
				if(nSts<0){
					msg = "ファイルに保存できませんでした";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
					EndWaitCursor();	// 砂時計カーソルを消去します。
					return;
				}
			}else{					//モﾉクロ
				nSts = WriteBmpFile(cPath, m_Width, m_Height);
				if(nSts<0){
					msg = "ファイルに保存できませんでした";
					AfxMessageBox(msg,MB_OK|MB_ICONWARNING);
					EndWaitCursor();	// 砂時計カーソルを消去します。
					return;
				}
			}
			//録画枚数を表示
			sprintf(cWk0,"FrameNo.[%d]",i);
			m_disp_status2.SetWindowText(cWk0);
		}

		m_disp_status1.SetWindowText("ファイル保存終了");

		msg = "ファイルに保存しました";
		AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
	}

	EndWaitCursor();	// 砂時計カーソル消去します。
}

//-----------------------------------------------------------------------------------------------
//	バージョン情報 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnVersionInf() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CAboutDlg DlgAbout;
	DlgAbout.DoModal();	
}

//-----------------------------------------------------------------------------------------------
//	プログラム終了 
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnProgramExit() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	unsigned long nRet;
	CString msg;

	msg = "プログラムを終了します";
	nRet = AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION);
	if(nRet==IDYES){
		CDialog::DestroyWindow();		//プログラム終了	
	}
}

//--------------------------------------------------------------------------------------------
//	シャッタースピード、解像度、トリガモードを設定
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

	BeginWaitCursor();	// 砂時計カーソルを表示します。
	//-----------------------------------------------------
	//	各種のパラメータを設定 
	//-----------------------------------------------------
	// シャッタースピードリストを取得 
	nRet = PDC_GetShutterSpeedFpsList(nDeviceNo,
										nChildNo,
										&nSize,
										nShutterList,
										&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "シャッタースピードリストが取得できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}
	// シャッター速度のリストを表示 
	m_change_shutter.ResetContent();
	for(i=0; i<nSize; i++){
			sprintf(cWk0,"1/%d",nShutterList[i]);
			m_change_shutter.InsertString(-1,cWk0);
	}

	// リストの0番目を採用する 
	m_change_shutter.SetCurSel(0);	


	//シャッタースピードを変数にセット
	//int		m_nShutterSpeed;	//シャッタースピード
	m_nShutterSpeed = nShutterList[0];


	//シャッタースピードの設定 
	nRet = PDC_SetShutterSpeedFps(nDeviceNo,
									nChildNo,
									m_nShutterSpeed,
									&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "シャッタースピードが設定できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}

	// 解像度のリストを取得 
	nRet = PDC_GetResolutionList(nDeviceNo,
										nChildNo,
										&nSize,
										nResolutionList,
										&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "解像度のリストが取得できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}	

	// 解像度のリストを表示 
	m_change_resolution.ResetContent();
	for(i=0; i<nSize; i++){
		nWidth	= nResolutionList[i] & 0xffff0000;
		nWidth	= nWidth>>16;
		nHeight = nResolutionList[i] & 0x0000ffff;
		sprintf(cWk0,"%dx%d",nWidth,nHeight);
		m_change_resolution.InsertString(-1,cWk0);
	}

	//リストの0番目を採用する 
	m_change_resolution.SetCurSel(0);	

	//解像度を変数にｾｯﾄ 
	m_dwResolution = nResolutionList[0];
		
	// 解像度を設定 
	nWidth	= m_dwResolution & 0xffff0000;
	nWidth	= nWidth>>16;
	nHeight = m_dwResolution & 0x0000ffff;

	nRet = PDC_SetResolution(nDeviceNo,
							nChildNo,
							nWidth,
							nHeight,
							&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "解像度が設定できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}
	// 解像度を設定 
	m_Width  = nWidth;
	m_Height = nHeight;

	
	// トリガモードを設定 	
	nRet = PDC_SetTriggerMode(nDeviceNo,
								PDC_TRIGGER_START,
								0,
								0,
								0,
								&nErrorCode);
	if(nRet == PDC_FAILED){
		msg = "トリガモードが設定できませんでした";
		AfxMessageBox(msg,MB_OK|MB_ICONWARNING);	
		EndWaitCursor();	// 砂時計カーソルを消去します。
		return;
	}
	
	//トリガモードを画面に表示
	m_disp_trigmode.SetWindowText("START");


	EndWaitCursor();	// 砂時計カーソルを消去します。
}

//-----------------------------------------------------------------------------------------------
//	CFastCamMonitorDlg 破棄
//-----------------------------------------------------------------------------------------------
void CFastCamMonitorDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	unsigned long nRet;
	unsigned long nErrorCode;
	nRet = PDC_CloseDevice(nDeviceNo, &nErrorCode);

	// 使用していれば開放する 
	if(m_BmpInfo)	
		delete[] m_BmpInfo;
	if(m_BmpImage)	
		delete[] m_BmpImage;
}

//-----------------------------------------------------------------------------------------------
//	BMPファイルを書き込み	8ビットモノクロ
//-----------------------------------------------------------------------------------------------
int	CFastCamMonitorDlg::WriteBmpFile(char *szFName,int nWidth, int nHeight)
{
	int		i;
	int		x,y;
	int		nCnt;
	int		nPos;
	CFile	file;

	//ファイルをオープンする
	if(!file.Open(szFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		return(-1);
	}

	//----------------------------------------------------------------------------
	// BMPヘッダ部 	
	//----------------------------------------------------------------------------
	//WORD  : BmpFileHeader.bfType;				BM という文字
	//DWORD : BmpFileHeader.bfSize;				ファイルの全体サイズ
	//WORD  : BmpFileHeader.bfReserved1;		0 にする
	//WORD  : BmpFileHeader.bfReserved2;		0 にする	
	//DWORD : BmpFileHeader.bfOffBits;			ビットマップビットのファイル先頭からのオフセット
	m_gnSizeFileHdr = sizeof( BITMAPFILEHEADER );	//BMPヘッダ部サイズ(14バイト)
	m_gnSizeInfoHdr = sizeof( BITMAPINFOHEADER );	//BMP情報部サイズ(40バイト)
	m_gnSizeRgbQuad = sizeof( RGBQUAD )*256;		//パレットサイズ(1024バイト)
	m_gnSizeTop	 = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad;
												//BM という文字
	memcpy( (char *)&m_BmpFileHdr.bfType, "BM", 2 );

	m_gnSizeBmpImage = nWidth * nHeight;		//ビットマップの横幅(Width) x 高さ(Height)
												//ファイルの全体サイズ
	m_BmpFileHdr.bfSize = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad + m_gnSizeBmpImage;
	m_BmpFileHdr.bfReserved1 = 0;				//0 にする
	m_BmpFileHdr.bfReserved2 = 0;				//0 にする
	m_BmpFileHdr.bfOffBits = m_gnSizeTop;		//ビットマップビットのファイル先頭からのオフセット

	file.Write(&m_BmpFileHdr, sizeof(BITMAPFILEHEADER));

	//----------------------------------------------------------------------------
	//	BMP情報部
	//----------------------------------------------------------------------------
	//DWORD : BmpInfoHeader.biSize;				BITMAPINFOHEADER型構造体のサイズ
	//											(単位はバイト)
	//LONG  : BmpInfoHeader.biWidth;			ビットマップの幅  (単位はピクセル)
	//LONG  : BmpInfoHeader.biHeight;			ビットマップの高さ(単位はピクセル)
	//WORD  : BmpInfoHeader.biPlanes;			1 にする
	//WORD  : BmpInfoHeader.biBitCount;			ピクセルあたりのカラービット数(1,4,8,24)
	//DWORD : BmpInfoHeader.biCompression;		圧縮方式(圧縮しない時は 0)
	//DWORD : BmpInfoHeader.biSizeImage;		ビットマップビットのサイズ	
	//											(単位はバイト) 圧縮するときのみ必要
	//LONG  : BmpInfoHeader.biXPelsPerMeter;	水平解像度(1メートルあたりのピクセル数)
	//LONG  : BmpInfoHeader.biYPelsPerMeter;	垂直解像度(1メートルあたりのピクセル数)
	//DWORD : BmpInfoHeader.biClrUsed;			イメージで使われている色の数
	//DWORD : BmpInfoHeader.biClrImportant;		イメージで使われている重要な色の数

	m_BmpInfoHdr.biSize			= 40L;			//BITMAPINFOHEADER型構造体のサイズ
	m_BmpInfoHdr.biWidth		= nWidth;		//ビットマップの幅  (単位はピクセル)
	m_BmpInfoHdr.biHeight		= nHeight;		//ビットマップの高さ(単位はピクセル)
	m_BmpInfoHdr.biPlanes		= 1;			//1 にする
	m_BmpInfoHdr.biBitCount 	= 8;			//ピクセルあたりのカラービット数(1,4,8,24)
	m_BmpInfoHdr.biCompression	= 0;			//圧縮方式(圧縮しない時は 0)
	m_BmpInfoHdr.biSizeImage = m_gnSizeBmpImage;//ビットマップビットのサイズ
												//圧縮するときのみ必要
	m_BmpInfoHdr.biXPelsPerMeter	= 19344;	//水平解像度(1メートルあたりのピクセル数)
	m_BmpInfoHdr.biYPelsPerMeter	= 19344;	//垂直解像度(1メートルあたりのピクセル数)
	m_BmpInfoHdr.biClrUsed			= 256;		//イメージで使われている色の数
	m_BmpInfoHdr.biClrImportant	=   0;			//イメージで使われている重要な色の数

	file.Write(&m_BmpInfoHdr,sizeof(BITMAPINFOHEADER));
	
	//-----------------
	//	パレットを書き込み
	//-----------------
	//BYTE  :	BmpRgbQuad.rgbBlue;		青の度合い
	//BYTE  :	BmpRgbQuad.rgbGreen;	緑の度合い
	//BYTE  :	BmpRgbQuad.rgbRed;		赤の度合い
	//BYTE  :	BmpRgbQuad.rgbReserved;	0 にする
	for( i=0; i<256; i++ ){
		m_BmpRgbQuad[i].rgbBlue		= i;	//青の度合い
		m_BmpRgbQuad[i].rgbGreen	= i;	//緑の度合い
		m_BmpRgbQuad[i].rgbRed		= i;	//赤の度合い
		m_BmpRgbQuad[i].rgbReserved	= 0;	//0 にする
	}

	nCnt = sizeof(RGBQUAD)*256;
	file.Write(&m_BmpRgbQuad[0], nCnt);

	//--------------------------
	//	ビットマップイメージを書き込み
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
	//ファイルをクローズする
	file.Close();

	return(0);
}

//-----------------------------------------------------------------------------------------------
//	BMPファイルを書き込み	24ビットフルカラー
//-----------------------------------------------------------------------------------------------
int	CFastCamMonitorDlg::WriteBmp24File(char *szFName,int nWidth, int nHeight)
{
	int		x,y;
	int		nPos0;
	int		nPos1;
	CFile	file;

	//ファイルをオープンする
	if(!file.Open(szFName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary)){
		return(-1);
	}

	//----------------------------------------------------------------------------
	// BMPヘッダ部 	
	//----------------------------------------------------------------------------
	//WORD  : BmpFileHeader.bfType;				BM という文字
	//DWORD : BmpFileHeader.bfSize;				ファイルの全体サイズ
	//WORD  : BmpFileHeader.bfReserved1;		0 にする
	//WORD  : BmpFileHeader.bfReserved2;		0 にする	
	//DWORD : BmpFileHeader.bfOffBits;			ビットマップビットのファイル先頭からのオフセット
	m_gnSizeFileHdr = sizeof( BITMAPFILEHEADER );	//BMPヘッダ部サイズ(14バイト)
	m_gnSizeInfoHdr = sizeof( BITMAPINFOHEADER );	//BMP情報部サイズ(40バイト)
	m_gnSizeRgbQuad = 0;							//パレットサイズ(0バイト)
	m_gnSizeTop	 = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad;
												//BM という文字
	memcpy( (char *)&m_BmpFileHdr.bfType, "BM", 2 );

	m_gnSizeBmpImage = nWidth * nHeight * 3;	//ビットマップビットの横幅(Width) x 高さ(Height)
												//ファイrの全体サイズ
	m_BmpFileHdr.bfSize = m_gnSizeFileHdr + m_gnSizeInfoHdr + m_gnSizeRgbQuad + m_gnSizeBmpImage;
	m_BmpFileHdr.bfReserved1 = 0;				//0 にする
	m_BmpFileHdr.bfReserved2 = 0;				//0 にする
	m_BmpFileHdr.bfOffBits = m_gnSizeTop;		//ビットマップビットのファイル先頭からのオフセット

	file.Write(&m_BmpFileHdr, sizeof(BITMAPFILEHEADER));

	//----------------------------------------------------------------------------
	//	BMP情報部
	//----------------------------------------------------------------------------
	//DWORD : BmpInfoHeader.biSize;				BITMAPINFOHEADER型構造体のサイズ
	//											(単位はバイト)
	//LONG  : BmpInfoHeader.biWidth;			ビットマップの幅  (単位はピクセル)
	//LONG  : BmpInfoHeader.biHeight;			ビットマップの高さ(単位はピクセル)
	//WORD  : BmpInfoHeader.biPlanes;			1 にする
	//WORD  : BmpInfoHeader.biBitCount;			ピクセルあたりのカラービット数(1,4,8,24)
	//DWORD : BmpInfoHeader.biCompression;		圧縮方式(圧縮しない時は 0)
	//DWORD : BmpInfoHeader.biSizeImage;		ビットマップビットのサイズ	
	//											(単位はバイト) 圧縮するときのみ必要
	//LONG  : BmpInfoHeader.biXPelsPerMeter;	水平解像度(1メートルあたりのピクセル数)
	//LONG  : BmpInfoHeader.biYPelsPerMeter;	垂直解像度(1メートルあたりのピクセル数)
	//DWORD : BmpInfoHeader.biClrUsed;			イメージで使われている色の数
	//DWORD : BmpInfoHeader.biClrImportant;		イメージで使われている重要な色の数

	m_BmpInfoHdr.biSize			= 40L;			//BITMAPINFOHEADER型構造体のサイズ
	m_BmpInfoHdr.biWidth		= nWidth;		//ビットマップの幅  (単位はピクセル)
	m_BmpInfoHdr.biHeight		= nHeight;		//ビットマップの高さ(単位はピクセル)
	m_BmpInfoHdr.biPlanes		= 1;			//1 にする
	m_BmpInfoHdr.biBitCount 	= 24;			//ピクセルあたりのカラービット数(1,4,8,24)
	m_BmpInfoHdr.biCompression	= 0;			//圧縮方式(圧縮しない時は 0)
	m_BmpInfoHdr.biSizeImage = m_gnSizeBmpImage;//ビットマップビットのサイズ
												//圧縮するときのみ必要
	m_BmpInfoHdr.biXPelsPerMeter	= 0;		//水平解像度(1メートルあたりのピクセル数)
	m_BmpInfoHdr.biYPelsPerMeter	= 0;		//垂直解像度(1メートルあたりのピクセル数)
	m_BmpInfoHdr.biClrUsed			= 0;		//イメージで使われている色の数
	m_BmpInfoHdr.biClrImportant		= 0;		//イメージで使われている重要な色の数

	file.Write(&m_BmpInfoHdr,sizeof(BITMAPINFOHEADER));

	//-----------------
	//	パレット書き込み
	//-----------------
	//カラーの為パレットはなし

	//--------------------------
	//	ビットマップイメージを書き込み
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

	//ファイルをクローズする
	file.Close();

	return(0);
}

