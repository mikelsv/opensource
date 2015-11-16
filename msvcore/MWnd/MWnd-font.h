bool useloadfont=0;
bool MWNDUSELOADFONTb=0;
bool MWNDCALLFONTS=1;

#ifdef MSV_USEXLIB
GC GetDC(HWND h);
//HFONT CreateFontIndirect(LOGFONTA *);
#endif



// MyFont2
// DataFile: ver, name, font: font_size, font_count, font_data: font_

class MyFontHead{
public:
	// ver, name size
	int ver, sz;
	char data[0];
};

class MyFontStringHead{
public:
	// font size, chars count
	int font, chars;
};

class MyFontChar{
public:
	unsigned short sx, sy;
	unsigned char data[0];
};

class MyFontString{
	MString _data;
	int sz, chars, height;
	MyFontChar **chr;

public:

	int SetData(int chars, int dsz, unsigned char *line, unsigned int *pos){
		_data.Reserv(dsz+chars*sizeof(void*));
		chr=(MyFontChar**)(_data.data+dsz);
		memcpy(_data.data, line, dsz);

		for(int i=0; i<chars; i++){
			chr[i]=(MyFontChar*)_data.pos(pos[i]);
		}

		this->chars=chars;
		return 1;
	}

	int GetW(int c)const{
		if(c<0 || c>=chars) return 0;
		return chr[c]->sx;
	}

	int GetH(int c)const{
		if(c<0 || c>=chars) return 0;
		return chr[c]->sy;
	}

	unsigned char* GetData(int c)const{
		if(c<0 || c>=chars) return 0;
		return chr[c]->data;
	}

	MyFontChar* GetChar(int c)const{
		if(c<0 || c>=chars) return 0;
		return chr[c];
	}

};

class MyFont2{
	int ver;
	MString name;

	MyFontString str[80];

public:
	void SetFont(VString n){ name=n; }

	int SetString(int font, int chars, int dsz, unsigned char *line, unsigned int *pos){
		if(font<0 || font>=80) return 0;
		return str[font].SetData(chars, dsz, line, pos);
	}		

	const MyFontString* GetString(int font)const{
		if(font<0 || font>=80) return 0;
		return &str[font];
	}

	int GetW(int f, int c)const{
		if(f<0 || f>80) return 0;
		return str[f].GetW(c);
	}

	int GetH(int f, int c)const{
		if(f<0 || f>80) return 0;
		return str[f].GetH(c);
	}

};

//	MFont mfont; MyFont2 mydeffont; MString df, hdf;
//	CreateMyFont(df, mydeffont, mfont);
//	DataToHex(df, "times_new_romain", hdf);
//	SaveFile("times_new_romain.h", hdf);


