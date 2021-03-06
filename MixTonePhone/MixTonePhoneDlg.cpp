
// MixTonePhoneDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MixTonePhone.h"
#include "MixTonePhoneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMixTonePhoneDlg *g_pMixTonePhoneDlg = NULL;

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
	, m_ConsoleMessage(_T(""))
	, m_PhoneNumber(_T(""))
{
	//使用控制台进行调试输出
#ifdef _DEBUG
	if (AllocConsole()) {
		HANDLE console = NULL;
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		FILE* pCout = NULL;
		freopen_s(&pCout,"CONOUT$", "wt", stdout);
	}
#endif
	pRemoteVideoDlg = NULL;
	pLocalVideoDlg = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ConsoleMessage = _T("");
}

void CMixTonePhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONSOLE_MESSAGE, m_ConsoleMessage);
	DDX_Text(pDX, IDC_PHONE_NUMBER, m_PhoneNumber);
	DDX_Control(pDX, IDC_REMOTE_VIDEO_WIN, m_RemoteVideoWin);
	DDX_Control(pDX, IDC_LOCAL_VIDEO_WIN, m_LocalVideoWin);
	DDX_Control(pDX, IDC_OPEN_BUTTON, m_OpenCloseButton);
	DDX_Control(pDX, IDC_VIDEO_MAKE_CALL, m_btnVideoCall);
}

BEGIN_MESSAGE_MAP(CMixTonePhoneDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ANSWER_CALL, &CMixTonePhoneDlg::OnBnClickedAnswerCall)
	ON_MESSAGE(WM_MT_EVENT, OnDispEvent)
	ON_BN_CLICKED(IDC_MAKE_CALL, &CMixTonePhoneDlg::OnBnClickedMakeCall)
	ON_BN_CLICKED(IDC_HANGUP_CALL, &CMixTonePhoneDlg::OnBnClickedHangupCall)
	ON_BN_CLICKED(IDC_LOCAL_VIDEO_SHOW, &CMixTonePhoneDlg::OnBnClickedLocalVideoShow)
	ON_BN_CLICKED(IDC_LOCAL_VIDEO_CLOSE, &CMixTonePhoneDlg::OnBnClickedLocalVideoClose)
	ON_BN_CLICKED(IDC_SYS_BUTTON, &CMixTonePhoneDlg::OnBnClickedSysButton)
	ON_BN_CLICKED(IDC_OPEN_BUTTON, &CMixTonePhoneDlg::OnBnClickedOpenButton)
	ON_BN_CLICKED(IDC_VIDEO_MAKE_CALL, &CMixTonePhoneDlg::OnBnClickedVideoMakeCall)
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
	m_btnVideoCall.SetImage(IDB_BITMAP);

	CtrlEnable(FALSE);
	this->GetWindowRect(&m_rcMaxMain);

	g_pMixTonePhoneDlg = this;
	MixTonePhoneCreate();

	//创建视频显示窗口
	CreatVideoWind();

	CRect rcMain,rcOpenCloseButton;
	this->GetWindowRect(&rcMain);
	m_OpenCloseButton.GetWindowRect(rcOpenCloseButton);

	rcMain.right = rcOpenCloseButton.right;

	m_bMaxMix = TRUE;//Mix

	//rcMainDlg.OffsetRect(0, rcMainDlg.right-rcOpenCloseButton.right);
	this->MoveWindow(rcMain);

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



LRESULT CMixTonePhoneDlg::OnDispEvent(WPARAM wParam, LPARAM lParam)
{
	int nEvent = (int)wParam;

	switch (nEvent)
	{
	case MT_EVENT_INCOMINGCALL:
		{
			LPMT_INCOMINGCALL pInComingCall = (LPMT_INCOMINGCALL)lParam;
			pjsua_call_info ci;

			pjsua_call_get_info(pInComingCall->call_id, &ci);
			m_ConsoleMessage.Format(_T("Incoming call from %.*s \r\n"),
				(int)ci.remote_info.slen, PjToStr(&ci.remote_info,TRUE));

			UpdateData(FALSE);
			
			//pjsua_call_answer(pInComingCall->call_id, 200, NULL, NULL);
		}
	break;
	default:
		break;
	}

	return 0;
}


