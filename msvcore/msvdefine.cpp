// Process upped
const unsigned int _proc_upped = (unsigned int)time(0);

VString _vstringnull_;

#ifdef WIN32 // Win32 include
int _TSYS=TSYS_WIN;
#pragma comment(lib,"Ws2_32.lib") 

int stdmkdir(const char *path, int mode){ return _mkdir(path); }
#else

void Sleep(unsigned int i){ usleep((int64)i*1000); return ; }
#ifndef USEMSV_FLASH
DWORD GetCurrentThreadId(){ return pthread_self(); }
#else
DWORD GetCurrentThreadId(){ return 0; }
#endif

int stdmkdir(const char *path, int mode){ return mkdir(path, mode); }

#ifdef USEMSV_ANDROID
int _TSYS=TSYS_ANDROID;
#else
int _TSYS=TSYS_LIN;
#endif

#endif

// WINCE & UNICODE
#ifdef WINCE
#include "crossplatform/wince-twoms.cpp"
#include "crossplatform/wince-two.h" // find it in my Center library
#else
#ifdef UNICODE
#include "crossplatform/wince-twoms.cpp"
#endif
#endif

// Unicode Problem
#ifdef WINCE
#else
#ifdef WIN32
#ifndef UNICODE
#define MODUNICODE(x) (x) //<< normal
//#define MODUNICODEV(x) VString(x)
#define MODUNICODEV(x, s) (LPCSTR)x, s
//(LPCSTR)x
#define MODLPWSTR	LPCSTR
#define MODLPWSTR_R(x) x
#define MODUNICODET(x) x
#else
#define MODLPWSTR_R(x) stoc((unsigned short*)x, wcslen((wchar_t*)x))
#endif
#else
//#define MODUNICODE(x) LPCSTR(x) //<< normal
#define MODUNICODE(x) LPCSTR(x)
//#define MODUNICODEV(x) VString(x)
#define MODUNICODEV(x, s) (LPCSTR)x, s
//(LPCSTR)x
#define MODLPWSTR	LPCSTR
#define MODUNICODET(x) x
#endif
#endif

#ifndef WIN32
void GetThreadTimes(HANDLE threadid, FILETIME *c, FILETIME*e, FILETIME *k, FILETIME *u){
	c->dwLowDateTime=0; c->dwHighDateTime=0;
	e->dwLowDateTime=0; e->dwHighDateTime=0;
	k->dwLowDateTime=0; k->dwHighDateTime=0;
	u->dwLowDateTime=0; u->dwHighDateTime=0;
	return ;
}

HANDLE GetCurrentThread(){
	return 0;
}
#endif


// Includes
class  UMX_data{ public: UMX_data *n; unsigned int sz; char data[0]; };
#include "list/UMatrix.cpp"

#include "MWnd/MRect.cpp"
#include "crossplatform/mte.cpp"

///////// HLString
class HLString: public LString {
public:
	HLString(){ MSVMEMORYCONTROLC; }
	~HLString(){ MSVMEMORYCONTROLD }

//HLString& operator+(HLString& ls, const MString& string){ ls+=string; return ls; }
//HLString& operator+(HLString& ls, const char* lpsz){ls.add(lpsz, strlen(lpsz)); return ls; }
//HLString& operator+(HLString& ls, LString& lst){ls.add(lst.operator MString(), lst.size()); return ls; }
//#ifdef GENERALCPP
	HLString& operator+(const MTEV& val);//{ MString v=val; add(v, v); return *this; }
//#endif
	HLString& operator+(const MString& string);//{ add(string, string); return *this; }
	HLString& operator+(const VString& string);//{ add(string, string); return *this; }
	HLString& operator+(const char* lpsz);//{ add(lpsz, strlen(lpsz)); return *this; }
	HLString& operator+(LString& lst);//{ add(lst.operator MString(), lst.size()); return *this; }
	HLString& operator+(const int &i){
		unsigned char data[S1K];
		int s=prmf_itos(data, S1K, i);
		add((char*)data, s);
		return *this;
	}

	void* addnfrv(int size){
		return &el(addnf(0, size));
	}

	VString addnfr(VString line){
		return VString(&el(addnf(line, line)), line);
	}
};

