#ifdef WIN32
#ifndef WINCE
HANDLE _std_input_handle = GetStdHandle(STD_INPUT_HANDLE);
HANDLE _std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

void MsvAllocConsole(){
	AllocConsole();
	_std_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	_std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////// Print
#ifdef USEMSV_TCPPRINT
	int ConnectPort(VString lip, int port);
	MString msvuse_tcpprint; SOCKET msvuse_tcpprint_sock=0;
#endif

#ifdef USEMSV_FILEPRINT
	MString msvuse_fileprint="msvuse_fileprint.log"; HFILE msvuse_fileprint_sock=0;
#endif

#ifdef USEMSV_LOGPRINT
	SendDataL msvuse_logprint;
	int msvuse_logprint_getsz(){ return msvuse_logprint.size(); }
	int msvuse_logprint_get(unsigned char * data, unsigned int sz){ int rd=msvuse_logprint.Get(data, sz); msvuse_logprint.Del(rd); return rd; }
#endif

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

#ifdef USEMSV_ANDROID_LOGPRINT
	__android_log_print(ANDROID_LOG_INFO, USEMSV_ANDROID_LOGPRINT, MString(line));
#endif


#ifdef WIN32
    DWORD wr;
    return WriteFile(_std_output_handle, line, line, &wr, 0); //printf(line);
#else
    return write(1, line, line);
#endif
}

// multiprint
inline int print(const VString &l1, const VString &l2){
    int r=print(l1); r=print(l2) && r; return r;
}
inline int print(const VString &l1, const VString &l2, const VString &l3){
    int r=print(l1); r=print(l2) && r; r=print(l3) && r; return r;
}
inline int print(const VString &l1, const VString &l2, const VString &l3, const VString &l4){
	int r=print(l1); r=print(l2) && r; r=print(l3) && r; r=print(l4) && r; return r;
}
inline int print(const VString &l1, const VString &l2, const VString &l3, const VString &l4, const VString &l5
, const VString &l6=VString(), const VString &l7=VString(), const VString &l8=VString(), const VString &l9=VString(), const VString &l10=VString()){
	int r=print(l1); r=print(l2) && r; r=print(l3) && r; r=print(l4) && r; r=print(l5) && r;
	r=print(l6) && r; r=print(l7) && r; r=print(l8) && r; r=print(l9) && r; r=print(l10) && r; return r;
}

// Global Error
int globalerror(){
	int error=0;
	print("\r\nGLOBALERROR\r\n");
	return 0;
}