/* Callback called by the library upon receiving incoming call */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
	pjsip_rx_data *rdata)
{
	pjsua_call_info ci;

	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);

 	pjsua_call_get_info(call_id, &ci);
// 
// 	PJ_LOG(3, (THIS_FILE, "Incoming call from %.*s!!",
// 		(int)ci.remote_info.slen,ci.remote_info.ptr));

	LPMT_INCOMINGCALL pInComingCall = new MT_INCOMINGCALL;
	pInComingCall->acc_id = acc_id;
	pInComingCall->call_id = call_id;
	pInComingCall->rdata = rdata;


	//play ringin.wav


	//ringing
	pjsua_call_answer(call_id, PJSIP_SC_RINGING, NULL, NULL);
	g_pMixTonePhoneDlg->PostMessage(WM_MT_EVENT, (WPARAM)MT_EVENT_INCOMINGCALL, (LPARAM)pInComingCall);
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

	//呼叫过程中的状态
	CString strMessage = _T("");
	strMessage.Format(_T("on_call_state Call %d state=%.*s \r\n"), call_id,
		(int)ci.state_text.slen,PjToStr(&ci.state_text,TRUE));

	g_pMixTonePhoneDlg->m_ConsoleMessage += strMessage;

	CWnd * pConsoleMessage = g_pMixTonePhoneDlg->GetDlgItem(IDC_CONSOLE_MESSAGE);
	if (pConsoleMessage)
		pConsoleMessage->SetWindowText(g_pMixTonePhoneDlg->m_ConsoleMessage);
}

static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
	if (g_pMixTonePhoneDlg != NULL)
	{
		//pjsip_status_code info->cbparam->code
		CString strRegMsg = _T("");
		if (PJSIP_SC_OK == info->cbparam->code)
		{
			strRegMsg.Format(_T("%s%s \r\n"), g_pMixTonePhoneDlg->m_strUserName , _T("在线"));	
			g_pMixTonePhoneDlg->CtrlEnable(TRUE);
		}
		else
		{
			strRegMsg.Format(_T("%s%s \r\n"), g_pMixTonePhoneDlg->m_strUserName, _T("离线"));
			g_pMixTonePhoneDlg->CtrlEnable(FALSE);
		}

		g_pMixTonePhoneDlg->SetWindowText(strRegMsg);
		CWnd * pConsoleMessage = g_pMixTonePhoneDlg->GetDlgItem(IDC_CONSOLE_MESSAGE);
		if (pConsoleMessage)
			pConsoleMessage->SetWindowText(strRegMsg);
	}
}


void CMixTonePhoneDlg::CreatVideoWind()
{
	CRect rcRClient, rcRWind, rcRemoteRWind, rcLWind, rcLocalWind;
	m_RemoteVideoWin.GetClientRect(&rcRClient);
	m_RemoteVideoWin.GetWindowRect(&rcRWind);
	m_LocalVideoWin.GetWindowRect(&rcLWind);


	HWND hwnd = pRemoteVideoDlg->GetSafeHwnd();
	if (hwnd == NULL)
	{
		pRemoteVideoDlg = new CVideoDlg;
		pRemoteVideoDlg->Create(IDD_VIDEO_DIALOG, CWnd::FromHandle(m_RemoteVideoWin.m_hWnd));
		m_RemoteVideoWin.ScreenToClient(&rcRWind);
		rcRemoteRWind.left = rcRWind.left + 1;
		rcRemoteRWind.right = rcRWind.right - 1;
		rcRemoteRWind.top = rcRWind.top + 1;
		rcRemoteRWind.bottom = rcRWind.bottom - 1;
		pRemoteVideoDlg->MoveWindow(rcRemoteRWind);
		pRemoteVideoDlg->ShowWindow(SW_SHOW);
	}

	hwnd = pLocalVideoDlg->GetSafeHwnd();
	if (hwnd == NULL)
	{
		pLocalVideoDlg = new CVideoDlg;
		pLocalVideoDlg->Create(IDD_VIDEO_DIALOG, CWnd::FromHandle(m_LocalVideoWin.m_hWnd));
		m_LocalVideoWin.ScreenToClient(&rcLWind);
		rcLocalWind.left = rcLWind.left + 1;
		rcLocalWind.right = rcLWind.right - 1;
		rcLocalWind.top = rcLWind.top + 1;
		rcLocalWind.bottom = rcLWind.bottom - 1;
		pLocalVideoDlg->MoveWindow(rcLocalWind);
		pLocalVideoDlg->ShowWindow(SW_SHOW);
	}

}



