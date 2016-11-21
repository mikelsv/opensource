// Size
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

// This System
#define TSYS_WIN	1
#define TSYS_LIN	2
#define TSYS_ANDROID	3

// This system Codepage
#define TSYSC_WIN	1
#define TSYSC_DOS	2
#define TSYSC_UTF	3

extern int _TSYS;
// ~ This System


// Global include
#include <fcntl.h>
#include <math.h>
#include <new>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <wchar.h>

#ifndef WINCE
#include <sys/timeb.h>
#endif
// ~ Global Include

// OS Include
#ifdef WIN32 // Win32 include
	#include <direct.h>
	#include <winsock2.h>
	#include <windows.h>	
	#include <ws2tcpip.h>

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

	typedef __int64 int64;
	typedef unsigned __int64 uint64;

	int stdmkdir(const char *path, int mode=755);

#else // Linux include
	#include <arpa/inet.h>
	#include <errno.h>
	#include <dirent.h>
	#include <netdb.h>
	#include <sys/ioctl.h>
	#include <sys/resource.h>
	#include <sys/socket.h>
	#include <sys/utsname.h>
	#include <sys/wait.h>
	#include <termios.h>
	#include <utime.h>

	#define memcpy memmove
	#define closesocket(sock) close(sock)

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

	typedef int				SOCKET;
	typedef int				HFILE;
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
	typedef long long		int64;
	typedef unsigned long long uint64;
	typedef sockaddr_in		SOCKADDR_IN;
	typedef sockaddr		SOCKADDR;
	typedef sockaddr		*LPSOCKADDR;

	typedef struct _FILETIME {
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	} FILETIME;
	
	void Sleep(unsigned int i);
	HANDLE GetCurrentThread();
	DWORD GetCurrentThreadId();
	void GetThreadTimes(HANDLE threadid, FILETIME *c, FILETIME*e, FILETIME *k, FILETIME *u);
	int stdmkdir(const char *path, int mode = 755);
#endif
// ~ OS Include

#ifdef __GNUC__
	#include <pthread.h>
	#include <unistd.h>
#endif


// Memory Control
#include <typeinfo>

#ifdef USEMSV_GENERALCPP

struct _msvcore_memcon_data{
	unsigned int id;
	const char *name;
	//unsigned int size;
	//unsigned int count, acount;
	int64 mcount, ucount, fcount;

	static int64 amcount, aucount, afcount;

	void init(int id, const char *name){
		if(!name){
			name = "<Unknown!!!>";
			printf("\r\nGLOBALERROR: Memory control type name is unknown!");
		}

		this->id = id;
		this->name = name;
		mcount = 0;
		ucount = 0;
		fcount = 0;
	}

	void m(){
		mcount ++;
		ucount ++;
		amcount ++;
		aucount ++;
	}

	void f(){
		fcount ++;
		ucount --;
		afcount ++;
		aucount --;
	}

};

int64 _msvcore_memcon_data::amcount = 0;
int64 _msvcore_memcon_data::aucount = 0;
int64 _msvcore_memcon_data::afcount = 0;

int msvcore_memcon_malloc_count = 0, msvcore_memcon_free_count = 0;
_msvcore_memcon_data *msvcore_memcon_data = 0;
int msvcore_memcon_data_sz = 0;
void **msvcore_memcon_data_void = 0;

bool MemConLock();
bool MemConUnLock();

void msvcore_memcon_print();
int msvcore_memcon_print_ltime = 0;

#if defined(WIN32) && defined(USEMSV_INTERCEPT_MALLOC)
#include "DbgHelp.h"
#include <WinBase.h>
#pragma comment(lib, "Dbghelp.lib")
#endif


// Stack
#if USEMSV_MEMORYCONTROL_GLOBAL && WIN32
#include "DbgHelp.h"
#include <WinBase.h>
#pragma comment(lib, "Dbghelp.lib")

void msvcore_memcon_stack_print(){
	HANDLE process;
	void *stack[100];
	unsigned short frames;
	unsigned char buf[sizeof(SYMBOL_INFO) + 256 * sizeof(char)];
	SYMBOL_INFO *symbol = (SYMBOL_INFO*)buf;

	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = CaptureStackBackTrace(0, 100, stack, NULL);
//	symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	for(int i = 0; i < frames; i++){
		SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
		printf("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
	}

	//free(symbol);
	return ;
}

