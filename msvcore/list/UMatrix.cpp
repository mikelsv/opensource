// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:34cad9373042c92256c5a7b1042fb289 : 2012.11.11 21:03:29


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class LString; // your class Index

// Defines 
//#define DLString_sz sizeof(DData) // General data size



#ifndef LString_minmem
#define LString_minmem	1024
#endif
#define LString_minel	16

//class LString_um{ char*n; unsigned int sz; char data[0]; };

class LString{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
UMX_data *data, *edata, *cdata; unsigned int cblockfn, esz;
const char nullel;
bool isdel;
public:
LString(unsigned int buf=LString_minmem) : nullel(){ Init(); bufsize=buf; }
~LString(){Clear();}

void operator=(const char *s){ Clear(); add(s, (unsigned int)strlen(s)); }
void operator=(const MString& s){ Clear(); add(s, s); }
void operator=(const VString& s){ Clear(); add(s, s); }
//void operator+=(const MString &s){ add(s, s); }
void operator+=(const char *s){ add(s, strlen(s)); }
void operator+=(const char s){ add((char*)&s, 1); }
void operator+=(const VString &s){ add(s, s); }
void resize(unsigned int s){ usedsize=s; }


void String(MString &ret){
	ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz;
	UMX_data *dt=data;

	while(dt){
		if(dt->sz+pos<ret.size()) sz=dt->sz; else { sz=ret.size()-pos; }
		memcpy(ret.pos(pos), dt->data, sz*sizeof(char)); pos+=sz;
		dt=dt->n;
	}	
}


const LString& operator+=(LString& lst){
unsigned int dsz, asz=lst.usedsize; UMX_data*dt=lst.data;

if(lst.data) while(dt){
	dsz=dt->sz; if(dsz<asz){ asz-=dsz; }else{ dsz=asz; }
	add((char*)&dt->data, dsz);
	dt=dt->n;
	}
	return *this;
}

int Send(SOCKET sock){
int ret=1, sz, rets=0; cdata=data; cblockfn=0;
	while(cdata){
		if(usedsize-cblockfn>cdata->sz) sz=cdata->sz; else sz=usedsize-cblockfn;
		ret=send(sock, (char*)(&cdata->data), sz, 0); if(ret>0) rets+=ret; else return 0;
		cblockfn+=cdata->sz; cdata=cdata->n; 
	}
return rets;
}

inline const unsigned int addnf(const char *s, const unsigned int sz=1){ if(allsize-usedsize<sz) usedsize=allsize; return add(s, sz); } // no frag
inline char* addnfv(const unsigned int sz){ return &el(addnf(0, sz)); } // no frag, virtual

// NOTE!! Modify new(e)char  => *e=0;


void Init(){ memset(this, 0, sizeof(LString)); }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(char)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(char); }
unsigned int size()const {return usedsize;}

//const LString& operator+=(char *s){ return add(s, strlen(s)/sizeof(char));}
//const LString& operator+=(MString &s){ return add(s, s);}

const char& operator[](const unsigned int e){ return getelement(e); }
const char& operator[](const int e){ return getelement(e); }

char& el(const unsigned int e){ return getelement(e); }

char& getelement(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz) return *((char*)(&cdata->data)+(e-cblockfn));
		cblockfn+=cdata->sz; cdata=cdata->n;
	}
return (char&)nullel;
}

void *GetBuffer(int sz){
	void *d = malloc(sz);
	memset(d, 0, sz);
	return d;
}

void DelBuffer(void *p){
	free(p);
}

inline const unsigned int add(const char &s){ return add(&s, sizeof(char)); }
//inline char& inserta(){return (char&)*insert();}

inline const unsigned int add(const char *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=LString_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s){ memcpy((char*)&edata->data+esz-frs, s, ws*sizeof(char)); }		
		usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(char); if(ws<1) ws=1; if(sz>ws) ws=sz;
	UMX_data *ndata=(UMX_data*)GetBuffer(ws*sizeof(char)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(char)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(char)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	ndata->sz=ws; esz=ws; //*(unsigned int*)((char*)ndata)=ws; esz=ws;// ndata+=UMSZ;
	if(edata){ edata->n=ndata; }
	edata=ndata; if(!data) data=edata;
	if(s) memcpy(edata->data, s+frs, (sz-frs)*sizeof(char));
return ret;
}

