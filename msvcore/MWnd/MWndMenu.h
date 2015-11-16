//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////// MWndElementMenu	//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class MWndMenuEl{
public:
	MString text;
	int y, cy;
	int uid;
};


class MWndMenu: public MWnd{
public:
	//MWndMenuLine menu;
    //MEdit tedit;
	//MEditCursor mcur;
	MList<MWndMenuEl> menu;

	int selected, isclicksel, selectedid;
	int mx, my;
	MFont font;
	MRGB selcolor;
	HWND shwnd;

	MWndMenu(){
		mstyle.bgcolor=MRGB("ffffff"); selected=-1; isclicksel=0; mx=0; my=0;
		mstyle.font.style=LEFT|TOP|FONT_MULTILINE; mstyle.font.lfHeight=16; mstyle.font.color="000000";
		mstyle.border.Set(1, 1, MWND_BORDER_DRAW, MRGB("0088cb"));
		selcolor=MRGB("c8e2ee");
		mousecursor=IDC_ARROW;
	}

	void Create(int x, int y, HWND hwnd=0, int uid=0){
		shwnd=hwnd;
		int sx=DCXSIZE, sy=DCYSIZE; SIZE sz; mx=0; my=0;
		
		UGLOCK(menu);
		MWndMenuEl *el=menu.First();
		while(el){
			SIZE sz=MWndIPE::GetTextSize(el->text, mstyle.font, mydefaultfont);
			if(mx<sz.cx) mx=sz.cx;
			el->y=my; el->cy=sz.cy; my+=sz.cy;
			el=menu.Next(el);
		}
		
		/*D_MWndMenuLine* ml=menu._a;
		while(ml){
			sz=tedit.GetTextSize(ml->text);
			if(mx<sz.cx) mx=sz.cx;
			ml->y=my; ml->cy=sz.cy; my+=sz.cy;
			ml=ml->_n;
		}*/

		my+=3; mx+=2;

		if(mx<10) mx=10;
		if(my<10) mx=10;
		if(x+mx+10>sx){ x=sx-mx-10; }
		if(y+my+10>sy){ y=sy-my-10; }

		MWnd::Create("MWndMenu", WS_VISIBLE | WS_POPUP, 0, MExRect(x, y, x+mx, y+my), 0, 0, uid, 0); 
		//mstyle.border.Set(1, 1, MWND_BORDER_DRAW, MRGB(0, 0, 0));
		SwitchWindow(1);
		::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|WS_EX_TOPMOST); isclicksel=0;
		Do();
		
	}

	void Insert(MString text, int pos=S2GM, int uid=0){
		UGLOCK(menu);
		MWndMenuEl *el=menu.New();
		el->text=text; el->uid=uid;
		//D_MWndMenuLine* ml=menu.insert(pos);
		//tedit.SetWindowText(text, ml->text);
		//ml->uid=uid;
	}


	//virtual void OnDeactivate(){ SendMessage(GetWindowHWnd(), WM_CLOSE, 0, 0); }
