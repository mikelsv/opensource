// Simple MSV Core

// Numbers
#define S1K 1024
#define S2K 2048
#define S4K 4096
#define S8K 8192
#define S16K 16384
#define S32K 32768
#define S64K 65536
#define S64KM 65535
#define S128K 131072
#define S256K 262144
#define S512K 524288
#define S1M 1048576
#define S2M 2097152
#define S4M 4194304
#define S8M 8388608
#define S16M 16777216
#define S32M 33554432
#define S64M 67108864
#define S128M 134217728
#define S256M 268435456
#define S512M 536870912
#define S1G 1073741824
#define S2G 2147483648u
#define S2GM (S2G-1)
#define S3G (2147483648u+1073741824)
#define S4G 4294967296ull
#define S4GM 4294967295ull
#define S8G (4294967296ull*2)
#define S16G (4294967296ull*4)
#define S32G (4294967296ull*8)
//#define S32G (4294967296ull*8)

// Global include
#include "string.h"
#include "wchar.h"
#include "stdlib.h"

// Defines
#ifdef WIN32
#ifndef __GNUC__
#include <direct.h>
#endif

#define S_IFLNK 0xC000
#define S_IRUSR 0x0100
#define S_IWUSR 0x0080
#define S_IXUSR 0x0040
#define S_IRGRP 0x0020
#define S_IWGRP 0x0010
#define S_IXGRP 0x0008
#define S_IROTH 0x0004
#define S_IWOTH 0x0002
#define S_IXOTH 0x0001

int stdmkdir(const char *path, int mode=755);

#else
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>

typedef int HFILE;
typedef unsigned int 	HANDLE;

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef long			LONG;
typedef unsigned int	UINT;
typedef void*			LPVOID;
typedef unsigned long*	LPDWORD;
typedef int				BOOL;
typedef int				INT;
typedef short			SHORT;
typedef WORD			WCHAR;
typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG;
typedef unsigned int	HWND;
typedef const CHAR		*LPCSTR;
typedef float			FLOAT;
typedef double			DOUBLE;

#define FILE_BEGIN		0
#define FILE_CURRENT	1
#define FILE_END		2

#define O_BINARY 0

#define _O_CREAT O_CREAT
#define _O_RDONLY O_RDONLY
#define _O_WRONLY O_WRONLY
#define _O_RDWR O_RDWR
#define _O_EXCL O_EXCL
#define _O_BINARY O_BINARY

#define _S_IREAD S_IREAD //???
#define _S_IWRITE S_IWRITE //???
#define _S_IFDIR S_IFDIR //???

void Sleep(unsigned int i);
DWORD GetCurrentThreadId();
int stdmkdir(const char *path, int mode=755);
#endif

#ifdef WIN32 // Win32 include
#pragma comment(lib,"Ws2_32.lib") 
#include "Winsock2.h"
#include <ws2tcpip.h>
#include "windows.h"
#include <direct.h>

typedef __int64 int64;
typedef unsigned __int64 uint64;

int stdmkdir(const char *path, int mode){ return _mkdir(path); }

#else // Linux include
#include <sys/utsname.h>
#include <sys/stat.h>
#include <utime.h>

#define memcpy memmove
typedef long long int64;
typedef unsigned long long uint64;

int stdmkdir(const char *path, int mode){ return mkdir(path, mode); }

void Sleep(unsigned int i);
#endif

#include <time.h>
#ifndef WINCE
#include <sys/timeb.h>
#endif

// include
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <new>

// Network 
#ifndef WIN32
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

typedef int SOCKET;
#define SOCKADDR sockaddr
#define SOCKADDR_IN sockaddr_in
typedef sockaddr *LPSOCKADDR;

#define closesocket(sock) close(sock)
#endif

// Functions

//#ifdef USEMSV_MEMORYCONTROL_MEMORY
#define _del(v){ if(v){ free(v); } }
//#define _dela(v){ if(v){ delete[] v; msv_memoty_control_destr(1, 0); } }
#define _deln(v){ if(v){ free(v); v=0; } }
#define _newn(sz) (unsigned char*)malloc(sz)

int stoi(char *line, unsigned int size=0, int radix=10);
int64 stoi64(char*line, unsigned int size=0, int radix=10);

// Classes
class VString{
public:
	unsigned char *data;
	unsigned int sz;

	VString(){ data=0; sz=0; }
	~VString(){ }

