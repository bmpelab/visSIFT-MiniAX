// FastCamMonitor.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "FastCamMonitor.h"
#include "FastCamMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFastCamMonitorApp

BEGIN_MESSAGE_MAP(CFastCamMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFastCamMonitorApp �R���X�g���N�V����

CFastCamMonitorApp::CFastCamMonitorApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CFastCamMonitorApp �I�u�W�F�N�g�ł��B

CFastCamMonitorApp theApp;


// CFastCamMonitorApp ������

BOOL CFastCamMonitorApp::InitInstance()
{
	// �A�v���P�[�V�����@�}�j�t�F�X�g���@visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll �o�[�W���� 6�@�ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �Ɂ@InitCommonControls() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// �W��������
	// �����̋@�\���g�킸�ɁA�ŏI�I�Ȏ��s�\�t�@�C���̃T�C�Y���k���������ꍇ�́A
	// �ȉ�����A�s�v�ȏ��������[�`����
	// �폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͑g�D���Ȃǂ́A
	// �K�؂ȕ�����ɕύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));

	CFastCamMonitorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
