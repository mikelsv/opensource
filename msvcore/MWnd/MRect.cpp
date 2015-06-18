// My Rectage class x,y - cx,cy

#ifndef WIN32 // LINUX
#define WINAPI //		__stdcall
#define WINAPIV //		__cdecl

typedef unsigned int 	HANDLE;
typedef HANDLE			HINSTANCE;
typedef HANDLE			HCURSOR;

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef long			LONG;
typedef unsigned int	UINT;
typedef void*			LPVOID;
typedef unsigned long*	LPDWORD;
typedef int				BOOL;
typedef int				INT;
typedef short			SHORT;
typedef WORD			WCHAR;
typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG;
typedef unsigned int	HWND;
typedef const CHAR		*LPCSTR;
typedef float			FLOAT;
typedef double			DOUBLE;

#endif


#ifndef WIN32

#ifndef NORECTSTRUCT
typedef struct tagRECT{
	long    left;
	long    top;
	long    right;
	long    bottom;
} RECT; // *PRECT; //, NEAR *NPRECT, FAR *LPRECT;

#endif

typedef struct _COORD {
    SHORT X;
    SHORT Y;
} COORD, *PCOORD;

int GetSystemMetrics(int t){ return 0; }

typedef struct tagLOGFONTA{
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    CHAR lfFaceName[ 32 ];
    } 	LOGFONTA;

typedef struct tagLOGFONTW{
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    WCHAR lfFaceName[ 32 ];
    } 	LOGFONTW;

typedef LOGFONTA LOGFONT;

#endif


#define SM_CXSCREEN             0
#define SM_CYSCREEN             1

#define DCXSIZE GetSystemMetrics(SM_CXSCREEN)
#define DCYSIZE GetSystemMetrics(SM_CYSCREEN)

class MRect{
public:
	int left;
	int top;
	int right;
	int bottom;

	MRect(){ left=0; right=0; top=0; bottom=0; }
	MRect(int _left, int _top, int _right, int _bottom){left=_left; top=_top; right=_right; bottom=_bottom;}

	inline operator RECT*()const{ return (RECT*)this;	}
	inline operator RECT&()const{ return *(RECT*)this;	}

	void Set(int _left, int _top, int _right, int _bottom){left=_left; top=_top; right=_right; bottom=_bottom;}
	void Set(VString line){
		VString o;

		o=PartLine(line, line, ","); left=o.toi();
		o=PartLine(line, line, ","); top=o.toi();
		o=PartLine(line, line, ","); right=o.toi();
		o=PartLine(line, line, ","); bottom=o.toi();
	}

	int GetW()const {return right-left;}
	int GetH()const {return bottom-top;}
	int Is()const { return right-left || bottom-top; }

	void Move(int x, int y){ left+=x; right+=x; top+=y; bottom+=y; }

	bool InRect(int x, int y)const {if(x>=left && x<=right && y>=top && y<=bottom) return 1; return 0;}
	
//	void Clear(){ left=0; top=0; right=0; bottom=0; }
	void Clean(){ left=0; top=0; right=0; bottom=0; }

	inline bool IsNull()const {if(!left && !top && !right && !bottom) return 0; return 1;}
	inline void Center(){int w=GetW()/2, h=GetH()/2; left=DCXSIZE/2-w; top=DCYSIZE/2-h; right=DCXSIZE/2+w; bottom=DCYSIZE/2+h;}
	bool Normal(){if(left<=right && top<=bottom) return 1; return 0; }
	void Union(MRect rc){
		if(left>rc.left) left=rc.left;
		if(right<rc.right) right=rc.right;
		if(top>rc.top) top=rc.top;
		if(bottom<rc.bottom) bottom=rc.bottom;
	}
	void Crop(MRect rc){
		if(left<rc.left) left=rc.left;
		if(right>rc.right) right=rc.right;
		if(top<rc.top) top=rc.top;
		if(bottom>rc.bottom) bottom=rc.bottom;
	}

	MRect& GetCenter(MRect rc){ // this <- rc
		left=(GetW()-rc.GetW())/2; right=left+rc.GetW();
		top=(GetH()-rc.GetH())/2; bottom=top+rc.GetH();
		return *this;
	}

};

