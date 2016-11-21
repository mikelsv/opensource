// Server logic
// Add port + set protocol type
// Accept(), select protocol
// new storm_core() class, init(), start()
// wait socket, recv(), storm_core()->recv();
// storm_core()->send();
// storm_core()->close(), stop();

#ifndef STORMSERVER_THREADS_MAX
	#define STORMSERVER_THREADS_MAX 16
#endif

#ifndef STORMSERVER_THREADS_MIN
	#define STORMSERVER_THREADS_MIN 4
#endif

#ifndef STORMSERVER_STACK_BUFF
	#define STORMSERVER_STACK_BUFF S32K
#endif

#ifndef STORMSERVER_SOCKETBLOCK_SZ
	#define STORMSERVER_SOCKETBLOCK_SZ S4K
#endif

#if !defined(STORMSERVER_POLL_AUTO) && !defined(STORMSERVER_POLL_SELECT) && !defined(STORMSERVER_POLL_EPOLL) && !defined(STORMSERVER_POLL_IOCP)
	#define STORMSERVER_POLL_AUTO
	//#error "Set poll type in Storm Server, use: #define STORMSERVER_POLL_AUTO // for auto select or STORMSERVER_POLL_SELECT, STORMSERVER_POLL_EPOLL and STORMSERVER_POLL_IOCP."
#endif

#ifdef STORMSERVER_POLL_AUTO
	#ifdef WIN32
		#define STORMSERVER_POLL_SELECT
		//#define STORMSERVER_POLL_IOCP
	#else
		#define STORMSERVER_POLL_EPOLL
		//#define STORMSERVER_POLL_SELECT
	#endif
#endif

#ifdef STORMSERVER_POLL_EPOLL
	#include <sys/epoll.h>
#endif

#ifdef STORMSERVER_POLL_SELECT
	VString stormserver_poll = "select";
#endif
#ifdef STORMSERVER_POLL_EPOLL
	VString stormserver_poll = "epoll";
#endif
#ifdef STORMSERVER_POLL_IOCP
	VString stormserver_poll = "iocp";
#endif

#ifndef STORMSERVER_CONFIG
#define STORMSERVER_CONFIG "stormserver.conf"
#endif


// TMP
// poll [sz]. 
#define AIFOP_RECV	(1<<16)	// >> << /\ -
#define AIFOP_SEND	(1<<17)	// >> << /\ -
#define AIFOP_ERR	(1<<0)	// >> << /\ -
#define AIFOP_TIME	(1<<1)	//    << /\ -
// +0
#define AIFOP_L1	(1<<2)	// 0 >> 1 xx
#define AIFOP_LU	(1<<3)	// 1 used xx
//#define AIFOP_UNLIM	64  // >>
//#define AIFOP_END (1<<4) // /\ -
#define AIFOP_CLOSE	(1<<5) // >> /\ -
//#define AIFOP_DEL (1<<6) // /\ -
//#define AIFOP_AUTO (1<<7) // /\ -
#define AIFOP_POLL (1<<7) // In poll

//#define AIFOP_LU2	(1<<10)

//#define AIFOP_SIGNAL S10B // mod 

//#define AIFOP_USER	S10B // infunc user signal

#define AIFOP_MAYMOD (AIFOP_RECV|AIFOP_SEND|AIFOP_ERR|AIFOP_TIME)
#define AIFOP_CLEARMOD (AIFOP_L1|AIFOP_LU|AIFOP_TIME)
#define AIFO_ASOCKET_MOVE	16

#ifndef OPENSSL_RECV_WAIT
	#define OPENSSL_RECV_WAIT	(-2147483647)
#endif

// storm mod state
#include "listen-http-modstate.h"

// storm core: based class & pooling sockets
#include "storm-core.h"

// core
StormCore MyStormCore;

// storm server: listen port & accept connections
#include "storm-server.h"

// server
StormServer MyStormServer(MyStormCore);

// crypto ssl
#include "storm-crypto.h"

// storm cmd/cgi
#include "listen-cgi.h"

// storm protocols: raw
#include "listen-raw.h"
// storm protocols: http
#include "listen-http.h"
// storm protocols: proxy
#include "listen-proxy.h"
// storm protocols: cache proxy
#include "listen-proxy-cache.h"

#ifdef USEMSV_STORMSERVER_HTTPJSON_CALL
// storm protocols: http json
#include "listen-http-json.h"
#endif

#ifdef USEMSV_STORMSERVER_SMTP_CALL
// storm protocols: http json
#include "listen-smtp.h"
#endif


#ifdef USEMSV_WEBSOCKETS
// storm protocols: websockets
#include "listen-websockets.h"
#endif

// storm protocols: xml
#ifdef USEMSV_XDATACONT
#include "listen-xml.h"
//#include "listen-xml2s.h"
#endif

#ifdef USEMSV_STORMSERVER_MITM
// storm titm proxy
#include "../../../../closesource/msvcore/server/storm/listen-mitm.h"
#endif

/*
StormCore MyStormCore;
StormServer MyStormServer(MyStormCore);

DWORD StormServerThread(LPVOID){
	MyStormServer.UseConf("stormserver.conf");
//	MyStormServer.InsertProto("raw", listen_raw::static_storm_new());
	MyStormServer.InsertProto("http", listen_http::static_storm_new());
	MyStormServer.Run();

	print("Open port failed. Exit.\r\n");

	Sleep(1000);
	exit(0);
	return 0;
}

// StartThread(StormServerThread);

*/