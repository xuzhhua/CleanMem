// CleanMem.h : CLEANMEM �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_CLEANMEM_H__B0433D2C_E4FF_4F28_BF51_D4720C6D4AA7__INCLUDED_)
#define AFX_CLEANMEM_H__B0433D2C_E4FF_4F28_BF51_D4720C6D4AA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CCleanMemApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� CleanMem.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CCleanMemApp : public CWinApp
{
public:
	CCleanMemApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCleanMemApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CCleanMemApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CLEANMEM_H__B0433D2C_E4FF_4F28_BF51_D4720C6D4AA7__INCLUDED_)