HLString& HLString::operator+(const MString& string){ add(string, string); return *this; }
HLString& HLString::operator+(const VString& string){ add(string, string); return *this; }
HLString& HLString::operator+(const char* string){ if(string) add(string, strlen(string)); return *this; }
HLString& HLString::operator+(LString& lst){ add(lst.operator MString(), lst.size()); return *this; }


// Includes
#include "crossplatform/filefunc.cpp"
#include "crossplatform/io.cpp"


// Functions //

void VersionsPrint(Versions *ver, int sz){
	print("Versions: \r\n");
	for(int i=0; i<sz; i++){
		//Itos it;
		//it.Format("%d. %s - %s\r\n", i, ver[i].ver, ver[i].ver);
		print(itos(i), ". ", ver[i].ver, " - ", ver[i].date, "\r\n");
	}
	//print("\r\n");
	return ;
}

// print area
#include <locale.h>

int _TSYSC=TSYSC_WIN; MString _TSYSL;

class _MSV_PRINT_OPTIONS{
public:
	_MSV_PRINT_OPTIONS(){
		setlocale(LC_ALL, "");
		VString loc(setlocale(LC_CTYPE, NULL));
		VString lang, cp; lang=PartLine(loc, cp, ".");

#ifdef WIN32
		int i=GetConsoleCP();
		if(i==866) _TSYSC=TSYSC_DOS;
		else // if(i==1251)
			_TSYSC=TSYSC_WIN;
//		if(msvcont_type==MSVCONT_DOS){ msvcont_type=MSVCONT_WIN; SetConsoleCP(1251); SetConsoleOutputCP(1251); }
		if(PartLineO(lang, "_")=="Russian") _TSYSL="ru"; else _TSYSL="en";
#else
		if(lang) _TSYSL=PartLineO(lang, "_"); else _TSYSL="en";
		cp=PartLineO(cp, "_");
		if(cp=="UTF-8" || cp=="utf-8") _TSYSC=TSYSC_UTF;
		else _TSYSC=TSYSC_UTF;
#endif
	}
}_MSV_PRINT_OPTIONS;


// Taphes
MString _procpath, _thispath; int msv_parse_lang=0;

