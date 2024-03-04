//*****************************************************
//�๦�ܣ���ϵͳ��ע���ע�ᡢ�޸ġ�ɾ����ȡ���ȼ���Ϣ
//
//����ʱ�䣺2002.12.17
//
//���ߣ����ٻ�
//*****************************************************

#include "stdafx.h"
#include "RegExec.h"
#include "resource.h"

CRegExec::CRegExec()
{
	m_ErrInfo.Empty();
	m_ErrPos = 0;
	m_Ret = 0;
}

CRegExec::~CRegExec()
{
	m_ErrInfo.Empty();
}

bool CRegExec::GetHotReg(CRegExec::RegKeyInfo &GetRegKeyInfo) //ȡ��ע�����Ϣ
{
	m_ErrPos = 0;
	m_Ret = 0;

	GetRegKeyInfo.phkResult = &GetRegKeyInfo.hKeyResult; //����佨����Ӧ��ϵ
	if (m_Ret = RegOpenKey(GetRegKeyInfo.hKey, GetRegKeyInfo.lpSubKey, GetRegKeyInfo.phkResult) != ERROR_SUCCESS)
	{
		m_ErrPos = 1;
		ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
		return false;
	}
	else
	{
		if (m_Ret = RegQueryValueEx(GetRegKeyInfo.hKeyResult, GetRegKeyInfo.lpQueryValueName, NULL, GetRegKeyInfo.lpType, GetRegKeyInfo.lpQueryData, GetRegKeyInfo.lpcbData) != ERROR_SUCCESS)
		{
			m_ErrPos = 2;
			ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
			return false;
		}
		else if (m_Ret = RegCloseKey(GetRegKeyInfo.hKeyResult) != ERROR_SUCCESS)
		{
			m_ErrPos = 3;
			ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
			return false;
		}
	}
	
	return true;
}

bool CRegExec::SetHotReg(CRegExec::RegKeyInfo &SetRegKeyInfo, bool IsDelete) //����ע�����Ϣ
{
	m_ErrPos = 0;
	m_Ret = 0;

	SetRegKeyInfo.phkResult = &SetRegKeyInfo.hKeyResult; //����佨����Ӧ��ϵ
	if (RegOpenKey(SetRegKeyInfo.hKey, SetRegKeyInfo.lpSubKey, SetRegKeyInfo.phkResult) != ERROR_SUCCESS) //��ע���؁E�
	{
		if (IsDelete == true)
			return true;

		if (m_Ret = RegCreateKey(SetRegKeyInfo.hKey, SetRegKeyInfo.lpSubKey, SetRegKeyInfo.phkResult) != ERROR_SUCCESS) //�½�ע���؁E�
		{
			m_ErrPos = 4;
			ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
			return false;
		}
		else if (m_Ret = RegOpenKey(SetRegKeyInfo.hKey, SetRegKeyInfo.lpSubKey, SetRegKeyInfo.phkResult) != ERROR_SUCCESS) //��ע���؁E�
		{
			m_ErrPos = 5;
			ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
			return false;
		}
	}

	if (IsDelete == false)
	{
		if (m_Ret = RegSetValueEx(SetRegKeyInfo.hKeyResult, SetRegKeyInfo.lpSetValueName, 0, SetRegKeyInfo.dwType, SetRegKeyInfo.lpSetData, SetRegKeyInfo.cbData) != ERROR_SUCCESS) //����ע���؁E�������
		{
			m_ErrPos = 6;
			ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
			return false;
		}
	}
	else
	{
		if (m_Ret = RegDeleteValue(SetRegKeyInfo.hKeyResult, SetRegKeyInfo.lpSetValueName) != ERROR_SUCCESS) //ɾ����E�����
		{
			m_ErrPos = 7;
			ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
			return false;
		}
	}
	
	if (m_Ret = RegCloseKey(SetRegKeyInfo.hKeyResult) != ERROR_SUCCESS) //�ر�ע���؁E�
	{
		m_ErrPos = 8;
		ErrMsgGet(m_ErrInfo, m_Ret, m_ErrPos);
		return false;
	}
	return true;
}

void CRegExec::ErrMsgGet(CString &ErrMsg, long ErrCD, long ErrPos)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrCD,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	char chErrCD[20];
	CString resErrPos;
	resErrPos.LoadString(IDS_ERRMSGPOS);

	ErrMsg.LoadString(IDS_ERRMSGCODE); 
	ErrMsg += _itoa(ErrCD, chErrCD, 10);
	ErrMsg += "\n";
	ErrMsg += (LPCTSTR)lpMsgBuf;
	ErrMsg += resErrPos;
	ErrMsg += _itoa(ErrPos, chErrCD, 10);
	// Free the buffer.
	//MessageBox(NULL, ErrMsg, "�������E, MB_OK | MB_ICONINFORMATION);
	LocalFree(lpMsgBuf);
}