#ifndef NOUSEMSV_WINAPI
int CreateMyFont(MString &out, MyFont2 &mf, MFont &font, int chars=256){
// 4,6,7,8,9,10,11,12,14,16,18,20,22,24,24,30,36,48,72
	int szs[]={4,6,7,8,9,10,11,12,14,16,18,20,22,24,24,30,36,48,72};
	int szsz=sizeof(szs)/sizeof(int);

	SIZE s, ms;// int sa;
	unsigned char buf[S16K];

	HLString ls; VString fn;
	MyFontHead head; head.ver=1; head.sz=font.GetFont().sz;

	
	// add header
	ls+VString((char*)&head, sizeof(head))+font.GetFont();
	
	//mf._data.Reserv(szsz*sizeof(MyFontF), 0); mf.sz=szsz; mf.font=(MyFontF*)mf._data.rchar();// font.lfWeight=0;

	for(int ii=0; ii<szsz; ii++){
	//MyFontF*mff=&mf.font[ii]; sa=0;
	//mff->sz=fval; mff->tsz=tval; mf.bl.set(szs[ii], 1); mf.pos[szs[ii]]=ii;
	//MString data; data.Reserv((tval-fval)*(2+4)); mff->szx=(BYTE*)data.rchar(); mff->szy=(BYTE*)data.pos(tval-fval); mff->data=(int*)data.pos((tval-fval)*2);
		// set font size
		font.lfHeight=szs[ii];

		// add string header
		MyFontStringHead shead;
		shead.font=font.lfHeight; shead.chars=chars;

		// char size
		ms.cx=0; ms.cy=0;

		// go
		HDC TmpDC=CreateCompatibleDC(0); HFONT m_Font; 

		m_Font=CreateFontIndirect(font);
		HFONT pOldFont=NULL;
		if (m_Font)
			pOldFont = (HFONT)SelectObject(TmpDC,m_Font);
		else
			pOldFont = (HFONT)SelectObject(TmpDC,GetStockObject(DEFAULT_GUI_FONT));

		for(int k=0; k<chars; k++){ char c[]={k};
			MyFontChar chr;

			GetTextExtentPoint32(TmpDC, MODUNICODE(c), 1, &s);
			chr.sx=s.cx; chr.sy=s.cy;
			if(ms.cx<s.cx)
				ms.cx=s.cx;
			if(ms.cy<s.cy)
				ms.cy=s.cy;

			if(ms.cx!=s.cx || ms.cy!=s.cy)
				int z=356;

			//mff->szx[k]=s.cx; mff->szy[k]=s.cy;
			//mff->data[k]=sa/8;
			//sa+=s.cx*s.cy; if(sa%8!=0) sa+=8-sa%8;
		}

		//shead.height=ms.cy;
		ls+VString((char*)&shead, sizeof(shead));

		SetTextColor(TmpDC,RGB(255,255,255));
		SetBkColor(TmpDC,RGB(0,0,0));
		//draw the text
		SetBkMode(TmpDC,OPAQUE);

//mff->_data.Reserv((2+4)*(tval-fval)+sa/8, 0); memcpy(mff->_data, data, data);
//mff->szx=(BYTE*)mff->_data.rchar(); mff->szy=(BYTE*)mff->_data.pos(tval-fval); mff->data=(int*)mff->_data.pos((tval-fval)*2);

		// bitmap info
		int width=ms.cx, height=ms.cy; RECT rc; rc.top=0; rc.left=0; rc.right=width; rc.bottom=height;
		BITMAPINFO bmInfo;
		memset(&bmInfo.bmiHeader,0,sizeof(BITMAPINFOHEADER));
		bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bmInfo.bmiHeader.biWidth=width;
		bmInfo.bmiHeader.biHeight=height;
		bmInfo.bmiHeader.biPlanes=1;
		bmInfo.bmiHeader.biBitCount=24;
		BYTE *pbase;

		HBITMAP TmpBmp=CreateDIBSection(TmpDC, &bmInfo, DIB_RGB_COLORS, (void**)&pbase, 0, 0);
		HGDIOBJ TmpObj=SelectObject(TmpDC, TmpBmp);
		if(!pbase) goto end_func;

		for(int i=0; i<chars; i++){ char c[]={i};
			memset(pbase, 0, height*((((24 * width) + 31) / 32) * 4));
			::DrawText(TmpDC, MODUNICODE(c), 1, (LPRECT)&rc, 0);

			GetTextExtentPoint32(TmpDC, MODUNICODE(c), 1, &s);

			CxImage itext;

			#ifdef WIN32 // no linux
			itext.CreateFromHBITMAP(TmpBmp);
			#endif

			// write char
			MyFontChar chr;
			chr.sx=s.cx; chr.sy=s.cy;
			memset(buf, 0, ((s.cx*s.cy)+7)/8);

			//mbool bl; bl=(bool*)mff->_data.pos((2+4)*(tval-fval)+mff->data[i]); int p=0;
			for (long iy=0; iy<s.cy; iy++){
				for (long ix=0; ix<s.cx; ix++){
					if(itext.GetPixelColor(ix,iy).rgbBlue){
						buf[(ix+iy*s.cx)/8]|=(1 << ((ix+iy*s.cx)%8));
						//bl.set(p, 1);
					}// p++;
					//buf[(ix+iy*s.cx)/8]|=(1 << ((ix+iy*s.cx)%8));
				}
			}

			ls+VString((char*)&chr, sizeof(chr));
			ls+VString(buf, ((s.cx*s.cy)+7)/8);
		}
end_func:

		if (pOldFont) SelectObject(TmpDC,pOldFont);
		if (m_Font) DeleteObject(m_Font);
		DeleteObject(TmpBmp);
		DeleteObject(SelectObject(TmpDC,TmpObj));
		DeleteDC(TmpDC);
	}

	ls.String(out);

	return 1;
}
#endif // NOUSEMSV_WINAPI


