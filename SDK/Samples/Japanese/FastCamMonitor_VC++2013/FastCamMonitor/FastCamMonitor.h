// FastCamMonitor.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CFastCamMonitorApp:
// このクラスの実装については、FastCamMonitor.cpp を参照してください。
//

class CFastCamMonitorApp : public CWinApp
{
public:
	CFastCamMonitorApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CFastCamMonitorApp theApp;