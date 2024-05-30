// AutoCombox1.cpp : implementation file  
//  
  
#include "stdafx.h"  
// #include "MyCombox.h"  
#include "AutoCombox1.h"  
  
  
// CAutoCombox1  
  
IMPLEMENT_DYNAMIC(CAutoCombox1, CComboBox)  
  
CAutoCombox1::CAutoCombox1()  
{  
    m_pEdit = NULL;  
    m_nFlag = 0;  
}  
  
CAutoCombox1::~CAutoCombox1()  
{  
    if (m_pEdit)  
    {  
        if (::IsWindow(m_hWnd))  
        {  
            m_pEdit->UnsubclassWindow();  
        }  
        delete m_pEdit;  
        m_pEdit = NULL;  
    }  
}  
  
  
BEGIN_MESSAGE_MAP(CAutoCombox1, CComboBox)  
END_MESSAGE_MAP()  
  
  
  
// CAutoCombox1 message handlers  
  
  
//自动选择最匹配的，如果没有，则不选择。////////////////////////////////  
void CAutoCombox1::AutoSelect()  
{  
    // Make sure we can 'talk' to the edit control  
    if ( m_pEdit == NULL )    
    {  
        m_pEdit = new CEdit();  
        m_pEdit->SubclassWindow(GetDlgItem(1001)->GetSafeHwnd());  
    }  
  
    // Save the state of the edit control  
    CString strText;            //取得输入字符串  
    int nStart = 0, nEnd = 0;   //取得光标位置  
    m_pEdit->GetWindowText(strText);  
    m_pEdit->GetSel(nStart, nEnd);  
  
    // Perform actual completion  
    int nBestIndex = -1;        //是否能找到匹配的字符  
    int nBestFrom  = INT_MAX;   //匹配开始的字符  
  
    if (!strText.IsEmpty())  
    {  
        for ( int nIndex=0; nIndex<GetCount(); ++nIndex )  
        {  
            CString str;  
            GetLBText(nIndex,str);  
  
            int nFrom = str.Find(strText);  
  
            if ( nFrom != -1 && nFrom < nBestFrom )//能匹配，而且是更好的匹配，才记录  
            {  
                nBestIndex = nIndex;  
                nBestFrom  = nFrom;  
            }  
        }//for  
    }  
  
    //Set select index  
    if (!GetDroppedState())  
    {  
        ShowDropDown(TRUE);  
        m_pEdit->SetWindowText(strText);  
        m_pEdit->SetSel(nStart, nEnd);  
    }  
  
    if ( GetCurSel() != nBestIndex )  
    {  
        // Select the matching entry in the list  
        SetCurSel(nBestIndex);  
  
        // Restore the edit control  
        m_pEdit->SetWindowText(strText);  
        m_pEdit->SetSel(nStart, nEnd);  
    }  
}  
  
//删除不匹配的，自动选择剩余中最匹配的，如果没有，则显示全部。//////////////  
void CAutoCombox1::AutoMatchAndSel()  
{  
    // Make sure we can 'talk' to the edit control  
    if ( m_pEdit == NULL )    
    {  
        m_pEdit = new CEdit();  
        m_pEdit->SubclassWindow(GetDlgItem(1001)->GetSafeHwnd());  
    }  
  
    // 保存edit控件的状态  
    CString strText;            //取得输入字符串  
    int nStart = 0, nEnd = 0;   //取得光标位置  
    m_pEdit->GetWindowText(strText);  
    m_pEdit->GetSel(nStart, nEnd);  
  
    //清空CComboBox里面的数据  
    CComboBox::ResetContent();  
  
    // 重新填充列表，并选择最合适的  
    int nBestIndex = -1;        //是否能找到匹配的字符  
    int nBestFrom  = INT_MAX;   //匹配开始的字符  
  
    if (!strText.IsEmpty())  
    {  
        for ( int nIndex=0; nIndex<m_strArr.GetSize(); ++nIndex )  
        {  
            int nFrom = m_strArr[nIndex].Find(strText);  
            char kk = m_strArr[nIndex].GetAt(0);  
            char jj = strText.GetAt(0);  
            BOOL flag = FALSE;  
            if (kk==jj)  
            {  
                flag = TRUE;  
            }  
            if ( nFrom != -1&&flag==TRUE)//能匹配  
            {  
                int n = CComboBox::AddString(m_strArr[nIndex]);  
  
                if (nFrom < nBestFrom)//更好的匹配，则记录  
                {  
                    nBestIndex = n;  
                    nBestFrom  = nFrom;  
                }  
            }  
        }//for  
    }  
  
    if (GetCount() == 0)    //没有的显示所有  
    {  
        for (int nIndex=0; nIndex<m_strArr.GetSize(); ++nIndex)  
        {  
            CComboBox::AddString(m_strArr[nIndex]);  
        }  
    }  
  
    //显示下拉列表  
    if (!GetDroppedState())  
    {  
        ShowDropDown(TRUE);  
    }  
  
    //设置选择项  
    // Select and Restore the edit control  
    SetCurSel(nBestIndex);  
    m_pEdit->SetWindowText(strText);  
    m_pEdit->SetSel(nStart, nEnd);  
}  
// manipulating listbox items  
int  CAutoCombox1::AddString(LPCTSTR lpszString)  
{  
    m_strArr.Add(lpszString);  
    return CComboBox::AddString(lpszString);  
}  
int  CAutoCombox1::DeleteString(UINT nIndex)  
{  
    m_strArr.RemoveAt(nIndex);  
    return CComboBox::DeleteString(nIndex);  
}  
int  CAutoCombox1::InsertString(int nIndex, LPCTSTR lpszString)  
{  
    m_strArr.InsertAt(nIndex, lpszString);  
    return CComboBox::InsertString(nIndex, lpszString);  
}  
void  CAutoCombox1::ResetContent()  
{  
    m_strArr.RemoveAll();  
    CComboBox::ResetContent();  
}  
  
//All Message Handle Dispatch  
BOOL CAutoCombox1::OnCommand(WPARAM wParam, LPARAM lParam)  
{  
    if ( HIWORD(wParam) == EN_CHANGE )  
    {     
        if (m_nFlag & 0x01)  
        {  
            AutoMatchAndSel();  
        }  
        else  
        {  
            AutoSelect();  
        }  
        return true;  
    }  
    else  
    {  
        return CComboBox::OnCommand(wParam, lParam);  
    }  
}  