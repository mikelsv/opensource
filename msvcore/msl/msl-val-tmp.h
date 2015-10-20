class msl_tvalue_base{
private:
	TString keyval;
	int keysize;

public:

	msl_tvalue_base(){
		THREADSTRINGINIT;
		keysize = 0;
	}

	~msl_tvalue_base(){
		THREADSTRINGDEST;
	}


	VString k(){
		return keyval.str(0, keysize);
	}

	VString v(){
		return keyval.str(keysize);
	}


protected:
	void BaseSetKeyVal(VString key, VString val){
		keyval.Add(key, val);
		keysize = key;
	}

	void BaseSetVal(VString val){
		BaseSetKeyVal(k(), val);
	}

	void BaseCleanKeyVal(){
		keyval.Clean();
		keysize = 0;
	}

	template<class B>
	B* BaseNew(){
		B *val = (B*) ThreadStringNew(sizeof(B));
		new(val)B;
		return val;
	}

	template<class B>
	void BaseDel(B *p){
		p->~B();
		ThreadStringDel((unsigned char*)p);
		return ;
	}

	template<class msl_value_base>
	friend class msl_value_template;
};


#define msl_tvalue msl_value_template<msl_tvalue_base>


/*

class msl_tvalue : public OMatrixT<msl_tvalue>{
public:
	msl_tvalue *_p, *_n;

private:
	TString keyval;
	int keysize;

public:
	msl_tvalue(){
		_p=0;
		_n=0;
		keysize = 0;
		THREADSTRINGINIT;
	}

	msl_tvalue(const VString text){
		THREADSTRINGINIT;
		SetKeyVal("", text);
	}

	msl_tvalue(msl_tvalue &val){
		THREADSTRINGINIT;
		Copy(val);
	}

	msl_tvalue* New(){
		msl_tvalue *p = ValueNew(); //new msl_tvalue;
		if(!p)
			return 0;

		OMAdd(p);
		return p;
	}

	msl_tvalue* New(msl_tvalue *prev){
		msl_tvalue *p = ValueNew();
		if(!p)
			return 0;

		OMAddEx(prev, p);
		return p;
	}

	msl_tvalue* Find(VString key){
		if(!this)
			return 0;
		
		for(msl_tvalue *p = _a; p; p = p->_n){
			if(p->key() == key)
				return p;
		}

		return 0;
	}

	msl_tvalue* Find(msl_tvalue *key){
		if(!this) return 0;
		
		for(msl_tvalue *p = _a; p; p = p->_n){
			if(p == key)
				return p;
		}

		return 0;
	}

	VString Get(VString key){
		msl_tvalue *p = Find(key);
		if(p)
			return p->val();
		else
			return VString();
	}

	msl_tvalue* GetV(VString key){
		msl_tvalue *p = Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_tvalue* GetV(msl_tvalue *key){
		msl_tvalue *p = Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_tvalue* Set(VString key, VString val){
		if(!this)
			return 0;

		msl_tvalue *p = Find(key);
		if(!p)
			p = New();

		if(p)
			p->SetKeyVal(key, val);

		return p;
	}

	msl_tvalue* Set(VString key, int64 val){
		Itos it;
		return Set(key, it.itos(val));
	}

	// Add
	void Add(VString key, msl_tvalue *val){
		msl_tvalue *p = New();
		p->SetKeyVal(key, "");
		p->Copy(val);
		//p->val=val; p->key=key;
		return ;
	}

	msl_tvalue* Add(VString key, VString val, msl_tvalue *prev){
		msl_tvalue *p = New(prev);
		p->SetKeyVal(key, val);
		return p;
	}

	msl_tvalue* Add(VString key, VString val){
		msl_tvalue *p = New();
		p->SetKeyVal(key, val);
		return p;
	}

	msl_tvalue* Add(VString key, int64 val){
		msl_tvalue *p = New();
		Itos it;
		p->SetKeyVal(key, it.itos(val));
		return p;
	}

	int Del(VString key){
		msl_tvalue *p = Find(key);
		if(p){
			OMDel(p);
			ValueDel(p);
			return 1;
		}
		return 0;
	}

protected:

	void operator=(msl_tvalue &val){
		Copy(val);
	}
	void operator=(msl_tvalue *val){
		if(val)
			Copy(*val);
	}

	void SetKeyVal(VString key, VString val){
		keyval.Add(key, val);
		keysize = key;
	}

	msl_tvalue* ValueNew(){
		msl_tvalue *val = (msl_tvalue*) ThreadStringNew(sizeof(msl_tvalue));
		new(val)msl_tvalue;
		return val;
	}

	void ValueDel(msl_tvalue *val){
		val->~msl_tvalue();
		ThreadStringDel((unsigned char*)val);
		return ;
	}

public:

	VString key(){
		return keyval.str(0, keysize);
	}

	VString val(){
		return keyval.str(keysize);
	}

	void Copy(msl_tvalue *val){
		if(val)
			Copy(*val);
	}

	void Move(msl_tvalue *val){
		if(val)
			Move(*val);
	}

	void Copy(msl_tvalue &val){
		Clear();
		this->SetKeyVal(key(), val.val());
		
		for(msl_tvalue *p = val._a; p; p = p->_n){
			Set(p->key(), p->val())->Copy(p);
		}

		return ;
	}

	void Move(msl_tvalue &val){
		Clear();
		_a = val._a;
		_e = val._e;
		val._a = 0;
		val._e = 0;
		//this->val -= val.val;		
		this->SetKeyVal(key(), val.val());
		return ;
	}

	msl_tvalue* n(){ return this ? _n : 0; }
	msl_tvalue* p(){ return this ? _p : 0; }
	msl_tvalue* a(){ return this ? _a : 0; }
	msl_tvalue* e(){ return this ? _e : 0; }

	void Clear(){
		msl_tvalue *p = _a, *d = p;

		while(p){
			d = p;
			p = p->_n;
			OMDel(d);
			//delete d;
			ValueDel(d);
		}

		 _a = 0;
		 _e = 0;

		 return ;
	}

	void ClearFull(){
		Clear();

		 keyval.Clean();
		 keysize = 0;
	}

	~msl_tvalue(){
		Clear();
		THREADSTRINGDEST;
	}

};



void GetLine(msl_tvalue &val, HLString &ls){
	int a=val.a()!=0 && val.a()->key(), f=1;
	if(a)
		ls+"{";
	else
		ls+"[";

	msl_tvalue *v=val._a;
	while(v){
		if(!f) ls+","; else f=0;
		if(v->_a){
			if(a)
				ls+"\""+v->key()+"\":";
			GetLine(*v, ls);
		} else{
			if(v->key())
				ls+"\""+v->key()+"\":";
			if(!v->val()) ls+"null";
			else ls+"\""+v->val()+"\"";
		}
		//ls+v->key+":"+v->val;
		v=v->_n;
	}

	if(a)
		ls+"}";
	else
		ls+"]";
	return ;
}

TString GetLine(msl_tvalue &val){
	if(!val._a) return TString();
	HLString ls;
	GetLine(val, ls);
	return ls;
}


void JsonToMsl(XDataEl *el, msl_tvalue &val, int clear=1){
	if(clear)
		val.Clear();

	Itos it;

	el=el->a(); int i=0;
	while(el){
		if(!el->a())
			val.Set(el->key ? el->key : it.itos(i++), el->val);
		else{
			JsonToMsl(el, *val.Set(el->key ? el->key : it.itos(i++), ""), clear);
		}
		el=el->n();
	}
	return ;
}
*/