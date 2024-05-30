
// ArchiveDlg.cpp : 实现文件

#include "stdafx.h"
#include "Archive.h"
#include "ArchiveDlg.h"
#include "afxdialogex.h"
#include "sm4.h"
#include "sm3.h"
#include <iostream>
#include <math.h>
#include "KM.h"
#include "INTmysql.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include <Windows.h>
#include "ViewDlg.h"
#pragma comment(lib,"msimg32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CArchiveDlg 对话框

CArchiveDlg::CArchiveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArchiveDlg::IDD, pParent)
	, PlainPath(_T(""))
	, OrgName(_T(""))
	, CipherPath(_T(""))
	, Remark(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CArchiveDlg::~CArchiveDlg()//析构函数
{
	TableOrg.clear();
	TRACE("析构函数已调用\n");
}

void CArchiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	DDX_Control(pDX, IDC_PROGRESS1, myprog);
	DDX_Control(pDX, IDC_COMBO1, ChoseOrg_box);
	DDX_CBString(pDX, IDC_COMBO1, OrgName);
	DDX_Text(pDX, IDC_EDIT3, Remark);
	DDX_Control(pDX, IDC_LIST1, PlainPath_list);
	DDX_Control(pDX, IDC_LIST2, CipherPath_list);
}

BEGIN_MESSAGE_MAP(CArchiveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CArchiveDlg::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON1, &CArchiveDlg::OnEncryptFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CArchiveDlg::OnReadFile)
	ON_BN_CLICKED(IDC_BUTTON3, &CArchiveDlg::OnDecryptFile)
	
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CArchiveDlg::OnCbnSelchangeCombo1)
	ON_STN_CLICKED(IDC_STATICPERCENT, &CArchiveDlg::OnStnClickedStaticpercent)
	ON_BN_CLICKED(IDC_VIEWBUTTON, &CArchiveDlg::OnBnClickedViewbutton)
	ON_BN_CLICKED(IDC_BUTTONSAVEPATH, &CArchiveDlg::OnBnClickedButtonsavepath)
	ON_EN_CHANGE(IDC_EDIT3, &CArchiveDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CArchiveDlg 消息处理程序

BOOL CArchiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	login_date=CTime::GetCurrentTime();
	m_date_64t = -1;
	EncBlockSize=102400;
	myprog.SetRange(0,100);
	myprog.SetPos(0);

	ButtonFont.CreatePointFont(120,_T("宋体"));
	GetDlgItem(IDC_BUTTONENCRYPT)->SetFont(&ButtonFont);
	GetDlgItem(IDC_VIEWBUTTON)->SetFont(&ButtonFont);
	NoteFont.CreatePointFont(80,_T("黑体"));
	GetDlgItem(IDC_STATIC_DEDNOTE)->SetFont(&NoteFont);
	
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(_T("无"));

	AddOrgCombMenu();//配置专业单位下拉列表
	GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICPERCENT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATICADMINNAME)->SetWindowTextW(AdminName);
	GetDlgItem(IDC_STATICDATESHOW)->SetWindowTextW(login_date.Format("%Y-%m-%d-%X"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

HBRUSH CArchiveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_ACCOUNT
		||pWnd->GetDlgCtrlID()==IDC_STATIC_LOGT
		||pWnd->GetDlgCtrlID()==IDC_STATICADMINNAME
		||pWnd->GetDlgCtrlID()==IDC_STATICDATESHOW
		//||pWnd->GetDlgCtrlID()==IDC_STATIC_CHOSETITLE
		//||pWnd->GetDlgCtrlID()==IDC_STATIC_SETTITLE
		//||pWnd->GetDlgCtrlID()==IDC_STATIC_OUTTITLE
		||pWnd->GetDlgCtrlID()==IDC_STATIC_SC
		||pWnd->GetDlgCtrlID()==IDC_STATIC_SD
		||pWnd->GetDlgCtrlID()==IDC_STATIC_SR
		//||pWnd->GetDlgCtrlID()==IDC_STATIC_DEDNOTE
		)
	{
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH)); 
	}
	if(pWnd->GetDlgCtrlID()==IDC_STATIC_DEDNOTE)
	{
		pDC->SetTextColor(RGB(98,98,98));//设置文本颜色为灰色
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH)); 
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CArchiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CArchiveDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);
	CBitmap mp;
	CDC mem;
	mem.CreateCompatibleDC(&dc);
	mem.SelectObject(mp);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&mem,0,0,SRCCOPY);
	
	//设置Page2、page3底图
	CDC     memDC;
	CBitmap	bitmap;
	BITMAP  bmp;
	bitmap.LoadBitmap(IDB_BMP_BK);
	bitmap.GetObject(sizeof(BITMAP), &bmp);
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(bitmap);
	StretchBlt(dc, 0, 0, rc.Width(), rc.Height(), memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	
	//设置Page2、page3标题
	CBitmap titlebmp;
	titlebmp.LoadBitmap(IDB_BMP_TITLE);
	CDC titleDC;
	titleDC.CreateCompatibleDC(&dc);
	CBitmap *pOldbmp;
	pOldbmp=titleDC.SelectObject(&titlebmp);
	TransparentBlt(dc.m_hDC,0,0,741,72,titleDC.m_hDC,0,0,740,71,RGB(255,255,255));
	titleDC.SelectObject(pOldbmp);

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CArchiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CArchiveDlg::OnReadFile()
{
	// TODO: 在此添加控件通知处理程序代码
	//清空文件路径变量
	SetDlgItemText(IDC_COMBO_ORG,NULL);
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(_T("无"));
	PlainPath_list.ResetContent();
	PlainPath_ary.RemoveAll();
	PlainName_ary.RemoveAll();
	PlainExt_ary.RemoveAll();
	CipherPath_list.ResetContent();
	CipherPath_ary.RemoveAll();
	m_dateCtrl.SetTime(&CTime::GetCurrentTime());
	
	//选取文件
	UpdateData(TRUE);
	CFileDialog CScanFileDlg(TRUE,NULL,NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,
	_T("（*.shp,*.dwg,*.mdb）|*.shp;*.dwg;*.mdb|.shp(*.shp)|*.shp|.dwg(*.dwg)|*.dwg|.mdb(*.mdb)|*.mdb|All Files(*.*)|*.*||"), AfxGetMainWnd());
	
	//同时选取多个文件
	CString strPath=_T("");
	CString strName=_T("");
	CString strExt=_T("");
	CScanFileDlg.m_ofn.nMaxFile=100*MAX_PATH;
	TCHAR *lsf = new TCHAR[CScanFileDlg.m_ofn.nMaxFile];
	CScanFileDlg.m_ofn.lpstrFile=lsf;
	ZeroMemory(CScanFileDlg.m_ofn.lpstrFile,sizeof(TCHAR) * CScanFileDlg.m_ofn.nMaxFile);//对内存块清零
	if(CScanFileDlg.DoModal() == IDOK)
	{
		POSITION m_Position;
		m_Position=CScanFileDlg.GetStartPosition();
		while(m_Position!=NULL)
		{
			strPath=CScanFileDlg.GetNextPathName(m_Position);
			PlainPath_ary.Add(strPath);
			PlainPath_list.InsertString(PlainPath_list.GetCount(),strPath);
			int len=strPath.GetLength();
			for(int i=len-1;i>0;i--)
			{
				if(strPath.GetAt(i)=='.')
				{
					strExt=strPath.Right(len-1-i);
				}
				else
				{
					if(strPath.GetAt(i)=='\\')//判断当前字符是否是‘\’
					{
						strName=strPath.Right(len-1-i);
						break;
					}
				}
			}
			PlainExt_ary.Add(strExt);
			PlainName_ary.Add(strName);
		}
	}
	delete lsf;
 	SelFileNum=PlainPath_list.GetCount();	
}

void CArchiveDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index=0;
	index=ChoseOrg_box.GetCurSel();
	ChoseOrg_box.GetLBText(index,OrgName);
	//ChoseOrg_box.GetWindowTextW(OrgName);
	int orgnamelen = WideCharToMultiByte(CP_ACP,0,OrgName,OrgName.GetLength(),NULL,0,NULL,NULL);	//CString OrgName转char*
	char* cOrgname=new char[orgnamelen+1];
	memset(cOrgname,0x00,orgnamelen+1);
	WideCharToMultiByte(CP_ACP,0,OrgName,OrgName.GetLength(),cOrgname,orgnamelen,NULL,NULL);

	_TABLE orgID;
	char *sql = new char[100];
		sprintf_s(sql,100, "select * from org where name = '%s';" ,cOrgname);
	if(MY_QUERY::exec_query_mysql(sql,orgID)==-3)
	{
		AfxMessageBox(_T("无法正常连接数据库,请检查您的网络！"));
		delete []sql;
		sql=NULL;
		delete []cOrgname;
		cOrgname=NULL;
		orgID.clear();
		return;
	}//检索service表失败
	else
	{
		int row=orgID.get_size();
		if(row>0)
		{
			CString corg;
			corg=orgID[0][0].get_cont();
			OrgID=_ttoi(corg);	//获取OrgID
			delete []sql;
			sql=NULL;
			delete []cOrgname;
			cOrgname=NULL;
			orgID.clear();
		}
		else
		{
			AfxMessageBox(_T("你选择的单位不在列表中，请重新选择！"));
			delete []sql;
			sql=NULL;
			delete []cOrgname;
			cOrgname=NULL;
			orgID.clear();
			return;	
		}
	}
}