bool operator==(const MRect&o, const MRect&t){if(o.left!=t.left || o.top!=t.top || o.right!=t.right || o.bottom!=t.bottom) return 0; return 1;}
bool operator!=(const MRect&o, const MRect&t){if(o.left!=t.left || o.top!=t.top || o.right!=t.right || o.bottom!=t.bottom) return 1; return 0;}


// SRect
class SRect{
public:
	int x, y, sx, sy;

	SRect(){ x=0; y=0; sx=0; sy=0; }
	SRect(int _x, int _y, int _sx, int _sy){ x=_x; y=_y; sx=_sx; sy=_sy; }
	SRect(const SRect &rc){ x=rc.x; y=rc.y; sx=rc.sx; sy=rc.sy; }
	SRect(const MRect &rc){ x=rc.left; y=rc.top; sx=rc.GetW(); sy=rc.GetH(); }

	void Set(int _x, int _y, int _sx, int _sy){ x=_x; y=_y; sx=_sx; sy=_sy; }

	// operations
	void Center(){
		x=DCXSIZE/2-sx/2;
		y=DCYSIZE/2-sy/2;
	}

	void And(SRect rc){
		int tx=maxel(x, rc.x), ty=maxel(y, rc.y);
		sx=minel(x+sx, rc.x+rc.sx)-tx;
		sy=minel(y+sy, rc.y+rc.sy)-ty;
		if(sx<0) sx=0; if(sy<0) sy=0;
		x=tx; y=ty;
		return ;

		if(x+sx>rc.x+rc.sx) sx=(rc.x+rc.sx)-(x);
		if(y+sy>rc.y+rc.sy) sy=(rc.y+rc.sy)-(y);
		if(x<rc.x) x=rc.x; if(y<rc.y) y=rc.y;
		if(sx<0) sx=0; if(sy<0) sy=0;
	}

	SRect RAnd(SRect rc){
		SRect r(*this);
		r.And(rc);
		return r;
	}

	void Move(int _x, int _y){ x+=_x; y+=_y; }

	// info
	int GetW()const { return sx; }
	int GetH()const { return sy; }

	bool InRect(int tx, int ty)const{
		if(tx>=x && tx<x+sx && ty>=y && ty<y+sy)
			return 1;
		return 0;
	}

	bool InRect(SRect rc)const{
		if(rc.x>=x && rc.x+rc.sx<=x+sx && rc.y>=y && rc.y+rc.sy<=y+sy)
			return 1;
		return 0;
	}

	bool Is(){
		return sx && sy;
	}

	// operators
	operator MRect(){ MRect rc; rc.left=x; rc.top=y; rc.right=x+sx; rc.bottom=y+sy; return rc; }

	SRect operator+(const SRect &r){
		SRect rc=*this;
		rc.x+=r.x;
		rc.y+=r.y;
		rc.sx+=r.sx;
		rc.sy+=r.sy;
		return rc;
	}

};

// Class W(indow)Rect.
class WRect{
public:
	int left;
	int top;
	int right;
	int bottom;

	WRect(){ left=0; right=0; top=0; bottom=0; }
	WRect(int _left, int _top, int _right, int _bottom){left=_left; top=_top; right=_right; bottom=_bottom;}

	inline operator RECT*()const{ return (RECT*)this;	}
	inline operator RECT&()const{ return *(RECT*)this;	}

	void Set(int _left, int _top, int _right, int _bottom){left=_left; top=_top; right=_right; bottom=_bottom;}

	int GetW()const {return right-left;}
	int GetH()const {return bottom-top;}
	int Is()const { return right-left || bottom-top; }

	void Move(int x, int y){ left+=x; right+=x; top+=y; bottom+=y; }

	bool InRect(int x, int y)const {if(x>=left && x<=right && y>=top && y<=bottom) return 1; return 0;}
	
//	void Clear(){ left=0; top=0; right=0; bottom=0; }
	void Clean(){ left=0; top=0; right=0; bottom=0; }

