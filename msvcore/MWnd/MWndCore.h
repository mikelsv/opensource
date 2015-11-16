/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////// MWnd - My Windows ///////////////////////////////////////////////////////////////////////////////////////////
/////////// Ver 0.1.0.0 21.05.2007 : 13:09 //////////////////////////////////////////////////////////////////////////////
/////////// Ver 0.1.1.0 07.10.2007 : 14:43 //////////////////////////////////////////////////////////////////////////////
/////////// Ver 0.2.0.0 16.05.2008 : 19:19 //////////////////////////////////////////////////////////////////////////////
/////////// Ver 0.3.0.0 10.02.2011 : 10:09 //////////////////////////////////////////////////////////////////////////////
/////////// Ver 0.3.1.0 28.08.2012 : 17:04 //////////////////////////////////////////////////////////////////////////////
/////////// Ver 0.4.0.0 20.10.2013 : 03:11 //////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MWND_VER "0.4.0.0"


class MWnd;

LRESULT WINAPI MWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


// Register Class
ATOM MWndClassRegister(HINSTANCE hInstance){
#ifndef WINCE
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIconSm		= 0;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	wcex.lpszClassName	= MODUNICODET("MWND");	
#else
	WNDCLASS wcex;
	wcex.lpszClassName	= MODUNICODE("MWND");
#endif
	wcex.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
#ifndef MSV_HWND_USEL3
	wcex.lpfnWndProc	= (WNDPROC)MWndProc;
#else
	wcex.lpfnWndProc	= (WNDPROC)MWndProc3;
#endif
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0; //LoadIcon(hInstance, (LPCTSTR)IDI_PRINTX);
	wcex.hCursor		= 0; //LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= 0; //(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//(LPCSTR)IDC_PRINTX;

#ifndef WINCE
	return RegisterClassEx(&wcex);
#else
	return RegisterClass(&wcex);
#endif	
}


// Initialize
void MWndInitialize(){
	MWndClassRegister(::GetModuleHandle(0));
	//memset(&mwndgstyle, 0, sizeof(MWndStyle));
	//mwndgstyle.use=1;
	//mwndgstyle.Clear();
	//mwndgstyle.visible=1;
	//mwndgstyle.enable=1;
	//mwndgstyle.bgcolor.set("d8e9b5");//FFF070
	//mwndgstyle.border.brcolor.set("e9a090");
	//mwndgstyle.font="Times New Roman";
	//mwndgstyle.fontcolor.set("000000");

	//MString path; path.Reserv(256);
	//path=GetWindowsDirectory(); path+="\\MWnd\\messages.conf";
	//windowsmessages.Open(path);
	return ;
}

// Variables
HINSTANCE thishinstance;
int __mwndtime=0;

DWORD MWndProc(LPVOID);
void RunMWndProc(){ StartThread(MWndProc); return ; }


// Start
class MWndStart{
public:
	MWndStart(){
		MWndInitialize();
		RunMWndProc();
		thishinstance=GetModuleHandle(0);
		//Sleep(10);
	}
};

MWndStart _mwndstartandnomorecreatethisunixparametherandmoremoremore;

void MWndWindowsAddWindow(MWnd *mwnd);
void MWndWindowsDelWindow(MWnd *mwnd);

// Child Windows
class MWndChild_d{
public:
	MWndChild_d *_p, *_n;
	MWnd *mwnd;
};

class MWndChild : public OMatrixT<MWndChild_d>, public TLock{
public:

	MWndChild(){}
	~MWndChild(){ Clear(); }

	void Add(MWnd *mwnd){
		UGLOCK(this);
		MWndChild_d *p=new MWndChild_d;
		p->mwnd=mwnd;
		OMAdd(p);
		return ;
	}

	void Del(MWnd *mwnd){
		UGLOCK(this);
		
		for(MWndChild_d *p=_a; p; p=p->_n){
			if(p->mwnd==mwnd){ OMDel(p); delete p; return ; }
		}

		return ;		
	}