int LoadMyFont(VString data, MyFont2 &fnt){
	//MyFontHead *head; MyFontStringHead *shead; MyFontChar *chr;
	unsigned char *line=data, *to=data.endu(), *lline;
	unsigned int p[65535]; memset(&p, 0, sizeof(p));

	// head
	MyFontHead &head=*(MyFontHead*)line;
	if(to-line<sizeof(head) || to-line<sizeof(head)+head.sz) return 0;
	fnt.SetFont(VString(head.data, head.sz));

	line+=sizeof(head)+head.sz;

	while(line<to){
		MyFontStringHead &shead=*(MyFontStringHead*)line;
		if(to-line<sizeof(shead))
			return 0;
		line+=sizeof(shead);
		lline=line;

		for(int c=0; c<shead.chars; c++){
			MyFontChar &chr=*(MyFontChar*)line;
			if(to-line<sizeof(chr) || to-line<sizeof(char)+((chr.sx*chr.sy)+7)/8)
				return 0;
			p[c]=line-lline;

			line+=sizeof(chr)+((chr.sx*chr.sy)+7)/8;
		}

		fnt.SetString(shead.font, shead.chars, line-lline, lline, p);
	}
	return 1;
}


int DataToHex(VString data, VString name, MString &out){
	unsigned char *line=data, *to=data.endu();

	HLString ls;
	unsigned char d[3], f=1, l=0; VString c(d, 2);

	ls+"unsigned char "+name+"[]={";
	

	for(line; line<to; line++){
		if(!f) ls+","; else f=0;
		if(l>128){ ls+"\r\n"; l=0; } l++;
		d[0]=*line/16; d[1]=*line%16;
		d[0]+= d[0]<10 ? '0' : 'A'-10;
		d[1]+= d[1]<10 ? '0' : 'A'-10;
		ls+"0x"+c;
	}

	ls+"};";
	ls.String(out);
	return 1;
}




// Old My Font
class MyFontF{public:
MString _data;
int sz, tsz; // 256, 65536
BYTE *szx, *szy; int*data;
};


class MyFont{ public:
MString name;
int sz;
BYTE pos[80];
BYTE _mb[64];
mbool bl;
MString _data;
MyFontF *font;
MyFont(){ memset(this, 0, sizeof(MyFont)); bl=(bool*)&_mb; }
void Clear() {
for(int ii=0; ii<sz; ii++){ font[ii]._data.Clean(); }
_data.Clean(); name.Clean(); font=0;
return ; }
};

class MyFontCur{public:
MyFont *pmf;
unsigned int pos;

MyFontCur(){pmf=0; pos=0;}

int Get(MyFont &mf, MFont &mff){
	int ii=mff.lfHeight; if(!ii) ii=16;
for(ii; ii<80; ii++){
	if(mf.pos[ii]) {pmf=&mf; pos=mf.pos[ii]; return 1; }
}
return 0;
}
};

#ifndef NOUSEMSV_WINAPI

