#ifndef THREADSTRING_STACK_SIZE
	#define THREADSTRING_STACK_SIZE (S128K - 32)
#endif

#ifndef THREADSTRING_BUFF_SIZE
	#define THREADSTRING_BUFF_SIZE (S128K)
#endif

class ThreadStringUListData{
public:
	ThreadStringUListData *next;
	unsigned int asz, usz, fsz;
	unsigned char data[0];
};

class ThreadStringUList{
	unsigned char buff[THREADSTRING_STACK_SIZE];
	ThreadStringUListData *data;
	unsigned int asz, usz, bsz;
	unsigned int stackused;
	unsigned int cltime;

public:

	ThreadStringUList(){
		asz = 0;
		usz = 0;
		bsz = 0;
		data = 0;		
		stackused = 0;
		cltime = time();
	}

	unsigned char* New(int sz){

#ifndef USEMSV_TSTRING_NOALARM
		if(cltime + 3600 < time()){
			cltime = time();
			globalerror("TString alarm! Memory leaks!");
		}
#endif

		if(data && data->fsz >= sz){
			unsigned char *ret = data->data + data->usz;
			data->usz += sz;
			usz += sz;
			data->fsz -= sz;

			return ret;
		}

		if(sz > THREADSTRING_BUFF_SIZE - sizeof(ThreadStringUListData)){
			ThreadStringUListData *d = (ThreadStringUListData*) malloc(sizeof(ThreadStringUListData) + sz);

			if(data){
				d->next = data->next;
				data->next = d;
			} else{
				data = d;
				d->next = 0;
			}
		
			d->asz = sz;
			d->usz = sz;
			d->fsz = 0;
			asz += sz;
			usz += sz;
			bsz ++;

			return d->data;
		}

		ThreadStringUListData *d;
		int bsize;

		if(!stackused){
			d = (ThreadStringUListData*) buff;
			bsize = THREADSTRING_STACK_SIZE;
			stackused = 1;
		} else{
			d = (ThreadStringUListData*) malloc(THREADSTRING_BUFF_SIZE);
			bsize = THREADSTRING_BUFF_SIZE;
		}

		d->asz = bsize - sizeof(ThreadStringUListData);
		d->usz = sz;
		d->fsz = d->asz - sz;

		asz += d->asz;
		usz += sz;
		bsz ++;

		if(!data || d->fsz > data->fsz){
			d->next = data;
			data = d;
		} else{
			d->next = data->next;
			data->next = d;
		}

		return d->data;
	}

	void UpTime(){
		cltime = time();
	}

	void Clear(){
		ThreadStringUListData *p = data, *d = p;
		
		while(p){
			p = p->next;
			if(d != (void*)buff)
				free(d);
			d = p;
		}

		data = 0;
		asz = 0;
		usz = 0;
		bsz = 0;
		stackused = 0;
		cltime = time();

		return ;
	}
};

class ThreadStringCore{
	//HLStringX<THREADSTRING_STACK_SIZE> ls;
	ThreadStringUList buff;

public:
	ThreadStringCore(){ };

	void Init();

	unsigned char* New(unsigned int sz){
		return buff.New(sz);
	}

	void Del(unsigned char* p){
		//return buff.Del(p);
	}

	void UpTime(){
		buff.UpTime();
	}

	void Clear(){
		buff.Clear();
	}

};

//#define DoTString() 
#ifdef WIN32
__declspec(thread) ThreadStringCore *do_tstring_core = 0;
__declspec(thread) int do_tstring_core_count = 0;
#else
__thread ThreadStringCore *do_tstring_core = 0;
__thread int do_tstring_core_count = 0;
#endif


void ThreadStringCore::Init(){
	if(!do_tstring_core){
		//do_tstring_core = new ThreadStringCore();
		msvnew(do_tstring_core, ThreadStringCore);
	}
	return ;
}


// functions
void ThreadStringInit(){
	if(!do_tstring_core)
		do_tstring_core->Init();

	if(!do_tstring_core_count)
		do_tstring_core->UpTime();
	
	do_tstring_core_count ++;

	return ;
}

void ThreadStringDest(){
	do_tstring_core_count --;

	if(!do_tstring_core_count)
		do_tstring_core->Clear();

#ifdef USEMSV_TSTRING_NOLEAKS
	//delete do_tstring_core;	
	msvdelete(do_tstring_core, ThreadStringCore);
	do_tstring_core = 0;
#endif

	 return ;
}

unsigned char * ThreadStringNew(int sz){
	return do_tstring_core->New(sz);
}

void ThreadStringDel(unsigned char *p){
	return do_tstring_core->Del(p);
}




//class TString : public VString{
//public:

	// --------------------------------------------------- Constructor  Конструктор
TString::TString(){ THREADSTRINGINIT; data = 0; sz = 0; }
TString::~TString(){ THREADSTRINGDEST; data = 0; sz = 0; }
void TString::Init(){ StringDel(data); data = 0; sz = 0; }

void TString::StringNew(const void *line){
	if(!sz){
		data = 0;
		return ;
	}

	data = ThreadStringNew(sz + 1);

	if(!data){
		sz = 0;
		return ;
	}

	if(line)
		memcpy(data, line, sz);

	data[sz] = 0;

	return ;
}

void TString::StringDel(unsigned char *line){
	ThreadStringDel(line);
	return ;
}

	// --------------------------------------------------- Create object  Создание обьекта

