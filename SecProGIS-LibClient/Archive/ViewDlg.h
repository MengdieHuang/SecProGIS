#pragma once
#include <afxcmn.h>
#include"ListCtrlSort.h"

// ViewDlg �Ի���

class ViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ViewDlg)

private://function
	void ViewDlg::SetCaption();
	void ViewDlg::AddEncList();
	CString ViewDlg::SecondToClock(__time64_t sec);
	CString ViewDlg::SecondToDate(__time64_t sec);
public:
	ViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ViewDlg();

// �Ի�������
	enum { IDD = IDD_VIEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl EncResult_list;
	ListCtrlSort EncResult_list;
	int AdminID;
	CFont BackButtonFont;
	CString ManagerID;
	CString ManagerName;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1();
};