unsigned int msvcore_memcon_stack_crc(){
	HANDLE process;
	void *stack[100];
	unsigned short frames;
	unsigned int ret = 0;

	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = CaptureStackBackTrace(0, 100, stack, NULL);

	for(int i = 0; i < frames; i++)
		ret ^= (unsigned int)stack[i];

	return ret;
}
#else
void msvcore_memcon_stack_print(){ return ; } 
unsigned int msvcore_memcon_stack_crc(){ return 0; }
#endif

// Find Leak
struct msvcore_memon_findleak{
	msvcore_memon_findleak *_p, *_n;
	unsigned int crc;
};

msvcore_memon_findleak *msvcore_memon_findleak_a = 0;
unsigned int msvcore_memon_findleak_lprint = 0;
// End of Find Leak


class msvcore_memcon_class{
public:

	msvcore_memcon_class(){

	}

	~msvcore_memcon_class(){
		free(msvcore_memcon_data);
		free(msvcore_memcon_data_void);
	}

} msvcore_memcon_class;

#ifdef USEMSV_MEMORYCONTROL_FINDLEAK
#ifndef USEMSV_MEMORYCONTROL_FINDLEAK_PRINT_BUFSZ
#define USEMSV_MEMORYCONTROL_FINDLEAK_PRINT_BUFSZ S1K
#endif

void msvcore_memcon_findleak_print(){
	unsigned int id[USEMSV_MEMORYCONTROL_FINDLEAK_PRINT_BUFSZ], count[USEMSV_MEMORYCONTROL_FINDLEAK_PRINT_BUFSZ], size = 0;
	memset(&id, 0, sizeof(id));
	memset(&count, 0, sizeof(count));

	MemConLock();

	msvcore_memon_findleak *p = msvcore_memon_findleak_a;
	while(p){
		int ok = 0;

		for(int i = 0; i < size; i++){
			if(p->crc == id[i]){
				count[i] ++;
				ok = 1;
				break;
			}
		}

		if(!ok){
			if(size < USEMSV_MEMORYCONTROL_FINDLEAK_PRINT_BUFSZ){
				id[size] = p->crc;
				count[size] = 1;
				size ++;
				ok = 1;
			}
		}

		if(!ok){
			printf("FAIL! Need more USEMSV_MEMORYCONTROL_FINDLEAK_PRINT_BUFSZ!\r\n");
		}

		p = p->_n;
	}

	printf("Findleak Results: \r\n");
	for(int i = 0; i < size; i++){
#ifdef USEMSV_MEMORYCONTROL_FINDLEAK_PRINTMIN
		if(count[i] >= USEMSV_MEMORYCONTROL_FINDLEAK_PRINTMIN)
#endif
		printf("%d: %d\r\n", id[i], count[i]);
	}

	MemConUnLock();

	return ;
}
#endif


void * msvcore_memcon_malloc(int sz, _msvcore_memcon_data *item){
	int prn = 0;

#ifdef USEMSV_MEMORYCONTROL_PRINT
	int tm = time(0);
	if(msvcore_memcon_print_ltime + USEMSV_MEMORYCONTROL_PRINT < tm){
		msvcore_memcon_print_ltime = tm;
		prn = 1;
	}
#endif

#ifdef USEMSV_MEMORYCONTROL_FINDLEAK
	int findleak = 0;
	if(item->id == USEMSV_MEMORYCONTROL_FINDLEAK){
		findleak = 1;
		sz += sizeof(msvcore_memon_findleak);
	}
#endif

	if(prn)
		msvcore_memcon_print();

	unsigned char *ret = (unsigned char*)malloc(sz);

#ifdef USEMSV_MEMORYCONTROL_FINDLEAK
	if(findleak){
		msvcore_memon_findleak *p = (msvcore_memon_findleak*)ret;

		MemConLock();

		if(!msvcore_memon_findleak_a){
			p->_n = 0;
		} else{
			p->_n = msvcore_memon_findleak_a;
			msvcore_memon_findleak_a->_p = p;
		}

		msvcore_memon_findleak_a = p;
		p->_p = 0;

		p->crc = msvcore_memcon_stack_crc();

#if USEMSV_MEMORYCONTROL_FINDLEAK_PRINT
		if(msvcore_memon_findleak_lprint + USEMSV_MEMORYCONTROL_FINDLEAK_PRINT < time(0)){
			msvcore_memon_findleak_lprint = time(0);
			msvcore_memcon_findleak_print();
		}
#endif

		MemConUnLock();

		ret += sizeof(msvcore_memon_findleak);
	}
#endif

	return ret;

}