inline char& inserta(){ return (char&)getelement(insert()); }
inline char* vinsert(){ return (char*)&getelement(insert()); }
inline char& repaira(){ return (char&)getelement(repair()); }
inline char* vrepair(){ return (char*)&getelement(repair()); }

inline unsigned int insert(){ char tm[sizeof(char)]; memset(&tm, 0, sizeof(char)); unsigned int ed=add((char*)&tm, 1); char*e=&getelement(ed); memset(e, 0, sizeof(*e)); return ed; }

bool del(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
				#ifdef DLString_constructor
			((char*)cdata.data+(e-cblockfn))->~char();
				#endif
			 if(isdel) ((mbool*)(cdata+cdata->sz))->set(e-cblockfn, 0);
			 return 1;
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int repair(){ if(!isdel) return insert();
int s; mbool *mb; if(!cdata || 0<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		//s=*(unsigned int*)((char*)cdata-UMSZ);
		int i=0; mb=(mbool*)(cdata->data+s);

		for(i; i<s; i++){ if(i%8==0 && mb->isbyte(i/8)==255){ i+=8; continue; }
		if(!mb->get(i)){// if(cblockfn+i<usedsize) return insert();
		 mb->set(i, 1);
#ifdef DLString_constructor
		new((char*)cdata->data+i)char;
#endif
		return cblockfn+i;
	}}
		cdata=cdata->n; cblockfn+=s;
	}
 return insert();
}

bool is(unsigned int e){ if(!isdel) return 1;
	if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
			 return ((mbool*)((char*)cdata->data+cdata->sz))->get(e-cblockfn);
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int getpos(const char *pos){
	cdata=data; cblockfn=0;
	while(cdata){
		if((char*)&cdata->data<=pos && pos<(char*)&cdata->data+cdata->sz){
			 return cblockfn+(pos-(char*)(&cdata->data));
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return -1;
}

// SP
unsigned int GetFreeBuffer(char**data){
	if(allsize==usedsize){ add((char*)&nullel, 1); usedsize--; }
	unsigned int fs=allsize-usedsize;
	*data=(char*)&edata->data+esz-fs;
	return fs;
}

void SetFreeBuffer(unsigned int add){ usedsize+=esz-add; }

//const LString& operator=(char *s){Clear(); return add(s, strlen(s));}
//const LString& operator=(MString s){Clear(); return add(s, s);}

operator MString(){
	MString ret; ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz;
	UMX_data*dt=data;

	if(data) while(dt){
		if(dt->sz+pos<ret.size()) sz=dt->sz;
		else{ sz=ret.size()-pos; }
		memcpy(ret.pos(pos), &dt->data, sz*sizeof(char)); pos+=sz;
		dt=dt->n;	
	}
	return ret;
}

char* pos(){ if(!data) return 0; return (char*)data->data; }
operator char*(){ if(!data) return 0; return (char*)data->data; }
char* nomov(){ return oneline(); }

char* oneline(bool delfree=1){
	if(!data) return 0; if(data==edata) return (char*)data->data;
	if(delfree) esz=usedsize; else esz=allsize;
	UMX_data *ndata=(UMX_data*)GetBuffer(esz*sizeof(char)+UMSZ);
	//*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	ndata->sz=esz;
	unsigned int pos=0, sz;

	UMX_data *dt=data, *dd;
	while(dt){
		if(dt->sz+pos<usedsize) sz=dt->sz; else{ sz=usedsize-pos; }
		memcpy(ndata->data+pos*sizeof(char), dt->data, sz*sizeof(char)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
//*(long*)(char*)&dt=*(long*)((char*)dt-UMCSZ);
 		dd=dt; dt=dt->n; DelBuffer(dd);
	}
	data=ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
	return (char*)data->data;
}

void Clear(){ if(!data) return ;
	UMX_data *dt=data, *dd;
	unsigned int pos=0, dsz, msz;

	while(dt){
		dsz=dt->sz; msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
#ifdef DLString_destructor
		for(unsigned int i=0; i<msz; i++){
			((char*)dt->data+i)->~char();
		}
#endif
		dd=dt; dt=dt->n;		
		DelBuffer(dd);
	}
	data=0; edata=0; cdata=0; allsize=0; usedsize=0; 
}

// Add Func
	void i_want_mod_usedsize(int pos){ usedsize=pos; }

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ExplodeLine; // your class Index

// Defines 
//#define DExplodeLine_sz sizeof(DData) // General data size



#ifndef ExplodeLine_minmem
#define ExplodeLine_minmem	1024
#endif
#define ExplodeLine_minel	16

//class ExplodeLine_um{ DUVINTS*n; unsigned int sz; DUVINTS data[0]; };

class ExplodeLine{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
UMX_data *data, *edata, *cdata; unsigned int cblockfn, esz;
const DUVINTS nullel;
bool isdel;
public:
ExplodeLine(unsigned int buf=ExplodeLine_minmem) : nullel(){ Init(); bufsize=buf; }
~ExplodeLine(){Clear();}

MString sdata; const MString nullelm;
const VString& el(unsigned int i){ if(i<usedsize) return (VString&)(this->operator [](i)); else return nullelm; }
VString& ele(unsigned int i){ if(i<usedsize) return (VString&)(this->operator [](i)); else return (VString&)nullelm; }
const DUVINTS& elr(unsigned int i){ if(i<usedsize) return this->operator [](i); else return nullel; }

bool SetData(const MString &line, bool cor){ return SetData(line.uchar(), line, cor); }
bool SetData(const unsigned char* line, const unsigned int size, bool cor){
		if(line) sdata.set(line, size);
		if(!cor) return 1;

		UMX_data *cdata=data;
		while(cdata){
			for(unsigned int i=0; i<cdata->sz; i++){
			((DUVINTS*)cdata->data+i)->pos=(char*)sdata.pos(((DUVINTS*)cdata->data+i)->pos-(char*)0);
			//cdata[i].pos=(unsigned char*)sdata.pos(cdata[i].pos-(unsigned char*)0);
			}
			cblockfn+=cdata->sz; cdata=cdata->n;
		}
return 1;
}

// NOTE!! Delete DUVINTS& el() // Delete new(e)DUVINTS; ELSE more errors!!!


void Init(){ memset(this, 0, sizeof(ExplodeLine)); }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(DUVINTS)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(DUVINTS); }
unsigned int size()const {return usedsize;}

//const ExplodeLine& operator+=(DUVINTS *s){ return add(s, strlen(s)/sizeof(DUVINTS));}
//const ExplodeLine& operator+=(MString &s){ return add(s, s);}

const DUVINTS& operator[](const unsigned int e){ return getelement(e); }
const DUVINTS& operator[](const int e){ return getelement(e); }

//DUVINTS& el(const unsigned int e){ return getelement(e); }

DUVINTS& getelement(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz) return *((DUVINTS*)(&cdata->data)+(e-cblockfn));
		cblockfn+=cdata->sz; cdata=cdata->n;
	}
return (DUVINTS&)nullel;
}

inline const unsigned int add(const DUVINTS &s){ return add(&s, sizeof(DUVINTS)); }
//inline DUVINTS& inserta(){return (DUVINTS&)*insert();}

inline const unsigned int add(const DUVINTS *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=ExplodeLine_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s){ memcpy((DUVINTS*)&edata->data+esz-frs, s, ws*sizeof(DUVINTS)); }		
		usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(DUVINTS); if(ws<1) ws=1; if(sz>ws) ws=sz;
	UMX_data *ndata=(UMX_data*)GetBuffer(ws*sizeof(DUVINTS)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(DUVINTS)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(DUVINTS)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	ndata->sz=ws; esz=ws; //*(unsigned int*)((char*)ndata)=ws; esz=ws;// ndata+=UMSZ;
	if(edata){ edata->n=ndata; }
	edata=ndata; if(!data) data=edata;
	if(s) memcpy(edata->data, s+frs, (sz-frs)*sizeof(DUVINTS));
return ret;
}

inline DUVINTS& inserta(){ return (DUVINTS&)getelement(insert()); }
inline DUVINTS* vinsert(){ return (DUVINTS*)&getelement(insert()); }
inline DUVINTS& repaira(){ return (DUVINTS&)getelement(repair()); }
inline DUVINTS* vrepair(){ return (DUVINTS*)&getelement(repair()); }

inline unsigned int insert(){ char tm[sizeof(DUVINTS)]; memset(&tm, 0, sizeof(DUVINTS)); unsigned int ed=add((DUVINTS*)&tm, 1); DUVINTS*e=&getelement(ed); memset(e, 0, sizeof(*e)); return ed; }

bool del(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
				#ifdef DExplodeLine_constructor
			((DUVINTS*)cdata.data+(e-cblockfn))->~DUVINTS();
				#endif
			 if(isdel) ((mbool*)(cdata+cdata->sz))->set(e-cblockfn, 0);
			 return 1;
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int repair(){ if(!isdel) return insert();
int s; mbool *mb; if(!cdata || 0<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		//s=*(unsigned int*)((char*)cdata-UMSZ);
		int i=0; mb=(mbool*)(cdata->data+s);

		for(i; i<s; i++){ if(i%8==0 && mb->isbyte(i/8)==255){ i+=8; continue; }
		if(!mb->get(i)){// if(cblockfn+i<usedsize) return insert();
		 mb->set(i, 1);
#ifdef DExplodeLine_constructor
		new((DUVINTS*)cdata->data+i)DUVINTS;
#endif
		return cblockfn+i;
	}}
		cdata=cdata->n; cblockfn+=s;
	}
 return insert();
}

bool is(unsigned int e){ if(!isdel) return 1;
	if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
			 return ((mbool*)((DUVINTS*)cdata->data+cdata->sz))->get(e-cblockfn);
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int getpos(const DUVINTS *pos){
	cdata=data; cblockfn=0;
	while(cdata){
		if((DUVINTS*)&cdata->data<=pos && pos<(DUVINTS*)&cdata->data+cdata->sz){
			 return cblockfn+(pos-(DUVINTS*)(&cdata->data));
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return -1;
}

void *GetBuffer(int sz){
	void *d = malloc(sz);
	memset(d, 0, sz);
	return d;
}

void DelBuffer(void *p){
	free(p);
}

// SP
unsigned int GetFreeBuffer(DUVINTS**data){
	if(allsize==usedsize){ add((DUVINTS*)&nullel, 1); usedsize--; }
	unsigned int fs=allsize-usedsize;
	*data=(DUVINTS*)&edata->data+esz-fs;
	return fs;
}

void SetFreeBuffer(unsigned int add){ usedsize+=esz-add; }

//const LString& operator=(char *s){Clear(); return add(s, strlen(s));}
//const LString& operator=(MString s){Clear(); return add(s, s);}

operator MString(){
	MString ret; ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz;
	UMX_data*dt=data;

	if(data) while(dt){
		if(dt->sz+pos<ret.size()) sz=dt->sz;
		else{ sz=ret.size()-pos; }
		memcpy(ret.pos(pos), &dt->data, sz*sizeof(DUVINTS)); pos+=sz;
		dt=dt->n;	
	}
	return ret;
}

DUVINTS* pos(){ if(!data) return 0; return (DUVINTS*)data->data; }
operator DUVINTS*(){ if(!data) return 0; return (DUVINTS*)data->data; }
DUVINTS* nomov(){ return oneline(); }

DUVINTS* oneline(bool delfree=1){
	if(!data) return 0; if(data==edata) return (DUVINTS*)data->data;
	if(delfree) esz=usedsize; else esz=allsize;
	UMX_data *ndata=(UMX_data*)GetBuffer(esz*sizeof(DUVINTS)+UMSZ);
	//*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	ndata->sz=esz;
	unsigned int pos=0, sz;

	UMX_data *dt=data, *dd;
	while(dt){
		if(dt->sz+pos<usedsize) sz=dt->sz; else{ sz=usedsize-pos; }
		memcpy(ndata->data+pos*sizeof(DUVINTS), dt->data, sz*sizeof(DUVINTS)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
//*(long*)(char*)&dt=*(long*)((char*)dt-UMCSZ);
 		dd=dt; dt=dt->n; DelBuffer(dd);
	}
	data=ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
	return (DUVINTS*)data->data;
}

void Clear(){ if(!data) return ;
	UMX_data *dt=data, *dd;
	unsigned int pos=0, dsz, msz;

	while(dt){
		dsz=dt->sz; msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
#ifdef DExplodeLine_destructor
		for(unsigned int i=0; i<msz; i++){
			((DUVINTS*)dt->data+i)->~DUVINTS();
		}
#endif
		dd=dt; dt=dt->n;		
		DelBuffer(dd);
	}
	data=0; edata=0; cdata=0; allsize=0; usedsize=0; 
}

// Add Func
	void i_want_mod_usedsize(int pos){ usedsize=pos; }

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class TULine; // your class Index

// Defines 
//#define DTULine_sz sizeof(DData) // General data size


 #define DTULine_destructor 1 
#ifndef TULine_minmem
#define TULine_minmem	1024
#endif
#define TULine_minel	16

//class TULine_um{ DUMS*n; unsigned int sz; DUMS data[0]; };

class TULine{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
UMX_data *data, *edata, *cdata; unsigned int cblockfn, esz;
const DUMS nullel;
bool isdel;
public:
TULine(unsigned int buf=TULine_minmem) : nullel(){ Init(); bufsize=buf; }
~TULine(){Clear();}

void insert(const VString& key, const VString& val){ DUMS &dt=inserta(); dt.key=key; dt.val=val; return ; }
VString Find(const VString& key){if(data!=edata) oneline();
DUMS *d=&el(0); unsigned int s=size();
for(unsigned int i=0; i<s; i++){ if(d[i].key==key) return d[i].val;} return "";
}
VString FindUp(const VString& key){if(data!=edata) oneline();
DUMS *d=&el(0); unsigned int s=size();
for(unsigned int i=0; i<s; i++){ if(compareu(d[i].key, key)) return d[i].val;} return "";
}
int FindI(const VString& key){if(data!=edata) oneline();
DUMS *d=&el(0); unsigned int s=size();
for(unsigned int i=0; i<s; i++){ if(d[i].key==key) return i;} return -1;
}

void Init(){ memset(this, 0, sizeof(TULine)); }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(DUMS)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(DUMS); }
unsigned int size()const {return usedsize;}

//const TULine& operator+=(DUMS *s){ return add(s, strlen(s)/sizeof(DUMS));}
//const TULine& operator+=(MString &s){ return add(s, s);}

const DUMS& operator[](const unsigned int e){ return getelement(e); }
const DUMS& operator[](const int e){ return getelement(e); }

DUMS& el(const unsigned int e){ return getelement(e); }

DUMS& getelement(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz) return *((DUMS*)(&cdata->data)+(e-cblockfn));
		cblockfn+=cdata->sz; cdata=cdata->n;
	}
return (DUMS&)nullel;
}

inline const unsigned int add(const DUMS &s){ return add(&s, sizeof(DUMS)); }
//inline DUMS& inserta(){return (DUMS&)*insert();}

inline const unsigned int add(const DUMS *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=TULine_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s){ memcpy((DUMS*)&edata->data+esz-frs, s, ws*sizeof(DUMS)); }		
		usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(DUMS); if(ws<1) ws=1; if(sz>ws) ws=sz;
	UMX_data *ndata=(UMX_data*)GetBuffer(ws*sizeof(DUMS)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(DUMS)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(DUMS)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	ndata->sz=ws; esz=ws; //*(unsigned int*)((char*)ndata)=ws; esz=ws;// ndata+=UMSZ;
	if(edata){ edata->n=ndata; }
	edata=ndata; if(!data) data=edata;
	if(s) memcpy(edata->data, s+frs, (sz-frs)*sizeof(DUMS));
return ret;
}

inline DUMS& inserta(){ return (DUMS&)getelement(insert()); }
inline DUMS* vinsert(){ return (DUMS*)&getelement(insert()); }
inline DUMS& repaira(){ return (DUMS&)getelement(repair()); }
inline DUMS* vrepair(){ return (DUMS*)&getelement(repair()); }

inline unsigned int insert(){ char tm[sizeof(DUMS)]; memset(&tm, 0, sizeof(DUMS)); unsigned int ed=add((DUMS*)&tm, 1); DUMS*e=&getelement(ed); memset(e, 0, sizeof(*e)); return ed; }

bool del(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
				#ifdef DTULine_constructor
			((DUMS*)cdata.data+(e-cblockfn))->~DUMS();
				#endif
			 if(isdel) ((mbool*)(cdata+cdata->sz))->set(e-cblockfn, 0);
			 return 1;
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int repair(){ if(!isdel) return insert();
int s; mbool *mb; if(!cdata || 0<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		//s=*(unsigned int*)((char*)cdata-UMSZ);
		int i=0; mb=(mbool*)(cdata->data+s);

		for(i; i<s; i++){ if(i%8==0 && mb->isbyte(i/8)==255){ i+=8; continue; }
		if(!mb->get(i)){// if(cblockfn+i<usedsize) return insert();
		 mb->set(i, 1);
#ifdef DTULine_constructor
		new((DUMS*)cdata->data+i)DUMS;
#endif
		return cblockfn+i;
	}}
		cdata=cdata->n; cblockfn+=s;
	}
 return insert();
}

bool is(unsigned int e){ if(!isdel) return 1;
	if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
			 return ((mbool*)((DUMS*)cdata->data+cdata->sz))->get(e-cblockfn);
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int getpos(const DUMS *pos){
	cdata=data; cblockfn=0;
	while(cdata){
		if((DUMS*)&cdata->data<=pos && pos<(DUMS*)&cdata->data+cdata->sz){
			 return cblockfn+(pos-(DUMS*)(&cdata->data));
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return -1;
}

void *GetBuffer(int sz){
	void *d = malloc(sz);
	memset(d, 0, sz);
	return d;
}

void DelBuffer(void *p){
	free(p);
}

// SP
unsigned int GetFreeBuffer(DUMS**data){
	if(allsize==usedsize){ add((DUMS*)&nullel, 1); usedsize--; }
	unsigned int fs=allsize-usedsize;
	*data=(DUMS*)&edata->data+esz-fs;
	return fs;
}

void SetFreeBuffer(unsigned int add){ usedsize+=esz-add; }

//const LString& operator=(char *s){Clear(); return add(s, strlen(s));}
//const LString& operator=(MString s){Clear(); return add(s, s);}

operator MString(){
	MString ret; ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz;
	UMX_data*dt=data;

	if(data) while(dt){
		if(dt->sz+pos<ret.size()) sz=dt->sz;
		else{ sz=ret.size()-pos; }
		memcpy(ret.pos(pos), &dt->data, sz*sizeof(DUMS)); pos+=sz;
		dt=dt->n;	
	}
	return ret;
}

DUMS* pos(){ if(!data) return 0; return (DUMS*)data->data; }
operator DUMS*(){ if(!data) return 0; return (DUMS*)data->data; }
DUMS* nomov(){ return oneline(); }

DUMS* oneline(bool delfree=1){
	if(!data) return 0; if(data==edata) return (DUMS*)data->data;
	if(delfree) esz=usedsize; else esz=allsize;
	UMX_data *ndata=(UMX_data*)GetBuffer(esz*sizeof(DUMS)+UMSZ);
	//*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	ndata->sz=esz;
	unsigned int pos=0, sz;

	UMX_data *dt=data, *dd;
	while(dt){
		if(dt->sz+pos<usedsize) sz=dt->sz; else{ sz=usedsize-pos; }
		memcpy(ndata->data+pos*sizeof(DUMS), dt->data, sz*sizeof(DUMS)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
//*(long*)(char*)&dt=*(long*)((char*)dt-UMCSZ);
 		dd=dt; dt=dt->n; DelBuffer(dd);
	}
	data=ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
	return (DUMS*)data->data;
}

void Clear(){ if(!data) return ;
	UMX_data *dt=data, *dd;
	unsigned int pos=0, dsz, msz;

	while(dt){
		dsz=dt->sz; msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
#ifdef DTULine_destructor
		for(unsigned int i=0; i<msz; i++){
			((DUMS*)dt->data+i)->~DUMS();
		}
#endif
		dd=dt; dt=dt->n;		
		DelBuffer(dd);
	}
	data=0; edata=0; cdata=0; allsize=0; usedsize=0; 
}

// Add Func
	void i_want_mod_usedsize(int pos){ usedsize=pos; }

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class TiULine; // your class Index

// Defines 
//#define DTiULine_sz sizeof(DData) // General data size


 #define DTiULine_destructor 1 
#ifndef TiULine_minmem
#define TiULine_minmem	1024
#endif
#define TiULine_minel	16

//class TiULine_um{ DUMSi*n; unsigned int sz; DUMSi data[0]; };

class TiULine{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
UMX_data *data, *edata, *cdata; unsigned int cblockfn, esz;
const DUMSi nullel;
bool isdel;
public:
TiULine(unsigned int buf=TiULine_minmem) : nullel(){ Init(); bufsize=buf; }
~TiULine(){Clear();}

void insert(const VString& key, const VString& val, unsigned int i){ DUMSi &dt=inserta(); dt.key=key; dt.val=val; dt.i=i; return ; }
MString Find(const VString& key){
	if(data!=edata) oneline();
	DUMSi *d=&el(0); unsigned int s=size();
	for(unsigned int i=0; i<s; i++){ if(d[i].key==key) return d[i].val;} return "";
}
int FindI(const MString& key){if(data!=edata) oneline();
	DUMSi *d=&el(0); unsigned int s=size();
	for(unsigned int i=0; i<s; i++){ if(d[i].key==key) return i;} return -1;
}

void Init(){ memset(this, 0, sizeof(TiULine)); }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(DUMSi)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(DUMSi); }
unsigned int size()const {return usedsize;}

//const TiULine& operator+=(DUMSi *s){ return add(s, strlen(s)/sizeof(DUMSi));}
//const TiULine& operator+=(MString &s){ return add(s, s);}

const DUMSi& operator[](const unsigned int e){ return getelement(e); }
const DUMSi& operator[](const int e){ return getelement(e); }

DUMSi& el(const unsigned int e){ return getelement(e); }

DUMSi& getelement(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz) return *((DUMSi*)(&cdata->data)+(e-cblockfn));
		cblockfn+=cdata->sz; cdata=cdata->n;
	}
return (DUMSi&)nullel;
}

inline const unsigned int add(const DUMSi &s){ return add(&s, sizeof(DUMSi)); }
//inline DUMSi& inserta(){return (DUMSi&)*insert();}

inline const unsigned int add(const DUMSi *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=TiULine_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s){ memcpy((DUMSi*)&edata->data+esz-frs, s, ws*sizeof(DUMSi)); }		
		usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(DUMSi); if(ws<1) ws=1; if(sz>ws) ws=sz;
	UMX_data *ndata=(UMX_data*)GetBuffer(ws*sizeof(DUMSi)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(DUMSi)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(DUMSi)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	ndata->sz=ws; esz=ws; //*(unsigned int*)((char*)ndata)=ws; esz=ws;// ndata+=UMSZ;
	if(edata){ edata->n=ndata; }
	edata=ndata; if(!data) data=edata;
	if(s) memcpy(edata->data, s+frs, (sz-frs)*sizeof(DUMSi));
return ret;
}

inline DUMSi& inserta(){ return (DUMSi&)getelement(insert()); }
inline DUMSi* vinsert(){ return (DUMSi*)&getelement(insert()); }
inline DUMSi& repaira(){ return (DUMSi&)getelement(repair()); }
inline DUMSi* vrepair(){ return (DUMSi*)&getelement(repair()); }

inline unsigned int insert(){ char tm[sizeof(DUMSi)]; memset(&tm, 0, sizeof(DUMSi)); unsigned int ed=add((DUMSi*)&tm, 1); DUMSi*e=&getelement(ed); memset(e, 0, sizeof(*e)); return ed; }

bool del(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
				#ifdef DTiULine_constructor
			((DUMSi*)cdata.data+(e-cblockfn))->~DUMSi();
				#endif
			 if(isdel) ((mbool*)(cdata+cdata->sz))->set(e-cblockfn, 0);
			 return 1;
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int repair(){ if(!isdel) return insert();
int s; mbool *mb; if(!cdata || 0<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		//s=*(unsigned int*)((char*)cdata-UMSZ);
		int i=0; mb=(mbool*)(cdata->data+s);

		for(i; i<s; i++){ if(i%8==0 && mb->isbyte(i/8)==255){ i+=8; continue; }
		if(!mb->get(i)){// if(cblockfn+i<usedsize) return insert();
		 mb->set(i, 1);
#ifdef DTiULine_constructor
		new((DUMSi*)cdata->data+i)DUMSi;
#endif
		return cblockfn+i;
	}}
		cdata=cdata->n; cblockfn+=s;
	}
 return insert();
}

bool is(unsigned int e){ if(!isdel) return 1;
	if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
			 return ((mbool*)((DUMSi*)cdata->data+cdata->sz))->get(e-cblockfn);
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int getpos(const DUMSi *pos){
	cdata=data; cblockfn=0;
	while(cdata){
		if((DUMSi*)&cdata->data<=pos && pos<(DUMSi*)&cdata->data+cdata->sz){
			 return cblockfn+(pos-(DUMSi*)(&cdata->data));
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return -1;
}

void *GetBuffer(int sz){
	void *d = malloc(sz);
	memset(d, 0, sz);
	return d;
}

void DelBuffer(void *p){
	free(p);
}

// SP
unsigned int GetFreeBuffer(DUMSi**data){
	if(allsize==usedsize){ add((DUMSi*)&nullel, 1); usedsize--; }
	unsigned int fs=allsize-usedsize;
	*data=(DUMSi*)&edata->data+esz-fs;
	return fs;
}

void SetFreeBuffer(unsigned int add){ usedsize+=esz-add; }

//const LString& operator=(char *s){Clear(); return add(s, strlen(s));}
//const LString& operator=(MString s){Clear(); return add(s, s);}

operator MString(){
	MString ret; ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz;
	UMX_data*dt=data;

	if(data) while(dt){
		if(dt->sz+pos<ret.size()) sz=dt->sz;
		else{ sz=ret.size()-pos; }
		memcpy(ret.pos(pos), &dt->data, sz*sizeof(DUMSi)); pos+=sz;
		dt=dt->n;	
	}
	return ret;
}

DUMSi* pos(){ if(!data) return 0; return (DUMSi*)data->data; }
operator DUMSi*(){ if(!data) return 0; return (DUMSi*)data->data; }
DUMSi* nomov(){ return oneline(); }

DUMSi* oneline(bool delfree=1){
	if(!data) return 0; if(data==edata) return (DUMSi*)data->data;
	if(delfree) esz=usedsize; else esz=allsize;
	UMX_data *ndata=(UMX_data*)GetBuffer(esz*sizeof(DUMSi)+UMSZ);
	//*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	ndata->sz=esz;
	unsigned int pos=0, sz;

	UMX_data *dt=data, *dd;
	while(dt){
		if(dt->sz+pos<usedsize) sz=dt->sz; else{ sz=usedsize-pos; }
		memcpy(ndata->data+pos*sizeof(DUMSi), dt->data, sz*sizeof(DUMSi)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
//*(long*)(char*)&dt=*(long*)((char*)dt-UMCSZ);
 		dd=dt; dt=dt->n; DelBuffer(dd);
	}
	data=ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
	return (DUMSi*)data->data;
}

void Clear(){ if(!data) return ;
	UMX_data *dt=data, *dd;
	unsigned int pos=0, dsz, msz;

	while(dt){
		dsz=dt->sz; msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
#ifdef DTiULine_destructor
		for(unsigned int i=0; i<msz; i++){
			((DUMSi*)dt->data+i)->~DUMSi();
		}
#endif
		dd=dt; dt=dt->n;		
		DelBuffer(dd);
	}
	data=0; edata=0; cdata=0; allsize=0; usedsize=0; 
}

// Add Func
	void i_want_mod_usedsize(int pos){ usedsize=pos; }

};// END TEMPLATE 
// Do Not Write To This File, All Data Well Be Lost!
// ##EMD5:34cad9373042c92256c5a7b1042fb289

