// CleanMemDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CleanMem.h"
#include "CleanMemDlg.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <wtypes.h>
#pragma comment (lib,"psapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

/////////////////////////////////////////////////////////////////////////////
// CCleanMemDlg ダイアログ

CCleanMemDlg::CCleanMemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCleanMemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCleanMemDlg)
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCleanMemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCleanMemDlg)
	DDX_Control(pDX, IDC_PERCENT, m_Percent);
	DDX_Control(pDX, IDC_CLOSE, m_Close);
	DDX_Control(pDX, IDC_EXIT, m_Exit);
	DDX_Control(pDX, IDC_CLEANMEM, m_CleanIt);
	DDX_Control(pDX, IDC_AUTORUN, m_AutoRun);
	DDX_Control(pDX, IDC_MEMPER, m_PerMem);
	DDX_Control(pDX, IDC_MEMORY, m_pcMemory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCleanMemDlg, CDialog)
	//{{AFX_MSG_MAP(CCleanMemDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	ON_BN_CLICKED(IDC_CLEANMEM, OnCleanmem)
	ON_BN_CLICKED(IDC_AUTORUN, OnAutorun)
	ON_BN_CLICKED(IDC_OPTION, OnOption)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_CBN_SELCHANGE(IDC_PERCENT, OnSelchangePercent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCleanMemDlg メッセージ ハンドラ

BOOL CCleanMemDlg::OnInitDialog()
{
	COLORREF	m_shBtnColor = RGB(255,255,210)/*30*/;

	CDialog::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->EnableMenuItem(IDM_CLOSE, MF_BYPOSITION || MF_DISABLED);
	}
	
	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	mCreateMyPopMenu();
	mCreateTrayIcon();
	
	mGetAutoRunInfo();
	CenterWindow();
	ShowWindow(SW_MINIMIZE);
	
	::SetTimer(this->m_hWnd, IDT_HIDEWINDOW, 50, NULL);
	::SetTimer(this->m_hWnd, IDT_CLEANMEM, 30000, NULL);
	
	m_Percent.AddString("10%");
	m_Percent.AddString("20%");
	m_Percent.AddString("30%");
	m_Percent.AddString("40%");
	m_Percent.AddString("50%");
	m_Percent.AddString("60%");
	m_Percent.AddString("70%");

	if (mGetMemPercent() == 9)
	{
		m_Percent.SetCurSel(3);
	} else {
		m_Percent.SetCurSel(mGetMemPercent());
	}
	
	mGetMemory();

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CCleanMemDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CCleanMemDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCleanMemDlg::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	//CDialog::OnCancel();
}

void CCleanMemDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	//CDialog::OnOK();
}

void CCleanMemDlg::OnExit() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::KillTimer(this->m_hWnd, IDT_CLEANMEM);
	Shell_NotifyIcon(NIM_DELETE, &m_TrayIcon);
	TrayPop.DestroyMenu();
	CDialog::OnOK();
}

LRESULT CCleanMemDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if (message == WM_TRAYMOUSE_MSG && lParam == WM_RBUTTONDOWN && wParam == ID_CLEANMEM_TRAY)
	{
		POINT p;
		GetCursorPos(&p);
		SetForegroundWindow();
		TrayPop.TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this );
	}

	if (message == WM_TRAYMOUSE_MSG && lParam == WM_LBUTTONDBLCLK && wParam == ID_CLEANMEM_TRAY)
	{
		OnOption();
	}

	if (message == WM_TIMER && wParam == IDT_HIDEWINDOW)
	{
		ShowWindow(SW_HIDE);
		::KillTimer(this->m_hWnd, IDT_HIDEWINDOW);
	}

	if (message == WM_TIMER && wParam == IDT_CLEANMEM)
	{
		if (mCheckMemory() == TRUE)
		{
			OnCleanmem();
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CCleanMemDlg::mAdjustTokenPrivilegesForNT()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	
	// Get a token for this process. 
	
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	
	// Get the LUID for the EmptyWorkingSet privilege. 
	
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid); 
	
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	
	// Get the EmptyWorkingSet privilege for this process. 
	
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
}

BOOL CCleanMemDlg::mEmptyAllSet()
{
	HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(SnapShot==NULL)
	{
		return FALSE;
	}
	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize=sizeof(ProcessInfo);
	
	BOOL Status=Process32First(SnapShot, &ProcessInfo);
	while(Status)
	{
		HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,ProcessInfo.th32ProcessID);
		if(hProcess)
		{
			SetProcessWorkingSetSize(hProcess,0,0);
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		Status=Process32Next(SnapShot,&ProcessInfo);
	}
	return TRUE;
}