int mainp(int args, char* arg[], ILink &link){
	MString commandline; if(args) commandline=MString(arg[0])+"?";
	for(int i=1; i<args; i++){
		commandline+=MString(arg[i])+(i+1<args ? " " : MString() );
	} //Log(debug_log, commandline);

	link.Link(commandline);
	_thispath.Reserv(S4K); _thispath=getcwd(_thispath, _thispath); cslash(_thispath);
	_procpath=link.GetOnlyPath(); if(_procpath) chdir(_procpath);
	_procpath.Reserv(S4K); _procpath=getcwd(_procpath, _procpath); cslash(_procpath);
	if(_thispath) chdir(_thispath);
	//print("_thispath:", _thispath, "\r\n");
	//print("_procpath:", _procpath, "\r\n");

	if(_TSYS==TSYS_LIN){
		MString tmp;
		sstat64 stt=GetFileInfo(tmp.Add("/usr/share/", PROJECTNAME));
		if((stt.st_mode&S_IFMT)==S_IFDIR){ _thispath=tmp; chdir(_thispath); }		
	}
	
	//if(_TSYS==TSYS_ANDROID){
#ifdef USEMSV_ANDROID
		MString tmp;
		sstat64 stt=GetFileInfo(tmp.Add("/sdcard/Android/data/", PROJECTNAME));
		
		if((stt.st_mode&S_IFMT)==S_IFDIR){ _thispath=tmp; chdir(_thispath); }
		else{
			stt=GetFileInfo(tmp.Add("/data/data/", PROJECTNAME));
			if((stt.st_mode&S_IFMT)==S_IFDIR){ _thispath=tmp; chdir(_thispath); }
		}
		signal(SIGABRT, SIG_IGN);
#endif

#ifndef WIN32
	// Запуск как демон под Линукс
	if(link.Find("--background")) daemon(1,0);
	// запрет сигналов на соккеты
	//signal(SIGPIPE, SIG_IGN);
//#else
	//WSADATA WsaData; WSAStartup (MAKEWORD( 2, 2 ), &WsaData);
#endif


// Sp Options
#ifdef USEMSV_TCPPRINT
	ConfLine core; core.Load("core.conf"); 
	if(!msvuse_tcpprint){
		msvuse_tcpprint=core["core.tcpprint"];
		if(msvuse_tcpprint) print("CORE TCP PRINT activated.\r\n");
	}
#endif

	// analys command line
	VString l="GO", t; int lto=2, ltop=1; 
	int usemsv=0, opt=0;
	// Itos it;

	while(l){
		if(!ltop && !lto) lto=1; ltop=lto;
		if(lto==1){ l=t; t=link.GetArg(); lto=0; }
		else if(lto==2){ l=link.GetArg(); t=link.GetArg(); lto=0; }		
		if(!l) break;

		if(usemsv){
			if(l=="--help"){ lto=1;
				print("Help list:\r\n\
 ## info\r\n\
--help				this help\r\n\
--list protos		list active protocols\r\n\
       pathes		list all pathes\r\n\
## msv options\r\n\
--uselang			change language\r\n\
--usepath			change thispath\r\n\
--useplang			load text from Lang(text) functions\r\n\
\r\n");
			}

			else if(l=="--list"){ lto=2;
				if(t=="protos"){
					//if(msv_sp_protos){ print("List Protos: ", msv_sp_protos(), "\r\n"); }
					//else
					print("List Protos: no active");
				}
				else if(t=="pathes"){
					print("Thispath: ", _thispath, "\r\n");
					print("Procpath: ", _procpath, "\r\n");
				}
				else print("uncnown list: '", t, "'\r\n");
			}
			else{ print("uncnown parameter: ", l, "='", t, "'\r\n"); lto=1; }
		}

		if(l=="--usemsv"){ print("MSV Activate. Programm will be stopped. For help use: --help.\r\n"); usemsv=1; lto=1; }
		
		if(l=="--uselang" && t){ _TSYSL=t; lto=2; }
		if(l=="--usepath" && t){ _thispath=t; chdir(_thispath); lto=2; }
		if(l=="--useplang"){ msv_parse_lang=1; lto=1; }
	}

	if(usemsv){ print(""); exit(0); }
	link.FirstArg();

return 1;
}

int usemsv_iskey(VString line){
	if(line=="--background") return 1;
	else if(line=="--uselang") return 2;
	else if(line=="--usepath") return 2;
	else if(line=="--useplang") return 1;
	return 0;
}

// Lang
VString msv_load_lang; MString msv_use_lang;
#include "crossplatform/lang.cpp"


// Memory Control