	inline bool IsNull()const {if(!left && !top && !right && !bottom) return 0; return 1;}
	inline void Center(){int w=GetW()/2, h=GetH()/2; left=DCXSIZE/2-w; top=DCYSIZE/2-h; right=DCXSIZE/2+w; bottom=DCYSIZE/2+h;}
	bool Normal(){if(left<=right && top<=bottom) return 1; return 0; }
	void Union(MRect rc){
		if(left>rc.left) left=rc.left;
		if(right<rc.right) right=rc.right;
		if(top>rc.top) top=rc.top;
		if(bottom<rc.bottom) bottom=rc.bottom;
	}
	void Crop(MRect rc){
		if(left<rc.left) left=rc.left;
		if(right>rc.right) right=rc.right;
		if(top<rc.top) top=rc.top;
		if(bottom>rc.bottom) bottom=rc.bottom;
	}

};
// Actions: MRect -> SRect, SRect -> MRect


// My Extension Rectage class x,y - cx,cy
// Defines
#define MRECT_LM 1
#define MRECT_LP 2
#define MRECT_TM 4
#define MRECT_TP 8
#define MRECT_RM 16
#define MRECT_RP 32
#define MRECT_BM 64
#define MRECT_BP 128


class MExRect{
public:
	int left;
	int top;
	int right;
	int bottom;
	BYTE set;//[8];// posi & %
//	bool isext;	
 
	MExRect(){ Null(); } //left=0; right=0; top=0; bottom=0;
	MExRect(const MRect &rect){Set(rect.left, rect.top, rect.right, rect.bottom);}
	MExRect(VString rect, const int rx=0, const int ry=0){ Set(rect, rx, ry); }
	MExRect(const char *rect, const int rx=0, const int ry=0){ Set(rect, rx, ry); }
	MExRect(int _left, int _top, int _right, int _bottom, BYTE p=0){Null(); left=_left; top=_top; right=_right; bottom=_bottom; set=p;}
	
	MExRect(const SRect &rc){
		left=rc.x; top=rc.y; right=rc.x+rc.sx; bottom=rc.y+rc.sy; set=0;
	}

	bool IsResize(){
		return set>0;
	}

	void Set(int _left, int _top, int _right, int _bottom, BYTE p=0){Null(); left=_left; top=_top; right=_right; bottom=_bottom; set=p;}
	inline void Null(){ memset(this, 0, sizeof(MExRect)); return ;
	//	left=0;
	//	right=0;
	//	top=0;
	//	bottom=0;
	//	(*(BYTE*)&set)=0;
	/*	_asm{
		push		eax
		//mov         dword ptr [ebp-8],ecx 
		mov         eax,dword ptr [this] 
		mov         dword ptr [eax],0 
		//mov         eax,dword ptr [this] 
		mov         dword ptr [eax+8],0 
		//mov         eax,dword ptr [this] 
		mov         dword ptr [eax+4],0 
		//mov         eax,dword ptr [this] 
		mov         dword ptr [eax+0Ch],0 
		//mov         eax,dword ptr [this] 
		mov         byte ptr [eax+10h],0
		pop			eax
	}*/
	}

	//MExRect(MString rect){Set(rect);}
	
	bool SetXY(VString rect, const int rx=0, const int ry=0){ Null();
		VString vl, vr, vt, vd; unsigned char *ln;
		vl=PartLine(rect, rect, ",");
		vt=PartLine(rect, rect, ",");
		vr=PartLine(rect, rect, ",");
		vd=PartLine(rect, rect, ",");

		if(!vl || !vr || !vt || !vd || rect) return 0;
		dspacev(vl, 3); dspacev(vr, 3); dspacev(vt, 3); dspacev(vd, 3);
		
		left=stoi(vl); if(left<0){ left*=-1; set|=MRECT_LM; }// if(m) set|=MRECT_LM;
		ln=vl; if(rts(vl.endu(), '%', ln)){ set|=MRECT_LP; }

		right=stoi(vr); if(right<0){ right*=-1; set|=MRECT_RM; }// if(m) set|=MRECT_LM;
		ln=vr; if(rts(vr.endu(), '%', ln)){ set|=MRECT_RP; }

		top=stoi(vt); if(top<0){ top*=-1; set|=MRECT_TM; }// if(m) set|=MRECT_LM;
		ln=vt; if(rts(vt.endu(), '%', ln)){ set|=MRECT_TP; }

		bottom=stoi(vd); if(bottom<0){ bottom*=-1; set|=MRECT_BM; }// if(m) set|=MRECT_LM;
		ln=vd; if(rts(vd.endu(), '%', ln)){ set|=MRECT_BP; }

		return 1;
	}

