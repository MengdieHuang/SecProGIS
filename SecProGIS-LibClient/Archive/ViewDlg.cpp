// ViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Archive.h"
#include "ViewDlg.h"
#include "afxdialogex.h"
#include "KM.h"
#include "INTmysql.h"
// ViewDlg �Ի���

IMPLEMENT_DYNAMIC(ViewDlg, CDialogEx)

ViewDlg::ViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ViewDlg::IDD, pParent)
{

}

ViewDlg::~ViewDlg()
{
}

void ViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, EncResult_list);
}


BEGIN_MESSAGE_MAP(ViewDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ViewDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &ViewDlg::OnLvnItemchangedList1)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &ViewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// ViewDlg ��Ϣ�������


BOOL ViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	BackButtonFont.CreatePointFont(100,_T("����"));
	GetDlgItem(IDOK)->SetFont(&BackButtonFont);
	SetCaption();
	ManagerID.Format(_T("%d"),AdminID);//��ǰ����ԱID(CString���ڱȽ�)
	AddEncList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void ViewDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CBitmap mp;
	CDC mem;
	mem.CreateCompatibleDC(&dc);
	mem.SelectObject(mp);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&mem,0,0,SRCCOPY);

	CDC     memDC;
	CBitmap	bitmap;
	BITMAP  bmp;
	bitmap.LoadBitmap(IDB_BMP_BK);
	bitmap.GetObject(sizeof(BITMAP), &bmp);
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(bitmap);
	StretchBlt(dc, 0, 0, rc.Width(), rc.Height(), memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	CBitmap titlebmp;
	titlebmp.LoadBitmap(IDB_BMP_TITLE);
	CDC titleDC;
	titleDC.CreateCompatibleDC(&dc);
	CBitmap *pOldbmp;
	pOldbmp=titleDC.SelectObject(&titlebmp);
	TransparentBlt(dc.m_hDC,0,0,741,72,titleDC.m_hDC,0,0,740,71,RGB(255,255,255));
	titleDC.SelectObject(pOldbmp);
}

HBRUSH ViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_ORDER){
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH)); 
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void ViewDlg::AddEncList()
{
	//���ܽ���б�
	int admidlen = WideCharToMultiByte(CP_ACP,0,ManagerID,ManagerID.GetLength(),NULL,0,NULL,NULL);	//CString admidתchar*
	char* cAdmID=new char[admidlen+1];
	memset(cAdmID,0x00,admidlen+1);
	WideCharToMultiByte(CP_ACP,0,ManagerID,ManagerID.GetLength(),cAdmID,admidlen,NULL,NULL);

	if(!MY_QUERY::configured()==0)
	{
		_TABLE TfromService;
		char *sql = new char[400];
		//sprintf_s(sql,400, "select * from service where admin = '%s' order by enc_tm desc;" ,cAdmID);
		sprintf_s(sql,400, "select service.md5_p, service.expire, service.enc_tm, service.fname, service.rmrk, org.name from service inner join org on service.org=org.id where service.admin = '%s' order by service.enc_tm desc;" ,cAdmID);
		if(MY_QUERY::exec_query_mysql(sql,TfromService)==-3)
		{
			AfxMessageBox(_T("�޷������������ݿ�,�����������磡"));
			delete []sql;
			sql=NULL;
			delete []cAdmID;
			cAdmID=NULL;
			TfromService.clear();
			return;
		}//����service��ʧ��
		else
		{
			CString OrgName;
			CString PlainMd5;
			CString Deadline;
			CString deadt;
			CString EncTime;
			CString enct;
			CString FileName;
			CString Remark;
			int RoeLen=TfromService.get_size();
			
			for(int i=0;i<RoeLen;i++)
			{
				PlainMd5=TfromService[i][0].get_cont();
				Deadline=TfromService[i][1].get_cont();
				deadt=SecondToDate((__time64_t)_ttoi(Deadline));
				EncTime=TfromService[i][2].get_cont();
				enct=SecondToClock((__time64_t)_ttoi(EncTime));
				FileName=TfromService[i][3].get_cont();
				Remark=TfromService[i][4].get_cont();
				OrgName=TfromService[i][5].get_cont();
				EncResult_list.InsertItem(i,ManagerName);
				EncResult_list.SetItemText(i,1,OrgName);
				EncResult_list.SetItemText(i,2,FileName);
				EncResult_list.SetItemText(i,3,deadt);
				EncResult_list.SetItemText(i,4,PlainMd5);
				EncResult_list.SetItemText(i,5,enct);
				EncResult_list.SetItemText(i,6,Remark);

			}
			TfromService.clear();
		}//����service��ɹ�
	}//���ݿ���ע��
	else
	{
		AfxMessageBox(_T("�޷����ӵ����ݿ⣬���ȼ����������,�ټ�������ļ�conf.txt��д�Ƿ�����"));
		return;
	}
}

void ViewDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EncResult_list.DeleteAllItems();
	CDialogEx::OnOK();
}

CString ViewDlg::SecondToClock(__time64_t sec)
{
	COleDateTime time=(COleDateTime)sec;
	CString clock;
	clock=time.Format(_T("%Y-%m-%d %H:%M:%S"));
	return clock;
}
CString ViewDlg::SecondToDate(__time64_t sec)
{
	CTime time(sec);
	CString date;
	date.Format(_T("%04d-%02d-%02d"),time.GetYear(),time.GetMonth(),time.GetDay());
	return date;
}

void ViewDlg::SetCaption()
{
	ListView_SetExtendedListViewStyleEx(EncResult_list.GetSafeHwnd(), LVS_EX_FULLROWSELECT , LVS_EX_FULLROWSELECT);
	EncResult_list.InsertColumn(0,_T("��װ�û�"),LVCFMT_CENTER,60);
	EncResult_list.InsertColumn(1,_T("רҵ��λ"),LVCFMT_CENTER,110);
	EncResult_list.InsertColumn(2,_T("�ļ�����"),LVCFMT_CENTER,150);
	EncResult_list.InsertColumn(3,_T("�鿴����"),LVCFMT_CENTER,80);
	EncResult_list.InsertColumn(4,_T("ԭ����ϢժҪ"),LVCFMT_CENTER,120);
	EncResult_list.InsertColumn(5,_T("��װʱ��"),LVCFMT_CENTER,120);
	EncResult_list.InsertColumn(6,_T("��ע��Ϣ"),LVCFMT_CENTER,70);
}

void ViewDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	
}

void ViewDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//AddEncList();
}
