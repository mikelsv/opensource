// NEW OBJECT MATRIX -- updated to -> Object List

// Options: Constructor, Destructor, List
#define OLIST_OPT_CON	1
#define OLIST_OPT_DES	2
#define OLIST_OPT_LIST	4

template <class PROCSTRUCTCN, const int options = OLIST_OPT_CON|OLIST_OPT_DES>
class OList : public TLock{
	OMatrixT<OmatrixBlock> data; int datasz;
	OMatrixT<PROCSTRUCTCN> use;//, free;

	int maxsz, newsz; // info
	int usesz, allsz;
	unsigned int ltime;

	// fast
	OmatrixBlock *fre;

public:
	OList(int max=S1M, int nsz=S16K/sizeof(PROCSTRUCTCN)){
		memset(this, 0, sizeof(OList)); maxsz=max; newsz=nsz;
	}

	~OList(){ Clear(); }

	void Clear(){
		UGLOCK(this);
		// delete elements
		while(PROCSTRUCTCN *el = Next(0)){
			Del(el);
		}

		// delete blocks
		OmatrixBlock*p=data._a, *d;
		while(p){
			d=p; p=p->_n;
			free(d);
		}
		use.OMClearS();
		data.OMClearS();
		usesz=0; allsz=0; datasz=0;
		fre=0;
	}

	PROCSTRUCTCN*GetFirst(){
		return use._a;
	}

	int size(){ return usesz; }

	PROCSTRUCTCN*New(){
		UGLOCK(this);

		if(!fre || fre->u==fre->a){
			OmatrixBlock*nw=data._a, *d=0;
			unsigned int s=newsz+1; fre=0;
			for(nw; nw; nw=nw->_n){
				if(s>nw->a-nw->u && nw->u!=nw->a){ fre=nw; s=nw->a-nw->u; }
				if(!nw->u) d=nw;
			}

			if(d && d!=fre && ltime+30<time()){
				datasz--; allsz-=d->a;			
				data.OMDel(d); free(d);
				ltime=time();
			}

			if(!fre){
				if(allsz>=maxsz || newsz<1)
					return 0; 

				nw=(OmatrixBlock*)malloc(sizeof(OmatrixBlock)+sizeof(PROCSTRUCTCN)*newsz+((newsz+7)/8));
				if(!nw)
					return 0;

				memset(nw, 0, sizeof(OmatrixBlock)+sizeof(PROCSTRUCTCN)*newsz+((newsz+7)/8));

				nw->u=0; nw->a=newsz; nw->sz=sizeof(PROCSTRUCTCN)*newsz;
				data.OMAdd(nw); datasz++; allsz+=newsz;
				fre=nw; ltime=time();
			}
		}

		// free to lists
		if(!fre){ globalerror("OMATRIX2 !fre"); return 0; }
		int num=fre->getfree(); usesz++; if(num<0 || num>=newsz){ globalerror("OMATRIX2 num>maxsz"); return 0; }
		PROCSTRUCTCN *el= ((PROCSTRUCTCN*)(fre->data)) + num;
		fre->u++; fre->set(num, 1);
		new(el)PROCSTRUCTCN;
	
		//AddToList<options&OLIST_OPT_LIST>(el);
		//	 // list
		//}

		return el;
	}

	//template <int option> void AddToList(PROCSTRUCTCN *el);

	void AddToList(PROCSTRUCTCN *el){}
	//template<> void AddToList<0>(PROCSTRUCTCN *el){}
	//template<> void AddToList<OLIST_OPT_LIST>(PROCSTRUCTCN *el){ use.OMAdd(el); }


	//template<> void AddToList<0>(){}
	//template <int option> void AddToList() {
	//  AddToList<option> ();
	//}

	bool Is(PROCSTRUCTCN*el){
		if(!el)
			return 0; // out lists

		UGLOCK(this);

		OmatrixBlock *nw = data._a;
		int num;

		for(nw; nw; nw = nw->_n){
			if(nw->ismy(el))
				break;
		}
		
		if(!nw)
			return 0;

		num = nw->getn(el);

		return nw->get(num) == 1;
	}


	bool Del(PROCSTRUCTCN*el){
		if(!el) return 0; // out lists
		UGLOCK(this);

		OmatrixBlock*nw=data._a; int num;
		for(nw; nw; nw=nw->_n){
			if(nw->ismy(el)){ break; }
		} if(!nw) return 0;

		num=nw->getn(el); if(!nw->get(num)) globalerror("OMATRIX2 DEL FREE");
		nw->set(num, 0); nw->u--; usesz--;
		el->~PROCSTRUCTCN();

		if(nw->u>=nw->a) globalerror("OMATRIX2");

		if(options&OLIST_OPT_LIST){
		//	use.OMDel(el); // if list
		}
	return 1;
	}

	PROCSTRUCTCN* Next(PROCSTRUCTCN*el){
		UGLOCK(this);

		if(!el){
			if(!data._a) return 0;
			el=((PROCSTRUCTCN*)(data._a->data));
		}
		else
			el++;

		OmatrixBlock*nw=data._a; unsigned int num;
		for(nw; nw; nw=nw->_n){
			if(nw->ismye(el)){ break; }
		}
		if(!nw)
			return 0;

		num=nw->getn(el);
		for(num; num<nw->a; num++){
			if(nw->get(num)){
				el=((PROCSTRUCTCN*)(nw->data))+num;
				return el;
			}
		}

		if(nw->_n)
			return Next(((PROCSTRUCTCN*)(nw->_n->data)) - 1);

		return 0;
	}

};