	void Clear(){ OMClear(); }
	
};


// MWnd
class MWnd : public TLock{
protected:
	// Windows List
	MWnd *_p, *_n;
	// Window handle
	HWND hwnd;
	// Parent
	MWnd *parent;
	// Childrens
	MWndChild child;

	// Rect & Client Rect
	MRect rect, clrect;

	// Style
	MWndStyle mstyle;
	int style;
	
	// image paint element
	MWndIPE ipe;

	// Mouse Cursor
	MCursor mousecursor;

public:
	MWnd(){
		_p=0; _n=0; parent=0; style=0; hwnd=0;
		mousecursor=IDC_ARROW;
		MWndWindowsAddWindow(this);
	}
	~MWnd(){
		if(parent) parent->DelChildWnd(this);
		// +++ ADD del childs!!!!
		MWndWindowsDelWindow(this);
	}

	// Get
	HWND GetHWND(){
		UGLOCK(this);
		if(hwnd) return hwnd;
		if(parent) return parent->GetHWND();
		return 0;
	}

	MRect GetWindowRect(){
		MRect rc(0,0, rect.GetW(), rect.GetH());
		return rc;
	}

	MRect GetClientRect(){
		MRect rc(0,0, clrect.GetW(), clrect.GetH());
		return rc;
	}

	MRect GetWindowPos(){
		return rect;
	}


// Modify
	void SetCursor(HCURSOR hCursor){
#ifndef MSV_USEXLIB
		::SetCursor(hCursor);
#else
		if(hwnd) XDefineCursor(mwnddisp, hwnd, hCursor);
#endif
		//	return (HCURSOR)::SendMessage(hwnd, BM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}
	HCURSOR GetCursor(){
		//SetClassLong(hWnd,GCL_HCURSOR,(LONG)normalcur);
		return (HCURSOR)::SendMessage(hwnd, BM_GETIMAGE, IMAGE_CURSOR, 0L);
	}


// Add Window
	void AddChildWnd(MWnd *mwnd){
		UGLOCK(this);
		child.Add(mwnd);
		return ;
	}

	void DelChildWnd(MWnd *mwnd){
		UGLOCK(this);
		child.Del(mwnd);
		return ;
	}

// Create
	virtual HWND Create(MString name, DWORD stl, DWORD exstyle, MExRect rc, MWnd *prnt=0, HINSTANCE hinstance=0, unsigned int uid=0, LPVOID lpParam=0, HWND hwparent=0){
		UGLOCK(this);

		HWND p=0; rect=rc; parent=prnt; style=stl;

		if(parent){
			p=parent->GetHWND();
			style|=WS_CHILD; //mstyle=parent->mstyle; /*parent->SetupElement()->elw=this;*/
		}
		if(hwparent){
			p=hwparent;
			style|=WS_CHILD;
		}
		//	RecheckRect();

		hwnd=MWndCreateWindow(exstyle, MODUNICODE("MWND"), MODUNICODE(name), style, rect, p, 0, thishinstance /*hinst*/, 0 /*visual lpParam*/);
		if(!hwnd) return 0;

		if(hwnd) IPENew(rect.GetW(), rect.GetH());
		//DMWndWindows dl(mywindows); dl=mywindows.Insert(); dl->mwnd=this; dl->hwnd=hwnd; sptext=name;
		//type=0; _uid=uid; if(!_uid) _uid=NewUID(); if(hwnd) IPENew(trect.GetW(), trect.GetH()); // if(!hwnd) ImageDestroy(_ipe);
		//thid=GetCurrentThreadId(); RecheckRect();

		if(prnt){ prnt->AddChildWnd(this); }

		MRect mr; ::GetClientRect(hwnd, mr); clrect=mr;

		SetCursor(mousecursor);

		SendMessage(hwnd, WM_CREATE, 0, 0);

		return hwnd;
	}

	// Destroy Window
	virtual int DestroyWindow(){
		UGLOCK(this);
		if(hwnd) ::MWndDestroyWindow(hwnd);
		hwnd=0;

		if(parent){ parent->DelChildWnd(this); }
		return 0;
	}


// Do
	int Do(){
		MSG Message = {0}; MTimer timer;
		double old_time = timer.dtime(), new_time=0, max_time=0, frame_time=timer.dtime();// oldFPSTime = old_time;

		while(Message.message!=WM_QUIT){
			if(PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE)){
				if(GetMessage(&Message, NULL, 0, 0)) {
					TranslateMessage(&Message);
					DispatchMessage(&Message);      
				} 
			}else{
				new_time = timer.dtime();
				if(new_time - frame_time > 1000.0){
					opengl_frames=opengl_frame; opengl_frame=0;
					if(new_time - frame_time < 10000.0) frame_time+=1000.0; else frame_time=new_time;
			//	fframesCnt = framesCnt; framesCnt = 0; //oldFPSTime = new_time;
				}
				if((new_time - old_time > 1000.0 / OPENGL_MAX_FREQ) && (new_time - old_time > max_time)){
					//++opengl_frame; //++framesCnt;
					max_time=OnImpulseTime(new_time - old_time);
					if(max_time<0) max_time=OPENGL_MAX_TIME;
					//display(new_time - old_time);
					old_time = new_time;

					//g_pBump->RenderMap(rand()%200, rand()%200);
					//PostMessage(g_hMainWindow, WM_PAINT, NULL, 0);
					//OnDo();
				}
				else 
					Sleep(1);
			}
		}
		return Message.wParam;
	}


// IPE
	int IPENew(int cx, int cy){
		if(cx>=0 && cy>=0) ipe.New(cx, cy);
		return 1;
	}

// Paint Window
	virtual int PaintWindow(){
		InvalidateRect(hwnd, rect, 0);

		if(!ipe.Is() || !hwnd) return 1;
		HDC dc = GetDC(hwnd);
		ipe.Draw(hwnd, dc, GetWindowRect());
		ReleaseDC(hwnd, dc);
		return 1;
	}