int CreateMyFont(const MString &file, MyFont&mf, MFont &font, int fval=0, int tval=256){
// 4,6,7,8,9,10,11,12,14,16,18,20,22,24,24,30,36,48,72
	int szs[]={4,6,7,8,9,10,11,12,14,16,18,20,22,24,24,30,36,48,72};
	int szsz=sizeof(szs)/sizeof(int); if(fval>=tval) return 0;

	SIZE s, ms; int sa; ms.cx=0; ms.cy=0; sa=0;
	
	mf._data.Reserv(szsz*sizeof(MyFontF), 0); mf.sz=szsz; mf.font=(MyFontF*)mf._data.rchar();// font.lfWeight=0;

	for(int ii=0; ii<szsz; ii++){
	MyFontF*mff=&mf.font[ii]; sa=0;
	mff->sz=fval; mff->tsz=tval; mf.bl.set(szs[ii], 1); mf.pos[szs[ii]]=ii;
	MString data; data.Reserv((tval-fval)*(2+4)); mff->szx=(BYTE*)data.rchar(); mff->szy=(BYTE*)data.pos(tval-fval); mff->data=(int*)data.pos((tval-fval)*2);
	font.lfHeight=szs[ii];

HDC TmpDC=CreateCompatibleDC(0); HFONT m_Font; 

m_Font=CreateFontIndirect(font);
HFONT pOldFont=NULL;
if (m_Font)
	pOldFont = (HFONT)SelectObject(TmpDC,m_Font);
else
	pOldFont = (HFONT)SelectObject(TmpDC,GetStockObject(DEFAULT_GUI_FONT));

for(int k=fval; k<tval; k++){ char c[]={k};
GetTextExtentPoint32(TmpDC, MODUNICODE(c), 1, &s);
mff->szx[k]=s.cx; mff->szy[k]=s.cy; if(ms.cx<s.cx) ms.cx=s.cx; if(ms.cy<s.cy) ms.cy=s.cy;
mff->data[k]=sa/8;
sa+=s.cx*s.cy; if(sa%8!=0) sa+=8-sa%8;
}

SetTextColor(TmpDC,RGB(255,255,255));
SetBkColor(TmpDC,RGB(0,0,0));
//draw the text
SetBkMode(TmpDC,OPAQUE);

mff->_data.Reserv((2+4)*(tval-fval)+sa/8, 0); memcpy(mff->_data, data, data);
mff->szx=(BYTE*)mff->_data.rchar(); mff->szy=(BYTE*)mff->_data.pos(tval-fval); mff->data=(int*)mff->_data.pos((tval-fval)*2);


int width=ms.cx, height=ms.cy; RECT rc; rc.top=0; rc.left=0; rc.right=width; rc.bottom=height;
BITMAPINFO bmInfo;
memset(&bmInfo.bmiHeader,0,sizeof(BITMAPINFOHEADER));
bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
bmInfo.bmiHeader.biWidth=width;
bmInfo.bmiHeader.biHeight=height;
bmInfo.bmiHeader.biPlanes=1;
bmInfo.bmiHeader.biBitCount=24;
BYTE *pbase;

HBITMAP TmpBmp=CreateDIBSection(TmpDC,&bmInfo,DIB_RGB_COLORS,(void**)&pbase,0,0);
HGDIOBJ TmpObj=SelectObject(TmpDC,TmpBmp);
if(!pbase) goto end_func;

for(int i=fval; i<tval; i++){ char c[]={i};
memset(pbase,0,height*((((24 * width) + 31) / 32) * 4));
::DrawText(TmpDC, MODUNICODE(c), 1, (LPRECT)&rc, 0);

CxImage itext;

#ifdef WIN32 // no linux
itext.CreateFromHBITMAP(TmpBmp);
#endif

mbool bl; bl=(bool*)mff->_data.pos((2+4)*(tval-fval)+mff->data[i]); int p=0;
for (long ix=0;ix<mff->szx[i];ix++){
	for (long iy=0;iy<mff->szy[i];iy++){
		if (itext.GetPixelColor(ix,iy).rgbBlue){
			bl.set(p, 1);
		} p++;
	}
}
}
end_func:

if (pOldFont) SelectObject(TmpDC,pOldFont);
if (m_Font) DeleteObject(m_Font);
DeleteObject(TmpBmp);
DeleteObject(SelectObject(TmpDC,TmpObj));
DeleteDC(TmpDC);
}

HFILE fl=CreateFile(file, _O_RDWR|O_CREAT|_O_EXCL|O_BINARY,  _S_IREAD| S_IWRITE);
if(!ishandle(fl)) return 0;

int wr; 

wr=WriteFile(fl, (char*)&mf, sizeof(MyFont)); if(wr!=sizeof(MyFont)) return 0;
wr=WriteFile(fl, (char*)mf._data, mf._data); if(wr!=mf._data.size()) return 0;

for(int ii=0; ii<szsz; ii++){
MyFontF*mff=&mf.font[ii]; wr=WriteFile(fl, (char*)mff, sizeof(MyFontF));
if(wr!=sizeof(MyFontF)) return 0;
}

for(int ii=0; ii<szsz; ii++){
MyFontF*mff=&mf.font[ii];
wr=WriteFile(fl, (char*)mff->_data, mff->_data);
if(wr!=mff->_data.size()) return 0;
}
CloseHandle(fl);

return 1;
}

