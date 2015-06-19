#define PROJECTNAME "mteln"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#define USEMSV_ITOS
#define USEMSV_BUFECHO
#define USEMSV_CONSOLE
#define USEMSV_CONSOLELINE

#include "../../opensource/msvcore/msvcore.cpp"

//#include "../msvlib/crossplatform-console.h"
//#include "../msvlib/line/ConsoleWindow.h"
//#include "../msvlib/line/consoleline.h"

Versions PROJECTVER[]={
	"0.0.1.1", "13.11.2012 16:05",
	"0.0.1.0", "06.11.2012 15:49",
	"0.0.0.1", "21.11.2011 11:42"
};

#include "mteln.h"


void SigStop(int c){
	BufEchoClass.End();
#ifdef WIN32
	SetConsoleTextAttribute(_std_output_handle, ConsoleLine::cicol(MRGB("555555"), MRGB("000000")));
#else
	SetLinuxColor(ConsoleLine::cilcol(MRGB("555555"))); SetLinuxBgColor(ConsoleLine::cilcol(MRGB("000000")));
#endif
	exit(0);
}
class SS{ public: ~SS(){ SigStop(0); } }ss;

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link);
	LoadLang("lang"); BufEchoClass.echo(0); // noecho();

	ConsoleLine cl;

	#ifndef WIN32
	signal(SIGIO, SIG_IGN); signal(SIGKILL, SigStop); signal(SIGSTOP, SigStop);
	#else
	signal(SIGBREAK, SigStop);
	#endif
	signal(SIGTERM, SigStop); signal(SIGINT, SigStop);

	
	print("mteln v", PROJECTVER[0].ver," (", PROJECTVER[0].date, ") [--uselang ", msv_use_lang, "]\r\n\r\n");

	// config
	VString host, bind, data, script;
	unsigned int ishelp=0, isver=0, islist=0, isnobuff=0, isnoecho=0;
	
	// analys command line
	VString l="GO", t; int lto=2;

	while(l){
		if(lto==1){ l=t; t=link.GetArg(); lto=0; }
		else if(lto==2){ l=link.GetArg(); t=link.GetArg(); lto=0; }
		if(!l) break;

		if(l=="--help" || l=="-h" || l=="/?"){ ishelp=1; lto=1; }
		else if(l=="--ver"){ lto=1; isver=1; }
		else if(lto=usemsv_iskey(l)){}
		else if((l=="--bind" || l=="-b") && t.is()){ bind=t; lto=2; }
		else if(l=="--listen" || l=="-l"){ islist=1; lto=1; }
		else if(l=="--nobuff" || l=="-nb"){ isnobuff=1; lto=1; }
		else if(l=="--noecho" || l=="-ne"){ isnoecho=1; lto=1; }
		else if(l=="--script" && t){ script=t; lto=2; }
		else if(!host){ host=l; lto=1; }
		else{ print(Lang("Warning: Uncnown parameter:"), " '", l, "'\r\n"); lto=1; }
		//l=PartArg(link.GetArg(), t);
	}
	
	if(isver){
		VersionsPrint(PROJECTVER, sizeof(PROJECTVER)/sizeof(Versions));
		return 0;
	}

	if(!host || ishelp){ // help
		print(Lang("Usage: mteln [flags: -flag val ] [--listen] host:port"), "\r\n");
		print(Lang(" ## msv options: use --usemsv --help"), "\r\n");
		print(Lang(" ## info"), "\r\n");
		print("-h, --help, /?", "\t\t", Lang("show this help message"), "\r\n");
		print("--ver", "\t\t\t", Lang("print versions list"), "\r\n");
		print(Lang(" ## actions"), "\r\n");
		print("-b, --bind host:ip", "\t", Lang("bind to local ip"), "\r\n");
		print("-l, --listen", "\t\t", Lang("listen port"), "\r\n");
		print(Lang(" ## options"), "\r\n");
		print("-nb, --nobuff", "\t\t", Lang("no buffering input"), "\r\n");
		print("-ne, --noecho", "\t\t", Lang("no input echo"), "\r\n");
		print("--script", "\t\t", Lang("run script"), "\r\n");
		return 0;
	}


	ConIp cip; SOCKET sock=0, asock;
	Itos it; MTelnScript mscr;

	if(isnobuff) BufEchoClass.buff(0);

	if(!islist){
		cip.Ip(host, bind);
		cl.Print(DrawDLine() + MRGB("ffffff") + Lang("Connect to")+ ": '" + host + "'. ");
		if(bind){ cl.Print(DrawDLine() + MRGB("ffffff") + Lang("Bind") + ": '" + bind + "'."); }

		//print(Lang("Connect to"), ": '", host, "'. "); if(bind) print(Lang("Bind"), ": '", bind, "'."); print("\r\n");
		sock=cip.Connect();

		if(sock<=0){ cl.Print(DrawDLine() + MRGB("ff0000") + Lang("Connection refused") + "."); return 0; }
		cl.Print(DrawDLine() + MRGB("00ff00") + Lang("Connected") + ".\r\n");
	}else{
		cip.IpL(host);
		cl.Print(DrawDLine() + MRGB("ffffff") + Lang("Listen") + ": '" + host + "'.\r\n");// if(bind) print(" Bind: '", bind, "'."); print("\r\n");
		asock=cip.Listen();
		if(asock<=0){ cl.Print(DrawDLine() + MRGB("ff0000") + Lang("Listen fail") + ".\r\n"); return 0; }
	}

	cl.Print(DrawDLine() + MRGB("555555") + "");