void msvcore_memcon_free(void *v, _msvcore_memcon_data *item){
#ifdef USEMSV_MEMORYCONTROL_FINDLEAK
	if(item->id == USEMSV_MEMORYCONTROL_FINDLEAK){
		msvcore_memon_findleak *p = (msvcore_memon_findleak*)((unsigned char*)v - sizeof(msvcore_memon_findleak));
		
		MemConLock();
		if(p == msvcore_memon_findleak_a){
			msvcore_memon_findleak_a = p->_n;
			msvcore_memon_findleak_a->_p = 0;
		} else{
			p->_p->_n = p->_n;
			p->_n->_p = p->_p;
		}

		MemConUnLock();


		v = p;
	}
#endif

	return free(v);
}

_msvcore_memcon_data* msvcore_memcon_find(int id, const char *name){
	for(int i = 0; i < msvcore_memcon_data_sz; i++){
		if(msvcore_memcon_data[i].id == id)
			return msvcore_memcon_data + i;
	}

	if(msvcore_memcon_data_sz % 128 == 0){
		_msvcore_memcon_data *d = (_msvcore_memcon_data*)malloc(sizeof(_msvcore_memcon_data) * (msvcore_memcon_data_sz + 128));
		if(!d)
			return 0;

		memcpy(d, msvcore_memcon_data, sizeof(_msvcore_memcon_data) * msvcore_memcon_data_sz);
		free(msvcore_memcon_data);
		msvcore_memcon_data = d;
	}

	msvcore_memcon_data[msvcore_memcon_data_sz].init(id, name);
	return &msvcore_memcon_data[msvcore_memcon_data_sz++];
}


_msvcore_memcon_data* msvcore_memcon_malloc_c(int tid, const char *name){
	MemConLock();
	_msvcore_memcon_data *item = msvcore_memcon_find(tid, name);
	item->m();

//#if USEMSV_MEMORYCONTROL_GLOBAL && USEMSV_MEMORYCONTROL_TEST1
//	if(!msvcore_memcon_data_void){
//		msvcore_memcon_data_void = (void**)malloc(S16K * sizeof(void*));
//		memset(msvcore_memcon_data_void, 0, S16K * sizeof(void*));
//	}
//
//	for(int i = 0; i < S16K; i++)
//		if(!msvcore_memcon_data_void[i]){
//			msvcore_memcon_data_void[i] = mem;
//			break;
//		}	
//#endif

	msvcore_memcon_malloc_count ++;

	MemConUnLock();
	return item;
}

_msvcore_memcon_data* msvcore_memcon_free_c(int tid, void *mem){
	MemConLock();
	_msvcore_memcon_data *item = msvcore_memcon_find(tid, 0);
	item->f();

#if USEMSV_MEMORYCONTROL_GLOBAL && USEMSV_MEMORYCONTROL_TEST1
	int f = 0;

	for(int i = 0; i < S16K; i++)
		if(msvcore_memcon_data_void[i] == mem){
			msvcore_memcon_data_void[i] = 0;
			f = 1;
			break;
		}

	if(!f || !mem)
		int ff = 1; // print("Fail free().\r\n");

#endif

	msvcore_memcon_free_count ++;

	MemConUnLock();
	return item;
}	


void* msvcore_memcon_malloc2(int tid, void *t, const char *name, int size, int mallocsize){
	_msvcore_memcon_data *item = msvcore_memcon_malloc_c(tid, name);
	void *ret = msvcore_memcon_malloc(mallocsize, item);
	return ret;
}