//#endif // #ifndef NOUSEMSV_WINAPI

int LoadMyFont(const MString &file, MyFont &mf){
#ifndef MWNDUSELOADFONT
	return 0;
#endif
HFILE fl=CreateFile(file, _O_RDONLY|O_BINARY, _S_IREAD| S_IWRITE);
if(!ishandle(fl)) return 0;
mf.Clear();

SetFilePointer(fl, 0, 0);
int read=ReadFile(fl, (char*)&mf, sizeof(MyFont)); if(read!=sizeof(MyFont)) return 0;
memset(&mf._data, 0, sizeof(MString));
mf._data.Reserv(mf.sz*sizeof(MyFontF), 0); mf.font=(MyFontF*)mf._data.rchar();
read=ReadFile(fl, mf._data, mf._data); if(read!=mf._data.size()) return 0;

for(int ii=0; ii<mf.sz; ii++){ MyFontF*mff=mf.font+ii;
read=ReadFile(fl, (char*)mff, sizeof(MyFontF)); if(read!=sizeof(MyFontF)) return 0;
unsigned int sz=mff->_data.size(); memset(&mff->_data, 0, sizeof(MString));
mff->_data.Reserv(sz);
}

for(int ii=0; ii<mf.sz; ii++){ MyFontF*mff=mf.font+ii; unsigned int tp=GetFilePointer(fl);
read=ReadFile(fl, (char*)mff->_data, mff->_data.size()); if(read!=mff->_data.size()) return 0;
mff->szx=(BYTE*)mff->_data.rchar(); mff->szy=(BYTE*)mff->_data.pos(mff->tsz-mff->sz); mff->data=(int*)mff->_data.pos((mff->tsz-mff->sz)*2);
}
CloseHandle(fl);
return 1;
};

//#ifndef NOUSEMSV_WINAPI

#ifndef MWNDUSELOADFONT
#define GetTextExtentPoint32M(a, b, c, d, e) GetTextExtentPoint32(a, MODUNICODEV(b, c), d)
//#define GetTextExtentPoint32MU(a, b, c, d, e) GetTextExtentPoint32(a, MODUNICODEV(b, c), d)
#else
//#define GetTextExtentPoint32MU(a, b, c, d, e) GetTextExtentPoint32M(a, b, c, d, e)

