#include "stdafx.h"
#include "global.h"
#include "utf.h"


CString PjToStr(const pj_str_t* str, BOOL utf)
{
	CStringA rab;
	rab.Format("%.*s", str->slen, str->ptr);
	if (utf)
	{
#ifdef _UNICODE
		WCHAR* msg;
		Utf8DecodeCP(rab.GetBuffer(), CP_ACP, &msg);
		return msg;
#else
		return Utf8DecodeCP(rab.GetBuffer(), CP_ACP, NULL);
#endif
	}
	else
	{
		return CString(rab);
	}
}



pj_str_t StrToPjStr(CString str)
{
	return pj_str(StrToPj(str));
}

char* StrToPj(CString str)
{
#ifdef _UNICODE
	return Utf8EncodeUcs2(str.GetBuffer());
#else
	return Utf8EncodeCP(str.GetBuffer(), CP_ACP);
#endif
}

CString XMLEntityDecode(CString str)
{
	str.Replace(_T("&lt;"), _T("<"));
	str.Replace(_T("&gt;"), _T(">"));
	str.Replace(_T("&quot;"), _T("\""));
	str.Replace(_T("&amp;"), _T("&"));
	return str;
}

CString XMLEntityEncode(CString str)
{
	str.Replace(_T("&"), _T("&amp;"));
	str.Replace(_T("<"), _T("&lt;"));
	str.Replace(_T(">"), _T("&gt;"));
	str.Replace(_T("\""), _T("&quot;"));
	return str;
}

CString GetSettingFilePath()
{
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

	return settingFilePath;
}


CString GetGetModuleFilePath()
{
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

	return strpathExe;
}

BOOL LoadSetting(LPMT_ACCOUNT pAccount)
{
	if (!pAccount)
		return FALSE;

	//load xmlSettingfile
	pugi::xml_document xmlDoc;
	if (!xmlDoc.load_file(GetSettingFilePath(), pugi::parse_default, pugi::encoding_utf8))
	{
		return FALSE;
	}
	else
	{
		pugi::xml_node nodeRoot = xmlDoc.child(_T("Account"));
		pugi::xml_node nodeNumber = nodeRoot.child(_T("Number"));
		pAccount->nNumber = nodeNumber.text().as_int();
		pugi::xml_node nodeUserName = nodeRoot.child(_T("UserName"));
		pAccount->strUserName = nodeUserName.text().as_string();
		pugi::xml_node nodePassword = nodeRoot.child(_T("Password"));
		pAccount->strPassword = nodePassword.text().as_string();
		pugi::xml_node nodeDoMain = nodeRoot.child(_T("DoMain"));
		pAccount->strDoMain = nodeDoMain.text().as_string();
		pugi::xml_node nodePort = nodeRoot.child(_T("Port"));
		pAccount->nPort = nodePort.text().as_int();
		return TRUE;
	}
}