void CMixTonePhoneDlg::ShowRemoteVideo(pjsua_call_id call_id)
{
	//Remote video
	int vid_idx; pjsua_vid_win_id Rwid;
	vid_idx = pjsua_call_get_vid_stream_idx(call_id);
	if (vid_idx < 0) return;

	pjsua_call_info ci;
	pjsua_call_get_info(call_id, &ci);
	//Type is not specified
	//PJMEDIA_TYPE_NONE,
	//The media is audio
	//PJMEDIA_TYPE_AUDIO,
	//The media is video
	//PJMEDIA_TYPE_VIDEO,
	//The media is application
	//PJMEDIA_TYPE_APPLICATION,
	//The media type is unknown or unsupported
	//PJMEDIA_TYPE_UNKNOWN

	if (PJMEDIA_TYPE_VIDEO != ci.media[vid_idx].type)
		return;

	Rwid = ci.media[vid_idx].stream.vid.win_in;
	if (Rwid < 0) return;

	pjsua_vid_win_info rwi;
	pjsua_vid_win_get_info(Rwid, &rwi);

	//CVideoDlg* pRemoteVideoDlg = new CVideoDlg;
	CRect rcRClient, rcRWind, rcRemoteRWind;
	pjmedia_coord Rpos = { 0, 0 };
	CWnd * pRemoteVideoWnd = g_pMixTonePhoneDlg->GetDlgItem(IDC_REMOTE_VIDEO_WIN);
	if (pRemoteVideoWnd)
	{
 		pRemoteVideoWnd->GetClientRect(&rcRClient);
		pRemoteVideoWnd->GetWindowRect(&rcRWind);

		pRemoteVideoDlg->ShowWindow(SW_SHOW);
	}

	//pjmedia_vid_dev_hwnd rwnd;
	//rwnd.type = PJMEDIA_VID_DEV_HWND_TYPE_NONE;
	//rwnd.info.win.hwnd = pRemoteVideoDlg->m_hWnd;
	//rwnd.info.win.hwnd = pRemoteVideoDlg->m_hWnd;
	//pjsua_vid_win_set_win(Rwid, &rwnd);

	pjsua_vid_win_set_pos(Rwid, &Rpos);

	pjmedia_rect_size size = { 320, 240 };
	size.h = rcRClient.Height();
	size.w = rcRClient.Width();
	pjsua_vid_win_set_size(Rwid, &size);

	//::SetParent(pRemoteVideoDlg->m_hWnd, pRemoteVideoWnd->m_hWnd);
	::SetParent((HWND)rwi.hwnd.info.win.hwnd, pRemoteVideoDlg->m_hWnd);
}

