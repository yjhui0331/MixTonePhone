#ifndef __MIX_TONE_PHONE_
#define __MIX_TONE_PHONE_

//#include "include\pjlib\include\"

using namespace std;

#include <pjsua-lib/pjsua.h>
//#include "..\pjproject\include\pjsip\include\pjsua-lib\pjsua.h"


#ifdef _DEBUG
#pragma comment(lib, "libpjproject-i386-Win32-vc14-Debug.lib")
#else
#pragma comment(lib, "libpjproject-i386-Win32-vc14-Release.lib")
#endif


#define THIS_FILE	"APP"

#define SIP_DOMAIN	"192.168.50.200"
#define SIP_USER	"5011"
#define SIP_PASSWD	"5011"



#endif