/*
	virtual bool DrawWindow(){//if(!(style&WS_VISIBLE)) return 0;
		MWnd::DrawWindow();
		
		D_MWndMenuLine* ml=menu._a; int i=0;
		while(ml){
			if(i==selected) DrawLine(0, ml->y, mx-2, ml->y+ml->cy, selcolor, 1);
			DrawText(ml->text, tedit.mstyle.font, GetClientRect(), MRect(0, ml->y, mx, ml->y+ml->cy), mcur);
			ml=ml->_n; i++;
		}

	   //MRect mr; GetWindowRect
	   //RePaintWindow(MRect(0, 0, mx, my));
	//MWnd::PaintWindow();
	//DrawText("TEXT", GetClientRect(), "000000");

	return 1;
	}*/

	virtual int PaintWindow(){//if(!(style&WS_VISIBLE)) return 0;

		UGLOCK(menu);
		MWndMenuEl *el=menu.First();  int i=0;
		while(el){
			if(i==selected) ipe.DrawBox(0, el->y, mx-2, el->y+el->cy, MRect(0, 0, 9999, 9999), selcolor);
			else ipe.DrawBox(0, el->y, mx-2, el->y+el->cy, MRect(0, 0, 9999, 9999), mstyle.bgcolor);
			ipe.DrawText(el->text, mydefaultfont, MRect(0, el->y, mx, el->y+el->cy), mstyle.font.color, mstyle.font.lfHeight);

			el=menu.Next(el); i++;
		}
		
		/*D_MWndMenuLine* ml=menu._a; int i=0;
		while(ml){
			if(i==selected) DrawLine(0, ml->y, mx-2, ml->y+ml->cy, selcolor, 1);
			DrawText(ml->text, tedit.mstyle.font, GetClientRect(), MRect(0, ml->y, mx, ml->y+ml->cy), mcur);
			ml=ml->_n; i++;
		}*/

	   //MRect mr; GetWindowRect
	   //RePaintWindow(MRect(0, 0, mx, my));
	//MWnd::PaintWindow();
	//DrawText("TEXT", GetClientRect(), "000000");

		MWnd::PaintWindow();
		return 1;
	}


	virtual bool OnKeyDown(UINT key, UINT flag){
		int nsel=selected;
		if(key==MK_UP){ nsel--; if(nsel<0) nsel=menu.Size()-1; OnSelected(nsel); }
		else if(key==MK_DOWN){ nsel++; if(nsel>menu.Size()-1) nsel=0; OnSelected(nsel); }
		else if(key==MK_ENTER){
				
			UGLOCK(menu);
			MWndMenuEl *el=menu.First();  int i=0;
			while(el){
					if(el->uid==selected)
						SendMessage(shwnd, WM_COMMAND, el->uid, 0);
				el=menu.Next(el); i++;
			}
				
			SendMessage(GetHWND(), WM_CLOSE, 0, 0);
		}
		
		return MWnd::OnKeyDown(key, flag);
	}

	virtual bool OnMouseMove(unsigned int keys, unsigned short cx, unsigned short cy){
		UGLOCK(menu);
		MWndMenuEl *el=menu.First(); int i=0;
		while(el){
			if(cy>=el->y && cy<el->y+el->cy) {
				OnSelected(i, el->uid); break;
			}
			el=menu.Next(el); i++;
		}
 		return MWnd::OnMouseMove(keys, cx, cy);
	}

	virtual bool OnMouseLeave(){
		OnSelected(-1, -1);
		return MWnd::OnMouseLeave();
	}

	virtual bool OnMouseClick(unsigned int keys, unsigned short cx, unsigned short cy){
		UGLOCK(menu);
		MWndMenuEl *el=menu.First(); int i=0;
		while(el){
			if(cy>=el->y && cy<el->y+el->cy) {
				OnSelected(i, el->uid); isclicksel=1;
				SendMessage(shwnd, WM_COMMAND, el->uid, 0);
				SendMessage(GetHWND(), WM_CLOSE, 0, 0);
			}
			el=menu.Next(el);
		}

		//D_MWndMenuLine* ml=menu._a; int i=0;
		//while(ml){
		//	if(cy>=ml->y && cy<ml->y+ml->cy) {
		//		OnSelected(i, ml->uid); isclicksel=1;
		//		SendMessage(shwnd, WM_COMMAND, ml->uid, 0);
		//		SendMessage(GetWindowHWnd(), WM_CLOSE, 0, 0);
		//	break; }
		//	ml=ml->_n; i++;
		//}
 		return MWnd::OnMouseClick(keys, cx, cy);
	}

	virtual void OnSelected(UINT id, UINT sid=0){
		if(selected!=id){
		print("S: ", itos(id), "\r\n");
			selected=id; selectedid=sid;
	//	DrawWindow(); AnPaint();
		PaintWindow();
		}
		return ;
	}

	virtual int GetSelected(){ if(isclicksel) return selected; return -1; }
	virtual int GetSelectedId(){ if(isclicksel) return selectedid; return -1; }

	virtual void OnDeactivate(){
		MWnd::OnDeactivate();
		SendMessage(GetHWND(), WM_CLOSE, 0, 0);
		SendMessage(GetHWND(), WM_NULL, 0, 0);
		return ;
	}


};
