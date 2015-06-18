#define USEMSV_MLIST_USED

template<class MListItem>
class MListData;

template<class MListItem>
class MListData{
public:
	MListData *_p, *_n;
	MListItem data;

	int _vs()const{ return (char*)&data-(char*)this; }
};


//template<class MListItem>
typedef int (*MLIST_SORTFUNC)(void *l, void *r);

template<class MListItem>
class MList : public TLock{
protected:
	MListData<MListItem> *_a, *_e;
	int sz;

public:
	MList(){ sz=0; _a=0; _e=0; }
	~MList(){ Clear(); }

	MListItem* New(MListItem *next=0){
		if(!TLock::lock) return 0;
		MListData<MListItem> *item=new MListData<MListItem>;
		OMAddEx(item, (MListData<MListItem>*) next); sz++;
		return &item->data;
	}

	//template<int strsize>
	MListItem* NewEx(int strsize, MListItem *next=0){
		if(!TLock::lock) return 0;
		MListData<MListItem> *item=(MListData<MListItem>*) malloc(sizeof(MListData<MListItem>)+strsize-sizeof(MListItem)); //new MListDataSz<MListItem, strsize-sizeof(MListItem)>;
		OMAddEx(item, (MListData<MListItem>*) next); sz++;
		return &item->data;
	}

	//template<class MListItemChild>
	//MListItem* NewEx(const MListItemChild *str, MListItem *next=0){
	//	if(!TLock::lock) return 0;
	//	MListData<MListItemChild> *item=new MListData<MListItemChild>;
	//	OMAddEx(item, (MListData<MListItem>*) next); sz++;
	//	return &item->data;
	//}

	void Clear(){
		UGLOCK(this);
		MListData<MListItem> *p=_a, *d=p;
		while(p){
			d=p; p=p->_n;
			OMDel(d); delete d;
		}
		 _a=0; _e=0; sz=0;
	}

	MListItem* Del(MListItem *item){
		UGLOCK(this);
		MListData<MListItem> *i=0; i=(MListData<MListItem>*) (((char*)item)-i->_vs());

		MListData<MListItem> *p=_a;
		while(p){
			if(p==i){
				if(p->_p)
					item=&(p)->_p->data;
				else
					item=0;

				OMDel(p); delete p; sz--;
				return item;
			}
			p=p->_n;
		}
		return item;
	}

	// Get
	MListItem* First(){
		if(!TLock::lock || !_a) return 0;
		return &_a->data;
	}

	MListItem* Next(MListItem*item){
		if(!TLock::lock) return 0;
		if(!item) return First();
		MListData<MListItem> *p=0; p=(MListData<MListItem>*) (((char*)item)-p->_vs());

		if(!p || !p->_n) return 0;
		return &(p)->_n->data;
	}

	MListItem* Prev(MListItem*item){
		if(!TLock::lock) return 0;
		if(!item) return 0;
		MListData<MListItem> *p=0; p=(MListData<MListItem>*) (((char*)item)-p->_vs());

		if(!p || !p->_p) return 0;
		return &(p)->_p->data;
	}

	MListItem* End(){
		if(!TLock::lock || !_e) return 0;
		return &_e->data;
	}

	// Find
	template<class MListItemAnyData>
	MListItem* Find(const void* FindFunc, MListItemAnyData &d){
		UGLOCK(this);

		typedef int (*MLIST_FINDFUNC)(MListItem &i, MListItemAnyData &d);
		
		MListData<MListItem> *p=_a;
		while(p){
			if((*(MLIST_FINDFUNC)FindFunc)(p->data, d)){ return &p->data; }
			p=p->_n;
		}
		return 0;
	}

	void Sort(const void* SortFunc){
		UGLOCK(this);
		MListData<MListItem> *p, *n;
		int s=1;

		typedef int (*MLIST_SORTFUNC)(MListItem &l, MListItem &r);

		while(s){
			p=_a; s=0;
			while(p && p->_n){
				if((*(MLIST_SORTFUNC)SortFunc)(p->data, p->_n->data)>0){
					n=p->_n;
					OMDel(n);
					OMAddEx(n, p);
					s=1;
					continue;
				}
				p=p->_n;
			}
		}
		return ;
	}

	int Size(){ return sz; }

protected:
	// Pointer operations
	// IF _p as _a && _n as _e;
	bool OMAdd(MListData<MListItem> *el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1; }
		_e->_n=el; el->_p=_e; el->_n=0; _e=el; return 1;
	}

	bool OMDel(MListData<MListItem> *el){
		if(el->_n) el->_n->_p=el->_p; else if(el!=_e){} else _e=el->_p; // if(el==_n)
		if(el->_p) el->_p->_n=el->_n; else if(el!=_a){} else _a=el->_n; // if(el==_p)
		return 1;
	}

	bool OMAddEx(MListData<MListItem> *el, MListData<MListItem> *next){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1; }

		if(!next){
			_e->_n=el; el->_p=_e; el->_n=0; _e=el;
			return 1;
		}

		el->_p=next->_p; el->_n=next; if(!next->_p) _a=el; if(el->_p) el->_p->_n=el; next->_p=el;

		//el->_p=p; el->_n=p->_n;
		//p->_n=el; if(el->_n) el->_n->_p=el; else _e=el;
		return 1;
	}
};