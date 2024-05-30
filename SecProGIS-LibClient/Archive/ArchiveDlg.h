
// ArchiveDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"
#include "afxdtctl.h"
#include "sm4.h"	//sm4.h
#include "sm3.h"
#include "afxcmn.h"
#include "LoginDlg.h"
#include "AutoCombox1.h"

// CArchiveDlg �Ի���
class CArchiveDlg : public CDialogEx
{
private://function
	void CArchiveDlg::AddOrgCombMenu();
	int CArchiveDlg::JudgeBeforeEncrypt();
// ����
public:
	CString CArchiveDlg::GetPlainDir(CString pPath);
	CString CArchiveDlg::Show(CString ex_Path);
	CArchiveDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CArchiveDlg();	//��������
// �Ի�������
	enum { IDD = IDD_ARCHIVE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CArray<CString,CString> PlainPath_ary;//��Ŷ����������·��������
	CArray<CString,CString> PlainName_ary;//��Ŷ���������֣��к�׺�ģ�
	CArray<CString,CString> PlainExt_ary;//��Ŷ�����ĵĸ�ʽ����
	CArray<CString,CString> CipherPath_ary;//��Ŷ����������·��������
	int SelFileNum;
	afx_msg void OnBnClickedStaticChosetitle();

	CBrush m_brush;
};
