
// UseLogDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "UseLog.h"
#include "UseLogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnCbnSelchangeCombo1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_CBN_SELCHANGE(IDC_COMBO1, &CAboutDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CUseLogDlg 对话框



CUseLogDlg::CUseLogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USELOG_DIALOG, pParent)
	, m_consoleTitle(_T("./"))
	, m_udpIp(_T("127.0.0.1"))
	, m_udpPort(9000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUseLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboxLevel);
	DDX_Control(pDX, IDC_COMBO2, m_comboxSink);
	DDX_Control(pDX, IDC_EDIT1, m_consoleTitleEdit);
	DDX_Control(pDX, IDC_EDIT2, m_udpAddrEdit);
	DDX_Control(pDX, IDC_EDIT3, m_udpPortEdit);
	DDX_Text(pDX, IDC_EDIT1, m_consoleTitle);
	DDX_Text(pDX, IDC_EDIT2, m_udpIp);
	DDX_Text(pDX, IDC_EDIT3, m_udpPort);
}

BEGIN_MESSAGE_MAP(CUseLogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUseLogDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUseLogDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CUseLogDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, &CUseLogDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CUseLogDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CUseLogDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CUseLogDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CUseLogDlg::OnBnClickedButton7)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CUseLogDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CUseLogDlg 消息处理程序

BOOL CUseLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	m_comboxLevel.AddString("Trace");
	m_comboxLevel.AddString("Debug");
	m_comboxLevel.AddString("Info");
	m_comboxLevel.AddString("Warn");
	m_comboxLevel.AddString("Error");
	m_comboxLevel.AddString("Fatal");
	m_comboxLevel.SetCurSel(1);
	
	m_comboxSink.AddString("FileSink");
	m_comboxSink.AddString("ConsoleSink");
	m_comboxSink.AddString("DebugViewSink");
	m_comboxSink.AddString("UdpSink");
	m_comboxSink.SetCurSel(0);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUseLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUseLogDlg::OnPaint()
{
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
HCURSOR CUseLogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUseLogDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	LOG_INFO("点击日志输出按钮");
}

void PrintThread();
void CUseLogDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//int no = 1;
	std::thread th = std::thread(PrintThread);
	th.detach();
}

void PrintThread()
{
	int no = 0;
	Sleep(1000);
	while (1)
	{
		LOG_INFO("循环输出日志no:" + std::to_string(no));
		no++;
		Sleep(500);
		if (no > 5)
			break;
	}
}


void CUseLogDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int level = m_comboxLevel.GetCurSel();
	LOG_INFO(std::to_string(level));
	//Logger::Instance().SetLogLevel((LogLevel)level);
	LOG_SETLEVEL((LogLevel)level);
}


void CUseLogDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代
	int* p = nullptr;
	*p = 123;
}


void CUseLogDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	Calculator cal;
	int result = cal.Add(12, 13);
	LOG_INFO("计算结果:" + std::to_string(result));

}

void AttachToParentConsole()
{
	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
	}
}

void CUseLogDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//ConsoleManager::Close();
	LOG_WARN("这是一条警告日志");
}


void CUseLogDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	std::cout << "测试输出" << std::endl;
}


void CUseLogDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	LOG_ERROR("这是一条错误日志");
}


void CUseLogDlg::OnCbnSelchangeCombo2()  
{  
   // TODO: 在此添加控件通知处理程序代码  
	UpdateData(true);
   int sink = m_comboxSink.GetCurSel();  
   CString sinkName;  
   m_comboxSink.GetLBText(sink, sinkName);  

   if (sinkName == "UdpSink")  
   {  
       // Fix for E0312: Explicitly convert ATL::CA2A to std::string  
       std::string udpSinkIp = std::string(CT2A(m_udpIp.GetString()));  
	   std::wstring consoleTitle = CharToWString(m_consoleTitle.GetString());
       // Fix for C4003: Provide required arguments to LOG_SETUDPSINK  
       LOG_SETUDPSINK(udpSinkIp, static_cast<uint16_t>(m_udpPort));
	   std::wstring appPath = GetExeDir() + L"\\LogUdpConsole.exe";
	   StartUdpConsole(appPath, consoleTitle, L"0.0.0.0", m_udpPort);
   }  

   // Fix for other sinks  
   LOG_SETSINK((Sink)sink);  
}

bool CUseLogDlg::StartUdpConsole(const std::wstring& exePath, const std::wstring& title, const std::wstring& ip, int port)
{
	std::wstring cmdLine =
		L"\"" + exePath + L"\" "
		L"\"" + title + L"\" "
		+ ip + L" "
		+ std::to_wstring(port);

	STARTUPINFOW si{};
	PROCESS_INFORMATION pi{};

	si.cb = sizeof(si);

	BOOL ok = CreateProcessW(
		nullptr,                    // lpApplicationName
		&cmdLine[0],               // lpCommandLine（必须可写）
		nullptr,
		nullptr,
		FALSE,
		CREATE_NEW_CONSOLE,         // 新控制台窗口
		nullptr,
		nullptr,
		&si,
		&pi
	);

	if (!ok)
	{
		/*std::wcout << L"CreateProcess 失败, 错误码="
			<< GetLastError() << std::endl;*/
		LOG_ERROR("创建进程失败：" + GetLastError());
		return false;
	}

	// 2. 关闭不需要的句柄
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return true;
}

std::wstring CUseLogDlg::CharToWString(const char* str)
{
	CA2W w(str, CP_ACP); // 或 CP_UTF8，取决于 str 编码
	return std::wstring(w);
	//if (str == nullptr)
	//{
	//	return L"";
	//}

	//int len = MultiByteToWideChar(
	//	CP_UTF8,
	//	0,
	//	str,
	//	-1,
	//	nullptr,
	//	0
	//);

	//std::wstring wstr(len, L'\0');

	//MultiByteToWideChar(
	//	CP_UTF8,
	//	0,
	//	str,
	//	-1,
	//	&wstr[0],
	//	len
	//);

	//// 去掉末尾 \0
	//if (!wstr.empty())
	//{
	//	wstr.pop_back();
	//}

	//return wstr;
}

std::wstring CUseLogDlg::GetExeDir()
{
	wchar_t path[MAX_PATH]{};
	DWORD len = GetModuleFileNameW(nullptr, path, MAX_PATH);
	if (len == 0)
	{
		return L"";
	}

	std::wstring fullPath(path, len);
	size_t pos = fullPath.find_last_of(L"\\/");
	if (pos == std::wstring::npos)
	{
		return L"";
	}

	// 返回目录，不包含文件名
	return fullPath.substr(0, pos);
}