	// show window
#ifndef WINCE
	void SwitchWindow(bool alttab){SwitchToThisWindow(GetHWND(), alttab); }
#else
	void SwitchWindow(bool alttab){ if(alttab) focus=this;}
#endif


// On
	virtual LRESULT DefWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){ return 0; }
	virtual int OnImpulseTime(int ms){ return 500; }

// On
	virtual void OnActivate(){
	//	ParentTranslate(GetUID(), WM_ACTIVE, 0, 0); return ;
	}
	virtual void OnDeactivate(){return ;}
	virtual bool OnCreate(CREATESTRUCT *create){
	//	ParentTranslate(GetUID(), WM_CREATE, 0, (LPARAM)create);
		return 1;
	}

	virtual bool OnSize(UINT type, unsigned short cx, unsigned short cy);
	virtual bool OnSize(){ return 0; }
//	virtual bool OnSizing(UINT nType, MRect*rc);
//	virtual bool OnMove(UINT nType, unsigned short cx, unsigned short cy);
//	virtual bool OnMoving(UINT nType, MRect*rc);

	virtual bool OnLButtonDown(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_LBUTTONDOWN, keys, MAKELONG(cx, cy));
		return 0;
	}
	virtual bool OnRButtonDown(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_RBUTTONDOWN, keys, MAKELONG(cx, cy));
		return 0;
	}
	virtual bool OnLButtonUp(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_LBUTTONUP, keys, MAKELONG(cx, cy));
		return 0;
	}
	virtual bool OnRButtonUp(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_RBUTTONUP, keys, MAKELONG(cx, cy));
		return 0;
	}
	virtual bool OnMouseClick(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_LBUTTONCLICK, keys, MAKELONG(cx, cy));
		return 0;
	}
	virtual bool OnMouseRClick(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_RBUTTONCLICK, keys, MAKELONG(cx, cy));
		return 0;
	}

	virtual bool OnNCLButtonDown(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_NCLBUTTONDOWN, keys, MAKELONG(cx, cy));
		return 0;
	}

	virtual bool OnMouseDoubleClick(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_LBUTTONDBLCLK, keys, MAKELONG(cx, cy));
	return 0;
	}

	virtual bool OnMouseNoClick(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_LBUTTONNOCLICK, keys, MAKELONG(cx, cy));
	return 0;
	}

	virtual bool OnMouseRNoClick(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_RBUTTONNOCLICK, keys, MAKELONG(cx, cy));
	return 0;
	}

	virtual bool OnLButtonDblClick(unsigned int keys, unsigned short cx, unsigned short cy){
	//if(NWM(WM_LBUTTONDBLCLK, MAKELONG(cx, cy), 0)) return 1;
	//	ParentTranslate(GetUID(), WM_LBUTTONDBLCLK, keys, MAKELONG(cx, cy));
	return 0;
	}

	virtual bool OnMouseMove(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_MOUSEMOVE, keys, MAKELONG(cx, cy));
		return 0;
	}

	virtual bool OnMouseLeave(){
		//if(NWM(WM_MOUSELEAVE, 0, 0)) return 1;
	//	ParentTranslate(GetUID(), WM_MOUSELEAVE, 0, 0);
	return 0;
	}

	virtual bool OnMouseActivate(HWND h, WORD w){
		//if(NWM(WM_MOUSEACTIVATE, (WPARAM)h, w)) return 1;
	//	ParentTranslate(GetUID(), WM_MOUSEACTIVATE, (unsigned int)h, w);
	return 0;
	}

	virtual bool OnMouseDeactivate(HWND h, WORD w){
		//ParentTranslate(GetUID(), WM_MOUSEDEACTIVATE, h, w);
	return 0;
	}

	virtual bool OnMouseWheel(unsigned int keys, unsigned short cx, unsigned short cy){
	//	ParentTranslate(GetUID(), WM_MOUSEWHEEL, keys, MAKELONG(cx, cy));
		//if(NWM(WM_MOUSEWHEEL, keys, MAKELONG(cx, cy))) return 1;
	//	int z = GET_WHEEL_DELTA_WPARAM(keys);
	//	if(z>0) ScrollUp(z); else ScrollDown(-z);
		//  if(z>0) mwnd->mstyle.scroll.Up(z); else mwnd->mstyle.scroll.Down(-z);
	return 0;
	}

	virtual bool OnKeyDown(UINT key, UINT flag){
	//	ParentTranslate(GetUID(), WM_KEYDOWN, key, flag);
	//	if(key==MK_SHIFT){_spkeydownshift=1;}
	//	else if(key==MK_CTRL) _spkeydownctrl=1;
	return 0;
	}

	virtual bool OnKeyUp(UINT key, UINT flag){
	//	ParentTranslate(GetUID(), WM_KEYUP, key, flag);
	//	if(key==MK_SHIFT){_spkeydownshift=0;}
	//	else if(key==MK_CTRL) _spkeydownctrl=0;
		return 0;
	}

	virtual bool OnChar(UINT key, UINT flag){
	//	ParentTranslate(GetUID(), WM_CHAR, key, flag);
	return 0;
	}

	virtual int OnCommand(WPARAM wparam, LPARAM lparam){
		
		return 0;
	}

	virtual bool OnTimeMouseActive(){ return 1; }
	virtual bool OnTimeFocus(){ return 1; }
	//virtual bool OnTime(int uid){ ParentTranslate(GetUID(), WM_TIMER, 0, 0); return 1; }

	int ParentTranslate(unsigned int uid, unsigned int mess, WPARAM wparam, LPARAM lparam){
	//	MWndTranslated tr; tr.mwnd=this; tr.uid=uid; tr.mess=mess; tr.wparam=wparam; tr.lparam=lparam;
	//	return ParentTranslate(tr);
		return 0;
	}

	// friends
	friend class MWndWindows;
	friend class OMatrixT<MWnd>;
};


