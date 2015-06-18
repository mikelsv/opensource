GLock consolelock;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// char, keys, locate, mouse, screen, focus
// [0==0] - char
// [0==1]
// [1,2==0] unicode;
// [1==0,2==1] mouse -- NO. END.
// [0] - data code: char, unicode, locate 
#define TRMP_NULL		0	// null
/// --- XXX --- NEW
// [0]=1 - char
// [0]=0, type: 1-128
// keys & char|unicode & mouse - 1-3 [
// [1.. ]- !keys & [ 1 - char, 2 - unciode, 3 - mouse], keys [4,5,6]
// 7 - location(x,y);
//  ]
// terminal
#define TRMPEX			128		
#define TRMP_CHAR		1	// char || unicode if [8==1]. || location if[8==1, 9==1], [128*128] s:2-3
#define TRMP_UCHAR		2
#define TRMP_MOUSE		3	// mouse: state(move, keydown, keyup, scroll) 
#define TRMP_KEYS		4	// keys
#define TRMP_UKEYS		5
#define TRMP_MKEYS		6
#define TRMP_COLOR		7
#define TRMP_BGCOLOR	8
#define TRMP_LOCATE		9
#define TRMP_CURSOR		10
#define TRMP_SCREEN		11	// byte==1 - focus, ==2 resize.
#define TRMP_MOVE		12
// 13,14
// ...15 16...
#define TRMP_GL			16	// graphics language + graphics object + graphics window;
#define TRMP_TST		17	// test operations: ping, myip, time
// ...31 32...
#define TRMP_RPC		32	// remote procedure
#define TRMP_STREAM		33	// stream data line
#define TRMP_CNTX		34 // centerix proto
// close FULLNULL;

// ... 127

#ifndef WIN32
#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.

#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.


#define FROM_LEFT_1ST_BUTTON_PRESSED    0x0001
#define RIGHTMOST_BUTTON_PRESSED        0x0002
#define FROM_LEFT_2ND_BUTTON_PRESSED    0x0004
#define FROM_LEFT_3RD_BUTTON_PRESSED    0x0008
#define FROM_LEFT_4TH_BUTTON_PRESSED    0x0010

typedef struct _CHAR_INFO {
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } Char;
    WORD Attributes;
} CHAR_INFO, *PCHAR_INFO;

typedef struct _SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
} SMALL_RECT, *PSMALL_RECT;

#endif


//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Console class //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/*#define WK_LEFT		37
#define WK_UP		38
#define WK_RIGHT	39
#define WK_DOWN		40
#define WK_DEL		46*/


#define CN_RALT		0x0001 // the right alt key is pressed. ANALOG RIGHT_ALT_PRESSED
#define CN_LALT		0x0002 // the left alt key is pressed.
#define CN_RCTRL	0x0004 // the right ctrl key is pressed.
#define CN_LCTRL	0x0008 // the left ctrl key is pressed.
#define CN_SHIFT	0x0010 // the shift key is pressed.
#define CN_NUM		0x0020 // the numlock light is on.
#define CN_SCROLL	0x0040 // the scrolllock light is on.
#define CN_CAPS		0x0080 // the capslock light is on.
#define CN_KEY		0x0100 // the key is enhanced.
#define CN_DOWN		0x0200 // the key is down (else up)

#define CN_FOCUS	0x0200 // the focus

#define CNMMOV		0x0200 // MOUSE_MOVED 0x0001
#define CNMDBL		0x0400 // DOUBLE_CLICK 0x0002
#define CNMWHE		0x0800 // MOUSE_WHEELED 0x0004

#define CNTKEY		(1*S8K)
#define CNTMOUSE	(2*S8K)
#define CNTWND		(3*S8K)
#define CNTMENU		(4*S8K)
#define CNTFOCUS	(5*S8K)
#define CNTTYPE		((1+2+4)*S8K)

#define CNML		S4K
#define CNMR		S16K
#define CNM2		S32K
#define CNM3		S16K
#define CNM4		S32K

#define CN_MINS		1
#define CN_MSEL		2

#define CN_ASC		0x0200
#define CN_UNI		0x0400


/////////////////////////////////////////////////////////////////////////////////////
class contype{
unsigned short type;
unsigned short code;
unsigned short mouse;
public:

