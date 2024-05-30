// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Archive.h"
#include "ArchiveDlg.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "KM.h"
#include "INTmysql.h"
#include "md5.h"
#include "cJSON.h"
#include "base64.h"
#include <string.h>
#pragma comment(lib,"msimg32.lib")
using namespace std;
//#include <stdio.h>
/*
#if _MSC_VER
#define snprintf _snprintf_s
#endif*/

// LoginDlg �Ի���

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LoginDlg::IDD, pParent)
	, ManagerName(_T(""))
	, ManagerPassword(_T(""))
{
	m_hIcon2 = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITUSERNAME, ManagerName);
	DDX_Text(pDX, IDC_EDITPASSWORD, ManagerPassword);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONLOGIN, &LoginDlg::OnBnClickedButtonlogin)
	ON_EN_CHANGE(IDC_EDITUSERNAME, &LoginDlg::OnEnChangeEditusername)
	ON_EN_CHANGE(IDC_EDITPASSWORD, &LoginDlg::OnEnChangeEditpassword)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// LoginDlg ��Ϣ�������

BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���

	SetIcon(m_hIcon2, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon2, FALSE);		// ����Сͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	TitleFont.CreatePointFont(120,_T("΢���ź�"));
	GetDlgItem(IDC_LOGINTITLE)->SetFont(&TitleFont);
	GetDlgItem(IDC_STATIC_LINKNOTE)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH LoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if(pWnd->GetDlgCtrlID()==IDC_STATIC_LINKNOTE)
	{
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH)); 
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void LoginDlg::OnPaint()
{

	//CDialog::OnPaint();
	
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CDC mem;
	mem.CreateCompatibleDC(&dc);
	mem.SelectObject(mp);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&mem,0,0,SRCCOPY);
	

	CDC     memDC;
	//CRect   rect;
	//GetWindowRect(&rect);
	CBitmap	bitmap;
	BITMAP  bmp;
	bitmap.LoadBitmap(IDB_LOGIN);
	bitmap.GetObject(sizeof(BITMAP), &bmp);
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(bitmap);
	StretchBlt(dc, 0, 0, rc.Width(), rc.Height(), memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	/*if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon2);
	}
	else
	{
		CDialogEx::OnPaint();
	}*/
}

void LoginDlg::OnEnChangeEditusername()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	UpdateData(false);
}


void LoginDlg::OnEnChangeEditpassword()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	UpdateData(false);
}

void LoginDlg::OnBnClickedButtonlogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CheckAdmin(ManagerName,ManagerPassword);

}

