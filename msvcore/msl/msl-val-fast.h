#define MSV_FASTVAL_ASZ	128
#define MSV_FASTVAL_SZ	S8K

class msv_fast_val_core_b{
public:
	msv_fast_val_core_b *_n;
	unsigned char data[0];

};

struct msv_fast_val_point{
	msv_fast_val_point *p;
	unsigned int i;
};

class msv_fast_val_core_d{
public:
	msv_fast_val_core_b *block;
	unsigned char *data, *fdata;
	unsigned int fdatasz;
	unsigned int count;
};

class msv_fast_val_core : public TLock{
	//msv_fast_val_core_d *block[MSV_FASTVAL_ASZ];
	//unsigned char *data[MSV_FASTVAL_ASZ];
	//unsigned char *fdata[MSV_FASTVAL_ASZ];
	//unsigned int fdatasz[MSV_FASTVAL_ASZ];
	msv_fast_val_core_d data[MSV_FASTVAL_ASZ];

public:
	msv_fast_val_core(){
		memset(data, 0, sizeof(data));
	}

	void* New(int sz){
		if(!sz)
			return 0;

		return malloc(sz);

		UGLOCK(this);

		int s = (sz + 3) / 4;

#ifdef sizeof(void*) > 4
		s ++;
#endif

		if(s < MSV_FASTVAL_ASZ){
			return New(s, sz);
		} else
			return malloc(sz);
	}

	void Del(void *p, int sz){
		if(!sz)
			return ;

		return free(p);

		UGLOCK(this);
		int s = (sz + 3) / 4;

#ifdef sizeof(void*) > 4
		s ++;
#endif

		if(s < MSV_FASTVAL_ASZ){
			return Free(p, s);
		} else
			return free(p);

		return ;
	}

private:

	void* New(int s, int sz){
		data[s].count ++;

		while(1){
			if(data[s].fdatasz){
				void *p = data[s].fdata;
				data[s].fdata += s * 4;
				data[s].fdatasz --;
				return p;
			}

			if(data[s].data){
				msv_fast_val_point *p = (msv_fast_val_point*)data[s].data, *p2 = p;
				data[s].data = (unsigned char*)p->p;

				//if(s && p->i != 0x17892)
				//	int rewt = 456;

				//if(data[s] < (void*)0xfffff && data[s] > 0)
				//	int ert = 456;

				return p;
			}

			NewBlock(s);
		}

		//p2 = (_void_in*)data[s];
		//while(p2){

		//	if(p2 < (void*)0xfffff && p2 > 0 || p2 > (void*)0xcdcd0000)
		//	int ert = 456;

		//	p2 = (_void_in*)p2->p2;
		//}

		return 0;
	}

	void NewBlock(int s){
		msv_fast_val_core_b *b = (msv_fast_val_core_b*)malloc(MSV_FASTVAL_SZ);

		if(!data[s].block)
			data[s].block = b;
		else{
			b->_n = data[s].block;
			data[s].block = b;
		}

		int rs = (s + 1) * 4;
		int bs = (MSV_FASTVAL_SZ - sizeof(msv_fast_val_core_b)) / rs;
		
		data[s].fdata = b->data;
		data[s].fdatasz = bs;

		return ;

		//for(int i = 0; i < bs; i++){
		//	_void_in *v = (_void_in*)&b->data[i * rs];
		//	v->p = data[s];
		//	data[s] = (unsigned char*)v;
		//}

		return ;
	}

	void Free(void *p, int s){

		//return free(p);

		//if(p < (void*)0xfffff)
		//	int ert = 456;

		data[s].count --;

		msv_fast_val_point *v = (msv_fast_val_point*)p;
		v->p = (msv_fast_val_point*)data[s].data;
		data[s].data = (unsigned char*)v;

		//if(s && v->i == 0x17892)
		//	int rewt = 456;

		//if(s)
		//	v->i = 0x17892;

		//if(data[s] < (void*)0xfffff)
		//	int ert = 456;

		return ;
	}

} msl_fast_val_core;


class FMString{
	VString data;

public:

	FMString(){ MSVMEMORYCONTROLC }
	~FMString(){ Clean(); MSVMEMORYCONTROLD }