void CArchiveDlg::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	CTime m_date;//!日期参数
	m_dateCtrl.GetTime(m_date);
	m_date_64t=m_date.GetTime();
	
	current_date=CTime::GetCurrentTime();
	current_date_64t=current_date.GetTime();
	if(current_date_64t >= m_date_64t){
		//AfxMessageBox(_T("请选择今日之后的日期作为文件失效期！"));
		m_dateCtrl.SetTime(&CTime::GetCurrentTime());
		return;
	}
}

void  CArchiveDlg::OnEncryptFile(){	//加密按钮
	if(!JudgeBeforeEncrypt()==1)
	{
		return;
	}
	for(int findex=0;findex<SelFileNum;findex++)//每个文件单独加密
	{
		UpdateData(true);
		CFile PlainFile;
		PlainFile.Open(PlainPath_ary[findex],CFile::typeBinary,NULL);		
		PlainLen=(int)PlainFile.GetLength();


		GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATICPERCENT)->ShowWindow(SW_SHOW);
		myprog.SetPos(1);
		GetDlgItem(IDC_STATICPERCENT)->SetWindowTextW(_T("正在生成文件摘要"));

		//生成明文MD5
		buf_p.receive=new BYTE[PlainLen]; //receive
		//BYTE *buf_p.
		memset(buf_p.receive,0x00,PlainLen);
		PlainFile.Read(buf_p.receive,PlainLen);
		u8 plain_md5[32];
		memset(plain_md5,0x00,32);
		sm3(buf_p.receive,PlainLen,plain_md5);
		TRACE("加密前明文MD5:\n");
		for(int i=0;i<32;i++){
			TRACE("%02x",plain_md5[i]);
		}
		delete[] buf_p.receive;
		buf_p.receive=NULL;
		PlainFile.Close();

		//获取密钥ContentKey[16]
		// *** use hexadecimal md5 ***
		char *md5_p = new char[66];
		memset(md5_p, '\0', 66);
		sprint_hex((char*)plain_md5, 32, md5_p);
		// *** use hexadecimal md5 ***

		//写入表设置
		if(!KM_SERV::configured()==1)
		{
			KM_SERV::set_conf("service",AdminID);
			//KM_SERV::set_admin(AdminID);//管理员
		}
	
		KM_SERV *serv = new KM_SERV(OrgID, KM_WORD((char*)md5_p, 64), (time_t)m_date_64t);

		delete []md5_p;
		md5_p=NULL;
		serv->gen_key(16);
		serv->get_skey((char*)ContentKey);
		TRACE("密钥:\n");
		for(int i=0;i<16;i++){
			TRACE("%02x",ContentKey[i]);
		}

		//写入加密标志位和截止时间
		PlainFile.Open(PlainPath_ary[findex],CFile::typeBinary,NULL);
		CFile CipherFile(CipherPath_ary[findex],CFile::modeCreate|CFile::modeReadWrite);
		IfEncFlag=1;
		CipherFile.Write(&IfEncFlag,4);
		CipherFile.Write(&m_date_64t,8);//写完Header Part1
		CipherFile.Seek(32,CFile::current);
		//CipherFile.Write(&cipher_md5,32);

		//边读边加密边写入新文件
		buf_p.receive=new BYTE[EncBlockSize];
		memset(buf_p.receive,0x00,EncBlockSize);
		buf_p.output=new BYTE[EncBlockSize];
		memset(buf_p.output,0x00,EncBlockSize);
		int BlockReadsize,CurrentPos;
		int TotalReadNum=0;
		CString percent,BaiFenHao,JinDustr,Pstr;
		JinDustr="文件加密已完成 ";
		BaiFenHao="%";
		while(true){
			BlockReadsize=PlainFile.Read(buf_p.receive,EncBlockSize);
			if(BlockReadsize>0){
				sm4_setkey_enc(&ctx,ContentKey);
				sm_crypt_ecb_16(&ctx,1,BlockReadsize,buf_p.receive,buf_p.output);
				CipherFile.Write(buf_p.output,BlockReadsize);
				CipherFile.Flush();
				TotalReadNum=TotalReadNum+BlockReadsize;
			}
			else{
				TotalReadNum=TotalReadNum+BlockReadsize;
				delete[] buf_p.receive;
				buf_p.receive=NULL;
				delete[] buf_p.output;
				buf_p.output=NULL;
				break;
			}
			if(TotalReadNum%(EncBlockSize*10)==0){
				CurrentPos=(int)ceil(((double)TotalReadNum/PlainLen)*100);
				if(CurrentPos!=100){
					myprog.SetPos(CurrentPos);
					percent.Format(_T("%d"),CurrentPos);
					Pstr=JinDustr+percent+BaiFenHao;
					GetDlgItem(IDC_STATICPERCENT)->SetWindowTextW(Pstr);
				}	
			}
		}
		//CipherFile.Close();
		PlainFile.Close();
	
		//写入密文md5
		int CipherLen=(int)CipherFile.GetLength()-44;
		buf_p.output=new BYTE[CipherLen];
		memset(buf_p.output,0x00,CipherLen);
		CipherFile.Seek(44,CFile::begin);
		CipherFile.Read(buf_p.output,CipherLen);
		u8 cipher_md5[32];
		memset(cipher_md5,0x00,32);
		sm3(buf_p.output,CipherLen,cipher_md5);
		TRACE("密文MD5:");
		for(int i=0;i<32;i++){
			TRACE("%02x",cipher_md5[i]);
		}
		delete[] buf_p.output;
		buf_p.output=NULL;
		CipherFile.Seek(12,CFile::begin);
		CipherFile.Write(&cipher_md5,32);//密文md5写入Header part2
		CipherFile.Flush();
		CipherFile.Close();

		// *** use hexadecimal md5 ***
		char *md5_c = new char[66];
		memset(md5_c, '\0', 66);
		sprint_hex((char*)cipher_md5, 32, md5_c);
		KM_WORD *md5c=new KM_WORD((char*)md5_c,64);
		// *** use hexadecimal md5 ***

		current_date=CTime::GetCurrentTime();
		current_date_64t=current_date.GetTime();
		if(serv->store(*md5c,(time_t)current_date_64t)==-1)//存数据失败
		{
			AfxMessageBox(_T("数据库连接失败，请检查您的网络是否断开,确认网络连接好后，再重新加密！"));
			myprog.SetPos(0);
			GetDlgItem(IDC_STATICPERCENT)->SetWindowTextW(SW_HIDE);
			GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
			delete md5c;
			delete serv;
			return;
		}
		else
		{
			delete md5c;
			delete serv;
		}
	
		//CString 转char* 
		int fnamelen = WideCharToMultiByte(CP_ACP,0,PlainName_ary[findex],PlainName_ary[findex].GetLength(),NULL,0,NULL,NULL);	//CString PlainName转char*
		char* fname=new char[fnamelen+1];
		memset(fname,0x00,fnamelen+1);
		WideCharToMultiByte(CP_ACP,0,PlainName_ary[findex],PlainName_ary[findex].GetLength(),fname,fnamelen,NULL,NULL);
	
		UpdateData(true);
		int remarklen = WideCharToMultiByte(CP_ACP,0,Remark,Remark.GetLength(),NULL,0,NULL,NULL);	//CString Remark转char*
		char* remark=new char[remarklen+1];
		memset(remark,0x00,remarklen+1);
		WideCharToMultiByte(CP_ACP,0,Remark,Remark.GetLength(),remark,remarklen,NULL,NULL);

		char *sql = new char[300];
		sprintf_s(sql, 300, "update service set fname = '%s', rmrk = '%s' where md5_c = '%s';", fname,remark,md5_c);
		if(MY_QUERY::exec_query_mysql(sql)==-3)
		{
			AfxMessageBox(_T("数据库连接失败，请检查您的网络是否断开,确认网络连接好后，再重新加密！"));
			myprog.SetPos(0);
			GetDlgItem(IDC_STATICPERCENT)->SetWindowTextW(SW_HIDE);
			GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
			delete []md5_c;
			md5_c=NULL;
			delete []sql;
			sql=NULL;
			delete []fname;
			fname=NULL;
			delete []remark;
			remark=NULL;
			return;
		}
		else
		{
			delete []md5_c;
			md5_c=NULL;
			delete []sql;
			sql=NULL;
			delete []fname;
			fname=NULL;
			delete []remark;
			remark=NULL;

			CurrentPos=100;
			myprog.SetPos(CurrentPos);
			percent.Format(_T("%d"),CurrentPos);
			Pstr=JinDustr+percent+BaiFenHao;
			GetDlgItem(IDC_STATICPERCENT)->SetWindowTextW(Pstr);
			/*CString note,ss;
			note=PlainName_ary[findex];
			ss="文件封装完成！";
			note=note+ss;
			MessageBox(note,
				_T("档案馆客户端SecProGIS-LibClient"),MB_ICONINFORMATION|MB_OK);*/
		}
	}
	CString ss;
	//note=PlainName_ary[findex];
	ss="文件封装完成！";
	//note=note+ss;
	MessageBox(ss,
		_T("档案馆客户端SecProGIS-LibClient"),MB_ICONINFORMATION|MB_OK);
}

