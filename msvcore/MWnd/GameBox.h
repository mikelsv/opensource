// game box
class GameBox{
protected:
	MWnd *mwnd;
	MWndIPE *ipe;
	SRect rect;

private:
	// map
	int map_sx, map_sy;
	// pos
	int pos_x, pos_y;
	// mouse
	int ldown, ldx, ldy;


public:
	GameBox(){
		mwnd=0;
		ipe=0;

		map_sx = 0;
		map_sy = 0;
		pos_x = 0;
		pos_y = 0;
		ldown = 0;
	}

	virtual void OnMWnd(MWnd *m, MWndIPE &i){ mwnd=m; ipe=&i; }

	virtual void OnCreate(){ return ; }
	virtual void OnStart(const MRect rc){ rect=rc; return ; }
	virtual int OnImpulseTime(int ms){ return 50; }

	virtual bool OnLButtonDown(unsigned int keys, unsigned short cx, unsigned short cy){ ldown = 1; ldx = cx; ldy = cy; return 0; }
	virtual bool OnLButtonUp(unsigned int keys, unsigned short cx, unsigned short cy){ ldown = 0; return 0; }
	virtual bool OnMouseMove(unsigned int keys, unsigned short cx, unsigned short cy){
		if(ldown){
			pos_x += ldx - cx;
			pos_y += ldy - cy;
			ldx=cx;
			ldy=cy;
		}

		if(pos_x < 0)
			pos_x = 0;

		if(pos_y < 0)
			pos_y = 0;

		if(pos_x > map_sx - rect.GetW())
			pos_x = map_sx - rect.GetW();

		if(pos_y > map_sy - rect.GetH())
			pos_y = map_sy - rect.GetH();

		return 0;
	}
	virtual bool OnMouseClick(unsigned int keys, unsigned short cx, unsigned short cy){ return 0; }
	virtual bool OnMouseWheel(unsigned int keys, unsigned short cx, unsigned short cy){ return 0; }

	int GetWidth(){ return rect.GetW(); }
	int GetHeight(){ return rect.GetH(); }
	SRect GetRect(){ return rect; }

	void SetMapSize(int x, int y){
		map_sx = x;
		map_sy = y;
	}

	void SetMapPos(int x, int y){
		pos_x = x;
		pos_y = y;
	}

	int GetPosX(){
		return pos_x;
	}

	int GetPosY(){
		return pos_y;
	}

	void OnDrawBox(const MRect rc){ rect=rc; OnDraw(); return ; }
	virtual void OnDraw(){ return ; }

	int DrawImage(MWndIPE_C img, MRect rc, MRect to, int opt=0){
		if(!ipe) return 0;
		return ipe->DrawImage(img, rc, to, opt);
	}

	int DrawLine(int x, int y, int tx, int ty, MRect to, MRGB rgb){
		if(!ipe) return 0;
		return ipe->DrawLine(x, y, tx, ty, to, rgb);
	}
	
	int DrawBox(SRect to, MRGB rgb){
		if(!ipe) return 0;
		return ipe->DrawBox(to, rgb);
	}

	int DrawCircle(int x, int y, int r, MRGB rgb, SRect rc=SRect(0, 0, 99999, 99999)){
		if(!ipe) return 0;
		return ipe->DrawCircle(x, y, r, rc, rgb, 1);
	}

	int DrawText(VString text, SRect rc, MRGB rgb=0, int fontsize=18, int style=0){
		if(!ipe) return 0;
		return ipe->DrawText(text, mydefaultfont, rc, rgb, fontsize, style);
	}

	virtual void PaintWindow(){
		if(!mwnd) return ;
		mwnd->PaintWindow();
		return ;
	}


};



class GameBoxWnd : public MWnd{
	GameBox *box;

public:

	GameBoxWnd(GameBox &usebox){
		SetBox(usebox);
		return ;
	}

	void FictiveCreate(SRect rc){
		rect=rc; clrect=rc;
		ipe.New(rc.sx, rc.sy);
		return ;
	}

