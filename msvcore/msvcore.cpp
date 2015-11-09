/*	Copyright (C) 2004-2100 Mishael Senin, aka MikelSV.  All rights reserved.

	Ћицензи€ попул€ризации автора. :]
	¬ы не должны измен€ть им€ и другую информацию о разработчике и не присваивать авторство себе.
	Ќе продавать и не брать денег за код из данной библиотеки.
	–азрешаетс€ модифицировать и дорабатывать.
	 од предоставлен как есть и автор не отвечает, даже если вы попытаетесь прострелить себе ногу.

	License popularizing author. [LPA]
	You should not change the name and other information about the developer and not assign authorship itself.
	Do not sell and do not take money for the code from this library.
	Permission is granted to modify and refine.
	Author bears no responsibility for any consequences of using this code.
	Responsibility for shooting through your leg lies entirely on you.
* /

//>> Velcome to MSV Lib. <<
//>> New name MSV Core <<

//>> So. What you want?
//>> I included Strings for You.
//>> What can i included for you?

// !!!!!>>>>>>>>>>> Add this to general cpp: #define USEMSV_GENERALCPP
//					Add to project: "..\..\opensource\msvcore\VString.cpp" "..\..\opensource\msvcore\MString.cpp"


// ----------------------------------------------------------------- Simple Variant ---------------------------
/*
#define USEMSV_GENERALCPP
#define PROJECTNAME "projectname"
#define PROJECTVER PROJECTNAME ## _versions

#include "../../opensource/msvcore/msvcore.cpp"

Versions PROJECTVER[]={
	// new version to up
	"0.0.0.1", "10.10.2013 21:24"
};

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link);
	print(PROJECTNAME, " v.", PROJECTVER[0].ver," (", PROJECTVER[0].date, ").\r\n");
	return 0;
}
*/
// ----------------------------------------------------------------- End of Simple Variant -----------------------

// Extensions
// #define USEMSV_ANDOID - for android apps
// #define USEMSV_LOGPRINT - save output data to memory buffer
// #define USEMSV_FILEPRINT - save output data to file
// #define USEMSV_FLASH - use for Crossbridge compile

// Teplates:
// #define USEMSV_ITOS	- use Itos templates

// Formats:
// #define USEMSV_XDATACONT - use XDataContainer ( Parsers: XML, Json )
// #define USEMSV_AMF - use amf encoder/decoder
// #define USEMSV_NMEA - use nmea functions
// #define USEMSV_CJX - use CJX container

// Other: 
// #define USEMSV_MSL_FL - use MSL Fast Line
// #define USEMSV_MSL_FV - use MSL Five
// #define USEMSV_CONFLINE - use ConfLine
// #define USEMSV_MODLINE - use ModLine
// #define USEMSV_CPPXCC - use CppXcc preprocessor

// Lists:
// #define USEMSV_ILIST - use IList template dunamic array. Vector, one resized memory block.
// #define USEMSV_MLIST - use MList template dunamic array
// #define USEMSV_OLIST - use OList template dunamic array
// #define USEMSV_ULIST - use UList template dynamic array. // defined by default 
// #define USEMSV_FSTRING - use FString class // defined by default 

// Server:
// #define USEMSV_STORMSERVER - use storm server
// #define USEMSV_HTTP - use http functions
// #define USEMSV_WEBSOCKETS - use WebSockets client and listen_websockets for storm
// #define USEMSV_LIGHTSERVER - use light server

// Network
// #define USEMSV_TRAFFIX - use Traffix class

// Extensions:
// #define USEMSV_PCRE - use PCRE functions // add to include path to pcre.h and pcre.lib
// #define USEMSV_MYSQL - use MySQL // add to include path to mysql.h and mysql.lib
// #define USEMSV_OPENSSL - use openssl // add to include path: ..\..\openssl-1.0.2\include

// Console:
// #define USEMSV_CONSOLE - use console functions
// #define USEMSV_CONSOLELINE - use console windows

// Memory:
// #define USEMSV_MEMORYCONTROL - interception malloc() & free()
// #define USEMSV_INTERCEPT_MALLOC - interception malloc() & free() // don't work

// API
// #define USEMSV_NESTAPI - use Nest API
// #define USEMSV_NESTAPI2 - use Nest API v.2

// Special:
// #define USEMSV_BUFECHO - Input / Output buffer echo control
// #define USEMSV_TESTS - Testing

