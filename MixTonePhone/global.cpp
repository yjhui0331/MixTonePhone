#include "stdafx.h"
#include "global.h"
#include "utf.h"

// global::global()
// {
// }
// 
// 
// global::~global()
// {
// }

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