void CArchiveDlg::OnDecryptFile()	//解密按钮
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFile CipherFile;
	CipherFile.Open(CipherPath,CFile::typeBinary,NULL);//CipherPath是在加密时获得的
	CString str;
	CString CipherName;
	str="(解密)";
	CipherName = str+PlainName;
	CFileDialog CSaveFileDlg(FALSE,NULL,CipherName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T(".shp(*.shp)|*.shp|.dwg(*.dwg)|*.dwg|.mdb(*.mdb)|*.mdb||All Files(*.*)|*.*||"), this);
	CSaveFileDlg.m_ofn.lpstrTitle = _T("解密后的文件另存为");
	CString DecryptFilePath;
	if(CSaveFileDlg.DoModal() == IDOK)
	{
		DecryptFilePath=CSaveFileDlg.GetPathName();
	}
	CFile DecryptFile(DecryptFilePath,CFile::modeCreate|CFile::modeReadWrite);
	
	buf_p.receive=new BYTE[PlainLen];
	memset(buf_p.receive,0x00,PlainLen);
	buf_p.output=new BYTE[PlainLen];
	memset(buf_p.output,0x00,PlainLen);

	char *ifenc=new char[2];
	memset(ifenc,0,2);
	char *deadline=new char[9];
	memset(deadline,0,9);
	CipherFile.Read(ifenc,1);
	CipherFile.Read(deadline,8);
	if(ifenc[0]==IfEncFlag){
		CipherFile.Read(buf_p.receive,PlainLen);
		sm4_setkey_dec(&ctx,ContentKey);
		sm_crypt_ecb_16(&ctx,0,PlainLen,buf_p.receive,buf_p.output);	//整块解密
		DecryptFile.Write(buf_p.output,PlainLen);
		DecryptFile.Flush();
		CipherFile.Close();
		DecryptFile.Close();
	}
	delete []ifenc;
	ifenc=NULL;
	delete []deadline;
	deadline=NULL;

	//计算解密后文件的MD5
	DecryptFile.Open(DecryptFilePath,CFile::typeBinary,NULL);
	buf_p.output=new BYTE[PlainLen];
	memset(buf_p.output,0x00,PlainLen);
	DecryptFile.Read(buf_p.output,PlainLen);
	u8 decryptfile_md5[32];
	memset(decryptfile_md5,0x00,32);
	sm3(buf_p.output,PlainLen,decryptfile_md5);
	DecryptFile.Close();
	
	TRACE("解密后明文MD5:");
	for(int i=0;i<32;i++){
		TRACE("%02x",decryptfile_md5[i]);
	}
	delete[] buf_p.receive;
	buf_p.receive=NULL;
	delete[] buf_p.output;
	buf_p.output=NULL;
	GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICPERCENT)->ShowWindow(SW_HIDE);

}