BOOL GetTextExtentPoint32M(IN HDC hdc, IN LPCSTR line, IN int size, OUT LPSIZE lsz, MyFontCur &cur){
//	if(!useloadfonts) return GetTextExtentPoint32(hdc,MODUNICODEV(line,size),lsz);
		if(!useloadfont) return GetTextExtentPoint32(hdc, MODUNICODE(line), size, lsz);
if(!cur.pmf) return  0;
	SIZE &sz=*lsz; sz.cx=0; sz.cy=0;

unsigned char*ln=(unsigned char*)line, *to=ln+size;
for(ln; ln<to; ln++){
	sz.cx+=cur.pmf->font[cur.pos].szx[*ln];
	if(sz.cy<cur.pmf->font[cur.pos].szy[*ln]) sz.cy=cur.pmf->font[cur.pos].szy[*ln];
}

return 1;
}

#endif
#endif // NOUSEMSV_WINAPI

bool DrawTextM(CxImage &img, char*line, unsigned int size, MRect mr, MyFontCur &cur){
SIZE sz; if(!cur.pmf) return 0; if(!useloadfont) return 0;
unsigned char*ln=(unsigned char*)line, *to=ln+size;
for(ln; ln<to; ln++){
	sz.cx=cur.pmf->font[cur.pos].szx[*ln];
	sz.cy=cur.pmf->font[cur.pos].szy[*ln];

	mbool bl; bl=(bool*)cur.pmf->font[cur.pos]._data.pos(
		(2+4)*(cur.pmf->font[cur.pos].tsz-cur.pmf->font[cur.pos].sz)+cur.pmf->font[cur.pos].data[*ln]); int i=0;
	for(int x=0; x<sz.cx; x++){
		for(int ii=0; ii<sz.cy; ii++){
			if(bl.get(i++))
				img.SetPixelColor(mr.left+x, mr.top+ii, 0xffffff);
		}
	} mr.left+=sz.cx;// mr.top+=sz.cy;

}
	return 1;
}

bool DrawTextM(CxImage &img, char*line, unsigned int size, MRect mr, MyFontCur &cur, const RGBQUAD rgb){
SIZE sz; if(!cur.pmf) return 0; if(!useloadfont) return 0;
unsigned char*ln=(unsigned char*)line, *to=ln+size;
for(ln; ln<to; ln++){
	sz.cx=cur.pmf->font[cur.pos].szx[*ln];
	sz.cy=cur.pmf->font[cur.pos].szy[*ln];

	mbool bl; bl=(bool*)cur.pmf->font[cur.pos]._data.pos(
		(2+4)*(cur.pmf->font[cur.pos].tsz-cur.pmf->font[cur.pos].sz)+cur.pmf->font[cur.pos].data[*ln]); int i=0;
	for(int x=0; x<sz.cx; x++){
		for(int ii=0; ii<sz.cy; ii++){
			if(bl.get(i++))
				img.SetPixelColor(mr.left+x, mr.top+ii, rgb);
		}
	} mr.left+=sz.cx;// mr.top+=sz.cy;

}
	return 1;
}