/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
	pjsua_call_info ci;

	pjsua_call_get_info(call_id, &ci);

	//显示对端视频
	//g_pMixTonePhone->ShowRemoteVideo(call_id);

	//g_pMixTonePhone->PostMessage(WM_MT_CREATVIDEOWIN_EVENT, 0, 0);

	/*
	//Call currently has no media, or the media is not used.
	PJSUA_CALL_MEDIA_NONE,
	//The media is active
	PJSUA_CALL_MEDIA_ACTIVE,
	//The media is currently put on hold by local endpoint
	PJSUA_CALL_MEDIA_LOCAL_HOLD,
	//The media is currently put on hold by remote endpoint
	PJSUA_CALL_MEDIA_REMOTE_HOLD,
	//The media has reported error (e.g. ICE negotiation)
	PJSUA_CALL_MEDIA_ERROR
	*/
	CString strMediaStatus = _T("");
	switch (ci.media_status)
	{
	case PJSUA_CALL_MEDIA_NONE:
		strMediaStatus = _T("PJSUA_CALL_MEDIA_NONE");
		break;
	case PJSUA_CALL_MEDIA_ACTIVE:
		strMediaStatus = _T("PJSUA_CALL_MEDIA_ACTIVE");
		break;
	case PJSUA_CALL_MEDIA_LOCAL_HOLD:
		strMediaStatus = _T("PJSUA_CALL_MEDIA_LOCAL_HOLD");
		break;
	case PJSUA_CALL_MEDIA_REMOTE_HOLD:
		strMediaStatus = _T("PJSUA_CALL_MEDIA_REMOTE_HOLD");
		break;
	case PJSUA_CALL_MEDIA_ERROR:
		strMediaStatus = _T("PJSUA_CALL_MEDIA_ERROR");
		break;
	default:
		strMediaStatus = _T("PJSUA_CALL_MEDIA_ERROR");
	}

	CString strMessage = _T("");
	strMessage.Format(_T("on_call_media_state Call %d \r\nmediastate=%s \r\n"), call_id,
		strMediaStatus);

	g_pMixTonePhoneDlg->m_ConsoleMessage += strMessage;

	CWnd * pConsoleMessage = g_pMixTonePhoneDlg->GetDlgItem(IDC_CONSOLE_MESSAGE);
	if (pConsoleMessage)
		pConsoleMessage->SetWindowText(g_pMixTonePhoneDlg->m_ConsoleMessage);
	 

	if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
		// When media is active, connect call to sound device.
		//媒体处于活动状态时，将呼叫连接至声音设备。  
		pjsua_conf_connect(ci.conf_slot, 0);
		pjsua_conf_connect(0, ci.conf_slot);

		//显示对端视频
		g_pMixTonePhoneDlg->ShowRemoteVideo(call_id);

		return;
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
	pjsua_logging_config log_cfg;
	pjsua_config_default(&ua_cfg);

	ua_cfg.max_calls = 1;//话机最大呼叫线路数
	ua_cfg.user_agent = pj_str("Mix Tone");
	ua_cfg.cb.on_incoming_call = &on_incoming_call;//通知 有呼叫到来
	ua_cfg.cb.on_call_media_state = &on_call_media_state;//媒体状态回调函数
	ua_cfg.cb.on_call_state = &on_call_state;//通知 呼叫状态变化
	ua_cfg.cb.on_reg_state2 = &on_reg_state2;//注册状态

	//log config
	pjsua_logging_config_default(&log_cfg);
	log_cfg.msg_logging = PJ_TRUE;
	log_cfg.console_level = 4;

	pjsua_media_config media_cfg;
	pjsua_media_config_default(&media_cfg);

	status = pjsua_init(&ua_cfg, &log_cfg, &media_cfg);
	if (status != PJ_SUCCESS)
	{
		// "Error in pjsua_init()";
		return false;
	}

	///init 配置初始化成功

	/* Add UDP transport. */
	pjsua_transport_config transport_cfg;
	pjsua_transport_config_default(&transport_cfg);
	transport_cfg.port = 5060;
	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, NULL);
	if (status != PJ_SUCCESS)
	{
		//"Error creating transport";
		m_ConsoleMessage = _T("Error creating transport");
		UpdateData(FALSE);
		return false;
	}


	// 计划单独封装登录功能 [12/28/2017 yangjianhui]
	/* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS)
	{
		//"Error starting pjsua"
		m_ConsoleMessage = _T("Error starting pjsua");
		UpdateData(FALSE);
		return false;
	}

	LPMT_ACCOUNT pAccount = new MT_ACCOUNT;
	if (!LoadSetting(pAccount))
	{
		//AfxMessageBox(_T("获取配置失败!"));
		return false;
	}

	if (!addAccount(pAccount))
	{
		//"Error adding account"
		m_ConsoleMessage = _T("Error adding account");

		UpdateData(FALSE);
	}
	delete pAccount;
	/*
	pjsua_acc_id acc_id;
	pjsua_acc_config acc_cfg;
	pjsua_acc_config_default(&acc_cfg);
	CString strACCID = _T("");
	strACCID.Format(_T("sip:%s@%s"), SIP_USER, SIP_DOMAIN);
	//acc_cfg.id = pj_str(_T("sip:") SIP_USER "@" SIP_DOMAIN);
	acc_cfg.id = StrToPjStr(strACCID);
	//acc_cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
	CString strRegUri = _T("");
	strRegUri.Format(_T("sip:%s"), SIP_DOMAIN);
	acc_cfg.reg_uri = StrToPjStr(strRegUri);
	acc_cfg.cred_count = 1;
	acc_cfg.cred_info[0].realm = pj_str("*");
	acc_cfg.cred_info[0].scheme = pj_str("digest");
	acc_cfg.cred_info[0].username = StrToPjStr(SIP_USER);
	acc_cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	acc_cfg.cred_info[0].data = StrToPjStr(SIP_PASSWD);
	acc_cfg.vid_in_auto_show = PJ_TRUE;//指定传入的视频是否自动显示在屏幕上
	acc_cfg.vid_cap_dev = PJMEDIA_VID_DEFAULT_CAPTURE_DEV; //PJMEDIA_VID_DEFAULT_CAPTURE_DEV
	acc_cfg.vid_out_auto_transmit = PJ_TRUE;//设置当有视频来电，或拨出电话时，是否默认激活视频传出

	// 	status = pjsua_verify_url("sip:" SIP_USER "@" SIP_DOMAIN);
	// 	if (status != PJ_SUCCESS)
	// 	{
	// 		//"Invalid URL in argv"
	// 		m_ConsoleMessage = "Invalid URL in argv";
	// 
	// 		UpdateData(FALSE);
	// 		return false;
	// 	}

	status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS)
	{
		//"Error adding account"
		m_ConsoleMessage = _T("Error adding account");

		UpdateData(FALSE);

		return false;
	}
	*/


	// codec_param [3/23/2018 yangjianhui]
	const pj_str_t codec_id = { "H264", 4 };
	pjmedia_vid_codec_param param;

	pjsua_vid_codec_get_param(&codec_id, &param);
	/* Modify param here */
	/* Sending 1280 x 720 */
	param.enc_fmt.det.vid.size.w = 1280;
	param.enc_fmt.det.vid.size.h = 720;
	param.enc_fmt.det.vid.fps.num = 25;
	//param.enc_fmt.det.vid.fps.tal = 1;


	/* recve 1920 x 1080 */
	param.dec_fmt.det.vid.size.w = 1920;
	param.dec_fmt.det.vid.size.h = 1080;
	pjsua_vid_codec_set_param(&codec_id, &param);

	return 0;
}