bool MWnd::OnSize(UINT type, unsigned short cx, unsigned short cy){
	if(!hwnd) return 1;

	IPENew(cx, cy);

	if(type==SIZE_MAXIMIZED){
		rect.Set(0,0,cx,cy);
	}  //rect.Set(0,0,cx/2,cy/2);}
	else if(type==SIZE_RESTORED){
		MRect r; ::GetWindowRect(GetHWND(), r); rect=r;
		//if(rect.GetW(0)!=cx){ rect.right=rect.left+cx; }
		//if(rect.GetH(0)!=cy){ rect.bottom=rect.top+cy; }
	}

	MRect mr; ::GetClientRect(hwnd, mr); clrect=mr;

	return 0;
}


// Windows List
class MWndWindows : public OMatrixT<MWnd>, public TLock{
public:

	void AddWindow(MWnd *mwnd){
		UGLOCK(this);
		OMAdd(mwnd);
	}

	void DelWindow(MWnd *mwnd){
		UGLOCK(this);
		OMDel(mwnd);
	}

	MWnd* GetWindow(HWND hwnd){
		if(!hwnd) return 0;
		for(MWnd *p=_a; p; p=p->_n){
			if(p->hwnd==hwnd) return p;
		}
		return 0;
	}

} MWndWindows;