int globalerror(const char*line){
	int error=0;
	print("\r\nGLOBALERROR: "); print(line); print("\r\n");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////// Logs
// define TIME is 32768 = (S32K)
#define LOGS_TIME_DEF	S32K

// Usage: Open(file); SetOutput(YOUDEF_1|YOUDEF_2|YOUDEF_3|LOGS_TIME_DEF, YOUDEF_3); LogTime(TYPE_ID) or LogMod(TYPE_ID); Log(TEXT);
// if(SetOutput() types == LogMod(TYPE_ID)) Log(TEXT) -> WriteToFile();

class Logs{
protected:
// data
	HFILE fl;	// open file
// options
	int outf, outp; // output: file, print()
	MString tmf, tmp; // time: file, print()
// realtime
	int logtype; bool logtime;

public:
	Logs(){ fl=0; outf=0; outp=0; logtype=0; logtime=0; }
	~Logs(){ Init(); }

	void Init(){
		if(ishandle(fl)) CloseHandle(fl); fl=0;
		outf=0; outp=0; logtype=0; logtime=0;
		tmf="d.m.Y H:i:s";
	}

	int Open(VString file){
		if(ishandle(fl)) CloseHandle(fl);
		fl=CreateFile(file, O_CREAT|O_RDWR|O_APPEND, S_IREAD|S_IWRITE);
		if(!ishandle(fl)){ fl=0; return 0; }
		return 1;	
	}

	int Close(){ if(ishandle(fl)) CloseHandle(fl); fl=0; return 1; }
	void SetOutput(int f, int p=0){ outf=f; outp=p; }
	void SetTime(VString f, VString p=VString()){ tmf=f; tmp=p; } // NOT ACTIVE

	void LogType(int type){ logtype=type; }
	void LogTime(int type=0){

	//void LogMod(int type=0){ CONFIG_LOG_T=type; }
//void LogTime(int type=0){
		//time_t rawtime; time(&rawtime);
		//VString t=ctime(&rawtime);
		MTime mt;
		MString t=mt.date(tmf);

		if(type) logtype=type;
		
		logtime=1;
		Log(t, ": ");
		logtime=0;
		return ;
	}

	int Log(VString line){
		if(ishandle(fl)){
			if((outf&logtype) && logtime <= (outf&LOGS_TIME_DEF)!=0) WriteFile(fl, line, line);
		}

		if(outp&logtype && logtime <= (outp&LOGS_TIME_DEF)!=0) print(line);
	
		return 1;
	}

	int Log(const VString &l1, const VString &l2, const VString &l3=VString(), const VString &l4=VString(), const VString &l5=VString()
, const VString &l6=VString(), const VString &l7=VString(), const VString &l8=VString(), const VString &l9=VString(), const VString &l10=VString()){
		int r=Log(l1); r=Log(l2) && r; r=Log(l3) && r; r=Log(l4) && r; r=Log(l5) && r;
		r=Log(l6) && r; r=Log(l7) && r; r=Log(l8) && r; r=Log(l9) && r; r=Log(l10) && r; return r;
	}
};



///////////////////////////////////////////////////////////////////////////////////// File IO
TString LoadFile(VString file){ if(!file) return TString();
	TString ret; // *file.end()==0 ? file : MString(file)
	HFILE fl; fl=CreateFile(file, O_RDONLY, S_IREAD| S_IWRITE);
	if(fl==-1) return MString();
	ret.Reserv(GetFileSize(fl));
	unsigned int rd=ReadFile(fl, ret, ret);
	CloseHandle(fl);
	if(rd!=ret.size()) return ret.str(0, rd);
	else return ret;
}

bool IsFile(VString file){
	HFILE fl; fl=CreateFile(file, O_RDONLY, S_IREAD| S_IWRITE);
	if(!ishandle(fl)) return 0;
	CloseHandle(fl); return 1;
}

unsigned int SaveFile(VString file, VString data){
	if(!file)
		return 0;

	HFILE fl; fl=CreateFile(file, O_RDWR|O_CREAT, S_IREAD| S_IWRITE); //  | S_IRGRP | S_IROTH
	if(fl==-1) return 0;
	unsigned int wr=WriteFile(fl, data, data); SetEndOfFile(fl);
	CloseHandle(fl);
	return wr;
}

unsigned int SaveFileAppend(VString file, VString data){
	HFILE fl; fl=CreateFile(file, O_RDWR|O_CREAT, S_IREAD| S_IWRITE); //  | S_IRGRP | S_IROTH
	if(fl==-1) return 0;
	SetFilePointer(fl, 0, FILE_END);
	unsigned int wr=WriteFile(fl, data, data);// SetEndOfFile(fl);
	CloseHandle(fl);
	return wr;
}

class AHFILE{
	HFILE hfile;

public:

	AHFILE(HFILE h){
		hfile=h;
	}

	operator HFILE(){
		return hfile;
	}

	~AHFILE(){
		if(ishandle(hfile))
			CloseHandle(hfile);
	}

};

bool MakeDir(VString path, VString fold){
#ifdef  __GNUC__
#define MAKEDIRSLASH    "/"
#else
#define MAKEDIRSLASH    "\\"
#endif
	MString rfold=Replace(fold, "\\", "/"); fold=rfold;
	VString o, t=fold, lo; MString tmp;
	while(t){
		o=PartLine(t, t, "/");
		o.sz=o.endu()-fold.data; o.data=fold;// if(lo==o) break;
		tmp.Add(path, path ? "/" : "",  o);
		MkDir(tmp);
		lo=o;
	}
	return 1;

	/*ExplodeLine el; explodem(el, fold, "/");
	for(unsigned int i=0; i<el.size(); i++){
		path+=MString(el.el(i))+MAKEDIRSLASH;
		MkDir(path);
	}*/
	return 1;
}


class VSi{ public:
VString key;
sstat64 stt;
bool isdir(){ return (stt.st_mode&S_IFMT)==S_IFDIR; }
bool islink(){ return (stt.st_mode&S_IFMT)==S_IFLNK; }
int64 size(){ return stt.st_size; }
};

#include "../list/IMatrix.cpp"
#include "sort.cpp"
#include "ef.cpp"

#define READDIRSZ	64

class Readdir{
HLString ls;
VSiLine line;
public:

int Clear(){ ls.Clear(); return 1;}
int Add(VString file, sstat64 &stt){
if(line.size==line.asize()) line.Add(READDIRSZ);
//VString fn=file;
line.n().key.set(&ls.el(ls.addnf(file, file)), file); line.n().stt=stt; line.size++;
return line.size;
}

int ReadDir(MString dir){
	ls.Clear(); line.Clear(); sstat64 stt; /*lsstat64 st;*/ VString path, fn; int fs=0; unsigned int pos;
char bf[S2K]; if(dir.size()>S2K-3) return 0; //memcpy(bf, dir, dir);
MSVEF ef; ef.fcmp=1; // ILink il; il.Ilink(dir); if(!il.file){ dir+"*"; il.file="*";}
//if(_TSYS==TSYS_WIN){ wcslash(dir); dir+="*.*"; }
// normal path for find
path.set(bf, normalpath(bf, S2K, dir, dir.size()));
while(path.sz>0){ if(*(path.end()-1)=='/' || *(path.end()-1)=='\\') break; path.sz--; fs++; }
ef.Read(VString(path.pos(path.sz), fs));
if(_TSYS==TSYS_WIN){ path.data[path.sz++]='*'; path.data[path.sz++]='.'; path.data[path.sz++]='*'; } path.data[path.sz]=0;

#ifdef WIN32  //int ReadDir(MString dir, ISLine &list){ 
	HANDLE find_handle; WIN32_FIND_DATA ff; char* d=(char*)&bf;
	find_handle=FindFirstFile(MODUNICODE(path),&ff);
	if(find_handle==INVALID_HANDLE_VALUE) return 0;
#else
	DIR *d=opendir(path ? MString(path) : "./"); if(!d) return 0;
#endif
	while(path.sz>0){ if(*(path.end()-1)=='/' || *(path.end()-1)=='\\') break; path.sz--; }

	while(d){
#ifdef WIN32
	if(fn.data) if(!FindNextFile(find_handle, &ff)) break;
#ifdef UNICODE // #unicode
	MString fnuni=MODLPWSTR_R(ff.cFileName); fn=fnuni;
#else
	fn=MODLPWSTR(ff.cFileName); 
#endif // #e unicode

#else
		struct dirent *dr=readdir(d); if(!dr) break; fn=dr->d_name;
#endif

if(fn!="."){
	if(path.sz+fn.sz+1>=S1K) break;
	if(fs){ pos=0; if(!ef.rts(fn, pos)) continue; }
	memcpy(path.end(), fn, fn); *(path.end()+fn.sz)=0;
	if(lstat64(path, &stt)) break;
	//else{ stat64tomstat(stt, st); }

	if(line.size==line.asize()) line.Add(READDIRSZ);
		
	line.n().key.set(&ls.el(ls.addnf(fn, fn)), fn); line.n().stt=stt;
		//line.n().is=(ff.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)>0;
		//line.n().size=ff.nFileSizeLow;
	line.size++;
}	//list.insert(ff.cFileName, ();
	}
#ifdef WIN32
	FindClose(find_handle);
#else
	closedir(d);
#endif
	return 1;}
/*
#else
//int ReadDir(MString dir, ISLine &list){
	DIR *d=opendir(dir);
	while(1){
	struct dirent *dr=readdir(d); 
	if (stat(dr->d_name, &stt)) continue;
		if(line.size==line.asize()) line.Add(READDIRSZ);
		VString fn=dr->d_name;
		line.n().key.set(&ls.el(ls.addnf(fn, fn), fn); line.n().stt=stt;
		//line.n().is=stt.st_mode&S_IFDIR;
		line.size++;
//	int sz=strlen(&dr->d_name);
//	if(sz>=0 && sz<=255) list.insert(&dr->d_name, stt.st_mode&S_IFDIR);	
	}
	closedir(d);
#endif*/

#define RD_NOP		S1M
#define RDS_NAME	1
#define RRDS_UNAME	2

int Resort(int type=RD_NOP|RDS_NAME){
	//HLString tls; VSiLine tline; tline.Add(line.size); memcpy(tline.data, line.data, line.size*sizeof(VSi));
	if(type&RD_NOP){
		for(unsigned int i=0; i<line.size; i++){
			if(line.data[i].key=="."){ memcpy(&line.data[i], &line.data[i+1], (line.size-i)*sizeof(VSi)); line.size--;  }		}
	}

	myfastsort_VSiLine(line);
	for(unsigned int i=0; i<line.size; i++){
			if(line.data[i].key==".."){
				VSi vs=line.data[i];
				memcpy(&line.data[1], &line.data[0], (i)*sizeof(VSi)); // line.size--;
				line.data[0]=vs;
			}
	}
return 1;
}


void operator=(Readdir &rd){
	for(unsigned int i=0; i<rd.size(); i++){
		Add(rd[i].key, rd[i].stt);
	}
	return ;
}


VSi& operator[](int p){ return line[p]; }
operator unsigned int(){ return line.size; }
unsigned int size(){ return line.size; }

};


// ReadStorage
int ReadStorage(ISLine& isli, MString path, MString as, int type=0){
	ISLine isl;
	if(as) wcslash(as); wcslash(path);// MSCR cach;
	//ReadDir(path, "*.*", isl); DISLine*dsl=isl._a, *dl;
	Readdir rd; rd.ReadDir(path+"/*.*");

//	if(dsl) dsl=dsl->_n; if(!as) dsl=isl._a;
    //while(dsl){
	for(int i=0; i<rd.size(); i++){
		//if(dsl->s=='.' || dsl->s==".." ) {dl=dsl; dsl=dsl->_n; isl.del(dl); continue; }
		if(rd[i].key=='.' || rd[i].key==".." ){ /*dl=dsl; dsl=dsl->_n; isl.del(dl);*/ continue; }

		if(rd[i].isdir()){
			if(type&1) isli.insert(as+rd[i].key, 1);
			ReadStorage(isli, path+rd[i].key, as+rd[i].key, type); /*dl=dsl; dsl=dsl->_n; isl.del(dl);*/ continue;
		}

	isli.insert(as+rd[i].key, 0);
   // dsl=dsl->_n;
	}
   return 1;
}



// Renew File
class RenewFile{
	unsigned int ltime;
	unsigned int sz, tm;
	VString file;
public:
	unsigned int reload;

	RenewFile(){ sz=0; tm=0; ltime=0; reload=30; }
	RenewFile(VString fl){ sz=0; tm=0; ltime=0; reload=30; file=fl;}

	void SetFile(VString f){ file=f; }

	bool IsNew(){ return IsNewTime() && IsNewFile(); }

	bool IsNewTime(){
		if(ltime+reload>=time()) return 0;
		ltime=time(); return 1;
	}

	bool IsNewFile(const VString&file){
		sstat64 stt=GetFileInfo(file);
		if(!stt.st_size || stt.st_size==sz && stt.st_mtime==tm) return 0;
		sz=stt.st_size; tm=stt.st_mtime; return 1;
	}

	bool IsNewFile(){
		sstat64 stt=GetFileInfo(file);
		if(!stt.st_size || stt.st_size==sz && stt.st_mtime==tm) return 0;
		sz=stt.st_size; tm=stt.st_mtime; return 1;
	}

};

// GetWindowsDirectory
MString GetWindowsDirectory(){
MString ret;
#ifdef WIN32
#ifdef UNICODE
	ret.Reserv(S2K);
	int s=::GetWindowsDirectory((LPWSTR)ret.rchar(), ret.size()/2);
	return stoc((unsigned short*)ret.rchar(), s); 
#else
	ret.Reserv(1024); ret=ret.str(0, GetWindowsDirectory(ret, ret.size()));
#endif
#endif
return ret;
}

MString windows_directory=GetWindowsDirectory();

// MTGet
TULine mimetypes;
#define MIMETYPE_HTML	"text/html"
#define MIMETYPE_CTHTML	"Content-Type: text/html"

VString defmimeypes="application/mac-binhex40	hqx\r\n\
application/mac-compactpro	cpt\r\n\
application/msword	doc\r\n\
application/octet-stream	bin dms lha lzh exe class so dll\r\n\
application/oda	oda\r\n\
application/pdf	pdf\r\n\
application/postscript	ai eps ps\r\n\
application/smil	smi smil\r\n\
application/vnd.mif	mif\r\n\
application/vnd.ms-excel	xls\r\n\
application/vnd.ms-powerpoint	ppt\r\n\
application/vnd.wap.wbxml	wbxml\r\n\
application/vnd.wap.wmlc	wmlc\r\n\
application/vnd.wap.wmlscriptc	wmlsc\r\n\
application/x-bcpio	bcpio\r\n\
application/x-cdlink	vcd\r\n\
application/x-cpio	cpio\r\n\
application/x-csh	csh\r\n\
application/x-director	dcr dir dxr\r\n\
application/x-dvi		dvi\r\n\
application/x-futuresplash	spl\r\n\
application/x-gtar	gtar\r\n\
application/x-hdf	hdf\r\n\
application/x-javascript	js\r\n\
application/x-koan	skp skd skt skm\r\n\
application/x-latex	latex\r\n\
application/x-netcdf	nc cdf\r\n\
application/x-sh	sh\r\n\
application/x-shar	shar\r\n\
application/x-shockwave-flash	swf\r\n\
application/x-stuffit	sit\r\n\
application/x-tar	tar\r\n\
application/x-tcl	tcl\r\n\
application/x-tex	tex\r\n\
application/x-texinfo	texinfo texi\r\n\
application/x-ustar	ustar\r\n\
application/x-wais-source	src\r\n\
application/xhtml+xml	xhtml xht\r\n\
application/zip	zip\r\n\
audio/basic	au snd\r\n\
audio/midi	mid midi kar\r\n\
audio/mpeg	mpga mp2 mp3\r\n\
audio/x-aiff	aif aiff aifc\r\n\
audio/x-mpegurl	m3u\r\n\
audio/x-pn-realaudio	ram rm\r\n\
audio/x-pn-realaudio-plugin	rpm\r\n\
audio/x-realaudio	ra\r\n\
audio/x-wav	wav\r\n\
chemical/x-pdb	pdb\r\n\
chemical/x-xyz	xyz\r\n\
image/bmp	bmp\r\n\
image/gif	gif\r\n\
image/ief	ief\r\n\
image/jpeg	jpeg jpg jpe\r\n\
image/png	png\r\n\
image/tiff	tiff tif\r\n\
image/vnd.djvu	djvu djv\r\n\
image/vnd.wap.wbmp	wbmp\r\n\
image/x-cmu-raster	ras\r\n\
image/x-rgb	rgb\r\n\
image/x-xbitmap	xbm\r\n\
image/x-xpixmap	xpm\r\n\
image/x-xwindowdump	xwd\r\n\
model/iges	igs iges\r\n\
model/mesh	msh mesh silo\r\n\
model/vrml	wrl vrml\r\n\
text/css	css\r\n\
text/html	html htm\r\n\
text/plain	asc txt\r\n\
text/richtext	rtx\r\n\
text/rtf	rtf\r\n\
text/sgml	sgml sgm\r\n\
text/vnd.wap.wml	wml\r\n\
text/vnd.wap.wmlscript	wmls\r\n\
text/x-setext	etx\r\n\
text/xml	xml xsl\r\n\
video/mpeg	mpeg mpg mpe\r\n\
video/quicktime	qt mov\r\n\
video/vnd.mpegurl	mxu\r\n\
video/x-msvideo	avi\r\n\
video/x-sgi-movie	movie\r\n\
";

struct TVS{
	VString k, v;
};

TVS defmimeypes_data[S1K]; int defmimeypes_sz=0;

unsigned int MTLoad(const MString& file){
	 
	VString line=defmimeypes, o, t, t2;
	int cnt=0;

	// do 1
	while(line){
		o=PartLine(line, line, "\r\n");
		o=PartLine(t, t, "\t");

		while(t){ t=PartLine(t, t, "\t"); cnt++; }
	}

	if(sizeof(defmimeypes_data)/sizeof(TVS)<cnt){ globalerror("msvio.h: MTLoad() count is > buffer"); return 0; }

	// do 2
	line=defmimeypes;

	while(line){
		o=PartLine(line, line, "\r\n");
		o=PartLine(t, t, "\t");

		while(t){ t2=PartLine(t, t, "\t"); defmimeypes_data[cnt].k=o; defmimeypes_data[cnt].v=t2; cnt++; }
	}

	defmimeypes_sz=cnt;

	return cnt;
}

VString MTGet(const VString &line){
	for(int i=0; i<defmimeypes_sz; i++){
		if(line==defmimeypes_data[i].v) return defmimeypes_data[i].k;

	}
	return "text/html";
}

//MString MTGet(const MString& s){ MString ret=mimetypes.Find(s); if(ret) return ret; return "text/html"; }


// NormalPath
VString NormalPath(VString path){
unsigned char*line=(unsigned char*)path.uchar(), *ln=line, *to=line+path.size(), *ret=ln; unsigned int r;
int c=0;//, m=0;
while(line<to){
	switch(*line){
		case '/': case '\\': // 1
			if(line>ln){ memcpy(ret, ln, line-ln); ret+=line-ln; ln=line; }
			if(ret>path.uchar() && ( *(ret-1)=='\\' || *(ret-1)=='/')){ ln=line+1; }
			else{ *ret++=*line; ln=line+1; c|=1; }

			//if(c&1) {ln=line; m=1;}
			//else {
			//if(line+1<to && line>ln && *line=='/' && *(line+1)=='/' && *(line-1)==':')  {line++;}
			//else {
			//if(m && line+1>ln) memcpy(ret, ln, line-ln); ret+=line-ln; ln=line; c|=1;}
			//}
		break;
		case '.': // 2
			if(ret==path.uchar() || ( *(ret-1)=='\\' || *(ret-1)=='/')){ r=0; c=0;
				if(line>ln){ memcpy(ret, ln, line-ln); ret+=line-ln; ln=line; }
				while(line < to && *line=='.'){ *line++; r++; }
					if(*line=='\\' || *line=='/' || line>=to){
						if(r==1) ln=line+1;
						else{
							if(ret-1>path.uchar()) ret--; //while(ret>path.uchar() && (*(ret-1)!='\\' || *ret!='/')) ret--; ret--;
							while(ret>path.uchar() && *(ret-1)!='\\' && *(ret-1)!='/') ret--;
							ln=line+1;
						}
				}
			}
			//if(c>=3) {ln=line; m=1;}
			//else if(c)
			//else if(c==2) {c=3;}
			 //{if(m && line>ln) memcpy(ret, ln, line-ln); ret+=line-ln; ln=line; c|=2;}
		break;
		case '?': if(line>ln) memcpy(ret, ln, line-ln); ret+=line-ln; ln=line;
			while(ret && *(ret-1)==32 && ret>path.uchar()) ret--;
			if(line>ln+1) {memcpy(ret, ln, line-ln); ret+=line-ln;} line=to-1;
		break;
		case '%':
			 if(line>ln) memcpy(ret, ln, line-ln); ret+=line-ln; 
			line++; if(*line>='0' && *line<='9') *ret=(*line-'0')*16; else if(*line>='A' && *line<='F') *ret=(*line-'A'+10)*16; else *ln=0;
			line++; if(*line>='0' && *line<='9') *ret+=(*line-'0'); else if(*line>='A' && *line<='F') *ret+=(*line-'A'+10);
		if(*ret=='.') { if(c>=3) ret--; c|=2;} else if(*ret=='\\' || *ret=='/') { if(c&1) ret--; c|=1;} else c=0;
	// is utf 
		if(*ret>127 && ret>path.uchar()) if(((*(ret)&192)==128) && (*(ret-1)&224)==192) 
		{r=((*(ret-1)&31)<<6)+(*(ret)&63); *(ret-1)=stoc(r); ret--;}
		if(*ret<32) *ret=' '; ln=line+1; ret++;// m=1;
		break;
		default : if(*line<32) *line=' '; c=0; break;
} line++; }
if(line>ln) memcpy(ret, ln, line-ln); ret+=line-ln;
while(ret && *(ret-1)==32 && ret>path.uchar()) ret--;
if(path.size()==ret-path.uchar()) return path; //if(!m) return path;
return path.str(0, ret-path.uchar());
}