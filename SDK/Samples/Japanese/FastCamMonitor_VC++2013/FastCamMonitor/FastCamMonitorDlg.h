// FastCamMonitorDlg.h : ヘッダー ファイル
//

#pragma once

#include "afxwin.h"

//PDCLIB.hの追加
#include "PDCLIB.h"

//定数の定義
#define TIMER_MODE_LIVE 1000
#define TIMER_MODE_RECORD 2000 
#define TIMER_MODE_PLAY 3000

// CFastCamMonitorDlg ダイアログ
class CFastCamMonitorDlg : public CDialog
{
// コンストラクション
public:
	CFastCamMonitorDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_FASTCAMMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
		
	//カメラパラメータ
	unsigned long nDeviceNo;    //デバイスナンバー
	unsigned long nChildNo;		//子デバイスナンバー
	unsigned long nStatus;     //デバイス状態
	unsigned long ChildNo[PDC_MAX_DEVICE];    //接続されている子デバイスナンバー一覧
	unsigned long  m_PlayFrameCount;	//再生カウンタ
	unsigned long nShutterList[PDC_MAX_LIST_NUMBER];    //シャッター速度一覧 
	unsigned long nResolutionList[PDC_MAX_LIST_NUMBER];    //解像度一覧
	BOOL	m_IsDraw;			//画像表示処理中フラグ
	BOOL	m_Execute_Live;		//ライブ中
	BOOL	m_Execute_Record;	//録画中
	unsigned long		m_nRecordRate;		//取り込みレート
	int		m_nShutterSpeed;	//シャッタースピード
	int		m_nTrigMode;		//トリガモード
	int		m_dwResolution;		//解像度
	unsigned long		m_Width;			//画像サイズ(Width)
	unsigned long		m_Height;			//画像サイズ(Height)
	unsigned long		m_nRecFrame;		//録画枚数
	unsigned long		m_nStartFrame;		//開始フレーム
	unsigned long		m_nEndFrame;		//最終フレーム

	//BMPファイル関係
	LPBITMAPINFO		m_BmpInfo;			//INFOポインタ
	BITMAPFILEHEADER	m_BmpFileHdr;		//HEADER
	BITMAPINFOHEADER	m_BmpInfoHdr;		//INFO
	RGBQUAD				m_BmpRgbQuad[256];	//RGBQUAD
	FILE				*m_pBmpFp;			//ファイルポインタ
	char*				m_BmpImage;			//画像イメージポインタ
	int					m_gnSizeFileHdr;	//Headerサイズ
	int					m_gnSizeInfoHdr;	//HEADER+INFOサイズ
	int					m_gnSizeInfo;		//INFOサイズ
	int					m_gnSizeRgbQuad;	//RGBQUADサイズ
	int					m_gnSizeTop;		//HEADER+INFO+RGBQUADサイズ
	int					m_gnSizeBmpImage;	//画像イメージサイズ

	void DspLiveCamera();
	void DspPlayCamera();
	void UpdateParam();
	void UpdateMovieRangeWindow();
	void ClearMovieRangeWindow();
    int	 WriteBmpFile(const TCHAR *szFName, int nWidth, int nHeight);
	int	 WriteBmp24File(const TCHAR *szFName,int nWidth, int nHeight);
	CStatic m_disp_status1;
	CStatic m_disp_status2;
	CStatic m_device_name;
	CStatic m_disp_recframe;
	CStatic m_disp_trigmode;
	CComboBox m_select_camera;
	CComboBox m_change_rate;
	CComboBox m_change_shutter;
	CComboBox m_change_resolution;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeSelectCamera();
	afx_msg void OnBnClickedRecordMemory();
	afx_msg void OnBnClickedRecordSaveHd();
	afx_msg void OnBnClickedPlayMemory();
	afx_msg void OnBnClickedVersionInf();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStopMemory();
	afx_msg void OnCbnSelchangeChangeShutter();
	afx_msg void OnCbnSelchangeChangeResolution();
	afx_msg void OnCbnSelchangeChangeRate();
	afx_msg void OnBnClickedModeLive();
	afx_msg void OnBnClickedModeMemory();
	afx_msg void OnBnClickedProgramExit();
};
