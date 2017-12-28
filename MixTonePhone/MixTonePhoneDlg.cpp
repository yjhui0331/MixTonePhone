
// MixTonePhoneDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MixTonePhone.h"
#include "MixTonePhoneDlg.h"
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


// CMixTonePhoneDlg 对话框



CMixTonePhoneDlg::CMixTonePhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MIXTONEPHONE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMixTonePhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMixTonePhoneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMixTonePhoneDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMixTonePhoneDlg 消息处理程序

BOOL CMixTonePhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMixTonePhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMixTonePhoneDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMixTonePhoneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/* Callback called by the library upon receiving incoming call */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
	pjsip_rx_data *rdata)
{
	pjsua_call_info ci;

	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);

	pjsua_call_get_info(call_id, &ci);

	PJ_LOG(3, (THIS_FILE, "Incoming call from %.*s!!",
		(int)ci.remote_info.slen,
		ci.remote_info.ptr));

	/* Automatically answer incoming calls with 200/OK */
	pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
	pjsua_call_info ci;

	PJ_UNUSED_ARG(e);

	pjsua_call_get_info(call_id, &ci);
	PJ_LOG(3, (THIS_FILE, "Call %d state=%.*s", call_id,
		(int)ci.state_text.slen,
		ci.state_text.ptr));
}

/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
	pjsua_call_info ci;

	pjsua_call_get_info(call_id, &ci);

	if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
		// When media is active, connect call to sound device.
		pjsua_conf_connect(ci.conf_slot, 0);
		pjsua_conf_connect(0, ci.conf_slot);
	}
}

/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
}




int CMixTonePhoneDlg::MixTonePhoneCreate()
{
	pj_status_t status;
	/* Create pjsua first! */
	status = pjsua_create();
	if (status != PJ_SUCCESS)
	{
		//"Error in pjsua_create()"
		return false;
	}


	/* Init pjsua */
	/*初始化PJSUA，设置回调函数 */

	pjsua_config ua_cfg;
	pjsua_config_default(&ua_cfg);
	ua_cfg.max_calls = 4;
	ua_cfg.user_agent = pj_str("Mix Tone");
	ua_cfg.cb.on_incoming_call = &on_incoming_call;
	ua_cfg.cb.on_call_media_state = &on_call_media_state;
	ua_cfg.cb.on_call_state = &on_call_state;

	pjsua_logging_config login_cfg;
	pjsua_logging_config_default(&login_cfg);
	login_cfg.console_level = 4;

	pjsua_media_config media_cfg;
	pjsua_media_config_default(&media_cfg);

	status = pjsua_init(&ua_cfg, &login_cfg, &media_cfg);
	if (status != PJ_SUCCESS)
	{
		// "Error in pjsua_init()";
		return false;
	}

	///init 配置初始化成功



	/* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS)
	{
		//"Error starting pjsua"
		return false;
	}

	/* Add UDP transport. */
	pjsua_transport_config transport_cfg;
	pjsua_transport_config_default(&transport_cfg);
	transport_cfg.port = 5060;
	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, NULL);
	if (status != PJ_SUCCESS)
	{
		//"Error creating transport";
		return false;
	}


	pjsua_acc_id acc_id;
	pjsua_acc_config acc_cfg;
	pjsua_acc_config_default(&acc_cfg);
	acc_cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
	acc_cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
	acc_cfg.cred_count = 1;
	acc_cfg.cred_info[0].realm = pj_str("*");
	acc_cfg.cred_info[0].scheme = pj_str("digest");
	acc_cfg.cred_info[0].username = pj_str(SIP_USER);
	acc_cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	acc_cfg.cred_info[0].data = pj_str(SIP_PASSWD);

	status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS)
	{
		//"Error adding account"
		return false;
	}



	return 0;
}




void CMixTonePhoneDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	MixTonePhoneCreate();
}


void CMixTonePhoneDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/* Destroy pjsua */
	pjsua_destroy();

	CDialog::OnClose();
}
