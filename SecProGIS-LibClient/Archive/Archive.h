
// Archive.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CArchiveApp:
// 有关此类的实现，请参阅 Archive.cpp
//

class CArchiveApp : public CWinApp
{

private://function
	//CString CArchiveApp::GetConfTxtPath();
	//void CArchiveApp::LinkDataBase(CString ConfPath);
public:
	CArchiveApp();
// 重写
public:
	virtual BOOL InitInstance();

// 实现
public:
	//CString ConfTxtPath;
	
	DECLARE_MESSAGE_MAP()

};

extern CArchiveApp theApp;