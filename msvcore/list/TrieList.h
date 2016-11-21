#ifndef USEMSV_ALIST
	#error Use: #define USEMSV_ALIST
#endif

// up is _l, _r when k == 0 && u == 2

template<class TrieListEl>
class TrieListElProto/* : public OMatrixT<TrieListEl>*/{
public:
	//TrieListEl *_a, *_e; // up, end. deleted
	TrieListEl *_p, *_n; // prev, next
	TrieListEl *_l, *_r; // left, right

	unsigned int k : 24, u : 2;
	//MString val;

	TrieListElProto(){
	//	_a = 0; _e = 0;
		_p = 0; _n = 0;
		_l = 0; _r = 0;

		k = 0;
		u = 0;
	}

/* Add:
	MyData data;

public:
	void SetItem(AnyType &t){}
	...
	void SetItem(TrieListEl &t){}
*/

	//

};

template<class TrieListEl, unsigned int tkeysize = 3>
class TrieList : private AListAllocHLString<TrieListEl>, public TrieListEl{
	//unsigned int keysize;

public:
	TrieList(){
//		_a = 0;
//		_e = 0;

		//keysize = tkeysize;
	}

	// Get
	TrieListEl* GetFirstParent(){
		//return (TrieListEl*)AList::GetThis();
		return this;
	}

	TrieListEl* GetOne(unsigned int k, TrieListEl *parent){
		if(!parent)
			return 0;

		TrieListEl *p = parent->_l;

		for(p; p; p = p->_n){
			if(p->k == k)
				return p;
		}

		return 0;
	}

	TrieListEl* GetOneUp(TrieListEl *parent){
		if(!parent)
			return 0;

		TrieListEl *p = parent->_l;

		for(p; p; p = p->_n){
			if(p->k == ((1 << (tkeysize * 8)) - 1) && p->u == 2)
				return p;
		}

		return 0;
	}

	TrieListEl* Get(VString key, TrieListEl *parent = 0){
		TrieListEl* p;
		
		if(parent && parent != this)
			p = GetOneUp(parent);
		else
			p = (TrieListEl*)this;

		for(unsigned int i = 0; i < key.size(); i += tkeysize){
			unsigned int k = 0;
			memcpy(&k, key.data + i, key.sz - i >= tkeysize ? tkeysize : key.sz - i);// k = *(key.data+i);

			p = GetOne(k, p);
			if(!p)
				return 0;
		}

		if(p->u)
			return p;

		return 0;
	}

	// Set
	TrieListEl* SetOne(unsigned int k, TrieListEl *parent, int isup = 0){
		if(!parent)
			return 0;

		TrieListEl *p = parent->_l, *el;

		if(!p || k < p->k){
			el = AListAllocHLString<TrieListEl>::AllocNew();
			OMatrixT<TrieListEl>::OMAddExP(parent->_l, parent->_r, 0, el);
			el->k = k;
			el->u = isup;
			return el;
		}

		for(p; p; p = p->_n){
			if(p->k == k){
				if(isup && p->u != 2)
					continue;
				return p;
			}
			else if(k < p->k){
				p = p->_p;
				break;
			}
			else if(!p->_n)
				break;
		}

		el = AListAllocHLString<TrieListEl>::AllocNew();
		OMatrixT<TrieListEl>::OMAddExP(parent->_l, parent->_r, p, el);
		el->k = k;
		el->u = isup;

		return el;
	}

	template<typename TN>
	TrieListEl* Set(VString key, TN &val, TrieListEl *parent = 0){
		TrieListEl* p;
		
		if(parent && parent != this)
			p = SetOne(((1 << (tkeysize * 8)) - 1), parent, 2);
		else
			p = (TrieListEl*)this;

		for(unsigned int i = 0; i < key.size(); i += tkeysize){
			unsigned int k = 0;
			memcpy(&k, key.data + i, key.sz - i >= tkeysize ? tkeysize : key.sz - i);// k = *(key.data+i);

			p = SetOne(k, p);
			if(!p)
				return 0;
		}

		if(p){
			p->u = 1;
			//p->val = val;
			p->SetItem(val);
			return p;
		}

		return 0;
	}

	// Del
	TrieListEl* DelOne(unsigned char *kp, int ks, TrieListEl *parent){
		if(!parent)
			return 0;

		unsigned int k = 0;
		memcpy(&k, kp, ks >= tkeysize ? tkeysize : ks);

		TrieListEl *p = GetOne(k, parent), *d;
		if(!p)
			return 0;

		if(ks <= 3){
			if(p->u && !p->_l){
				OMatrixT<TrieListEl>::OMDelP(parent->_l, parent->_r, p);
				Clear(p);
				AllocFree(p);
				return parent;
			}
		} else{
			d = DelOne(k + 3 , ks - 3, p);
			if(p && !p->u && !p->_l){
				OMatrixT<TrieListEl>::OMDelP(parent->_l, parent->_r, p);
				Clear(p);
				AllocFree(p);
				return parent;
			}
		}

		return 0;
	}

	TrieListEl* Del(TrieListEl *el, VString key, TrieListEl *parent = 0){
		return DelOne(key, key, parent);
	}

	void ClearUp(TrieListEl *parent = 0){
		if(!parent)
			return ;

		TrieListEl *el;

		if(parent != this)
			el = GetOneUp(parent);
		else
			el = this;

		if(el)
			el = el->_l;
		
		for(el; el; el = el->_n){
			Clear(el);
		}

		return;
	}

	// Clear
	void Clear(TrieListEl *parent = 0){
		if(!parent)
			return ;



//		TrieListEl *el = parent->_a;
//		for(el; el; el = el->_n){
//			Clear(el);
//		}

		TrieListEl *el = parent->_l;
		for(el; el; el = el->_n){
			Clear(el);
		}

		return ;
	}

};


template<class TrieListEl, unsigned int tkeysize = 3>
class TrieListP{
	TrieList<TrieListEl, tkeysize> *tl;
	TrieListEl *el;

public:
	TrieListP(TrieList<TrieListEl, tkeysize> &t){
		tl = &t;
		el = t.GetFirstParent();
	}

	TrieListP& operator=(TrieListEl *p){
		el = p;
		return *this;
	}

	TrieListEl* operator()(VString key){
		return Get(key);
	}

	//template<typename TN>
	//TN operator[](VString key){
	//	TrieListEl *t = Get(key);
	//	return t->GetItem();
	//}

	operator bool(){
		return el != 0;
	}

	TrieListEl* Get(VString key){
		if(!el)
			return 0;

		return tl->Get(key, el);
	}

	template<typename TN>
	TrieListEl* Set(VString key, TN &val){
		if(!el)
			return 0;
		return tl->Set(key, val, el);
	}

	void ClearUp(){
		if(!el)
			return ;
		return tl->ClearUp(el);
	}

	void Clear(){
		if(!el)
			return ;
		return tl->Clear(el);
	}

};



/// TEST
class TrieTest : public TrieListElProto<TrieTest>{
	VString line;
	int i;

public:
	void SetItem(VString ln){
		line = ln;
	}

	void SetItem(int v){
		i = v;
	}
};


int TrieTestTest(){
	TrieList<TrieTest> l;
	TrieListP<TrieTest> p(l), p2(l);

	int i = 4;
	p.Set("one", i);

	p.Set("one", "111");
	p2 = p.Set("two", "222");
	p.Set("one apple", "777");

	p2.Set("qwe", "666");

	p2 = p.Get("one");
	p2 = p.Get("two");
	p2 = p2.Get("qwe");

	return 0;
}