	MWndIPE& GetIpe(){
		return ipe;
	}

	void SetBox(GameBox &usebox){
		box=&usebox;
		usebox.OnMWnd(this, this->ipe);
		//memcpy(&box, &usebox, sizeof(void*));
	}

	virtual bool OnCreate(CREATESTRUCT *create){
		box->OnCreate();
		return MWnd::OnCreate(create);
	}

	int OnImpulseTime(int ms){
		int wait=box->OnImpulseTime(ms);
		PaintWindow();
		return wait;
	}

	void StartGame(){
		box->OnStart(clrect);
		PaintWindow();
	}

	virtual bool OnMouseMove(unsigned int keys, unsigned short cx, unsigned short cy){
		box->OnMouseMove(keys, cx, cy);
		return MWnd::OnMouseMove(keys, cx, cy);
	}

	virtual bool OnLButtonDown(unsigned int keys, unsigned short cx, unsigned short cy){
		box->OnLButtonDown(keys, cx, cy);
		return MWnd::OnLButtonDown(keys, cx, cy);
	}

	virtual bool OnLButtonUp(unsigned int keys, unsigned short cx, unsigned short cy){
		box->OnLButtonUp(keys, cx, cy);
		return MWnd::OnLButtonUp(keys, cx, cy);
	}

	virtual bool OnMouseClick(unsigned int keys, unsigned short cx, unsigned short cy){
		box->OnMouseClick(keys, cx, cy);
		return MWnd::OnMouseClick(keys, cx, cy);
	}

	virtual bool OnMouseWheel(unsigned int keys, unsigned short cx, unsigned short cy){
		box->OnMouseWheel(keys, cx, cy);
		return MWnd::OnMouseWheel(keys, cx, cy);
	}

	virtual int PaintWindow(){
		box->OnDrawBox(clrect);
		return MWnd::PaintWindow();
	}

};


// Game Object
#define GO_MOUSE_MOVE	1
#define GO_MOUSE_UNMOVE	2
#define GO_MOUSE_CLICK	4
#define GO_MOUSE_ALL	255
#define GO_SHOW			256
#define GO_GLOBAL		512
#define GO_DEL			S1K

#define GOMOUSE_MOVE	1
#define GOMOUSE_UNMOVE	2
#define GOMOUSE_LBUTTONDOWN	3
#define GOMOUSE_RBUTTONDOWN	4
#define GOMOUSE_LBUTTONUP	5
#define GOMOUSE_RBUTTONUP	6
#define GOMOUSE_CLICK		7
#define GOMOUSE_RCLICK		8
#define GOMOUSE_WHEEL		9

class GameObjectD{
public:
	// id
	int gid, id;
	// parent 
	GameObjectD *parent;

	// rect
	int z;
	SRect rect;

	// options
	int opt;
	int mouse, sel;

	// data
	void *el;	
		
	GameObjectD(){
		el=0; parent=0;
	}

	void SClear(){
		gid=0; id=0; z=0;
		opt=GO_MOUSE_ALL|GO_SHOW; mouse=0; sel=0;
	}

	GameObjectD* P(GameObjectD *p){
		parent=p;
		return this;
	}

	GameObjectD* Opt(int v){
		opt|=v;
		return this;
	}

	GameObjectD* NOpt(int v){
		opt&=~v;
		return this;
	}

	GameObjectD* El(void *v){
		el=v;
		return this;
	}



};

struct GameObjectFindByIdD{ 
	int gid, id;
	GameObjectFindByIdD(int g, int i){ gid=g; id=i; }
};

int GameObjectFindById(GameObjectD &l, GameObjectFindByIdD &r){
	return l.gid==r.gid && l.id==r.id;
}