#ifndef PROJECTNAME
	#error Please set #define PROJECTNAME "you_project_name"
#endif

// Include Defines
#include "msvdefine.h"

// Include Virtual Strings
#include "VString.h"

// Include Real Strings
#include "MString.h"

// Include Temp or Thread String
#include "TString.h"

// Include Defines uses VString & MString
// Include Defines Mobile (???)
#include "msvdefine_m.h"

// Include 4D Strings
// #include...


#ifdef USEMSV_GENERALCPP

// MMatrix
#include "list/MMatrix.h"
#include "list/MMatrixCore.h"

#ifdef USEMSV_FLASH
	#include "flash/flash.cpp"
#endif

#include "list/UList.cpp"
#include "msvdefine.cpp"

#include "crossplatform/treads.cpp"
//#include "crossplatform-filefunc.cpp"
#include "crossplatform/mscr.cpp"
#include "crossplatform/send.cpp"
//#include "msvio.h"
//#include "msvnet.h"
#include "crossplatform/msvnet.cpp"

//#include "list/FString.cpp"
//#include "list/TString.cpp" // Thread String

#ifdef USEMSV_ITOS
	#include "crossplatform/itos.cpp"
#endif

// intercept malloc & free
#ifdef USEMSV_INTERCEPT
	#include "special/asmp.h"
	#include "special/interception.h"
#endif

#ifdef USEMSV_CPPXCC 
	#include "msl/xcc.h"
#endif

#ifdef USEMSV_INTERCEPT_MALLOC
	#include "special/intercept-malloc.h"
#endif

// Inject Dll
#ifdef USEMSV_INJECTDLL
	#include "crossplatform/injectdll.cpp"
#endif

#ifdef USEMSV_XDATACONT
	#include "proto/xdatacont.cpp"
#endif

#ifdef USEMSV_CJX
	#include "proto/cjx.cpp"
#endif

#ifdef USEMSV_MLIST
	#include "list/MList.h"
#endif

#ifdef USEMSV_ILIST
	#include "list/IList.h"
#endif

#ifdef USEMSV_OLIST
	#include "list/OList.h"
	//#include "list/OList3.h"
#endif

//#ifdef USEMSV_ULIST
//	#include "list/UList.h"
//#endif

#ifdef USEMSV_AMF
	#include "proto/amf.cpp"
#endif

#ifdef USEMSV_PCRE
	#include "special/pcre-include.h"
#endif

#ifdef USEMSV_CONSOLE
	#include "crossplatform/console.cpp"
#endif

#ifdef USEMSV_CONSOLELINE
	#include "line/consoleline.cpp"
	#include "line/ConsoleWindow.cpp"
#endif

#ifdef USEMSV_OPENSSL
	#include "crossplatform/openssl.cpp"
#endif

#ifdef USEMSV_MODLINE
	#include "cntx/modline.cpp"
#endif

#ifdef USEMSV_WEBSOCKETS
	#include "proto/websockets.cpp"
#endif

#ifdef USEMSV_MYSQL
	#include "proto/mysql-con.cpp"
#endif

#ifdef USEMSV_MSL_FL
	#include "msl-fl/msl-fl.h"
#endif

#ifdef USEMSV_MSL_FV
	#include "msl/msl-five.h"
#endif

#ifdef USEMSV_HTTP
	#include "crossplatform/iheader.cpp"
	#include "crossplatform/http.cpp"
#endif

#ifdef USEMSV_CONFLINE
	#include "special/confline.cpp"
#endif

#ifdef USEMSV_NESTAPI
	#include "proto/nest_api.h"
#endif

#ifdef USEMSV_NESTAPI2
	#include "proto/nest_api2.h"
#endif

#ifdef USEMSV_STORMSERVER
	#include "server/storm/storm-include.h"
#endif

#ifdef USEMSV_LIGHTSERVER
	#include "server/light/light.h"
#endif

#ifdef USEMSV_ANDOID
	#include "android/device.cpp"
#endif

#ifdef USEMSV_TRAFFIX
	#include "net/traffix.h"
#endif

#ifdef USEMSV_NMEA
	#include "sat/nmea.cpp"
#endif

#ifdef USEMSV_BUFECHO
	#include "special/bufecho.cpp"
#endif

#ifdef USEMSV_TESTS
	#include "special/tests.cpp"
#endif


#endif



// NEED


