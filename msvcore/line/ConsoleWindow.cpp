class GlobalConsole;
class ConsoleWindowP;
class CWGlobal;
class TermoListen;
//GLock consolelock;

#define CNACT	1
#define CNVIS	2
#define CNVIONE	4

#define CONSOLEWND_ISDEL	1
#define CONSOLEWND_ISACT	2
#define CONSOLEWND_ISVIS	4

#define GFUNCSDEF(func, def) inline bool func()const{ return (stat&def)>0; } \
inline bool func(bool v){ if(v) stat|=def; else stat&=~def; return v; }
#define GFUNCSDEFT(func, stat, def) inline bool func()const{ return (stat&def)>0; } \
inline bool func(bool v){ if(v) stat|=def; else stat&=~def; return v; }


//bool ifrecv(SOCKET sock, unsigned int wt=0);
contype ConsoleRead(unsigned int &linuxread, unsigned int &linuxread_esc);

unsigned short brel[10]={0x2554, 0x2550, 0x2557, 0x2551, 0x2550, 0x2551, 0x255A, 0x2550, 0x255D, 0x25A0 }; // border element
// 0 1 2
// 3 4 5
// 6 7 8. 9 - border
//brel[0]=; brel[1]=; brel[2]=; brel[3]=; brel[4]=; brel[5]=; brel[6]=; brel[7]=; brel[8]=; brel[9]=;

class ConsoleWindow: public MSVCOT, public GLock{ //public: // protected:
// positions
protected:
//// data
//int selx, sely, seltx, selty; //select block
//public:
GlobalConsole*gcon;
ConsoleWindowP*gwnd;
friend class GlobalConsole;
friend class ConsoleWindowP;
friend class CWFileSystemD;

// functins
public:
	MRect rect; //int px, py, tx, ty; // positions x, y, x2, y2, c
	//bool cf_rn; // use \r\n else all \r or \n = \r\n.


	ConsoleWindow(){
		//memset(&gcon, 0, (char*)&rect-(char*)&gcon+sizeof(MRect));
		gcon=0; gwnd=0;

	}
	~ConsoleWindow(){ StopCot(); }

// gcon
	void Conn(GlobalConsole*gc, ConsoleWindowP*gw){ gcon=gc; gwnd=gw; }

// virtual
	virtual DWORD COT(LPVOID lp){// Clear('i', "ff0000", ""); Drawn("Testing...\r\n"); int testtime=1000;
		return 0;
		while(1){ if(!cotuse) return 0;
			Sleep(100);
		}
		return 1;
	}

	virtual void ReDraw(){ return ; }
	//virtual void DrawIt(){  }

// On
	virtual void OnSize(int sx, int sy){ rect.Set(0, 0, sx, sy); }
	virtual void OnCreate(){  }

// Act
	virtual int Read(contype ct);

// Set
	void SetHead(VString h);
	void SetCurPos(COORD cd);

// Draw
	void Draw(int x, int y, unsigned short c, unsigned short col=65534, unsigned short bg=65534);
	void Draw(int x, int y, const VString line, unsigned short col=65534, unsigned short bg=65534);
	void Draw(int x, int y, DrawDLine &dl);
	void Draw(int x, int y, DrawBLine &dl);
	
	//virtual void Draw(int x, int y, unsigned short c, int sz, unsigned short col=65534, unsigned short bg=65534);
	void Drawn(const VString line, unsigned short col=65534, unsigned short bg=65534);
	void DrawnU(const VString line, unsigned short col=65534, unsigned short bg=65534);
	void Drawn(unsigned short *ca, int cs, unsigned short col=65534, unsigned short bg=65534);
		

	//void Drawn(unsigned char c, unsigned short col=65534, unsigned short bg=65534){
	//return Drawn(stosr(c), col, bg); }


	void Drawn(char c, unsigned short col=65534, unsigned short bg=65534){
	unsigned short s=ctos(c); return Drawn(&s, 1, col, bg); }

	//void Drawn(unsigned char *c, unsigned short col=65534, unsigned short bg=65534){
	//for(int i=0; i<strlen((char*)c); i++) Drawn(stosr(c[i]), col, bg); return ; }
	/*
	void Drawn(char *c, unsigned short col=65534, unsigned short bg=65534){ if(!c) return ;
	for(int i=0; i<strlen((char*)c); i++) Drawn(stosr(c[i]), col, bg); return ; }

	void Drawn(const VString vs, unsigned short col=65534, unsigned short bg=65534){
	char*c=vs; int sz=vs; if(!c) return ;
	for(int i=0; i<sz; i++) Drawn(stosr(c[i]), col, bg); return ; }
	*/
	//void Drawn(const MString &vs, unsigned short col=65534, unsigned short bg=65534){
	//char*c=vs; int sz=vs; if(!c) return ;
	//for(int i=0; i<sz; i++) Drawn(stosr(c[i]), col, bg); return ; }

	bool Draw_s(int x, int y, unsigned short c, unsigned short col, unsigned short bg){
	//	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return 0;
	//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
	//	UGLock(*this); int r=0;
	////	if((char*)&vbuf[x+(y+bufext)*sx]<_buf.rchar() || (char*)&vbuf[x+(y+bufext)*sx]>=_buf.end())
	////		globalerror("MEM!!!\r\n");
	////	if((char*)&cbuf[x+(y+bufext)*sx]<_buf.rchar() || (char*)&cbuf[x+(y+bufext)*sx]>=_buf.end())
	////		globalerror("MEM!!!\r\n");
	////	if((char*)&bbuf[x+(y+bufext)*sx]<_buf.rchar() || (char*)&bbuf[x+(y+bufext)*sx]>=_buf.end())
	////		globalerror("MEM!!!\r\n");
	//	//return 4+2+1;
	//	if(vbuf[x+(y+bufup)*sx]!=c){ vbuf[x+(y+bufup)*sx]=c; r+=1; }
	//	if(cbuf[x+(y+bufup)*sx]!=col){ cbuf[x+(y+bufup)*sx]=col; r+=2; } 
	//	if(bbuf[x+(y+bufup)*sx]!=bg){ bbuf[x+(y+bufup)*sx]=bg; r+=4; } 
	//	//	gcon->Draw(this, x, y, c, col, bg);

//		return r;
		return 1;
	}

	void DrawAsWindow();

//// Global Edit
//	bool GWGESet(VString text);
//	MString WGGeGet();
//	MString WGGeGetSafe();
//	bool WGGeIs();

};


/////////////////////////////////////////////////////////////////////////////////////
/////// Console Window Edit ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class CWEdit : public ConsoleWindow{
public:
// data buf <title> a <text> e. key
	MString _buf; unsigned short *buf, *abuf, *ebuf, *end, *key;
	bool ispass; int bufsz, ln;
	MString charset;
//friend
	friend class CWGlobal;
	friend class ConsoleWindow;

	CWEdit(){ buf=0; abuf=0; ebuf=0; key=0; end=0; NewBuf(S1K); ispass=0; ln=0; }
	

	void NewBuf(int sz){
		MString bf; bf-=_buf;
		_buf.Reserv(sz, 0); memcpy(_buf, bf, _buf.size()<bf.size() ? _buf.size(): bf.size());
		buf=(unsigned short*)_buf.rchar();
		key=buf+((unsigned short*)bf.rchar()-key);
		ebuf=buf+((unsigned short*)bf.rchar()-ebuf);
		abuf=buf+((unsigned short*)bf.rchar()-abuf);
		end=(unsigned short*)_buf.end();
		bufsz=sz/2;
		Sleep(100);
	return ;
	}

	void SetTitle(VString line){
		int tsz=ebuf-abuf, sz=line.size();//strlen(ttl)*2;
		if(_buf.size()/2<tsz+sz) NewBuf(_buf.size()+S1K<sz ? S1K : sz+S1K-sz%S1K);
		if(_buf.size()/2<tsz+sz) return ;
		memcpy(buf+sz, abuf, tsz*2);
		for(int i=0; i<sz; i++){ buf[i]=ctos(line[i]); }
		key-=int(abuf-buf)-line.size(); ebuf-=int(abuf-buf)-line.size(); abuf-=int(abuf-buf)-line.size(); 
		//memcpy(buf, ttl, sz);
		ReDraw();
	}

	void SetCharset(VString val){
		charset=val;
	}

	void Set(VString val){
		MString set;
		if(charset.compareu("short")){ }
		else if(charset.compareu("utf")){
			set=utftos(val); val=set;            
		}
		else{
			set=ctos(val, val); val=set;
		}

		if(bufsz<abuf-buf+set.size()/2){ NewBuf(abuf-buf+set.size()/2); }

		memcpy(abuf, set, set.size()); ebuf+=set.size()/2; ReDraw();
	}

	int movekey();
	void ReDraw(){ int sx=rect.GetW(), sy=rect.GetH();
		if(movekey()) return ;
		//if(sx>0){ while((key-buf)/sx>=ln+2) ln++; while((key-buf)/sx<ln) ln--; }

		int i, k;// if(ln<0 || ln>S1M) return ;
		for(k=0; k<sy; k++){
			for(i=0; i<sx; i++){
				if(ebuf-buf>(k+ln)*sx+i)
				Draw(/*rect.left+*/i,/* rect.top+*/k, (!ispass || &buf[(k+ln)*sx+i]<abuf) ? buf[(k+ln)*sx+i] : '*');
				else Draw(/*rect.left+*/i, /*rect.top+*/k, ' ');
			}
		} movekey();
	}

#define CWCEDITOP_INS	1
#define CWCEDITOP_DEL	2
#define CWCEDITOP_CLEAR	3

	void DrawKey(unsigned short* fr, unsigned short* to){
		int sx=rect.GetW(), sy=rect.GetH();

		if(fr==to){
			int x=fr-buf-ln*sx, y=0; while(x>=sx && sx){ x-=sx; y++; }
			Draw(/*rect.left+*/x, /*rect.top+*/y, (!ispass || &buf[(y+ln)*sx+x]<abuf) ? buf[(y+ln)*sx+x] : '*');
		}else{
			for(fr; fr<to; fr++){
				int x=fr-buf-ln*sx, y=0; while(x>=sx && sx){ x-=sx; y++; }
				Draw(/*rect.left+*/x, /*rect.top+*/y, (!ispass || &buf[(y+ln)*sx+x]<abuf) ? buf[(y+ln)*sx+x] : '*');			
			}
		}
	}

	void DrawKey(unsigned short* pos, int op){ int sx=rect.GetW(), sy=rect.GetH();
		if(movekey()) return ;
		//if(sx>0){ while((key-buf)/sx>=ln+2) ln++; while((key-buf)/sx<ln) ln--; }

		int x=pos-buf-ln*sx, y=0; while(x>=sx && sx){ x-=sx; y++; }

		switch(op){
			case CWCEDITOP_INS:{
				if(pos+1>=ebuf){ DrawKey(pos, pos);
					//Draw(rect.left+x, rect.top+y, (!ispass || &buf[(y+ln)*sx+x]<abuf) ? buf[(y+ln)*sx+x] : '*');
				}
				
				DrawKey(pos, ebuf); movekey(); return;

				//int tx=ebuf-buf-ln*sx, ty=0; while(tx>sx && sx){ tx-=sx; ty++; }
				//if(ty>=sy){ ty=sy-1; tx=sx; }

				//int iy, ix=x, ttx;
				//for(iy=y; iy<=y; iy++){ ttx= iy<y ? sx : tx;
				//	for(ix; ix<ttx; ix++){
				//		Draw(rect.left+ix, rect.top+iy, &buf[(iy+ln)*sx+ix]<end ? buf[(iy+ln)*sx+ix] : ' ');
				//	} ix=0;
				//} movekey();
				break;}
			case CWCEDITOP_DEL:{
				if(pos>=ebuf){ DrawKey(pos, pos); } //Draw(rect.left+x, rect.top+y, ' '); return ; }
				DrawKey(pos, ebuf+1); movekey(); return;

				//int tx=ebuf+1-buf-ln*sx, ty=0; while(tx>sx && sx){ tx-=sx; ty++; }
				//if(ty>=sy){ ty=sy-1; tx=sx; }

				//int iy, ix=x, ttx;
				//for(iy=y; iy<=y; iy++){ ttx= iy<y ? sx : tx;
				//	for(ix; ix<ttx; ix++){
				//		Draw(rect.left+ix, rect.top+iy, &buf[(iy+ln)*sx+ix]<end ? buf[(iy+ln)*sx+ix] : ' ');
				//	} ix=0;
				//} movekey();

				break;}
			case CWCEDITOP_CLEAR:{
				DrawKey(abuf, pos); movekey(); return;

				/*int iy, ix, tx;
				for(iy=0; iy<=y; iy++){ tx= iy<y ? sx : x;
					for(ix=0; ix<tx; ix++){
						Draw(rect.left+ix, rect.top+iy, &buf[(iy+ln)*sx+ix]<end ? buf[(iy+ln)*sx+ix] : ' ');
					}
				}
				movekey();*/
				break;}
		}
		

		// if(ln<0 || ln>S1M) return ;
		/*for(k=0; k<sy; k++){
			for(i=0; i<sx; i++){
				if(rbuf-buf>(k+ln)*sx+i)
				Draw(rect.left+i, rect.top+k, (!ispass || &buf[(k+ln)*sx+i]<tbuf) ? buf[(k+ln)*sx+i] : '*');
				else Draw(rect.left+i, rect.top+k, ' ');
			}
		}*/
	}

	void SetRect(int x, int y, int sx, int sy){ rect.Set(x, y, sx, sy); }

MString GetTextS(){ MString ret; ret=VString((char*)abuf, (ebuf-abuf)*2); ebuf=abuf; key=abuf; ReDraw(); return ret; }
MString GetTextU(){ MString ret; ret= stoutf(VString((char*)abuf, (ebuf-abuf)*2)); ebuf=abuf; key=abuf; ReDraw(); return ret; }
MString GetText(){ MString ret; ret=stoc(abuf, (ebuf-abuf)); ebuf=abuf; key=abuf; ReDraw(); return ret; }

MString GetTextSafeS(){ return VString((char*)abuf, (ebuf-abuf)*2); }
MString GetTextSafeU(){ return stoutf(VString((char*)abuf, (ebuf-abuf)*2)); }
MString GetTextSafe(){ return stoc(abuf, (ebuf-abuf)); }

bool GetIs(){ return ebuf-abuf; }

int ReadLine(char*c, int sz){ contype ct; for(int i=0; i<sz; i++){ ct.SetKey(1, 0, c[i]); Read(ct); } return 1; }

virtual int Read(contype ct){
		if(ConsoleWindow::Read(ct)) return 1;
		unsigned int c;
//
		if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ c=ct.getxcode();// print(itos(c)); print(".");
	if(!(ct.getkeys()&CN_KEY)){

	if(c==MK_TAB) return 1;

	if(c==MK_BACKSPASE || c==127) {
		if(key>abuf){// && key<=rbuf){
			memcpy(key-1, key, (ebuf-key)*2); key--; ebuf--; *ebuf=32;// if(key==rbuf) *key=0;
			//if(key!=rbuf) {ReDraw();} else { print(32); print(8);}
			DrawKey(key, CWCEDITOP_DEL); //ReDraw();
		}
	} else if(c==MK_ESCAPE){
		if(ebuf>abuf){
			key=abuf;
			memset(abuf, 0, (ebuf-abuf)*2); // readinput_normal(buf, key, rbuf, ispass);
			DrawKey(ebuf, CWCEDITOP_CLEAR); ebuf=abuf;
		}
	} else if(c==13 || c==10){ return 1; //break; //		print("\r\n");
	} else {
		if(ebuf-abuf>=_buf.size()/2) NewBuf(_buf.size()+S1K); //break;
		if(key!=ebuf){
			memcpy(key+1, key, (ebuf-key)*2);// ReDraw(); //readinput_normal(buf, key, rbuf+1, ispass);			
		} *(key)=c; ebuf++; key++; DrawKey(key-1, CWCEDITOP_INS); //if(ispass) print("*"); else print(c);
		//cinfo(cursz, 1);
	}
	} else {
		if(c==MK_DELETE){ if(key>=buf && key<ebuf) {
            memcpy(key, key+1, (ebuf-key-1)*2); ebuf--; *ebuf=32;
			DrawKey(key, CWCEDITOP_DEL);
			//ReDraw(); //readinput_normal(buf, key, rbuf, ispass);
		}}
		else if(c==MK_LEFT) {if(key>abuf) {
			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
			unsigned short *k=key; rrts_con(abuf, ebuf, key); movekey();
			} else{ key--; movekey(); }
		}}
		else if(c==MK_RIGHT){if(key<ebuf){
			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
			unsigned short *k=key; rts_con(buf, ebuf-buf, key); movekey();
			} else{ key++; movekey(); }
		}}
		else if(c==MK_UP){  }
		else if(c==MK_DOWN){  }
		else if(c==MK_END){  key=ebuf; movekey();  }
		else if(c==MK_HOME){ key=abuf; movekey(); }
		}
	}//}  return rbuf-buf;
//
return 1;
}


};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class CWGSelectWindow : public ConsoleWindow{
//friend
	friend class CWGlobal;

public:
	CWGSelectWindow(){ }


	//virtual int Pos(short _x, short _y, short _cx, short _cy){ ConsoleWindow::Pos(_x,_y,_cx,_cy); return 1; }


	//virtual void DrawIt();// DrawAsWindow();// VString d;
	//int as=sx, p=sx/2;
	//for(int i=0; i<s; i++){ Draw(i+p, 0, folder[i]); }

	//for(int i=0; i<sy-2; i++){
	//	if(dir.size()>i+pos) d=dir[i].key; else d.Empty(); int k=0, tk=d.size(); if(tk>sx-2) tk=sx-2;
	//	for(k=0; k<tk; k++){ Draw(k+1, i+1, d[k], defc, (i+pos!=cur || !(isact&CNACT)) ? defbg : defs ); } for(k; k<sx-2; k++){ Draw(k+1, i+1, ' '); }
//	}
//}

virtual int Read(contype ct){ // if 0 - next, if 1 - ok & end;
	if(ConsoleWindow::Read(ct)) return 1;
	if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ bool di=0;
		if(ct.getkeys()&CN_KEY){
		switch(ct.getcode()){
			case MK_UP:  di=1; break;
			//case MK_DOWN: if(cur<dir.size()-1) cur++; di=1; break;
		}}
	//	if(ct.getcode()==MK_ENTER) { RunIt(ct.getkeys()&CN_SHIFT); }

		if(di) {ReDraw(); return 1;}
	}
return 0;
}


};

/////////////////////////////////////////////////////////////////////////////////////
/////// Console Window Global ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class CWGlobal: public ConsoleWindow{ public:
//MString _buf; unsigned short *buf, *key;
CWEdit cne; CWGSelectWindow cns;

CWGlobal(){
	cne.gcon=gcon; cns.gcon=gcon;
	COORD cd=TTYSize();
	if(cd.X && cd.Y){
	//	MString s=itos(cd.X)+"x"+itos(cd.Y);
	//	write(0, s, s); exit(0);
		OnSize(cd.X, cd.Y);}// printf("!!!!!!!!!!!!!"); Sleep(6000);}
}//_buf.Reserv(S1K); buf=(unsigned short*)_buf.rchar(); key=buf; }//dcol=MRGB("00aa00").sh(); dbg=MRGB("00ff00").sh(); }

	void OnSize(int sx, int sy){// int r=sx*sy;
		//ConsoleWindow::OnSize(sx, sy);
		ReDraw();
		//if(!r) LoadBanner(); 
	}

/*	void NewBuf(int sz){
		MString bf; bf-=_buf;
		_buf.Reserv(sz, 0); memcpy(_buf, bf, _buf.size()<bf.size() ? _buf.size(): bf.size()); buf=(unsigned short*)_buf.rchar(); key=buf+(unsigned short*)bf.rchar()-key; Sleep(100); 
		return ;
	}*/

	void ReDraw(){ if(!rect.GetW() || !rect.GetH()) return ;
		// cne.OnPos(rect.left, rect.bottom-2, rect.right, rect.bottom); cne.gcon=gcon;
	}