	contype(){type=0;}

unsigned short gett() { return type&CNTTYPE; }
// ...
// ...
unsigned int getsz(){
unsigned short t=gett();
switch(t){
case CNTKEY: return 4;
case CNTMOUSE: return 6;
case CNTWND: return 6;
//case CNTMENU:
case CNTFOCUS: return 2;
}
return 4;
}

unsigned short getx(){ return code&(S16M-1); }
unsigned short gety(){ return mouse&(S16M-1); }
COORD getxy(){ COORD cr={code&(S16M-1), mouse&(S16M-1)}; return cr; }
bool isfocus(){ return type&CN_FOCUS; }

BYTE getmouse(){
BYTE m=0;
if(type&CNML) m+=FROM_LEFT_1ST_BUTTON_PRESSED;
if(type&CNMR) m+=RIGHTMOST_BUTTON_PRESSED;
if(type&CNM2) m+=FROM_LEFT_2ND_BUTTON_PRESSED;
if(type&CNM3) m+=FROM_LEFT_3RD_BUTTON_PRESSED;
if(type&CNM4) m+=FROM_LEFT_4TH_BUTTON_PRESSED;
return m;
}

BYTE getmousea(){ return (type&(CNMWHE+CNMDBL+CNMMOV))>>9; }

// get code
unsigned short getcode() { return code; }
// get ext(enhanced) code.
unsigned int getxcode() { if(getkeys()&CN_KEY) return code+S64K; return code; }
// get keys
unsigned short getkeys() { return type&(CN_DOWN*2-1); }


void SetKey(bool down, unsigned short keys, unsigned short key){ // sz4
	type=keys&(CN_DOWN-1)|CNTKEY|(down*CN_DOWN); code=key; return ;
}

void SetMouse(unsigned short keys, unsigned short clk, unsigned short bn, COORD coord){ // sz6
	type=keys&(CN_DOWN*2-1)|CNTMOUSE|((clk&7)<<9)|CNML*(FROM_LEFT_1ST_BUTTON_PRESSED&bn);
	code=coord.X|CNMR*(RIGHTMOST_BUTTON_PRESSED&bn)|CNM2*(FROM_LEFT_2ND_BUTTON_PRESSED&bn);
	mouse=coord.Y|CNM3*(FROM_LEFT_3RD_BUTTON_PRESSED&bn)|CNM4*(FROM_LEFT_4TH_BUTTON_PRESSED&bn); return ;
}

void SetWindow(COORD coord){ // sz6
	type=CNTWND; code=coord.X; mouse=coord.Y; return ;
}
void SetWindow(unsigned short x, unsigned short y){ // sz6
	type=CNTWND; code=x; mouse=y; return ;
}

void SetFocus(bool is){// sz2
type=CNTFOCUS|CN_FOCUS; return ;
}


int GetTermo(char d[8]){
	int s=0;
	switch(gett()){
		case CNTKEY:
			unsigned short k=getkeys(); if(!(k&CN_DOWN)) return 0;
			unsigned short c=getcode();
			if(!k){
				if(c<128) {d[0]=c; return 1;}
				d[0]=TRMPEX+TRMP_CHAR; *(unsigned short*)&d[1]=c; return 3;
			} else {
				*(unsigned short*)&d[1]=k; 
				if(c<128){ d[0]=TRMPEX+TRMP_KEYS; d[3]=c; return 4;}
				d[0]=TRMPEX+TRMP_UKEYS; *(unsigned short*)&d[3]=c; return 5;
			}
		break;
	}
return s;
}

#ifdef WIN32
void GetWinf(_INPUT_RECORD &ir){
	switch(gett()){
		case CNTKEY:
			ir.EventType=KEY_EVENT;
			ir.Event.KeyEvent.uChar.UnicodeChar=getcode();
			ir.Event.KeyEvent.bKeyDown=getkeys()&CN_DOWN;
			ir.Event.KeyEvent.dwControlKeyState=getkeys()&(CN_DOWN-1);
			ir.Event.KeyEvent.wRepeatCount=1;
			ir.Event.KeyEvent.wVirtualKeyCode=ir.Event.KeyEvent.uChar.UnicodeChar;
			return ;
		case CNTMOUSE:
			ir.EventType=MOUSE_EVENT;
			ir.Event.MouseEvent.dwControlKeyState=getkeys()&(CN_DOWN-1);
			ir.Event.MouseEvent.dwEventFlags=getmousea();
			ir.Event.MouseEvent.dwButtonState=(type&CNML)*FROM_LEFT_1ST_BUTTON_PRESSED|
				(type&CNMR)*RIGHTMOST_BUTTON_PRESSED|(type&CNM2)*FROM_LEFT_2ND_BUTTON_PRESSED|
				(type&CNM3)*FROM_LEFT_3RD_BUTTON_PRESSED|(type&CNM4)*FROM_LEFT_4TH_BUTTON_PRESSED;
			ir.Event.MouseEvent.dwMousePosition=getxy();
			return ;
		case CNTWND:
			ir.EventType=WINDOW_BUFFER_SIZE_EVENT;
			ir.Event.WindowBufferSizeEvent.dwSize=getxy();
			break;
		case CNTMENU:
			ir.EventType=MENU_EVENT;
			break;
		case CNTFOCUS:
			ir.EventType=FOCUS_EVENT; ir.Event.FocusEvent.bSetFocus=isfocus();
			break;
	}
return ;
}
#endif

};
/////////////////////////////////////////////////////////////////////////////////////


