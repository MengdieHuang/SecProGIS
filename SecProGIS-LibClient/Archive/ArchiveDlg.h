
// ArchiveDlg.h : 头文件
//

#pragma once

#include "afxwin.h"
#include "afxdtctl.h"
#include "sm4.h"	//sm4.h
#include "sm3.h"
#include "afxcmn.h"
#include "LoginDlg.h"
#include "AutoCombox1.h"

// CArchiveDlg 对话框
class CArchiveDlg : public CDialogEx
{
private://function
	void CArchiveDlg::AddOrgCombMenu();
	int CArchiveDlg::JudgeBeforeEncrypt();
// 构造
public:
	CString CArchiveDlg::GetPlainDir(CString pPath);
	CString CArchiveDlg::Show(CString ex_Path);
	CArchiveDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CArchiveDlg();	//析构函数
// 对话框数据
	enum { IDD = IDD_ARCHIVE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int PlainLen;
	int EncBlockSize;
	int OrgID;
	int AdminID;
	CString AdminName;
	LONG IfEncFlag;//"1"or"0"
	u8 ContentKey[16];
	buf_pointer buf_p;
	sm4_context ctx;
	_TABLE TableOrg;
	CFont ButtonFont;
	CFont NoteFont;
	CDateTimeCtrl m_dateCtrl;
	CTime login_date;
	CTime current_date;
	__time64_t m_date_64t ;
	__time64_t current_date_64t;
	CString Remark;
	CString PlainPath;
	CString PlainName;
	CString PlainExt;
	CString OrgName;
	CString CipherPath;
	CProgressCtrl myprog;
	//CComboBox ChoseOrg_box;
	CAutoCombox1 ChoseOrg_box;
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEncryptFile();
	afx_msg void OnReadFile();
	afx_msg void OnDecryptFile();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnStnClickedStaticpercent();
	afx_msg void OnBnClickedViewbutton();
	afx_msg void OnBnClickedButtonlogout();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonsavepath();
	afx_msg void OnStnClickedStatictitle();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonencrypt();

	CListBox PlainPath_list;
	CListBox CipherPath_list;
	CArray<CString,CString> PlainPath_ary;//存放多个明文完整路径的数组
	CArray<CString,CString> PlainName_ary;//存放多个明文名字（有后缀的）
	CArray<CString,CString> PlainExt_ary;//存放多个明文的格式类型
	CArray<CString,CString> CipherPath_ary;//存放多个密文完整路径的数组
	int SelFileNum;
	afx_msg void OnBnClickedStaticChosetitle();

	CBrush m_brush;
};