	bool Set(VString rect, const int rx=0, const int ry=0){ Null();
		VString vl, vr, vt, vd; unsigned char *ln;
		vl=PartLine(rect, rect, ",");
		vr=PartLine(rect, rect, ",");
		vt=PartLine(rect, rect, ",");
		vd=PartLine(rect, rect, ",");

		if(!vl || !vr || !vt || !vd || rect) return 0;
		dspacev(vl, 3); dspacev(vr, 3); dspacev(vt, 3); dspacev(vd, 3);
		
		left=stoi(vl); if(left<0){ left*=-1; set|=MRECT_LM; }// if(m) set|=MRECT_LM;
		ln=vl; if(rts(vl.endu(), '%', ln)){ set|=MRECT_LP; }

		right=stoi(vr); if(right<0){ right*=-1; set|=MRECT_RM; }// if(m) set|=MRECT_LM;
		ln=vr; if(rts(vr.endu(), '%', ln)){ set|=MRECT_RP; }

		top=stoi(vt); if(top<0){ top*=-1; set|=MRECT_TM; }// if(m) set|=MRECT_LM;
		ln=vt; if(rts(vt.endu(), '%', ln)){ set|=MRECT_TP; }

		bottom=stoi(vd); if(bottom<0){ bottom*=-1; set|=MRECT_BM; }// if(m) set|=MRECT_LM;
		ln=vd; if(rts(vd.endu(), '%', ln)){ set|=MRECT_BP; }

	/*
		MString tmp; int pos=0, lpos=0, m=0;
		//block	one
		if(!rts(rect, ',', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); dspacea(tmp, 3); if(tmp[0]=='-'){m=1; tmp=tmp.Mid(1);} left=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){left=rx-left; set|=1;} if(left<0){left*=-1; set|=1;} if(m)set|=1;  lpos=0; if(rts(tmp, '%', lpos)){set|=2;}
		lpos=++pos; m=0;
		//block	two
		if(!rts(rect, ',', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); dspacea(tmp, 3); if(tmp[0]=='-'){m=1; tmp=tmp.Mid(1);} top=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){top=ry-top; set|=4;} if(top<0){top*=-1; set|=4;} if(m)set|=4;  lpos=0; if(rts(tmp, '%', lpos)){set|=8;}
		lpos=++pos; m=0;
		//block	three
		if(!rts(rect, ',', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); dspacea(tmp, 3); if(tmp[0]=='-'){m=1; tmp=tmp.Mid(1);} right=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){right=rx-right; set|=16;} if(right<0){right*=-1; set|=16;} if(m)set|=16;  lpos=0; if(rts(tmp, '%', lpos)){set|=32;}
		lpos=++pos; m=0;
		//block	four
		if(rts(rect, ',', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); dspacea(tmp, 3); if(tmp[0]=='-'){m=1; tmp=tmp.Mid(1);} bottom=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){bottom=ry-bottom; set|=64;} if(bottom<0){bottom*=-1; set|=64;} if(m)set|=64;  lpos=0; if(rts(tmp, '%', lpos)){set|=128;}
		lpos=++pos;	*/
		return 1;
	}

/*		bool Set(MString rect, int rx=0, int ry=0){Null();
		MString tmp; int pos=0, lpos=0;
		//block	one
		if(!rts(rect, ',', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); left=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){left=rx-left; set&=1;[0]=1;} lpos=0; if(rts(tmp, '%', lpos)){set[1]=1;}
		lpos=++pos;
		//block	two
		if(!rts(rect, '-', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); top=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){top=ry-top; set[2]=1;} lpos=0; if(rts(tmp, '%', lpos)){set[3]=1;}
		lpos=++pos;
		//block	three
		if(!rts(rect, ',', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); right=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){right=rx-right; set[4]=1;} lpos=0; if(rts(tmp, '%', lpos)){set[5]=1;}
		lpos=++pos;
		//block	four
		if(rts(rect, '-', pos)){Null(); return 0;}
		tmp=rect.Mid(lpos, pos-lpos); bottom=stoi(tmp); lpos=0; if(rts(tmp, '$', lpos)){bottom=ry-bottom; set[6]=1;} lpos=0; if(rts(tmp, '%', lpos)){set[7]=1;}
		lpos=++pos;	
	}  */