class Console{
public:

int cur, sz, asz, curln;//  cursor, size,  def 4kbuf, 4k col, 4kbg
MString _buf; short *buf; unsigned short *col, *bg; unsigned int *ln;
// default 1k lines
unsigned short defc, defbg, curc, curt;
int sx, sy; int fx, fy; int cx, cy; // size & font & cursor
// struct: 0,0 - sx,sy. -1
int useconsole, inconsole; BYTE cursz; BYTE mode;
#define MWNL	1 // new line

#ifdef WIN32
#define CNIRSZ	4
_INPUT_RECORD ir[CNIRSZ];
#endif

char readbuf[128]; int readbufs;


// Command line save
// save command in conffile
// line pos

Console(){ cur=0; sz=0; curln=0; sx=80; sy=24; fx=8; fy=10; curt=0; cursz=25;
defc=MRGB("000000"); defbg=MRGB("ffffff"); curc=MRGB("ffffff");
//mstyle.bgcolor=; mstyle.fontcolor=;
reline(); useconsole=1; inconsole=0;
}

int console(int val){ return useconsole=val;} int console(){return useconsole;}
int consolein(int val){return inconsole=val;} int consolein(){return inconsole;}

/////////////////////////////////////////////////////////////////////////////////////

int reline(int s=S4K){
_buf.Reserv(s*5); asz=s;
buf=(short*)_buf.pos(0); col=(unsigned short*)_buf.pos(s); bg=(unsigned short*)_buf.pos(s*2); ln=(unsigned int*)_buf.pos(s*3);
//ReDraw();
return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
bool OnSize(UINT nType, unsigned short cx, unsigned short cy){
int rtert=4;


return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
int locate(int x, int y){


	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
//// print section /// send to terminal /////////////////////////////////////////////
/*
int print(short val){
if(sz>=asz) return 0;
//buf[sz]=val; col[sz]=defc; bg[sz]=defbg; sz++; 

if(useconsole) {
	if(val==MK_BACKSPASE){ _putwch(MK_BACKSPASE); _putwch(32); }
		_putwch(val);} //_putwch
//else
	return 1;
}*/
/////////////////////////////////////////////////////////////////////////////////////
inline short print(unsigned short line) { return print((short)line); }
inline short print(char line) { return print((short)ctos(line)); }
inline short print(int line) { return print((short)ctos(line)); }
inline short print(const MString &line) { return print(line, line); }
/////////////////////////////////////////////////////////////////////////////////////
inline int print(char *line){
	int s=strlen(line); for(int i=0; i<s; i++){ print((short)ctos(*line)); line++;} return 1;
}
inline int print(char *line, int s) {
	for(int i=0; i<s; i++){ print((short)ctos(*line)); line++;} return 1;}
inline int print(short *line, int s) {
	for(int i=0; i<s; i++){ print(*line); line++;} return 1;}
inline int print(unsigned short *line, int s) {
	for(int i=0; i<s; i++){ print(*line); line++;} return 1;}

/////////////////////////////////////////////////////////////////////////////////////

int setcurpos(int x, int y){ COORD cr={x, y}; return setcurpos((cr)); }

int setcurpos(COORD &cd){
	if(!inconsole){
#ifdef WIN32
SetConsoleCursorPosition(_std_output_handle, cd);
#else
		char d[]={27, '[', cd.X, '1', ';', cd.Y, '2', 'f', 0 };
write(0, (char*)&d, 8);
#endif
	}
return 1;
}


//inline int putchar(int line){
//if(useconsole) {_putwch(buf[sz-1]);}
//}

inline int readinput(unsigned short*buf, int tbufsz, bool ispass=0){
unsigned short *key=buf; return readinput(buf, tbufsz, key, ispass);
}
/////////////////////////////////////////////////////////////////////////////////////
void cinfo(int sz, bool is){
#ifdef WIN32
	CONSOLE_CURSOR_INFO ci; ci.dwSize=sz; ci.bVisible=is; SetConsoleCursorInfo(_std_output_handle, &ci);
#else
	// add linux code
#endif
}
/////////////////////////////////////////////////////////////////////////////////////
#define readinput_normal(buf, key, rbuf, ispass){ \
for(unsigned short *k=key; k<rbuf; k++){ if(ispass) print("*"); else print(*k); } print(32); setcurpos(cx, cy); }

//GetConsoleScreenBufferInfo(_std_output_handle, &bi); \

// print(8); //for(unsigned short *k=key; k<rbuf; k++){ print(8); }

//
//#ifdef WIN32
void _movekey(int x){ if(x<-S16K || x>S16K) return ;
while(cx<0) {cx+=sx; cy--;}
while(cx>=sx) {cx-=sx; cy++;}
setcurpos(cx, cy);

#ifdef WIN32_NOIS
//CONSOLE_SCREEN_BUFFER_INFO bi;
GetConsoleScreenBufferInfo(_std_output_handle, &bi);
bi.dwCursorPosition.X+=x;
while(bi.dwCursorPosition.X<0) {bi.dwCursorPosition.X+=sx; bi.dwCursorPosition.Y--;}
while(bi.dwCursorPosition.X>=sx) {bi.dwCursorPosition.X-=sx; bi.dwCursorPosition.Y++;}
setcurpos(bi.dwCursorPosition);
cx=bi.dwCursorPosition.X; cy=bi.dwCursorPosition.Y;
#endif
}
//#endif

/////////////////////////////////////////////////////////////////////////////////////
int readinput(unsigned short*buf, int tbufsz, unsigned short *&key, bool ispass=0){
	unsigned short*rbuf=buf; unsigned short c;// CONSOLE_SCREEN_BUFFER_INFO bi;

while(1){
contype ct=read();

if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ c=ct.getcode();// print(itos(c)); print(".");
	if(!(ct.getkeys()&CN_KEY)){
		

	if(c==MK_BACKSPASE || c==127){ if(key>buf){// && key<=rbuf){
			memcpy(key-1, key, (rbuf-key)*2); print(8); key--; rbuf--;
			if(key!=rbuf) {readinput_normal(buf, key, rbuf, ispass);} else { print(32); print(8);}}
	} else if(c==MK_ESCAPE) { if(rbuf>buf){ _movekey(buf-key); key=buf;
		memset(buf, 0, (rbuf-buf)*2); readinput_normal(buf, key, rbuf, ispass);
		rbuf=buf;}
	} else if(c==13 || c==10){
		print("\r\n"); break;
	} else {
		if(rbuf-buf>=tbufsz) break;
		if(key!=rbuf){
			memcpy(key+1, key, (rbuf-key)*2); readinput_normal(buf, key, rbuf+1, ispass);			
		} *(key)=c; rbuf++; key++; if(ispass) print("*"); else print(c);
		cinfo(cursz, 1); }
	} else {
		if(c==MK_DELETE){ if(key>=buf && key<rbuf){
            memcpy(key, key+1, (rbuf-key-1)*2); rbuf--;
			readinput_normal(buf, key, rbuf, ispass);
		}}
		else if(c==MK_LEFT){ if(key>buf){
			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)){
			unsigned short *k=key; rrts_con(buf, rbuf, key); _movekey(key-k);
			} else {_movekey(-1); key--;}
		}}
		else if(c==MK_RIGHT){ if(key<rbuf){
			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)){
			unsigned short *k=key; rts_con(buf, rbuf-buf, key); _movekey(key-k);
			} else {_movekey(1); key++;}
		}}
		else if(c==MK_UP){  }
		else if(c==MK_DOWN){  }
		else if(c==MK_END){ _movekey(rbuf-key); key=rbuf;  }
		else if(c==MK_HOME){ _movekey(-(key-buf)); key=buf; }
		}
}}  return rbuf-buf;
/*
print(8);// print(' '); print(8);// c=i;
//read(0, &c, 1);
if(c==8 || c==127) {if(rbuf>buf) {if(rbuf-buf>1 && *(unsigned char*)(rbuf-1)>128) rbuf-=2; else rbuf--;} else putchar(':'); if(c==127) { putchar(8);} putchar(' '); putchar(8);}
else if(c==13 || c==10) {
if(_TSYS==TSYS_LIN && c==10) print("\e[1A");
*rbuf=0; if(_TSYS==TSYS_WIN) print("\r\n"); break; //return ret.set(buf, bufsz);
}else { if(c>=128){
	if(_TSYS==TSYS_LIN) {*(short*)(rbuf)=c; rbuf++; *(rbuf)=0; print(rbuf-1);}
	else {if(stosrdos(c)>128) {*(unsigned short*)(rbuf)=stoutf(stosrdos(c)); rbuf+=2;} else {*(rbuf)=stosrdos(c); rbuf++;}}
} else {*(rbuf)=c; rbuf++;} /*putchar(8);* / } if(ispass) {putchar(8);putchar('*');}
*/
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
contype read(){
	/*short c; */ contype ct;

	if(readbufs>=2 && readbufs>=((contype*)readbuf)->getsz()){
		int s=((contype*)readbuf)->getsz();
		memcpy(&ct, readbuf, s); memcpy(readbuf, readbuf+s, readbufs-s);
		return ct;
	}

/////////////////////////////////////////////////////////////////////////////////////
	if(!inconsole){
#ifdef WIN32
	DWORD rd; //	SHIFT_PRESSED
	ReadConsoleInputW(_std_input_handle, ir, 1+0*CNIRSZ, &rd);
	for(unsigned int i=0; i<rd; i++){
		switch(ir[i].EventType){
			case KEY_EVENT:
				if(!ir[i].Event.KeyEvent.uChar.UnicodeChar) {ir[i].Event.KeyEvent.uChar.UnicodeChar=ir[i].Event.KeyEvent.wVirtualKeyCode; ir[i].Event.KeyEvent.dwControlKeyState|=CN_KEY;}
					//ir[i].Event.KeyEvent.uChar.UnicodeChar
				ct.SetKey(ir[i].Event.KeyEvent.bKeyDown, ir[i].Event.KeyEvent.dwControlKeyState ,ir[i].Event.KeyEvent.uChar.UnicodeChar); break;
			case MOUSE_EVENT:
				ct.SetMouse(ir[i].Event.MouseEvent.dwControlKeyState, ir[i].Event.MouseEvent.dwEventFlags, ir[i].Event.MouseEvent.dwButtonState, ir[i].Event.MouseEvent.dwMousePosition); break;
			case WINDOW_BUFFER_SIZE_EVENT:
				ct.SetWindow(ir[i].Event.WindowBufferSizeEvent.dwSize); break;
			case FOCUS_EVENT:
				ct.SetFocus(ir[i].Event.FocusEvent.bSetFocus); break;
}}


/*		if(ir.EventType==KEY_EVENT){
			if(ir.Event.KeyEvent.bKeyDown) return ir.Event.KeyEvent.uChar.UnicodeChar;
		}
		if(ir.EventType==MOUSE_EVENT){
			if(ir.Event.MouseEvent.dwButtonState)
				int ert=45;
			//print(itos((int)ir.Event.MouseEvent.dwButtonState)+" ");
		}

		if(ir.EventType!=16 && ir.EventType!=8){
			int rty=456;//if(ir.Event.KeyEvent.bKeyDown) return ir.Event.KeyEvent.uChar.UnicodeChar;
		}*/
		
		// Sleep(20);
#endif
	}
//    return _getwche();
//return stosr(c);
return ct;
}
/////////////////////////////////////////////////////////////////////////////////////

