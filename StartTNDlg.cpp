// StartTNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StartTN.h"
#include "StartTNDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStartTNDlg dialog

CStartTNDlg::CStartTNDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStartTNDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStartTNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStartTNDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CStartTNDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CStartTNDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CStartTNDlg message handlers

BOOL CStartTNDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

#ifdef WIN32_PLATFORM_WFSP
	if (!m_dlgCommandBar.Create(this) ||
	    !m_dlgCommandBar.InsertMenuBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create CommandBar\n");
		return FALSE;      // fail to create
	}
#endif // WIN32_PLATFORM_WFSP
	// TODO: Add extra initialization here
	
	
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CStartTNDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_STARTTN_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_STARTTN_DIALOG));
	}
}
#endif


BOOL BrowseDirectory(TCHAR *title,TCHAR *fileDir)  
{  
	TCHAR dir[MAX_PATH];
	ZeroMemory(&dir,MAX_PATH);

	BROWSEINFO bi;  
	bi.hwndOwner      = NULL;  
	bi.pidlRoot       = NULL;  
	bi.pszDisplayName = dir;  
	bi.lpszTitle      = title;  
	bi.ulFlags        = 0;  
	bi.lpfn           = NULL;  
	bi.lParam         = NULL;  
	bi.iImage         = NULL; 

	if (!SHGetPathFromIDList(SHBrowseForFolder(&bi),dir))
	{
		return (FALSE);
	}

	lstrcpy(fileDir,dir);

	return (TRUE);  
}

void CStartTNDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	PROCESS_INFORMATION processInfo;

	const int nBufSize = 512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);
	BOOL bGetPath = GetModuleFileName(NULL,chBuf,nBufSize);
	TCHAR *p = wcsrchr(chBuf,'\\');
	*p = 0;
	CString strNEWPath   =   chBuf   +   CString(_T( "\\NEventWatcher.exe")); 
	CString	strHMIPath = chBuf +   CString(_T( "\\TestMode.exe")); 
	///< Current directory is the CL dirctory it's ok 
	int iCmp = _tcscmp(chBuf,TEXT("\\SDMMC\\CL"));

	_tprintf(TEXT("\r\n ++++++ StartMap: Path is : %s +++++\r\n"),chBuf);
	if (!CreateProcess(strNEWPath, NULL, NULL, NULL, NULL
		, CREATE_NEW_CONSOLE
		, NULL, NULL, NULL, &processInfo))
	{
		DWORD dwCPError = GetLastError();
		printf("++++++++++ StartTestMode: NEventWathcer failed in current directory error code is %d +++++++++\r\n",dwCPError);		

		/* Cannot found the file-NeventWatcher in current directory then goto default directory */		
		if (2 == dwCPError){				
			if (!CreateProcess(_T("\\SDMMC\\CL\\NEventWatcher.exe"), NULL, NULL, NULL, NULL
				, CREATE_NEW_CONSOLE
				, NULL, NULL, NULL, &processInfo))
			{				
				printf("++++++++++ StartTestMode:NEventWathcer failed in default directory Last error code is %d +++++++++\r\n",GetLastError());		
				return ;
			}
			printf("++++++++++ StartTestMode: NEventWathcer started in default directory +++++++++\r\n");

			CloseHandle(processInfo.hThread);
			CloseHandle(processInfo.hProcess);

			Sleep(1000);

			if (!CreateProcess(_T("\\SDMMC\\CL\\TestMode.exe"), NULL, NULL, NULL, NULL
				, CREATE_NEW_CONSOLE
				, NULL, NULL, NULL, &processInfo))
			{	
				printf("++++++++++ StartTestMode: TestMode failed in default directory +++++++++\r\n");
				return ;
			}
			printf("++++++++++ StartTestMode: TestMode started  in default directory +++++++++\r\n");
			CloseHandle(processInfo.hThread);
			CloseHandle(processInfo.hProcess);
		}
		else{
			return ;
		}
	}
	else{
		printf("++++++++++ StartTestMode: NeventWatcher started in current directory +++++++++\r\n");

		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);

		Sleep(1000);

		if (CreateProcess(strHMIPath, NULL, NULL, NULL, NULL
			, CREATE_NEW_CONSOLE
			, NULL, NULL, NULL, &processInfo))
		{	
			printf("++++++++++ StartTestMode: Neventwatcher failed in current directory +++++++++\r\n");
			return ;
		}
		printf("++++++++++ StartTestMode: Neventwatcher started in current directory +++++++++\r\n");
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);

		return ;
	}	

