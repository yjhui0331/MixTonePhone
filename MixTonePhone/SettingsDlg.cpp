// SettingsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MixTonePhone.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"


// CSettingsDlg 对话框

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, m_nNumber(0)
	, m_strPassword(_T(""))
	, m_strServerAddress(_T(""))
	, m_nPort(0)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUMBER_EDIT, m_nNumber);
	DDX_Text(pDX, IDC_PASSWROD_EDIT, m_strPassword);
	DDX_Text(pDX, IDC_SERVER_EDIT, m_strServerAddress);
	DDX_Text(pDX, IDC_PORT_EDIT, m_nPort);
	DDV_MinMaxInt(pDX, m_nPort, 1, 65535);
	DDV_MinMaxInt(pDX, m_nNumber, 1, INT_MAX);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序


BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString strexeFile = _T("");
	LPTSTR ptr;
	ptr = strexeFile.GetBuffer(MAX_PATH);
	GetModuleFileName(NULL, ptr, MAX_PATH);
	strexeFile.ReleaseBuffer();
	//--
	CString strpathExe = _T("");
	ptr = strpathExe.GetBuffer(MAX_PATH + 1);
	GetModuleFileName(NULL, ptr, MAX_PATH);
	strpathExe.ReleaseBuffer();
	strpathExe = strexeFile.Mid(0, strexeFile.ReverseFind('\\'));
	//--
	CString fileName = PathFindFileName(strexeFile);
	fileName = fileName.Mid(0, fileName.ReverseFind('.'));
	CString settingFile = _T("");
	settingFile.Format(_T("%s.xml"), fileName);

	CString settingFilePath = _T("");
	settingFilePath.Format(_T("%s\\%s"), strpathExe, settingFile);

	CXMLFile xmlFile;
	if (xmlFile.LoadFromFile(settingFilePath.GetBuffer()))//加载xml文件
	{
		
	}
	else
	{
		// 打开文件失败创建新文件 [4/18/2018 yangjianhui]
		CXMLElement* xmlRoot = new CXMLElement();
		xmlRoot->Create(_T("XML:ROOT"), XET_TAG);
		CXMLElement* xmlContacts = new CXMLElement();
		xmlContacts->Create(_T("Account"), XET_TAG);
		xmlRoot->AppendChild(xmlContacts);
		xmlFile.SetRoot(xmlRoot);

		xmlFile.SaveToFile(settingFilePath.GetBuffer());
	}

	GetDlgItem(IDC_NUMBER_EDIT)->SetWindowText(_T(""));
	CEdit* pPortEdit = (CEdit*)GetDlgItem(IDC_PORT_EDIT);
	pPortEdit->SetSel(0, -1);
	pPortEdit->Clear();
	
	m_nNumber = 1000;
	m_nPort = 5060;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CSettingsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
	{
		return;
	}

	CString strmessage = _T("");
	strmessage.Format(_T("m_nNumber = %d , m_strPassword = %s ,m_strServerAddress\
			= %s , m_nPort = %d"), m_nNumber, m_strPassword, m_strServerAddress, m_nPort);

	AfxMessageBox(strmessage);


	CDialogEx::OnOK();
}
