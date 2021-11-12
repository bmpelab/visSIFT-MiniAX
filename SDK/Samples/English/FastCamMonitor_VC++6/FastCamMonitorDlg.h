// FastCamMonitorDlg.h 
//

#if !defined(AFX_FASTCAMMONITORDLG_H__E0C9DFAD_7210_48B6_BCBB_D96665E32CF0__INCLUDED_)
#define AFX_FASTCAMMONITORDLG_H__E0C9DFAD_7210_48B6_BCBB_D96665E32CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//add PDCLIB.h
#include "PDCLIB.h"

//define constant numbers
#define TIMER_MODE_LIVE 1000
#define TIMER_MODE_RECORD 2000 
#define TIMER_MODE_PLAY 3000

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorDlg 

class CFastCamMonitorDlg : public CDialog
{
// construct
public:
	CFastCamMonitorDlg(CWnd* pParent = NULL);	

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

	
	//{{AFX_VIRTUAL(CFastCamMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

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
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_FASTCAMMONITORDLG_H__E0C9DFAD_7210_48B6_BCBB_D96665E32CF0__INCLUDED_)