bool DrawTextMDC(CxImage &img, char*line, unsigned int size, MRect mr, MyFontCur &cur, const RGBQUAD rgb, const RGBQUAD trgb){
SIZE sz; if(!cur.pmf) return 0; if(!useloadfont) return 0; MRect tmr=mr;

unsigned char*ln=(unsigned char*)line, *to=ln+size;
for(ln; ln<to; ln++){
	sz.cx=cur.pmf->font[cur.pos].szx[*ln];
	sz.cy=cur.pmf->font[cur.pos].szy[*ln];

	mbool bl; bl=(bool*)cur.pmf->font[cur.pos]._data.pos(
		(2+4)*(cur.pmf->font[cur.pos].tsz-cur.pmf->font[cur.pos].sz)+cur.pmf->font[cur.pos].data[*ln]); int i=0;
	for(int x=0; x<sz.cx; x++){
		for(int ii=0; ii<sz.cy; ii++){
			if(bl.get(i++)){
				img.SetPixelColor(mr.left+x+1, mr.top+ii, trgb);
				img.SetPixelColor(mr.left+x, mr.top+ii+1, trgb);
				img.SetPixelColor(mr.left+x+1, mr.top+ii+1, trgb);
				img.SetPixelColor(mr.left+x-1, mr.top+ii, trgb);
				img.SetPixelColor(mr.left+x, mr.top+ii-1, trgb);
			}
		}
	} mr.left+=sz.cx;// mr.top+=sz.cy;

}

ln=(unsigned char*)line; to=ln+size; mr=tmr;
for(ln; ln<to; ln++){
	sz.cx=cur.pmf->font[cur.pos].szx[*ln];
	sz.cy=cur.pmf->font[cur.pos].szy[*ln];

	mbool bl; bl=(bool*)cur.pmf->font[cur.pos]._data.pos(
		(2+4)*(cur.pmf->font[cur.pos].tsz-cur.pmf->font[cur.pos].sz)+cur.pmf->font[cur.pos].data[*ln]); int i=0;
	for(int x=0; x<sz.cx; x++){
		for(int ii=0; ii<sz.cy; ii++){
			if(bl.get(i++))
				img.SetPixelColor(mr.left+x, mr.top+ii, rgb);
		}
	} mr.left+=sz.cx;// mr.top+=sz.cy;

}
	return 1;
}

/*
MString DrawTextMText(char*line, unsigned int size, MyFontCur &cur){
SIZE sz; if(!cur.pmf) return MString(); if(!useloadfont) return MString();
unsigned char*ln=(unsigned char*)line, *to=ln+size;
MString ms="{"; int64 val=0;
for(ln; ln<to; ln++){
	sz.cx=cur.pmf->font[cur.pos].szx[*ln];
	sz.cy=cur.pmf->font[cur.pos].szy[*ln];

	mbool bl; bl=(bool*)cur.pmf->font[cur.pos]._data.pos(
		(2+4)*(cur.pmf->font[cur.pos].tsz-cur.pmf->font[cur.pos].sz)+cur.pmf->font[cur.pos].data[*ln]); int i=0;
	for(int x=0; x<sz.cx; x++){
		for(int ii=0; ii<sz.cy; ii++){
			val*=2;
			if(bl.get(i++)) val++;
				//img.SetPixelColor(mr.left+x, mr.top+ii, 0xffffff);
		}
		ms+="0x"+itos(val, 16, 2); if(x+1<sz.cx) ms+=","; val=0;
	} //mr.left+=sz.cx; // mr.top+=sz.cy;

}
	return ms+"}";
}*/




MyFont2 mydefaultfont;

class _MWNDAUTOLOADFONTL{
public:
	_MWNDAUTOLOADFONTL(){
//#ifdef MWNDUSEAUTOLOADFONT
#ifdef MWNDUSELOADFONT
		// Create Font
		// MFont mfont; mfont.NullFn(); CreateMyFont("mfont.fnt", mfdef, mfont);
		// Load Font
		//MWNDUSELOADFONTb=useloadfont=LoadMyFont(MWNDUSEAUTOLOADFONT, MWNDUSELOADFONT);
		MWNDUSELOADFONTb=useloadfont=LoadMyFont(LoadFile(MWNDUSELOADFONT), mydefaultfont);
#ifndef MWNDUSEOPENGL
		if(MWNDUSELOADFONTb) MWNDCALLFONTS=0;
#endif
#endif
#ifdef MWNDUSELOADFONT_MEM
		MWNDUSELOADFONTb=useloadfont=LoadMyFont(VString((char*)MWNDUSELOADFONT_MEM, sizeof(MWNDUSELOADFONT_MEM)), mydefaultfont);
#endif
//#endif
	}
}_MWNDAUTOLOADFONTL;


#ifndef NOUSEMSV_WINAPI

// MWnd Fonts Class
class MHDC{
	HDC dc;
	HWND hwnd;
public:

	MHDC(){ dc=0; hwnd=0; }
	MHDC(HDC c, HWND hw){ dc=c; hwnd=hw; }
	~MHDC(){ Close(); }
	
