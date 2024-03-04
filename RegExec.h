/*
*****************************************************
类功能：向系统及注册表注册、修改、删除、取得热键信息
作者：徐仲华

修改纪录：
    2002.12.17  建立
	2003.03.09  由于定时功能不需要注册热键，所以将注
	            册表操作类作为基类

*****************************************************
*/
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRegExec  //向注册表添加/取得信息的类
{
public:
	CRegExec();
	~CRegExec();
	struct RegKeyInfo //热键信息的结构、用于取得注册表内已设置的信息
	{
		//RegOpenKey/RegCreateKey/RegDeleteKey/RegCloseKey使用
		HKEY    hKey;             // 要打开的根Key句柄
		LPCTSTR lpSubKey;         // 要打开的SubKey的字符串
		PHKEY   phkResult;        // 打开键的句柄指针(不要赋值)
		HKEY    hKeyResult;       // 打开键的句柄(不要赋值)
		//RegQueryValueEx使用
		LPTSTR  lpQueryValueName; // 数据名的字符串(RegQueryValueEx用)
		LPDWORD lpType;           // 所取值类型(RegQueryValueEx用)
		LPBYTE  lpQueryData;      // 数据缓冲区(RegQueryValueEx用)
		LPDWORD lpcbData;         // 数据缓冲区大小(RegQueryValueEx用)
		//RegSetValueEx使用
		LPCTSTR lpSetValueName;   // 数据名的字符串(RegSetValueEx用)
		DWORD   dwType;           // 设置的数据类型(RegSetValueEx使用)
		CONST   BYTE *lpSetData;  // 数据地址(RegSetValueEx用)
		DWORD   cbData;           // 数据大小(RegSetValueEx用)
	};
	//全体使用
	CString m_ErrInfo;            // 若函数执行失败，返回错误信息
	long m_Ret;                   // 若函数执行失败，返回代码
	long m_ErrPos;                // 若函数执行失败，返回错误位置
	bool GetHotReg(RegKeyInfo &GetRegKeyInfo); //取得注册表信息
	bool SetHotReg(RegKeyInfo &SetRegKeyInfo, bool IsDelete = false); //设置注册表信息
	void ErrMsgGet(CString &ErrMsg, long ErrCD, long ErrPos);  //取得并显示错误信息
};
