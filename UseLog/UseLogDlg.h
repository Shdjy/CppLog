
// UseLogDlg.h: 头文件
//

#pragma once


// CUseLogDlg 对话框
class CUseLogDlg : public CDialogEx
{
// 构造
public:
	CUseLogDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USELOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_comboxLevel;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	CComboBox m_comboxSink;
	afx_msg void OnCbnSelchangeCombo2();

private:
	bool StartUdpConsole(const std::wstring& exePath, const std::wstring& title, const std::wstring& ip, int port);
	std::wstring CharToWString(const char* str);
	std::string CStringToString(const CString& cs);
	std::wstring GetExeDir();
public:	
	CEdit m_consoleTitleEdit;
	CEdit m_udpAddrEdit;
	CEdit m_udpPortEdit;
	CString m_consoleTitle;
	CString m_udpIp;
	int m_udpPort;
};
