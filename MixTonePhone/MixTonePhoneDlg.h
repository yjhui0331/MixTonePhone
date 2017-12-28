
// MixTonePhoneDlg.h: 头文件
//

#pragma once


// CMixTonePhoneDlg 对话框
class CMixTonePhoneDlg : public CDialog
{
// 构造
public:
	CMixTonePhoneDlg(CWnd* pParent = NULL);	// 标准构造函数

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
	DECLARE_MESSAGE_MAP()
public:
	int MixTonePhoneCreate();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