	//inline operator MRect(){}
	//inline operator RECT(){ return (RECT*)this;	}

	//int l(int rx){ return set[0] ? (set[1] ? (100-left)*rx/100 : rx-left) : (set[1] ? (left)*rx/100 : left);}
	//int t(int ry){ return set[2] ? (set[3] ? (100-top)*ry/100 : ry-top) : (set[3] ? (top)*ry/100 : top);}
	//int r(int rx){ return set[4] ? (set[5] ? (100-right)*rx/100 : rx-right) : (set[5] ? (right)*rx/100 : right);}
	//int b(int ry){ return set[6] ? (set[7] ? (100-bottom)*ry/100 : ry-bottom) : (set[7] ? (bottom)*ry/100 : bottom);}

	int l(int rx)const { return set&1 ? (set&2 ? (100-left)*rx/100 : rx-left) : (set&2 ? (left)*rx/100 : left);}
	int t(int ry)const { return set&4 ? (set&8 ? (100-top)*ry/100 : ry-top) : (set&8 ? (top)*ry/100 : top);}
	int r(int rx)const { return set&16 ? (set&32 ? (100-right)*rx/100 : rx-right) : (set&32 ? (right)*rx/100 : right);}
	int b(int ry)const { return set&64 ? (set&128 ? (100-bottom)*ry/100 : ry-bottom) : (set&128 ? (bottom)*ry/100 : bottom);}
	
	int GetW(const int rx)const{return r(rx)-l(rx);}
	int GetH(const int ry)const{return b(ry)-t(ry);}

	RECT GetRect(int rx, int ry){RECT rect={l(rx), t(ry), r(rx), b(ry)}; return rect;}
	MRect GetMRect(int rx, int ry){MRect rect; rect.Set(l(rx), t(ry), r(rx), b(ry)); return rect;}

	MExRect& operator=(const MRect &rect){Set(rect.left, rect.top, rect.right, rect.bottom); return *this;}

	operator MRect()const {MRect mr(l(0), t(0), r(0), b(0)); return mr;}

	bool InRect(int x, int y, int rx, int ry)const{if(x>=l(rx) && x<=r(rx) && y>=t(ry) && y<=b(ry)) return 1; return 0;}
	
	inline bool IsNull()const{if(!left && !top && !right && !bottom) return 0; return 1;}
//	inline void Center(){int w=GetW()/2, h=GetH()/2; left=DCXSIZE/2-w; top=DCYSIZE/2-h; right=DCXSIZE/2+w; bottom=DCYSIZE/2+h;}


};

//////////////////////////////////////////////////////////////////////////////////////////
///// MCursor - My Cyrsor ////////////////////////////////////////////////////////////////
//#ifdef WIN32

#ifndef WIN32
HCURSOR LoadCursor( HINSTANCE hInstance, LPCSTR lpCursorName){ return 0; }
#define DEFAULT_CHARSET         1
#endif

class MCursor{
HANDLE h;
public:
	MCursor(){}
	MCursor(MODLPWSTR c){ h=0;
#ifndef WINCE
		h=LoadCursor(0, c);
#endif
}
//	MCursor(MODLPWSTR c){h=LoadCursor(0, c);}

inline operator HCURSOR(){return (HCURSOR)h;}
inline HANDLE operator=(HCURSOR c){return h=c;}
#ifdef WINCE
inline MODLPWSTR operator=(MODLPWSTR c){ return c;}
#else
inline MODLPWSTR operator=(MODLPWSTR c){h=LoadCursor(0, c); return c;}
#endif
//inline char* operator=(char* c){h=LoadCursor(0, MODUNICODE(VString(c))); return c;}
//inline LPWSTR operator=(LPWSTR c){h=LoadCursor(0, c); return c;}
inline MCursor(HCURSOR c){h=c;}
};
//#endif