void CArchiveDlg::AddOrgCombMenu()
{
	//专业单位可选列表
	if(!MY_QUERY::configured()==0)
	{
		if(MY_QUERY::exec_query_mysql("select * from org;", TableOrg)==-3)
		{
			AfxMessageBox(_T("配置文件conf.txt填写有误,无法正常连接数据库！"));
			return;
		}
		else
		{
			CString option;
			for(int i=0;i<TableOrg.get_size();i++){
				option=TableOrg[i][1].get_cont();
				ChoseOrg_box.AddString(option);
			}
		}
	}

}
int CArchiveDlg::JudgeBeforeEncrypt()
{
	if(PlainPath_ary.IsEmpty())
	{
		AfxMessageBox(_T("请先选择要加密的文件！"));
		return false;
	}
	ChoseOrg_box.GetWindowTextW(OrgName);
	if(OrgName.IsEmpty())
	{
		AfxMessageBox(_T("请选择查看文件的专业单位"));
		return false;
	}
	int orgnamelen = WideCharToMultiByte(CP_ACP,0,OrgName,OrgName.GetLength(),NULL,0,NULL,NULL);	//CString OrgName转char*
	char* cOrgname=new char[orgnamelen+1];
	memset(cOrgname,0x00,orgnamelen+1);
	WideCharToMultiByte(CP_ACP,0,OrgName,OrgName.GetLength(),cOrgname,orgnamelen,NULL,NULL);

	_TABLE orgID;
	char *sql = new char[100];
		sprintf_s(sql,100, "select * from org where name = '%s';" ,cOrgname);
	if(MY_QUERY::exec_query_mysql(sql,orgID)==-3)
	{
		AfxMessageBox(_T("无法正常连接数据库,请检查您的网络！"));
		delete []sql;
		sql=NULL;
		delete []cOrgname;
		cOrgname=NULL;
		orgID.clear();
		return false;
	}//检索service表失败
	else
	{
		int row=orgID.get_size();
		if(row>0)
		{
			CString corg;
			corg=orgID[0][0].get_cont();
			OrgID=_ttoi(corg);	//获取OrgID
			delete []sql;
			sql=NULL;
			delete []cOrgname;
			cOrgname=NULL;
			orgID.clear();
		}
		else
		{
			AfxMessageBox(_T("你选择的单位不在列表中，请重新选择！"));
			delete []sql;
			sql=NULL;
			delete []cOrgname;
			cOrgname=NULL;
			orgID.clear();
			return false;	
		}
	}
	if(m_date_64t == -1)
	{
		AfxMessageBox(_T("请选择文件查看截止日期！"));
		return false;
	}
	else if(current_date_64t >= m_date_64t)
	{
		AfxMessageBox(_T("请选择当前日期之后的时间作为截止日期！"));
		return false;
	}
	else if(CipherPath_ary.IsEmpty())
	{
		AfxMessageBox(_T("请选择加密文件的存储路径！"));
		return false;
	}
	else
	{
		return true;
	}
}