void LoginDlg::CheckAdmin(CString adm,CString pas)
{
	ConfTxtPath=GetConfTxtPath();//��ȡconf.txt·��
	LinkDataBase(ConfTxtPath);//�������ݿ�
	if(!MY_QUERY::configured()==0)
	{
		int admlen = WideCharToMultiByte(CP_ACP,0,adm,adm.GetLength(),NULL,0,NULL,NULL);	//CString adminתchar*
		char* cAdmin=new char[admlen+1];
		memset(cAdmin,0x00,admlen+1);
		WideCharToMultiByte(CP_ACP,0,adm,adm.GetLength(),cAdmin,admlen,NULL,NULL);

		int paslen = WideCharToMultiByte(CP_ACP,0,pas,pas.GetLength(),NULL,0,NULL,NULL);	//CString pasתchar*
		char* cPas=new char[paslen+1];
		memset(cPas,0x00,paslen+1);
		WideCharToMultiByte(CP_ACP,0,pas,pas.GetLength(),cPas,paslen,NULL,NULL);
		
		//���Password��char*
		char* pass_in=new char[paslen];
		memset(pass_in,0x00,paslen);
		for(int i=0;i<paslen;i++)
		{
			pass_in[i]=cPas[i];
		}
		delete []cPas;
		cPas=NULL;

		//����Password MD5
		u8 pas_md5[16];
		memset(pas_md5,0x00,16);

		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx,pass_in,paslen);
		MD5_Final(pas_md5,&ctx);
		delete []pass_in;
		pass_in=NULL;
		TRACE("�û�����MD5:\n");
		for(int i=0;i<16;i++){
			TRACE("%02x",pas_md5[i]);
		}

		// *** use hexadecimal md5 ***
		char *md5_log = new char[33];
		memset(md5_log, '\0', 33);
		sprint_hex((char*)pas_md5,16, md5_log);
		// *** use hexadecimal md5 ***

		_TABLE TableAdmin;
		char *sql = new char[100];
		sprintf_s(sql, 100, "select * from admin where acc = '%s';",cAdmin);

		GetDlgItem(IDC_STATIC_LINKNOTE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_LINKNOTE)->SetWindowTextW(_T("�����������ݿ�...�����ĵȴ�"));
		if(MY_QUERY::exec_query_mysql(sql,TableAdmin)==-3)
		{
			GetDlgItem(IDC_STATIC_LINKNOTE)->ShowWindow(SW_HIDE);
			AfxMessageBox(_T("�޷����ӵ����ݿ⣬���ȼ���������磬��֤�����������ټ�鰲װ·���µ������ļ�conf.txt��д�Ƿ�����"));
			delete []sql;
			sql=NULL;
			delete []cAdmin;
			cAdmin=NULL;
			delete []md5_log;
			md5_log=NULL;
			TableAdmin.clear();
			return;
		}
		else
		{
			GetDlgItem(IDC_STATIC_LINKNOTE)->ShowWindow(SW_HIDE);
			if(TableAdmin.get_size()==0)
			{
				AfxMessageBox(_T("��������û����������������������룡"));
				delete []sql;
				sql=NULL;
				delete []cAdmin;
				cAdmin=NULL;
				delete []md5_log;
				md5_log=NULL;
				TableAdmin.clear();
				return;	
				
			}
			else
			{
				if(strcmp(TableAdmin[0][1].get_cont(),cAdmin)==0)//���ִ�Сд�Ƚ��û���
				{
					if(strcmp(TableAdmin[0][2].get_cont(),md5_log)==0)//CHECKͨ��
					{
						CString id;
						id=TableAdmin[0][0].get_cont();
						admid=_ttoi(id);
						delete []sql;
						sql=NULL;
						delete []cAdmin;
						cAdmin=NULL;
						delete []md5_log;
						md5_log=NULL;
						TableAdmin.clear();
						this->OnOK();//���ٵ�ǰ����
					}//������ȷ
					else
					{
						AfxMessageBox(_T("��������û����������������������룡"));
						delete []sql;
						sql=NULL;
						delete []cAdmin;
						cAdmin=NULL;
						delete []md5_log;
						md5_log=NULL;
						TableAdmin.clear();
						return;		
					}
				}
				else
				{
					AfxMessageBox(_T("��������û����������������������룡"));
					delete []sql;
					sql=NULL;
					delete []cAdmin;
					cAdmin=NULL;
					delete []md5_log;
					md5_log=NULL;
					TableAdmin.clear();
					return;	
				}
			}//�������ù���Ա����
		}//���ݿ����ӳɹ�
	}
	else
	{
		AfxMessageBox(_T("�޷����ӵ����ݿ⣬��⵽δ��д�����ļ���������дconf.txt�ļ���"));
		return;
	}
	/*
	if(!MY_QUERY::configured()==0)
	{
		int admlen = WideCharToMultiByte(CP_ACP,0,adm,adm.GetLength(),NULL,0,NULL,NULL);	//CString adminתchar*
		char* cAdmin=new char[admlen+1];
		memset(cAdmin,0x00,admlen+1);
		WideCharToMultiByte(CP_ACP,0,adm,adm.GetLength(),cAdmin,admlen,NULL,NULL);

		int paslen = WideCharToMultiByte(CP_ACP,0,pas,pas.GetLength(),NULL,0,NULL,NULL);	//CString pasתchar*
		char* cPas=new char[paslen+1];
		memset(cPas,0x00,paslen+1);
		WideCharToMultiByte(CP_ACP,0,pas,pas.GetLength(),cPas,paslen,NULL,NULL);
		
		//���Password��char*
		char* pass_in=new char[paslen];
		memset(pass_in,0x00,paslen);
		for(int i=0;i<paslen;i++)
		{
			pass_in[i]=cPas[i];
		}
		delete []cPas;
		cPas=NULL;

		//����Password MD5
		u8 pas_md5[16];
		memset(pas_md5,0x00,16);

		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx,pass_in,paslen);
		MD5_Final(pas_md5,&ctx);
		delete []pass_in;
		pass_in=NULL;
		TRACE("�û�����MD5:\n");
		for(int i=0;i<16;i++){
			TRACE("%02x",pas_md5[i]);
		}

		// *** use hexadecimal md5 ***
		char *md5_log = new char[33];
		memset(md5_log, '\0', 33);
		sprint_hex((char*)pas_md5,16, md5_log);
		// *** use hexadecimal md5 ***

		string base64_str;
		base64_str=base64_encode((unsigned char*)md5_log,strlen(md5_log));
		delete []md5_log;
		md5_log=NULL;

		//Create Json Message {name:str_name,password:base64_str}
		cJSON *pJsonRoot = NULL;
		pJsonRoot = cJSON_CreateObject();
		if(pJsonRoot == NULL)
		{
			cJSON_Delete(pJsonRoot);
			//printf("Create JSON Error");
			AfxMessageBox(_T("Create JSON Error"));
			return;
		}
		cJSON_AddStringToObject(pJsonRoot,"name",cAdmin);
		cJSON_AddStringToObject(pJsonRoot,"password",base64_str.c_str());
		if(pJsonRoot == NULL)
		{
			cJSON_Delete(pJsonRoot);
			//printf("Create JSON Error");
			AfxMessageBox(_T("Create JSON Error"));
			return;
		}
		char *pJsonPrintMessage = cJSON_Print(pJsonRoot);
		string pJosnStrForLogin = pJsonPrintMessage;

		//Create Socket
		int num ;
		SOCKET s;
		WSADATA wsa;
		struct sockaddr_in serv;
		WSAStartup(MAKEWORD(2,1),&wsa);
		if ((s = socket(AF_INET, SOCK_STREAM, 0))<0)  
		{  
			//printf("Create Socket Error");
			AfxMessageBox(_T("Create Socket Error"));
			return;
		}
		memset(&serv,0,sizeof(serv));  
		serv.sin_family = AF_INET;  
		serv.sin_port = htons(80);
		serv.sin_addr.S_un.S_addr = inet_addr("222.31.102.4");  
		if ((connect(s, (struct sockaddr *)&serv, sizeof(serv)))<0)  
		{  
		   //printf("Create Connect Error");
			AfxMessageBox(_T("��������ʧ�ܣ�������������"));
		   return;
		}  
		char sndBuf[2048] = {0};
		char rcvBuf[2048] = {0};
		memset(sndBuf,0,2048);
		memset(rcvBuf,0,2048);
	
		//Send message
		int MsgLen = strlen(pJsonPrintMessage)+strlen("LoginMsg=");
		_snprintf_s(sndBuf,sizeof(sndBuf)-1,
			"POST http://localhost/Mysite/Admin_login.php HTTP/1.1\n"
			//"POST http://localhost/Mysite/JsonTest1.php HTTP/1.1\r\n"
			"Host: 222.31.102.4\r\n"
			"Content-Type: application/x-www-form-urlencoded;charset=utf-8\r\n"//����   application/x-www-form-urlencoded
			"Content-Length: %d\r\n"
			"\r\n" 
			"LoginMsg=%s",
			 MsgLen,
			 pJsonPrintMessage
			);
		puts(sndBuf);
		if((num = send(s,sndBuf,2048,0))<0)
		{
		   //printf("Send Message Error");
			AfxMessageBox(_T("Send Message Error"));
		   return;
		}
		do
		{
			if((num = recv(s,rcvBuf,2048,0))<0)
			{
			  //printf("Recv Message Error");
				AfxMessageBox(_T("Recv Message Error"));
			  return;
			}
		} while (num > 0);
		closesocket(s);
		free(pJsonPrintMessage);
		cJSON_Delete(pJsonRoot);
		delete []cAdmin;
		cAdmin=NULL;

		//Parse Json
		string ReturnMsg;
		ReturnMsg = rcvBuf;
		string JsonMsg;
		string SubstrChar = "RetMsg";
		int nPos = (int)ReturnMsg.find(SubstrChar);
		if(nPos != string::npos)
		{
			JsonMsg = ReturnMsg.substr(nPos+6);
		}
		else 
		{
		   return;
		}
		cJSON *pJsonParse = cJSON_Parse(JsonMsg.c_str());
		if(pJsonParse == NULL)
		{
			cJSON_Delete(pJsonParse);
			AfxMessageBox(_T("Parse Json Error"));
			//printf("Parse Json Error");
			return;
		}
		cJSON *pSubParse = cJSON_GetObjectItem(pJsonParse,"Status");
		if(pSubParse == NULL)
		{
			//cJSON_Delete(pSubParse);
			//printf("Get Message Error");
			AfxMessageBox(_T("Get Message Error"));
			return;
		}
		string  ReturnLoginMsg = pSubParse->valuestring;
		if(!(ReturnLoginMsg.compare("login successful!")))
		{
		  //printf("LoginSuccess\n");
			//AfxMessageBox(_T("LoginSuccess\n"));
			
			cJSON *pSubParse = cJSON_GetObjectItem(pJsonParse,"IdMessage");
			if(pSubParse == NULL)
			{
				//cJSON_Delete(pSubParse);
				//printf("Get Message Error");
				AfxMessageBox(_T("Get Message Error"));
				return;
			}

			string  id = pSubParse->valuestring;
			admid=atoi(id.c_str());
			cJSON_Delete(pJsonParse);
			this->OnOK();//���ٵ�ǰ����
		}
		else
		{
			AfxMessageBox(_T("��������û����������������������룡"));
			cJSON_Delete(pJsonParse);
			return;
		}

	}
	else
	{
		AfxMessageBox(_T("�޷����ӵ����ݿ⣬���ȼ����������,�ټ�������ļ�conf.txt��д�Ƿ�����"));
		return;
	}*/
}

