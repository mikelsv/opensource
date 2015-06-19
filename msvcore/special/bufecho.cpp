// Buffer Echo Class
#define MSVBEC_ECHO	1
#define MSVBEC_BUFF	2

class BufEchoClass{
	int opt, dopt;

public:
	BufEchoClass(){
		opt=0;

#ifndef WIN32 //Linux HAVE_TERMIOS_H
		struct termios t; tcgetattr(0, &t);
		if(t.c_lflag&ECHO) opt|=MSVBEC_ECHO;
		if(t.c_lflag&ICANON) opt|=MSVBEC_BUFF;
#else
//#ifdef WIN32 //Windows
#ifndef WINCE
		DWORD par; 
		GetConsoleMode(_std_input_handle, &par);
		if(par&ENABLE_ECHO_INPUT) opt|=MSVBEC_ECHO;
		if(par&ENABLE_ECHO_INPUT) opt|=MSVBEC_BUFF;
#endif
//#endif
#endif
		dopt=opt; //MSVBEC_ECHO|MSVBEC_BUFF;
	}
	
	int echo(){ return opt&MSVBEC_ECHO; }
	int echo(int v){
		if((v!=0) ^ ((opt&MSVBEC_ECHO)==0)) return echo();

#ifndef WIN32 //Linux HAVE_TERMIOS_H
		struct termios t; tcgetattr(0, &t);
		if(v) t.c_lflag |=ECHO; else t.c_lflag &= ~ECHO;
		tcsetattr(0, TCSAFLUSH, &t);
#else
//#ifdef WIN32 //Windows
#ifndef WINCE
	DWORD par; 
	GetConsoleMode(_std_input_handle, &par);
	if(v) par|=ENABLE_ECHO_INPUT; else par&=~ENABLE_ECHO_INPUT;
	SetConsoleMode(_std_input_handle, par);
#endif
//#endif
#endif
		if(v) opt|=MSVBEC_ECHO; else opt&=~MSVBEC_ECHO;
		return echo();
	}

	int buff(){ return opt&MSVBEC_BUFF; }
	int buff(int v){
		if((v!=0) ^ ((opt&MSVBEC_BUFF)==0)) return buff();

#ifndef WIN32 //Linux HAVE_TERMIOS_H
		struct termios t; tcgetattr(0, &t);
		if(v) t.c_lflag |= ICANON; else t.c_lflag &= ~ICANON;
		tcsetattr(0, TCSAFLUSH, &t);
#else
//#ifdef WIN32 //Windows
#ifndef WINCE
	DWORD par; 
	GetConsoleMode(_std_input_handle, &par);
	if(v) par|=ENABLE_LINE_INPUT; else par&=~ENABLE_LINE_INPUT;
	SetConsoleMode(_std_input_handle, par);
#endif
//#endif
#endif		
		if(v) opt|=MSVBEC_BUFF; else opt&=~MSVBEC_BUFF;
		return buff();
	}

	void End(){
		echo(dopt&MSVBEC_ECHO);
		buff(dopt&MSVBEC_BUFF);
	}

	~BufEchoClass(){
		End();
	}

}BufEchoClass;