BOOL CMixTonePhoneDlg::addAccount(LPMT_ACCOUNT pAccount)
{
	if (!pAccount)
		return FALSE;


	m_nNumber  = pAccount->nNumber;
	m_strUserName = pAccount->strUserName;
	m_strPassword = pAccount->strPassword;
	m_strDoMain = pAccount->strDoMain;
	m_nPort = pAccount->nPort;

	pjsua_acc_id acc_id;
	pjsua_acc_config acc_cfg;
	pjsua_acc_config_default(&acc_cfg);
	CString strACCID = _T("");
	strACCID.Format(_T("sip:%d@%s:%d"), pAccount->nNumber, pAccount->strDoMain, pAccount->nPort);
	acc_cfg.id = StrToPjStr(strACCID);
	CString strRegUri = _T("");
	strRegUri.Format(_T("sip:%s:%d"), pAccount->strDoMain, pAccount->nPort);
	acc_cfg.reg_uri = StrToPjStr(strRegUri);
	acc_cfg.cred_count = 1;
	acc_cfg.cred_info[0].realm = pj_str("*");
	acc_cfg.cred_info[0].scheme = pj_str("digest");
	//CString strusername(Account->strUserName.c_str());
	acc_cfg.cred_info[0].username = StrToPjStr(pAccount->strUserName);
	acc_cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	//CString strPassword(Account->strPassword.c_str());
	acc_cfg.cred_info[0].data = StrToPjStr(pAccount->strPassword);
	
	acc_cfg.vid_in_auto_show = PJ_TRUE;//指定传入的视频是否自动显示在屏幕上
	acc_cfg.vid_cap_dev = PJMEDIA_VID_DEFAULT_CAPTURE_DEV; //PJMEDIA_VID_DEFAULT_CAPTURE_DEV
	acc_cfg.vid_out_auto_transmit = PJ_TRUE;//设置当有视频来电，或拨出电话时，是否默认激活视频传出


	pj_status_t status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS)
	{
		//"Error adding account"
		m_ConsoleMessage = _T("Error adding account");

		UpdateData(FALSE);

		return false;
	}

	return TRUE;
}

BOOL CMixTonePhoneDlg::modifyAccount(LPMT_ACCOUNT pAccount)
{
	if (!pAccount)
		return FALSE;

	pjsua_acc_del(0);
	g_pMixTonePhoneDlg->addAccount(pAccount);

	return TRUE;
}




void CMixTonePhoneDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/* Destroy pjsua */
	pjsua_destroy();

	CDialog::OnClose();
}


void CMixTonePhoneDlg::OnBnClickedAnswerCall()
{
	pjsua_call_id call_id = 0;

	/* Automatically answer incoming calls with 200/OK */
	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS)
	{
		pjsua_call_setting call_setting;
		pjsua_call_setting_default(&call_setting);
		call_setting.vid_cnt = call_info.rem_vid_cnt > 0 ? 1:0;
		pjsua_call_answer2(call_id, &call_setting, 200, NULL, NULL);
		OpenCloseVideoPan();
	}


}


void CMixTonePhoneDlg::OnBnClickedMakeCall()
{
	pjsua_acc_id acc_id = 0;
	//CreatVideoWind();

	UpdateData(TRUE);
	if (m_PhoneNumber.IsEmpty())
		return;

	CString strDsturi = _T("");
	strDsturi.Format(_T("sip:%s@%s"), m_PhoneNumber, m_strDoMain);
	pj_str_t dst_uri = StrToPjStr(strDsturi);


	//设置 呼叫属性
	pjsua_call_setting call_setting;
	pjsua_call_setting_default(&call_setting);
	//call_setting.flag = 0;
	call_setting.vid_cnt = 0;//是否启动视频

// 	if (call_setting.vid_cnt)
// 	{
// 		OpenCloseVideoPan();
// 	}


	pjsua_call_make_call(acc_id, &dst_uri, &call_setting, NULL, NULL, NULL);

}


void CMixTonePhoneDlg::OnBnClickedHangupCall()
{
	// TODO: 在此添加控件通知处理程序代码
	//pjsua_call_hangup(call_id，PJSIP_SC_TEMPORARILY_UNAVAILABLE，NULL，NULL）;

	pjsua_call_id call_id = 0;

	/* Automatically answer incoming calls with 200/OK */
	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) == PJ_SUCCESS)
	{
		pjsua_call_hangup(call_id, 0, NULL, NULL);

		m_bMaxMix = FALSE;
		OpenCloseVideoPan();
	}
}


void CMixTonePhoneDlg::OnBnClickedLocalVideoShow()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取预览窗口
	pjmedia_vid_dev_index widx;
	widx = PJMEDIA_VID_DEFAULT_CAPTURE_DEV;
	pjsua_vid_win_info wi;
	pjsua_vid_win_id wid = pjsua_vid_preview_get_win(widx);

	//设置预览设置
	pjsua_vid_preview_param pre_param;
	pj_status_t status;
	pjmedia_coord pos = { 0, 0 };

	CRect rcClient, rcWind;
	CWnd * pLocalVideoWnd = GetDlgItem(IDC_LOCAL_VIDEO_WIN);
	if (pLocalVideoWnd)
	{
		pLocalVideoWnd->GetClientRect(&rcClient);
		pLocalVideoWnd->GetWindowRect(&rcWind);
	}

	pjsua_vid_preview_param_default(&pre_param);
	pre_param.rend_id = PJMEDIA_VID_DEFAULT_RENDER_DEV;
	pre_param.show = PJ_FALSE;
	status = pjsua_vid_preview_start(PJMEDIA_VID_DEFAULT_CAPTURE_DEV, &pre_param);
	wid = pjsua_vid_preview_get_win(widx);
	pjsua_vid_win_get_info(wid, &wi);

	pjmedia_rect_size size = { 320, 240 };
	size.h = rcClient.Height();
	size.w = rcClient.Width();
	pjsua_vid_win_set_size(wid, &size);

	pjsua_vid_win_set_pos(wid, &pos);

	pjsua_vid_win_set_show(wid, PJ_TRUE);

	::SetParent((HWND)wi.hwnd.info.win.hwnd, pLocalVideoDlg->m_hWnd);

	//wid = pjsua_vid_preview_get_win(widx);

	pLocalVideoDlg->ShowWindow(SW_SHOW);
}


