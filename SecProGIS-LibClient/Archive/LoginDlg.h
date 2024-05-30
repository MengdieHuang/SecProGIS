#pragma once

// LoginDlg �Ի���

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

private://function
	void LoginDlg::CheckAdmin(CString adm,CString pas);
	CBitmap mp;
	CString LoginDlg::GetConfTxtPath();
	void LoginDlg::LinkDataBase(CString ConfPath);
public:
	LoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	HICON m_hIcon2;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int admid;
	afx_msg void OnBnClickedButtonlogin();
	virtual BOOL OnInitDialog();
	CString ManagerName;
	CString ManagerPassword;
	CFont TitleFont;
	CFont ETitleFont;
	afx_msg void OnEnChangeEditusername();
	afx_msg void OnEnChangeEditpassword();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedLogintitle();
	afx_msg void OnStnClickedStaticPic();
	CString ConfTxtPath;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