class GameObject: public GameBox, public MList<GameObjectD>{
	GameObjectD *mouse_move;
public:
	int mouse_down, mouse_x, mouse_y, mouse_mx, mouse_my;
public:
	int scroll_x, scroll_y, scroll_sx, scroll_sy;

public:
	GameObject(){
		mouse_move=0;
		mouse_down=0;
		scroll_x=0; scroll_y=0; scroll_sx=0; scroll_sy=0;
	}
	
	// Set
	GameObjectD* GoSet(int gid, int id, const SRect &rc, int z=0){
		UGLOCK(this);
		GameObjectFindByIdD gof(gid, id);
		GameObjectD *p=Find((void*)GameObjectFindById, gof);
		if(!p) p=New(); 

		p->SClear(); p->gid=gid; p->id=id; p->rect=rc; p->z=z;
		return p;
	}

	GameObjectD* GoAdd(int gid, int id, const SRect &rc, int z=0){
		UGLOCK(this);
		GameObjectFindByIdD gof(gid, id);
		GameObjectD *p=0; //=Find((void*)GameObjectFindById, gof);
		if(!p) p=New(); 

		p->SClear(); p->gid=gid; p->id=id; p->rect=rc; p->z=z;
		return p;
	}


	int GoSetV(int gid, int id, void *el){
		UGLOCK(this);
		GameObjectFindByIdD gof(gid, id);
		GameObjectD *p=Find((void*)GameObjectFindById, gof);
		if(!p)
			return 0;
		p->el=el;
		return 1;
	}

	void GoShow(int gid, int id=0, int show=1){
		UGLOCK(this);
		GameObjectD *god=0, *sgod=0;
		while(god=Next(god)){
			if(god->gid==gid && (id==0 || god->id==id))
				if(show)
					god->opt|=GO_SHOW;
				else
					god->opt&=~GO_SHOW;
		}
		return ;
	}

	void GoHide(int gid, int id=0){
		return GoShow(gid, id, 0);
	}

	void GoDel(int gid, int id=0){
		UGLOCK(this);
		GameObjectD *god=0, *sgod=0;
		while(god=Next(god)){
			if(god->gid==gid && (id==0 || god->id==id))
				god=Del(god);
		}
		return ;
	}

	void GoDel(GameObjectD *gdel){
		UGLOCK(this);
		GameObjectD *god=0, *sgod=0;
		while(god=Next(god)){
			if(god==gdel)
				god=Del(god);
		}
		return ;
	}

	// Window Scroll
	void SetScroll(int x, int y){
		scroll_sx=x; scroll_sy=y;
		if(scroll_x>x-rect.sx) scroll_x=x-rect.sx;
		if(scroll_y>y-rect.sy) scroll_y=y-rect.sy;
		if(scroll_x<0) scroll_x=0;
		if(scroll_y<0) scroll_y=0;
		return ;
	}

	// MWnd On
	virtual bool OnLButtonDown(unsigned int keys, unsigned short cx, unsigned short cy){
		mouse_down=1; mouse_x=cx; mouse_y=cy;
		CallGoOnMouse(GOMOUSE_LBUTTONDOWN, keys, cx, cy);
		return 0;
	}

	virtual bool OnLButtonUp(unsigned int keys, unsigned short cx, unsigned short cy){
		mouse_down=0;
		CallGoOnMouse(GOMOUSE_LBUTTONUP, keys, cx, cy);
		return 0;
	}

	virtual bool OnMouseMove(unsigned int keys, unsigned short cx, unsigned short cy){
		CallGoOnMouse(GOMOUSE_MOVE, keys,cx, cy);
		return 0;
	}

	virtual bool OnMouseWheel(unsigned int keys, unsigned short cx, unsigned short cy){
		CallGoOnMouse(GOMOUSE_WHEEL, keys, cx, cy);
		return 0;
	}

//	virtual bool OnMouseGo(GameObjectD *god, int opt, unsigned int keys, unsigned short cx, unsigned short cy){
//		return 0;
//	}

