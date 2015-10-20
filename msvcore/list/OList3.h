// Object List 3

// Options: Constructor, Destructor, List
#define OLIST_OPT_CON	1
#define OLIST_OPT_DES	2
//#define OLIST_OPT_LIST	4

// Fast malloc / free (structure);

class OList3DataEl{
public:
	unsigned char *data;
	unsigned int all, used;
};

class OList3Data{
	OList3DataEl *data;
	int all, used;

public:
	OList3Data(){
		data = 0;
		all = 0;
		used = 0;
	}

	OList3DataEl* New(int sz){
		unsigned char *nblock = (unsigned char*) malloc(sz);

		if(used >= all){
			OList3DataEl * odata = data;
			data = (OList3DataEl*) malloc(sizeof(OList3DataEl) * (all + 16));
			memcpy(data, odata, sizeof(OList3DataEl) * all);
			free(odata);
			all += 16;
		}

		if(!used || data[used-1].data < nblock){
			data[used].data = nblock;
			data[used].all = sz;
			data[used].used = 0;
			used++;

			return &data[used-1];
		}

		for(int i = 0; i < used; i++){
			if(nblock < data[i].data){
				memcpy(data + i, data + i + 1, used * sizeof(OList3DataEl));
				used ++;
				return &data[i];
			}
		}

		// fatal error
		return 0;
	}

	OList3DataEl* Get(unsigned char *d, int ds){
		OList3DataEl *a = data, *p; //, *e = data + used;
		int s = used;

		while(s > 1){
			p = a + (s / 2);
			if(d < p->data)
				s /= 2;
			else if(p->data + p->used * s < d){
				s /= 2;
				a = p;
			}else
				return p;
		}

		globalerror("FATAL ERROR! OList3DataEl.Get(){ return 0; }");
		return 0;
	}

	void Del(unsigned char *v){
		OList3DataEl * d = Get(v, 0);

		free(d->data);
		memcpy(d + 1, d, (data + used) - d);
		used --;
		return ;
	}
};

template <class OListItem, const int options=OLIST_OPT_CON|OLIST_OPT_DES>
class OList3 : public TLock{

	int maxsz, newsz;
	int usesz, allsz;

	OMatrixT<OmatrixBlock> data;
	OmatrixBlock *fre;

public:
	OList3(int max = S16M, int nsz = S16K / sizeof(OListItem)){
		//memset(this, 0, sizeof(OList));
		maxsz = max;
		newsz = nsz;

		usesz = 0;
		allsz = 0;
		fre = 0;
	}

	~OList3(){
		Clear();
	}

	OListItem* New(){
		UGLOCK(this);
		





	}
	
	void Del(OListItem *el){
		if(!el)
			return 0;


	}

	int Size(){
		return usesz;
	}

	void Clear(){
		UGLOCK(this);

		// please delete all elements or use OLitst(auto delete elements).

		// delete blocks
		OmatrixBlock*p=data._a, *d;
		while(p){
			d=p; p=p->_n;
			free(d);
		}

		//use.OMClearS();
		data.OMClearS();
		usesz=0; allsz=0;// datasz=0;
		fre=0;
	}

	//sizeof(PROCSTRUCTCN) < 




};

//#if sizeof(void*) > 9000
//#warning "Your procrastination is too large"
//#endif