void CArchiveDlg::OnStnClickedStaticpercent()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CArchiveDlg::OnBnClickedViewbutton()
{
	// TODO: 在此添加控件通知处理程序代码
	ViewDlg viewdlg;
	viewdlg.AdminID=AdminID;
	viewdlg.ManagerName=AdminName;
	if(viewdlg.DoModal()==IDOK)
	{
		
	}
}

void CArchiveDlg::OnBnClickedButtonsavepath()
{
	// TODO: 在此添加控件通知处理程序代码
	if(PlainPath_ary.IsEmpty())
	{
		AfxMessageBox(_T("请先选择要加密的文件！"));
		return;
	}

	CipherPath_list.ResetContent();
	CipherPath_ary.RemoveAll();

	//选择加密后的文件存储目录
	if(SelFileNum==1)	//加密一个文件
	{
		//获取PlainPath_list中的文件明文路径，并存入数组中
		CString BuChongEnc;
		BuChongEnc="(加密)";
		CString CipherName;//密文文件名（含后缀）
		CipherName = BuChongEnc+PlainName_ary[0];

		CString CipherExtSet=_T("");
		CString CipherExt1,CipherExt2,CipherExt3,CipherExt4;
		CipherExt1=".";
		CipherExt2="(*.";
		CipherExt3=")";
		CipherExtSet=CipherExt1+PlainExt_ary[0]+CipherExt2+PlainExt_ary[0]+CipherExt3;//密文文件格式，与明文保持一致
		//CipherExtSet.ReleaseBuffer();

		CFileDialog CSaveFileDlg(FALSE,NULL,CipherName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,this);
		CSaveFileDlg.m_ofn.lpstrFilter = CipherExtSet;
		CipherExtSet.ReleaseBuffer();
		CSaveFileDlg.m_ofn.lpstrTitle = _T("加密后的文件另存为");
		CString strPath;
		if(CSaveFileDlg.DoModal() == IDOK)
		{
			strPath=CSaveFileDlg.GetPathName();
			CipherPath_ary.Add(strPath);
			CipherPath_list.InsertString(CipherPath_list.GetCount(),strPath);
			//UpdateData(false);
		}
	}
	else	//加密多个文件
	{
		CString p0Path,cPath,pDir,cDir;
		PlainPath_list.GetText(0,p0Path);
		pDir=GetPlainDir(p0Path);
		cDir=Show(pDir);
		CString BuChongEnc,str;

		str="\\";
		BuChongEnc="(加密)";
		CString CipherName;//密文文件名（含后缀）
		for(int findex=0;findex<SelFileNum;findex++)
		{
			//自动生成密文路径
			CipherName=BuChongEnc+PlainName_ary[findex];
			cPath=cDir+str+CipherName;
			CipherPath_ary.Add(cPath);
			CipherPath_list.InsertString(CipherPath_list.GetCount(),cPath);
		}	
	}
}