	VString(const VString &line){ data=line; sz=line.sz;  }
	VString(const char *line){ data=(unsigned char*)line; if(line) sz=strlen((const char*)line); else sz=0; }
	VString(const unsigned char *line){ data=(unsigned char*)line; if(line) sz=strlen((const char*)line); else sz=0; }
	VString(const char *line, const unsigned int s){ data=(unsigned char*)line; if(s<S2G) sz=s; else sz=0; }
	VString(const unsigned char *line, const unsigned int s){ data=(unsigned char *)line; sz=s; }

	void set(VString line){ *this=line; }
	void set(const char* line, const unsigned int size){ sz=size; data=(unsigned char*)line; return ;}
	void setu(const unsigned char* line, const unsigned int size){ sz=size; data=(unsigned char*)line; return ;}
	void delq(){ if(sz>1 && *data=='"' && *(data+sz-1)=='"'){ data++; sz-=2; } }

	bool operator!()const{ if (sz>0) return 0; return 1; }
	operator void*()const { return data; }
	operator char*()const{ return (char*)data; }
	operator unsigned char*()const { return data; }
	operator int()const{ if(data) return sz; return 0; }
	char operator[](int index) const{ if(!sz || (unsigned int)index>=sz) return 0; return *(data+index); }
	char operator[](unsigned int index) const{ if(!sz || index>=sz) return 0; return *(data+index); }

	char * rchar() const{ return (char*)data; }
	unsigned char * uchar() const{ return (unsigned char*)data;}
	char* pos(int p) const{ return (char*)(data+p); }
	unsigned char* upos(int p) const{ return data+p; }
	char * end() const{ return (char*)data+sz; }
	unsigned char * endu() const{ return data+sz; }
	unsigned char endo() const{ if(data && sz) return *(data+sz-1); return 0; }
	unsigned char enduc() const { if(!sz || !data) return 0; return data[sz-1]; }
	char goc(unsigned int index) const{ if(index<0 || index>=sz) return 0; return *(data+index); }

	bool is()const{ return sz>0; }
	bool IsEmpty()const{ return !data || !sz; }
	unsigned int size()const{ return sz; }

	int toi()const { return stoi((char*)data, sz); }

	void Clean(){ sz=0; data=0; return ; }

};

class MString : public VString{

public:

	MString(){ data=0; Init(); }
	~MString(){ _deln(data); }
	void Init(){ _deln(data); sz=0; }

	void MStringNew(const void*line){
		if(!sz){ data=0; return ; }
		data=_newn(sz+1);
		if(!data){ sz=0; return ; }
		if(line) memcpy(data, line, sz);
		data[sz]=0;
		return ;
	}

	MString(const char string){ sz=sizeof(string); MStringNew(&string); return ; }
	MString(const char *line){ if(line){ sz=strlen((const char*)line); MStringNew(line); } else { data=0; sz=0; } }
	MString(const unsigned char *line){ if(line){ sz=strlen((const char*)line); MStringNew(line); } else { data=0; sz=0; } }
	MString(const char *line, const unsigned int s){ sz=s; MStringNew(line); }
	MString(const unsigned char *line, const unsigned int s){ sz=s; MStringNew(line); }

	MString(const short string){ sz=sizeof(string); MStringNew(&string); return ; }
	MString(const int code){ sz=1; MStringNew(&code); return ; }
	MString(const unsigned int code){ sz=1; MStringNew(&code); return ; }
	MString(const unsigned int code, const unsigned int size){ sz=size; MStringNew(0); memset(data, code, sz); return ; }

	MString(const VString &line){ sz=line.sz; MStringNew(line); }
	MString(const MString &line){ sz=line.sz; MStringNew(line); }

	const MString& operator=(const char string){ unsigned char* odata=data; sz=1; MStringNew(&string); _del(odata); return *this; }
	const MString& operator=(const char* line){ unsigned char *odata=data; sz=line ? strlen((const char*)line) : 0; MStringNew(line); _deln(odata); return *this; }
	const MString& operator=(const unsigned char* line){ unsigned char *odata=data; sz=line ? strlen((const char*)line) : 0; MStringNew(line); _deln(odata); return *this; }
	const MString& operator=(const VString& line){ unsigned char *odata=data; sz=line; MStringNew(line); _deln(odata); return *this; }
	const MString& operator=(const MString& line){ unsigned char *odata=data; sz=line; MStringNew(line); _deln(odata); return *this; }

	MString& set(const VString line){ unsigned char *odata=data; sz=line.sz; MStringNew(line); _deln(odata); return *this; }
	MString& set(const char* line, const unsigned int size){ unsigned char *odata=data; sz=size;  MStringNew(line); _deln(odata); return *this; }
	MString& set(const unsigned char* line, const unsigned int size){ unsigned char *odata=data; sz=size;  MStringNew(line); _deln(odata); return *this; }
	MString& setu(const unsigned char* line, const unsigned int size){ unsigned char *odata=data; sz=size; MStringNew(line); _deln(odata); return *this; }

