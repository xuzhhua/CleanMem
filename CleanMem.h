// CleanMem.h : CLEANMEM アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_CLEANMEM_H__B0433D2C_E4FF_4F28_BF51_D4720C6D4AA7__INCLUDED_)
#define AFX_CLEANMEM_H__B0433D2C_E4FF_4F28_BF51_D4720C6D4AA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CCleanMemApp:
// このクラスの動作の定義に関しては CleanMem.cpp ファイルを参照してください。
//

class CCleanMemApp : public CWinApp
{
public:
	CCleanMemApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCleanMemApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CCleanMemApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CLEANMEM_H__B0433D2C_E4FF_4F28_BF51_D4720C6D4AA7__INCLUDED_)
