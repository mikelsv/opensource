// My Window Visible Modification ////////////////////////
class MWndVMod{
public:
int type;
// 1 - pixcel;
// 2 - line
// 3 - circle
MString name;
};

/// My Windows Styles ///////////////////////////////////////////////////////////////////////////////////////////////////
/// My Windows Border Style /////////////////////////////////////////////////////////////////////////////////////////////
class M4BYTE{ public:
	BYTE left, top, right, bottom;
	void Clear(){(*(int*)&left)=0;}

	void operator=(int i){ left=i; top=i; right=i; bottom=i; }
	void set(int v) {left=v; top=v; right=v; bottom=v; return ;}
};

class MWndBorderStyle{
public:
BYTE type;

/*
#define MWND_LEFT 0
#define MWND_TOP 2
#define MWND_RIGHT 3
#define MWND_BOTTOM 3
*/

#define MWND_BORDER_DRAW	1 
#define MWND_BORDER_RESIZE	2
#define MWND_BORDER_DRESIZE	3
#define MWND_BORDER_UP		4
#define MWND_BORDER_DOWN	8
#define MWND_BORDER_ALLTYPE	12

//#define MWND_BORDER_LINE 1
//#define MWND_BORDER_UP 2
//#define MWND_BORDER_DOWN 4
//#define MWND_BORDER_UNKNOWN 8
//#define MWND_BORDER_USER 128


#define MWND_BORDER_USER 128

M4BYTE border;
M4BYTE resize;
MRGB brcolor;

MWndBorderStyle*user;		// MWND_BORDER_ARESIZE|MWND_BORDER_LINE

MWndBorderStyle(){memset(this, 0, sizeof(MWndBorderStyle));}
void Set(BYTE br, BYTE sz, BYTE mod=0, MRGB color=15, MWndBorderStyle*usr=0){
	memset(&border, br, 4); memset(&resize, sz, 4); if((int)color!=15) brcolor=color; user=usr; type=mod;
}

void Set(M4BYTE br, M4BYTE sz, BYTE mod=0, MRGB color=0, MWndBorderStyle*usr=0){
	 border=br; resize=sz; if((int)color!=15) brcolor=color; user=usr; type=mod;
}

void Mode(BYTE mod){type|=mod;}
void UnMode(BYTE mod){type&=(mod*-1);}



};

/// My Windows Border Style /////////////////////////////////////////////////////////////////////////////////////////////
class MWndScrollStyle{ public:
#define MWND_SCROLL_AUTO	1  // 1 - no auto
#define MWND_VSCROLL_AUTO	2  // 2 - no auto
#define MWND_SCROLL_NO_YES	4
#define MWND_VSCROLL_NO_YES	8
//#define MWND_SCROLL_YES		4
//#define MWND_VSCROLL_YES	8
#define MWND_SCROLL_RIGHT	16
#define MWND_SCROLL_TOP		32
#define MWND_SCROLL_SET		64
#define MWND_VSCROLL_SET	128


	BYTE style, trans;
	int size, vsize;
	int pos, vpos;
	
	MWndScrollStyle(){ memset(this, 0, sizeof(MWndScrollStyle)); }
	void Up(int s){ if(vpos>s) vpos-=s; else vpos=0; }
	void Down(int s){ if(vpos+s<=vsize) vpos+=s; else vpos=vsize-1; }

};


/// My Windows Style ////////////////////////////////////////////////////////////////////////////////////////////////////
class MWndStyle{ // default stule structure

// ADD Global Style Table
	// Add style parameters table 
public:
//unsigned int*use;
bool visible;
bool enable;
bool noclear;

BYTE transperent, region; // 0 - 255, 0

MWndBorderStyle border;
MWndScrollStyle scroll;

MExRect rect;

// Colors
MRGB bgcolor;
MFont font;
MRGB fontcolor;
//MString font;

//BYTE fontstyle;
//#define MWND_FONT_BOLD 1
//#define MWND_FONT_ITALIC 2


MWndVMod*vmod;

//MWndStyle(){use=new unsigned int; *use=1;}
//bool Delete(){use--; if(*use<1) return 1; return 0;}
//MWndStyle*Add(){use++; return this;}
void Clear(){
	memset(this, 0, sizeof(MWndStyle));
	return ;
}

/*
bool LoadDesign(MSVCFL &conf, VString sid){
	CFLelCur cur(conf), cur2(conf);
	cur2=cur.FindN(sid);

	if(!cur2.el().key) return 0;

	CFLel *k;
	k=&cur2.Find("visible"); if(k->key) visible=k->val.toi();
	k=&cur2.Find("border.type"); if(k->key) border.type=k->val.toi();
	k=&cur2.Find("border.border"); if(k->key) border.border.set(k->val);
	k=&cur2.Find("border.resize"); if(k->key) border.resize.set(k->val);
	k=&cur2.Find("border.color"); if(k->key) border.brcolor.set(k->val);
	k=&cur2.Find("bgcolor"); if(k->key) bgcolor.set(k->val);

	return 1;

//
//	unsigned int dtl=conf.Find(sid);
//if(dtl==-1 || !conf.conf[dtl].a==CFLN) return 0;
//MTILine&con=*conf.conf[dtl].aup;
//for(int i=0; i<con.size; i++){
//if((MString&)con[i].key=="visible") visible=((MString&)con[i].val).toi();
//
//if((MString&)con[i].key=="border.type") border.type=((MString&)con[i].val).toi();
//if((MString&)con[i].key=="border.border") border.border.set(((MString&)con[i].val).toi());
//if((MString&)con[i].key=="border.resize") border.resize.set(((MString&)con[i].val).toi());
//if((MString&)con[i].key=="border.color") border.bordercolor.set(((MString&)con[i].val));
//
//if((MString&)con[i].key=="bgcolor") bgcolor.set((MString&)con[i].val);
//}
//return 1;
}*/

//~MWndStyle(){_deln(use);}
};