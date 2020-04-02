
// FtpClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FtpClient.h"
#include "FtpClientDlg.h"
#include "mt.h"
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
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFtpClientDlg 对话框



CFtpClientDlg::CFtpClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FTPCLIENT_DIALOG, pParent)
	, m_strPwd(_T(""))
	, m_strUrl(_T(""))
	, m_strUsername(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME2);
}

void CFtpClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_btnDownload);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_btnPause);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_btnQuit);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_btnUpload);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_URL, m_strUrl);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Control(pDX, IDC_LIST1, m_listFile);
	DDX_Control(pDX, IDC_EDIT_URL, m_editUrl);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_EDIT_PWD, m_editPwd);
}

BEGIN_MESSAGE_MAP(CFtpClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CFtpClientDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CFtpClientDlg::OnBnClickedButtonQuery)
	ON_LBN_SELCHANGE(IDC_LIST1, &CFtpClientDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CFtpClientDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CFtpClientDlg::OnBnClickedButtonUpload)
END_MESSAGE_MAP()


// CFtpClientDlg 消息处理程序

BOOL CFtpClientDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strUrl = TEXT("49.235.3.103");
	m_strUsername = TEXT("uftp");
	m_strPwd = TEXT("2382525abc");
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFtpClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFtpClientDlg::OnPaint()
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
HCURSOR CFtpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFtpClientDlg::OnBnClickedButtonQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CFtpClientDlg::OnBnClickedButtonQuery()
{
	UpdateData(TRUE);
	FTP_INFO* pp = new FTP_INFO;
	pp->pList = &m_listFile;
	pp->strUsername = m_strUsername;
	pp->strPwd = m_strPwd;
	pp->strUrl = m_strUrl;
	while (m_listFile.GetCount() != 0)
		m_listFile.DeleteString(0);
	AfxBeginThread(mtQuery, pp);

}


void CFtpClientDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_editUrl.EnableWindow(FALSE);
	m_editUsername.EnableWindow(FALSE);
	m_editPwd.EnableWindow(FALSE);
	m_btnUpload.EnableWindow(FALSE);
	m_btnQuery.EnableWindow(FALSE);
	m_btnDownload.EnableWindow(TRUE);
}


void CFtpClientDlg::OnBnClickedButtonDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	FTP_INFO* pp = new FTP_INFO;
	pp->pList = &m_listFile;
	pp->strUsername = m_strUsername;
	pp->strPwd = m_strPwd;
	pp->strUrl = m_strUrl;
	AfxBeginThread(mtDownloadFile, pp);

	m_btnDownload.EnableWindow(FALSE);
	m_editUrl.EnableWindow(TRUE);
	m_editUsername.EnableWindow(TRUE);
	m_editPwd.EnableWindow(TRUE);
	m_btnUpload.EnableWindow(TRUE);
	m_btnQuery.EnableWindow(TRUE);

}


void CFtpClientDlg::OnBnClickedButtonUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_editUrl.EnableWindow(FALSE);
	m_editUsername.EnableWindow(FALSE);
	m_editPwd.EnableWindow(FALSE);
	m_btnQuery.EnableWindow(FALSE);
	FTP_INFO* pp = new FTP_INFO;
	pp->pList = nullptr;
	pp->strUsername = m_strUsername;
	pp->strPwd = m_strPwd;
	pp->strUrl = m_strUrl;
	AfxBeginThread(mtUploadFile, pp);

	m_editUrl.EnableWindow(TRUE);
	m_editUsername.EnableWindow(TRUE);
	m_editPwd.EnableWindow(TRUE);
	m_btnQuery.EnableWindow(TRUE);

}