/*	// Draw Menu =¶-„„¨¨¨LLL---¶¶¶¶¶¶TTT¶¶¶+++
	Draw(0,0, brel[0]); Draw(sx-1, 0, brel[2]);
    	for(int i=1; i<sx-1; i++) Draw(i, 0, brel[1]);
		for(int i=0; i<sx; i++) Draw(i, sy-1, brel[7]);

		for(int i=1; i<sy-2; i++) {Draw(0, i, brel[3]); Draw(sx-1, i, brel[5]); }
	
	// Menu 
		char o[]="_F1-F4_  _F5_ _F6_", t[]="_F7-F9 _F10 _F11 _F12"; unsigned short cl=MRGB("00aa00").sh();
		//Draw(0, 2, 0x25A0, cl); Draw(0, 2+strlen(o), 0x25A0, cl);
		for(i=0; i<strlen(o); i++){
			if(o[i]=='_') Draw(0, 1+i, brel[9], cl);
			else Draw(0, 1+i, o[i], cl);
		}

		for(i=0; i<strlen(t); i++){
			if(t[i]=='_') Draw(sx-1, 1+i, brel[9], cl);
			else Draw(sx-1, 1+i, t[i], cl);
		}
	*/

	virtual int Read(contype ct){
		if(ConsoleWindow::Read(ct)) return 1;
		//CONSOLE_SCREEN_BUFFER_INFO bi;
		unsigned short c;
		cne.Read(ct);
//
		if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ c=ct.getcode();// print(itos(c)); print(".");
/*	if(!(ct.getkeys()&CN_KEY)){


	if(c==MK_BACKSPASE || c==127) { if(key>buf){// && key<=rbuf){
			memcpy(key-1, key, (rbuf-key)*2); print(8); key--; rbuf--;
			if(key!=rbuf) {readinput_normal(buf, key, rbuf, ispass);} else { print(32); print(8);}}
	} else if(c==MK_ESCAPE) { if(rbuf>buf){ _movekey(buf-key, bi); key=buf;
		memset(buf, 0, (rbuf-buf)*2); readinput_normal(buf, key, rbuf, ispass);
		rbuf=buf;}
	} else if(c==13 || c==10){
		print("\r\n"); return 1; //break;
	} else {
		if(rbuf-buf>=_buf) return 1;//break;
		if(key!=rbuf) {
			memcpy(key+1, key, (rbuf-key)*2); readinput_normal(buf, key, rbuf+1, ispass);			
		} *(key)=c; rbuf++; key++; if(ispass) print("*"); else print(c);
		cinfo(cursz, 1); }
	} else {
		if(c==MK_DELETE) { if(key>=buf && key<rbuf) {
            memcpy(key, key+1, (rbuf-key-1)*2); rbuf--;
			readinput_normal(buf, key, rbuf, ispass);
		}}
		else if(c==MK_LEFT) {if(key>buf) {
			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
			unsigned short *k=key; rrts_con(buf, rbuf-buf, key); _movekey(key-k, bi);
			} else {_movekey(-1, bi); key--;}
		}}
		else if(c==MK_RIGHT) {if(key<rbuf){
			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
			unsigned short *k=key; rts_con(buf, rbuf-buf, key); _movekey(key-k, bi);
			} else {_movekey(1, bi); key++;}
		}}
		else if(c==MK_UP) {  }
		else if(c==MK_DOWN) {  }
		else if(c==MK_END) { _movekey(rbuf-key, bi); key=rbuf;  }
		else if(c==MK_HOME) { _movekey(-(key-buf), bi); key=buf; }
		}*/
		}//}  return rbuf-buf;

//
return 1;
}


	void LoadBanner(){
		MString bn=LoadFile("/system/conf/hello_banner.conf"); Drawn(bn);
		//for(int i=0; i<bn; i++){ Drawn( }
	}

DWORD COT(LPVOID lp){ReDraw();
	while(1){ if(!cottid) return 0;
	Sleep(100);

	
	}return 1;
}


};

///////////////////////////////////////////////////////////////////////////////////////
/////// Global Consile Matrix /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////// Global Consile Data /////////////////////////////////////////////////////////
//class ConsoleWindowP{ public: ConsoleWindowP *p, *n; ConsoleWindow *w; 
//inline ConsoleWindow* operator->(){ return w; }
//};

//// New Matrix : Object Matrix
class ConsoleWindowP : public GLock{ public: // !CLONE OMATRIX!
// void*
	ConsoleWindowP *_p, *_n; // +data
	ConsoleWindow *wnd;
	GlobalConsole*gcon;
// positions
	MRect rect;
// cursor
	int cx, cy;
// data
	unsigned short *vbuf, *cbuf, *bbuf; mbool upbuf; // data buffer & colors & background & update
	unsigned short defc, defbg, defs, bufup;		// def color, def bg, def string, buffer up lines.
	MString _buf, head; int curpos;
	MRect cedit;
// stat
	int stat;//, isact;

// con
	ConsoleWindowP(ConsoleWindow*w, GlobalConsole *gc){
		memset(&cx, 0, sizeof(*this)-((char*)&cx-(char*)this));
		wnd=w;// memcpy(&gcon+sizeof(gcon), &w->rect, sizeof(rect));
		gcon=gc; w->gcon=gc; w->gwnd=this;
		head="Default Console Window";
		defc=MRGB("ffffff").sh(); defbg=MRGB("000000").sh(); defs=MRGB("00ff00").sh();
	}
	~ConsoleWindowP(){
		_buf.Clean(); head.Clean(); 
	}

// stat
	GFUNCSDEF(Del, CONSOLEWND_ISDEL);
//	GFUNCSDEF(Act, CONSOLEWND_ISACT);
	bool Act()const{ return (stat&CONSOLEWND_ISACT)>0; } \
	bool Act(bool v);
	GFUNCSDEF(Visible, CONSOLEWND_ISVIS);

// set
	void SetHead(VString h);
	void SetCurPos(COORD cd){ int sx=rect.GetW(), sy=rect.GetH(); curpos=cd.X+cd.Y*sx; }
	

// on
	virtual int OnPos(short _x, short _y, short _cx, short _cy){
		if(rect.left==_x && rect.right==_cx && rect.top==_y && rect.bottom==_cy) return 1;
		rect.left=_x; rect.right=_cx; rect.top=_y; rect.bottom=_cy;
			OnSize(rect.GetW(), rect.GetH()); return 1;
	}

	virtual int OnSize(int sx, int sy);


// draw
	void ReDraw(){ return wnd->ReDraw(); }
	void Draw(ConsoleWindow *wnd, int x, int y, unsigned short c, unsigned short col=65534, unsigned short bg=65534);
	void Draw(ConsoleWindow *wnd, int x, int y, const VString line, unsigned short col=65534, unsigned short bg=65534);
	void Draw(ConsoleWindow *wnd, int x, int y, DrawDLine &dl);
	void Draw(ConsoleWindow *wnd, int x, int y, DrawBLine &dl);

	void Drawn(ConsoleWindow *wnd, unsigned short *ca, int cs, unsigned short col=65534, unsigned short bg=65534);
	void Drawn(ConsoleWindow *wnd, const VString line, unsigned short col=65534, unsigned short bg=65534);
	void DrawnU(ConsoleWindow *wnd, const VString line, unsigned short col=65534, unsigned short bg=65534);
// cedit
//	void UseCedit(MRect mr){ cedit=mr; }
//	void DelCedit(){ cedit.Null(); }

};


class ConsoleWindowPA{ public:
	// void*
	ConsoleWindowP *_p, *_n;

// IF _p as _a && _n as _e;
bool Add(ConsoleWindowP*el){
	if(!_p) { _p=el; _n=el; el->_p=0; el->_n=0; return 1;}
	_n->_n=el; el->_p=_n; el->_n=0; _n=el; return 1;
}

bool Del(ConsoleWindowP*el){
	if(el->_n) el->_n->_p=el->_p; else if(el!=_n) globalerror("ConsoleWindowP _N"); else _n=el->_p; // if(el==_n)
	if(el->_p) el->_p->_n=el->_n; else if(el!=_p) globalerror("ConsoleWindowP _P"); else _p=el->_n; // if(el==_p)
return 1;
}

bool isfirst(void*el){ ConsoleWindowP *i=_p; while(i){ if(i==el) return 1; i=i->_n; } return 0; }

};

///////////////////////////////////////////////////////////////////////////////////////
/////// Global Consile ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
class GlobalConsole: public MSVCOT{
friend class VirtualConsole;

private:
// positions
	MRect rect;		// windows size
// data
	unsigned short *vbuf, *cbuf, *bbuf; mbool upbuf; // data buffer & colors & background & update
	unsigned short defc, defbg, defs, bufup;		 // def color, def bg, def string, buffer up lines.
	MString _buf, head;
// windows
	ConsoleWindowPA wnds; int wndsz;
	int doit;
// global
public:
	CWGlobal gw;
	CWEdit cedit;
protected:
	int drawglobtime;
	ConsoleWindowP *actp;
	//ConsoleWindowP *a, *e;
//delete it
	int inconsole, outconsole;
// in & out
	VirtualConsole *tlin, *tlout;
#ifdef WIN32
#define CNIRSZ	4
	_INPUT_RECORD ir[CNIRSZ];
#else
	unsigned int lttytm;
#endif
// input
	int inx, iny, incol, inbgcol;
	unsigned int linuxread, linuxread_esc; // for linux

public:
// constructor
	GlobalConsole(){
		//memcpy(this, 0, sizeof(*this));
		rect.Clean(); vbuf=0; cbuf=0; bbuf=0; linuxread=0; linuxread_esc=0;
		defc=MRGB("ffffff").sh(); defbg=MRGB("000000").sh(); defs=MRGB("00ff00").sh();
		wndsz=0; actp=0; wnds._p=0; wnds._n=0;
		bufup=200; drawglobtime=1;
		inx=0; iny=0;
		tlin=0; tlout=0; inconsole=0; outconsole=0;
#ifdef WIN32
		CONSOLE_SCREEN_BUFFER_INFO info;
		if(GetConsoleScreenBufferInfo(_std_output_handle, &info)){
			rect.Set(0, 0, info.srWindow.Right-info.srWindow.Left+1, info.srWindow.Bottom-info.srWindow.Top+1);
			if(rect.GetW()==80 && rect.GetH()==25 && 0){
				//rect.Set(0, 0, 100, 45);
				//OnSize(100, 45);
			} else { /*rect.top++;*/ OnSize(rect.GetW(), rect.GetH()-1); }
			//rect.Set(0, 0, info.dwSize.X, info.dwSize.Y);
			//COORD cd; cd.X=rect.GetW(); cd.Y=rect.GetH();
			//SetConsoleScreenBufferSize(_std_output_handle, cd);
			//SMALL_RECT sr={rect.left, rect.top, rect.right-1, rect.bottom-1};
			//SetConsoleWindowInfo(_std_output_handle, 1, &sr);			
		} else{
			//rect.Set(0, 0, 80, 25);
			OnSize(rect.GetW(), rect.GetH());
		}
#else
		COORD cd=TTYSize();
		OnSize(cd.X, cd.Y);
		//rect.Set(0, 0, cd.X, cd.Y);
#endif		
		cedit.gcon=this;
	}

	~GlobalConsole(){  }

// New & Del
	bool NewWnd(ConsoleWindow *wind, int isdel=0, int repos=1){
		UGLOCK(consolelock);
		ConsoleWindowP*p=new ConsoleWindowP(wind, this); if(!p) return 0;
        wnds.Add(p);// p->w=wind;
		//wind->gcon=this; p->gcon=this;
		p->Del(isdel);
//		wind->isact=CNVIS;
		wndsz++; __l.UnLock();
		if(repos) RePos();
		wind->OnCreate();
		return 0;
	}

	bool DelWnd(ConsoleWindow *wind){
		UGLOCK(consolelock);
		ConsoleWindowP *p=wnds._p;
		while(p->wnd==wind){
			wnds.Del(p);
			wndsz--;
			if(p->Del()) delete wind;
			delete p;
			return 1;
		}
		return 0;
	}

// Cot
	DWORD COT(LPVOID lp){
		while(1){ if(!cotuse) return 0;
			Sleep(100);

#ifndef WIN32
			if(lttytm/30!=time()/30){
				COORD cd=TTYSize(); lttytm=time(); if(cd.X!=rect.GetW() || cd.Y!=rect.GetH()){ OnSize(cd.X, cd.Y); }
			}
#endif
		}
		return 1;
	}

// In / Out
	int TLOut(){ return tlout!=0; }

// On
	virtual int OnPos(short _x, short _y, short _cx, short _cy){
		if(rect.left==_x && rect.right==_cx && rect.top==_y && rect.bottom==_cy) return 1;
		rect.left=_x; rect.right=_cx; rect.top=_y; rect.bottom=_cy;
			OnSize(rect.GetW(), rect.GetH()); return 1;
	}

	virtual int OnSize(int sx, int sy){ if(sx*sy>S1M) return 0;
		if(rect.GetW()==sx && rect.GetH()==sy) return 1;
	//	if(rect.left==0 || rect.right==cx || rect.top==0 || rect.bottom==cy) return 1;
		UGLOCK(consolelock);
		rect.right=rect.left+sx; rect.bottom=rect.top+sy;
		vbuf=0; cbuf=0; bbuf=0;

		_buf.Reserv(sx*(sy+bufup)*2*3+sx*(sy+bufup)/8+1, 0); // sizeof(short), 3 buffers, mbool
		vbuf=(unsigned short*)_buf.pos(0);
		cbuf=(unsigned short*)_buf.pos(sx*(sy+bufup)*2);
		bbuf=(unsigned short*)_buf.pos(sx*(sy+bufup)*2*2);
		upbuf=_buf.pos(sx*(sy+bufup)*2*3);

		//for(int i=0; i<rect.GetH(); i++){
		//	print("\r\n");
		//}

		ConsoleWindow cw; ItosUX<128> ut; ut.Reserv(sx, ' ');
		ConsoleWindowP cwp(&cw, this); unsigned short s=' '; cw.rect=cwp.rect=rect;
		cwp.Act(1);
		Draw(&cwp, 0, sy-1, (unsigned short*)&s, 1, 0, 0);
		for(int i=0; i<rect.GetH(); i++){
			Draw(&cwp, 0, i, ut.ret, ut.ret, 0, 0);
		}

		RePos();
		return 1;
	}
		
// Re
	void RePos(){ //gw.Pos(0, 0, rect.X, rect.Y);
		if(!wndsz) return ;
		ActItLast();

		if(wndsz==1){
			wnds._p->OnPos(rect.left, rect.top, rect.right, rect.bottom);
		}
		if(wndsz==2){
		//	wnds._p->_n->OnPos(rect.left, rect.top, rect.right, rect.bottom-2);
		//}
		//else if(wndsz==3){
			wnds._p->OnPos(rect.left, rect.top, rect.right/2, rect.bottom);
			wnds._p->_n->OnPos(rect.right/2, rect.top, rect.right, rect.bottom);
		}
		//if(wndsz>2){ wnd[sz-1]->Pos(0, 0, rect.X, rect.Y-2); wnd[sz-1]->DrawFull(); }
	}
	/////////////////////////////////////////////////////////////////////////////////////
	void RePosTwoWnd(){ //gw.Pos(0, 0, rect.X, rect.Y);
		//wnd[0]->Pos(0, 0, rect.X, rect.Y);
		//if(sz==1){  }
		//else if(sz==2){
		//	wnd[0]->Pos(0, 0, rect.X/2, rect.Y-2); wnd[1]->Pos(rect.X/2, 0, rect.X, rect.Y-2);
		////	wnd[1]->Pos(0, 0, rect.X, rect.Y-2);
		//}
		//else if(sz==3){
		//	wnd[1]->Pos(0, 0, rect.X/2, rect.Y-2); wnd[2]->Pos(rect.X/2, 0, rect.X, rect.Y-2);
		//}
		//ActIt(sz-1); 
		////if(sz>2) { wnd[sz-1]->Pos(0, 0, rect.X, rect.Y-2); wnd[sz-1]->DrawFull(); }
	}
// Action
	void ActIt(ConsoleWindow *wind){ if(!wndsz) return ;
	//if(actp) actp->Act(0);
	//if(actp && actp->_n){ actp=actp->_n; actp->Act(1); }
	//else actp=wnds._p;
		ConsoleWindowP*p=wnds._p;
		while(p){
			if(p->wnd==wind){ p->Act(1); }
			else if(p->Act()) p->Act(0);
			p=p->_n;
		}
	}

	void ActItNext(){ if(!wndsz) return ;
		ConsoleWindowP*p=wnds._p;
		while(p){
			if(p->Act()){
				p->Act(0);
				if(p->_n) p->_n->Act(1);
				else wnds._n->Act(1);
				return ;
			}
			p=p->_n;
		}
		wnds._n->Act(1);
	}

	void ActItLast(){
		return ActIt(wnds._n->wnd);
	}

