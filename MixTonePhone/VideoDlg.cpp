// VideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MixTonePhone.h"
#include "VideoDlg.h"
#include "afxdialogex.h"


// CVideoDlg �Ի���

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIDEO_DIALOG, pParent)
{
	COLORREF color = RGB(0,0,0);

	this->SetBackgroundColor(color);
}

CVideoDlg::~CVideoDlg()
{
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
END_MESSAGE_MAP()


// CVideoDlg ��Ϣ�������
