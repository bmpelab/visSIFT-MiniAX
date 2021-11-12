// FastCamMonitorDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_FASTCAMMONITORDLG_H__31092FFA_2C20_4F0A_B234_ED7FB299A3C0__INCLUDED_)
#define AFX_FASTCAMMONITORDLG_H__31092FFA_2C20_4F0A_B234_ED7FB299A3C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//PDCLIB.h�̒ǉ�
#include "PDCLIB.h"

//�萔�̒�`
#define TIMER_MODE_LIVE 1000
#define TIMER_MODE_RECORD 2000 
#define TIMER_MODE_PLAY 3000

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorDlg �_�C�A���O

class CFastCamMonitorDlg : public CDialog
{
// �\�z
public:
	CFastCamMonitorDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
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

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFastCamMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
		
	//�J�����p�����[�^
	unsigned long nDeviceNo;    //�f�o�C�X�i���o�[
	unsigned long nChildNo;		//�q�f�o�C�X�i���o�[
	unsigned long nStatus;     //�f�o�C�X���
	unsigned long ChildNo[PDC_MAX_DEVICE];    //�ڑ�����Ă���q�f�o�C�X�i���o�[�ꗗ
	unsigned long  m_PlayFrameCount;	//�Đ��J�E���^
	unsigned long nShutterList[PDC_MAX_LIST_NUMBER];    //�V���b�^�[���x�ꗗ 
	unsigned long nResolutionList[PDC_MAX_LIST_NUMBER];    //�𑜓x�ꗗ
	BOOL	m_IsDraw;			//�摜�\���������t���O
	BOOL	m_Execute_Live;		//���C�u��
	BOOL	m_Execute_Record;	//�^�撆
	unsigned long		m_nRecordRate;		//��荞�݃��[�g
	int		m_nShutterSpeed;	//�V���b�^�[�X�s�[�h
	int		m_nTrigMode;		//�g���K���[�h
	int		m_dwResolution;		//�𑜓x
	unsigned long		m_Width;			//�摜�T�C�Y(Width)
	unsigned long		m_Height;			//�摜�T�C�Y(Height)
	unsigned long		m_nRecFrame;		//�^�文��
	unsigned long		m_nStartFrame;		//�J�n�t���[��
	unsigned long		m_nEndFrame;		//�ŏI�t���[��

	//BMP�t�@�C���֌W
	LPBITMAPINFO		m_BmpInfo;			//INFO�|�C���^
	BITMAPFILEHEADER	m_BmpFileHdr;		//HEADER
	BITMAPINFOHEADER	m_BmpInfoHdr;		//INFO
	RGBQUAD				m_BmpRgbQuad[256];	//RGBQUAD
	FILE				*m_pBmpFp;			//�t�@�C���|�C���^
	char*				m_BmpImage;			//�摜�C���[�W�|�C���^
	int					m_gnSizeFileHdr;	//Header�T�C�Y
	int					m_gnSizeInfoHdr;	//HEADER+INFO�T�C�Y
	int					m_gnSizeInfo;		//INFO�T�C�Y
	int					m_gnSizeRgbQuad;	//RGBQUAD�T�C�Y
	int					m_gnSizeTop;		//HEADER+INFO+RGBQUAD�T�C�Y
	int					m_gnSizeBmpImage;	//�摜�C���[�W�T�C�Y

	void DspLiveCamera();
	void DspPlayCamera();
	void UpdateParam();
	void UpdateMovieRangeWindow();
	void ClearMovieRangeWindow();
	int	 WriteBmpFile(char *szFName,int nWidth, int nHeight);
	int	 WriteBmp24File(char *szFName,int nWidth, int nHeight);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FASTCAMMONITORDLG_H__31092FFA_2C20_4F0A_B234_ED7FB299A3C0__INCLUDED_)
