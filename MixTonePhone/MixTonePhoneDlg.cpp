
// MixTonePhoneDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MixTonePhone.h"
#include "MixTonePhoneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMixTonePhoneDlg *g_pMixTonePhone = NULL;

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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ConsoleMessage = _T("");
}

void CMixTonePhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONSOLE_MESSAGE, m_ConsoleMessage);
	DDX_Text(pDX, IDC_PHONE_NUMBER, m_PhoneNumber);
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

	g_pMixTonePhone = this;
	MixTonePhoneCreate();

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
			m_ConsoleMessage.Format("Incoming call from %.*s \r\n",
				(int)ci.remote_info.slen, ci.remote_info.ptr);

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

// 	pjsua_call_get_info(call_id, &ci);
// 
// 	PJ_LOG(3, (THIS_FILE, "Incoming call from %.*s!!",
// 		(int)ci.remote_info.slen,ci.remote_info.ptr));

	LPMT_INCOMINGCALL pInComingCall = new MT_INCOMINGCALL;
	pInComingCall->acc_id = acc_id;
	pInComingCall->call_id = call_id;
	pInComingCall->rdata = rdata;

	pjsua_call_answer(call_id, 180, NULL, NULL);
	
	g_pMixTonePhone->PostMessage(WM_MT_EVENT, (WPARAM)MT_EVENT_INCOMINGCALL, (LPARAM)pInComingCall);
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

	CString strMessage = "";
	strMessage.Format("Call %d state=%.*s \r\n", call_id,
		(int)ci.state_text.slen,ci.state_text.ptr);

	g_pMixTonePhone->m_ConsoleMessage += strMessage;

	CWnd * pConsoleMessage = g_pMixTonePhone->GetDlgItem(IDC_CONSOLE_MESSAGE);
	if (pConsoleMessage)
		pConsoleMessage->SetWindowText(g_pMixTonePhone->m_ConsoleMessage);
}

static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
	if (g_pMixTonePhone != NULL)
	{

		//pjsip_status_code info->cbparam->code
		if (PJSIP_SC_OK == info->cbparam->code)
		{
			g_pMixTonePhone->SetWindowText(SIP_USER" 在线");
		}
		else
		{
			g_pMixTonePhone->SetWindowText(SIP_USER" 离线");
		}
	}
}

/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
	pjsua_call_info ci;

	pjsua_call_get_info(call_id, &ci);

	
	//Remote video
	int vid_idx; pjsua_vid_win_id Rwid;
	vid_idx = pjsua_call_get_vid_stream_idx(call_id);
	if (vid_idx >= 0)
	{
		pjsua_call_info ci;
		pjsua_call_get_info(0, &ci);
		Rwid = ci.media[vid_idx].stream.vid.win_in;
	}
	pjsua_vid_win_info rwi;
	pjsua_vid_win_get_info(Rwid, &rwi);

	CRect rcRClient, rcRWind;	
	pjmedia_coord Rpos = { 0, 0 };
	CWnd * RemoteVideoWnd = g_pMixTonePhone->GetDlgItem(IDC_REMOTE_VIDEO_WIN);
	if (RemoteVideoWnd)
	{
		RemoteVideoWnd->GetClientRect(&rcRClient);
		RemoteVideoWnd->GetWindowRect(&rcRWind);
		//Rpos.x = rcRWind.left;
		//Rpos.y = rcRWind.right;
	}



	pjsua_vid_win_set_pos(Rwid, &Rpos);

	pjmedia_rect_size size = { 320, 240 };
	size.h = rcRClient.Height();
	size.w = rcRClient.Width();
	pjsua_vid_win_set_size(Rwid, &size);

	
	::SetParent((HWND)rwi.hwnd.info.win.hwnd, RemoteVideoWnd->m_hWnd);

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
	ua_cfg.max_calls = 1;
	ua_cfg.user_agent = pj_str("Mix Tone");
	ua_cfg.cb.on_incoming_call = &on_incoming_call;//通知 有呼叫到来
	ua_cfg.cb.on_call_media_state = &on_call_media_state;//媒体状态回调函数
	ua_cfg.cb.on_call_state = &on_call_state;//通知 呼叫状态变化
	ua_cfg.cb.on_reg_state2 = &on_reg_state2;//注册状态

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


	// 计划单独封装登录功能 [12/28/2017 yangjianhui]
	/* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS)
	{
		//"Error starting pjsua"
		m_ConsoleMessage = "Error starting pjsua";
		UpdateData(FALSE);
		return false;
	}

	/* Add UDP transport. */
	pjsua_transport_config transport_cfg;
	pjsua_transport_config_default(&transport_cfg);
	transport_cfg.port = SIP_PORT;
	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, NULL);
	if (status != PJ_SUCCESS)
	{
		//"Error creating transport";
		m_ConsoleMessage = "Error creating transport";
		UpdateData(FALSE);
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
	acc_cfg.vid_in_auto_show = PJ_TRUE;
	acc_cfg.vid_cap_dev = 0; // = PJ_TRUE;
	acc_cfg.vid_out_auto_transmit = PJ_TRUE;

	status = pjsua_acc_add(&acc_cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS)
	{
		//"Error adding account"
		m_ConsoleMessage = "Error adding account";

		UpdateData(FALSE);

		return false;
	}

	return false;

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

void CMixTonePhoneDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/* Destroy pjsua */
	pjsua_destroy();

	CDialog::OnClose();
}


