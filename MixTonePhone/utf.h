#ifndef __MIX_TONE_UTF_
#define __MIX_TONE_UTF_

#pragma once

char* Utf8DecodeCP(char* str, int codepage, wchar_t** ucs2);
char* Utf8EncodeCP(const char* src, int codepage);
char* Utf8EncodeUcs2(const wchar_t* src);

#endif // !1