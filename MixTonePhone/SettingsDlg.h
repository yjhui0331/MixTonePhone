#pragma once


// CSettingsDlg �Ի���

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nNumber;
	CString m_strPassword;
	CString m_strServerAddress;
	int m_nPort;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