void CMixTonePhoneDlg::OnBnClickedAnswerCall()
{

	/* Automatically answer incoming calls with 200/OK */
	pjsua_call_answer(0, 200, NULL, NULL);

	
}


void CMixTonePhoneDlg::OnBnClickedMakeCall()
{
	UpdateData(TRUE);
	if (m_PhoneNumber.IsEmpty())
		return;

	CString strDsturi = "";
	strDsturi.Format("sip:%s@%s", m_PhoneNumber,SIP_DOMAIN);
	pj_str_t dst_uri = pj_str(strDsturi.GetBuffer(strDsturi.GetLength()));


	//设置 呼叫属性
	pjsua_call_setting call_setting;
	pjsua_call_setting_default(&call_setting);
	call_setting.flag = 0;
	call_setting.vid_cnt = 1;//是否启动视频


	pjsua_call_make_call(0, &dst_uri, &call_setting, NULL, NULL, NULL);
		
	/*

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
		//pos.x = rcWind.left;
		//pos.y = rcWind.right;
		pLocalVideoWnd->ShowWindow(SW_HIDE);
		return;
	}

	pjsua_vid_preview_param_default(&pre_param);

	pre_param.rend_id = PJMEDIA_VID_DEFAULT_RENDER_DEV;
	pre_param.show = PJ_TRUE; 
	status = pjsua_vid_preview_start(PJMEDIA_VID_DEFAULT_CAPTURE_DEV, &pre_param);
	wid = pjsua_vid_preview_get_win(widx);
	pjsua_vid_win_get_info(wid, &wi);



	pjmedia_rect_size size = { 320, 240 };
	size.h = rcClient.Height();
	size.w = rcClient.Width();
	pjsua_vid_win_set_size(wid, &size);
	pjsua_vid_win_set_pos(wid, &pos);
	::SetParent((HWND)wi.hwnd.info.win.hwnd, pLocalVideoWnd->m_hWnd);

	pLocalVideoWnd->ShowWindow(SW_SHOW);
	*/

	/*
	//Remote video
	int vid_idx; pjsua_vid_win_id Rwid;
	vid_idx = pjsua_call_get_vid_stream_idx(0);
	if (vid_idx >= 0)
	{
		pjsua_call_info ci;
		pjsua_call_get_info(0, &ci);
		Rwid = ci.media[vid_idx].stream.vid.win_in;
	}
	pjsua_vid_win_info rwi;
	pjsua_vid_win_get_info(Rwid,&rwi);

	CRect rcRClient, rcRWind;
	CWnd * RemoteVideoWnd = GetDlgItem(IDC_REMOTE_VIDEO_WIN);
	if (RemoteVideoWnd)
	{
		RemoteVideoWnd->GetClientRect(&rcRClient);
		RemoteVideoWnd->GetWindowRect(&rcRWind);
		//pos.x = rcWind.left;
		//pos.y = rcWind.right;
	}
	pjmedia_coord Rpos = { 0, 0 };
	pjsua_vid_win_set_pos(Rwid, &Rpos);


	::SetParent((HWND)rwi.hwnd.info.win.hwnd, RemoteVideoWnd->m_hWnd);
	*/
}


void CMixTonePhoneDlg::OnBnClickedHangupCall()
{
	// TODO: 在此添加控件通知处理程序代码
	//pjsua_call_hangup();

	pjsua_call_hangup_all();

	CWnd * RemoteVideoWnd = GetDlgItem(IDC_REMOTE_VIDEO_WIN);
	RemoteVideoWnd->Invalidate();
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
		//pos.x = rcWind.left;
		//pos.y = rcWind.right;
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

	::SetParent((HWND)wi.hwnd.info.win.hwnd, pLocalVideoWnd->m_hWnd);
	wid = pjsua_vid_preview_get_win(widx);

	pLocalVideoWnd->ShowWindow(SW_SHOW);

	



	//---对端视频
	
	
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

	//pjsua_vid_win_set_show(wid, PJ_FALSE);
	pjsua_vid_preview_stop(PJMEDIA_VID_DEFAULT_CAPTURE_DEV);
	
	::InvalidateRect((HWND)wi.hwnd.info.win.hwnd, &rcClient, true);
	::InvalidateRect(pLocalVideoWnd->m_hWnd, &rcClient, true);
	pLocalVideoWnd->Invalidate();
	pLocalVideoWnd->UpdateWindow();
	::UpdateWindow((HWND)wi.hwnd.info.win.hwnd);
	::ShowWindow((HWND)wi.hwnd.info.win.hwnd, SW_HIDE);
	pLocalVideoWnd->ShowWindow(SW_HIDE);
}
