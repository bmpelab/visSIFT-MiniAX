// FastCamMonitorDlg.h : ヘッダー ファイル
//

#if !defined(AFX_FASTCAMMONITORDLG_H__31092FFA_2C20_4F0A_B234_ED7FB299A3C0__INCLUDED_)
#define AFX_FASTCAMMONITORDLG_H__31092FFA_2C20_4F0A_B234_ED7FB299A3C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//PDCLIB.hの追加
#include "PDCLIB.h"

//定数の定義
#define TIMER_MODE_LIVE 1000
#define TIMER_MODE_RECORD 2000 
#define TIMER_MODE_PLAY 3000

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorDlg ダイアログ

class CFastCamMonitorDlg : public CDialog
{
// 構築
public:
	CFastCamMonitorDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CFastCamMonitorDlg)
	enum { IDD = IDD_FASTCAMMONITOR_DIALOG };
	CStatic	m_disp_status1;
	CStatic m_disp_status2;
	CStatic m_device_name;
	CStatic m_disp_recframe;
	CStatic m_disp_trigmode;
	CComboBox m_select_camera;
	CComboBox m_change_rate;
	CComboBox m_change_shutter;
	CComboBox m_change_resolution;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFastCamMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFastCamMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeChangeRate();
	afx_msg void OnSelchangeChangeShutter();
	afx_msg void OnSelchangeChangeResolution();
	afx_msg void OnSelchangeSelectCamera();
	afx_msg void OnModeLive();
	afx_msg void OnModeMemory();
	afx_msg void OnRecordMemory();
	afx_msg void OnRecordSaveHd();
	afx_msg void OnPlayMemory();
	afx_msg void OnStopMemory();
	afx_msg void OnVersionInf();
	afx_msg void OnProgramExit();
	afx_msg void OnDestroy();
	//}}AFX_MSG
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
	int	 WriteBmpFile(char *szFName,int nWidth, int nHeight);
	int	 WriteBmp24File(char *szFName,int nWidth, int nHeight);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FASTCAMMONITORDLG_H__31092FFA_2C20_4F0A_B234_ED7FB299A3C0__INCLUDED_)
