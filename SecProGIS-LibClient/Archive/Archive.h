
// Archive.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CArchiveApp:
// �йش����ʵ�֣������ Archive.cpp
//

class CArchiveApp : public CWinApp
{

private://function
	//CString CArchiveApp::GetConfTxtPath();
	//void CArchiveApp::LinkDataBase(CString ConfPath);
public:
	CArchiveApp();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
public:
	//CString ConfTxtPath;
	
	DECLARE_MESSAGE_MAP()

};

extern CArchiveApp theApp;