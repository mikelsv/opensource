// NEW IDEAL MATRIX -- updated to -> Ideal List

// Use Constructor & Destructor
#define ILIST_OPT_CON	1
#define ILIST_OPT_DES	2

template<class Item, int opt=ILIST_OPT_CON|ILIST_OPT_DES>
class IList{
	// size & basesize
	unsigned int allsize, basesize;

public:
	Item *data;
	unsigned int sz;

	IList(int bs=1024){ data=0; sz=0; allsize=0; basesize=bs; }
	~IList(){ Clear(); }

	bool Add(unsigned int add=S16K/sizeof(Item)){
		if(add<=0) return 0;
		data=(Item*)AddVData(data, allsize*sizeof(Item), add*sizeof(Item));
//		if(opt&ILIST_OPT_CON){
			for(unsigned int i=allsize; i<allsize+add; i++){ new(&data[i])Item; }
//		}
		allsize+=add;
		return 1;
	}

	void* AddVData(void *v, int sz, int nsz){
		void *nw=_newn(sz+nsz);
		if(sz) memcpy((char*)nw, (char*)v, sz);
		memset((char*)nw+sz, 0, nsz);
		_del(v); return nw;
	}

	inline bool A(){ if(sz==allsize) return Add(); return 0; }
	inline bool A(int sz){ if(size==allsize) return Add(sz); return 0; }

	inline Item& operator [](unsigned int i){ return data[i]; }
	
	inline unsigned int size(){ return sz; }
	inline unsigned int asize(){ return allsize; }
	
	inline Item& n(){ A(); return data[sz]; }
	void Added(){ sz++; return ; }

	void Clear(){ 
		if(opt&ILIST_OPT_DES){
			for(unsigned int i=0; i<sz; i++){ data[i].~Item(); }
		}
		_deln(data); sz=0; allsize=0;
	}

};