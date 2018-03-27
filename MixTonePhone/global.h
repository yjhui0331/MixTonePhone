#ifndef __MIX_TONE_GLOBAL_
#define __MIX_TONE_GLOBAL_

#pragma once

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
// class global
// {
// public:
// 	global();
// 	~global();
// };

CString PjToStr(const pj_str_t* str, BOOL utf = FALSE);
pj_str_t StrToPjStr(CString str);
char* StrToPj(CString str);




#endif