void CArchiveDlg::OnEnChangeEdit3()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}

static int CALLBACK _SHBrowseForFolderCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )  
{  
    switch(uMsg)  
    {  
    case BFFM_INITIALIZED:      
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);   //传递默认打开路径 break;  
    case BFFM_SELCHANGED:    //选择路径变化，BFFM_SELCHANGED  
        {  
            char curr[MAX_PATH];     
            SHGetPathFromIDList((LPCITEMIDLIST)lParam,(LPWSTR)(LPCTSTR)curr);     
            ::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)&curr[0]);    
        }  
        break;  
    default:  
        break;  
    }   
    return 0;  
}

CString CArchiveDlg::Show(CString ex_Path)  
{  
        TCHAR           szFolderPath[MAX_PATH] = {0};  
        CString         strFolderPath = TEXT("");  
		TCHAR szPath[MAX_PATH] ;//初始化时浏览文件夹的路径  
		_tcscpy_s(szPath,ex_Path);//自己设置的文件夹路径  

        BROWSEINFO      sInfo;  
        ::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
        sInfo.pidlRoot = 0;  
        sInfo.lpszTitle= _T("请选择一个文件夹存储加密后的文件：");  
        sInfo.ulFlags  = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;  
        sInfo.lpfn     = _SHBrowseForFolderCallbackProc;  
		sInfo.lParam   =LPARAM(szPath);    //设置默认路径
        // 显示文件夹选择对话框  
        LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
        if (lpidlBrowse != NULL)  
        {  
            // 取得文件夹名  
            if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
            {  
                strFolderPath = szFolderPath;  
            }  
        }  
        if(lpidlBrowse != NULL)  
        {  
            ::CoTaskMemFree(lpidlBrowse);  
        }  
        return strFolderPath;  
 }  

CString CArchiveDlg::GetPlainDir(CString pPath)
{
	//获取明文所在文件夹目录
	int templen=pPath.GetLength();
	CString str;
	for(int i=templen-1;i>0;i--)
	{
		if(pPath.GetAt(i)=='\\')//判断当前字符是否是‘\’
		{
			str=pPath.Left(i);	
			return str;
			break;
		}
	}
}