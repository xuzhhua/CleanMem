// CleanMemDlg.h : ヘッダー ファイル
//

#if !defined(AFX_CLEANMEMDLG_H__06E4F9C5_44B8_4191_9852_4FEF2EDE123B__INCLUDED_)
#define AFX_CLEANMEMDLG_H__06E4F9C5_44B8_4191_9852_4FEF2EDE123B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RegExec.h"
#include "BtnST.h"
#include "BCMenu.h"
#include "WinXPButtonST.h"

#define WM_TRAYMOUSE_MSG	WM_USER + 101
#define IDT_CLEANMEM		255
#define ID_CLEANMEM_TRAY	265
#define IDT_HIDEWINDOW		275
/////////////////////////////////////////////////////////////////////////////
// CCleanMemDlg ダイアログ

class CCleanMemDlg : public CDialog
{
// 構築
public:
	CCleanMemDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCleanMemDlg)
	enum { IDD = IDD_CLEANMEM_DIALOG };
	CComboBox	m_Percent;
	CButtonST	m_Close;
	CButtonST	m_Exit;
	CButtonST	m_CleanIt;
	CButton		m_AutoRun;
	CStatic		m_PerMem;
	CProgressCtrl	m_pcMemory;
	//}}AFX_DATA
	void mAdjustTokenPrivilegesForNT(void);
	BOOL mEmptyAllSet(void);
	void mGetAutoRunInfo(void);
	void mGetMemory(void);
	BOOL mCheckMemory(void);
	void mCreateMyPopMenu();
	void mCreateTrayIcon();
	int  mGetMemPercent();

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCleanMemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_TrayIcon;
	BCMenu TrayPop;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCleanMemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnExit();
	afx_msg LRESULT OnTaskBarCreated(WPARAM wp, LPARAM lp);
	afx_msg void OnCleanmem();
	afx_msg void OnAutorun();
	afx_msg void OnOption();
	afx_msg void OnClose();
	afx_msg void OnSelchangePercent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CLEANMEMDLG_H__06E4F9C5_44B8_4191_9852_4FEF2EDE123B__INCLUDED_)
