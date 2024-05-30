// ViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Archive.h"
#include "ViewDlg.h"
#include "afxdialogex.h"
#include "KM.h"
#include "INTmysql.h"
// ViewDlg 对话框

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


// ViewDlg 消息处理程序


BOOL ViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	BackButtonFont.CreatePointFont(100,_T("宋体"));
	GetDlgItem(IDOK)->SetFont(&BackButtonFont);
	SetCaption();
	ManagerID.Format(_T("%d"),AdminID);//当前管理员ID(CString用于比较)
	AddEncList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void ViewDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
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

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_ORDER){
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH)); 
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void ViewDlg::AddEncList()
{
	//加密结果列表
	int admidlen = WideCharToMultiByte(CP_ACP,0,ManagerID,ManagerID.GetLength(),NULL,0,NULL,NULL);	//CString admid转char*
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
			AfxMessageBox(_T("无法正常连接数据库,请检查您的网络！"));
			delete []sql;
			sql=NULL;
			delete []cAdmID;
			cAdmID=NULL;
			TfromService.clear();
			return;
		}//检索service表失败
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
		}//连接service表成功
	}//数据库已注册
	else
	{
		AfxMessageBox(_T("无法连接到数据库，请先检查您的网络,再检查配置文件conf.txt填写是否有误！"));
		return;
	}
}

void ViewDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
	EncResult_list.InsertColumn(0,_T("封装用户"),LVCFMT_CENTER,60);
	EncResult_list.InsertColumn(1,_T("专业单位"),LVCFMT_CENTER,110);
	EncResult_list.InsertColumn(2,_T("文件名称"),LVCFMT_CENTER,150);
	EncResult_list.InsertColumn(3,_T("查看期限"),LVCFMT_CENTER,80);
	EncResult_list.InsertColumn(4,_T("原文消息摘要"),LVCFMT_CENTER,120);
	EncResult_list.InsertColumn(5,_T("封装时间"),LVCFMT_CENTER,120);
	EncResult_list.InsertColumn(6,_T("备注信息"),LVCFMT_CENTER,70);
}

void ViewDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
}

void ViewDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//AddEncList();
}
