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
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序