void msvcore_memcon_free2(int tid, void *t, const char *name, int size, void *freev){
	if(!freev)
		return ;

	_msvcore_memcon_data *item = msvcore_memcon_free_c(tid, freev);
	return msvcore_memcon_free(freev, item);
}		

#ifdef USEMSV_MEMORYCONTROL_GLOBAL
	void * operator new(size_t sz) throw(){
		return msvcore_memcon_malloc2(1, 0, "class Malloc", 0, sz);
		//return msvcore_memcon_malloc(sz);
	}
	void operator delete(void * p) throw(){
		return msvcore_memcon_free2(1, 0, "class Malloc", 0, p);
		//return msvcore_memcon_free(p);
	}
#endif

#else
	struct _msvcore_memcon_data;

	void* msvcore_memcon_malloc(int sz);
	void msvcore_memcon_free(void *v);

	void* msvcore_memcon_malloc2(int tid, void *t, const char *name, int size, int mallocsize);
	void msvcore_memcon_free2(int tid, void *t, const char *name, int size, void *freev);

	_msvcore_memcon_data* msvcore_memcon_malloc_c(int tid, const char *name);
	_msvcore_memcon_data* msvcore_memcon_free_c(int tid, void *mem);

	#ifdef USEMSV_MEMORYCONTROL_GLOBAL
		void* operator new(size_t sz);
		void operator delete(void * p);
	#endif
#endif

#ifdef USEMSV_MEMORYCONTROL_GLOBAL
#define malloc(v) msvcore_memcon_malloc2(typeid(*this).hash_code(), this, typeid(*this).name(), sizeof(*this), v)
//#define malloc(v) msvcore_memorycontrol_malloc(v)
#define free(v) msvcore_memcon_free2(typeid(*this).hash_code(), this, typeid(*this).name(), sizeof(*this), v)
//#define free(v) msvcore_memorycontrol_free(v)
#else
#ifdef USEMSV_MEMORYCONTROL
#error "Please set to global flags: /D "USEMSV_MEMORYCONTROL_GLOBAL" "
#endif
#endif
// Memory Control End

	
#define _del(v){ if(v){ free(v); } } //msv_memoty_control_destr(1, 0); } }
//#define _dela(v){ if(v){ delete[] v; msv_memoty_control_destr(1, 0); } }
#define _deln(v){ if(v){ free(v); v=0; } } //msv_memoty_control_destr(1, 0); } }
#define _newn(sz) (unsigned char*) malloc(sz) //msv_memoty_control_malloc(sz)

#define msvnew(a, b) a = (b*) malloc(sizeof(b)); new(a)b;
#define msvdelete(a, b) a->~b(); free(a);


// Types
typedef unsigned char  BYTE;
typedef unsigned long  DWORD;
// u
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#define VOIDSIZE (sizeof(void*))
#define UMSZ (sizeof(char*)+sizeof(int))
#define UMCSZ sizeof(char*)

#ifndef NULL
#define NULL	0
#endif
					
#ifndef TRUE
#define	TRUE true
#endif

#ifndef FALSE
#define	FALSE false
#endif

//#ifdef __GNUC__
//#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
//#endif
//#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
//#endif
//#endif

#ifdef USEMSV_ANDROID
	#define __S_ISUID       04000   /* Set user ID on execution.  */
	#define __S_ISGID       02000   /* Set group ID on execution.  */
	#define __S_ISVTX       01000   /* Save swapped text after use (sticky).  */
	#define __S_IREAD       0400    /* Read by owner.  */
	#define __S_IWRITE      0200    /* Write by owner.  */
	#define __S_IEXEC       0100    /* Execute by owner.  */

	#define S_IREAD		__S_IREAD
	#define S_IWRITE	__S_IWRITE
#endif

// Network
typedef unsigned char MACADDR[6];
typedef unsigned char IPV6[16];
struct MACADDR_S{ MACADDR mac; };
typedef struct {
  DWORD adr[4];
  operator bool(){ return adr[0] || adr[1] || adr[2] || adr[3]; }
} IP6_ADDRESS;

// Struct
class DUINTS{ public: unsigned int pos; unsigned int size;
	DUINTS(){}
	DUINTS(unsigned int p, unsigned int s){pos=p; size=s;}
};