/*
	PROCESS_INFORMATION processInfo;


	if (!CreateProcess(_T("\\SDMMC\\CL\\NEventWatcher.exe"), NULL, NULL, NULL, NULL
		, CREATE_NEW_CONSOLE
		, NULL, NULL, NULL, &processInfo))
	{
		AfxMessageBox(_T("Failed to start NEventWatcher"));
	}
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	Sleep(2000);

	if (!CreateProcess(_T("\\SDMMC\\CL\\TestMode.exe"), NULL, NULL, NULL, NULL
		, CREATE_NEW_CONSOLE
		, NULL, NULL, NULL, &processInfo))
	{
		AfxMessageBox(_T("Failed to start TestMode"));
	}
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
*/


/*************************************************************************
	ToDo: Modify to specify the Dir
			Not Finish yet!!
*************************************************************************/
/*	
	TCHAR *TNPath;
	TCHAR *Title = _T("Choose CL Dir...");

	BrowseDirectory(Title,TNPath);

	CString Path = TNPath;
	CString TestMode = _T("\\TestMode.exe");
	CString TestModePath = Path + TestMode;
	
	//LPTSTR p = TestModePath.GetBuffer(-1);

	PROCESS_INFORMATION processInfo;

	if (!CreateProcess(TestModePath, NULL, NULL, NULL, NULL
		, CREATE_NEW_CONSOLE
		, NULL, NULL, NULL, &processInfo))
	{
		AfxMessageBox(_T("Failed to start NEventWatcher"));
	}
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);

	//printf("Path=%s\n",TNPath);
*/
}

void CStartTNDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	PROCESS_INFORMATION processInfo;

	const int nBufSize = 512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);
	BOOL bGetPath = GetModuleFileName(NULL,chBuf,nBufSize);
	TCHAR *p = wcsrchr(chBuf,'\\');
	*p = 0;
	CString strHMIPath   =   chBuf   +   CString(_T( "\\NEventWatcher.exe")); 
	CString	strNEWPath   = chBuf +   CString(_T( "\\TestMode.exe")); 
	///< Current directory is the CL dirctory it's ok 
	int iCmp = _tcscmp(chBuf,TEXT("\\SDMMC\\CL"));

	_tprintf(TEXT("\r\n ++++++ StartMap: Path is : %s +++++\r\n"),chBuf);
	if (!CreateProcess(strNEWPath, NULL, NULL, NULL, NULL
		, CREATE_NEW_CONSOLE
		, NULL, NULL, NULL, &processInfo))
	{
		DWORD dwCPError = GetLastError();
		printf("++++++++++ StartTestMode: NEventWathcer failed in current directory error code is %d +++++++++\r\n",dwCPError);		

		/* Cannot found the file-NeventWatcher in current directory then goto default directory */		
		if (2 == dwCPError){				
			if (!CreateProcess(_T("\\SDMMC\\CL\\TestMode.exe"), NULL, NULL, NULL, NULL
				, CREATE_NEW_CONSOLE
				, NULL, NULL, NULL, &processInfo))
			{				
				printf("++++++++++ StartTestMode:TestMode failed in default directory Last error code is %d +++++++++\r\n",GetLastError());		
				return ;
			}
			printf("++++++++++ StartHMI: TestMode started in default directory +++++++++\r\n");

			CloseHandle(processInfo.hThread);
			CloseHandle(processInfo.hProcess);

			Sleep(1000);

			if (!CreateProcess(_T("\\SDMMC\\CL\\NEventWatcher.exe"), NULL, NULL, NULL, NULL
				, CREATE_NEW_CONSOLE
				, NULL, NULL, NULL, &processInfo))
			{	
				printf("++++++++++ StartHMI: HMI failed in default directory +++++++++\r\n");
				return ;
			}
			printf("++++++++++ StartHMI: HMI started  in default directory +++++++++\r\n");
			CloseHandle(processInfo.hThread);
			CloseHandle(processInfo.hProcess);
		}
		else{
			return ;
		}
	}
	else{
		printf("++++++++++ StartTestMode: NeventWatcher started in current directory +++++++++\r\n");

		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);

		Sleep(1000);

		if (CreateProcess(strHMIPath, NULL, NULL, NULL, NULL
			, CREATE_NEW_CONSOLE
			, NULL, NULL, NULL, &processInfo))
		{	
			printf("++++++++++ StartTestMode: TestMode failed in current directory +++++++++\r\n");
			return ;
		}
		printf("++++++++++ StartTestMode: TestMode started in current directory +++++++++\r\n");
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);

		return ;
	}	
}
