
#define FILE_READ_DATA            ( 0x0001 )    // file & pipe
#define FILE_SHARE_READ                 0x00000001
#define FILE_SHARE_WRITE                0x00000002

#define _O_CREAT 0x8000000
#define _O_EXCL 0x4000000

#undef _O_RDONLY
#define _O_RDONLY GENERIC_READ
#undef _O_WRONLY
#define _O_WRONLY GENERIC_WRITE
#undef _O_RDWR
#define _O_RDWR (GENERIC_READ|GENERIC_WRITE)

#undef _S_IREAD
#define _S_IREAD 0x00000001 
#undef _S_IWRITE
#define _S_IWRITE 0x00000002

#define O_CREAT _O_CREAT
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_RDWR _O_RDWR
#define O_EXCL _O_EXCL
#define O_BINARY _O_BINARY

#define S_IREAD _S_IREAD
#define S_IWRITE _S_IWRITE
#define O_BINARY _O_BINARY


#define FILE_BEGIN 0
#define FILE_CUR 1
#define FILE_END 2

int GetWindowsDirectory(char*data, int sz){
if(sz>=strlen("\\Windows\\")) memcpy(data, "\\Windows\\", strlen("\\Windows\\")); return strlen("\\Windows\\"); return 0; }
int GetCurrentDirectory(int sz, char*data){ return 0; }
int mkdir(const char*path){ return CreateDirectory(MODUNICODE(TString(path)), 0); }
int rmdir(const char*path){ return RemoveDirectory(MODUNICODE(TString(path))); }




HFILE open(const char * file, int rd, int pm){
	int cr=OPEN_EXISTING; if(rd&_O_CREAT) {cr=OPEN_ALWAYS; if(rd&_O_EXCL) cr=CREATE_NEW;};
return (HFILE)CreateFile(MODUNICODE(file), (unsigned int)(rd&(GENERIC_READ|GENERIC_WRITE)), pm, 0, cr, 0, 0);
}

long lseek(HFILE fl, long pos, int t){ return ::SetFilePointer((HANDLE)fl, pos, 0, t); }
long _lseek(HFILE fl, long pos, int t){ return lseek(fl, pos, t); }
long _lseeki64(HFILE fl, int64 pos, int t){ return ::SetFilePointer((HANDLE)fl, (long)(pos&0xffffffff), (long*)&pos+1, t); }

long tell(HFILE fl){ return ::SetFilePointer((HANDLE)fl, 0, 0, FILE_CUR); }
unsigned int chsize(HFILE fl){ return ::SetEndOfFile((HANDLE)fl); }
unsigned int chsize(HFILE fl, int pos){ lseek(fl, pos, FILE_BEGIN); return ::SetEndOfFile((HANDLE)fl); }

int read(HFILE fl, void * buffer, unsigned int size){ DWORD dw; ::ReadFile((HANDLE)fl, buffer, size, &dw, 0); return dw; }
int write(HFILE fl, const void * buffer, unsigned int size){ DWORD dw; ::WriteFile((HANDLE)fl, buffer, size, &dw, 0);  return dw;}
long _write(HFILE fl, const void * buffer, unsigned int size){ return write(fl, buffer, size); }

int unlink(const char *file){ return DeleteFile(MODUNICODE(file)); }

int close(HFILE fl){ return ::CloseHandle((HANDLE)fl); }