class DUVINTS{ public: char *pos; unsigned int size;
	DUVINTS(){}
	DUVINTS(char *p, unsigned int s){pos=p; size=s;}
};


// Data //
static const char cb16[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Functions //

// Num & char
#define isnumd(val) ((unsigned char)val<48 || (unsigned char)val>57 ? 0 : 1)
int isnum(unsigned char *line, unsigned int sz);
inline unsigned char lowd(const unsigned char ch){ return ((ch>=65 && ch<=90 || ch>=192 && ch<=223) ? ch+32 : ch); }
inline unsigned char upd(const unsigned char ch){ return ((ch>=97 && ch<=122 ||  ch>=224) ? ch-32 : ch); }
int up(unsigned char *line, unsigned int sz);
int low(unsigned char *line, unsigned int sz);
inline unsigned char stoic(const unsigned char ch){ return '1'+ch%10; }
inline unsigned char stoihc(const unsigned char ch){ return (ch%16<10 ? '1' : 'a')+ch%16; }
//#define itosd(val) return ('0'+val)


// String to int
//int stoi(char*val, unsigned int size);
int stoi(char*line, unsigned int size=0, int radix=10);
unsigned int stoui(char*line, unsigned int size=0, int radix=10);
int64 stoi64(char*val, unsigned int size=0, int radix=10);
double stod(char*line, unsigned int size=0, int radix=10);

// bits operation
#define divur(a, b) (a/b+(a%b)>0)
#define reval(a, b) {a^=b; b^=a; a^=b;}
#define minel(a, b) ((a<=b) ? a : b)
#define maxel(a, b) ((a>=b) ? a : b)

// handle
#define IsHandle(h) ((h) && ((int)h)!=-1)
#define ishandle(h) ((h) && ((int)h)!=-1)

// Test time: tbtime(); tctime(); tetime(); int sec=sec, millim=micro sec.
#define tbtime timeb ft, fte; int tbtime_sec, tbtime_millim, tbtime_result; ftime(&ft); // print("Time: ", itos(sec*1000+millitm), "ms.\r\n");
#define tctime ftime(&ft); // continue
#define tetime ftime(&fte); tbtime_sec=(int)(fte.time-ft.time); tbtime_millim=fte.millitm-ft.millitm; if(fte.millitm<ft.millitm){ tbtime_millim+=1000; tbtime_sec--; } tbtime_result = tbtime_sec * 1000 + tbtime_millim;
// print()
//#define tcetime ftime(&fte); sec=fte.time-ft.time; millim=fte.millitm-ft.millitm; if(fte.millitm<ft.millitm){ millim+=1000; sec--; }


// Templates
template <class OMatrix>
class OMatrixT{ public:
OMatrix *_a, *_e; // +data

OMatrixT(){ _a=0; _e=0; }

	// IF _p as _a && _n as _e;
	bool OMAdd(OMatrix*el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1;}
		//_e->_n=el; el->_p=_e; el->_n=0; _e=el; return 1;
		el->_n=0; el->_p=_e; _e->_n=el; _e=el; return 1;
	}

	static bool OMAddP(OMatrix *&_a, OMatrix *&_e, OMatrix*el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1;}
		//_e->_n=el; el->_p=_e; el->_n=0; _e=el; return 1;
		el->_n=0; el->_p=_e; _e->_n=el; _e=el; return 1;
	}

	bool OMDel(OMatrix*el){
		if(el->_n) el->_n->_p=el->_p; else if(el==_e) _e=el->_p;
		if(el->_p) el->_p->_n=el->_n; else if(el==_a) _a=el->_n;
		return 1;
	}

	static bool OMDelP(OMatrix *&_a, OMatrix *&_e, OMatrix *el){
		if(el->_n) el->_n->_p=el->_p; else if(el==_e) _e=el->_p;
		if(el->_p) el->_p->_n=el->_n; else if(el==_a) _a=el->_n;
		return 1;
	}

	bool OMAddEx(OMatrix*p, OMatrix*el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1;}

		if(!p){
			el->_p=0; el->_n=_a; _a->_p=el; _a=el;
			//if(_a==_e) _e=el; its fail
			return 1;
		}

		el->_p=p; el->_n=p->_n;
		p->_n=el; if(el->_n) el->_n->_p=el; else _e=el;
		return 1;
	}

	static bool OMAddExP(OMatrix *&_a, OMatrix *&_e, OMatrix*p, OMatrix*el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1; }

		if(!p){
			el->_p=0; el->_n=_a; _a->_p=el; _a=el;
			//if(_a==_e) _e=el; its fail
			return 1;
		}

		el->_p=p; el->_n=p->_n;
		p->_n=el; if(el->_n) el->_n->_p=el; else _e=el;
		return 1;
	}

	OMatrix *OMNew(bool r=1){
		OMatrix *p=new OMatrix;
		if(r && p) OMAdd(p);
		return p;
	}

	void OMRealDel(OMatrix *d){
		OMatrix *p=_a;
		while(p){
			if(p==d){ OMDel(p); delete p; return ; }
			p=p->_n;
		}
		return ;
	}

	void OMClear(bool r=1){
		OMatrix *p=_a, *d=p;
		while(p){
			d=p; p=p->_n; OMDel(d); if(r) delete d;
		}
		 _a=0; _e=0;
	}

	void OMClearS(){ _a=0; _e=0; }
};


