// Base Class For Value

template<class msl_value_base>
class msl_value_template;

class msl_value_base{
public:
	MString key, val;

	VString k(){
		return key;
	}

	VString v(){
		return val;
	}

protected:
	void BaseSetKeyVal(VString key, VString val){
		this->key = key;
		this->val = val;
	}

	void BaseSetVal(VString val){
		this->val = val;
	}

	void BaseCleanKeyVal(){
		key.Clean();
		val.Clean();
	}

	template<class B>
	B* BaseNew(){
		B *p;
		msvnew(p, B);
		return p;
	}

	template<class B>
	void BaseDel(B *p){
		msvdelete(p, B);
		return ;
	}

	template<class msl_value_base>
	friend class msl_value_template;
};

//class A{
//
//};
//
//template <class O>
//class B{
//
//
//};
//
//template <class R>
//class C : public A, public B<C>{
//
//};


template<class t_msl_value>
class msl_value_template : public t_msl_value{

public:
	msl_value_template *_a, *_e, *_p, *_n;

	// Constructor
	msl_value_template(){
		MSVMEMORYCONTROLC;
		_a = 0; _e = 0; _p = 0; _n = 0;
	}

	msl_value_template(const VString text){
		MSVMEMORYCONTROLC;
		_a = 0; _e = 0; _p = 0; _n = 0;
		t_msl_value::BaseSetVal(text);		
	}

	msl_value_template(msl_value_template &val){
		MSVMEMORYCONTROLC;
		_a = 0; _e = 0; _p = 0; _n = 0;
		Copy(val);
	}

	// New
	msl_value_template* New(){
		//msl_value_template *p = t_msl_value::BaseNew<msl_value_template>();
		msl_value_template *p = t_msl_value::template BaseNew<msl_value_template>();
		if(!p)
			return 0;

		OMAdd(p);
		return p;
	}

	msl_value_template* New(msl_value_template *prev){
		msl_value_template *p = t_msl_value::template BaseNew<msl_value_template>();
		if(!p)
			return 0;

		OMAddEx(prev, p);
		return p;
	}

	// Find
		msl_value_template* Find(VString key){
		if(!this)
			return 0;
		
		for(msl_value_template *p = _a; p; p = p->_n){
			if(p->k() == key)
				return p;
		}

		return 0;
	}

	msl_value_template* Find(msl_value_template *key){
		if(!this)
			return 0;
		
		for(msl_value_template *p = _a; p; p = p->_n){
			if(p == key)
				return p;
		}

		return 0;
	}

	msl_value_template* FindByKey(VString key, VString val){
		if(!this)
			return 0;
		
		for(msl_value_template *p = _a; p; p = p->_n){
			if(p->Get(key) == val)
				return p;
		}

		return 0;
	}

	// Size
	int Size(){
		if(!this)
			return 0;

		int sz = 0;
		
		for(msl_value_template *p = _a; p; p = p->_n)
			sz++;

		return sz;
	}

	// Get
	VString Get(VString key){
		msl_value_template *p = Find(key);
		if(p)
			return p->v();
		else
			return VString();
	}