void setcol(const MRGB& col){ defc=col.sh(); }
void setbgcol(const MRGB& col){ defbg=col.sh(); }
void setcurcol(const MRGB& col){ curc=col.sh(); }

};

COORD TTYSize(){
	COORD cd={0, 0};
#ifndef WIN32
	int rc, fd;
	struct winsize ws;
	
    fd = open( "/dev/tty", O_RDONLY );
    if(fd<0){ return cd; }

    rc = ioctl( fd, TIOCGWINSZ, &ws);
//	printf( "terminal size: %dx%d\n", ws.ws_col, ws.ws_row );	
	
	close(fd);
	if(rc){ // !=sizeof(winsize)
		//print("unnormal size winsize\r\n");
		return cd;
	} else{ cd.X=ws.ws_col; cd.Y=ws.ws_row; }	
#endif
	return cd;
 //   printf( "terminal size: %dx%d\n", ws.ws_col, ws.ws_row );
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Console class END///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// Pipeline class /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#define PPLNULL	0
#define PPLPIPE	1
#define PPLRBUF	S8K

struct pipe_timeline{ unsigned int pos, tm, ml; };

#ifdef WIN32
#define pipe _pipe
#endif

class PipeEnv{
	MString d;
public:

	void New(int sz){
		d.Reserv(sz*sizeof(void*));
		return ;
	}

	void Set(int i, VString val){
		//if(d.size()/sizeof(void*)<i)

	}



};

GLock pipelock;// int maxpipelock=512, thispipelock=0;

class PipeLine{
	int upipe[2];
	int stat;

	VString env, inp;
	HLString *tml;

	// For OutBuffer
	int isoutbuffer, isprocess;

	unsigned char buf[PPLRBUF];           //буфер ввода/вывода

#ifdef WIN32
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;        //структура security для пайпов
	PROCESS_INFORMATION pi;

	HANDLE newstdin, newstdout, read_stdout, write_stdin;  //дескрипторы пайпов

	FILETIME StartTime, EndTime;
	FILETIME KernelTime, UserTime;

	unsigned long exitcode;  //код завершения процесса
	unsigned long bread;   //кол-во прочитанных байт
	unsigned long avail;   //кол-во доступных байт
#else
	int p_stdin[2], p_stdout[2], p_stderr[2], p_stdret[2], pid;// int *prc=&rc; int *rrc=new int;


#endif


public:
	int64 usertime, kerneltime, cputime;

	PipeLine(){
		stat=PPLNULL; isoutbuffer=0; isprocess=0;
		tml=0;
	}

	~PipeLine(){
#ifdef WIN32
		if(isprocess) EndProcess();
#else
		int rc; HLString hls, ehls;
		if(isprocess) EndProcess(rc, hls, ehls, 0);
#endif
	}

	int Create(){
	//#ifdef __GNUC__

	//	if(pipe( upipe ) == -1 ) return 0;

	stat=PPLPIPE;
	return 1;
	}

	void OutBuffer(int i){ isoutbuffer=i; }
	unsigned char* GetData(){ return buf; }


#define bzero(a) memset(a, 0, sizeof(a)) //для сокращения писанины


#ifdef WIN32
#define pipe _pipe

	bool IsWinNT(){  //проверка запуска под NT
		OSVERSIONINFO osv;
		osv.dwOSVersionInfoSize = sizeof(osv);
		GetVersionEx(&osv);
		return (osv.dwPlatformId == VER_PLATFORM_WIN32_NT);
	}

	void ErrorMessage(char *str){  //вывод подробной информации об ошибке
	LPVOID msg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // язык по умолчанию
		(LPTSTR) &msg,
		0,
		NULL
	);

	printf("%s: %s\n",str,msg);
	LocalFree(msg);
	}

	int fork(){ return 0; }
	int execle(char *){ return 0; }
#endif

	void SetEnv(VString line){ env=line; }
	void SetInput(VString line){ inp=line; }
	void SetTimeLine(HLString &tm){ tml=&tm; }
	void SetTimeLine(){ tml=0; }

#ifndef WIN32

int Run(MString cmd, int &rc, HLString &hls, HLString &ehls, int tocon=1){
cputime=0; kerneltime=0; usertime=0;

if(isprocess) EndProcess(rc, hls, ehls, tocon);

pipelock.Lock();
//if(thispipelock+4>maxpipelock) return 0;
if(pipe(p_stdin)<0){ Sleep(100);// print("IP. ");
	pid=0; while(pipe(p_stdin)<0 && pid++<5) Sleep(100);
	if(pid>=5){
		//close(p_stdin[0]); close(p_stdin[1]);
		//close(p_stdout[0]); close(p_stdout[1]);
		//close(p_stderr[0]); close(p_stderr[1]);
		print("Died on pipe(p_stdin, ", itos(errno), ")!\n"); pipelock.UnLock(); return 0;
	}
}
if(pipe(p_stdout)<0){ Sleep(10);// print("OP. ");
	pid=0; while(pipe(p_stdout)<0 && pid++<5) Sleep(100);
	if(pid>=5){
		close(p_stdin[0]); close(p_stdin[1]);
		//close(p_stdout[0]); close(p_stdout[1]);
		//close(p_stderr[0]); close(p_stderr[1]);
		print("Died on pipe(p_stdout, ", itos(errno), ")!\n"); pipelock.UnLock(); return 0;
	}
}
if(pipe(p_stderr)<0){ Sleep(10);// print("EP. ");
	pid=0; while(pipe(p_stderr)<0 && pid++<5) Sleep(100);
	if(pid>=5){
		close(p_stdin[0]); close(p_stdin[1]);
		close(p_stdout[0]); close(p_stdout[1]);
		//close(p_stderr[0]); close(p_stderr[1]);
		print("Died on pipe(p_stderr, ", itos(errno), ")!\n"); pipelock.UnLock(); return 0;
	}
}
if(pipe(p_stdret)<0){ Sleep(10);// print("RP. ");
	pid=0; while(pipe(p_stdret)<0 && pid++<5) Sleep(100);
	if(pid>=5){
		close(p_stdin[0]); close(p_stdin[1]);
		close(p_stdout[0]); close(p_stdout[1]);
		close(p_stderr[0]); close(p_stderr[1]);
		print("Died on pipe(p_stdret, ", itos(errno), ")!\n"); pipelock.UnLock(); return 0;
	}
}
//thispipelock+=4;
pipelock.UnLock();

if ((pid = fork())<0) { printf("Died on fork()!\n"); return 0; }

if(pid==0){
    close(p_stdin[1]); close(p_stdout[0]); close(p_stderr[0]); close(p_stdret[0]);

    dup2(p_stdin[0], fileno(stdin));
    dup2(p_stdout[1], fileno(stdout));
    dup2(p_stderr[1], fileno(stderr));
	

//	write( fileno(stdout), cmd, cmd);
	//*prc=3;

/*	ILink il; il.Ilink(cmd); char *p[S1K], *e[S1K];
	HLString arg; arg+il.GetPath()+il.file+_rc;
	VString a; il.FirstArg(); while(a=il.GetArg()){ arg+a+_rc; } arg+_rc; arg.oneline();

	// p,e.
	int i=0; 
	while(ln<to){
		p[i]=ln; i++; if(i>=S1K-1) break;
		while(ln<to && *ln!=0) ln++;
		while(ln<to && *ln==0) ln++;	
	} p[i]=0;*/

	char*ln=env, *to=ln+env.size();
	//ln=env; to=ln+env.size(); i=0;
	while(ln<to){
		if(*ln) putenv(ln);// printf(ln); printf("\r\n");
		//e[i]=ln; i++; if(i>=S1K-1) break;
		while(ln<to && *ln!=0) ln++;
		while(ln<to && *ln==0) ln++;	
	} //e[i]=0;

 //putenv(MString("DOCUMENT_ROOT=/home/server/vm/projects/MSV-SNT/Termo/http-serv/"));
 //putenv(MString("SCRIPT_FILENAME=/home/server/vm/projects/MSV-SNT/Termo/http-serv/index.php"));

	//rc=execve(il.GetPath()+il.file, p, e);

	rc=system(cmd);// if(rc<0) rc=0;
//	HLString ls; ls+"<"+itos(rc)+">"; write(fileno(stdout), ls.nomov(), ls.size());
	write(p_stdret[1], &rc, 4);

struct rusage ru;
rc=getrusage(RUSAGE_SELF, &ru);
if(rc){ memset(&ru.ru_utime, 0, sizeof(struct timeval)*2); }
write(p_stdret[1], &ru.ru_utime, sizeof(struct timeval)*2);

	close(p_stdin[0]); close(p_stdout[1]); close(p_stderr[1]); close(p_stdret[1]);
    exit(0);
}

	if(pid>0){ close(p_stdin[0]); close(p_stdout[1]); close(p_stderr[1]); close(p_stdret[1]); }

	if(inp) write(p_stdin[1], inp, inp);

	isprocess=1;
	if(!isoutbuffer) Process(rc, hls, ehls, tocon);

	return 1;
}

int Process(int &rc, HLString &hls, HLString &ehls, int tocon=1){
	if(!isprocess) return 0;

	//char buf[S4K]; int rbufi=1, ebufi=1;
	fd_set rfds, efds; timeval tm; int sel, maxs=0;
	int rbufi=1, ebufi=1;

	while(1){
		FD_ZERO(&rfds); FD_ZERO(&efds); maxs=0; tm.tv_sec=1; tm.tv_usec=0;
		FD_SET(p_stdout[0], &rfds); FD_SET(p_stderr[0], &rfds); maxs=p_stdout[0]>p_stderr[0] ? p_stdout[0] : p_stderr[0];
		FD_SET(p_stdout[0], &efds); FD_SET(p_stderr[0], &efds); maxs++;

		sel=select(maxs, &rfds, 0, &efds, &tm);// print("S ");
		
		if(FD_ISSET(p_stdout[0], &rfds)){// print("R ");
			rbufi=read(p_stdout[0], buf, sizeof(buf));// if(rbufi<=0) break; // PPLRBUF
			
			if(tocon) print(VString(buf, rbufi));
			
			if(tml){
				pipe_timeline pt; pt.pos=hls.size();
				timeb tm=alltime(); pt.tm=tm.time; pt.ml=tm.millitm; *tml+VString((char*)&pt, sizeof(pt));
			}
			
			if(isoutbuffer) return rbufi;
			else hls.add((char*)buf, rbufi);
			
			if(rbufi>0) continue;
		}

		if(FD_ISSET(p_stderr[0], &rfds)){// print("RE ");
			ebufi=read(p_stderr[0], &buf, sizeof(buf));// if(bufi<0) break;
			
			if(tocon) print(VString(buf, ebufi));
			
			if(isoutbuffer) return ebufi;
			else ehls.add((char*)buf, ebufi);
			
			if(ebufi>0) continue;
			//if(ebufi>0) break;
		}

		if(FD_ISSET(p_stdout[0], &efds) || FD_ISSET(p_stderr[0], &efds) || ( rbufi<=0 && ebufi<=0)){ break; }

		if(sel<0) break;
	}

	return EndProcess(rc, hls, ehls, tocon);
}

int EndProcess(int &rc, HLString &hls, HLString &ehls, int tocon=1){
	if(!isprocess) return 0;
	fd_set rfds, efds; timeval tm; int sel, maxs=0;
	int ebufi;



//	print("s:", itos(sel), " ");

//}
/*	  
	while((bufi=read(p_stdout[0], &buf, S4K))>0){
		print(VString(buf, bufi));
		hls.add(buf, bufi);
	}

	while((bufi=read(p_stderr[0], &buf, S4K))>0){
		print(VString(buf, bufi));
		ehls.add(buf, bufi);
	}
*/

	//HLString ls; ls+"("+itos(*rc)+")"; write(fileno(stdout), ls.nomov(), ls.size());
	read(p_stdret[0], &rc, 4);
	struct timeval ctm[2];
	int r=read(p_stdret[0], &ctm, sizeof(struct timeval)*2);
	if(r==sizeof(struct timeval)*2){  }
	usertime=int64(ctm[0].tv_sec)*1000000+ctm[0].tv_usec*1000;
	kerneltime=int64(ctm[1].tv_sec)*1000000+ctm[1].tv_usec*1000;
	cputime=kerneltime+usertime;

	r=waitpid(pid, 0, 0);// print(itos(r), ". ");
	//if(rbufi+ebufi<=0) return 0;
	//if(hls){ printf("HLS!!! "); } // printf(hls.oneline()); }
	//if(ehls) printf("EHLS!!! ");

	//printf("END!!! "); printf(itos(rc)); printf("\r\n");
	close(p_stdin[1]); close(p_stdout[0]); close(p_stderr[0]); close(p_stdret[0]);
	if(tml){ tml->oneline(); tml=0; }
	//pipelock.Lock(); thispipelock-=4; pipelock.UnLock();
	return 1;
}

#else // WIN 32
//#ifdef WIN32_NOUSEIT_456

int Run(MString cmd, int &rc, HLString &hls, HLString &ehls, int tocon=1){
	cputime=0; kerneltime=0; usertime=0;

	if(isprocess)
		EndProcess();

	if(IsWinNT()){	//инициализация security для Windows NT
		InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&sd, true, NULL, false);
		sa.lpSecurityDescriptor = &sd;
	}
	else sa.lpSecurityDescriptor = NULL;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;	//разрешаем наследование дескрипторов

	if(!CreatePipe(&newstdin,&write_stdin,&sa,0)){	//создаем пайп для stdin
		ErrorMessage("CreatePipe");
		//getch();
		return 0;
	}

	if(!CreatePipe(&read_stdout,&newstdout,&sa,0)){ //создаем пайп для stdout
		ErrorMessage("CreatePipe");
		//getch();
		CloseHandle(newstdin);
		CloseHandle(write_stdin);
		return 0;
	}

	GetStartupInfo(&si);	//создаем startupinfo для дочернего процесса

  /*
  Параметр dwFlags сообщает функции CreateProcess
  как именно надо создать процесс.

  STARTF_USESTDHANDLES управляет полями hStd*.
  STARTF_USESHOWWINDOW управляет полем wShowWindow.
  */

	  si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
	  si.wShowWindow = SW_HIDE;
	  si.hStdOutput = newstdout;
	  si.hStdError = newstdout;   //подменяем дескрипторы для
	  si.hStdInput = newstdin;    // дочернего процесса

	//создаем дочерний процесс
	// ILink il; il.Ilink(cmd);

	if(!CreateProcess(NULL, MODUNICODE(cmd), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, env, NULL, &si, &pi)){
		//il.GetPath()+il.file, il.iquest
		ErrorMessage("CreateProcess");
		//getch();
		CloseHandle(newstdin);
		CloseHandle(newstdout);
		CloseHandle(read_stdout);
		CloseHandle(write_stdin);
		return 0;
	}

	if(inp) WriteFile(write_stdin, inp, inp.size(), &bread, NULL);
	//CloseHandle(newstdin);
	//CloseHandle(write_stdin);


	bzero(buf);

	memset(&StartTime, 0, sizeof(StartTime)*4);
	exitcode=0;

	isprocess=1;
	if(!isoutbuffer) Process(rc, hls, ehls, tocon);

	return 1;
	}

	int Process(int &rc, HLString &hls, HLString &ehls, int tocon=1){
		if(!isprocess) return 0;

		//_INPUT_RECORD ir[4];
		//DWORD rd; //	SHIFT_PRESSED
		//ReadConsoleInputW(read_stdout, ir, 4, &rd);

		//CHAR_INFO ci[60]; COORD cd={10,10}, p={0, 0}; SMALL_RECT sr; memset(ci, 0, sizeof(ci));
		//int r=ReadConsoleOutput(read_stdout, ci, cd, p, &sr);

	for(;;){	//основной цикл программы
		GetExitCodeProcess(pi.hProcess, &exitcode); //пока дочерний процесс не закрыт

		PeekNamedPipe(read_stdout, buf, PPLRBUF-1, &bread, &avail, NULL); if(!bread) Sleep(1);

		//Проверяем, есть ли данные для чтения в stdout

    if(bread!=0){
		bzero(buf);
		while(avail){
			ReadFile(read_stdout,buf,PPLRBUF-1,&bread,NULL);  //читаем из пайпа stdout

			if(tml){
				pipe_timeline pt; pt.pos=hls.size();
				timeb tm=alltime(); pt.tm=tm.time; pt.ml=tm.millitm; *tml+VString((char*)&pt, sizeof(pt));
			}

			if(tocon) printf("%s",buf);

			if(isoutbuffer) return bread;
			else{ hls+=VString((char*)&buf, bread); }
			
			avail-=bread; 

			bzero(buf); break;
		}
	}
	else Sleep(1);

		if(exitcode!=STILL_ACTIVE)
			break;
	
	}

	if(tml){ tml->oneline(); tml=0; }
	EndProcess();

	return 0;
	}

	int EndProcess(){
		if(!isprocess) return 0;
		isprocess=0;

		GetProcessTimes(pi.hProcess, &StartTime, &EndTime, &KernelTime, &UserTime);
		kerneltime=*(int64*)&KernelTime/10; usertime=*(int64*)&UserTime/10; cputime=kerneltime+usertime;

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(newstdin);            //небольшая уборка за собой
		CloseHandle(newstdout);
		CloseHandle(read_stdout);
		CloseHandle(write_stdin);

		pi.hThread=0; pi.hProcess=0; newstdin=0; newstdout=0; read_stdout=0; write_stdin=0;

		return 1;
	}