	VString Add(VString a, VString b){
		New(a.sz + b.sz);
		memcpy(data.data, a, a);
		memcpy(data.data + a.sz, b, b);

		return data;
	}

	operator VString(){
		return data;
	}

	void operator =(VString val){
		New(val, val);
	}

	void operator -=(FMString &val){
		Clean();

		data = val.data;
		val.data.Clean();
	}

	bool is(){
		return data;
	}

	bool operator!()const{ if (data.sz>0) return 0; return 1; }
	operator void*()const { return data.data; }
	operator char*()const{ return (char*)data.data; }
	operator unsigned char*()const { return data.data; }
	operator int()const{ if(data.data) return data.sz; return 0; }

	void Clean(){
		msl_fast_val_core.Del(data, data);
		data.Clean();
	}

private:
	void New(int sz){
		if(data)
			msl_fast_val_core.Del(data, data);
		
		data.data = (unsigned char*)msl_fast_val_core.New(sz);
		data.sz = sz;
	}

	void New(unsigned char *line, int sz){
		if(data)
			msl_fast_val_core.Del(data, data);
		
		data.data = (unsigned char*)msl_fast_val_core.New(sz);
		memcpy(data, line, sz);
		data.sz = sz;
	}

};


bool operator==(const FMString &string, const FMString &stringt){
	return compare(string, string, stringt, stringt);
 }
bool operator==(const FMString& string, const char* stringt){
	return compare(string, string, (char*)stringt, (unsigned int)strlen(stringt));
}
bool operator==(char* stringt, const FMString& string){
	return compare(string, string, stringt, (unsigned int)strlen(stringt));
}

bool operator==(const FMString &string, const VString &stringt){
	return compare(string, string, stringt, stringt);
}

bool operator==(const VString &string, const FMString &stringt){
	return compare(string, string, stringt, stringt);
}


class msl_fvalue : public OMatrixT<msl_fvalue>{
public:
	msl_fvalue *_p, *_n;
	FMString key, val;

	msl_fvalue(){
		_p=0; _n=0;
#ifdef USEMSV_MSL_TEST
		UGLOCK(msl_value_test_loc);
		msl_value_test_count++;
#endif
		MSVMEMORYCONTROLC
	}

	msl_fvalue(const VString text){
		val = text;
		MSVMEMORYCONTROLC
	}

	msl_fvalue(msl_fvalue &val){
		Copy(val);
	}

	msl_fvalue* New(){
		//msl_fvalue *p=new msl_fvalue;
		msl_fvalue *p= (msl_fvalue*)msl_fast_val_core.New(sizeof(msl_fvalue));
		new(p)msl_fvalue;

		if(!p) return 0;

		OMAdd(p);
		return p;
	}

	msl_fvalue* New(msl_fvalue *prev){
		//msl_fvalue *p=new msl_fvalue;
		msl_fvalue *p= (msl_fvalue*)msl_fast_val_core.New(sizeof(msl_fvalue));
		new(p)msl_fvalue;

		if(!p) return 0;

		OMAddEx(prev, p);
		return p;
	}

	msl_fvalue* Find(VString key){
		if(!this)
			return 0;
		
		for(msl_fvalue *p = _a; p; p = p->_n){
			if(p->key == key)
				return p;
		}

		return 0;
	}

	msl_fvalue* FindByKey(VString key, VString val){
		if(!this)
			return 0;
		
		for(msl_fvalue * p = _a; p; p = p->_n){
			if(p->Get(key) == val)
				return p;
		}

		return 0;
	}

	msl_fvalue* Find(msl_fvalue *key){
		if(!this)
			return 0;
		
		for(msl_fvalue *p = _a; p; p = p->_n){
			if(p == key)
				return p;
		}

		return 0;
	}

	int Size(){
		if(!this) return 0;
		int sz=0;
		
		for(msl_fvalue*p=_a; p; p=p->_n){
			sz++;
		}

		return sz;
	}

	VString Get(VString key){
		msl_fvalue *p=Find(key);
		if(p)
			return p->val;
		else
			return VString();
	}

