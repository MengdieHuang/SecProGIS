#include "stdafx.h"
#include <Windows.h>
#include "BaseFunc.h"

using namespace std;
using namespace BaseFunc;

unsigned BaseFunc::selFile( string &strFile,const string &strExt,bool bOpen )
{
    string strDir = "D:\\Downloads";//这里通过strFile解析目录,CFileDialog会自动记住
    string filename = "hi.txt";  //通过strFile解析文件名
    string filter = strExt + "文件 (*." + strExt + ")|*." + strExt + "||";
    string ext = "." + strExt;

    CFileDialog dlg(bOpen,ext.c_str(),filename.c_str(),OFN_READONLY|OFN_OVERWRITEPROMPT,filter.c_str(),NULL);
    dlg.GetOFN().lpstrInitialDir = strFile.c_str();// 默认目录

    if (dlg.DoModal())
    {
        strFile = dlg.GetPathName();
        return IDOK;
    }

    return IDCANCEL;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
        break;
    }

    return 0;
}

unsigned BaseFunc::selDir( string &strDir,void *hwnd/* = NULL*/ )
{
    char szDir[MAX_PATH];
    BROWSEINFO bi; 
    bi.hwndOwner = (HWND)hwnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = (LPWSTR)szDir;
    bi.lpszTitle = _T("选择位置：");
    bi.iImage = 0;

    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = (LPARAM)(LPCTSTR)strDir.c_str();

    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if (lp && SHGetPathFromIDList(lp,(LPWSTR)szDir))
    {
        strDir = szDir;
        return IDOK;
    }

    return IDCANCEL;
}