#endif

#ifdef NOUSEIT_571
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwDummy;
	CONST COORD origin = { 0, 0 };
SECURITY_ATTRIBUTES sa; sa.nLength = sizeof(sa); sa.lpSecurityDescriptor = NULL; sa.bInheritHandle = TRUE;

HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,&sa,CONSOLE_TEXTMODE_BUFFER,NULL);
//FillConsoleOutputCharacter(hConsole, '\0', MAXLONG, origin, &dwDummy); // fill screen buffer with zeroes
//SetStdHandle(STD_OUTPUT_HANDLE, hConsole); // to be inherited by child process

PROCESS_INFORMATION pi; STARTUPINFO si; 
SECURITY_ATTRIBUTES sd;
HANDLE newstdin,newstdout,read_stdout,write_stdin;

InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
SetSecurityDescriptorDacl(&sd, true, NULL, false);
sa.lpSecurityDescriptor = &sd;

sa.nLength = sizeof(SECURITY_ATTRIBUTES);
sa.bInheritHandle = true;       //разрешаем наследование дескрипторов

if(!CreatePipe(&newstdin,&write_stdin,&sa,0)){
	print("#!PIPE ERROR!#\r\n"); return 0;
}

if (!CreatePipe(&read_stdout,&newstdout,&sa,0)) {
	print("#!PIPE ERROR!#\r\n"); return 0;
}
    HANDLE inWrite_, outRead_, process_;
	   HANDLE inRead, inWrite, outRead, outWrite, errWrite;


	 // Create non-inheritable copies of handle, our child process should not inherit
        HANDLE curProc = GetCurrentProcess();
     //   DuplicateHandle(curProc, write_stdin, curProc, &inWrite_, 0, FALSE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);
     //   DuplicateHandle(curProc, read_stdout, curProc, &outRead_, 0, FALSE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);

        // Create a copy of out handle for error output
    //    DuplicateHandle(curProc, newstdout, curProc, &errWrite, 0, TRUE, DUPLICATE_SAME_ACCESS);


