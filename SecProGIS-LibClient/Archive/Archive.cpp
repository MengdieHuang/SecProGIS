
// Archive.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Archive.h"
#include "ArchiveDlg.h"
#include "LoginDlg.h"
#include "ViewDlg.h"
#include "INTmysql.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArchiveApp

BEGIN_MESSAGE_MAP(CArchiveApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CArchiveApp ����

CArchiveApp::CArchiveApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CArchiveApp ����

CArchiveApp theApp;


// CArchiveApp ��ʼ��

BOOL CArchiveApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//ConfTxtPath=GetConfTxtPath();//��ȡconf.txt·��
	//LinkDataBase(ConfTxtPath);//�������ݿ�

	//�ж�������
	//__time64_t today_64t;
	__time64_t endtime_64t=2543507200;//201801101ʧЧ
	CTime TodayTime=CTime::GetCurrentTime();
	CTime EndTime(endtime_64t);
	CTimeSpan span=EndTime-TodayTime;
	int iDay=span.GetDays();
	//today_64t=TodayTime.GetTime();
	if(iDay<=0)
	{
		AfxMessageBox(_T("SecProGIS-LibCLient V1.5�汾ʹ�������ѵ�"));
		return FALSE;
	}
	else if(iDay<=7&&iDay>0)//2018��7��4��ʧЧ
	{
		CString str1=_T("SecProGIS-LibClient V1.5�汾ʹ��ʱ�仹ʣ");
		CString str2;
		str2.Format(_T("%d"),iDay);
		CString str3=_T("��");
		AfxMessageBox(str1+str2+str3);
	}
	else
	{
	
	}
	CArchiveDlg dlg;
	LoginDlg logindlg;
	INT_PTR mResponse=logindlg.DoModal();
	dlg.AdminID=logindlg.admid;
	dlg.AdminName=logindlg.ManagerName;

	if(mResponse==IDCANCEL)
	{
		return FALSE;
	
	}
	//dlg.AdminID=logindlg.admid;
	//dlg.AdminName=logindlg.ManagerName;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���

	}
	else if (nResponse == IDCANCEL)
	{

	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
/*
CString CArchiveApp::GetConfTxtPath()
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

void CArchiveApp::LinkDataBase(CString ConfPath)
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
}*/