	msl_value_template* GetV(VString key){
		msl_value_template *p = Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_value_template* GetV(msl_value_template *key){
		msl_value_template *p = Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_value_template* GetVByPos(int pos){
		msl_value_template *p = _a;
		while(p){
			if(!pos--)
				return p;
			p = p->_n;
		}

		return 0;
	}

	// SetGet
	msl_value_template* SGet(VString key){
		int isnew;
		return SGet(key, isnew);
	}

	msl_value_template* SGet(VString key, int &isnew){
		msl_value_template *p = Find(key);
		if(!p){
			p = New();
			isnew = 1;
			if(p){
				p->key = key; p->val.sz = 0xffffffff;
			}
		} else
			isnew = 0;
		return p;
	}

	// Set
	msl_value_template* Set(VString key, VString val){
		if(!this)
			return 0;

		msl_value_template *p = Find(key);
		if(!p)
			p = New();

		if(p)
			p->BaseSetKeyVal(key, val);

		return p;
	}

	msl_value_template* Set(VString key, int64 val){
		Itos it;
		return Set(key, it.itos(val));
	}

	msl_value_template* Set(VString key, msl_value_template *val){
		if(!this)
			return 0;

		msl_value_template *p = Find(key);
		if(!p)
			p = New();

		if(p){
			p->key = key;
			p->Copy(val);
		}

		return p;
	}

	VString operator [](VString key){
		if(!this)
			return VString();

		return Get(key);
	}

	// Add
	void Add(VString key, msl_value_template *val){
		msl_value_template *p = New();
		p->key = key;
		p->Copy(val);
		//p->val=val; p->key=key;
		return ;
	}

	void Add(VString key, VString val, msl_value_template *prev){
		msl_value_template *p = New(prev);
		p->BaseSetKeyVal(key, val);	
		return ;
	}

	msl_value_template * Add(VString key, VString val){
		msl_value_template *p = New();
		p->BaseSetKeyVal(key, val);
		return p;
	}

	msl_value_template* Add(VString key, int64 val){
		msl_value_template *p = New();
		Itos it;
		p->BaseSetKeyVal(key, it.itos(val));
		return p;
	}

	// operator=
protected:
	void operator=(msl_value_template &val){
		Copy(val);
	}
	void operator=(msl_value_template *val){
		if(val)
			Copy(*val);
	}

public:

	// Copy
	void Copy(msl_value_template *val){
		if(val)
			Copy(*val);
	}

	void Copy(msl_value_template &val){
		Clear();
		this->BaseSetVal(val.v());
		
		for(msl_value_template *p = val._a; p; p = p->_n){
			Set(p->k(), p->v())->Copy(p);
		}
		return ;
	}

	// Move
	void Move(msl_value_template *val){
		if(val)
			Move(*val);
	}

	void Move(msl_value_template &val){
		Clear();
		_a = val._a;
		_e = val._e;
		val._a = 0;
		val._e = 0;
		this->val -= val.val;
		return ;
	}

	int Memory(int &count){
		int mem = sizeof(*this) + this->k() + this->v();
		count ++;

		msl_value_template *p = _a;
		while(p){
			//mem+=sizeof(msl_value)+key+val;
			//count++;
			mem += p->Memory(count);
			p = p->_n;
		}

		return mem;
	}

	msl_value_template* n(){ return this ? _n : 0; }
	msl_value_template* p(){ return this ? _p : 0; }
	msl_value_template* a(){ return this ? _a : 0; }
	msl_value_template* e(){ return this ? _e : 0; }

	// Del
	int Del(VString key){
		msl_value_template *p = Find(key);
		if(p){
			OMDel(p);
			t_msl_value::BaseDel(p);
			return 1;
		}
		return 0;
	}

	void Del(msl_value_template *p){
		if(!p)
			return ;

		// test in this line
		OMDel(p);
		t_msl_value::BaseDel(p);

		return ;
	}

	void Clear(){
		msl_value_template *p = _a, *d = p;

		while(p){
			d = p;
			p = p->_n;
			OMDel(d);
			t_msl_value::BaseDel(d);
		}

		_a=0;
		_e=0;

		return ;
	}

	void ClearFull(){
		Clear();

		msl_value_base::BaseCleanKeyVal();
	}

	~msl_value_template(){
		Clear();
		MSVMEMORYCONTROLD
	}

	// OMatrixT<msl_value_template>
	bool OMAdd(msl_value_template*el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1;}
		//_e->_n=el; el->_p=_e; el->_n=0; _e=el; return 1;
		el->_n=0; el->_p=_e; _e->_n=el; _e=el; return 1;
	}

	bool OMDel(msl_value_template*el){
		if(el->_n) el->_n->_p=el->_p; else if(el==_e) _e=el->_p;
		if(el->_p) el->_p->_n=el->_n; else if(el==_a) _a=el->_n;
		return 1;
	}

	bool OMAddEx(msl_value_template*p, msl_value_template*el){
		if(!_a){ _a=el; _e=el; el->_p=0; el->_n=0; return 1;}

		if(!p){
			el->_p=0; el->_n=_a; _a->_p=el; _a=el;
			//if(_a==_e) _e=el; its fail
			return 1;
		}

		el->_p=p; el->_n=p->_n;
		p->_n=el; if(el->_n) el->_n->_p=el; else _e=el;
		return 1;
	}


};


/*

// Values
class msl_value : public OMatrixT<msl_value>{
public:
	// prev, next, up first, up end
	msl_value *_p, *_n;//, *_a, *_e;

#ifdef USEMSV_MSL_FASTVAL
	FMString key, val;
#else
	MString key, val;
#endif

	msl_value(){
		_p=0; _n=0;
#ifdef USEMSV_MSL_TEST
		UGLOCK(msl_value_test_loc);
		msl_value_test_count++;
#endif
		MSVMEMORYCONTROLC
	}

	msl_value(const VString text){
		val = text;
		MSVMEMORYCONTROLC
	}

	msl_value(msl_value &val){
		Copy(val);
	}

	msl_value* New(){
		msl_value *p=new msl_value;
		if(!p) return 0;

		OMAdd(p);
		return p;
	}

	msl_value* New(msl_value *prev){
		msl_value *p=new msl_value;
		if(!p) return 0;

		OMAddEx(prev, p);
		return p;
	}

	msl_value* Find(VString key){
		if(!this) return 0;
		
		for(msl_value*p=_a; p; p=p->_n){
			if(p->key==key) return p;
		}

		return 0;
	}

	msl_value* FindByKey(VString key, VString val){
		if(!this) return 0;
		
		for(msl_value*p=_a; p; p=p->_n){
			if(p->Get(key)==val) return p;
		}

		return 0;
	}

	msl_value* Find(msl_value *key){
		if(!this) return 0;
		
		for(msl_value*p=_a; p; p=p->_n){
			if(p==key) return p;
		}

		return 0;
	}

	int Size(){
		if(!this) return 0;
		int sz=0;
		
		for(msl_value*p=_a; p; p=p->_n){
			sz++;
		}

		return sz;
	}

	VString Get(VString key){
		msl_value *p=Find(key);
		if(p)
			return p->val;
		else
			return VString();
	}

	msl_value* GetV(VString key){
		msl_value *p=Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_value* GetV(msl_value *key){
		msl_value *p=Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_value* GetVByPos(int pos){
		msl_value *p=_a;
		while(p){
			if(!pos--) return p;
			p=p->_n;
		}
		return 0;
	}

	msl_value* SGet(VString key){
		int isnew;
		return SGet(key, isnew);
	}

	msl_value* SGet(VString key, int &isnew){
		msl_value *p=Find(key);
		if(!p){
			p=New(); isnew=1;
			if(p){
				p->key=key; p->val.sz=0xffffffff;
			}
		} else isnew=0;
		return p;
	}

	msl_value* Set(VString key, VString val){
		if(!this)
			return 0;

		msl_value *p=Find(key);
		if(p){
			p->val=val;
		} else{
			p=New();
			if(p){
				p->key=key; p->val=val;
			}
		}
		return p;
	}

	msl_value* Set(VString key, msl_value *val){
		if(!this)
			return 0;

		msl_value *p=Find(key);
		if(p){
			p->Copy(val);
		} else{
			p=New();
			if(p){
				p->key=key; p->Copy(val);
			}
		}
		return p;
	}

	VString operator [](VString key){
		if(!this)
			return VString();

		return Get(key);
	}

	// Add
	void Add(VString key, msl_value *val){
		msl_value *p=New();
		p->key=key;
		p->Copy(val);
		//p->val=val; p->key=key;
		return ;
	}

	void Add(VString key, VString val, msl_value *prev){
		msl_value *p=New(prev);
		p->key=key;
		p->val=val;		
		return ;
	}

	msl_value * Add(VString key, VString val){
		msl_value *p=New();
		p->val=val; p->key=key;
		return p;
	}

	int Del(VString key){
		msl_value *p=Find(key);
		if(p){
			OMDel(p);
			delete p;
			return 1;
		}
		return 0;
	}

	void Del(msl_value *p){
		if(!p) return ;

		// test in this line
		OMDel(p); delete p;

		return ;
	}

protected:

	void operator=(msl_value &val){
		Copy(val);
	}
	void operator=(msl_value *val){
		if(val)
			Copy(*val);
	}

public:

	void Copy(msl_value *val){
		if(val) Copy(*val);
	}

	void Move(msl_value *val){
		if(val) Move(*val);
	}

	void Copy(msl_value &val){
		Clear();
		this->val=val.val;
		
		for(msl_value*p=val._a; p; p=p->_n){
			Set(p->key, p->val)->Copy(p);
		}
		return ;
	}

	void Move(msl_value &val){
		Clear();
		_a=val._a; _e=val._e; val._a=0; val._e=0;
		this->val-=val.val;		
		return ;
	}

	int Memory(int &count){
		int mem=sizeof(msl_value)+key.sz+val.sz; count++;

		msl_value *p=_a;
		while(p){
			//mem+=sizeof(msl_value)+key+val;
			//count++;
			mem+=p->Memory(count);
			p=p->_n;
		}
		return mem;
	}

	msl_value* n(){ return this ? _n : 0; }
	msl_value* p(){ return this ? _p : 0; }
	msl_value* a(){ return this ? _a : 0; }

	void Clear(){
		msl_value *p=_a, *d=p;
		while(p){
			d=p; p=p->_n; OMDel(d); delete d;
		}
		 _a=0; _e=0;
	}

	void ClearFull(){
		Clear();

		 key.Clean();
		 val.Clean();
	}

	~msl_value(){
		Clear();
#ifdef USEMSV_MSL_TEST
		UGLOCK(msl_value_test_loc);
		msl_value_test_count--;
#endif
		MSVMEMORYCONTROLD
	}
};
*/