GetStartupInfo(&si);      //создаем startupinfo для дочернего процесса

/*
  Параметр dwFlags сообщает функции CreateProcess как именно надо создать процесс.
  STARTF_USESTDHANDLES управляет полями hStd*.
  STARTF_USESHOWWINDOW управляет полем wShowWindow.
*/

si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
si.wShowWindow = SW_HIDE;
si.hStdOutput = _std_output_handle; //newstdout;
si.hStdError = newstdout;   //подменяем дескрипторы для
si.hStdInput = _std_input_handle;//  newstdin;    // дочернего процесса

//char app_spawn[] = "c:\\windows\\system32\\cmd.exe";
char app_spawn[] = "c:\\nc\\far\\far.exe";


//создаем дочерний процесс CREATE_NEW_CONSOLE DETACHED_PROCESS
if (!CreateProcess(app_spawn,NULL,NULL,NULL,0,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi)){
//	ErrorMessage("CreateProcess");
//	getch();
	CloseHandle(newstdin);
	CloseHandle(newstdout);
	CloseHandle(read_stdout);
	CloseHandle(write_stdin);
	print("#!command not found!#\r\n");
	return 0;
}

unsigned long exit=0;  //код завершения процесса
unsigned long bread;   //кол-во прочитанных байт
unsigned long avail;   //кол-во доступных байт
MString buf; buf.Reserv(S1K);


  for(;;)      //основной цикл программы
  { Sleep(10);
    GetExitCodeProcess(pi.hProcess,&exit); //пока дочерний процесс // не закрыт
    if (exit != STILL_ACTIVE) break;

    PeekNamedPipe(write_stdin, buf, 1023, &bread, &avail, NULL); //read_stdout
    //Проверяем, есть ли данные для чтения в stdout
	if (bread!=0) { buf.Clean(0);
		ReadFile(write_stdin, buf, 1023, &bread, NULL);
		//printf("%s",buf);
		print(buf.str(0, bread));

	}
//*
      if (avail > 1023*2)
      {
        while (bread >= 1023)
        {
          ReadFile(read_stdout,buf,1023,&bread,NULL);  //читаем из
                                                       // пайпа stdout
          printf("%s",buf);
         // bzero(buf);

        }
	  }

      else {

       // printf("%s",buf);
      }
	}//*/

