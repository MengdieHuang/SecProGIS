#include "stdafx.h"
#include <Windows.h>
#include "BaseFunc.h"

using namespace std;
using namespace BaseFunc;

unsigned BaseFunc::selFile( string &strFile,const string &strExt,bool bOpen )
{
    string strDir = "D:\\Downloads";//����ͨ��strFile����Ŀ¼,CFileDialog���Զ���ס
    string filename = "hi.txt";  //ͨ��strFile�����ļ���
    string filter = strExt + "�ļ� (*." + strExt + ")|*." + strExt + "||";
    string ext = "." + strExt;

    CFileDialog dlg(bOpen,ext.c_str(),filename.c_str(),OFN_READONLY|OFN_OVERWRITEPROMPT,filter.c_str(),NULL);
    dlg.GetOFN().lpstrInitialDir = strFile.c_str();// Ĭ��Ŀ¼

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
    bi.lpszTitle = _T("ѡ��λ�ã�");
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