	msl_fvalue* GetV(VString key){
		msl_fvalue *p=Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_fvalue* GetV(msl_fvalue *key){
		msl_fvalue *p=Find(key);
		if(p)
			return p;
		else
			return 0;
	}

	msl_fvalue* GetVByPos(int pos){
		msl_fvalue *p=_a;
		while(p){
			if(!pos--) return p;
			p=p->_n;
		}
		return 0;
	}

	//msl_fvalue* SGet(VString key){
	//	int isnew;
	//	return SGet(key, isnew);
	//}

	//msl_fvalue* SGet(VString key, int &isnew){
	//	msl_fvalue *p=Find(key);
	//	if(!p){
	//		p=New(); isnew=1;
	//		if(p){
	//			p->key=key; p->val.sz=0xffffffff;
	//		}
	//	} else isnew=0;
	//	return p;
	//}

	msl_fvalue* Set(VString key, VString val){
		if(!this)
			return 0;

		msl_fvalue *p=Find(key);
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

	msl_fvalue* Set(VString key, msl_fvalue *val){
		if(!this)
			return 0;

		msl_fvalue *p=Find(key);
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
	void Add(VString key, msl_fvalue *val){
		msl_fvalue *p=New();
		p->key=key;
		p->Copy(val);
		//p->val=val; p->key=key;
		return ;
	}

	void Add(VString key, VString val, msl_fvalue *prev){
		msl_fvalue *p=New(prev);
		p->key=key;
		p->val=val;		
		return ;
	}

	msl_fvalue * Add(VString key, VString val){
		msl_fvalue *p=New();
		p->val=val; p->key=key;
		return p;
	}

	int Del(VString key){
		msl_fvalue *p = Find(key);
		if(p){
			OMDel(p);

			p->~msl_fvalue();
			msl_fast_val_core.Del(p, sizeof(msl_fvalue));

			//delete p;
			return 1;
		}
		return 0;
	}

protected:

	void operator=(msl_fvalue &val){
		Copy(val);
	}
	void operator=(msl_fvalue *val){
		if(val)
			Copy(*val);
	}

public:

	void Copy(msl_fvalue *val){
		if(val) Copy(*val);
	}

	void Move(msl_fvalue *val){
		if(val) Move(*val);
	}

	void Copy(msl_fvalue &val){
		Clear();
		this->val=val.val;
		
		for(msl_fvalue*p=val._a; p; p=p->_n){
			Set(p->key, p->val)->Copy(p);
		}
		return ;
	}

	void Move(msl_fvalue &val){
		Clear();
		_a=val._a; _e=val._e; val._a=0; val._e=0;
		this->val-=val.val;		
		return ;
	}

	msl_fvalue* n(){ return this ? _n : 0; }
	msl_fvalue* p(){ return this ? _p : 0; }
	msl_fvalue* a(){ return this ? _a : 0; }

	void Clear(){
		msl_fvalue *p=_a, *d=p;
		while(p){
			d=p; p=p->_n;
			OMDel(d);
			//delete d;
			d->~msl_fvalue();
			msl_fast_val_core.Del(d, sizeof(msl_fvalue));
		}
		 _a=0; _e=0;
	}

	void ClearFull(){
		Clear();

		 key.Clean();
		 val.Clean();
	}

	~msl_fvalue(){
		Clear();
#ifdef USEMSV_MSL_TEST
		UGLOCK(msl_value_test_loc);
		msl_value_test_count--;
#endif
		MSVMEMORYCONTROLD
	}

};



void JsonToMsl(XDataEl *el, msl_fvalue &val, int clear=1){
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

void GetLine(msl_fvalue &val, HLString &ls);

MString GetLine(msl_fvalue &val){
	if(!val._a) return MString();
	HLString ls;
	GetLine(val, ls);
	return ls;
}

void GetLine(msl_fvalue &val, HLString &ls){
	int a=val.a()!=0 && val.a()->key.is(), f=1;
	if(a)
		ls+"{";
	else
		ls+"[";

	msl_fvalue *v=val._a;
	while(v){
		if(!f) ls+","; else f=0;
		if(v->_a){
			if(a)
				ls+"\""+ (VString)v->key+"\":";
			GetLine(*v, ls);
		} else{
			if(v->key)
				ls+"\""+(VString)v->key+"\":";
			if(!v->val) ls+"null";
			else ls+"\""+(VString)v->val+"\"";
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