#ifndef WIN32
typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
typedef struct tagRGBQUAD RGBQUAD;
typedef DWORD   COLORREF;

#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

//////////////////////////////////////////////////////////////////////////////////////////
unsigned short RGB16(RGBQUAD &rgb){
	unsigned short rgb565 = (0x1f & (rgb.rgbBlue >> 3)) | (0x7e0 & (rgb.rgbGreen << 3)) | (0x1f000 & (rgb.rgbRed << 8)); return rgb565; }
RGBQUAD RGB16R(unsigned short col){
	RGBQUAD rgb;
	rgb.rgbBlue=(col&0x1f)<<3;
	rgb.rgbGreen=(col&0x7e0)>>3;
	rgb.rgbRed=(col&0x1f000)>>8;
	return rgb;
}
COLORREF RGB16C(unsigned short col){
	return RGB((col&0x1f000)>>8, (col&0x7e0)>>3, (col&0x1f)<<3);
}

///// MRGB - My Red Green Blue ///////////////////////////////////////////////////////////
struct MRGB{
	BYTE	blue;//red;
	BYTE	green;
	BYTE	red;//blue;	

	MRGB(){ }
	MRGB(BYTE r, BYTE g, BYTE b){ red=r; green=g; blue=b; }
	MRGB(const int color){ red=GetRValue(color); green=GetGValue(color); blue=GetBValue(color); }
	MRGB(const unsigned short cl){ unsigned int c=RGB16C(cl); red=GetRValue(c); green=GetGValue(c); blue=GetBValue(c); }
	MRGB(const char* cl){ set(cl); return ; }
	MRGB(const VString cl){ set(cl); return ; }
	MRGB(const RGBQUAD cl){ red=cl.rgbRed; green=cl.rgbGreen; blue=cl.rgbBlue; }

	inline void set(BYTE r, BYTE g, BYTE b){ red=r; green=g; blue=b; }
	
	inline void set(const char *ln){ set((unsigned char*)ln); return ; }
	inline void set(const VString cl){
		if (cl.size()<6) { blue=0; green=0; red=0; return ;}
		set(cl.uchar()); return;}

	inline void set(unsigned char *ln){ if(!ln){ red=0; green=0; blue=0; return ; } char c;
		c=upd(*ln); ln++; red=((c>47 && c<58) ? c-48 : c-55)*16;
		c=upd(*ln); ln++; red+=((c>47 && c<58) ? c-48 : c-55)%16;
		c=upd(*ln); ln++; green=((c>47 && c<58) ? c-48 : c-55)*16;
		c=upd(*ln); ln++; green+=((c>47 && c<58) ? c-48 : c-55)%16;
		c=upd(*ln); ln++; blue=((c>47 && c<58) ? c-48 : c-55)*16;
		c=upd(*ln); ln++; blue+=((c>47 && c<58) ? c-48 : c-55)%16;
	return ;}
	
	void operator=(const int color){
		red=GetRValue(color); green=GetGValue(color); blue=GetBValue(color);
		return; }

	void operator=(const VString& cl){
		if(cl.size()!=6){ blue=0; green=0; red=0; return ; }
		set(cl);
	return;}

	void operator=(const char*line){ return set(line); return; }

	int operator==(const int color){
		if(RGB(red, green, blue)==color) return 1; return 0; }

	int operator==(const MRGB color){
		if(color.red==red && color.green==green && color.blue==blue)return 1; return 0;}

	operator RGBQUAD*(){return (RGBQUAD*)this;}
	operator RGBQUAD&(){return *(RGBQUAD*)&blue;}
	operator unsigned int(){ return RGB(red, green, blue); }
	operator unsigned short(){ return sh(); }
	unsigned short sh()const { return RGB16(*(RGBQUAD*)this); }
	//RGBQUAD shr()const { return RGB16R()this); }
	//COLORREF shc()const { return RGB16C(*(RGBQUAD*)this); }