	void ActItVis(){ if(!wndsz) return ;
		ConsoleWindowP*p=wnds._p; int a=0, ws=wndsz+1;
		while(ws){
			if(!p) p=wnds._p;
				if(!a) if(p->Act()){ a=1; p->Act(0); ws=wndsz+1; }
				else{
					if(p->Visible()){ p->Act(1); return ; }
				}
				p=p->_n;
			ws--;
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////////
	void Do(){ doit=1; contype ct; int i;
	if(!tlin)
		while(doit){
			ct=Read();
			InRead(ct);
		}
	else
		while(doit){
			Sleep(10);
		}
		return ;
	}

	///////////////////////////////////////////////////////////////////////////////////
	void DoStop(){ doit=0; }

	void InRead(contype ct){
//		if(tlin){ tlin->SendCT(ct); continue; }
		if(ct.gett()==CNTWND){ OnSize(ct.getx(), ct.gety()); return ; }
		if(!wndsz) return ;
		ConsoleWindowP*p=wnds._n, *pr=0;
		while(p){
			if(p->Act()){
//				if(p->cedit.Is()) if(cedit.Read(ct)){ pr=p; }
				p->wnd->Read(ct);
				//if(!pr && p->wnd->Read(ct)) pr=p;
				break;
			}
			p=p->_n;
		}
		//if(!pr){ wnds._n->wnd->Read(ct); }
		return ;
	}


/////////////////////////////////////////////////////////////////////////////////////
//	void DoO(contype ct){ int i;
//		for(i=0; i<sz; i++){
//				if(wnd[i]->isact&CNACT) if(wnd[i]->Read(ct)){ i=-1; break;} }
//			if(i!=-1) wnd[0]->Read(ct);
//		return ;
//	}

/////////////////////////////////////////////////////////////////////////////////////
	contype Read(){
		contype ct;

		return ConsoleRead(linuxread, linuxread_esc);
	/*
		if(readbufs>=2 && readbufs>=((contype*)readbuf)->getsz()){
			int s=((contype*)readbuf)->getsz();
			memcpy(&ct, readbuf, s); memcpy(readbuf, readbuf+s, readbufs-s);
			return ct;
		}*/
	/////////////////////////////////////////////////////////////////////////////////////
		if(!inconsole){
#ifdef WIN32
		DWORD rd; //	SHIFT_PRESSED
		/*if(nowaitcon){*/ int i=GetNumberOfConsoleInputEvents(_std_input_handle, &rd); if(i && !rd) return ct;// }
		ReadConsoleInputW(_std_input_handle, ir, 1+0*CNIRSZ, &rd);
		for(int i=0; i<rd; i++){
			switch(ir[i].EventType){
				case KEY_EVENT:
					if(!ir[i].Event.KeyEvent.uChar.UnicodeChar){ ir[i].Event.KeyEvent.uChar.UnicodeChar=ctos(ir[i].Event.KeyEvent.wVirtualKeyCode); ir[i].Event.KeyEvent.dwControlKeyState|=CN_KEY; }
						//ir[i].Event.KeyEvent.uChar.UnicodeChar
					ct.SetKey(ir[i].Event.KeyEvent.bKeyDown, ir[i].Event.KeyEvent.dwControlKeyState ,ir[i].Event.KeyEvent.uChar.UnicodeChar); break;
				case MOUSE_EVENT:
					ct.SetMouse(ir[i].Event.MouseEvent.dwControlKeyState, ir[i].Event.MouseEvent.dwEventFlags, ir[i].Event.MouseEvent.dwButtonState, ir[i].Event.MouseEvent.dwMousePosition); break;
				case WINDOW_BUFFER_SIZE_EVENT:
					ct.SetWindow(ir[i].Event.WindowBufferSizeEvent.dwSize); break;
				case MENU_EVENT:{
					int i=456;
					break;}
				case FOCUS_EVENT:
					ct.SetFocus(ir[i].Event.FocusEvent.bSetFocus); break;
			}
		}
		return ct;
//#else
#endif
//		#include <curses.h>
		int c;
		if(!ifrecv(0, 0)) return ct;

		if(!linuxread_esc && linuxread){ c=linuxread; linuxread=0; }
		else{ c=0; read(0, &c, 1); } // c=getc(stdin);
		if(c==10) c=13;

		if(linuxread_esc==3){
			if((linuxread&224)==192 && (c&192)==128){
				c=((linuxread&31)<<6)+(c&63);
				ct.SetKey(1, 0, c); linuxread=0; linuxread_esc=0;
			}
			else {
				ct.SetKey(1, 0, linuxread); linuxread=c;
				if((c&224)!=192) linuxread_esc=0;
			}
			return ct;
		}

		//if(linuxread>1<<24){ linuxread_esc=0; linuxread=0; }

		if(!linuxread_esc && c==27){
			if(!ifrecv(0, 0)){ ct.SetKey(1, 0, 27); return ct; }
			linuxread_esc=1; linuxread=0; return ct;
		}
		else if(linuxread_esc==1){
			if(c=='['){ linuxread_esc=2; return ct; }
			else{ ct.SetKey(1, 0, 27); linuxread_esc=0; linuxread=c; return ct; }
		}
		else
			if(linuxread_esc==2){
				linuxread=(linuxread<<8)+c;

				// virtualcode
				int ret=1;
				switch(linuxread){
					// level one
					// case '[': ct.SetKey(1, 0, 27); break;
	//					case 'O': ctrl=CN_RCTRL; break;

					// level two
					case 'A': ct.SetKey(1, CN_KEY, MK_UP); break;
					case 'B': ct.SetKey(1, CN_KEY, MK_DOWN); break;
					case 'C': ct.SetKey(1, CN_KEY, MK_RIGHT); break;
					case 'D': ct.SetKey(1, CN_KEY, MK_LEFT); break;

					// level three
					case ('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_HOME); break;
					case ('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_DELETE); break;
					case ('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_END); break;

					// level four
					case ('1'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F1); break;
					case ('1'<<16)+('2'<<8)+126: ct.SetKey(1, CN_KEY, MK_F2); break;
					case ('1'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F3); break;
					case ('1'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F4); break;
					case ('1'<<16)+('5'<<8)+126: ct.SetKey(1, CN_KEY, MK_F5); break;
					case ('1'<<16)+('7'<<8)+126: ct.SetKey(1, CN_KEY, MK_F6); break;
					case ('1'<<16)+('8'<<8)+126: ct.SetKey(1, CN_KEY, MK_F7); break;
					case ('1'<<16)+('9'<<8)+126: ct.SetKey(1, CN_KEY, MK_F8); break;
					case ('2'<<16)+('0'<<8)+126: ct.SetKey(1, CN_KEY, MK_F9); break;
					case ('2'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F10); break;
					case ('2'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F11); break;
					case ('2'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F12); break;
					default: ret=0; break;
				}
				if(ret){ linuxread=0; linuxread_esc=0; return ct; }
				//linuxread<<=8; linuxread=c;
				if(c==126){ linuxread=0; linuxread_esc=0; return ct; }
			 }

		if(!linuxread_esc){
			if((c&224)==192){ linuxread_esc=3; linuxread=c; }
			else ct.SetKey(1, 0, c);
		}
		else
			ct.SetKey(1, 0, c);
		
		return ct;


		//if(linuxread_esc && !linuxread && c!='['){ ct.SetKey(1, 0, 27); linuxread_esc=0; linuxread=c; return ct; }

	
		//if(linuxread_esc==2){
		//	if((linuxread&224)==192 && (c&192)==128){
		//		c=((linuxread&31)<<6)+(c&63);
		//		ct.SetKey(1, 0, c); linuxread=0; linuxread_esc=0;
		//	}
		//	else { ct.SetKey(1, 0, linuxread); linuxread=c; }
		//	return ct;
		//}

		//if(c==27) print("[c:", itos(c), "-", "^[", "]");
		//else print("[c:", itos(c), "-", VString((char*)&c, 1), "]");

		

		if(linuxread_esc){
			linuxread=linuxread<<8+c;
			print("[CODE:", itos(linuxread), "]"); Sleep(1000);

			// virtualcode
			int ret=1;
			switch(linuxread){
				// level one
				// case '[': ct.SetKey(1, 0, 27); break;
//					case 'O': ctrl=CN_RCTRL; break;

				// level two
				case ('['<<8)+'A': ct.SetKey(1, CN_KEY, MK_UP); print("[MK_UP]"); break;
				case ('['<<8)+'B': ct.SetKey(1, CN_KEY, MK_DOWN); break;
				case ('['<<8)+'C': ct.SetKey(1, CN_KEY, MK_RIGHT); break;
				case ('['<<8)+'D': ct.SetKey(1, CN_KEY, MK_LEFT); break;

				// level three
				case ('['<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_HOME); print("[MK_HOME]"); break;
				case ('['<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_DELETE); print("[MK_DELETE]"); break;
				case ('['<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_END); print("[MK_END]"); break;

				// level four
				case ('['<<24)+('1'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F1); print("[MK_F1]"); break;
				case ('['<<24)+('1'<<16)+('2'<<8)+126: ct.SetKey(1, CN_KEY, MK_F2); print("[MK_F2]"); break;
				case ('['<<24)+('1'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F3); print("[MK_F3]"); break;
				case ('['<<24)+('1'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F4); break;
				case ('['<<24)+('1'<<16)+('5'<<8)+126: ct.SetKey(1, CN_KEY, MK_F5); break;
				case ('['<<24)+('1'<<16)+('7'<<8)+126: ct.SetKey(1, CN_KEY, MK_F6); break;
				case ('['<<24)+('1'<<16)+('8'<<8)+126: ct.SetKey(1, CN_KEY, MK_F7); break;
				case ('['<<24)+('1'<<16)+('9'<<8)+126: ct.SetKey(1, CN_KEY, MK_F8); break;
				case ('['<<24)+('2'<<16)+('0'<<8)+126: ct.SetKey(1, CN_KEY, MK_F9); break;
				case ('['<<24)+('2'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F10); break;
				case ('['<<24)+('2'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F11); break;
				case ('['<<24)+('2'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F12); break;
				default: ret=0; break;
			}
			if(ret){ linuxread=0; linuxread_esc=0; return ct; }
			//linuxread<<=8; linuxread=c;
			if(c==126){ linuxread=0; return ct; }
		}

		

		//if((c&224)==192){ linuxread_esc=2; linuxread=c; }
		//else
			ct.SetKey(1, 0, c);

		return ct;


			
			if(linuxread && linuxread!=27 && (linuxread&72448)!=6912 && (linuxread&1792768)!=1792768
				&& (linuxread&224)!=192){
				ct.SetKey(1, 0, linuxread); linuxread=0; return ct;
			}

			//int c=getc(stdin);
			if(c==10) c=13; int t=0, ctrl=0;
			//print("   >",itos(c), "<   ");
			//ct.SetKey(1, 0, c);
			//if(!linuxread && c==127) c=46;
			
			if(linuxread)
			switch(linuxread){
			// one
				case 27: 
					if(c=='['){ linuxread<<=8; linuxread+=c; }
					else if(c=='O'){ linuxread<<=8; linuxread+=c; }
					else linuxread=c;
					return ct;
					break;
			// two
				case ((27<<8)+'O'): ctrl=CN_RCTRL;
				case ((27<<8)+'['):
					if(c=='A'){ c=MK_UP; t=CN_KEY; }
					if(c=='B'){ c=MK_DOWN; t=CN_KEY; }
					if(c=='C'){ c=MK_RIGHT; t=CN_KEY; }
					if(c=='D'){ c=MK_LEFT; t=CN_KEY; }
					if(c=='1' || c=='3' || c=='4'){ linuxread<<=8; linuxread+=c; }
					if(t){ ct.SetKey(1, t|ctrl, c); linuxread=0; }
					return ct;
					break;
			// three
				case ((27<<16)+('['<<8)+'1'):
					if(c>='1' && c<='9'){ linuxread<<=8; linuxread+=c; }
					if(c==126){ ct.SetKey(1, CN_KEY, MK_HOME); linuxread=0; return ct; } break;

				case ((27<<16)+('['<<8)+'2'):
					if(c>='1' && c<='4'){ linuxread<<=8; linuxread+=c; }

				case ((27<<16)+('['<<8)+'3'): if(c==126){ ct.SetKey(1, CN_KEY, MK_DELETE); linuxread=0; } return ct; break;

				case ((27<<16)+('['<<8)+'4'): if(c==126){ ct.SetKey(1, CN_KEY, MK_END); linuxread=0; } return ct; break;
			// four
			// F1-F12
			}

			if(c==126 && linuxread>=((27<<24)+('['<<16)+'1'<<8+'1') && linuxread<=((27<<24)+('['<<16)+'2'<<8+'1')){
				int k=0;
				if(linuxread&65535>='1'<<8+'1' && linuxread&65535<='1'<<8+'5'){ k=linuxread&255-'1'+1; }
				if(linuxread&65535>='1'<<8+'7' && linuxread&65535<='1'<<8+'9'){ k=linuxread&255-'7'+6; }
				if(linuxread&65535>='2'<<8+'0' && linuxread&65535<='1'<<8+'4'){ k=linuxread&255-'0'+9; }
				if(k){ ct.SetKey(1, CN_KEY, (MK_F1+k)&255); linuxread=0; return ct; }
			}

			if(linuxread){
				if((linuxread&224)==192 && (c&192)==128){
					c=((linuxread&31)<<6)+(c&63);
					ct.SetKey(1, 0, c); linuxread=0;
				}
				else { ct.SetKey(1, 0, linuxread); linuxread=c; }
				return ct;
			}
			
			//else if(linuxread==27 && c=='[') linuxread=c;
			//else if(linuxread=='['){ int t=0;
			//	if(c=='A'){ c=MK_UP; t=CN_KEY; }
			//	if(c=='B'){ c=MK_DOWN; t=CN_KEY; }
			//	if(c=='C'){ c=MK_RIGHT; t=CN_KEY; }
			//	if(c=='D'){ c=MK_LEFT; t=CN_KEY; }
			//	ct.SetKey(1, t, c); linuxread=0;
			//}
			//else if(linuxread){
			//	if((linuxread&224)==192 && (c&192)==128){
			//		c=((linuxread&31)<<6)+(c&63);
			//		ct.SetKey(1, 0, c); linuxread=0;
			//	}
			//	else { ct.SetKey(1, 0, linuxread); linuxread=c; }
			//}
			
			if(!linuxread){
				if(c==27) linuxread=27;
				else if((c&224)==192) linuxread=c;
				else ct.SetKey(1, 0, c);
			}
			
			//print(itos(c));
			//printf( "%c", c);
//#endif
		}
		return ct;
	}

	int SetCurPos(int x, int y){ COORD cr={x, y}; return SetCurPos(cr); }

/////////////////////////////////////////////////////////////////////////////////////
//draw
	bool Draw(ConsoleWindowP*wind, unsigned short x, unsigned short y, unsigned short *cha, int chs, unsigned short col, unsigned short bg){ if(!wind || !wind->Act()) return 0;
	unsigned short sx=wind->rect.GetW(), sy=wind->rect.GetH();
	if(x<0 || y<0 || x>sx || y>sy) return 0;
	x+=wind->rect.left; y+=wind->rect.top;

for(int i=0; i<chs; i++){ // GENERAL FOR
	short ch=cha[i]; if(!ch) ch=' ';

	if(x-wind->rect.left>=sx){
		x-=sx; y++;
		if(y-wind->rect.top>=sy) return 0;
	}

CHAR_INFO ci;
// TLOUT
if(tlout){
//	int r=wnd[0]->Draw_s(x, y, ch, col, bg);
//	if(!r) return 1;
	if(x!=inx || y!=iny){ tlout->VCSetCursor(x, y); inx=x; iny=y; } // pos x,y
	if(col!=incol){ tlout->VCSetColor(col); incol=col; } // color
	if(bg!=inbgcol){ tlout->VCSetBgColor(bg); inbgcol=bg; }
	tlout->VCSetChar(ch);
	inx++; if(inx>=rect.left){ inx=0; iny++; }

	x++;
	continue;
}

/*/// TL HTTP OUT
if(tlhout){
	int r=wnd[0]->Draw_s(x, y, ch, col, bg);
	if(!r) return 1;
	if(x!=inx || y!=iny) { tlhout->SendLocate(x, y); inx=x; iny=y; } // pos x,y
	if(r&2) if(col!=incol){ tlhout->SendColor(col); incol=col; } // color
	if(r&4) if(bg!=inbgcol){ tlhout->SendBgColor(bg); inbgcol=bg; }
	if(r&1) tlhout->SendUChar(ch);
	inx++; if(inx>=rect.X){ inx=0; iny++; }
return 1;
}
/*/// TLOUT END

if(!outconsole){// ch='A';
ci.Char.UnicodeChar=ch; ci.Attributes=cicol(col, bg);
//if(!ci.Char.UnicodeChar) ci.Char.UnicodeChar=32;
//WriteConsoleOutputW(_std_output_handle, &ci, s, p, &sr);
//	if(!wndsz) return 0;
//	int r=wnds._p->wnd->Draw_s(x, y, ch, col, bg);
//	if(!r)
//		return 1;
	if(x!=inx || y!=iny){ SetCurPos(x, y);/* inx=x; iny=y;*/ } 

#ifdef WIN32
//if(x!=inx || y!=iny){ SetCurPos(x, y);/* inx=x; iny=y;*/ }
	COORD s={1,1}, p={0,0}; SMALL_RECT sr={x,y,x,y};

//	if(r&7){
		WriteConsoleOutputW(_std_output_handle, &ci, s, p, &sr);
		inx++;
		//ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
		//write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
//	}

	//inx++; //if(inx>=rect.X){ inx=0; iny++; }
#else
//	if(cx!=x && cy!=y) set
	//int r=wnd[0]->Draw_s(x, y, ch, col, bg);
	//if(!r) return 1;
	//if(x!=inx || y!=iny){ SetCurPos(x, y);/* inx=x; iny=y;*/ } // pos x,y
	//if(r&2) if(col!=incol){ tlhout->SendColor(col); incol=col; } // color
	//if(r&4) if(bg!=inbgcol){ tlhout->SendBgColor(bg); inbgcol=bg; }
	//if(r&2)
	col=cilcol(col);
		if(col!=incol){ SetLinuxColor(col); incol=col; } // color
	//if(r&4)
	bg=cilcol(bg);
		if(bg!=inbgcol){ SetLinuxBgColor(bg); inbgcol=bg; } // color
	
//	if(r&7){ // sumbol
		ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
		write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
		inx++;
//	}
	//if(inx>=rect.X){ inx=0; iny++; }
//	if(ci.Char.AsciiChar==9)
//		inx+=8-inx%8;
//	else inx++;
//	if(inx>=rect.X){ inx=0; iny++; MoveCurPos(-inx, 1); }
	//write(0, &ci.Char.UnicodeChar, ci.Char.UnicodeChar<256 ? 1 : 1);
#endif
		x++;
}

} // GENERAL FOR END
return 1;
}

bool DrawFull(ConsoleWindowP*wind){
	int x=wind->rect.left; int y=wind->rect.top;
	int sx=wind->rect.GetW(); int sy=wind->rect.GetH();
	if(tlout /*|| tlhout*/) return 0;

if(!outconsole){
CHAR_INFO ci;
for(int k=0; k<sy; k++)
for(int i=0; i<sx; i++){
	COORD s={1,1}, p={0,0}; SMALL_RECT sr={i+x,k+y,i+x,k+y};
	ci.Char.UnicodeChar=wind->vbuf[i+sx*k+sx*wind->bufup];
	if(!ci.Char.UnicodeChar) ci.Char.UnicodeChar=32;
	ci.Attributes=cicol(wind->cbuf[i+sx*k+sx*wind->bufup], wind->bbuf[i+sx*k+sx*wind->bufup]);
#ifdef WIN32
	WriteConsoleOutputW(_std_output_handle, &ci, s, p, &sr); 
#else

	//int r=wnd[0]->Draw_s(i, k, ch, col, bg);
	//if(!r) return 1;
	if(i!=inx || k!=iny){ SetCurPos(i, k);/* inx=x; iny=y;*/ } // pos x,y

	unsigned short col=cilcol(wind->cbuf[i+sx*k+sx*wind->bufup]);
		if(col!=incol){ SetLinuxColor(col); incol=col; } // color
	//if(r&4)
	col=cilcol(wind->bbuf[i+sx*k+sx*wind->bufup]);
		if(col!=inbgcol){ SetLinuxBgColor(col); inbgcol=col; } // color
	
	//SetCurPos(i, k);
	//if(r&2) if(col!=incol){ tlhout->SendColor(col); incol=col; } // color
	//if(r&4) if(bg!=inbgcol){ tlhout->SendBgColor(bg); inbgcol=bg; }
	//if(r&1){
		ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
		if(!ci.Char.UnicodeChar) ci.Char.UnicodeChar=' ';
		write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
		inx++;
	//}

//		ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
//		write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
//		if(ci.Char.AsciiChar==9) inx+=8-inx%8; else inx++;
//		if(inx>=rect.X){ inx=0; iny++; MoveCurPos(-inx, 1); }
	//write(0, &ci.Char.UnicodeChar, ci.Char.UnicodeChar<256 ? 1 : 2);
#endif

}}
return 1;
}

	bool DrawMove(ConsoleWindowP*wind, int x, int y, int cx, int cy, int t, int s, int r=0){
//	if(tlout){ tlout->SendMove(x, y, cx, cy, t, s); return 1; }	
	//if(tlhout){ tlhout->SendMove(x, y, cx, cy, t, s); return 1; }	
	//wind->DrawMove(x, y, cx, cy, t, s);
	DrawFull(wind); return 1;
	return 1;
	}
	
// cursor
	int SetCurPos(COORD cd){// if(!this) return 0;
	// TLOUT
	if(tlout){ // if move text??
		if(inx!=cd.X ||iny!=cd.Y) tlout->VCSetCursor(cd.X, cd.Y); inx=cd.X; iny=cd.Y; return 1;
	}
	// TL HTTP OUT
	//if(tlhout){
	//	if(incurx!=cd.X ||incury!=cd.Y) tlhout->SendCursor(cd.X, cd.Y); incurx=cd.X; incury=cd.Y; return 1;
	//}

	if(!inconsole){
		//if(incurx!=cd.X ||incury!=cd.Y)
	#ifdef WIN32
		SetConsoleCursorPosition(_std_output_handle, cd);
	/*
		while(cd.X<0){ cd.Y--; cd.X+=rect.X; }
		while(cd.X>=rect.X){ cd.Y++; cd.X-=rect.X; }
		int mx=cd.X-inx; int my=cd.Y-iny;

		gtx+=mx; gty+=my;
		inx=cd.X; iny=cd.Y;
		cd.X=gtx; cd.Y=gty;
		SetConsoleCursorPosition(_std_output_handle, cd);
	*/
	#else
		Itos it, itc; int sx=rect.GetW();
		while(cd.X<0 && sx){ cd.Y--; cd.X+=sx; }
		while(cd.X>=sx && sx){ cd.Y++; cd.X-=sx; }
		int mx=cd.X-inx; int my=cd.Y-iny;

		char d[]={27, '[', '0', 'C'}; //, ';', 0, '2', 'H',  	27, '['};
		char nm[]="0123456789";

		int m=mx<0 ? -1 : 1;
		//new EF
		//mx=cd.X;
	while(mx){
		if(mx<0) it.Add(VString((char*)&d, 2), mx>-10 ? VString((char*)&nm[-mx], 1) : "9", VString("D", 1));
		//, itc.itos(cd.X), "C");
		else it.Add(VString((char*)&d, 2), mx<10 ? VString((char*)&nm[mx], 1) : "9", VString("C", 1));//, itc.itos(cd.X), "C");
		if(mx) write(1, it.ret, it);
		//mx-=min(mx, 9);
		//mx-=min(mx<0 ? -mx : mx, 9)*m;
		if(mx<0) mx+=mx>-10 ? -mx : 9; else mx-=mx<10 ? mx : 9;
	} inx=cd.X;

		m=my<0 ? -1 : 1;
	while(my){
	//	if(my<0) it.Add(VString((char*)&d, 2), itc.itos(min(-my, 9)), VString("F", 1));//, itc.itos(cd.X), "C");
	//	else it.Add(VString((char*)&d, 2), itc.itos(min(mx, 9)), VString("E", 1));//, itc.itos(cd.X), "C");
		if(my<0) it.Add(VString((char*)&d, 2), my>-10 ? VString((char*)&nm[-my], 1) : "9", VString("A", 1));//, itc.itos(cd.X), "C");
		else it.Add(VString((char*)&d, 2), my<10 ? VString((char*)&nm[my], 1) : "9", VString("B", 1));//, itc.itos(cd.X), "C");
		//else it.Add(VString((char*)&d, 2), "1", VString("E", 1));//, itc.itos(cd.X), "C");
		write(1, it.ret, it);
		if(my<0) my+=my>-10 ? -my : 9; else my-=my<10 ? my : 9;
		//if(my<0) my+=min(-my, 9); else // else my-=min(mx, 9);
		//	my+=max(-my, -9);
		//my-=min(my<0 ? -my : my, 9)*m;
	} iny=cd.Y;

	// mode B>E, A>F

	/*
		char d[]={27, '[', '0', '1', ';', 0, '2', 'H', 
			27, '['};
		char d2[]={'B', 27, '['};
		it.Add(VString((char*)&d, 10), itc.itos(cd.Y), VString((char*)&d2, 1));//, itc.itos(cd.X), "C");
		//, '0'+cd.Y,  '0'+cd.X, 'B'};
		write(0, VString(it), it);

		//char d3[]={ 27, '[' };
		if(cd.X){
			it.Add(VString((char*)&d, 2), itc.itos(cd.X), "a"); //itc.itos(cd.X-cd.Y)
				write(0, VString(it), it);
		}*/

	//	MString ms=itos(cd.X)+" "+itos(cd.Y);
	//	write(0, ms, ms);
	#endif
		}
	return 1;
	}

	

// Set
	void SetTitle(VString title){
		if(tlout){
			tlout->VCSetTitle(title);
			return ;
		}

#ifdef WIN32
		SetConsoleTitle(MODUNICODE(title));
#endif
		return ;
	}


	/////////////////////////////////////////////////////////////////////////////////////
	void SetLinuxColor(unsigned short col){
		char d[]={27, '[', '0', ';', '3', '1', 'm'};
		//char d[]={27, '[', '2', ';', '3', 'x'};
		//Esc[2;nx
//		print("z", itos(col), " ");
		d[2]=col>9 ? '1' : '0';
		d[5]=col%10+'0';
		write(0, &d, sizeof(d));
	return ;
	}
	
	void SetLinuxBgColor(unsigned short col){
		char d[]={27, '[', '0', ';', '4', '2', 'm'};
		d[2]=col>9 ? '1' : '0';
		d[5]=col%10+'0';
		write(0, &d, sizeof(d));
	return ;
	}

/////////////////////////////////////////////////////////////////////////////////////
// color
	WORD cicol(unsigned short c, unsigned short b){
	WORD ret=0;// int z=0;
	int t=c&0x1f; if(t>=32/3) ret|=FOREGROUND_BLUE; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
	t=(c&0x7e0)>>5; if(t>=64/3) ret|=FOREGROUND_GREEN; if(t>=64*2/3) ret|=FOREGROUND_INTENSITY;t=(c&0x1f800)>>11;
	if(t>=32/3) ret|=FOREGROUND_RED; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
	//if(z>=64) ret|=FOREGROUND_INTENSITY; z=0;

	t=b&0x1f; if(t>=32/3) ret|=BACKGROUND_BLUE; if(t>=32*2/3) ret|=BACKGROUND_INTENSITY;
	t=(b&0x7e0)>>5; if(t>=64/3) ret|=BACKGROUND_GREEN; if(t>=64*2/3) ret|=BACKGROUND_INTENSITY;
	t=(b&0x1f800)>>11; if(t>=32/3) ret|=BACKGROUND_RED; if(t>=32*2/3) ret|=BACKGROUND_INTENSITY;//if(z>=64) ret|=BACKGROUND_INTENSITY; z=0;

	ret|=COMMON_LVB_LEADING_BYTE|COMMON_LVB_TRAILING_BYTE|COMMON_LVB_GRID_HORIZONTAL|COMMON_LVB_UNDERSCORE;
	return ret;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	WORD cilcol(unsigned short c){
		c=cicol(c, 0);

		int i=c&FOREGROUND_INTENSITY ? 10 : 0;
		c=c&(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		switch(c){
			case 0: return i+0; // black
			case FOREGROUND_RED: return i+1; // red
			case FOREGROUND_GREEN: return i+2; // green
			case FOREGROUND_BLUE: return i+4; // blue

			case FOREGROUND_BLUE+FOREGROUND_GREEN: return i+6; // cyan
			case FOREGROUND_BLUE+FOREGROUND_RED: return i+5; // magenta
			case FOREGROUND_GREEN+FOREGROUND_RED: return i+3; // yellow
			default: return i+7; break;
		}

		return 0;

	WORD ret=0; int r, g, b;// int z=0; 31.63.31
	b=c&0x1f; g=(c&0x7e0)>>5; r=(c&0x1f800)>>11;
//	print("c", itos(r), " ", itos(g), " ", itos(b), ". ");

	if(b<8 && g<16 && r<8) return 0;
	if(b>=8 && g<16 && r<8) return 4; // blue
	if(b<8 && g>=16 && r<8) return 2; // green
	if(b<8 && g<16 && r>=8) return 1; // red

	if(b>=8 && g>=16 && r<8) return 6; // cyan??
	if(b<8 && g>=16 && r>=8) return 3; // yellow
	if(b>=8 && g<16 && r>=8) return 5; // magenta
	//if(b<8 && g<16 && r<8) return 0;
	return 7;

//“ÂÍÒÚ       ‘ÓÌ         ÷‚ÂÚ
//30          40          ◊ÂÌ˚È
//31          41           ‡ÒÌ˚È
//32          42          «ÂÎÂÌ˚È
//33          43          ∆ÂÎÚ˚È
//34          44          —ËÌËÈ
//35          45          ‘ËÓÎÂÚÓ‚˚È
//36          46          √ÓÎÛ·ÓÈ
//37          47          ¡ÂÎ˚È
	}

	//WORD cilbgcol(unsigned short c){
	//WORD ret=0; int r, g, b;// int z=0; 31.63.31
	//b=c&0x1f; g=(c&0x7e0)>>5; r=(c&0x1f800)>>11;

	//if(b<8 && g<16 && r<8) return 0;
	//if(b>=8 && g<16 && r<8) return 4; // blue
	//if(b<8 && g>=16 && r<8) return 2; // green
	//if(b<8 && g<16 && r>=8) return 1; // red

	//if(b>=8 && g>=16 && r<8) return 6; // cyan??
	//if(b<8 && g>=16 && r>=8) return 3; // yellow
	//if(b>=8 && g<16 && r>=8) return 5; // magenta
	////if(b<8 && g<16 && r<8) return 0;
	//return 7;
	//}

};

/////////////////////////////////////////////////////////////////////////////////////
int ConsoleWindow::Read(contype ct){ // if 0 - next, if 1 - ok & end;
	if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){
		if(ct.getcode()==MK_TAB && ct.getkeys()&(CN_RCTRL|CN_LCTRL)){ gcon->ActItVis(); return 1; }
	}
return 0;
}

void ConsoleWindow::SetHead(VString h){ gwnd->SetHead(h); }
void ConsoleWindow::SetCurPos(COORD cd){ if(gwnd) gwnd->SetCurPos(cd); }

// Global Class Functions

void ConsoleWindow::DrawAsWindow(){
	int sx=rect.GetW(), sy=rect.GetH();
	if(!sx || !sy) return ; // Draw Menu =¶-„„¨¨¨LLL---¶¶¶¶¶¶TTT¶¶¶++
	unsigned short bg=gwnd->defbg, &defc=gwnd->defc;
	if(!gwnd->Act()) bg/=2;

	for(int i=1; i<sx-1; i++){ Draw(i, 0, brel[1], defc, bg); Draw(i, sy-1, brel[7], defc, bg);}
	for(int i=1; i<sy-1; i++){ Draw(0, i, brel[3], defc, bg); Draw(sx-1, i, brel[5], defc, bg); }
	Draw(0,0, brel[0], defc, bg); Draw(sx-1, 0, brel[2], defc, bg);
	Draw(0,sy-1, brel[6], defc, bg); Draw(sx-1, sy-1, brel[8], defc, bg);

	//if(gcon->drawglobtime){
	//	MTime64 mt; MString ms=mt.date("H:i"); Draw(sx-5, 0, ms, defc, bg);
	//}	
}	

void ConsoleWindowP::Drawn(ConsoleWindow *wnd, const VString line, unsigned short col, unsigned short bg){
	if(!this){ return ; }
//	UGLOCK(*this);
	if(col==65534) col=defc; if(bg==65534) bg=defbg;
	unsigned short ca[32]; int cs=0;
	for(int i=0; i<line.sz; i++){
		ca[cs]=ctos(line[i]); cs++;
		if(cs>=32){
			Drawn(wnd, (unsigned short*)&ca, cs, col, bg); cs=0; }
	}
	if(cs) Drawn(wnd, (unsigned short*)&ca, cs, col, bg);
}

void ConsoleWindowP::DrawnU(ConsoleWindow *wnd, const VString line, unsigned short col, unsigned short bg){
	MString ln=utftos(line);
	return Drawn(wnd, (unsigned short*)ln.rchar(), ln.size()/2, col, bg);
}

/*
void ConsoleWindow::Drawn(char*line, unsigned short col, unsigned short bg){
	if(col==65534) col=defc; if(bg==65534) bg=defbg;
	unsigned short ca[32]; int cs=0;
	for(int i=0; i<strlen(line); i++){ ca[cs]=stosr(line[i]); cs++; if(cs>=32){ Drawn((unsigned short*)&ca, cs, col, bg); cs=0; }}
	if(cs) Drawn((unsigned short*)&ca, cs, col, bg);
}*/

void ConsoleWindowP::Drawn(ConsoleWindow *wnd, unsigned short *ca, int cs, unsigned short col, unsigned short bg){
	if(!this){ return ; }
	UGLOCK(*this);
	if(col==65534) col=defc; if(bg==65534) bg=defbg; if(!rect.GetW() || !rect.GetH()) return ;
	int sx=rect.GetW(), sy=rect.GetH(), su, ss, ii;
	MRect rc=wnd->rect;

for(int i=0; i<cs; i++){ // GENERAL FOR
	unsigned short c=ca[i];

	// to window
	if(curpos/sx<rc.top) curpos+=(rc.top-curpos/sx)*sx-curpos%sx;
	if(curpos%sx<rc.left) curpos+=rc.left-curpos%sx;
	

	short x=curpos%sx, y=curpos/sx;
	if(c==13){ curpos-=x; }
	else if(c==10){ curpos+=sx-x; }
	else if(c==9)// && (curpos%sx)%8)
	{ x+=8-(curpos%sx)%8; curpos+=8-(curpos%sx)%8; } // no return's
	
	if(x>=rc.right){ x=0; y++; }
	if(y>=rc.bottom){
		y--; curpos-=sx; su; //su=(sx*(sy+bufup)-sx);
		ss=rc.right-rc.left;

		for(ii=rc.top; ii<rc.bottom-1; ii++){
			memcpy(vbuf+(sx*(ii+bufup))+rc.left, vbuf+(sx*(ii+bufup+1))+rc.left, ss*2);
			memcpy(cbuf+(sx*(ii+bufup))+rc.left, cbuf+(sx*(ii+bufup+1))+rc.left, ss*2);
			memcpy(bbuf+(sx*(ii+bufup))+rc.left, bbuf+(sx*(ii+bufup+1))+rc.left, ss*2);
			/*for( int z=0; z<ss; z++){
				*(vbuf+(sx*(ii+bufup))+rc.left+z)='A'+ii;
			}*/
			//memset(vbuf+(sx*(ii+bufup))+rc.left, 'A'+ii, ss*2); 
		}
		memset(vbuf+(sx*(ii+bufup))+rc.left, 0, ss*2); 
		memset(cbuf+(sx*(ii+bufup))+rc.left, 0, ss*2); 
		memset(bbuf+(sx*(ii+bufup))+rc.left, 0, ss*2); 

	//memcpy(vbuf, vbuf+sx, su*2); memcpy(cbuf, cbuf+sx, su*2); memcpy(bbuf, bbuf+sx, su*2);
	//memset(vbuf+su, 0, sx*2); memset(cbuf+su, 0, sx*2); memset(bbuf+su, 0, sx*2);
	//gcon->DrawFull(this);
		//c=13;
		if(!gcon->TLOut())
			//rect.Move(0, 1);
			//gcon->Draw(this, x, y+1, &c, 1, col, bg);
		//print("\n");
		gcon->DrawMove(this, x, y, sx, sy, 0, 1);
		//continue;
	}

	if(c==13 || c==10 || c==9){
		int zx=x+this->rect.left, zy=y+this->rect.top; gcon->SetCurPos(zx, zy);
		continue;
//		return ;
	}

	vbuf[x+(y+bufup)*sx]=c; cbuf[x+(y+bufup)*sx]=col; bbuf[x+(y+bufup)*sx]=bg;	
	gcon->Draw(this, x, y, &c, 1, col, bg);
	curpos++;

} //	GENERAL FOR END
return ;
}

void ConsoleWindowP::Draw(ConsoleWindow *wnd, int x, int y, unsigned short c, unsigned short col, unsigned short bg){
	if(!this){ globalerror("ConsoleWindowP::Drawn is null"); return ; }
	int sx=rect.GetW(), sy=rect.GetH();	
	//if(!wnd){ globalerror("ConsoleWindowP::Draw wnd is null"); exit(1); }
	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return ; if(col==65534) col=defc; if(bg==65534) bg=defbg;
	x+=wnd->rect.left; y+=wnd->rect.top;
	UGLOCK(*this);
	//if(vbuf[x+(y+bufext)*sx]!=c || cbuf[x+(y+bufext)*sx]!=col || bbuf[x+(y+bufext)*sx]!=bg)
	{
		if(x<0 || x>=sx || y<0 || y>=sy) return ;
 		vbuf[x+(y+bufup)*sx]=c; cbuf[x+(y+bufup)*sx]=col; bbuf[x+(y+bufup)*sx]=bg;
		gcon->Draw(this, x, y, &c, 1, col, bg);
	}
	return ;
}

void ConsoleWindowP::Draw(ConsoleWindow *wnd, int x, int y, VString line, unsigned short col, unsigned short bg){
	if(!this){ globalerror("ConsoleWindowP::Drawn is null"); return ; }
	int sx=rect.GetW(), sy=rect.GetH();
	//if(!wnd){ globalerror("ConsoleWindowP::Draw wnd is null"); exit(1); }
	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return ; if(col==65534) col=defc; if(bg==65534) bg=defbg;
	x+=wnd->rect.left; y+=wnd->rect.top;
	UGLOCK(*this);
	//if(vbuf[x+(y+bufext)*sx]!=c || cbuf[x+(y+bufext)*sx]!=col || bbuf[x+(y+bufext)*sx]!=bg)
	for(int i=0; i<line.size(); i++){
		if(x<0 || x>=sx || y<0 || y>=sy) return ;
		unsigned short c=ctos(line[i]);
 		vbuf[x+(y+bufup)*sx]=c; cbuf[x+(y+bufup)*sx]=col; bbuf[x+(y+bufup)*sx]=bg;
		gcon->Draw(this, x, y, &c, 1, col, bg);
		x++;
	}
	return ;
}

void ConsoleWindowP::Draw(ConsoleWindow *wnd, int x, int y, DrawBLine &dl){
	if(!this){ globalerror("ConsoleWindowP::Drawn is null"); return ; }
	int sx=rect.GetW(), sy=rect.GetH();
	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return ;
	x+=wnd->rect.left; y+=wnd->rect.top;
	UGLOCK(*this);

	unsigned short *vb=dl.getvbuf(), *cb=dl.getcbuf(), *bb=dl.getbbuf();
	//unsigned short col=defc, bg=defbg; unsigned short c;

	for(int i=0; i<dl.getw(); i++){
		for(int k=0; k<dl.geth(); k++){

			int px=x+i, py=y+k, pos=i+k*dl.getw(), bpos=px+(py+bufup)*sx;
			
			// test
			if(px<0 || px>=sx || py<0 || py>=sy) continue;
			if(!vb[pos]) continue;

			vbuf[bpos]=vb[pos]; cbuf[bpos]= (cb[pos]==65534 ? defc : cb[pos]); bbuf[bpos]= (bb[pos]==65534 ? defbg : bb[pos]);
			gcon->Draw(this, px, py, &vbuf[bpos], 1, cbuf[bpos], bbuf[bpos]);
		}
	}
	return ;
}




void ConsoleWindowP::Draw(ConsoleWindow *wnd, int x, int y, DrawDLine &dl){
	if(!this){ globalerror("ConsoleWindowP::Drawn is null"); return ; }
	int sx=rect.GetW(), sy=rect.GetH();
	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return ;
	x+=wnd->rect.left; y+=wnd->rect.top;
	UGLOCK(*this);
	
	unsigned short col=defc, bg=defbg; unsigned short c;
	DrawDLineU *get;
	
	while(get=dl.Get()){
		switch(get->type){
			case DRAWDLINE_TEXT:{
				for(int i=0; i<get->t.sz; i++){
					if(x<0 || x>=sx || y<0 || y>=sy) return ;
					c=ctos(get->t.data[i]);
 					vbuf[x+(y+bufup)*sx]=c; cbuf[x+(y+bufup)*sx]=col; bbuf[x+(y+bufup)*sx]=bg;
					gcon->Draw(this, x, y, &c, 1, col, bg);
					x++; if(x>=sx){ y++; x=wnd->rect.left; }
				}
			break;}

			case DRAWDLINE_COLOR:
				col=MRGB(get->c.rgb).sh();
			break;

			case DRAWDLINE_BGCOLOR:
				bg=MRGB(get->c.rgb).sh();
			break;

			case DRAWDLINE_FONT:
			break;

			case DRAWDLINE_END:
				c=' ';
				for(x; x<wnd->rect.right; x++)
					gcon->Draw(this, x, y, &c, 1, col, bg);
				if(x>=sx){ y++; x=wnd->rect.left; }
			break;
		}
	}
	return ;
}



bool ConsoleWindowP::Act(bool v){
	if(v) stat|=CONSOLEWND_ISACT; else stat&=~CONSOLEWND_ISACT;
	if(v){
		gcon->SetTitle(head); // SetConsoleTitle
		gcon->cedit.rect=cedit;
	}
	return v;
}

void ConsoleWindowP::SetHead(VString h){
	head=h;
	if(Act()) gcon->SetTitle(head); // SetConsoleTitle
}

int ConsoleWindowP::OnSize(int sx, int sy){ if(sx*sy>S1M) return 0;
	if(rect.GetW()==sx && rect.GetH()==sy && _buf.size()==sx*(sy+bufup)*2*3+sx*(sy+bufup)/8+1) return 1;
//	if(rect.left==0 || rect.right==cx || rect.top==0 || rect.bottom==cy) return 1;
//	UGLOCK(consolelock);
	rect.right=rect.left+sx; rect.bottom=rect.top+sy;
	vbuf=0; cbuf=0; bbuf=0;
	
	_buf.Reserv(sx*(sy+bufup)*2*3+sx*(sy+bufup)/8+1, 0); // sizeof(short), 3 buffers, mbool
	vbuf=(unsigned short*)_buf.pos(0);
	cbuf=(unsigned short*)_buf.pos(sx*(sy+bufup)*2);
	bbuf=(unsigned short*)_buf.pos(sx*(sy+bufup)*2*2);
	upbuf=_buf.pos(sx*(sy+bufup)*2*3);

//#ifndef WIN32
//	gcon->SetCurPos(0, 0); int cp=curpos;
//	for(int i=0; i<rect.GetH(); i++){
//		Drawn(wnd, "\n", 1);
//	}
//	curpos=cp;
//#endif

	wnd->OnSize(sx, sy);
	ReDraw();

	//UseCedit(MRect(0, sy-2, sx, sy));
	return 1;
}



// Console Window Draw
void ConsoleWindow::Drawn(const VString line, unsigned short col, unsigned short bg){
	return gwnd->Drawn(this, line, col, bg);
}
void ConsoleWindow::DrawnU(const VString line, unsigned short col, unsigned short bg){
	return gwnd->DrawnU(this, line, col, bg);
}

void ConsoleWindow::Drawn(unsigned short *ca, int cs, unsigned short col, unsigned short bg){
	return gwnd->Drawn(this, ca, cs, col, bg);
}

void ConsoleWindow::Draw(int x, int y, unsigned short c, unsigned short col, unsigned short bg){
	return gwnd->Draw(this, x, y, c, col, bg);
}

void ConsoleWindow::Draw(int x, int y, const VString line, unsigned short col, unsigned short bg){
	return gwnd->Draw(this, x, y, line, col, bg);
}

void ConsoleWindow::Draw(int x, int y, DrawDLine &dl){
	return gwnd->Draw(this, x, y, dl);
}

void ConsoleWindow::Draw(int x, int y, DrawBLine &dl){
	return gwnd->Draw(this, x, y, dl);
}




//void ConsoleWindow::Clear(unsigned short cha, const MRGB&col, const MRGB&bg){ 
//	unsigned short c=col.sh(), b=bg.sh();
//	for(int i=0; i<sx; i++)
//		for(int k=0; k<sy; k++){Draw(i, k, cha, c, b); }
//	gcon->DrawFull(this);
//}
//
//void ConsoleWindow::Clear(int x, int y, int tx, int ty, unsigned short cha, unsigned short col, unsigned short bg){ 
//	maxel(x,0); maxel(y,0); minel(tx,sx); minel(ty,sy);
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	for(int i=x; i<=tx; i++)
//		for(int k=y; k<=ty; k++){ Draw(i, k, cha, col, bg); }
//	//gcon->DrawFull(this);
//}
//
//void ConsoleWindow::ClearLine(int y, unsigned short cha, unsigned short col, unsigned short bg){ 
//	int x=0, tx=sx;// maxel(y,0); minel(ty,sy);
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	for(int i=x; i<=tx; i++)
//		{ Draw(i, y, cha, col, bg); }
//		//gcon->Draw(this, x, y, );
//}
//
//void ConsoleWindow::DrawFull(){ if(!gcon) return ; GWGESet(cmdt); gcon->DrawFull(this); return ; }
//

//bool ConsoleWindow::GWGESet(VString text){ if(!gcon) return 0;
//	gcon->gw.cne.SetTitle(text);
//	return 1;
//}
//
//MString ConsoleWindow::WGGeGet(){ if(!gcon) return 0;
//	return gcon->gw.cne.GetText();
//}
//
//MString ConsoleWindow::WGGeGetSafe(){ if(!gcon) return 0;
//	return gcon->gw.cne.GetTextSafe();
//}
//
//bool ConsoleWindow::WGGeIs(){ if(!gcon) return 0;
//	return gcon->gw.cne.GetIs();
//}

int CWEdit::movekey(){ if(rect.left<-S16K || rect.left>S16K || !rect.Is()) return 0;
	if(rect.GetW()>0){
		bool m=0;
		while((key-buf)/rect.GetW()>=ln+2){
			ln++; m=1;
		}
		while((key-buf)/rect.GetW()<ln){
			ln--; m=1;
		}
		if(m){ ReDraw(); return 1; }
	}
	COORD coord;
	coord.X=rect.left+(key-buf)%rect.GetW();
	coord.Y=rect.top+(key-buf)/rect.GetW()-ln;
	gcon->SetCurPos(coord);
	return 0;
}


int VirtualConsole::VCSetConsole(GlobalConsole *con){
	console=con; if(!con) return 0;  
	console->tlin=this;
	console->tlout=this;
	return 1;
}
int VirtualConsole::InRead(contype ct){
	if(console) console->InRead(ct);
	return 0;
}




//void CWGSelectWindow::DrawIt(){
//	Drawn("111111112222222222223333333344444444");
//return ;
//}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


//class GlobalConsole;
//#define CNACT	1
//#define CNVIS	2
//#define CNVIONE	4
//
//unsigned short brel[10]={0x2554, 0x2550, 0x2557, 0x2551, 0x2550, 0x2551, 0x255A, 0x2550, 0x255D, 0x25A0 }; // border element
//// 0 1 2
//// 3 4 5
//// 6 7 8. 9 - border
////brel[0]=; brel[1]=; brel[2]=; brel[3]=; brel[4]=; brel[5]=; brel[6]=; brel[7]=; brel[8]=; brel[9]=;
//
//int CWGLOBTime=1;
//
///////////////////////////////////////////////////////////////////////////////////////
//class ConsoleWindow: public MSVCOT, public GLock{ public: // protected:
//int sx, sy, cmd, wid; // size x,y; is command, windows id
//public:
//short x, y, cx, cy; // positions , cursor;
////protected:
//unsigned short *vbuf, *cbuf, *bbuf; unsigned short defc, defbg, defs;  // buffer & colors & sel
//MString _buf, head; int curpos; int bufext; // save x last lines
//BYTE cursz, level, isdel, isact; MString cmdt;
////Readdir dir; MString folder, sel;
//
////friend GlobalConsole;
//
//public:
//GlobalConsole*gcon;
//
//virtual void ItFocus(){ }
//
//ConsoleWindow(){ head="Default Console Window"; bufext=0;
//	sx=0; sy=0; cmd=0; wid=0; curpos=0; cursz=10; level=1; isdel=0; isact=0;
//	defc=MRGB("ffffff").sh(); defbg=MRGB("000000").sh(); defs=MRGB("00ff00").sh(); gcon=0;
//}
//virtual int Activate(){ COTS(); return 1;}
//virtual int Deactivate() { cottid=0; return 1;}
//
//virtual int Size(int x, int y){ if(x*y>S1M) return 0;
//	if(sx==x && sy==y) return 1; Lock(); vbuf=0; cbuf=0; bbuf=0;
//	sx=x; sy=y; _buf.Reserv(x*(y+bufext)*2*3*2, 0); // del *2
//	vbuf=(unsigned short*)_buf.pos(0); cbuf=(unsigned short*)_buf.pos(x*(y+bufext)*2); bbuf=(unsigned short*)_buf.pos(x*(y+bufext)*2*2);
//    UnLock(); return 1;
//}
//
//virtual int Pos(short _x, short _y, short _cx, short _cy){
//	x=_x; y=_y; cx=_cx; cy=_cy; Size(cx-x, cy-y); return 1;
//}
//
//
//void Draw(int x, int y, unsigned short c, unsigned short col=65534, unsigned short bg=65534);
//void Draw(int x, int y, const VString line, unsigned short col=65534, unsigned short bg=65534);
//void Drawn(const VString line, unsigned short col=65534, unsigned short bg=65534);
////virtual void Draw(int x, int y, unsigned short c, int sz, unsigned short col=65534, unsigned short bg=65534);
//void Drawn(unsigned short *ca, int cs, unsigned short col=65534, unsigned short bg=65534);
//
////void Drawn(unsigned char c, unsigned short col=65534, unsigned short bg=65534){
////return Drawn(stosr(c), col, bg); }
//
//void Drawn(char c, unsigned short col=65534, unsigned short bg=65534){
//unsigned short s=stosr(c); return Drawn(&s, 1, col, bg); }
//
////void Drawn(unsigned char *c, unsigned short col=65534, unsigned short bg=65534){
////for(int i=0; i<strlen((char*)c); i++) Drawn(stosr(c[i]), col, bg); return ; }
///*
//void Drawn(char *c, unsigned short col=65534, unsigned short bg=65534){ if(!c) return ;
//for(int i=0; i<strlen((char*)c); i++) Drawn(stosr(c[i]), col, bg); return ; }
//
//void Drawn(const VString vs, unsigned short col=65534, unsigned short bg=65534){
//char*c=vs; int sz=vs; if(!c) return ;
//for(int i=0; i<sz; i++) Drawn(stosr(c[i]), col, bg); return ; }
//*/
////void Drawn(const MString &vs, unsigned short col=65534, unsigned short bg=65534){
////char*c=vs; int sz=vs; if(!c) return ;
////for(int i=0; i<sz; i++) Drawn(stosr(c[i]), col, bg); return ; }
//
//bool Draw_s(int x, int y, unsigned short c, unsigned short col, unsigned short bg){
//	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return 0;
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	UGLock(*this); int r=0;
////	if((char*)&vbuf[x+(y+bufext)*sx]<_buf.rchar() || (char*)&vbuf[x+(y+bufext)*sx]>=_buf.end())
////		globalerror("MEM!!!\r\n");
////	if((char*)&cbuf[x+(y+bufext)*sx]<_buf.rchar() || (char*)&cbuf[x+(y+bufext)*sx]>=_buf.end())
////		globalerror("MEM!!!\r\n");
////	if((char*)&bbuf[x+(y+bufext)*sx]<_buf.rchar() || (char*)&bbuf[x+(y+bufext)*sx]>=_buf.end())
////		globalerror("MEM!!!\r\n");
//	//return 4+2+1;
//	if(vbuf[x+(y+bufext)*sx]!=c){ vbuf[x+(y+bufext)*sx]=c; r+=1; }
//	if(cbuf[x+(y+bufext)*sx]!=col){ cbuf[x+(y+bufext)*sx]=col; r+=2; } 
//	if(bbuf[x+(y+bufext)*sx]!=bg){ bbuf[x+(y+bufext)*sx]=bg; r+=4; } 
//	//	gcon->Draw(this, x, y, c, col, bg);
//
//	return r;
//}
//
//	void DrawAsWindow(){ if(!sx || !sy) return ; // Draw Menu =¶-„„¨¨¨LLL---¶¶¶¶¶¶TTT¶¶¶++
//		short bg=defbg;
//		if(!(isact&CNACT)) bg/=2;
//	
//		for(int i=1; i<sx-1; i++) {Draw(i, 0, brel[1], defc, bg); Draw(i, sy-1, brel[7], defc, bg);}
//		for(int i=1; i<sy-1; i++) {Draw(0, i, brel[3], defc, bg); Draw(sx-1, i, brel[5], defc, bg); }
//		Draw(0,0, brel[0], defc, bg); Draw(sx-1, 0, brel[2], defc, bg);
//		Draw(0,sy-1, brel[6], defc, bg); Draw(sx-1, sy-1, brel[8], defc, bg);
//	
//		if(CWGLOBTime){
//			MTime64 mt; MString ms=mt.date("H:i"); Draw(sx-5, 0, ms, defc, bg);
//		}
//	
//	}	
//	// Menu 
//	/*	char o[]="_F1-F4_  _F5_ _F6_", t[]="_F7-F9 _F10 _F11 _F12"; unsigned short cl=MRGB("00aa00").sh();
//		//Draw(0, 2, 0x25A0, cl); Draw(0, 2+strlen(o), 0x25A0, cl);
//		for(i=0; i<strlen(o); i++){
//			if(o[i]=='_') Draw(0, 1+i, brel[9], cl);
//			else Draw(0, 1+i, o[i], cl);
//		}
//
//		for(i=0; i<strlen(t); i++){
//			if(t[i]=='_') Draw(sx-1, 1+i, brel[9], cl);
//			else Draw(sx-1, 1+i, t[i], cl);
//		}
//	*/
//virtual int Read(contype ct);
//
//
//int Act(int act){ isact=act; DrawIt(); return 1; }
////
//void Clear(unsigned short cha, const MRGB&col, const MRGB&bg);
//void Clear(int x, int y, int tx, int ty, unsigned short cha, unsigned short col=65534, unsigned short bg=65534);
//void ClearLine(int y, unsigned short cha, unsigned short col=65534, unsigned short bg=65534);
//
//void DrawFull();
//virtual void DrawIt(){}
//
//DWORD COT(LPVOID lp){if(!cottid) return 0; return 1;}
//// color
//void setcol(const MRGB& col){ defc=col.sh(); }
//void setbgcol(const MRGB& col){ defbg=col.sh(); }
//
//bool GWGESet(VString text);
//MString WGGeGet();
//MString WGGeGetSafe();
//bool WGGeIs();
//
////void RunConsoleCommand(GlobalConsole &cons);
//
//void DestroyConsole(){ cottid=0; return ; }
//
//bool DrawMove(int x, int y, int cx, int cy, int t, int s){
//	memcpy(vbuf, vbuf+sx, (sx*(sy+bufext)-sx)*2);
//	memcpy(cbuf, cbuf+sx, (sx*(sy+bufext)-sx)*2);
//	memcpy(bbuf, bbuf+sx, (sx*(sy+bufext)-sx)*2);
//
//	memset(vbuf+sx*(sy+bufext)-sx, 0, sx*2); memset(cbuf+sx*(sy+bufext)-sx, 0, sx*2); memset(bbuf+sx*(sy+bufext)-sx, 0, sx*2);
//return 1;
//}
//
//// cursor
//int MoveCurPos(int x, int y){ curpos+=x+y*sx; if(curpos<0) curpos=0; if(curpos>sx*sy) curpos=sx*sy-1; return 1; }
//COORD GetCurPos(){ COORD cr; if(!sx || !sy){ cr.X=0; cr.Y=0; return cr; } cr.X=curpos%sx; cr.Y=curpos/sx; return cr; }
////int Clear(int x, int y){ curpos+=x+y*sx; if(curpos<0) curpos=0; if(curpos>sx*sy) curpos=sx*sy-1; return 1; }
//
//};
////#include "UMatrix.h"
//
//
//class ConsoleWindowP{ public: ConsoleWindow *w;
//inline ConsoleWindow* operator->(){ return w; }
//};
//
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//class CWEdit : public ConsoleWindow{
//public:
//MString _buf; unsigned short *buf, *tbuf, *rbuf, *key;
//bool ispass; int ln;
//
//CWEdit(){ _buf.Reserv(S1K); buf=(unsigned short*)_buf.rchar(); tbuf=buf; key=buf; rbuf=buf; ln=0; }
//
//void NewBuf(int sz){
//	MString bf; bf-=_buf;
//	_buf.Reserv(sz, 0); memcpy(_buf, bf, _buf.size()<bf.size() ? _buf.size(): bf.size());
//	buf=(unsigned short*)_buf.rchar();
//	key=buf+((unsigned short*)bf.rchar()-key);
//	rbuf=buf+((unsigned short*)bf.rchar()-rbuf);
//	tbuf=buf+((unsigned short*)bf.rchar()-tbuf);
//	Sleep(100);
//return ;
//}
//
//void Title(VString line){
//	int tsz=rbuf-tbuf, sz=line.size();//strlen(ttl)*2;
//if(_buf.size()/2<tsz+sz) NewBuf(_buf.size()+S1K);
//if(_buf.size()/2<tsz+sz) return ;
//memcpy(buf+sz, tbuf, tsz*2);
//for(int i=0; i<sz; i++){ buf[i]=stosr(line[i]); }
//key-=int(tbuf-buf)-line.size(); rbuf-=int(tbuf-buf)-line.size(); tbuf-=int(tbuf-buf)-line.size(); 
////memcpy(buf, ttl, sz);
//ReDraw();
//}
//
//void movekey();
//void ReDraw(){ if(sx>0){ while((key-buf)/sx>=ln+2) ln++; while((key-buf)/sx<ln) ln--; }
//	int i=x, k=0;// if(ln<0 || ln>S1M) return ;
//	for (k=0; k<cy; k++){
//		for (i=0; i<sx; i++){
//			if(rbuf-buf>(k+ln)*sx+i)
//			Draw(x+i, k, !ispass ? buf[(k+ln)*sx+i] : '*');
//			else Draw(x+i, k, ' ');
//		}
//	} movekey();
//}
//
//MString GetText(){ MString ret; if(buf) ret.set((char*)tbuf, (rbuf-tbuf)*2); rbuf=tbuf; key=tbuf; ReDraw(); return ret; }
//MString GetTextSafe(){ MString ret; if(buf) ret.set((char*)tbuf, (rbuf-tbuf)*2); return ret; }
//bool GetIs(){ return rbuf-tbuf; }
//
//int ReadLine(char*c, int sz){ contype ct; for(int i=0; i<sz; i++){ ct.SetKey(1, 0, c[i]); Read(ct); } return 1; }
//
//virtual int Read(contype ct){
//		if(ConsoleWindow::Read(ct)) return 1;
//		unsigned short c;
////
//		if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ c=ct.getcode();// print(itos(c)); print(".");
//	if(!(ct.getkeys()&CN_KEY)){
//
//	if(c==MK_BACKSPASE || c==127) { if(key>tbuf){// && key<=rbuf){
//			memcpy(key-1, key, (rbuf-key)*2); key--; rbuf--;// if(key==rbuf) *key=0;
//			//if(key!=rbuf) {ReDraw();} else { print(32); print(8);}
//	ReDraw();}
//	} else if(c==MK_ESCAPE) { if(rbuf>tbuf){ key=tbuf;
//		memset(tbuf, 0, (rbuf-tbuf)*2);// readinput_normal(buf, key, rbuf, ispass);
//		rbuf=tbuf; ReDraw();}
//	} else if(c==13 || c==10){return 1; //break; //		print("\r\n");
//	} else {
//		if(rbuf-tbuf>=_buf) NewBuf(_buf.size()+S1K); //break;
//		if(key!=rbuf) {
//			memcpy(key+1, key, (rbuf-key)*2); ReDraw(); //readinput_normal(buf, key, rbuf+1, ispass);			
//		} *(key)=c; rbuf++; key++; ReDraw(); //if(ispass) print("*"); else print(c);
//		//cinfo(cursz, 1);
//	}
//	} else {
//		if(c==MK_DELETE) { if(key>=buf && key<rbuf) {
//            memcpy(key, key+1, (rbuf-key-1)*2); rbuf--;
//			ReDraw(); //readinput_normal(buf, key, rbuf, ispass);
//		}}
//		else if(c==MK_LEFT) {if(key>tbuf) {
//			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
//			unsigned short *k=key; rrts_con(buf, rbuf-buf, key); movekey();
//			} else { key--; movekey();}
//		}}
//		else if(c==MK_RIGHT) {if(key<rbuf){
//			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
//			unsigned short *k=key; rts_con(buf, rbuf-buf, key); movekey();
//			} else { key++; movekey();}
//		}}
//		else if(c==MK_UP) {  }
//		else if(c==MK_DOWN) {  }
//		else if(c==MK_END) {  key=rbuf; movekey();  }
//		else if(c==MK_HOME) { key=tbuf; movekey(); }
//		}
//	}//}  return rbuf-buf;
////
//return 1;
//}
//
//
//};
//
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//class CWGSelectWindow : public ConsoleWindow{ public:
//
//		CWGSelectWindow(){ }
//
//	virtual int Pos(short _x, short _y, short _cx, short _cy){ ConsoleWindow::Pos(_x,_y,_cx,_cy); return 1; }
//
//
//	virtual void DrawIt();// DrawAsWindow();// VString d;
//	//int as=sx, p=sx/2;
//	//for(int i=0; i<s; i++){ Draw(i+p, 0, folder[i]); }
//
//	//for(int i=0; i<sy-2; i++){
//	//	if(dir.size()>i+pos) d=dir[i].key; else d.Empty(); int k=0, tk=d.size(); if(tk>sx-2) tk=sx-2;
//	//	for(k=0; k<tk; k++){ Draw(k+1, i+1, d[k], defc, (i+pos!=cur || !(isact&CNACT)) ? defbg : defs ); } for(k; k<sx-2; k++){ Draw(k+1, i+1, ' '); }
////	}
////}
//
//virtual int Read(contype ct){ // if 0 - next, if 1 - ok & end;
//	if(ConsoleWindow::Read(ct)) return 1;
//	if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ bool di=0;
//		if(ct.getkeys()&CN_KEY){
//		switch(ct.getcode()){
//			case MK_UP:  di=1; break;
//			//case MK_DOWN: if(cur<dir.size()-1) cur++; di=1; break;
//		}}
//	//	if(ct.getcode()==MK_ENTER) { RunIt(ct.getkeys()&CN_SHIFT); }
//
//		if(di) {DrawIt(); return 1;}
//	}
//return 0;
//}
//
//
//};
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//class CWGlobal: public ConsoleWindow{ public:
////MString _buf; unsigned short *buf, *key;
//CWEdit cne; CWGSelectWindow cns;
//
//CWGlobal(){
//	cne.gcon=gcon; cns.gcon=gcon;
//	COORD cd=TTYSize();
//	if(cd.X && cd.Y){
//	//	MString s=itos(cd.X)+"x"+itos(cd.Y);
//	//	write(0, s, s); exit(0);
//		Size(cd.X, cd.Y);}// printf("!!!!!!!!!!!!!"); Sleep(6000);}
//}//_buf.Reserv(S1K); buf=(unsigned short*)_buf.rchar(); key=buf; }//dcol=MRGB("00aa00").sh(); dbg=MRGB("00ff00").sh(); }
//
//	virtual int Size(int x, int y){ int r=sx*sy;
//		ConsoleWindow::Size(x,y); ReDraw();
//		//if(!r) LoadBanner(); 
//		return 1;
//	}
//
///*	void NewBuf(int sz){
//		MString bf; bf-=_buf;
//		_buf.Reserv(sz, 0); memcpy(_buf, bf, _buf.size()<bf.size() ? _buf.size(): bf.size()); buf=(unsigned short*)_buf.rchar(); key=buf+(unsigned short*)bf.rchar()-key; Sleep(100); 
//		return ;
//	}*/
//
//	void ReDraw(){ if(!sx || !sy) return ;
//		 cne.Pos(x, cy-2, cx, cy); cne.gcon=gcon;
//	}
///*	// Draw Menu =¶-„„¨¨¨LLL---¶¶¶¶¶¶TTT¶¶¶+++
//	Draw(0,0, brel[0]); Draw(sx-1, 0, brel[2]);
//    	for(int i=1; i<sx-1; i++) Draw(i, 0, brel[1]);
//		for(int i=0; i<sx; i++) Draw(i, sy-1, brel[7]);
//
//		for(int i=1; i<sy-2; i++) {Draw(0, i, brel[3]); Draw(sx-1, i, brel[5]); }
//	
//	// Menu 
//		char o[]="_F1-F4_  _F5_ _F6_", t[]="_F7-F9 _F10 _F11 _F12"; unsigned short cl=MRGB("00aa00").sh();
//		//Draw(0, 2, 0x25A0, cl); Draw(0, 2+strlen(o), 0x25A0, cl);
//		for(i=0; i<strlen(o); i++){
//			if(o[i]=='_') Draw(0, 1+i, brel[9], cl);
//			else Draw(0, 1+i, o[i], cl);
//		}
//
//		for(i=0; i<strlen(t); i++){
//			if(t[i]=='_') Draw(sx-1, 1+i, brel[9], cl);
//			else Draw(sx-1, 1+i, t[i], cl);
//		}
//	*/
//
//	virtual int Read(contype ct){
//		if(ConsoleWindow::Read(ct)) return 1;
//		//CONSOLE_SCREEN_BUFFER_INFO bi;
//		unsigned short c;
//		cne.Read(ct);
////
//		if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ c=ct.getcode();// print(itos(c)); print(".");
///*	if(!(ct.getkeys()&CN_KEY)){
//
//
//	if(c==MK_BACKSPASE || c==127) { if(key>buf){// && key<=rbuf){
//			memcpy(key-1, key, (rbuf-key)*2); print(8); key--; rbuf--;
//			if(key!=rbuf) {readinput_normal(buf, key, rbuf, ispass);} else { print(32); print(8);}}
//	} else if(c==MK_ESCAPE) { if(rbuf>buf){ _movekey(buf-key, bi); key=buf;
//		memset(buf, 0, (rbuf-buf)*2); readinput_normal(buf, key, rbuf, ispass);
//		rbuf=buf;}
//	} else if(c==13 || c==10){
//		print("\r\n"); return 1; //break;
//	} else {
//		if(rbuf-buf>=_buf) return 1;//break;
//		if(key!=rbuf) {
//			memcpy(key+1, key, (rbuf-key)*2); readinput_normal(buf, key, rbuf+1, ispass);			
//		} *(key)=c; rbuf++; key++; if(ispass) print("*"); else print(c);
//		cinfo(cursz, 1); }
//	} else {
//		if(c==MK_DELETE) { if(key>=buf && key<rbuf) {
//            memcpy(key, key+1, (rbuf-key-1)*2); rbuf--;
//			readinput_normal(buf, key, rbuf, ispass);
//		}}
//		else if(c==MK_LEFT) {if(key>buf) {
//			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
//			unsigned short *k=key; rrts_con(buf, rbuf-buf, key); _movekey(key-k, bi);
//			} else {_movekey(-1, bi); key--;}
//		}}
//		else if(c==MK_RIGHT) {if(key<rbuf){
//			if(ct.getkeys()&(CN_LCTRL|CN_RCTRL)) {
//			unsigned short *k=key; rts_con(buf, rbuf-buf, key); _movekey(key-k, bi);
//			} else {_movekey(1, bi); key++;}
//		}}
//		else if(c==MK_UP) {  }
//		else if(c==MK_DOWN) {  }
//		else if(c==MK_END) { _movekey(rbuf-key, bi); key=rbuf;  }
//		else if(c==MK_HOME) { _movekey(-(key-buf), bi); key=buf; }
//		}*/
//		}//}  return rbuf-buf;
//
////
//return 1;
//}
//
//
//	void LoadBanner(){
//		MString bn=LoadFile("/system/conf/hello_banner.conf"); Drawn(bn);
//		//for(int i=0; i<bn; i++){ Drawn( }
//	}
//
//DWORD COT(LPVOID lp){ReDraw();
//	while(1){ if(!cottid) return 0;
//	Sleep(100);
//
//	
//	}return 1;
//}
//
//
//};
//
//class MyHttp;
///////////////////////////////////////////////////////////////////////////////////////
/////// Global Consile ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//#define GCN_MAXWND	16
//class GlobalConsole{ public:
//COORD rect; BOOL doit;
//ConsoleWindowP wnd[GCN_MAXWND]; int sz;
//CWGlobal gw;
//
//#ifndef WIN32
//int linuxread;
//#endif
//
//// in out
//int inconsole, outconsole;
//TermoListen*tlin, *tlout;// MyHttpGC *tlhout;
////
//int inx, iny; unsigned short incol, inbgcol; short incurx, incury, incurp, inkeys;
//
//// int tabkey
//
//#ifdef WIN32
//#define CNIRSZ	4
//_INPUT_RECORD ir[CNIRSZ];
//#endif
//
//GlobalConsole(){
//#ifdef WIN32
//	SetConsoleCP(1251);
//#else 
//	linuxread=0;
//#endif
////	SetConsoleOutputCP(1251);
//	memset(wnd, 0, sizeof(wnd)); sz=0; doit=0;
//	inconsole=0; outconsole=0; tlin=tlout=0; /*tlhout=0;*/ rect.X=80; rect.Y=25;
//	NewWnd(&gw, 0, 1);
//
//	inx=0; iny=0; incol=65535; inbgcol=0; incurx=0; incury=0; incurp=25; inkeys=0;
//}
//
//~GlobalConsole(){
//	for(int i=0; i<sz; i++){ if(wnd[i]->isdel){ wnd[i]->cottid=0; Sleep(500); delete wnd[i].w; }}
//}
//
//void SetIn(TermoListen*tl){ tlin=tl; }
//void SetOut(TermoListen*tl){ tlout=tl; }
////void SetOut(MyHttpGC*tl){ tlhout=tl; }
////void SetCol(unsigned short col){ wnd[0]-> }
//
////ConsoleWindowLine wnd;
//
//bool NewWnd(ConsoleWindow *wind, int isdel, int repos=0){
//	for(int i=0; i<GCN_MAXWND; i++){
//		if(!wnd[i].w) { wnd[i].w=wind; sz++;
//		wnd[i]->gcon=this; wnd[i]->isdel=isdel; wnd[i]->Activate();
//		wnd[i]->isact=CNVIS;
//		if(repos) RePos();
//		return 1;}
//	}
////	ConsoleWindow &win=wnd.inserta(); win.Activate();
//return 0;
//}
//
/////
//bool Draw(ConsoleWindow*wind, unsigned short x, unsigned short y, unsigned short *cha, int chs, unsigned short col, unsigned short bg){ if(!wind) return 0;
//	 if(x<0 || y<0 || x>wind->cx || y>wind->cy) return 0;
//	x+=wind->x; y+=wind->y;
//
//for(int i=0; i<chs; i++){ // GENERAL FOR
//	short ch=cha[i];
//
//CHAR_INFO ci;
//// TLOUT
//if(tlout){
//	int r=wnd[0]->Draw_s(x, y, ch, col, bg);
//	if(!r) return 1;
//	if(x!=inx || y!=iny) { tlout->SendLocate(x, y); inx=x; iny=y; } // pos x,y
//	if(r&2) if(col!=incol){ tlout->SendColor(col); incol=col; } // color
//	if(r&4) if(bg!=inbgcol){ tlout->SendBgColor(bg); inbgcol=bg; }
//	if(r&1) tlout->SendUChar(ch);
//	inx++; if(inx>=rect.X){ inx=0; iny++; }
//return 1;
//}
//
///*/// TL HTTP OUT
//if(tlhout){
//	int r=wnd[0]->Draw_s(x, y, ch, col, bg);
//	if(!r) return 1;
//	if(x!=inx || y!=iny) { tlhout->SendLocate(x, y); inx=x; iny=y; } // pos x,y
//	if(r&2) if(col!=incol){ tlhout->SendColor(col); incol=col; } // color
//	if(r&4) if(bg!=inbgcol){ tlhout->SendBgColor(bg); inbgcol=bg; }
//	if(r&1) tlhout->SendUChar(ch);
//	inx++; if(inx>=rect.X){ inx=0; iny++; }
//return 1;
//}
///*/// TLOUT END
//
//if(!outconsole){// ch='A';
//ci.Char.UnicodeChar=ch; ci.Attributes=cicol(col, bg);
////if(!ci.Char.UnicodeChar) ci.Char.UnicodeChar=32;
////WriteConsoleOutputW(_std_output_handle, &ci, s, p, &sr);
//	int r=wnd[0]->Draw_s(x, y, ch, col, bg);
//	if(!r)
//		return 1;
//	if(x!=inx || y!=iny){ SetCurPos(x, y);/* inx=x; iny=y;*/ } 
//
//#ifdef WIN32
////if(x!=inx || y!=iny){ SetCurPos(x, y);/* inx=x; iny=y;*/ }
//	COORD s={1,1}, p={0,0}; SMALL_RECT sr={x,y,x,y};
//
//	if(r&7){
//		WriteConsoleOutputW(_std_output_handle, &ci, s, p, &sr);
//		inx++;
//		//ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
//		//write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
//	}
//
//	//inx++; //if(inx>=rect.X){ inx=0; iny++; }
//#else
////	if(cx!=x && cy!=y) set
//	//int r=wnd[0]->Draw_s(x, y, ch, col, bg);
//	//if(!r) return 1;
//	//if(x!=inx || y!=iny){ SetCurPos(x, y);/* inx=x; iny=y;*/ } // pos x,y
//	//if(r&2) if(col!=incol){ tlhout->SendColor(col); incol=col; } // color
//	//if(r&4) if(bg!=inbgcol){ tlhout->SendBgColor(bg); inbgcol=bg; }
//	//if(r&2)
//		if(col!=incol){ SetLinuxColor(col); incol=col; } // color
//	//if(r&4)
//		if(bg!=inbgcol){ SetLinuxBgColor(bg); inbgcol=bg; } // color
//	
//	if(r&7){ // sumbol
//		ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
//		write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
//		inx++;
//	}
//	//if(inx>=rect.X){ inx=0; iny++; }
////	if(ci.Char.AsciiChar==9)
////		inx+=8-inx%8;
////	else inx++;
////	if(inx>=rect.X){ inx=0; iny++; MoveCurPos(-inx, 1); }
//	//write(0, &ci.Char.UnicodeChar, ci.Char.UnicodeChar<256 ? 1 : 1);
//#endif
//}
//
//} // GENERAL FOR END
//return 1;
//}
//
//bool DrawFull(ConsoleWindow*wind){
//	int x=wind->x; int y=wind->y;
//	if(tlout /*|| tlhout*/) return 0;
//
//if(!outconsole){
//CHAR_INFO ci;
//for(int k=0; k<wind->sy; k++)
//for(int i=0; i<wind->sx; i++){
//	COORD s={1,1}, p={0,0}; SMALL_RECT sr={i+x,k+y,i+x,k+y};
//	ci.Char.UnicodeChar=wind->vbuf[i+wind->sx*k+wind->sx*wind->bufext];
//	if(!ci.Char.UnicodeChar) ci.Char.UnicodeChar=32;
//	ci.Attributes=cicol(wind->cbuf[i+wind->sx*k+wind->sx*wind->bufext], wind->bbuf[i+wind->sx*k+wind->sx*wind->bufext]);
//#ifdef WIN32
//	WriteConsoleOutputW(_std_output_handle, &ci, s, p, &sr); 
//#else
//
//	//int r=wnd[0]->Draw_s(i, k, ch, col, bg);
//	//if(!r) return 1;
//	if(i!=inx || k!=iny){ SetCurPos(i, k);/* inx=x; iny=y;*/ } // pos x,y
//	//SetCurPos(i, k);
//	//if(r&2) if(col!=incol){ tlhout->SendColor(col); incol=col; } // color
//	//if(r&4) if(bg!=inbgcol){ tlhout->SendBgColor(bg); inbgcol=bg; }
//	//if(r&1){
//		ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
//		write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
//		inx++;
//	//}
//
////		ci.Char.UnicodeChar=stoutf(ci.Char.UnicodeChar);
////		write(0, &ci.Char.UnicodeChar, 2); //tlhout->SendUChar(ch);
////		if(ci.Char.AsciiChar==9) inx+=8-inx%8; else inx++;
////		if(inx>=rect.X){ inx=0; iny++; MoveCurPos(-inx, 1); }
//	//write(0, &ci.Char.UnicodeChar, ci.Char.UnicodeChar<256 ? 1 : 2);
//#endif
//
//}}
//return 1;
//}
//
//bool DrawMove(ConsoleWindow*wind, int x, int y, int cx, int cy, int t, int s, int r=0){
//if(tlout){ tlout->SendMove(x, y, cx, cy, t, s); return 1; }	
////if(tlhout){ tlhout->SendMove(x, y, cx, cy, t, s); return 1; }	
//wind->DrawMove(x, y, cx, cy, t, s);
//DrawFull(wind); return 1;
//return 1;
//}
//
//
///////////////////////////////////////////////////////////////////////////////////////
//int MoveCurPos(int x, int y){
//COORD cr; cr.X=incurx+x; cr.Y=incury+y;
//	return SetCurPos(cr);
//}
///////////////////////////////////////////////////////////////////////////////////////
//COORD GetCurPos(){
//COORD cr; cr.X=incurx; cr.Y=incury;
//	return cr;
//}
///////////////////////////////////////////////////////////////////////////////////////
//int SetCurPos(int x, int y){ COORD cr={x, y}; return SetCurPos((cr)); }
///////////////////////////////////////////////////////////////////////////////////////
//int SetCurPos(COORD cd){ if(!this) return 0;
//// TLOUT
//if(tlout){
//	if(incurx!=cd.X ||incury!=cd.Y) tlout->SendCursor(cd.X, cd.Y); incurx=cd.X; incury=cd.Y; return 1;
//}
//// TL HTTP OUT
////if(tlhout){
////	if(incurx!=cd.X ||incury!=cd.Y) tlhout->SendCursor(cd.X, cd.Y); incurx=cd.X; incury=cd.Y; return 1;
////}
//
//if(!inconsole){
//	//if(incurx!=cd.X ||incury!=cd.Y)
//#ifdef WIN32
//	SetConsoleCursorPosition(_std_output_handle, cd);
///*
//	while(cd.X<0){ cd.Y--; cd.X+=rect.X; }
//	while(cd.X>=rect.X){ cd.Y++; cd.X-=rect.X; }
//    int mx=cd.X-inx; int my=cd.Y-iny;
//
//	gtx+=mx; gty+=my;
//	inx=cd.X; iny=cd.Y;
//	cd.X=gtx; cd.Y=gty;
//	SetConsoleCursorPosition(_std_output_handle, cd);
//*/
//#else
//	Itos it, itc;
//	while(cd.X<0){ cd.Y--; cd.X+=rect.X; }
//	while(cd.X>=rect.X){ cd.Y++; cd.X-=rect.X; }
//    int mx=cd.X-inx; int my=cd.Y-iny;
//
//	char d[]={27, '[', '0', 'C'}; //, ';', 0, '2', 'H',  	27, '['};
//	char nm[]="0123456789";
//
//	int m=mx<0 ? -1 : 1;
//	//new EF
//	//mx=cd.X;
//while(mx){
//	if(mx<0) it.Add(VString((char*)&d, 2), mx>-10 ? VString((char*)&nm[-mx], 1) : "9", VString("D", 1));
//	//, itc.itos(cd.X), "C");
//	else it.Add(VString((char*)&d, 2), mx<10 ? VString((char*)&nm[mx], 1) : "9", VString("C", 1));//, itc.itos(cd.X), "C");
//	if(mx) write(0, VString(it), it);
//	//mx-=min(mx, 9);
//	//mx-=min(mx<0 ? -mx : mx, 9)*m;
//	if(mx<0) mx+=mx>-10 ? -mx : 9; else mx-=mx<10 ? mx : 9;
//} inx=cd.X;
//
//	m=my<0 ? -1 : 1;
//while(my){
////	if(my<0) it.Add(VString((char*)&d, 2), itc.itos(min(-my, 9)), VString("F", 1));//, itc.itos(cd.X), "C");
////	else it.Add(VString((char*)&d, 2), itc.itos(min(mx, 9)), VString("E", 1));//, itc.itos(cd.X), "C");
//	if(my<0) it.Add(VString((char*)&d, 2), my>-10 ? VString((char*)&nm[-my], 1) : "9", VString("A", 1));//, itc.itos(cd.X), "C");
//	else it.Add(VString((char*)&d, 2), my<10 ? VString((char*)&nm[my], 1) : "9", VString("B", 1));//, itc.itos(cd.X), "C");
//	//else it.Add(VString((char*)&d, 2), "1", VString("E", 1));//, itc.itos(cd.X), "C");
//	write(0, VString(it), it);
//	if(my<0) my+=my>-10 ? -my : 9; else my-=my<10 ? my : 9;
//	//if(my<0) my+=min(-my, 9); else // else my-=min(mx, 9);
//	//	my+=max(-my, -9);
//	//my-=min(my<0 ? -my : my, 9)*m;
//} iny=cd.Y;
//
//// mode B>E, A>F
//
///*
//	char d[]={27, '[', '0', '1', ';', 0, '2', 'H', 
//		27, '['};
//	char d2[]={'B', 27, '['};
//	it.Add(VString((char*)&d, 10), itc.itos(cd.Y), VString((char*)&d2, 1));//, itc.itos(cd.X), "C");
//	//, '0'+cd.Y,  '0'+cd.X, 'B'};
//	write(0, VString(it), it);
//
//	//char d3[]={ 27, '[' };
//	if(cd.X){
//		it.Add(VString((char*)&d, 2), itc.itos(cd.X), "a"); //itc.itos(cd.X-cd.Y)
//			write(0, VString(it), it);
//	}*/
//
////	MString ms=itos(cd.X)+" "+itos(cd.Y);
////	write(0, ms, ms);
//#endif
//	}
//return 1;
//}
///////////////////////////////////////////////////////////////////////////////////////
//void SetLinuxColor(unsigned short col){
//char d[]={27, '[', '1', ';', '3', '1', 'm'};
////char d[]={27, '[', '2', ';', '3', 'x'};
////Esc[2;nx
//char c=cilcol(col); d[4]=stoihc(c);
//	write(0, &d, sizeof(d));
//return ;
//}
//
//void SetLinuxBgColor(unsigned short col){
//char d[]={27, '[', '1', ';', '1', '2', 'm'};
//char c=cilbgcol(col);// d[4]=stoihc(c);
//	write(0, &d, sizeof(d));
//return ;
//}
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//contype Read(){
//contype ct;
///*
//	if(readbufs>=2 && readbufs>=((contype*)readbuf)->getsz()){
//		int s=((contype*)readbuf)->getsz();
//		memcpy(&ct, readbuf, s); memcpy(readbuf, readbuf+s, readbufs-s);
//		return ct;
//	}*/
///////////////////////////////////////////////////////////////////////////////////////
//	if(!inconsole){
//#ifdef WIN32
//	DWORD rd; //	SHIFT_PRESSED
//	ReadConsoleInputW(_std_input_handle, ir, 1+0*CNIRSZ, &rd);
//	for(int i=0; i<rd; i++){
//		switch(ir[i].EventType){
//			case KEY_EVENT:
//				if(!ir[i].Event.KeyEvent.uChar.UnicodeChar) {ir[i].Event.KeyEvent.uChar.UnicodeChar=stos(ir[i].Event.KeyEvent.wVirtualKeyCode); ir[i].Event.KeyEvent.dwControlKeyState|=CN_KEY;}
//					//ir[i].Event.KeyEvent.uChar.UnicodeChar
//				ct.SetKey(ir[i].Event.KeyEvent.bKeyDown, ir[i].Event.KeyEvent.dwControlKeyState ,ir[i].Event.KeyEvent.uChar.UnicodeChar); break;
//			case MOUSE_EVENT:
//				ct.SetMouse(ir[i].Event.MouseEvent.dwControlKeyState, ir[i].Event.MouseEvent.dwEventFlags, ir[i].Event.MouseEvent.dwButtonState, ir[i].Event.MouseEvent.dwMousePosition); break;
//			case WINDOW_BUFFER_SIZE_EVENT:
//				ct.SetWindow(ir[i].Event.WindowBufferSizeEvent.dwSize); break;
//			case FOCUS_EVENT:
//				ct.SetFocus(ir[i].Event.FocusEvent.bSetFocus); break;
//}}
//#else
////		#include <curses.h>
//		if(linuxread && linuxread!=27 && linuxread!='['){ ct.SetKey(1, 0, linuxread); linuxread=0; return ct; }
//		int c=getc(stdin); if(c==10) c=13;
//		//print(itos(c), " ");
//		//ct.SetKey(1, 0, c);
//		
//		if(!linuxread && c==27) linuxread=27;
//		else if(linuxread==27 && c=='[') linuxread=c;
//		else if(linuxread=='['){ int t=0;
//		if(c=='A'){ c=MK_UP; t=CN_KEY; }
//		if(c=='B'){ c=MK_DOWN; t=CN_KEY; }
//		if(c=='C'){ c=MK_RIGHT; t=CN_KEY; }
//		if(c=='D'){ c=MK_LEFT; t=CN_KEY; }
//			ct.SetKey(1, t, c); linuxread=0;
//		}
//		else if(linuxread){ ct.SetKey(1, 0, linuxread); linuxread=c; }
//		else ct.SetKey(1, 0, c);
//		
//		//print(itos(c));
//		//printf( "%c", c);
//#endif
//	}
//return ct;
//}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//void ActIt(int nw=0){ int i=0; if(!sz) return ;
//for(i=1; i<sz; i++){
//	if(wnd[i]->isact&CNACT) { if(!nw) return ; wnd[i]->Act(wnd[i]->isact&(~CNACT)); i++; break; } }
//
//while(i<sz){ if(wnd[i]->isact&CNVIS) {wnd[i]->Act(wnd[i]->isact|CNACT); wnd[i]->DrawFull(); return ;} i++;}
//i=1;
//while(i<sz){ if(wnd[i]->isact&CNVIS) {wnd[i]->Act(wnd[i]->isact|CNACT); wnd[i]->DrawFull(); return ;} i++;}
//}
//
///////////////////////////////////////////////////////////////////////////////////////
//void Do(){doit=1; contype ct; int i;
//while(doit){
//	ct=Read();
//	if(tlin){ tlin->SendCT(ct); continue; }
//	for(i=0; i<sz; i++){
//		if(wnd[i]->isact&CNACT) if(wnd[i]->Read(ct)){ i=-1; break;} }
//	if(i!=-1) wnd[0]->Read(ct);
//	Sleep(20);
//}
//return ;
//}
///////////////////////////////////////////////////////////////////////////////////////
//void DoO(contype ct){ int i;
//	for(i=0; i<sz; i++){
//			if(wnd[i]->isact&CNACT) if(wnd[i]->Read(ct)){ i=-1; break;} }
//		if(i!=-1) wnd[0]->Read(ct);
//	return ;
//}
///////////////////////////////////////////////////////////////////////////////////////
//bool OnSize(unsigned short cx, unsigned short cy){
//	rect.X=cx; rect.Y=cy; RePos();
//return 1;
//}
///////////////////////////////////////////////////////////////////////////////////////
//void RePos(){ //gw.Pos(0, 0, rect.X, rect.Y);
//	wnd[0]->Pos(0, 0, rect.X, rect.Y);
//	if(sz==1){  }
//	else if(sz==2){
//		wnd[1]->Pos(0, 0, rect.X, rect.Y-2);
//	}
//	//else if(sz==3){
//	//	wnd[1]->Pos(0, 0, rect.X/2, rect.Y-2); wnd[2]->Pos(rect.X/2, 0, rect.X, rect.Y-2);
//	//}
//	ActIt(sz-1); 
//	if(sz>2) { wnd[sz-1]->Pos(0, 0, rect.X, rect.Y-2); wnd[sz-1]->DrawFull(); }
//}
///////////////////////////////////////////////////////////////////////////////////////
//void RePosTwoWnd(){ //gw.Pos(0, 0, rect.X, rect.Y);
//	wnd[0]->Pos(0, 0, rect.X, rect.Y);
//	if(sz==1){  }
//	else if(sz==2){
//		wnd[0]->Pos(0, 0, rect.X/2, rect.Y-2); wnd[1]->Pos(rect.X/2, 0, rect.X, rect.Y-2);
//	//	wnd[1]->Pos(0, 0, rect.X, rect.Y-2);
//	}
//	else if(sz==3){
//		wnd[1]->Pos(0, 0, rect.X/2, rect.Y-2); wnd[2]->Pos(rect.X/2, 0, rect.X, rect.Y-2);
//	}
//	ActIt(sz-1); 
//	//if(sz>2) { wnd[sz-1]->Pos(0, 0, rect.X, rect.Y-2); wnd[sz-1]->DrawFull(); }
//}
///////////////////////////////////////////////////////////////////////////////////////
//WORD cicol(unsigned short c, unsigned short b){
//WORD ret=0;// int z=0;
//int t=c&0x1f; if(t>=32/3) ret|=FOREGROUND_BLUE; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
//t=(c&0x7e0)>>5; if(t>=64/3) ret|=FOREGROUND_GREEN; if(t>=64*2/3) ret|=FOREGROUND_INTENSITY;t=(c&0x1f800)>>11; if(t>=32/3) ret|=FOREGROUND_RED; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
////if(z>=64) ret|=FOREGROUND_INTENSITY; z=0;
//
//t=b&0x1f; if(t>=32/3) ret|=BACKGROUND_BLUE; if(t>=32*2/3) ret|=BACKGROUND_INTENSITY;
//t=(b&0x7e0)>>5; if(t>=64/3) ret|=BACKGROUND_GREEN; if(t>=64*2/3) ret|=BACKGROUND_INTENSITY;
//t=(b&0x1f800)>>11; if(t>=32/3) ret|=BACKGROUND_RED; if(t>=32*2/3) ret|=BACKGROUND_INTENSITY;//if(z>=64) ret|=BACKGROUND_INTENSITY; z=0;
//
//ret|=COMMON_LVB_LEADING_BYTE|COMMON_LVB_TRAILING_BYTE|COMMON_LVB_GRID_HORIZONTAL|COMMON_LVB_UNDERSCORE;
//return ret;
//}
///////////////////////////////////////////////////////////////////////////////////////
//WORD cilcol(unsigned short c){
//WORD ret=0;// int z=0;
//int t=c&0x1f; if(t>=32/3) ret|=FOREGROUND_BLUE; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
//t=(c&0x7e0)>>5; if(t>=64/3) ret|=FOREGROUND_GREEN; if(t>=64*2/3) ret|=FOREGROUND_INTENSITY;t=(c&0x1f800)>>11; if(t>=32/3) ret|=FOREGROUND_RED; if(t>=32*2/3) ret|=FOREGROUND_INTENSITY;
////if(z>=64) ret|=FOREGROUND_INTENSITY; z=0;
//return ret;
//}
//
//WORD cilbgcol(unsigned short c){
//WORD ret=0; int r, g, b;// int z=0; 31.63.31
//b=c&0x1f; g=(c&0x7e0)>>5; r=(c&0x1f800)>>11;
//
//if(b<8 && g<16 && r<8) return 0;
//if(b>=8 && g<16 && r<8) return 4; // blue
//if(b<8 && g>=16 && r<8) return 2; // green
//if(b<8 && g<16 && r>=8) return 1; // red
//
//if(b>=8 && g>=16 && r<8) return 6; // cyan??
//if(b<8 && g>=16 && r>=8) return 3; // yellow
//if(b>=8 && g<16 && r>=8) return 5; // magenta
////if(b<8 && g<16 && r<8) return 0;
//return 7;
//}
//
//};
///////////////////////////////////////////////////////////////////////////////////////
/////// ConsoleWindow::Draw ///////////////////////////////////////////////////////////
//void ConsoleWindow::Draw(int x, int y, unsigned short c, unsigned short col, unsigned short bg){
//	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return ; if(col==65534) col=defc; if(bg==65534) bg=defbg;
//UGLock(*this);
//	//if(vbuf[x+(y+bufext)*sx]!=c || cbuf[x+(y+bufext)*sx]!=col || bbuf[x+(y+bufext)*sx]!=bg)
//	{
// 		vbuf[x+(y+bufext)*sx]=c; cbuf[x+(y+bufext)*sx]=col; bbuf[x+(y+bufext)*sx]=bg;
//		gcon->Draw(this, x, y, &c, 1, col, bg);
//	}
//	return ;
//}
///*
//void ConsoleWindow::Draw(int x, int y, VString line, unsigned short col, unsigned short bg){
//	if(x<0 || x>=sx || y<0 || y>=sy || !vbuf || !gcon || !bbuf) return ; if(col==65534) col=defc; if(bg==65534) bg=defbg;
//UGLock(*this);
//	//if(vbuf[x+(y+bufext)*sx]!=c || cbuf[x+(y+bufext)*sx]!=col || bbuf[x+(y+bufext)*sx]!=bg)
//	{
// 		//vbuf[x+(y+bufext)*sx]=c; cbuf[x+(y+bufext)*sx]=col; bbuf[x+(y+bufext)*sx]=bg;
//		gcon->Draw(this, x, y, c, col, bg);
//	}
//	return ;
//}*/
//
///*
//void ConsoleWindow::Draw(int x, int y, unsigned short c, int sz, unsigned short col, unsigned short bg){
//	if(x<0 || x>=sx || y<0 || y>=sy) return ; if(col==65534) col=defc; if(bg==65534) bg=defbg; UGLock(*this);
//	for(int i=0; i<sz; i++){
//		vbuf[x+y*sx]=c; cbuf[x+y*sx]=col; bbuf[x+y*sx]=bg;
//		gcon->Draw(this, x, y, c, col, bg);
//	x++; if(x>sx) {y++; x=0;}
//	}
//	return ;
//}*/
//
//
//void ConsoleWindow::Draw(int x, int y, const VString line, unsigned short col, unsigned short bg){
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	unsigned short ca[32]; int cs=0; curpos=x+y*sx;
//	for(int i=0; i<line.sz; i++){ ca[cs]=stosr(line[i]); cs++; if(cs>=32){ Drawn((unsigned short*)&ca, cs, col, bg); cs=0; }}
//	if(cs) Drawn((unsigned short*)&ca, cs, col, bg);
//}
//
//void ConsoleWindow::Drawn(const VString line, unsigned short col, unsigned short bg){
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	unsigned short ca[32]; int cs=0;
//	for(int i=0; i<line.sz; i++){ ca[cs]=stosr(line[i]); cs++; if(cs>=32){ Drawn((unsigned short*)&ca, cs, col, bg); cs=0; }}
//	if(cs) Drawn((unsigned short*)&ca, cs, col, bg);
//}
///*
//void ConsoleWindow::Drawn(char*line, unsigned short col, unsigned short bg){
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	unsigned short ca[32]; int cs=0;
//	for(int i=0; i<strlen(line); i++){ ca[cs]=stosr(line[i]); cs++; if(cs>=32){ Drawn((unsigned short*)&ca, cs, col, bg); cs=0; }}
//	if(cs) Drawn((unsigned short*)&ca, cs, col, bg);
//}*/
//
//void ConsoleWindow::Drawn(unsigned short *ca, int cs, unsigned short col, unsigned short bg){ UGLock(*this);
//	if(col==65534) col=defc; if(bg==65534) bg=defbg; if(!sx || !sy) return ;
//
//for(int i=0; i<cs; i++){ // GENERAL FOR
//	unsigned short c=ca[i];
//
//	short x=curpos%sx, y=curpos/sx;
//	if(c==13) {curpos-=x; }
//	else if(c==10) {curpos+=sx; }
//	else if(c==9)// && (curpos%sx)%8)
//	{ x+=8-(curpos%sx)%8; curpos+=8-(curpos%sx)%8;} // no return's
//	
//	if(x>=sx) {x=0; y++;} if(y>=sy){
//		y--; curpos-=sx;
////	memcpy(vbuf, vbuf+sx, (sx*(sy+bufext)-sx)*2);
////	memcpy(cbuf, cbuf+sx, (sx*(sy+bufext)-sx)*2);
////	memcpy(bbuf, bbuf+sx, (sx*(sy+bufext)-sx)*2);
////	memset(vbuf+sx*(sy+bufext)-sx, 0, sx*2); memset(cbuf+sx*(sy+bufext)-sx, 0, sx*2); memset(bbuf+sx*(sy+bufext)-sx, 0, sx*2);
//	//gcon->DrawFull(this);
//	gcon->DrawMove(this, x, y, sx, sy, 0, 1);
//	}
//
//	if(c==13 || c==10 || c==9){
//		int zx=x+this->x, zy=y+this->y; gcon->SetCurPos(zx, zy);		
//		return ;
//	}
//
//	vbuf[x+(y+bufext)*sx]=c; cbuf[x+(y+bufext)*sx]=col; bbuf[x+(y+bufext)*sx]=bg;	
//	gcon->Draw(this, x, y, &c, 1, col, bg);
//	curpos++;
//
//} //	GENERAL FOR END
//return ;
//}
//
//void ConsoleWindow::Clear(unsigned short cha, const MRGB&col, const MRGB&bg){ 
//	unsigned short c=col.sh(), b=bg.sh();
//	for(int i=0; i<sx; i++)
//		for(int k=0; k<sy; k++){Draw(i, k, cha, c, b); }
//	gcon->DrawFull(this);
//}
//
//void ConsoleWindow::Clear(int x, int y, int tx, int ty, unsigned short cha, unsigned short col, unsigned short bg){ 
//	maxel(x,0); maxel(y,0); minel(tx,sx); minel(ty,sy);
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	for(int i=x; i<=tx; i++)
//		for(int k=y; k<=ty; k++){ Draw(i, k, cha, col, bg); }
//	//gcon->DrawFull(this);
//}
//
//void ConsoleWindow::ClearLine(int y, unsigned short cha, unsigned short col, unsigned short bg){ 
//	int x=0, tx=sx;// maxel(y,0); minel(ty,sy);
//	if(col==65534) col=defc; if(bg==65534) bg=defbg;
//	for(int i=x; i<=tx; i++)
//		{ Draw(i, y, cha, col, bg); }
//		//gcon->Draw(this, x, y, );
//}
//
//void ConsoleWindow::DrawFull(){ if(!gcon) return ; GWGESet(cmdt); gcon->DrawFull(this); return ; }
//
//bool ConsoleWindow::GWGESet(VString text){ if(!gcon) return 0;
//	gcon->gw.cne.Title(text);
//	return 1;
//}
//
//MString ConsoleWindow::WGGeGet(){ if(!gcon) return 0;
//	return gcon->gw.cne.GetText();
//}
//
//MString ConsoleWindow::WGGeGetSafe(){ if(!gcon) return 0;
//	return gcon->gw.cne.GetTextSafe();
//}
//
//bool ConsoleWindow::WGGeIs(){ if(!gcon) return 0;
//	return gcon->gw.cne.GetIs();
//}
//
//int ConsoleWindow::Read(contype ct){ // if 0 - next, if 1 - ok & end;
//	if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){
//		if(ct.getcode()==MK_TAB && ct.getkeys()&(CN_RCTRL|CN_LCTRL)) {gcon->ActIt(1); return 1;}
//	}
//return 0;
//}
//
//void CWEdit::movekey(){ if(x<-S16K || x>S16K) return ;
//if(sx>0){ bool m=0; while((key-buf)/sx>=ln+2){ ln++; m=1;} while((key-buf)/sx<ln) {m=1; ln--;} if(m) return ReDraw(); }
//COORD coord; coord.X=x+(key-buf)%sx; coord.Y=y+(key-buf)/sx-ln; gcon->SetCurPos(coord);
//}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//class CWCommandLine: public ConsoleWindow{ public:
////MString folder; Readdir dir;
//int tabs;
//Readdir dir; MString folder, sel;
//
//	CWCommandLine(){ folder=GetThisPath(); bufext=100; Size(x,y); tabs=0;}
//
//	int Pos(short _x, short _y, short _cx, short _cy){ ConsoleWindow::Pos(_x,_y,_cx,_cy); return 1; }
//
//	void RunConsoleCommand(GlobalConsole &cons);
//
//DWORD COT(LPVOID lp){ //Clear('i', "ff0000", ""); Drawn("Testing...\r\n");// int testtime=1000;
//	Sleep(2000);
//
//// Testing mess;
////	for(int i=0; i<30; i++) { Drawn(".\r\n"); Sleep(1);}
////	Drawn("\r\n\r\n\r\n\r\n\r\n");
//	Drawn("Welocme to LL aka Tentacle :)\r\n");
//	Drawn("You system cache: ");
//	Drawn(clientunicach.GetCach());
//	Drawn("\r\n\r\n"); Drawn("Enter command.\r\n\r\n\r\n\r\n\r\n");
//	//for(int i=0; i<12; i++){ Drawn("\r\n"); Sleep(100); }
//
//cmdt=folder+">"; GWGESet(cmdt);
//
//	/*if(testtime>0){
//	int a=rand()%S1G, b=rand()%S1G;
//	MString ret=itos(a)+itos(b)+itos(a*b)+"\r\n";
//	for(int i=0; i<ret.size(); i++){
//	Drawn(ret[i]); Sleep(10);
//	}	testtime--; if(!testtime) Drawn("...done\r\n");  }*/
//
//	while(1){ if(!cottid) return 0;
//		Sleep(100);
//	}return 1;
//}
//
///////
//virtual int Read(contype ct){ // if 0 - next, if 1 - ok & end;
//	if(ConsoleWindow::Read(ct)) return 1;
//	if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ bool di=0;
////		if(ct.getkeys()&CN_KEY){
////		switch(ct.getcode()){
////			case MK_UP: if(cur>0) cur--; di=1; break;
////			case MK_DOWN: if(cur<dir.size()-1) cur++; di=1; break;
////		}}
//		if(ct.getcode()==MK_ENTER) { RunIt(); }
//		if(ct.getcode()==MK_TAB) { RunTab(); di=1; tabs++;}
//
//		if(di) {DrawIt(); return 1;}
//	}
//return 0;
//}
//////////
//	//
//
//	int RunIt(){ DrawIt(); RunConsoleCommand(*gcon); return 1; }
//	int RunTab(){ int did=-1; MString nm=unistoutf(WGGeGetSafe()); HLString ls; // VString f;
//		char *nmc=nm.end(); if(nm.end()) for(nmc; nmc>=nm.rchar(); nmc--){ 
//			if(*nmc==' '){ nm.set(nmc+1, nm.end()-nmc-1); break; }
//		} if(nm.is() && *(nm.end()-1)==':') nm+="/";
//
//		nmc=nm.end()-1; if(nm.end()) for(nmc; nmc>=nm.rchar(); nmc--){ if(*nmc=='/') break; }
//		VString m; m.set(nmc+1, nm.end()-nmc-1);
//
//		char tmp[S1K]; int tmpi=0;
//		Readdir dird; ILink il; il.Link(nm); ls+nm;
//		//if(!il.ext) ls+"*.*"; else 
//		ls+"*"; FindFile(ls, &dird); 
//		ls.Clear();
//		
//		for(int i=0; i<dird.size(); i++){
//			if(dird[i].key.incompare(m) ){
//				if(tabs==1 && m!="." && m!="..")
//				if(did==-1){ did=i; tmpi=dird[i].key; if(tmpi>S1K) tmpi=S1K; memcpy(tmp, dird[i].key, tmpi); }
//				else{ tmpi=dird[i].key.comparesz(tmp, tmpi); if(!tmpi) did=S2GM; }				
//			if(tabs>1) ls+dird[i].key+"  \t";
//			}
//		}
//
//		if(did>=0 && did<S2GM && tmpi){ gcon->gw.cne.ReadLine(tmp+m.size(), tmpi-m.size()); tabs=0; }
//		if(tabs>1){ Drawn(VString(ls.oneline(), ls.size())); Drawn("\r\n"); tabs=0; }
//		return 1;
//	}
//
//
//};
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//class CWFileSystem: public CWCommandLine{ //ConsoleWindow{
//public:
//#define CN_VTYPE_STD	1
//	int pos, cur, vtype;
//	//Readdir dir; MString folder, sel;
//
//	CWFileSystem(){ folder=GetThisPath(); vtype=CN_VTYPE_STD; }
//	virtual int Pos(short _x, short _y, short _cx, short _cy){ ConsoleWindow::Pos(_x,_y,_cx,_cy); ReDir(); return 1; }
//
//
//	void ReDir(){
//		dir.ReadDir(folder); dir.Resort();
////		FindFile(folder+"*", &dir);
//		pos=0; cur=0;
//		if(sel) for(int i=0; i<dir.size(); i++){ if(dir[i].key==VString(sel)) {cur=i; break;} } sel.Empty();
//		cmdt=folder+">"; GWGESet(cmdt); DrawIt();
//	}
//
//	virtual void DrawIt(){ DrawAsWindow(); VString d;
//	int s=folder.size(), as=sx, p=(sx-s)/2; int pi;
//	//for(int i=0; i<s; i++){ Draw(i+p, 0, folder[i]); }
//	Draw(p, 0, folder);
//	if(cur<pos) pos=cur; if(cur>=pos+sy-3) pos=cur-(sy-3)+1;
//
//	for(int i=0; i<sy-3; i++){ pi=i+pos;
//		if(dir.size() && dir[pi].key==".") { if(cur==pi) cur=pi+1; continue;}
//		if(dir.size()>pi) d=dir[pi].key; else d.Empty(); int k=0, tk=d.size(); if(tk>sx-2) tk=sx-2;
//		//for(k=0; k<tk; k++){ // draw name & free		
//		//	Draw(k+1, i+1, d[k], defc, (pi!=cur || !(isact&CNACT)) ? defbg : defs ); }
//		Draw(1, i+1, d, defc, (pi!=cur || !(isact&CNACT)) ? defbg : defs );
//		for(k=d.size(); k<sx-2; k++){ Draw(k+1, i+1, ' '); }
//		// draw ext & info
//		if(cur==pi){ Itos it; VString v="<dir>"; if(dir.size()>pi && !dir[pi].isdir()) v=it.itos(dir[pi].size());
//			for(k=0; k<tk; k++){ Draw(k+1, sy-2, d[k], defc, defbg); } tk+=2;
//			for(k; k<tk; k++){ Draw(k+1, sy-2, ' ', defc, defbg); } tk+=v.size();
//			for(k; k<tk; k++){ Draw(k+1, sy-2, v[v.size()-(tk-k)], defc, defbg); }	
//			for(k; k<sx-2; k++){ Draw(k+1, sy-2, ' ', defc, defbg); }
//		}
//	}
//}
//
//virtual int Read(contype ct){ // if 0 - next, if 1 - ok & end;
//	if(ConsoleWindow::Read(ct)) return 1;
//	if(ct.gett()==CNTKEY && ct.getkeys()&CN_DOWN){ int _cur=cur;
//		if(ct.getkeys()&CN_KEY){
//		switch(ct.getcode()){
//			case MK_UP: if(cur>0) cur--; break;
//			case MK_DOWN: if(cur<dir.size()-1) cur++; break;
//			case MK_HOME: cur=0; break;
//			case MK_END: cur=dir.size()-1; break;
//			case MK_PGUP: cur-=sy-3; if(cur<0) cur=0; break;
//			case MK_PGDOWN: cur+=sy-3; if(cur>dir.size()-1) cur=dir.size()-1; break;
//		}}
//		if(ct.getcode()==MK_ENTER) { RunIt(ct.getkeys()&CN_SHIFT); }
//
//		if(_cur!=cur){ DrawIt(); return 1; }
//	}
//return 0;
//}
//
//virtual int RunIt(int sh=0){
//	if(WGGeIs()){ RunConsoleCommand(*gcon); return 1; }
//	if(cur<0 || cur>=dir.size() ) {ReDir(); return 1;}
//	VString vs=dir[cur].key;
//	if(vs=="." || !vs || !dir[cur].isdir()) return 1;
//	if(vs=="..") { //if(sh) {ShellExecute(0, "open", GetRealPath(folder), 0, 0, 1); return 1;}
//	LevelDown(); return 1; }
//
////if(sh || !dir[cur].isdir()) {ShellExecute(0, "open", GetRealPath(folder+MString(dir[cur].key)), 0, 0, 1); return 1; }
//
//	LevelUp(vs);
//return 1;
//}
//
//void LevelDown(){
//	unsigned char*ch=(unsigned char*)folder.end()-2; if(!folder) return;
//	while(ch>=folder.uchar()){
//		if(*ch=='/' || *ch=='\\'){
//			if(ch>=folder.uchar()){
//				sel.set(ch+1, (unsigned char*)folder.end()-ch-2);
//				folder.resize(ch-folder.uchar()+1); *folder.end()=0;}
//			else folder="/"; break;
//		} ch--;
//	} //if(!set) sel.set(ch+1, folder.end()-ch-2);
//		ReDir();
//}
//
//void LevelUp(VString lp){
//folder=(char*)(HLString()+folder+lp+SSLASH); ReDir();
//}
//
//DWORD COT(LPVOID lp){// Clear('i', "ff0000", ""); Drawn("Testing...\r\n"); int testtime=1000;
//unsigned short us=0;
//	while(1){ if(!cottid) return 0;
//	Sleep(100);
//
//	//if(testtime>0){
//	//int a=rand()%S1G, b=rand()%S1G;
//	//MString ret=itos(a)+itos(b)+itos(a*b)+"\r\n";
//	//for(int i=0; i<ret.size(); i++){
//	//Drawn(us++);// Sleep(10);
//	//}	testtime--; if(!testtime) Drawn("...done\r\n");  }
//	
//	}
//	return 1;
//}
//
//
//};
//
//
//void CWGSelectWindow::DrawIt(){
//	Drawn("111111112222222222223333333344444444");
//return ;
//}


	contype ConsoleRead(unsigned int &linuxread, unsigned int &linuxread_esc){
		contype ct;
	/*
		if(readbufs>=2 && readbufs>=((contype*)readbuf)->getsz()){
			int s=((contype*)readbuf)->getsz();
			memcpy(&ct, readbuf, s); memcpy(readbuf, readbuf+s, readbufs-s);
			return ct;
		}*/
	/////////////////////////////////////////////////////////////////////////////////////
		if(1){
#ifdef WIN32
		DWORD rd; //	SHIFT_PRESSED
		_INPUT_RECORD ir[1];

		/*if(nowaitcon){*/ int i=GetNumberOfConsoleInputEvents(_std_input_handle, &rd); if(i && !rd) return ct;// }
		ReadConsoleInputW(_std_input_handle, ir, 1, &rd);
		for(int i=0; i<rd; i++){
			switch(ir[i].EventType){
				case KEY_EVENT:
					if(!ir[i].Event.KeyEvent.uChar.UnicodeChar){ ir[i].Event.KeyEvent.uChar.UnicodeChar=ctos(ir[i].Event.KeyEvent.wVirtualKeyCode); ir[i].Event.KeyEvent.dwControlKeyState|=CN_KEY; }
						//ir[i].Event.KeyEvent.uChar.UnicodeChar
					ct.SetKey(ir[i].Event.KeyEvent.bKeyDown, ir[i].Event.KeyEvent.dwControlKeyState ,ir[i].Event.KeyEvent.uChar.UnicodeChar); break;
				case MOUSE_EVENT:
					ct.SetMouse(ir[i].Event.MouseEvent.dwControlKeyState, ir[i].Event.MouseEvent.dwEventFlags, ir[i].Event.MouseEvent.dwButtonState, ir[i].Event.MouseEvent.dwMousePosition); break;
				case WINDOW_BUFFER_SIZE_EVENT:
					ct.SetWindow(ir[i].Event.WindowBufferSizeEvent.dwSize); break;
				case MENU_EVENT:{
					int i=456;
					break;}
				case FOCUS_EVENT:
					ct.SetFocus(ir[i].Event.FocusEvent.bSetFocus); break;
			}
		}
		return ct;
//#else
#endif
//		#include <curses.h>
		int c;
		if(!ifrecv(0, 0)) return ct;

		if(!linuxread_esc && linuxread){ c=linuxread; linuxread=0; }
		else{ c=0; read(0, &c, 1); } // c=getc(stdin);
		if(c==10) c=13;

		if(linuxread_esc==3){
			if((linuxread&224)==192 && (c&192)==128){
				c=((linuxread&31)<<6)+(c&63);
				ct.SetKey(1, 0, c); linuxread=0; linuxread_esc=0;
			}
			else {
				ct.SetKey(1, 0, linuxread); linuxread=c;
				if((c&224)!=192) linuxread_esc=0;
			}
			return ct;
		}

		//if(linuxread>1<<24){ linuxread_esc=0; linuxread=0; }

		if(!linuxread_esc && c==27){
			if(!ifrecv(0, 0)){ ct.SetKey(1, 0, 27); return ct; }
			linuxread_esc=1; linuxread=0; return ct;
		}
		else if(linuxread_esc==1){
			if(c=='['){ linuxread_esc=2; return ct; }
			else{ ct.SetKey(1, 0, 27); linuxread_esc=0; linuxread=c; return ct; }
		}
		else
			if(linuxread_esc==2){
				linuxread=(linuxread<<8)+c;

				// virtualcode
				int ret=1;
				switch(linuxread){
					// level one
					// case '[': ct.SetKey(1, 0, 27); break;
	//					case 'O': ctrl=CN_RCTRL; break;

					// level two
					case 'A': ct.SetKey(1, CN_KEY, MK_UP); break;
					case 'B': ct.SetKey(1, CN_KEY, MK_DOWN); break;
					case 'C': ct.SetKey(1, CN_KEY, MK_RIGHT); break;
					case 'D': ct.SetKey(1, CN_KEY, MK_LEFT); break;

					// level three
					case ('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_HOME); break;
					case ('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_DELETE); break;
					case ('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_END); break;

					// level four
					case ('1'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F1); break;
					case ('1'<<16)+('2'<<8)+126: ct.SetKey(1, CN_KEY, MK_F2); break;
					case ('1'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F3); break;
					case ('1'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F4); break;
					case ('1'<<16)+('5'<<8)+126: ct.SetKey(1, CN_KEY, MK_F5); break;
					case ('1'<<16)+('7'<<8)+126: ct.SetKey(1, CN_KEY, MK_F6); break;
					case ('1'<<16)+('8'<<8)+126: ct.SetKey(1, CN_KEY, MK_F7); break;
					case ('1'<<16)+('9'<<8)+126: ct.SetKey(1, CN_KEY, MK_F8); break;
					case ('2'<<16)+('0'<<8)+126: ct.SetKey(1, CN_KEY, MK_F9); break;
					case ('2'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F10); break;
					case ('2'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F11); break;
					case ('2'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F12); break;
					default: ret=0; break;
				}
				if(ret){ linuxread=0; linuxread_esc=0; return ct; }
				//linuxread<<=8; linuxread=c;
				if(c==126){ linuxread=0; linuxread_esc=0; return ct; }
			 }

		if(!linuxread_esc){
			if((c&224)==192){ linuxread_esc=3; linuxread=c; }
			else ct.SetKey(1, 0, c);
		}
		else
			ct.SetKey(1, 0, c);
		
		return ct;


		//if(linuxread_esc && !linuxread && c!='['){ ct.SetKey(1, 0, 27); linuxread_esc=0; linuxread=c; return ct; }

	
		//if(linuxread_esc==2){
		//	if((linuxread&224)==192 && (c&192)==128){
		//		c=((linuxread&31)<<6)+(c&63);
		//		ct.SetKey(1, 0, c); linuxread=0; linuxread_esc=0;
		//	}
		//	else { ct.SetKey(1, 0, linuxread); linuxread=c; }
		//	return ct;
		//}

		//if(c==27) print("[c:", itos(c), "-", "^[", "]");
		//else print("[c:", itos(c), "-", VString((char*)&c, 1), "]");

		

		if(linuxread_esc){
			linuxread=linuxread<<8+c;
			print("[CODE:", itos(linuxread), "]"); Sleep(1000);

			// virtualcode
			int ret=1;
			switch(linuxread){
				// level one
				// case '[': ct.SetKey(1, 0, 27); break;
//					case 'O': ctrl=CN_RCTRL; break;

				// level two
				case ('['<<8)+'A': ct.SetKey(1, CN_KEY, MK_UP); print("[MK_UP]"); break;
				case ('['<<8)+'B': ct.SetKey(1, CN_KEY, MK_DOWN); break;
				case ('['<<8)+'C': ct.SetKey(1, CN_KEY, MK_RIGHT); break;
				case ('['<<8)+'D': ct.SetKey(1, CN_KEY, MK_LEFT); break;

				// level three
				case ('['<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_HOME); print("[MK_HOME]"); break;
				case ('['<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_DELETE); print("[MK_DELETE]"); break;
				case ('['<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_END); print("[MK_END]"); break;

				// level four
				case ('['<<24)+('1'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F1); print("[MK_F1]"); break;
				case ('['<<24)+('1'<<16)+('2'<<8)+126: ct.SetKey(1, CN_KEY, MK_F2); print("[MK_F2]"); break;
				case ('['<<24)+('1'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F3); print("[MK_F3]"); break;
				case ('['<<24)+('1'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F4); break;
				case ('['<<24)+('1'<<16)+('5'<<8)+126: ct.SetKey(1, CN_KEY, MK_F5); break;
				case ('['<<24)+('1'<<16)+('7'<<8)+126: ct.SetKey(1, CN_KEY, MK_F6); break;
				case ('['<<24)+('1'<<16)+('8'<<8)+126: ct.SetKey(1, CN_KEY, MK_F7); break;
				case ('['<<24)+('1'<<16)+('9'<<8)+126: ct.SetKey(1, CN_KEY, MK_F8); break;
				case ('['<<24)+('2'<<16)+('0'<<8)+126: ct.SetKey(1, CN_KEY, MK_F9); break;
				case ('['<<24)+('2'<<16)+('1'<<8)+126: ct.SetKey(1, CN_KEY, MK_F10); break;
				case ('['<<24)+('2'<<16)+('3'<<8)+126: ct.SetKey(1, CN_KEY, MK_F11); break;
				case ('['<<24)+('2'<<16)+('4'<<8)+126: ct.SetKey(1, CN_KEY, MK_F12); break;
				default: ret=0; break;
			}
			if(ret){ linuxread=0; linuxread_esc=0; return ct; }
			//linuxread<<=8; linuxread=c;
			if(c==126){ linuxread=0; return ct; }
		}

		

		//if((c&224)==192){ linuxread_esc=2; linuxread=c; }
		//else
			ct.SetKey(1, 0, c);

		return ct;


			
			if(linuxread && linuxread!=27 && (linuxread&72448)!=6912 && (linuxread&1792768)!=1792768
				&& (linuxread&224)!=192){
				ct.SetKey(1, 0, linuxread); linuxread=0; return ct;
			}

			//int c=getc(stdin);
			if(c==10) c=13; int t=0, ctrl=0;
			//print("   >",itos(c), "<   ");
			//ct.SetKey(1, 0, c);
			//if(!linuxread && c==127) c=46;
			
			if(linuxread)
			switch(linuxread){
			// one
				case 27: 
					if(c=='['){ linuxread<<=8; linuxread+=c; }
					else if(c=='O'){ linuxread<<=8; linuxread+=c; }
					else linuxread=c;
					return ct;
					break;
			// two
				case ((27<<8)+'O'): ctrl=CN_RCTRL;
				case ((27<<8)+'['):
					if(c=='A'){ c=MK_UP; t=CN_KEY; }
					if(c=='B'){ c=MK_DOWN; t=CN_KEY; }
					if(c=='C'){ c=MK_RIGHT; t=CN_KEY; }
					if(c=='D'){ c=MK_LEFT; t=CN_KEY; }
					if(c=='1' || c=='3' || c=='4'){ linuxread<<=8; linuxread+=c; }
					if(t){ ct.SetKey(1, t|ctrl, c); linuxread=0; }
					return ct;
					break;
			// three
				case ((27<<16)+('['<<8)+'1'):
					if(c>='1' && c<='9'){ linuxread<<=8; linuxread+=c; }
					if(c==126){ ct.SetKey(1, CN_KEY, MK_HOME); linuxread=0; return ct; } break;

				case ((27<<16)+('['<<8)+'2'):
					if(c>='1' && c<='4'){ linuxread<<=8; linuxread+=c; }

				case ((27<<16)+('['<<8)+'3'): if(c==126){ ct.SetKey(1, CN_KEY, MK_DELETE); linuxread=0; } return ct; break;

				case ((27<<16)+('['<<8)+'4'): if(c==126){ ct.SetKey(1, CN_KEY, MK_END); linuxread=0; } return ct; break;
			// four
			// F1-F12
			}

			if(c==126 && linuxread>=((27<<24)+('['<<16)+'1'<<8+'1') && linuxread<=((27<<24)+('['<<16)+'2'<<8+'1')){
				int k=0;
				if(linuxread&65535>='1'<<8+'1' && linuxread&65535<='1'<<8+'5'){ k=linuxread&255-'1'+1; }
				if(linuxread&65535>='1'<<8+'7' && linuxread&65535<='1'<<8+'9'){ k=linuxread&255-'7'+6; }
				if(linuxread&65535>='2'<<8+'0' && linuxread&65535<='1'<<8+'4'){ k=linuxread&255-'0'+9; }
				if(k){ ct.SetKey(1, CN_KEY, (MK_F1+k)&255); linuxread=0; return ct; }
			}

			if(linuxread){
				if((linuxread&224)==192 && (c&192)==128){
					c=((linuxread&31)<<6)+(c&63);
					ct.SetKey(1, 0, c); linuxread=0;
				}
				else { ct.SetKey(1, 0, linuxread); linuxread=c; }
				return ct;
			}
			
			//else if(linuxread==27 && c=='[') linuxread=c;
			//else if(linuxread=='['){ int t=0;
			//	if(c=='A'){ c=MK_UP; t=CN_KEY; }
			//	if(c=='B'){ c=MK_DOWN; t=CN_KEY; }
			//	if(c=='C'){ c=MK_RIGHT; t=CN_KEY; }
			//	if(c=='D'){ c=MK_LEFT; t=CN_KEY; }
			//	ct.SetKey(1, t, c); linuxread=0;
			//}
			//else if(linuxread){
			//	if((linuxread&224)==192 && (c&192)==128){
			//		c=((linuxread&31)<<6)+(c&63);
			//		ct.SetKey(1, 0, c); linuxread=0;
			//	}
			//	else { ct.SetKey(1, 0, linuxread); linuxread=c; }
			//}
			
			if(!linuxread){
				if(c==27) linuxread=27;
				else if((c&224)==192) linuxread=c;
				else ct.SetKey(1, 0, c);
			}
			
			//print(itos(c));
			//printf( "%c", c);
//#endif
		}
		return ct;
	}

#include "signal.h"

GlobalConsole *gcsig=0;

// signals
void sigwinch(int ){
	if(gcsig){
		COORD cd=TTYSize();
		gcsig->OnSize(cd.X, cd.Y);
	}
	return ;
}

int addglobalconsolesig(GlobalConsole &gc){
	gcsig=&gc;
#ifndef WIN32
	signal(SIGWINCH, sigwinch);
#endif
	return 1;
}