/*
// Memory Control
_msv_memory_control* msv_memory_control=0; int _msv_memory_control_sz=0, _msv_memory_control_msz=0;

_msv_memory_control* msv_memoty_control_find(int tid){
	for(int i=0; i<_msv_memory_control_sz; i++){
		if(msv_memory_control[i].id==tid)
			return msv_memory_control+i;

		//if(msv_memory_control[i].id>tid)
		//	return 0;

		//if(msv_memory_control[i].id>tid){
		//	memcpy(msv_memory_control+i, msv_memory_control+i+1, sizeof(_msv_memory_control)*(_msv_memory_control_sz-i));
		//	_msv_memory_control_sz++;
		//	return msv_memory_control+i;
		//}
	}

	return 0;

	_msv_memory_control_sz++;
	return msv_memory_control+_msv_memory_control_sz-1;
}

_msv_memory_control * msv_memoty_control_add(int tid){
	if(_msv_memory_control_sz==_msv_memory_control_msz){
		_msv_memory_control *m=(_msv_memory_control*)malloc(sizeof(_msv_memory_control)*(_msv_memory_control_sz+32)), *d=msv_memory_control;
		msv_memory_control=m; free(d);
		_msv_memory_control_msz+=32;
	}

	for(int i=0; i<_msv_memory_control_sz; i++){
		if(msv_memory_control[i].id==tid)
			return msv_memory_control+i;

		if(msv_memory_control[i].id>tid && 0){
			memcpy(msv_memory_control+i+1, msv_memory_control+i, sizeof(_msv_memory_control)*(_msv_memory_control_sz-i));
			_msv_memory_control_sz++;
			return msv_memory_control+i;
		}
	}

	_msv_memory_control_sz++;
	return msv_memory_control+_msv_memory_control_sz-1;
}

void *msv_memoty_control_malloc(int sz){
	 msv_memoty_control_constr(1, 0, "malloc", 1);
	 return malloc(sz);
}

void msv_memoty_control_constr(int tid, void *t, const char *name, int size){
#ifndef USEMSV_MEMORYCONTROL
	return ;
#endif

	//print("#", itos(tid), name, "\r\n");

	MemConLock();

	_msv_memory_control *m=msv_memoty_control_find(tid);
	if(!m){
		m=msv_memoty_control_add(tid);
		m->id=tid; m->name=name; m->size=size; m->count=0; m->acount=0;
	}

	m->count++; m->acount++;

	MemConUnLock();
	return ;
}

void msv_memoty_control_destr(int tid, void *t){
#ifndef USEMSV_MEMORYCONTROL
	return ;
#endif

	MemConLock();

	_msv_memory_control *m=msv_memoty_control_find(tid);
	//if(m)
		//print("~", m->name, "\r\n");

	if(m){
		if(m->count>0)
			m->count--;
		else
			int ertyrty=546;
		//	globalerror("msv_memoty_control_destr tid.count<=0");
	}else{
		int sdfgsg=34546;
		//globalerror("msv_memoty_control_destr tid not found");
	}

	MemConUnLock();
	return ;
}

int msv_memoty_control_size(){
	return _msv_memory_control_sz;
}

_msv_memory_control msv_memoty_control_pid(int pid){
	MemConLock();
	_msv_memory_control *m, d;

	if(pid>=0 && pid<_msv_memory_control_sz)
		m=msv_memory_control+pid;
	else
		m=0;

	if(m)
		d=*m;
	else
		memset(&d, 0, sizeof(d));
	MemConUnLock();
	return d;
}

_msv_memory_control msv_memoty_control_tid(int tid){
	MemConLock();
	_msv_memory_control *m=msv_memoty_control_find(tid), d;
	if(m)
		d=*m;
	else
		memset(&d, 0, sizeof(d));
	MemConUnLock();
	return d;
}

void MemoryControlPrint(HLString &ls){
	for(int i=0; i<msv_memoty_control_size(); i++){
		_msv_memory_control m=msv_memoty_control_pid(i);
		ls+"#"+itos(m.id)+"\t"+m.name+"("+itos(m.size)+") "+itos(m.count)+", "+itos(m.acount)+"\r\n";
	}
	return ;
}*/

/// BAD BAD CODE
int rtms(unsigned char *line, unsigned int sz, unsigned char *fr, unsigned int frsz, int &pos, bool res){ //read to more sumbol
	if(!line || !fr) return 0;
if (!res){																			//=0
unsigned char *to=line+sz; line+=pos;

for (line; line<to; line++){
	if (*line==*fr) { bool e=1;
		if (frsz>1){// char*nl=line+1, *nf=fr+1, *nt=nf-1+frsz;
			for (unsigned int i=1; i<frsz; i++) if (!(*(line+i)==*(fr+i))){e=0; break;} 
			//for (nf; nf<nt; nf++) if (!(*nl==*nf)){e=0; break;} 
		}
		if (e) {pos=sz-(to-line); return 1;}
}} pos=sz-(to-line); return 0;
}
else{
unsigned char *to=line; line+=pos-frsz;

for (line; line>=to; line--){
	if (*line==*fr) { bool e=1;
		if (frsz>1){// char*nl=line+1, *nf=fr+1, *nt=nf-1+frsz;
			for (unsigned int i=1; i<frsz; i++) if (!(*(line+i)==*(fr+i))){e=0; break;} 
			//for (nf; nf<nt; nf++) if (!(*nl==*nf)){e=0; break;} 
		}
		if (e) {pos=sz-(to-line); return 1;}
	}} pos=sz-(to-line); return 0;
}
return 0;
}

inline int rtms(const MString &line, const MString fr, int &pos, bool res){ //read to more sumbol to
	return rtms(line.uchar(), line, fr.uchar(), fr, pos, res);
}