	MString& Add(VString o, VString t, VString th=VString(), VString f=VString(), VString fi=VString()){
		sz=o.size()+t.size()+th.size()+f.size()+fi.size(); if(!sz){ _deln(data); return *this;}
		unsigned char* odata=data;
		MStringNew(0); int p=0;
		memcpy(data, o, o); p+=o.size();
		memcpy(data+p, t, t); p+=t.size();
		memcpy(data+p, th, th); p+=th.size();
		memcpy(data+p, f, f); p+=f.size();
		memcpy(data+p, fi, fi); p+=fi.size();
		data[sz]=0; _del(odata);
		return *this;
	}

	MString& Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7=VString(), VString l8=VString(), VString l9=VString()){
		sz=l1.size()+l2.size()+l3.size()+l4.size()+l5.size()+l6.size()+l7.size()+l8.size()+l9.size();
		if(!sz){ _deln(data); return *this;}
		unsigned char* odata=data, *p;
		MStringNew(0); p=data;
		memcpy(p, l1, l1); p+=l1.sz;
		memcpy(p, l2, l2); p+=l2.sz;
		memcpy(p, l3, l3); p+=l3.sz;
		memcpy(p, l4, l4); p+=l4.sz;
		memcpy(p, l5, l5); p+=l5.sz;
		memcpy(p, l6, l6); p+=l6.sz;
		memcpy(p, l7, l7); p+=l7.sz;
		memcpy(p, l8, l8); p+=l8.sz;
		memcpy(p, l9, l9); p+=l9.sz;
		data[sz]=0; _del(odata);
	return *this;
	}

	MString& Reserv(int size, int p=0){
		if(size==sz) return *this; if(size<=0){ _del(data); data=0; sz=0; return *this; }
		unsigned char*odata=data; int s=sz; sz=size;
		MStringNew(0); int m= s<size ? s : size;
		memcpy(data, odata, m); _del(odata);
		memset(data+m, p, sz-m);
		data[size]=0;
		return *this;
	}

	void Clean(){ Init(); }


};

// functions
int64 stoi64(VString val, int radix){ return stoi64((char*)val.data, val.sz, radix); }
int stoih(const VString &val) {return stoi(val, val, 16);}
int stoi(char *line, unsigned int size, int radix){ return (int)stoi64(line, size, radix); }
unsigned int stoui(char *line, unsigned int size, int radix){ return (unsigned int)stoi64(line, size, radix); }

int64 stoi64(char*line, unsigned int size, int radix){
int64 ret=0; if(!line) return 0; if(!size) size=(unsigned int)strlen(line);
if(!size) return 0;
char *to=line+size; bool min=0; int t;

while(line<to){
if(*line=='-'){min=1; line++; break;}
if(*line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z') break;
line++;
}
while(line<to){
	if(*line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z'){
		t=((*line>='0' && *line<='9') ? *line-48 : (*line>='a' && *line<='z') ? *line-'a'+10 : *line-'A'+10 );
		if(t>=radix) break;
		ret=ret*radix+t;
	}
else break; line++;
}

if(min) ret*=-1;
return ret;
}

// handle
#define IsHandle(h) ((h) && ((int)h)!=-1)
#define ishandle(h) ((h) && ((int)h)!=-1)

#include "crossplatform-rts.cpp"
#include "crossplatform-filefunc.cpp"


#ifdef WIN32
#ifndef WINCE
HANDLE _std_input_handle = GetStdHandle(STD_INPUT_HANDLE);
HANDLE _std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
#endif

// print
inline int print(const VString &line){ if(!line) return 0;

#ifdef USEMSV_TCPPRINT
	if(msvuse_tcpprint && msvuse_tcpprint_sock==0){
		msvuse_tcpprint_sock=ConnectPort(msvuse_tcpprint, 0);
		if(!msvuse_tcpprint_sock) msvuse_tcpprint_sock=-1;
	}
	if(msvuse_tcpprint_sock>0) send(msvuse_tcpprint_sock, line, line, 0);
#endif

#ifdef USEMSV_FILEPRINT
		if(msvuse_fileprint && msvuse_fileprint_sock==0){
		msvuse_fileprint_sock=CreateFile(msvuse_fileprint, O_CREAT|O_RDWR, S_IREAD|S_IWRITE);
		if(ishandle(msvuse_fileprint_sock)) SetEndOfFile(msvuse_fileprint_sock);
		if(!msvuse_fileprint_sock) msvuse_fileprint_sock=-1;
	}
	if(msvuse_fileprint_sock>0) WriteFile(msvuse_fileprint_sock, line, line);
#endif

#ifdef USEMSV_LOGPRINT
	msvuse_logprint.Set(line); if(msvuse_logprint.size()>S1M) msvuse_logprint.Del(msvuse_logprint.size()%S1M);
#endif


#ifdef WIN32
    DWORD wr;
    return WriteFile(_std_output_handle, line, line, &wr, 0); //printf(line);
#else
    return write(1, line, line);
#endif
}

// Functions
// PartLine
VString PartLine(VString line, VString &two, VString el){
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ two.Clean(); return line; }
	two.setu(pos+el.sz, line.endu()-pos-el.sz);
	return VString(line.uchar(), pos-line.uchar());
}

