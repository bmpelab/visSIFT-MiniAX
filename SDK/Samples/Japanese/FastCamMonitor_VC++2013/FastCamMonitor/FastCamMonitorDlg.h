// FastCamMonitorDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

#include "afxwin.h"

//PDCLIB.h�̒ǉ�
#include "PDCLIB.h"

//�萔�̒�`
#define TIMER_MODE_LIVE 1000
#define TIMER_MODE_RECORD 2000 
#define TIMER_MODE_PLAY 3000

// CFastCamMonitorDlg �_�C�A���O
class CFastCamMonitorDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CFastCamMonitorDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FASTCAMMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
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
