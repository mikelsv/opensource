#ifndef USEMSV_ALIST
	#error Use: #define USEMSV_ALIST
#endif

template<class TrieListEl>
class TrieListElProto/* : public OMatrixT<TrieListEl>*/{
public:
	TrieListEl *_a, *_e; // up, end
	TrieListEl *_p, *_n; // prev, next
	TrieListEl *_l, *_r; // left, right

	unsigned int k : 24, u : 8;
	//MString val;

	TrieListElProto(){
		_a = 0; _e = 0;
		_p = 0; _n = 0;
		_l = 0; _r = 0;

		k = 0;
		u = 0;
	}

	//void SetItem(TrieListEl &t){}

};

template<class TrieListEl>
class TrieList : private AListAllocHLString<TrieListEl>, public TrieListEl{
public:
	TrieList(){
		_a = 0;
		_e = 0;
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

	TrieListEl* Get(VString key, TrieListEl *parent = 0){
		TrieListEl* p = parent ? parent : (TrieListEl*)this;

		for(int i = 0; i < key.size(); i += 1){
			unsigned int k = 0;
			memcpy(&k, key.data + i, key.sz - i >= 3 ? 3 : key.sz - i); k = *(key.data+i);

			p = GetOne(k, p);
			if(!p)
				return 0;
		}

		if(p->u)
			return p;

		return 0;
	}

	// Set
	TrieListEl* SetOne(unsigned int k, TrieListEl *parent){
		if(!parent)
			return 0;

		TrieListEl *p = parent->_l, *el;

		if(!p || k < p->k){
			el = AllocNew();
			OMatrixT<TrieListEl>::OMAddExP(parent->_l, parent->_r, 0, el);
			el->k = k;
			el->u = 0;
			return el;
		}

		for(p; p; p = p->_n){
			if(p->k == k)
				return p;
			else if(k < p->k){
				p = p->_p;
				break;
			}
			else if(!p->_n)
				break;
		}

		el = AllocNew();
		OMatrixT<TrieListEl>::OMAddExP(parent->_l, parent->_r, p, el);
		el->k = k;
		el->u = 0;

		return el;
	}

	template<typename TN>
	TrieListEl* Set(VString key, TN &val, TrieListEl *parent = 0){
		TrieListEl* p = parent ? parent : (TrieListEl*)this;

		for(int i = 0; i < key.size(); i += 1){
			unsigned int k = 0;
			memcpy(&k, key.data + i, key.sz - i >= 3 ? 3 : key.sz - i); k = *(key.data+i);

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
		memcpy(&k, kp, ks >= 3 ? 3 : ks);

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

	// Clear
	void Clear(TrieListEl *parent = 0){
		if(!parent)
			return ;

		TrieListEl *el = parent->_a;
		for(el; el; el = el->_n){
			Clear(el);
		}

		el = parent->_l;
		for(el; el; el = el->_n){
			Clear(el);
		}

		return ;
	}

};


template<class TrieListEl>
class TrieListP{
	TrieList<TrieListEl> *tl;
	TrieListEl *el;

public:
	TrieListP(TrieList<TrieListEl> &t){
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

	void Clear(){
		if(!el)
			return ;
		return tl->Clear(el);
	}

};




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