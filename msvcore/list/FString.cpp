// FString -> FStringData[pos];
// FStringData[pos - 4] = FString*

#define FSTRING_BLOCK	S16K

class FString;

class FStringData{
public:
	FStringData *_p, *_n;
	unsigned int count; // blocks
	unsigned int pcount; // points to block
	unsigned int size; // all size
	unsigned int used; // used
	unsigned int free; // free
	unsigned char data[1];
};

class FStringVoid{
public:
	FString *v;
	unsigned char d[1];
};


class FStringCore : public TLock{
	OMatrixT<FStringData> data;

	void New(FString *val, VString from);
	void Del(FString *val);

	FStringData * Find(unsigned char *val){
		// please Lock

		FStringData *p = data._a;
		while(p){
			if(p->data <= val && p->data + p->size > val)
				return p;
			p = p->_n;
		}
		
		return 0;
	}

	FStringData * FindFree(unsigned int sz){
		FStringData *p = data._a;
		sz += sizeof(void*);

		while(p){
			if(p->free >= sz)
				return p;
			p = p->_n;
		}

		if(sizeof(FStringData) + sz > FSTRING_BLOCK)
			sz = sizeof(FStringData) + sz;
		else
			sz = FSTRING_BLOCK;

		p = (FStringData*) malloc(sz);
		memset(p, 0, sz);

		p->size = sz - sizeof(FStringData) + 1;
		p->used = 0;
		p->free = p->size;
		p->count = 0;

		data.OMAdd(p);

		return p;
	}

} FStringCore;



class FString : public VString{




};



void FStringCore::New(FString *val, VString from){
	if(!from.data || !from.sz){
		val->data = 0;
		val->sz = 0;
		return ;
	}
		
	UGLOCK(this);

	FStringData *p = FindFree(from.sz);
	FStringVoid *v = (FStringVoid*)(p->data + p->used);
		
	v->v = val;
	memcpy(v->d, from.data, from.sz);
	val->data = v->d;
	val->sz = from.sz;

	p->count ++;
	p->used += sizeof(void*) + from.sz;
	p->free -= sizeof(void*) + from.sz;

	return ;
}

void FStringCore::Del(FString *val){
	if(!val->data)
		return ;

	UGLOCK(this);

	FStringData *p = Find(val->data);
	FStringVoid *v = (FStringVoid*)(val->data - sizeof(void*));

	val->sz = 0;
	val->data = 0;

	v->v = 0;
	p->count --;
}