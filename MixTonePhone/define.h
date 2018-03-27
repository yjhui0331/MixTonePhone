#ifndef __MIX_TONE_PHONE_
#define __MIX_TONE_PHONE_

//#include "include\pjlib\include\"

#include "utf.h"
#include <stdio.h>

using namespace std;

#include <pjsua-lib/pjsua.h>
//#include <pj/string.h>
//#include "..\pjproject\include\pjsip\include\pjsua-lib\pjsua.h"


#ifdef _DEBUG
#pragma comment(lib, "libpjproject-i386-Win32-vc14-Debug.lib")
#else
#pragma comment(lib, "libpjproject-i386-Win32-vc14-Release.lib")
#endif


#define THIS_FILE	"APP"

#define SIP_DOMAIN	_T("192.168.50.200")
#define SIP_USER	_T("5011")
#define SIP_PASSWD	_T("5011")
#define SIP_PORT    5060


// #define SIP_DOMAIN	"wh82011.f3322.net"
// #define SIP_USER	"1003"
// #define SIP_PASSWD	"1234"
// #define SIP_PORT    5060


typedef struct tagMT_INCOMINGCALL
{
	pjsua_acc_id acc_id;
	pjsua_call_id call_id;
	pjsip_rx_data *rdata;
}MT_INCOMINGCALL, *LPMT_INCOMINGCALL;


#endif