void CCleanMemDlg::mGetMemory(void)
{
	MEMORYSTATUSEX mMemEx;
	mMemEx.dwLength = sizeof (mMemEx);

	GlobalMemoryStatusEx(&mMemEx);

	m_pcMemory.SetRange32(0, int(mMemEx.ullTotalPhys / 1024 / 1024));
	m_pcMemory.SetPos(int(mMemEx.ullAvailPhys / 1024 / 1024));

	CString strPerMem;
	char buffer[200];

	DWORDLONG s = (mMemEx.ullAvailPhys / 1024 / 1024);
	sprintf(buffer, "%d ",s);

	strPerMem+=buffer;
	strPerMem.TrimRight();
	strPerMem+= " / ";

	s = (mMemEx.ullTotalPhys / 1024 / 1024);
	sprintf(buffer, "%d ",s);
	strPerMem+=buffer;
	strPerMem.TrimRight();
	strPerMem+=" MB ";

	s = (100 - mMemEx.dwMemoryLoad);
	sprintf(buffer, "%d ",s);
	strPerMem+=buffer;
	strPerMem.TrimRight();
	strPerMem+="% Free";

	m_PerMem.SetWindowText(strPerMem);

	strcpy(m_TrayIcon.szTip, strPerMem);
	strcpy(m_TrayIcon.szInfoTitle, strPerMem);
	m_TrayIcon.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	
	Shell_NotifyIcon(NIM_MODIFY, &m_TrayIcon);
}

LRESULT CCleanMemDlg::OnTaskBarCreated(WPARAM wp, LPARAM lp)
{
	m_TrayIcon.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	Shell_NotifyIcon(NIM_ADD, &m_TrayIcon);

	return 0;
}

void CCleanMemDlg::mGetAutoRunInfo()
{
	CRegExec creAutoRun;
	CRegExec::RegKeyInfo rkiRegInfo;
	CString RegData;
	CString RegValueName;
	unsigned long datalen;
	unsigned char querydata[255];
	unsigned long datatype = REG_SZ;
	
	rkiRegInfo.hKey = HKEY_CURRENT_USER;
	rkiRegInfo.lpSubKey = "software\\microsoft\\windows\\currentversion\\run";
	rkiRegInfo.lpType = &datatype;
	RegValueName = "CleanMem";
	datalen = 255;
	
	rkiRegInfo.lpQueryValueName = RegValueName.GetBuffer(8);
	RegValueName.ReleaseBuffer(); 
	rkiRegInfo.lpcbData = &datalen;
	rkiRegInfo.lpQueryData = querydata;
	
	if (creAutoRun.GetHotReg(rkiRegInfo) == false && creAutoRun.m_Ret != ERROR_INVALID_FUNCTION)
	{
		AfxMessageBox(creAutoRun.m_ErrInfo);
	}
	else
	{
		if (GetModuleFileName(NULL, RegData.GetBuffer(255), 255) == 0)
		{
			CString StrTmp;
			StrTmp.LoadString(IDS_GETAPPNAMEERR);
			AfxMessageBox(StrTmp);
			return;
		}
		RegData.ReleaseBuffer();
		if (rkiRegInfo.lpQueryData == RegData)
		{
			m_AutoRun.SetCheck(1);
		}
	}
}

void CCleanMemDlg::OnCleanmem() 
{
	mAdjustTokenPrivilegesForNT();
	mEmptyAllSet();
	
	mGetMemory();	
}

void CCleanMemDlg::OnOption() 
{
	ShowWindow(SW_SHOW);
	Sleep(1);
	ShowWindow(SW_RESTORE);
	CenterWindow();
}

void CCleanMemDlg::OnAutorun() 
{
	CRegExec creAutoRun;
	CRegExec::RegKeyInfo rkiRegInfo;
	CString RegData;
	CString RegValueName;
	CString buffer;
	
	buffer = "software\\microsoft\\windows\\currentversion\\run";
	
	RegValueName = "CleanMem";
	rkiRegInfo.dwType = REG_SZ;
	rkiRegInfo.cbData = 255;
	rkiRegInfo.lpSetValueName = RegValueName;
	rkiRegInfo.hKey = HKEY_CURRENT_USER;
	rkiRegInfo.lpSubKey = (LPCTSTR)buffer;
	
	if (m_AutoRun.GetCheck() == 1)
	{
		if (GetModuleFileName(NULL, RegData.GetBuffer(255), 255) == 0)
		{
			CString StrTmp;
			StrTmp.LoadString(IDS_GETAPPNAMEERR);
			AfxMessageBox(StrTmp);
			return;
		}
		RegData.ReleaseBuffer();
		rkiRegInfo.lpSetData = (CONST BYTE *)RegData.GetBuffer(255);
		RegData.ReleaseBuffer();
		
		if (creAutoRun.SetHotReg(rkiRegInfo) == false)
		{
			AfxMessageBox(creAutoRun.m_ErrInfo);
		}		
	}
	else
	{
		if (creAutoRun.SetHotReg(rkiRegInfo, true) == false)
		{
			AfxMessageBox(creAutoRun.m_ErrInfo);
		}		
	}	
}

