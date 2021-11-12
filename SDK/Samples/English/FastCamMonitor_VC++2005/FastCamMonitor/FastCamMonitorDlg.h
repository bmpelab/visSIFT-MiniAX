// FastCamMonitorDlg.h : header file
//

#pragma once

#include "PDCLIB.h"

#define TIMER_MODE_LIVE 1000
#define TIMER_MODE_RECORD 2000 
#define TIMER_MODE_PLAY 3000

// CFastCamMonitorDlg dialog
class CFastCamMonitorDlg : public CDialog
{
// Construction
public:
	CFastCamMonitorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FASTCAMMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	
	// Camera parameters	
	unsigned long nDeviceNo;    //device number
	unsigned long nChildNo;		//child device number
	unsigned long nStatus;     //device status 
	unsigned long ChildNo[PDC_MAX_DEVICE]; //the number of child devices    
	unsigned long  m_PlayFrameCount;	//playback image counter
	unsigned long nShutterList[PDC_MAX_LIST_NUMBER]; //list of shutter speed    
	unsigned long nResolutionList[PDC_MAX_LIST_NUMBER]; //list of resolution    
	BOOL	m_IsDraw;			//flag for drawing image
	BOOL	m_Execute_Live;		//execute live 
	BOOL	m_Execute_Record;	//execute recording
	unsigned long		m_nRecordRate; //current frame rate		
	int		m_nShutterSpeed; //current shutter speed
	int		m_nTrigMode;	//current trigger mode
	int		m_dwResolution;	//current resolution
	unsigned long		m_Width; //image width
	unsigned long		m_Height;  //image height
	unsigned long		m_nRecFrame; //the number of recorded frames 
	unsigned long		m_nStartFrame; //start frame number
	unsigned long		m_nEndFrame; //end frame number

	//BMP file information
	LPBITMAPINFO		m_BmpInfo;		
	BITMAPFILEHEADER	m_BmpFileHdr; 
	BITMAPINFOHEADER	m_BmpInfoHdr;		
	RGBQUAD				m_BmpRgbQuad[256];	
	FILE				*m_pBmpFp;			
	char*				m_BmpImage;			
	int					m_gnSizeFileHdr;	
	int					m_gnSizeInfoHdr;	
	int					m_gnSizeInfo;		
	int					m_gnSizeRgbQuad;	
	int					m_gnSizeTop;		
	int					m_gnSizeBmpImage;	


	void DspLiveCamera();
	void DspPlayCamera();
	void UpdateParam();
	void UpdateMovieRangeWindow();
	void ClearMovieRangeWindow();
	int	 WriteBmpFile(char *szFName,int nWidth, int nHeight);
	int	 WriteBmp24File(char *szFName,int nWidth, int nHeight);
public:
	CStatic m_disp_status1;
	CStatic m_disp_status2;
	CStatic m_device_name;
	CStatic m_disp_recframe;
	CStatic m_disp_trigmode;
	CComboBox m_select_camera;
	CComboBox m_change_rate;
	CComboBox m_change_shutter;
	CComboBox m_change_resolution;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeChangeRate();
	afx_msg void OnCbnSelchangeChangeShutter();
	afx_msg void OnCbnSelchangeChangeResolution();
	afx_msg void OnCbnSelchangeSelectCamera();
	afx_msg void OnBnClickedModeLive();
	afx_msg void OnBnClickedModeMemory();
	afx_msg void OnBnClickedRecordMemory();
	afx_msg void OnBnClickedRecordSaveHd();
	afx_msg void OnBnClickedPlayMemory();
	afx_msg void OnBnClickedStopMemory();
	afx_msg void OnBnClickedVersionInf();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedProgramExit();
	afx_msg void OnTimer(UINT nIDEvent);
};
