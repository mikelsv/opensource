#ifdef WIN32
#include <io.h>
#define lseek64 _lseeki64
#define tell64 _telli64
#define lstat64	_stati64
#define _rename rename

#ifdef __GNUC__
#define chsize64 ftruncate
#else
#define chsize64 _chsize_s
#endif

typedef struct _stati64 sstat64;
#else
#define _open(...)	open(__VA_ARGS__)
#define _read(...) read(__VA_ARGS__)
#define _write(...) write(__VA_ARGS__)
#define _close(...) close(__VA_ARGS__)
#define _rename rename

#define _fstati64 fstat64
#define _stati64 stat64
//#define sstat64 struct stat64
typedef struct stat64 sstat64;

#endif

#define FSCFSZ	(1024+512) // FileSystem path max size

int normalpath(char*file, int asz, char *in, int ins, bool r=0, int opt=0);

//#ifdef USEMSV_ITOS
//#define LFSNORMALPATH(it, file) ItosX<FSCFSZ> it; it.Reserv(FSCFSZ-1); \
//	{ int rsz=normalpath(it.ret, it.ret, file, file, 0, 1); file.set(it.ret, rsz); } //if(file.endu()!=0){ it=file; file=it; }
//#else
#define LFSNORMALPATH(it, file) unsigned char it[FSCFSZ]; \
	{ int rsz=normalpath((char*)it, FSCFSZ, file, file, 0, 1); file.set((char*)it, rsz); } //if(file.endu()!=0){ it=file; file=it; }
//#endif


#include <sys/stat.h>
//#define _stati64 stat64
//#define sstat64 struct stat64
//#define sstat64 struct _stati64


#ifdef WIN32
inline int fileeof(HFILE hfile){ return chsize64(hfile, tell64(hfile)); }
#else
inline int fileeof(HFILE hfile){ return ftruncate(hfile, lseek64(hfile, 0, SEEK_CUR)); }
#endif

HFILE CreateFile(VString file, int op, int pm){ if(!file) return -1; return _open(TString(file), op|O_BINARY, pm); }
int ReadFile(HFILE fl, void* buf, unsigned int sz){ return _read(fl, buf, sz); }
int WriteFile(HFILE fl, const void* buf, unsigned int sz){ return _write(fl, buf, sz); }
int64 GetFilePointer(HFILE fl){ return lseek64(fl, 0, FILE_CURRENT); }
int64 SetFilePointer(HFILE fl, int64 pos, int origin=FILE_BEGIN){ return lseek64(fl, pos, origin); }
int SetEndOfFile(HFILE hfile){ return fileeof(hfile); }

sstat64 GetFileInfo(VString file){
	sstat64 stt;// lsstat64 st;
	TString fl=file;
	if(_stati64(fl, &stt)) memset(&stt, 0, sizeof(stt));
	//else{ stat64tomstat(stt, st); }
    return stt;
}

sstat64 GetFileInfo(HFILE hfile){
	sstat64 stt;// lsstat64 st;
	if(_fstati64(hfile, &stt)) memset(&stt, 0, sizeof(stt));
//	else{ stat64tomstat(stt, st); }
	return stt;
}

int CloseHandle(HFILE fl){
	if(!ishandle(fl))
		return 0;
	return _close(fl);
}

//
int64 GetFileSize(HFILE hfile){
	int64 pos=GetFilePointer(hfile);
	int64 sz=SetFilePointer(hfile, 0, FILE_END);
	SetFilePointer(hfile, pos, FILE_BEGIN);
	return sz;
}


bool IsDir(VString path){ sstat64 stt=GetFileInfo(TString(path)); return (stt.st_mode&S_IFDIR)!=0; }
int MkDir(VString file, unsigned int mode=0){ return stdmkdir(TString(file), mode); }


int DeleteFile(VString file){ LFSNORMALPATH(it, file); return unlink(file); }

int MoveFile(VString from, VString to){
	LFSNORMALPATH(it, from);
	LFSNORMALPATH(ut, to);

	return !_rename(from, to);
}



//// File System (Virtual) File Functions /////
int normalpath(char*file, int asz, char *in, int ins, bool r, int opt){ // opt =1 then save "/../"
	//print(itos(asz), " ", itos(ins), " '", VString(in, ins), "'", "\r\n");
	if(!asz) return 0;

	char*ln=file, *to=in+ins, *tof=file+asz; char o='\\', t='/'; if(r){ o='/'; t='\\'; }
	for(in; in<to; in++){// print("'", itos(*in), "'");
		if(*in==0){
			//print("normalpath!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			continue;
		}
		if(*in==o){
			*ln=t; ln++;
		}
		else if(*in=='.' && ln-1>=file && *(ln-1)=='.' && ((!opt) && ln-1==file || ln-2>=file && *(ln-2)==t)
			&& (!opt || !(ln-4>=file && *(ln-3)=='.' && *(ln-4)=='.' && (ln-4==file || *(ln-5)==t)))){ // << ../../
			//char *lln=ln;
			ln-=2; while(ln>file){ if(*ln!='/' && *ln!='\\'){ break; } ln--; }
			if(ln>file) {ln--; while(ln>file){ if(*ln==t) break; ln--; }}
			
			//if(opt&1 && lln){ ln==lln; if(ln>=file) *ln=*in; ln++; }
		}
		else {
			if(ln>=file) *ln=*in; ln++;
		} // if(ln==file || *(ln-1)!='/' )

		if(*(ln-1)==t && ln-1>file && *(ln-2)==t){ ln--; }
		if(ln>=tof){ *file=0; return 0; }
	}
	if(ln<tof) *ln=0;
	return (int)(ln-file);
}

/*
void stat64tostat(struct stat &stt, sstat64 &s){
	stt.st_dev=s.st_dev;
	stt.st_ino=s.st_ino;
	stt.st_mode=s.st_mode;
	stt.st_nlink=s.st_nlink;
	stt.st_uid=s.st_uid;
	stt.st_gid=s.st_gid;
	stt.st_rdev=s.st_rdev;
	stt.st_size=s.st_size;
#ifndef WIN32
	stt.st_blksize=s.st_blksize;
	stt.st_blocks=s.st_blocks;
#endif
	stt.st_atime=s.st_atime;
	stt.st_mtime=s.st_mtime;
	stt.st_ctime=s.st_ctime;
	return ;
}

void stat64toxft(xftstat &stt, sstat64 &s){
	stt.st_mode=s.st_mode;
	stt.st_nlink=s.st_nlink;
	stt.st_size=s.st_size;
	stt.st_atm=s.st_atime;
	stt.st_mtm=s.st_mtime;
	stt.st_ctm=s.st_ctime;
	return ;
}

void stat64toxft_r(sstat64 &stt, xftstat &s){
	memset(&stt, 0, sizeof(stt));
	stt.st_mode=s.st_mode;
	stt.st_nlink=s.st_nlink;
	stt.st_size=s.st_size;
	stt.st_atime=s.st_atm;
	stt.st_mtime=s.st_mtm;
	stt.st_ctime=s.st_ctm;
	return ;
}*/