	MString GetString(){
		MString ret; ret.Reserv(6); char*r=ret;
		*r=red/16+( red/16 < 10 ? '0' : 55 ); *r++;
		*r=red%16+( red%16 < 10 ? '0' : 55 ); *r++;
		*r=green/16+( green/16 < 10 ? '0' : 55 ); *r++;
		*r=green%16+( green%16 < 10 ? '0' : 55 ); *r++;
		*r=blue/16+( blue/16 < 10 ? '0' : 55 ); *r++;
		*r=blue%16+( blue%16 < 10 ? '0' : 55 ); *r++;
		return ret;
	}

};


#define LEFT	0
#define TOP		0
#define CENTER	1
#define RIGHT	2
#define VCENTER	4
#define BOTTOM	8

/*
#define LEFT 0
#define RIGHT 1
#define CENTER 2
#define TOP 1
#define BOTTOM 2

#define ILEFT 0
#define IRGHT 1
#define ICENTER 2
#define ITOP 4
#define IBOTTOM 5
#define IVCENTER 6
*/
//#define FONT_FULLWORDS 16
//#define FONT_CUTWORDS 16
#define FONT_TPOINT 16

#define FONT_MULTILINE 32
#define FONT_AUTOLINE 64
//#define FONT_AUTOILINE 64

////////////////////////////////////////////////////////////////////////////////////////////////////
///// MFont - My Font //////////////////////////////////////////////////////////////////////////////
class MFont{
public:
//	BYTE align;
//	BYTE valign;
	BYTE style;
//	BYTE for_other_any_data;

    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    WCHAR lfFaceName[32];
	MRGB color, bgcolor;

public:
	MFont(){
		memset(this, 0, sizeof(MFont)); style=0; SetFont("Times New Romain"); lfHeight=20; lfWeight=200;
		color=0; bgcolor=1;
		lfCharSet=DEFAULT_CHARSET;
	}
	MFont(const LOGFONT &font){ style=0; *(LOGFONT*)&lfHeight=font; }

	void SetFont(VString font, LONG h=0){
		if(!font || font.sz>=32)
			return;

		for(int i=0; i<font.sz; i++){
			lfFaceName[i]=font[i];
		}
		lfFaceName[font.sz]=0;

		//if(font) memcpy(lfFaceName, font, font);
		if(h) lfHeight=h;
		return ;
	}

	VString GetFont(){ return VString((char*)lfFaceName, flen()*2); }

	int flen(){
	 	for(int i=0; i<16; i++){
			if(lfFaceName[i]==0) return i;
		}
		return 15;
	 }

	void SetStyle(BYTE st){ style=st; return ; }
	void NullFn(){ memset(lfFaceName, 0, sizeof(lfFaceName)); return ; }
//	MString Font(){return low(lfFaceName);}
	
	operator LOGFONT*(){ return (LOGFONT*)&lfHeight; }

};


class MEditCursor{public:
int x, y, h;
int cx, cy, ch;
bool paint; bool alt; unsigned short sz;
MRGB col;
MEditCursor(){memset(this, 0, sizeof(MEditCursor));}
void Clear(){memset(this, 0, sizeof(MEditCursor));}
};

//#define MWK_SHIFT	16
//#define MWK_CTRL	17


#define MK_BACKSPASE			8
#define MK_TAB			9
#define MK_ENTER			13

#undef MK_SHIFT
#define MK_SHIFT			16
#define MK_CTRL			17
#define MK_ESCAPE			27

#define MK_PGUP				33
#define MK_PGDOWN			34
#define MK_END				35
#define MK_HOME				36

//#define MK_LEFT				37
#define MK_LEFT				(S64K+37)
//#define MK_UP				38
#define MK_UP				(S64K+38)
//#define MK_RIGHT			39
#define MK_RIGHT			(S64K+39)
//#define MK_DOWN				40
#define MK_DOWN				(S64K+40)


#define MK_DELETE			46


// use xcode [create ~11.09.2011]
#define MK_F1				(S64K+112)
#define MK_F2				(S64K+113)
#define MK_F3				(S64K+114)
#define MK_F4				(S64K+115)
#define MK_F5				(S64K+116)
#define MK_F6				(S64K+117)
#define MK_F7				(S64K+118)
#define MK_F8				(S64K+119)
#define MK_F9				(S64K+120)
#define MK_F10				(S64K+121)
#define MK_F11				(S64K+122)
#define MK_F12				(S64K+123)