// data
	char buf[S16K], inbuf[S16K]; int inbufs=0;
	//GlobalConsole gw; //ConsoleWindow cw; gw.NewWnd(&cw);

	while(1){
		if(islist){
			if(ifrecv(asock)){
				sockaddr_in from; int fromlen=sizeof(from);
				sock=accept(asock, (struct sockaddr*)&from, (socklen_t*)&fromlen);
				if(!sock){ cl.Print(DrawDLine() + MRGB("ff0000") + Lang("Accept fail") + "\r\n"); return 0; }
				it.Format("%s: %ip!:%d.\r\n", Lang("Accept connection"), (unsigned int)ntohl(from.sin_addr.s_addr), htons(from.sin_port));
				//print(it);
				cl.Print(DrawDLine() + MRGB("00ff00") + it.ret);
			}
			if(!sock){ Sleep(100); continue; }
		}

		if(script){ mscr.Run(script); mscr.Sock(sock);  }

		while(1){
			if(script){ mscr.Do();  }

			if(ifrecv(sock)){
				int rd=recv(sock, buf, sizeof(buf), 0);
				if(rd<0){ cl.Print(DrawDLine() + MRGB("550000") + "\r\n" + Lang("Connection refused") + ".\r\n"); }
				else if(rd==0){  cl.Print(DrawDLine() + MRGB("555500") + "\r\n" + Lang("Connection closed") + ".\r\n"); }
				else cl.Print(DrawDLine() + MRGB("ffffff") + VString(buf, rd)); //print(VString(buf, rd));

				if(rd<=0){ closesocket(sock); sock=0; break; }
			}

			contype ct=cl.Read(); unsigned int c;
			if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){
								
				c=ct.getxcode(); if(c==65552) continue;
				inbuf[inbufs++]=c;

				if(!isnoecho) cl.Print(DrawDLine() + MRGB("555555") + VString((char*)&c, 1)); //cw.Drawn(c, MRGB("555555"));
				if(c=='\r') cl.Print(DrawDLine() + MRGB("555555") + "\n"); //cw.Drawn('\n', MRGB("555555"));
				if(c==8){ inbufs--; if(inbufs) inbufs--; }

				if(c=='\r' || inbufs+1>=sizeof(inbuf) || isnobuff){
					if(c=='\r' && inbufs<sizeof(inbuf)){ inbuf[inbufs++]='\n'; }
					int se=send(sock, inbuf, inbufs, 0); inbufs=0;
				}				
			}
			Sleep(10);
		}
		if(!islist) break;
	}

	return 0;
}