/*	if(kbhit()){ //continue;
		ct=console.read(); ct.GetWinf(ir);
		if(ct.gett()==CNTKEY)
			int etety=56;
		unsigned short keys=ct.getkeys(), us=ct.getcode();
		unsigned char s=ct.getcode(); *buf=s;
		//WriteConsoleInput(write_stdin, &ir, 1, &rd);
		//WriteFile(write_stdin, &c, 1, &rd, 0);
	//	WriteFile(write_stdin,&s,1,&bread,NULL);
		//WriteConsoleOutput(write_stdin, &ir, 1, &rd);
    //if (kbhit())      //проверяем, введено ли что-нибудь с клавиатуры

      //bzero(buf);
	//	buf.Clean(0);
     // *buf = (char)getche();
* /


	if(ct.gett()==CNTKEY && keys&CN_DOWN){
      //printf("%c",*buf); //console.print(*buf.rchar());
		WriteFile(write_stdin,&s,1,&bread,NULL); //отправляем это в stdin
		//WriteConsoleOutput(write_stdin, &us, 0, 0, 0);
		//WriteConsoleInput(write_stdin, &ir, 1, &rd);
		if(!(ct.getkeys()&CN_KEY)) console.print(us);
		if (s == '\r') { s = '\n';
			WriteFile(write_stdin,&s,1,&bread,NULL); console.print(s); //формирум конец строки, если нужно
	}}
        //printf("%c",*buf);
		//console.print(*buf.rchar())
	}*/


  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  CloseHandle(newstdin);            //небольшая уборка за собой
  CloseHandle(newstdout);
  CloseHandle(read_stdout);
  CloseHandle(write_stdin);


  return 1;
}}
#endif

};

class GlobalConsole;


class VirtualConsole{
	int sumbol[16], sumbols;
	GlobalConsole *console;

public:

	VirtualConsole(){ console=0; sumbols=0; }
	
// GlobalConsole
	int VCSetConsole(GlobalConsole *con);
	int InRead(contype ct);


// Real Console
	virtual int VCSetTitle(VString title){ return 0; }
	virtual int VCSetCursor(unsigned short x, unsigned short y){ return 0; }
	virtual int VCSetColor(unsigned short col){ return 0; }
	virtual int VCSetBgColor(unsigned short col){ return 0; }
	virtual int VCSetChar(unsigned short chr){ return 0; }

};


#define DRAWDLINE_TEXT	1
#define DRAWDLINE_COLOR	2
#define DRAWDLINE_BGCOLOR	3
#define DRAWDLINE_FONT	4
#define DRAWDLINE_RECT	5
//#define DRAWDLINE_RN	6
#define DRAWDLINE_END	7

