// FastCamMonitor.h : FASTCAMMONITOR アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_FASTCAMMONITOR_H__759EE81B_A440_462E_8819_6733C5DB049C__INCLUDED_)
#define AFX_FASTCAMMONITOR_H__759EE81B_A440_462E_8819_6733C5DB049C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CFastCamMonitorApp:
// このクラスの動作の定義に関しては FastCamMonitor.cpp ファイルを参照してください。
//

class CFastCamMonitorApp : public CWinApp
{
public:
	CFastCamMonitorApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFastCamMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CFastCamMonitorApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FASTCAMMONITOR_H__759EE81B_A440_462E_8819_6733C5DB049C__INCLUDED_)