void MWndWindowsAddWindow(MWnd *mwnd){ MWndWindows.AddWindow(mwnd); }
void MWndWindowsDelWindow(MWnd *mwnd){ MWndWindows.DelWindow(mwnd); }



// MWndProcess
DWORD MWndProc(LPVOID){


	return 0;
}


// MWndProc Values
MWnd *mwnd_mouseclick=0; 
int mwnd_mouseclickx, mwnd_mouseclicky, mwnd_mousemulticlicktime, mwnd_mousemulticlicktimemicro;

// MWndProc
LRESULT CALLBACK MWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam){
	UGLOCK(MWndWindows);

	MWnd*mwnd=MWndWindows.GetWindow(hwnd);
	if(mwnd){
		switch(message){
			case WM_SIZE:
				//mw->RecheckRect();
				//RECT rc; GetWindowRect(mw->GetHWND(), &rc);
				//rx=DCXSIZE, ry=DCYSIZE; if(mw->parent) {rx=mw->parent->trect.GetW(); ry=mw->parent->trect.GetH();}
				//mw->trect.left=mw->rect.l(rx); mw->trect.right=mw->rect.r(rx);
				//mw->trect.top=mw->rect.t(ry); mw->trect.bottom=mw->rect.b(ry);
				message=mwnd->OnSize(wparam, LOWORD(lparam), HIWORD(lparam));
				//mw->RecheckRect();
				return message;
			break;

			case WM_CREATE:
				mwnd->OnCreate((CREATESTRUCT*)lparam);
			break;



			case WM_ACTIVATE:
				if(LOWORD(wparam)==WA_ACTIVE){ mwnd->OnActivate();
				//	if(CMPO(mw->_restoret, MWND_MINIMIZE)) SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
					//mw->DrawWindow();
				//	focus=mw;
				}
				if(LOWORD(wparam)==WA_CLICKACTIVE){
					int rrr=456;
				}
				else if(LOWORD(wparam)==WA_INACTIVE){
					mwnd->OnDeactivate();
					//::SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
					//SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				}
			break;

			case WM_LBUTTONDOWN:
				mwnd_mouseclick=mwnd;
				timeb tb; ftime(&tb);
				if(((tb.time-mwnd_mousemulticlicktime)*1000+tb.millitm-mwnd_mousemulticlicktimemicro*4)<=500 && mwnd_mouseclickx==LOWORD(lparam) && mwnd_mouseclicky==HIWORD(lparam)){
					mwnd_mouseclickx=-1; mwnd->OnMouseDoubleClick(wparam, LOWORD(lparam), HIWORD(lparam));
				} else{
					mwnd_mousemulticlicktime=tb.time; mwnd_mousemulticlicktimemicro=tb.millitm/4;
					mwnd_mouseclickx=LOWORD(lparam); mwnd_mouseclicky=HIWORD(lparam);
				}
				mwnd->OnLButtonDown(wparam, LOWORD(lparam), HIWORD(lparam));
			break;

			case WM_LBUTTONUP:
				//mwnd=mw->InRect(LOWORD(lparam), HIWORD(lparam), WM_LBUTTONUP); cr=mw->CRect(LOWORD(lparam), HIWORD(lparam));
				if(!mwnd) return 0;
				if(mwnd_mouseclick==mwnd && mwnd_mouseclickx==LOWORD(lparam) && mwnd_mouseclicky==HIWORD(lparam)) mwnd->OnMouseClick(wparam, LOWORD(lparam), HIWORD(lparam));
				else {
					//if(mwnd_mouseclick) mouseclick->OnMouseNoClick(wparam, cr.x, cr.y);
				} mwnd_mouseclick=0;
				return mwnd->OnLButtonUp(wparam, LOWORD(lparam), HIWORD(lparam));
			break;

			case WM_MOUSEMOVE:
				return mwnd->OnMouseMove(wparam, LOWORD(lparam), HIWORD(lparam));
			break;

			case WM_MOUSEWHEEL:
				return mwnd->OnMouseWheel(wparam, LOWORD(lparam), HIWORD(lparam));
			break;


/*
			mw=mw->InRect(LOWORD(lparam), HIWORD(lparam), WM_LBUTTONDOWN); cr=mw->CRect(LOWORD(lparam), HIWORD(lparam));
			if(!mw) return 0; mouseclick=mw; if(focus!=mw){
				mw->OnActivate(); if(focus) focus->OnDeactivate();
			}
			focus=mw; timeb tb; ftime(&tb);// message=(tb.time)%16*1000+tb.millitm;
			if(((tb.time-mousemulticlicktime)*1000+tb.millitm-mousemulticlicktimemicro*4)<=500 && mouseclickx==LOWORD(lparam) && mouseclicky==HIWORD(lparam)){
				mouseclickx=-1; mw->OnMouseDoubleClick(wparam, cr.x, cr.y);
			}
			else {
				//mousemulticlicktime=mousemulticlickfirsttime=(tb.time%256)*1000+tb.millitm;
				mousemulticlicktime=tb.time; mousemulticlicktimemicro=tb.millitm/4;
				mouseclickx=LOWORD(lparam); mouseclicky=HIWORD(lparam);
			}
		// BORDER //
			if(((MWndElementBorder*)mw)->MWndElementBorder::InRect(cr.x+mw->clrect.left,cr.y+mw->clrect.top, WM_LBUTTONDOWN)) ((MWndElementBorder*)mw)->MWndElementBorder::OnLButtonDown(wparam, cr.x+mw->clrect.left, cr.y+mw->clrect.top);
			return mw->OnLButtonDown(wparam, cr.x, cr.y); break;
		
		case WM_LBUTTONUP:
			mw=mw->InRect(LOWORD(lparam), HIWORD(lparam), WM_LBUTTONUP); cr=mw->CRect(LOWORD(lparam), HIWORD(lparam));
			if(!mw) return 0;
			if(mouseclick==mw) if(mouseactive) mouseactive->OnMouseClick(wparam, cr.x, cr.y);
			else {
				if(mouseclick) mouseclick->OnMouseNoClick(wparam, cr.x, cr.y);
			} mouseclick=0;
			return mw->OnLButtonUp(wparam, cr.x, cr.y); break;
*/

			case WM_COMMAND:
				mwnd->OnCommand(wparam, lparam);
			break;

			case WM_PAINT:
				mwnd->PaintWindow();
				break; // LOWORD(lparam), HIWORD(lparam)
			case WM_CLOSE:
				return mwnd->DestroyWindow(); /*CloseWindow();*/
				break;
			case WM_DESTROY:
				PostQuitMessage(0); return 0;
				break;
		}

		mwnd->DefWindowProc(hwnd, message, wparam, lparam);
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}