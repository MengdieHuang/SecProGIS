
// Archive.cpp : 定义应用程序的类行为。
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


// CArchiveApp 构造

CArchiveApp::CArchiveApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CArchiveApp 对象

CArchiveApp theApp;


// CArchiveApp 初始化

BOOL CArchiveApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//ConfTxtPath=GetConfTxtPath();//获取conf.txt路径
	//LinkDataBase(ConfTxtPath);//连接数据库

	//判断试用期
	//__time64_t today_64t;
	__time64_t endtime_64t=2543507200;//201801101失效
	CTime TodayTime=CTime::GetCurrentTime();
	CTime EndTime(endtime_64t);
	CTimeSpan span=EndTime-TodayTime;
	int iDay=span.GetDays();
	//today_64t=TodayTime.GetTime();
	if(iDay<=0)
	{
		AfxMessageBox(_T("SecProGIS-LibCLient V1.5版本使用期限已到"));
		return FALSE;
	}
	else if(iDay<=7&&iDay>0)//2018年7月4日失效
	{
		CString str1=_T("SecProGIS-LibClient V1.5版本使用时间还剩");
		CString str2;
		str2.Format(_T("%d"),iDay);
		CString str3=_T("天");
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
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码

	}
	else if (nResponse == IDCANCEL)
	{

	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
/*
CString CArchiveApp::GetConfTxtPath()
{
	//获取conf.txt路径
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
	
	//CString转char*
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