// int 
TString::TString(const int code){ THREADSTRINGINIT; sz = 1; StringNew(&code); return ; }
TString::TString(const unsigned int code){ THREADSTRINGINIT; sz = 1; StringNew(&code); return ; }
TString::TString(const unsigned int code, const unsigned int size){ THREADSTRINGINIT; sz = size; StringNew(0); memset(data, code, sz); return ; }

// char
TString::TString(const char string){ THREADSTRINGINIT; sz = sizeof(string); StringNew(&string); return ; }
TString::TString(const char *line){ THREADSTRINGINIT; if(line){ sz = strlen((const char*)line); StringNew(line); } else { data = 0; sz = 0; } }
TString::TString(const unsigned char *line){ THREADSTRINGINIT; if(line){ sz = strlen((const char*)line); StringNew(line); } else { data = 0; sz = 0; } }
TString::TString(const char *line, const unsigned int s){ THREADSTRINGINIT; sz = s; StringNew(line); }
TString::TString(const unsigned char *line, const unsigned int s){ THREADSTRINGINIT; sz = s; StringNew(line); }
TString::TString(const short string){ THREADSTRINGINIT; sz = sizeof(string); StringNew(&string); return ; }

// short
TString::TString(const short *string, int size){
	 THREADSTRINGINIT;
	if(!size){
		data = 0;
		return ;
	}

	MString ret = stoc((unsigned short*)string, size);
	sz = ret; StringNew(ret);

	return ;
}

// string
TString::TString(const VString &line){ THREADSTRINGINIT; sz = line.sz; StringNew(line); }
TString::TString(const TString &line){ THREADSTRINGINIT; sz = line.sz; StringNew(line); }
//TString::TString(const MString &line){ sz = line.sz; StringNew(line); }

	// --------------------------------------------------- Set	Установка

const TString& TString::operator=(const char string){ unsigned char* odata = data; sz = 1; StringNew(&string); StringDel(odata); return *this; }
const TString& TString::operator=(const char* line){ unsigned char *odata = data; sz = line ? strlen((const char*)line) : 0; StringNew(line); StringDel(odata); return *this; }
const TString& TString::operator=(const unsigned char* line){ unsigned char *odata=data; sz = line ? strlen((const char*)line) : 0; StringNew(line); StringDel(odata); return *this; }
const TString& TString::operator=(const VString& line){ unsigned char *odata = data; sz = line; StringNew(line); StringDel(odata); return *this; }
//const TString& TString::operator=(const MString& line){ unsigned char *odata=data; sz=line; MStringNew(line); _deln(odata); return *this; }

TString& TString::set(const VString line){ unsigned char *odata = data; sz = line.sz; StringNew(line); StringDel(odata); return *this; }
TString& TString::set(const char* line, const unsigned int size){ unsigned char *odata = data; sz = size;  StringNew(line); StringDel(odata); return *this; }
TString& TString::set(const unsigned char* line, const unsigned int size){ unsigned char *odata = data; sz = size;  StringNew(line); StringDel(odata); return *this; }
TString& TString::setu(const unsigned char* line, const unsigned int size){ unsigned char *odata = data; sz = size; StringNew(line); StringDel(odata); return *this; }

	// --------------------------------------------------- Add  Добавление	

const TString& TString::operator+=(const VString& string){
	unsigned char* odata = data;
	int szt = string.sz;
	int szo = sz;
	sz = szo + szt; 

	StringNew(0);
	memcpy(data, odata, szo);
	memcpy(data + szo, string.data, szt);

	if(sz)
		data[sz] = 0;

	if(odata)
		StringDel(odata);

	return *this;
}

TString& TString::Add(VString l1, VString l2, VString l3, VString l4, VString l5){
	MSTRING_TEMPALTE_ADD5;
}

TString& TString::Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9){
	MSTRING_TEMPALTE_ADD9;
}

TString& TString::Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9,
	VString l10, VString l11, VString l12, VString l13, VString l14, VString l15, VString l16, VString l17, VString l18, VString l19){
		MSTRING_TEMPALTE_ADD19;
}

TString& TString::AddR(VString &l1, VString &l2, VString &l3, VString &l4, VString &l5, VString &l6, VString &l7, VString &l8, VString &l9){
	MSTRING_TEMPALTE_ADDR9;
}

//---------------------------------------------------- Move   Перемещение

void TString::move(TString& string){
	StringDel(data);
	sz = string.sz; data = string.data;
	string.data = 0; string.sz = 0;
	return ;
}

TString& TString::operator-=(TString& string){
	StringDel(data);
	sz = string.sz; data = string.data;
	string.data = 0; string.sz = 0;
	return *this;
}

//---------------------------------------------------- Operations  Операции

TString& TString::Reserv(int size, int p){
	if(size == sz)
		return *this;
	
	if(size <= 0){
		StringDel(data);
		data = 0;
		sz = 0;
		return *this;
	}

	unsigned char *odata = data;
	int s = sz; sz = size;

	StringNew(0);
	int m = minel(s, size);
	memcpy(data, odata, m);
	memset(data + m, p, sz - m);
	data[size] = 0;

	StringDel(odata);
	return *this;
}

void TString::RClean(int sz, int p){
	Reserv(sz);
	CleanData(p);
}

void TString::CleanData(int p){
	memset(data, p, sz);
}

void TString::Clean(){ Init(); }



//};