#define OMatrixTemplateAdd(_a, _e, el) \
	if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; } \
	else {	el->_n=0; el->_p=_e; _e->_n=el; _e=el; }

#define OMatrixTemplateDel(_a, _e, el) \
	if(el->_n) el->_n->_p=el->_p; else if(el==_e) _e=el->_p; \
	if(el->_p) el->_p->_n=el->_n; else if(el==_a) _a=el->_n;

	

struct OmatrixBlock{
	OmatrixBlock *_p, *_n;
	unsigned int u, a, sz; // use, all, size
	unsigned char data[];

	int ismy(void*d){ if(d>=&data[0] && d<&data[sz]) return 1; return 0; }
	int ismye(void*d){ if(d>=&data[0] && d<=&data[sz]) return 1; return 0; }
	int get(unsigned int num){ return num<a && data[sz+num/8]&(1 << (num%8)); }
	unsigned int getn(void *d){ return (unsigned int)(((unsigned char*)d-&data[0])/(sz/a)); }
	int set(unsigned int num, int val){
		if(num>=a) return 0;
		if(val) data[sz+num/8]|=1 << (num%8);
		else data[sz+num/8]&=~(1 << (num%8));
		return 1;
	}
	int getfree(){
		for(unsigned int i=0; i<a; i+=8){
			if(data[sz+i/8]!=255){
				unsigned char c=data[sz+i/8];
				if(!(c&1)) return i+0;
				if(!(c&2)) return i+1;
				if(!(c&4)) return i+2;
				if(!(c&8)) return i+3;
				if(!(c&16)) return i+4;
				if(!(c&32)) return i+5;
				if(!(c&64)) return i+6;
				if(!(c&128)) return i+7;
				return data[sz+i/8]&(1 << (i%8));
			}
		}
		return -1;
	}
};

template<class OList, class Item>
class OListCursor{
	OList *olist;

public:
	Item *el;

public:
	OListCursor(OList &l){
		olist=&l; el=0;
	}

	bool Next(){
		el=olist->Next(el);
		return el!=0;
	}
};

// Global Error
int globalerror();
int globalerror(const char*line);


#define MSVMEMORYCONTROLCD(tid, name, size) //\
//  msv_memoty_control_constr(tid, name, size);

#if defined(WIN32) && defined(USEMSV_MEMORYCONTROL)
#define MSVMEMORYCONTROLC msvcore_memcon_malloc_c(typeid(*this).hash_code(), typeid(*this).name());
//	msv_memoty_control_constr(typeid(*this).hash_code(), this, typeid(*this).name(), sizeof(*this));

#define MSVMEMORYCONTROLD msvcore_memcon_free_c(typeid(*this).hash_code(), this);
//	msv_memoty_control_destr(typeid(*this).hash_code(), this);

#else

#define MSVMEMORYCONTROLC
//    msv_memoty_control_constr(0, this, typeid(*this).name(), sizeof(*this));

#define MSVMEMORYCONTROLD
//    msv_memoty_control_destr(0, this);

#endif