CString LoginDlg::GetConfTxtPath()
{
	//��ȡconf.txt·��
	TCHAR EXEFilePath[MAX_PATH];
	memset(EXEFilePath, '\0', MAX_PATH);
	GetModuleFileName(NULL,EXEFilePath,MAX_PATH);
	CString CurPath=EXEFilePath;
	int templen=CurPath.GetLength();
	for(int i=templen;i>-1;i--){
		if(CurPath[i]!='\\'){
			CurPath.Delete(i,1);
		}
		else{
			break;
		}
	}
	CString AllPath=CurPath+"conf.txt";
	return AllPath;
}

void LoginDlg::LinkDataBase(CString ConfPath)
{
	CString DBIpAddress;
	GetPrivateProfileString((CString)"DBConfigure",(CString)"ip",NULL,DBIpAddress.GetBufferSetLength(15),15,ConfPath);
	CString DBUser;
	GetPrivateProfileString((CString)"DBConfigure",(CString)"user",NULL,DBUser.GetBufferSetLength(15),15,ConfPath);
	CString DBPassword;
	GetPrivateProfileString((CString)"DBConfigure",(CString)"password",NULL,DBPassword.GetBufferSetLength(15),15,ConfPath);
	CString DBName;
	GetPrivateProfileString((CString)"DBConfigure",(CString)"dbname",NULL,DBName.GetBufferSetLength(15),15,ConfPath);
	
	//CStringתchar*
	int iplen = WideCharToMultiByte(CP_ACP,0,DBIpAddress,DBIpAddress.GetLength(),NULL,0,NULL,NULL);
	char* DataBaseIp=new char[iplen+1];
	memset(DataBaseIp,0x00,iplen+1);
	WideCharToMultiByte(CP_ACP,0,DBIpAddress,DBIpAddress.GetLength(),DataBaseIp,iplen,NULL,NULL);

	int userlen = WideCharToMultiByte(CP_ACP,0,DBUser,DBUser.GetLength(),NULL,0,NULL,NULL);
	char* DataBaseUser=new char[userlen+1];
	memset(DataBaseUser,0x00,userlen+1);
	WideCharToMultiByte(CP_ACP,0,DBUser,DBUser.GetLength(),DataBaseUser,userlen,NULL,NULL);

	int passwordlen = WideCharToMultiByte(CP_ACP,0,DBPassword,DBPassword.GetLength(),NULL,0,NULL,NULL);
	char* DataBasePassword=new char[passwordlen+1];
	memset(DataBasePassword,0x00,passwordlen+1);
	WideCharToMultiByte(CP_ACP,0,DBPassword,DBPassword.GetLength(),DataBasePassword,passwordlen,NULL,NULL);

	int dbnamelen = WideCharToMultiByte(CP_ACP,0,DBName,DBName.GetLength(),NULL,0,NULL,NULL);
	char* DataBaseName=new char[dbnamelen+1];
	memset(DataBaseName,0x00,dbnamelen+1);
	WideCharToMultiByte(CP_ACP,0,DBName,DBName.GetLength(),DataBaseName,dbnamelen,NULL,NULL);
	
	MY_QUERY::set_conf(DataBaseIp, 3306, DataBaseUser, DataBasePassword, DataBaseName);

	DBIpAddress.ReleaseBuffer();
	DBUser.ReleaseBuffer();
	DBPassword.ReleaseBuffer();
	DBName.ReleaseBuffer();
	delete []DataBaseIp;
	DataBaseIp=NULL;
	delete []DataBaseUser;
	DataBaseUser=NULL;
	delete []DataBasePassword;
	DataBasePassword=NULL;
	delete []DataBaseName;
	DataBaseName=NULL;
}


