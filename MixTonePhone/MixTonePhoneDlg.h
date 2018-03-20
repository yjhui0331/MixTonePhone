
// MixTonePhoneDlg.h: 头文件
//

#pragma once

#define WM_MT_EVENT	WM_USER + 101

//Event Type
//预留sys event
#define MT_EVENT_INCOMINGCALL	0x10


// CMixTonePhoneDlg 对话框
class CMixTonePhoneDlg : public CDialog
{
// 构造
public:
	CMixTonePhoneDlg(CWnd* pParent = NULL);	// 标准构造函数

	static void __stdcall DispEventCallback(int nEvent, int nError,
		int nDataLen, void *pData, unsigned int uTag, void *param);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIXTONEPHONE_DIALOG };
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

	// Generated message map functions
	afx_msg LRESULT OnDispEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	int MixTonePhoneCreate();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	CString m_ConsoleMessage;

	afx_msg void OnBnClickedAnswerCall();

public:
	typedef struct tagMT_INCOMINGCALL
	{
		pjsua_acc_id acc_id;
		pjsua_call_id call_id;
		pjsip_rx_data *rdata;
	}MT_INCOMINGCALL, *LPMT_INCOMINGCALL;
	afx_msg void OnBnClickedMakeCall();
	afx_msg void OnBnClickedHangupCall();
	CString m_PhoneNumber;
};
