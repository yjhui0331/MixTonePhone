// SettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MixTonePhone.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"


// CSettingsDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTINGS_DIALOG, pParent)
	, m_nNumber(0)
	, m_strPassword(_T(""))
	, m_strServerAddress(_T(""))
	, m_nPort(0)
	, m_strUserName(_T(""))
{
	m_bExist = FALSE;
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
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_strUserName);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingsDlg ��Ϣ�������


BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_strSettingFilePath = GetSettingFilePath();

	//load xmlSettingfile
	pugi::xml_document xmlDoc;
	if (!xmlDoc.load_file(m_strSettingFilePath, pugi::parse_default, pugi::encoding_utf8))
	{
		m_nNumber = 1000;
		m_nPort = 5060;
		m_bExist = FALSE;
		UpdateData(FALSE);
		return FALSE;
	}
	else
	{
		pugi::xml_node nodeRoot = xmlDoc.child(_T("Account"));
		pugi::xml_node nodeNumber = nodeRoot.child(_T("Number"));
		m_nNumber = nodeNumber.text().as_int();
		pugi::xml_node nodeUserName = nodeRoot.child(_T("UserName"));
		m_strUserName = nodeUserName.text().as_string();
		pugi::xml_node nodePassword = nodeRoot.child(_T("Password"));
		m_strPassword = nodePassword.text().as_string();
		pugi::xml_node nodeDoMain = nodeRoot.child(_T("DoMain"));
		m_strServerAddress = nodeDoMain.text().as_string();
		pugi::xml_node nodePort = nodeRoot.child(_T("Port"));
		m_nPort = nodePort.text().as_int();
		m_bExist = TRUE;
	}
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CSettingsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	if (m_strServerAddress.IsEmpty())
	{
		AfxMessageBox(_T("���������ò���Ϊ��!"));
		return;
	}

	CString strmessage = _T("");
	strmessage.Format(_T("m_nNumber = %d , m_strPassword = %s ,m_strServerAddress\
			= %s , m_nPort = %d"), m_nNumber, m_strPassword, m_strServerAddress, m_nPort);

	// save xmlSettingfile [4/18/2018 yangjianhui]
	pugi::xml_document xmlDoc;
	pugi::xml_node nodeRoot = xmlDoc.append_child(_T("Account"));
	// ����
	pugi::xml_node pre = xmlDoc.prepend_child(pugi::node_declaration);
	pre.append_attribute(_T("version")) = _T("1.0");
	pre.append_attribute(_T("encoding")) = _T("utf-8");

	// ע�ͽڵ�1
	pugi::xml_node nodeCommentAccount = nodeRoot.append_child(pugi::node_comment);
	nodeCommentAccount.set_value(_T("Account info"));
	// ��ͨ�ڵ� Number
	pugi::xml_node nodeNumber = nodeRoot.append_child(_T("Number"));
	CString strNumber = _T("");
	strNumber.Format(_T("%d"), m_nNumber);
// 	nodeStudents.set_value(strNumber.GetBuffer());
	nodeNumber.text().set(strNumber);

	// ��ͨ�ڵ� UserName
	pugi::xml_node nodeUserName = nodeRoot.append_child(_T("UserName"));
	nodeUserName.text().set(m_strUserName);


	// ��ͨ�ڵ� Password
	pugi::xml_node nodePassword = nodeRoot.append_child(_T("Password"));
	nodePassword.text().set(m_strPassword);

	// ��ͨ�ڵ� DoMain
	pugi::xml_node nodeDoMain = nodeRoot.append_child(_T("DoMain"));
	nodeDoMain.text().set(m_strServerAddress);


	// ��ͨ�ڵ� Port
	pugi::xml_node nodePort = nodeRoot.append_child(_T("Port"));
	CString strPort = _T("");
	strPort.Format(_T("%d"), m_nPort);
	nodePort.text().set(strPort);

	//reset register
	LPMT_ACCOUNT pAccount = new MT_ACCOUNT;
	pAccount->nNumber = m_nNumber;
	pAccount->strUserName = m_strUserName;
	pAccount->strPassword = m_strPassword;
	pAccount->strDoMain = m_strServerAddress;
	pAccount->nPort = m_nPort;

	if (!m_bExist)
		g_pMixTonePhoneDlg->addAccount(pAccount);
	else
		g_pMixTonePhoneDlg->modifyAccount(pAccount);

	//pjsua_acc_add
	delete pAccount;

	xmlDoc.save_file(m_strSettingFilePath, _T("\t"), 1U, pugi::encoding_utf8);

	CDialogEx::OnOK();
}
