#ifndef __MIX_TONE_GLOBAL_
#define __MIX_TONE_GLOBAL_

#pragma once

#include "define.h"

#include <pjsua-lib/pjsua.h>
#include <pjsua-lib/pjsua_internal.h>


CString PjToStr(const pj_str_t* str, BOOL utf = FALSE);
pj_str_t StrToPjStr(CString str);
char* StrToPj(CString str);

//CString AnsiToUnicode(CStringA str);
CString XMLEntityDecode(CString str);
CString XMLEntityEncode(CString str);

CString GetSettingFilePath();

CString GetGetModuleFilePath();


BOOL LoadSetting(LPMT_ACCOUNT pAccount);

#endif