	HDC GetDC(HWND hw){ Close(); dc=::GetDC(hw); if(dc){ hwnd=hw;  }else dc=0; return dc; }
	operator HDC(){ return dc; }


	void Close(){ if(dc && hwnd){ ReleaseDC(hwnd, dc); dc=0; hwnd=0; } }
};


class MWndFontsControl{
	HWND hwnd;
	HDC getdc;
	HDC credc;
	HFONT hfont;
	HFONT holdfont;
	BITMAPINFO binfo;
	BYTE *imgdata;
	HBITMAP hbmp;
	HGDIOBJ hobj;
	

public:
	MWndFontsControl(){ getdc=0; hwnd=0; credc=0; hfont=0; holdfont=0; imgdata=0; hbmp=0; hobj=0; }
	~MWndFontsControl(){ Close(); }

	operator HDC(){ return credc; }
	operator HFONT(){ return hfont; }

	void Close(){
		if(holdfont) ::SelectObject(credc, holdfont); 
		if(hobj){ DeleteObject(::SelectObject(credc, hobj)); imgdata=0; }

		if(getdc){ ReleaseDC(hwnd, getdc); }
		if(credc){ DeleteDC(credc); }
		if(hfont){ DeleteObject(hfont); }
		if(imgdata) delete imgdata;
		if(hbmp) DeleteObject(hbmp);
		
		hwnd=0; getdc=0; credc=0; hfont=0; holdfont=0; imgdata=0; hbmp=0; hobj=0;  
	}

	HDC GetCrea(){ return credc; }

	HDC GetDC(HWND hw){ if(getdc) ReleaseDC(hwnd, getdc); getdc=::GetDC(hw); if(getdc) hwnd=hw; return getdc; }
	HDC CreateCompatibleDC(HDC c){ if(credc) DeleteDC(credc); credc=::CreateCompatibleDC(c); return credc; }

	HFONT CreateFontIndirect(CONST LOGFONT* lf, bool sel){
		if(hfont) DeleteObject(hfont); hfont=::CreateFontIndirect(lf);
		if(sel) return SelectObject(credc, hfont);
		return hfont;
	}

	//HFONT DeleteFontIndirect(){
	//	if (pOldFont) SelectObject(TmpDC,pOldFont);
	//	DeleteObject(m_Font);
	//}

	HFONT SelectObject(HDC c, HFONT f){
		if(hfont)
			holdfont=(HFONT)::SelectObject(credc, hfont);
		else
			holdfont=(HFONT)::SelectObject(credc, GetStockObject(DEFAULT_GUI_FONT));
		return holdfont;
	}

	void SetColors(COLORREF text, COLORREF bg, int mode){ SetTextColor(credc, text); SetBkColor(credc, bg); SetBkMode(credc, mode); }

	BITMAPINFO* GetBitmap(int width, int height, int bpp){
		memset(&binfo.bmiHeader,0,sizeof(BITMAPINFOHEADER));
		binfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		binfo.bmiHeader.biWidth=width;
		binfo.bmiHeader.biHeight=height;
		binfo.bmiHeader.biPlanes=1;
		binfo.bmiHeader.biBitCount=bpp;
		return &binfo;
	}

	BYTE* CreateDIBSection(int width, int height, int bpp=24, unsigned int t=DIB_RGB_COLORS, int c=1){
		if(hbmp) DeleteObject(hbmp);
		hbmp=::CreateDIBSection(credc, GetBitmap(width, height, bpp), t, (void**)&imgdata, 0, 0);
		if(c) hobj=::SelectObject(credc, hbmp);
		return imgdata;
	}

	int MakeFirst(HWND hw, CONST LOGFONT* lf){
		GetDC(hw);
		CreateCompatibleDC(0);
		CreateFontIndirect(lf, 1);
		SetColors(RGB(255, 255, 255), RGB(0,0,0), OPAQUE);
		return 1;
	}
};

#endif