	virtual bool OnMouseClick(unsigned int keys, unsigned short cx, unsigned short cy){
		CallGoOnMouse(GOMOUSE_CLICK, keys,cx, cy);
		return 0;
	}

	// Call On
	GameObjectD *CallGoOnMouse(int &x, int &y){
		UGLOCK(this);
		GameObjectD *god=0, *sgod=0;
		while(god=Next(god)){
			if((god->opt&GO_GLOBAL && god->rect.InRect(x, y) || !(god->opt&GO_GLOBAL) && god->rect.InRect(x+scroll_x, y+scroll_y)) && (!sgod || god->z>sgod->z) && god->opt&GO_SHOW && god->opt&GO_MOUSE_MOVE) //  &&
				sgod=god;
		}

		return sgod;
	}

	void CallGoOnMouse(int act, int keys, int cx, int cy){
		GameObjectD *god=CallGoOnMouse(cx, cy);

		if(act==GOMOUSE_MOVE){
			mouse_mx=cx; mouse_my=cy;
			if(god){
				if(mouse_move!=god){
					if(mouse_move){
						mouse_move->mouse&=~GO_MOUSE_MOVE;
						GoOnMouse(GOMOUSE_UNMOVE, *mouse_move);
					}
					mouse_move=god;
					god->mouse|=GO_MOUSE_MOVE;
					GoOnMouse(GOMOUSE_MOVE, *mouse_move);
				}
 			//	OnMouseGo(sgod, GO_MOUSE_MOVE, keys, cx-sgod->rect.x, cy-sgod->rect.y);
			} else{
				if(mouse_move){
					mouse_move->mouse&=~GO_MOUSE_MOVE;
					GoOnMouse(GOMOUSE_UNMOVE, *mouse_move);
					mouse_move=0;
				}
			}
		}

		if(act==GOMOUSE_WHEEL){
			int z = GET_WHEEL_DELTA_WPARAM(keys);
			scroll_y=scroll_y-z;
			if(scroll_y>scroll_sy-rect.sy) scroll_y=scroll_sy-rect.sy;
			if(scroll_y<0) scroll_y=0;
		}

		if(god)
			GoOnMouse(act, *god);

		return ;
	}


	// Go On
	virtual void GoOnMouse(int act, GameObjectD &god){
		return ;
	}

	// On Draw
	virtual void OnDraw(){
		Lock();
		GameObjectD *god=0; int scry=-scroll_y;
		while(god=Next(god)){
			if(!(god->opt&GO_SHOW))
				continue;

			if(!(god->opt&GO_GLOBAL)){
				god->rect.Move(0, scry);	
				if(rect.RAnd(god->rect).Is())
					OnDraw(god);
				god->rect.Move(0, -scry);
			} else{
				if(!rect.RAnd(god->rect).Is())
					continue;
				OnDraw(god);
			}

			if(god->opt&GO_DEL)
				god=Del(god);

		}
		UnLock();

		return ;
	}

	virtual void OnDraw(GameObjectD *god){
		return ;
	}

	// Clear
	void Clear(){ MList<GameObjectD>::Clear(); }

};


class GameObjectExDraw{
public:
	virtual void OnDraw(GameBox *box) = 0;
};


class GameObjectExD{
public:
	 GameObjectExDraw *el;

};

class GameObjectEx{
	OList<GameObjectExD> list;

public:
	void Add(GameObjectExDraw *el){
		UGLOCK(list);
		list.New()->el=el;
	}

	void Add(GameObjectExDraw &el){
		UGLOCK(list);
		list.New()->el=&el;
	}

	void Del(GameObjectExDraw *el){
		UGLOCK(list);
		
		GameObjectExD *p=0;
		while(p=list.Next(p)){
			if(p->el==el){
				list.Del(p);
				break;
			}
		}

		return ;
	}

	void Draw(GameBox *box){
		UGLOCK(list);
		
		GameObjectExD *p=0;
		while(p=list.Next(p)){
			p->el->OnDraw(box);
		}

		return ;
	}




};