void CMixTonePhoneDlg::OnBnClickedLocalVideoClose()
{
	// TODO: 在此添加控件通知处理程序代码

	pjsua_vid_win_info wi;
	pjsua_vid_win_id wid = pjsua_vid_preview_get_win(PJMEDIA_VID_DEFAULT_CAPTURE_DEV);

	if (wid == PJSUA_INVALID_ID)
		return;

	pjsua_vid_win_get_info(wid, &wi);

	CRect rcClient, rcWind;
	CWnd * pLocalVideoWnd = GetDlgItem(IDC_LOCAL_VIDEO_WIN);
	if (pLocalVideoWnd)
	{
		pLocalVideoWnd->GetClientRect(&rcClient);
		pLocalVideoWnd->GetWindowRect(&rcWind);
	}

	pjsua_vid_win_set_show(wid, PJ_FALSE);
	pjsua_vid_preview_stop(PJMEDIA_VID_DEFAULT_CAPTURE_DEV);

	pLocalVideoWnd->Invalidate();
	pLocalVideoWnd->UpdateWindow();
	::UpdateWindow((HWND)wi.hwnd.info.win.hwnd);
	//::ShowWindow((HWND)wi.hwnd.info.win.hwnd, SW_HIDE);
	pLocalVideoDlg->ShowWindow(SW_HIDE);
}


void CMixTonePhoneDlg::OnBnClickedSysButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CSettingsDlg SettingDlg(this);
	SettingDlg.DoModal();
}


void CMixTonePhoneDlg::OnBnClickedOpenButton()
{
	// TODO: 在此添加控件通知处理程序代码
	return OpenCloseVideoPan();
}

void CMixTonePhoneDlg::OpenCloseVideoPan()
{
	if (m_bMaxMix == TRUE)//Mix
	{
		CRect rcMain;
		this->GetWindowRect(&rcMain);
		rcMain.right = rcMain.left + m_rcMaxMain.Width();
		this->MoveWindow(rcMain);
		m_OpenCloseButton.SetWindowText(_T("<"));
		m_bMaxMix = FALSE;
		return;
	}
	else//Max
	{
		CRect rcOpenCloseButton, rcMain;
		m_OpenCloseButton.GetWindowRect(&rcOpenCloseButton);
		this->GetWindowRect(&rcMain);
		rcMain.right = rcOpenCloseButton.right;
		this->MoveWindow(rcMain);
		m_OpenCloseButton.SetWindowText(_T(">"));
		m_bMaxMix = TRUE;
		return;
	}
}

void CMixTonePhoneDlg::CtrlEnable(BOOL bEnable)
{
	GetDlgItem(IDC_MAKE_CALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_ANSWER_CALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_HANGUP_CALL)->EnableWindow(bEnable);
	//GetDlgItem(IDC_MAKE_CALL)->EnableWindow(bEnable);
	//GetDlgItem(IDC_MAKE_CALL)->EnableWindow(bEnable);
}

void CMixTonePhoneDlg::OnBnClickedVideoMakeCall()
{
	// TODO: 在此添加控件通知处理程序代码
	pjsua_acc_id acc_id = 0;
	CreatVideoWind();

	UpdateData(TRUE);
	if (m_PhoneNumber.IsEmpty())
		return;

	CString strDsturi = _T("");
	strDsturi.Format(_T("sip:%s@%s"), m_PhoneNumber, m_strDoMain);
	pj_str_t dst_uri = StrToPjStr(strDsturi);


	//设置 呼叫属性
	pjsua_call_setting call_setting;
	pjsua_call_setting_default(&call_setting);
	//call_setting.flag = 0;
	call_setting.vid_cnt = 1;//是否启动视频

	if (call_setting.vid_cnt)
	{
		OpenCloseVideoPan();
	}


	pjsua_call_make_call(acc_id, &dst_uri, &call_setting, NULL, NULL, NULL);
}

void CMixTonePhoneDlg::PlaySound()
{
		LPCTSTR lpstrWav = NULL;
		// IDR_WAVE1 是资源里音频文件的名称
		lpstrWav = MAKEINTRESOURCE(IDR_WAVE1);
		// playsound第二个参数是获得当前应用程序的模块句柄。
		// playsound第三个参数添加参数SND_RESOURCE，指明声音来至程序的资源文件里
		::PlaySound(lpstrWav, GetModuleHandle(NULL), SND_RESOURCE | SND_NODEFAULT | SND_NOWAIT | SND_ASYNC);
}


void CMixTonePhoneDlg::StopSound()
{
	sndPlaySound(NULL, NULL); //停止
}