BOOL CCleanMemDlg::mCheckMemory(void)
{
	MEMORYSTATUSEX mMemEx;
	mMemEx.dwLength = sizeof (mMemEx);
	unsigned long lCurPer;
	
	lCurPer = (m_Percent.GetCurSel() + 1) * 10;

	GlobalMemoryStatusEx(&mMemEx);
	
	if ((100 - mMemEx.dwMemoryLoad) <= lCurPer)
	{
		return TRUE;
	}

	return FALSE;
}

void CCleanMemDlg::mCreateMyPopMenu()
{
	CString StrTmp;
	
	TrayPop.CreatePopupMenu();
	StrTmp.LoadString(IDS_MENUCLEAN); 
	TrayPop.AppendMenu(MF_STRING, IDC_CLEANMEM, StrTmp);
	
	StrTmp.LoadString(IDS_MENUOPTION); 
	TrayPop.AppendMenu(MF_STRING, IDC_OPTION, StrTmp);

	TrayPop.AppendMenu(MF_SEPARATOR);
	StrTmp.LoadString(IDS_MENUEXIT); 
	TrayPop.AppendMenu(MF_STRING, IDC_EXIT, StrTmp);
	
	//TrayPop.SetDefaultItem(0,TRUE);
	
	//TrayPop.SetMenuDrawMode(TRUE);
	//TrayPop.SetBitmapBackground(RGB(0, 128, 128));
	//TrayPop.SetIconSize(16, 15);
	//TrayPop.LoadToolbar(IDR_TOOLBAR);
}

void CCleanMemDlg::mCreateTrayIcon()
{
	CString StrTmp;
	
	StrTmp.LoadString(IDS_TRAYINFOTITLE); 
	m_TrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_TrayIcon.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY);
	m_TrayIcon.hWnd = this->m_hWnd;
	strcpy(m_TrayIcon.szTip, StrTmp);
	strcpy(m_TrayIcon.szInfoTitle, StrTmp);
	m_TrayIcon.uID = ID_CLEANMEM_TRAY;
	m_TrayIcon.uCallbackMessage = WM_TRAYMOUSE_MSG;
	m_TrayIcon.uFlags = /*NIF_INFO | */NIF_ICON | NIF_TIP | NIF_MESSAGE;
	m_TrayIcon.uTimeout = 1000;
	m_TrayIcon.dwInfoFlags = NIIF_INFO;
	StrTmp.LoadString(IDS_TRAYINFO); 
	strcpy(m_TrayIcon.szInfo, StrTmp);
	
	Shell_NotifyIcon(NIM_ADD, &m_TrayIcon);

}

void CCleanMemDlg::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	ShowWindow(SW_HIDE);
	//CDialog::OnClose();
}

void CCleanMemDlg::OnSelchangePercent() 
{
	CRegExec creEnableApp;
	CRegExec::RegKeyInfo rkiRegInfo;
	CString RegData;
	CString RegValueName;
	char buffer[20];

	rkiRegInfo.hKey = HKEY_CURRENT_USER;
	rkiRegInfo.lpSubKey = "Software\\CleanMem";
	RegValueName = "MemPercent";
	rkiRegInfo.lpSetValueName = RegValueName;
	rkiRegInfo.dwType = REG_SZ;
	rkiRegInfo.cbData = 15;
	
	_itoa(m_Percent.GetCurSel(), buffer, 20);
	RegData = RegData + buffer;

	rkiRegInfo.lpSetData = (CONST BYTE *)RegData.GetBuffer(20);
	RegData.ReleaseBuffer();
	
	if (creEnableApp.SetHotReg(rkiRegInfo) == false)
		AfxMessageBox(creEnableApp.m_ErrInfo);

	m_CleanIt.SetFocus();
}

int CCleanMemDlg::mGetMemPercent()
{
	CRegExec creAutoRun;
	CRegExec::RegKeyInfo rkiRegInfo;
	CString RegData;
	CString RegValueName;
	unsigned long datalen;
	unsigned char querydata[255];
	unsigned long datatype = REG_SZ;
	
	rkiRegInfo.hKey = HKEY_CURRENT_USER;
	rkiRegInfo.lpSubKey = "Software\\CleanMem";
	rkiRegInfo.lpType = &datatype;
	RegValueName = "MemPercent";
	datalen = 255;
	
	rkiRegInfo.lpQueryValueName = RegValueName.GetBuffer(8);
	RegValueName.ReleaseBuffer(); 
	rkiRegInfo.lpcbData = &datalen;
	rkiRegInfo.lpQueryData = querydata;
	
	if (creAutoRun.GetHotReg(rkiRegInfo) == false && creAutoRun.m_Ret != ERROR_INVALID_FUNCTION)
	{
		AfxMessageBox(creAutoRun.m_ErrInfo);
		return 9;
	}
	else
	{
		if (creAutoRun.m_Ret == ERROR_INVALID_FUNCTION)
		{
			return 9;
		}

		RegData = querydata;
		return atoi(RegData.Left(1));
	}
}