VString PartLineFind(VString line, VString el){
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ return VString(); }
	return VString(pos, el);
}


// Connect functions
void KeepAlive(SOCKET sock){
	int i=1;
	if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&i, 4)) print("SOCKOPT ERROR\r\n");
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i))) print("SOCKOPT ERROR\r\n");
	return ;
}

bool isip(const char*line, unsigned int size){
	if(!line) return 0;
	int sz= size ? size : strlen(line);
	int i=0, s=0; char*l=(char*)line, *t=l+sz;

	for(l; l<t; l++){
		if(*l=='.'){ i++; s=0;}
		else if(*l<'0' || *l>'9') return 0;
		else{ s=s*10+*l-'0'; }
		
		if(i>3 || s>255) return 0;
	}
	if(i!=3 || s>255) return 0;
	return 1;
}

bool isip(const VString line){ return isip((char*)line.data, line.sz); }

unsigned int GetIPh(VString ipa){
	if(!ipa || ipa.sz>=S2K-1) return 0;
	unsigned char safe[S2K]; memcpy(safe, ipa, ipa); safe[ipa.sz]=0;
	char *ip=(char*)&safe;
	//ItosX<S2K> it; it=ipa; char*ip=it.ret;
	if(isip(ip)) return inet_addr(ip);
    
	struct addrinfo *ai=0; unsigned int ipr=0;
	if(getaddrinfo(ip, 0, 0, &ai)) return 0;
	ipr=((struct sockaddr_in*)ai->ai_addr)->sin_addr.s_addr;
	freeaddrinfo(ai);
	return ipr;
}

unsigned int GetIP(VString ipa){ return htonl(GetIPh(ipa)); }

int ConnectPortI(unsigned int lip, int port){ if(!lip) return 0;
	SOCKADDR_IN csocket; int err;
	csocket.sin_addr.s_addr = htonl(lip);
	csocket.sin_family = AF_INET;
	csocket.sin_port = htons(port);

	SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	err=connect(sock, (struct sockaddr*)&csocket, sizeof(csocket));
	if (err<0){ closesocket(sock); return 0; }
	KeepAlive(sock);
	return (int)sock;
}

int ConnectPort(VString lip, int port=0){ if(!lip) return 0;
	VString _p; lip=PartLine(lip, _p, ":"); if(_p) port=_p.toi();
	return ConnectPortI(GetIP(lip), port);
}

int OpenPort(unsigned short port, const char*ip=0, const int maxconn=SOMAXCONN){
#ifdef WIN32
	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED); 
#else
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
#endif

	if(sock==-1) return 0;

SOCKADDR_IN sin;
 	sin.sin_family = AF_INET;
 	sin.sin_port = htons(port);
 	if (ip) sin.sin_addr.s_addr = GetIPh(ip);
	else sin.sin_addr.s_addr = INADDR_ANY;

//int i=1;
//if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&i, 4)) print("SOCKOPT ERROR\r\n");
//if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i))) print("SOCKOPT ERROR\r\n");
	KeepAlive(sock);
	int err = bind(sock, (LPSOCKADDR)&sin, sizeof(sin) ); if(err<0){ closesocket(sock); return 0; }
	err = listen( sock, maxconn); if(err<0) { closesocket(sock); return 0; }
	return (int)sock;
}

// Activate Socket
class _msv_active_socket{
public:
	_msv_active_socket(){
#ifdef WIN32
		WSADATA WsaData; WSAStartup(MAKEWORD(2, 2), &WsaData);
#else
		signal(SIGPIPE, SIG_IGN);
#endif 
	}

}_msv_active_socket;