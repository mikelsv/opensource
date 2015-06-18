#define PROJECTNAME "mscr"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#include "../../opensource/msvcore/msvcore.cpp"

Versions PROJECTVER[]={
"0.0.1.0", "16.01.2010 09:24",
};

#include "bufecho.h"

int SigStopa=0;
void SigStop(int c){ BufEchoClass.End(); if(c!=1) exit(0); }
class SS{ public: SS(){ signal(SIGTERM, SigStop); } ~SS(){ SigStop(1); } }ss;

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link); LoadLang("lang");

	print("mscr v", PROJECTVER[0].ver," (", PROJECTVER[0].date, ") [--uselang ", msv_use_lang, "]\r\n\r\n");
	
	MHash a(MHASHT_SHA1), b(MHASHT_SHA256);


	// config
	MSCR mscr; MHash mh(MHASHT_MD5); VString htype="MSCR", file; int usemh=0;
	MString buf;
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
		else if(l=="-md5"){ usemh=1; htype="MD5"; mh.Type(MHASHT_MD5); lto=1; }
		else if(l=="-rmd160"){ usemh=1; htype="RMD160"; mh.Type(MHASHT_RMD160); lto=1; }
		else if(l=="-sha1"){ usemh=1; htype="SHA1"; mh.Type(MHASHT_SHA1); lto=1; }
		else if(l=="-sha256"){ usemh=1; htype="SHA256"; mh.Type(MHASHT_SHA256); lto=1; }
		else if(l=="-a" || l=="--all"){ usemh=255; htype="ALL"; mh.Type(MHASHT_ALL);  lto=1; }
		else if((l=="-f" || l=="--file") && t){ file=t; lto=2; }
		else{ print(Lang("Warning: Uncnown parameter:"), " '", l, "'\r\n"); lto=1; }
		//l=PartArg(link.GetArg(), t);
	}
	
	if(isver){
		VersionsPrint(PROJECTVER, sizeof(PROJECTVER)/sizeof(Versions));
		return 0;
	}

	if(ishelp){ // help
		print(Lang("Usage: mscr [flags: -flag val ]"), "\r\n");
		print(Lang(" ## msv options: use --usemsv --help"), "\r\n");
		print(Lang(" ## info"), "\r\n");
		print("-h, --help, /?", "\t\t", Lang("show this help message"), "\r\n");
		print("--ver", "\t\t\t", Lang("print versions list"), "\r\n");
		print(Lang(" ## options"), "\r\n");
		print("-md5", "\t\t\t", Lang("use MD5 algorithm"), "\r\n");
		print("-rmd160", "\t\t\t", Lang("use RMD160 algorithm"), "\r\n");
		print("-sha1", "\t\t\t", Lang("use SHA1 algorithm"), "\r\n");
		print("-sha256", "\t\t\t", Lang("use SHA256 algorithm"), "\r\n");
		print("-a, --all", "\t\t", Lang("use All algorithms"), "\r\n");
		print("-f, --file", "\t\t", Lang("read file"), "\r\n");
		return 0;
	}


	if(file){
		HFILE fl=CreateFile(file, O_RDONLY, S_IREAD);
		if(!ishandle(fl)){ print(Lang("File not found"), "\r\n"); return 1; }
		
		int defb=S16K; buf.Reserv(defb);
		while(1){
			int rd=ReadFile(fl, buf, buf);

			if(usemh!=1) mscr.Add(buf, rd);
			if(usemh) mh.Add(VString(buf.data, rd));
			if(rd!=defb) break;
		}
		CloseHandle(fl);
	}
	else {

		int bufi, c; unsigned char*l, *t; 
		int defb=1024; buf.Reserv(defb); l=buf; t=buf.uchar()+defb;
		VString line;

		BufEchoClass.echo(0); BufEchoClass.buff(0);
		print(Lang("Hash type"), ": ", htype, "\r\n");
		if(_TSYS==TSYS_WIN){ print("  ", Lang("Use Ctrl+Z for send EOF"), ".\r\n"); }
		else print("  ", Lang("Use Ctrl+D for send EOF"), ".\r\n");

		while((c=getchar())!=EOF){
			*l++=c;
			if(_TSYS==TSYS_WIN){
				 printf("%c", c); if(c==13) print("\n");
			}else{
				if(c==4){ l--; break; }
				//printf("_%d,", c);
				//if(lc=='^'){
				//	if(c=='D') break;
				//	else{ l--; *l++=lc; *l++=c; lc=0; }	 // printf("%c%c", lc, c);
				//} else if(!lc && c=='^'){ lc=c; l--; }
				//else { printf("%c", c); }
			}
			if(l==t){
				line.setu(buf.uchar(), l-buf.uchar());
				if(usemh!=1) mscr.Add(line);
				if(usemh) mh.Add(line);

				//if(!usemh) mscr.Add(buf.uchar(), l-buf.uchar());
				//else mh.Add(VString(buf.uchar(), l-buf.uchar()));
			//	if(l-buf.uchar()>defb){ memcpy(buf.uchar(), buf.uchar()+defb, l-buf.uchar()-defb); l-=defb; }
				l=buf;
			}
		}

		if(l!=buf.uchar()){
			line.setu(buf.uchar(), l-buf.uchar());
			if(usemh!=1) mscr.Add(line);
			if(usemh) mh.Add(line);

			//if(!usemh) mscr.Add(buf.uchar(), l-buf.uchar());
			//else mh.Add(VString(buf.uchar(), l-buf.uchar()));
		} print("\r\n");

	}

	print("Result: \r\n");

	if(usemh!=1){ print("MSCR: ", mscr.GetHash(), "\r\n"); }
	else if(usemh==1){ mh.Finish(); print(htype, ": ", mh.GetH(), "\r\n"); }
		
	if(usemh==255){
		mh.Finish(MHASHT_MD5); print("MD5: ", mh.GetH(), "\r\n");
		mh.Finish(MHASHT_RMD160); print("RMD160: ", mh.GetH(), "\r\n");
		mh.Finish(MHASHT_SHA1); print("SHA1: ", mh.GetH(), "\r\n");
		mh.Finish(MHASHT_SHA256); print("SHA256: ", mh.GetH(), "\r\n");
	}

	
	return 0;


	{
	//fd_set rfds; int sel=0;
	int bufi; unsigned char*l, *t; // char buf[S4K];
	HLString ls; MString text;  VString file, o;

	VString iquest=link.iquest;
	if(iquest.incompare("-md5")){ usemh=1; htype="MD5"; iquest=iquest.str(4); iquest=dspacev(iquest, 1); }
	else if(iquest.incompare("-rmd160")){ usemh=1; htype="RMD160"; iquest=iquest.str(4); iquest=dspacev(iquest, 1); }

	if(iquest.incompare("-file")){
		iquest=iquest.str(5); iquest=dspacev(iquest, 1);
		file=PartLine(iquest, iquest, " "); iquest=dspacev(iquest, 1);
	}

	//print("MSCR hash. Optinons: -md5. "); if(m5){ print("MD5 style. "); }
	print("MSCH hash v", PROJECTVER[0].ver," (", PROJECTVER[0].date, "). [--uselang ", msv_use_lang, "]\r\n");
	print(Lang("Optinons"), ": -md5 ", Lang("use MD5 algorytm"), ". "); print(Lang("Output format"), ": ", htype); print("\r\n");


	if(!iquest){
		BufEchoClass.echo(0); BufEchoClass.buff(0);
	
		if(_TSYS==TSYS_WIN){ print("  ", Lang("Use Ctrl+Z for send EOF"), "."); }
		else print("  ", Lang("Use Ctrl+D for send EOF"), ".");
	}
	print("\r\n");

	//bufi=read(0, &buf, S4K);
	//print("IN: ", itos(bufi), "\r\n");
	//Sleep(100);

	int c; int defb=1024, isin=0; buf.Reserv(defb); l=buf; t=buf.uchar()+defb;
	if(!iquest)
	while((c=getchar())!=EOF){
	*l++=c;
	if(_TSYS==TSYS_WIN){
		printf("%c", c);
	}else{
		if(c==4){ l--; break; }
		//printf("_%d,", c);
		//if(lc=='^'){
		//	if(c=='D') break;
		//	else{ l--; *l++=lc; *l++=c; lc=0; }	 // printf("%c%c", lc, c);
		//} else if(!lc && c=='^'){ lc=c; l--; }
		//else { printf("%c", c); }
	}
	if(l==t){
		if(!usemh) mscr.Add(buf.uchar(), l-buf.uchar());
		else mh.Add(VString(buf.uchar(), l-buf.uchar()));
	//	if(l-buf.uchar()>defb){ memcpy(buf.uchar(), buf.uchar()+defb, l-buf.uchar()-defb); l-=defb; }
		l=buf; isin=1;  }
	}

	if(l!=buf.uchar()){
		if(!usemh) mscr.Add(buf.uchar(), l-buf.uchar());
		else mh.Add(VString(buf.uchar(), l-buf.uchar()));
		isin=1;
	}

	//	FD_ZERO(&rfds);
	//	FD_SET(0, &rfds); timeval tm; tm.tv_sec=2; tm.tv_usec=0;
		//if((sel=select(1, &rfds, 0, 0, &tm))>0){// print("S ");
		//if(!feof(stdin)){
		//	print("read\r\n");
		//	bufi=fread(&buf, 1, S4K, stdin);// if(rbufi<=0) break;
		//	print(VString(buf, bufi));
	//		ls.add(buf, bufi); Sleep(50);
	//		if(bufi>0) continue;
	//	}
	//	break;
	//}

	//if(ls){ mscr.Add((unsigned char*)ls.oneline(), ls.size()); } // print("'", VString((unsigned char*)ls.oneline(), ls.size()), "'\r\n");
	//else
	if(!isin && iquest.is()){
		if(!usemh) mscr.Add(iquest);
		else mh.Add(iquest);

	} // if(link.iquest) print("'", link.iquest, "'\r\n"); 
	//if(!iquest) nolineechobuf(0);

	}

	if(!usemh) print("\r\nMSCR: ", mscr.GetHash(), "\r\n");
	else{ mh.Finish(); print("\r\nMD5: ", mh.GetH(), "\r\n"); }

	return 1;
}