enum DRAWDLINE_TYPES{
	CCDRW_NULL,
	CCDRW_BG,
	CCDRW_RN,
	CCDRW_ENDLINE
};

struct DrawDLineText{ int type; unsigned int sz; unsigned char data[]; };
struct DrawDLineColor{ int type; RGBQUAD rgb; };
struct DrawDLineFont{ int type; LOGFONT font; };
struct DrawDLineRect{ int type; RECT rect; };
struct DrawDLineEnd{ int type; };

union DrawDLineU{
	int type;
	DrawDLineText t;
	DrawDLineColor c;
	DrawDLineFont f;
	DrawDLineRect r;
	DrawDLineEnd e;
};

class DrawDLine{
HLString data;
int bls, pos; DRAWDLINE_TYPES ntype;

public:
	DrawDLine(){ bls=0; pos=0; ntype=CCDRW_NULL; }
	~DrawDLine(){  }

	DrawDLine& operator+(const VString line){
		DrawDLineText d;
		d.type=DRAWDLINE_TEXT; d.sz=line;
		data.add((char*)&d, sizeof(d)); data+line; bls++;
		return *this;
	}
	
	DrawDLine& operator+(const char *line){ return operator+(VString(line)); }

	DrawDLine& operator+(DRAWDLINE_TYPES t){
		if(t==CCDRW_ENDLINE){
			DrawDLineText d; d.type=DRAWDLINE_END;
			data.add((char*)&d, sizeof(d)); bls++;
			return *this;
		}
		ntype=t;
		return *this;
	}

	DrawDLine& operator+(MRGB line){
		DrawDLineColor d;
		d.type=DRAWDLINE_COLOR; d.rgb=line;
		if(ntype==CCDRW_BG){ d.type=DRAWDLINE_BGCOLOR; ntype=CCDRW_NULL; }
		data.add((char*)&d, sizeof(d)); bls++;
		return *this;
	}

	DrawDLine& operator+(MRect line){
		DrawDLineRect d;
		d.type=DRAWDLINE_RECT; d.rect=line;
		data.add((char*)&d, sizeof(d)); bls++;
		return *this;
	}

	DrawDLine& operator+(LOGFONT line){
		DrawDLineFont d;
		d.type=DRAWDLINE_FONT; d.font=line;
		data.add((char*)&d, sizeof(d)); bls++;
		return *this;
	}

	DrawDLineU* Get(){
		char *d=data.oneline()+pos;
		if(pos>=data.size()) return 0;
		switch(*(int*)d){
			case DRAWDLINE_TEXT:
				pos+=sizeof(DrawDLineText)+*(unsigned int*)(d+sizeof(int));
				break;

			case DRAWDLINE_COLOR:
				pos+=sizeof(DrawDLineColor);
				break;

			case DRAWDLINE_BGCOLOR:
				pos+=sizeof(DrawDLineColor);
				break;

			case DRAWDLINE_RECT:
				pos+=sizeof(DrawDLineRect);
				break;

			case DRAWDLINE_FONT:
				pos+=sizeof(DrawDLineFont);
				break;
			case DRAWDLINE_END:
				pos+=sizeof(DrawDLineEnd);
				break;
			default: return 0; break;

		}
		return (DrawDLineU*)d;
	}

	void Clear(){
		data.Clear(); bls=0; pos=0; ntype=CCDRW_NULL;
	}

};


class DrawBLine{
	// rect
	MRect rect; int sz;
	// data
	MString data; // data, color, bgcolor.
	// link
	unsigned short *vbuf, *cbuf, *bbuf;
	// cursors
	int x, y, pos;
	// colors
	unsigned short col, bg;
	// type
	DRAWDLINE_TYPES ntype;

public:
	DrawBLine(){ Clean(); }
	~DrawBLine(){ Clean(); }

	void Clean(){ data.Clean(); sz=0; vbuf=0; cbuf=0; bbuf=0; x=0; y=0; rect.Clean(); col=65534; bg=65534; ntype=CCDRW_NULL; }

	void Box(const MRect &rc){
		sz=rc.GetW()*rc.GetH();
		if(sz>S1M){ sz=0; return ; }
		
		// reserve
		data.Reserv(sz*6); rect=rc;
		vbuf=(unsigned short*)data.data;
		cbuf=(unsigned short*)(data.data+sz*2);
		bbuf=(unsigned short*)(data.data+sz*4);

		for(int i=0; i<sz; i++){ cbuf[i]=65534; } memcpy(bbuf, cbuf, sz*2);

		// cursors
		x=0; y=0; pos=0;
		return ;
	}

	void Box(int sx, int sy){ Box(MRect(0, 0, sx, sy)); return ; }

	// locate
	void Locate(int _x, int _y){
		x=_x; y=_y; pos=x+y*rect.GetW();
	}
	// get locate
	int getx()const{ return x; }
	int gety()const{ return y; }
	int getp()const{ return pos; }

	// get size
	int getw(){ return rect.GetW(); }
	int geth(){ return rect.GetH(); }
	
	// get data
	unsigned short* getvbuf(){ return vbuf; }
	unsigned short* getcbuf(){ return cbuf; }
	unsigned short* getbbuf(){ return bbuf; }

	// Clear
	void Clear(unsigned short chr, unsigned short _col=65534, unsigned short _bg=65534){
		Locate(0, 0); col=_col; bg=_bg;
		for(int i=0; i<sz; i++){
			vbuf[i]=chr; cbuf[i]=_col; bbuf[i]=_bg;
		}
	}

	// Draw
	DrawBLine& operator+(const VString line){
		for(int i=0; i<line.sz; i++){
			if(pos>=sz) break;
			vbuf[pos]=line[i]; cbuf[pos]=col; bbuf[pos]=bg; pos++;
		}
		return *this;
	}
	
	DrawBLine& operator+(const char *line){ return operator+(VString(line)); }

	DrawBLine& operator+(DRAWDLINE_TYPES t){
		if(t==CCDRW_RN){
			pos=pos-pos%rect.GetW()+rect.GetW();
		}
		//if(t==CCDRW_ENDLINE){
		//	DrawDLineText d; d.type=DRAWDLINE_END;
		//	data.add((char*)&d, sizeof(d)); bls++;
		//	return *this;
		//}
		else ntype=t;
		return *this;
	}

	DrawBLine& operator+(MRGB line){
		if(ntype!=CCDRW_BG){ col=line; }
		else{ bg=line; ntype=CCDRW_NULL; }
		return *this;
	}

};