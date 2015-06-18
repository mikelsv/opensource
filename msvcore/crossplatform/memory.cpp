/*
inline void * new_ncm(size_t size){
void *p; while ((p = malloc(size)) == 0) {Sleep(1);}
	return p;
}

// sup func
void * GetBufferNCM(int size){ void *buf=new_ncm(size); memset(buf, 0, size); return buf; }
//#define GetBufferNCM(cl, size){GetBufferNCM(sizeof(cl)*size)}
void DelBufferNCM(void * buf){ free(buf); buf=0; return ; }
			 
void* InsertVDataNCM(void *v, int sz, int pos, int nsz, bool null){
void *nw=new_ncm(sz+nsz);
memcpy(nw, v, pos); memcpy((char*)nw+pos+nsz, (char*)v+pos, sz-pos);
if(null) memset((char*)nw+pos, 0, nsz);
free(v); return nw;
}

void* DeleteVDataNCM(void *v, int sz, int pos, int dsz){
void *nw=new_ncm(sz-dsz);
memcpy(nw, v, pos); memcpy((char*)nw+pos, (char*)v+pos+dsz, sz-pos-dsz);
free(v); return nw;
}
*/						

/*
class _MemoryControlGetBuffer{
public:

	void * GetBuffer(int size){ void *buf=_newn(size); memset(buf, 0, size); return buf; }

	void* InsertVData(void *v, int sz, int pos, int nsz, bool null){
		void *nw=_newn(sz+nsz);
		memcpy(nw, v, pos); memcpy((char*)nw+pos+nsz, (char*)v+pos, sz-pos);
		if(null) memset((char*)nw+pos, 0, nsz);
		_del(v); return nw;
	}

	void* AddVData(void *v, int sz, int nsz){
		void *nw=_newn(sz+nsz);
		if(sz) memcpy((char*)nw, (char*)v, sz);
		memset((char*)nw+sz, 0, nsz);
		_del(v); return nw;
	}

	void* DeleteVData(void *v, int sz, int pos, int dsz){
		void *nw=_newn(sz-dsz);
		memcpy(nw, v, pos); memcpy((char*)nw+pos, (char*)v+pos+dsz, sz-pos-dsz);
		_del(v); return nw;
	}

	void DelBuffer(void * buf){ _del(buf); return ; }

} MemoryControlGetBuffer;


void * GetBuffer(int size){
	return MemoryControlGetBuffer.GetBuffer(size);
}

void * AddVData(void *v, int sz, int nsz){
	return MemoryControlGetBuffer.AddVData(v, sz, nsz);
}

void DelBuffer(void * buf){
	return MemoryControlGetBuffer.DelBuffer(buf);
}*/

// NORMAL MEMORY OPERATIONS
/*#define LMATRIX_VER "0.1.2.0"
void * GetBuffer(int size){ void *buf=_newn(size); memset(buf, 0, size); return buf; }
void * GetBufferNC(int size){ return _newn(size); }
//#define GetBuffercl(cl, size){ GetBuffer(sizeof(cl)*size) } 
void DelBuffer(void * buf){ _del(buf); return ; }

void* InsertVData(void *v, int sz, int pos, int nsz, bool null){
void *nw=_newn(sz+nsz);
memcpy(nw, v, pos); memcpy((char*)nw+pos+nsz, (char*)v+pos, sz-pos);
if(null) memset((char*)nw+pos, 0, nsz);
_del(v); return nw;
}

void* AddVData(void *v, int sz, int nsz){
void *nw=_newn(sz+nsz);
if(sz) memcpy((char*)nw, (char*)v, sz);
memset((char*)nw+sz, 0, nsz);
_del(v); return nw;
}

void* DeleteVData(void *v, int sz, int pos, int dsz){
void *nw=_newn(sz-dsz);
memcpy(nw, v, pos); memcpy((char*)nw+pos, (char*)v+pos+dsz, sz-pos-dsz);
_del(v); return nw;
}*/


mbool::mbool(){data=0; sz=0;}
mbool::mbool(void*val){data=(BYTE*)val; sz=0;}
void mbool::Del(){_deln(data);}
bool* mbool::New(unsigned int size){ sz=size/8 + (size%8>0 ? 1 : 0); data=(BYTE*)malloc(sz); return (bool*)data; }
bool* mbool::operator=(bool*val){data=(BYTE*)val; return val;}
bool mbool::operator[](int num){return 0!=(data[num/8]&(1 << (num%8)));}
//inline bool operator[](int num, int s){return 0!=(data[num/8]&(1 << (num%8)));}
bool mbool::operator[](unsigned int num){return 0!=(data[num/8]&(1 << (num%8)));}
bool mbool::get(unsigned int num){return 0!=(data[num/8]&(1 << (num%8)));}
bool mbool::operator[](unsigned short num){return 0!=(data[num/8]&(1 << (num%8)));}
mbool::operator bool*(){return (bool*)data;}
void mbool::set(int num, bool val){
	if(!val) data[num/8]&=~(1 << (num%8));
	else data[num/8]|=1 << (num%8);
	return ;}
	//data[num/8]|=(1 << (num%8));
	//data[num/8]^=(1 << (num%8));
	//data[num/8]|=val*(1 << (num%8));
//	print("Set "+itos(num)+"="+itos(val)+" e:"+itos(data[num/8]&(1 << (num%8))));
void mbool::i(int num, bool val){ set(num, val); return ; }
bool mbool::i(int num){ return 0!=(data[num/8]&(1 << (num%8))); }
bool mbool::is(unsigned int size){ size=size/8 + (size%8>0 ? 1 : 0); if(size==sz) return 1; return 0;}
void mbool::Null(){memset(data, 0, sz); }
BYTE mbool::isbyte(const unsigned int b){return *(data+b);}



/*
class mbool{
BYTE * data;
unsigned int sz;
public:
mbool(){data=0; sz=0;}
mbool(void*val){data=(BYTE*)val; sz=0;}
void Del(){_deln(data);}
bool* New(unsigned int size){ sz=size/8 + (size%8>0 ? 1 : 0); data=(BYTE*)GetBuffer( sz ); return (bool*)data; }
bool* operator=(bool*val){data=(BYTE*)val; return val;}
inline bool operator[](int num){return 0!=(data[num/8]&(1 << (num%8)));}
//inline bool operator[](int num, int s){return 0!=(data[num/8]&(1 << (num%8)));}
inline bool operator[](unsigned int num){return 0!=(data[num/8]&(1 << (num%8)));}
inline bool get(unsigned int num){return 0!=(data[num/8]&(1 << (num%8)));}
inline bool operator[](unsigned short num){return 0!=(data[num/8]&(1 << (num%8)));}
inline operator bool*(){return (bool*)data;}
void set(int num, bool val){
	//data[num/8]|=(1 << (num%8));
	//data[num/8]^=(1 << (num%8));
	//data[num/8]|=val*(1 << (num%8));
	if(!val) data[num/8]&=~(val*(1 << (num%8)));
	else data[num/8]|=val*(1 << (num%8));
//	print("Set "+itos(num)+"="+itos(val)+" e:"+itos(data[num/8]&(1 << (num%8))));
	return ;}
inline void i(int num, bool val){ set(num, val); return ; }
inline bool i(int num){ return 0!=(data[num/8]&(1 << (num%8))); }
bool is(unsigned int size){ size=size/8 + (size%8>0 ? 1 : 0); if(size==sz) return 1; return 0;}
void Null(){memset(data, 0, sz); }
BYTE isbyte(const unsigned int b){return *(data+b);}
};*/

/*
struct MEMCon{
	void * mem;
	int size;
};
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class DMEMUMatrix; // your class Index

// Defines 
//#define DDMEMUMatrix_sz sizeof(DData) // General data size



#define DMEMUMatrix_minmem	1048576
#define DMEMUMatrix_minel	131072

class DMEMUMatrix{// Unlimited Matrix
protected:
int allsize, usedsize, bufsize, blocks;
MEMCon *data, *edata, *cdata; unsigned int cblockfn, esz;
const MEMCon nullel;

public:
DMEMUMatrix(unsigned int buf=DMEMUMatrix_minmem) : nullel(){ Init(); bufsize=buf; }
~DMEMUMatrix(){Clear();}

void Init(){ memset(this, 0, sizeof(DMEMUMatrix));}

void NewBuf(int buf){ bufsize=buf; }
unsigned int size()const {return usedsize;}

//const DMEMUMatrix& operator+=(MEMCon *s){ return add(s, strlen(s)/sizeof(MEMCon));}
//const DMEMUMatrix& operator+=(MString &s){ return add(s, s);}

const MEMCon& operator[](const unsigned int e){
int s; if(!cdata || e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-8);
		if(e<cblockfn+s)  return cdata[e-cblockfn];
		//cdata=(MEMCon*)(char*)((cdata)-4);
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-4); cblockfn+=s;
	} return nullel;
}

const MEMCon& operator[](const int e){
int s; if(!cdata || (unsigned int)e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-8);
		if((unsigned int)e<cblockfn+s)  return cdata[(unsigned int)e-cblockfn];
		//cdata=(MEMCon*)(char*)((cdata)-4);
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-4); cblockfn+=s;
	} return nullel;
}

MEMCon& getelement(unsigned int e){
int s; if(!cdata || e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-8);
		if(e<cblockfn+s)  return cdata[e-cblockfn];
		//cdata=(MEMCon*)(char*)((cdata)-4);
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-4); cblockfn+=s;
	}
return (MEMCon&)nullel;
}

MEMCon& el(unsigned int e){
int s; if(!cdata || e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-8);
		if(e<cblockfn+s)  return cdata[e-cblockfn];
		//cdata=(MEMCon*)(char*)((cdata)-4);
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-4); cblockfn+=s;
	}
return (MEMCon&)nullel;
}

inline const unsigned int add(const MEMCon &s){ return add(&s, 1); }
inline MEMCon& insert(){return getelement(add(&MEMCon(), 1));}

inline const unsigned int add(const MEMCon *s, const unsigned int sz=1){ if(!s || !sz) return -1;
	if(!bufsize) bufsize=1024; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		memcpy(edata+esz-frs, s, ws*sizeof(MEMCon)); usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=DMEMUMatrix_minmem/sizeof(MEMCon); if(ws<DMEMUMatrix_minel) ws=DMEMUMatrix_minel; if(sz>ws) ws=sz;
	char *ndata=(char*)GetBufferNCM(ws*sizeof(MEMCon)+8); blocks++;
	 allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	*(unsigned int*)((char*)ndata)=ws; esz=ws; ndata+=8;
	if(edata){ *(unsigned int*)((char*)edata-4)=*(unsigned int*)&ndata; }
	edata=(MEMCon*)((char*)ndata); if(!data) data=edata;
	memcpy(edata, s+frs, (sz-frs)*sizeof(MEMCon));
return ret;
}


// SP
unsigned int GetFreeBuffer(MEMCon**data){
if(allsize==usedsize) {add((MEMCon*)&nullel, 1); usedsize--;}
unsigned int fs=allsize-usedsize;
*data=edata+esz-fs;
return fs;
}

void SetFreeBuffer(unsigned int add){usedsize+=esz-add; }

//const LString& operator=(char *s){Clear(); return add(s, strlen(s));}
//const LString& operator=(MString s){Clear(); return add(s, s);}
/*
operator MString(){
	MString ret; ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz, dsz;
MEMCon*dt=(MEMCon*)data;

if(data) while(dt){
dsz=*(unsigned int*)((char*)dt-8);
if(dsz+pos<ret.size()) sz=dsz;
else {sz=ret.size()-pos;}
memcpy(ret.pos(pos), dt, sz*sizeof(MEMCon)); pos+=dsz;
//memcpy(&dd, (char*)(dt-4), 4);
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-4);
}
return ret;
}* /

MEMCon* pos(){return data;}
operator MEMCon*(){return data;}

bool oneline( bool delfree=1){ if(!data) return 0; if(data==edata) return 1;
	if(delfree) esz=usedsize; else esz=allsize;
	char *ndata=(char*)GetBufferNCM(esz*sizeof(MEMCon)+8);
	*(unsigned int*)((char*)ndata)=esz; ndata+=8;
	unsigned int pos=0, sz, dsz;
MEMCon*dt=data, *dd;
while(dt){
dd=(MEMCon*)((char*)dt-8); dsz=*(unsigned int*)((char*)dd);
if(dsz+pos<(unsigned int)usedsize) sz=dsz;
else {sz=usedsize-pos;}
memcpy(ndata+pos*sizeof(MEMCon), dt, sz*sizeof(MEMCon)); pos+=dsz;
//memcpy(&dd, (char*)(dt-4), 4);
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-4);
DelBufferNCM(dd);
}
data=(MEMCon*)ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
return 1;
}

void Clear(){ if(!data) return ;
MEMCon*dt=(MEMCon*)data, *dd;
#ifdef DDMEMUMatrix_destructor
unsigned int pos=0, dsz, msz;
while(dt){
dsz=*(unsigned int*)((char*)dt-8); msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
dd=(MEMCon*)((char*)dt-8); // ((char*)(dt))-8;
//dt=(char*)*(unsigned int*)((char*)dd+4); //(void*)((char*)dd+4);// (void*)dd+4;
for(int i=0; i<msz; i++){
dt[i].~MEMCon();
}
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-4);
DelBufferNCM(dd);
}
#else if
while(dt){
dd=(MEMCon*)((char*)dt-8); // ((char*)(dt))-8;
//dt=(char*)*(unsigned int*)((char*)dd+4); //(void*)((char*)dd+4);// (void*)dd+4;
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-4);
DelBufferNCM(dd);
}
#endif
data=0; edata=0; cdata=0; allsize=0; usedsize=0;
}
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW LIVE MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class MEMMatrix; class D_MEMMatrix; class VDMEMMatrix; class MEMMatrixD; class DMEMMatrix; // your class Index // DDS

// Defines 
#define DMEMMatrix_sz sizeof(D_MEMMatrix) // Data matrix size
#define DMEMMatrix_gsz sizeof(MEMMatrix) // Global data size
#define DMEMMatrix_dsz sizeof(MEMMatrixD) // Matrix Data size

 #define DMEMMatrix_isdelete 1 // + 8 byte 


#define DMEMMatrix_min 128
#define DMEMMatrix_max 65535




class D_MEMMatrix{public: // data class // add your data
	D_MEMMatrix *_p, *_n;
	void * mem;
	int size;	
};

class VDMEMMatrix{ public: D_MEMMatrix *v; }; // virtual class // add your data

class MEMMatrixD{ public:
D_MEMMatrix*data;
unsigned short datas;
unsigned short databls;
#ifdef DMEMMatrix_isdelete
mbool datadel;
unsigned short datadelbls;
#endif
};

class DMEMMatrix{
public:
MEMMatrix*lmatrix; D_MEMMatrix*dmatrix;
unsigned int _dblock; unsigned short _dnum;

DMEMMatrix(MEMMatrix&lm){lmatrix=0; Lock(&lm);} DMEMMatrix(MEMMatrix*lm){lmatrix=0; Lock(lm);}// DMEMMatrix(void*lm){}
DMEMMatrix& operator=(int lm){dmatrix=0; return *this;}
~DMEMMatrix(){UnLock();}
void operator=(DMEMMatrix&dlm){ Lock(dlm.lmatrix); memcpy(this, &dlm, sizeof(DMEMMatrix)); }
//void operator=(MEMMatrix*lm){Lock(lm);} void operator=(MEMMatrix&lm){Lock(&lm);}
void operator=(D_MEMMatrix*dm){dmatrix=dm;}
inline D_MEMMatrix* operator->()const{return dmatrix;} operator D_MEMMatrix* (){return dmatrix;}

inline void New(){_dblock=0; _dnum=0;} void Lock(MEMMatrix*lm); void UnLock(); inline bool GetData();
};


class MEMMatrix{
protected:
#ifdef DMEMMatrix_extrime
public:
#endif
MEMMatrixD*_data;
unsigned int _gblocks; /* global blocks size* /  unsigned int _gbfi;		// global block for insert

unsigned int _sz;
//unsigned int _dblock; unsigned short _dnum;
unsigned short _mode;
public: unsigned short _cursors; char _sunc;
// special //unsigned int _ft; // fasttime //unsigned int _st; // smalltime

public:
/////////////////////////////////////////////////////////////////////////////////////////////////////
MEMMatrix(unsigned short mode=0){Init(); if(mode) _mode=mode;}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Init(){ memset(this, 0, DMEMMatrix_gsz); _mode=50; NewData();  }
inline unsigned int size()const{return _sz;}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMode(int m){_mode=m;}

D_MEMMatrix *_a, *_e;

D_MEMMatrix* insert(void *mem, unsigned int size){
D_MEMMatrix* ins=_a;
if(!_a) {_e=Insert(); _a=_e;} else {ins=_e; _e=Insert(); ins->_n=_e; _e->_p=ins;}
_e->mem=mem; _e->size=size; return _e;
}

D_MEMMatrix* find(void*m){ if(!m || !_a) return 0;
D_MEMMatrix* ins=_a;
while(ins){if(ins->mem<=m && ((char*)(ins->mem)+ins->size)>m) return ins;
ins=ins->_n;
}// ERROR: No find
return ins;
}

D_MEMMatrix* pos(int id)const{ if(!_a) return 0;
D_MEMMatrix* ins=_a;
while(id && ins){
ins=ins->_n;
}// ERROR: No find
return ins;
}

bool del(D_MEMMatrix * mem){
D_MEMMatrix *pi, *ni, *dl; 
	if (mem->_n) dl=mem->_n->_p;
	else if (mem->_p) dl=mem->_p->_n;
	else {DeleteData(_a); _a=0; _e=0; return 1;}
    if (mem->_n) ni=mem->_n; else ni=NULL;
	if (mem->_p) pi=mem->_p; else pi=NULL;
	if (ni) ni->_p=pi;
	else _e=mem->_p;
	if (pi) pi->_n=ni;
	else _a=mem->_n;
	//DMEMMatrix dlm(this); dlm=dl;
	DeleteData(dl);
return 1;
}
/*
bool del(int id){
D_MTextLine* ins=_a, *pi, *ni, *dl; 
if(id<_sz-id){for(int i=0; i<id; i++) ins=ins->_n; }
else {ins=_e; for(int i=_sz-1; i>id; i--) {ins=ins->_p;}}
if(!ins) return 1;

	if (ins->_n) dl=ins->_n->_p;
	else if (ins->_p) dl=ins->_p->_n;
	else {delete _a; _a=0; _e=0; return 1;}
    if (ins->_n) ni=ins->_n; else ni=NULL;
	if (ins->_p) pi=ins->_p; else pi=NULL;
	if (ni) ni->_p=pi;
	else _e=ins->_p;
	if (pi) pi->_n=ni;
	else  _a=ins->_n;
	delete dl;
    return 1;
}* /

/////////////////////////////////////////////////////////////////////////////////////////////////////
int NewData(){
	if(_gbfi+1<_gblocks) {_gbfi++; return 1;}
	_data=(MEMMatrixD*)InsertVDataNCM(_data, DMEMMatrix_dsz*_gblocks, _gblocks*DMEMMatrix_dsz, DMEMMatrix_dsz, 1);
	int nbls;
	_gblocks++; nbls=_sz*_mode/100; if(nbls<DMEMMatrix_dsz*2/DMEMMatrix_sz) nbls=DMEMMatrix_dsz*2/DMEMMatrix_sz;
	if(nbls<DMEMMatrix_min) nbls=DMEMMatrix_min; else if(nbls>DMEMMatrix_max) nbls=DMEMMatrix_max;
	_gbfi=_gblocks-1; //_sz+=nbls;
	_data[_gbfi].data=(D_MEMMatrix*)GetBufferNCM(nbls*DMEMMatrix_sz);
	_data[_gbfi].databls=nbls; _data[_gbfi].datas=0;
#ifdef DMEMMatrix_isdelete
	int t=(nbls)/8+((nbls)%8>0);
	_data[_gbfi].datadel=(bool*)GetBufferNCM(t);
	memset(_data[_gbfi].datadel, 255, t);
#endif

#ifdef MEMMatrixDebug
	float prs=_sz*DMEMMatrix_sz; float prb=_gblocks*DMEMMatrix_dsz+DMEMMatrix_gsz+nbls*DMEMMatrix_sz;
	unsigned int allsize=0; for(int i=0; i<_gblocks; i++){allsize+=_data[i].databls*DMEMMatrix_sz;}
#ifdef DMEMMatrix_isdelete
	for(int i=0; i<_gblocks; i++){allsize+=_data[i].databls*t;}
#endif
	allsize+=DMEMMatrix_dsz*_gblocks;
	//printf("MEMMatrix: add %d blocks, otn: %f / %f = %f , Allsize: %d \r\n", nbls, prs, prb, prs/prb, allsize);
#endif
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Sunc(){ return ;
if(_sunc && !_cursors && 0){
for(unsigned int i=0; i<_gblocks; i++){
if(_data[i].databls==0
#ifdef DMEMMatrix_isdelete
 || _data[i].databls==_data[i].datas && _data[i].databls==_data[i].datadelbls
#endif
){_data[i].~MEMMatrixD();
_data=(MEMMatrixD*)DeleteVDataNCM(_data, _gblocks*DMEMMatrix_dsz, i*DMEMMatrix_dsz, DMEMMatrix_dsz);
 }}
}}

/////////////////////////////////////////////////////////////////////////////////////////////////////
D_MEMMatrix* Insert(){ _sz++;
	if(_data[_gbfi].datas>=_data[_gbfi].databls) { NewData();}
#ifdef DMEMMatrix_isdelete

#endif
#ifdef DMEMMatrix_constructor
	D_MEMMatrix*dl=Ret(_gbfi, _data[_gbfi].datas++);
	return new(dl)D_MEMMatrix;
#else
	return Ret(_gbfi, _data[_gbfi].datas++);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
inline D_MEMMatrix *Ret(unsigned int bl, unsigned int num){
#ifdef DMEMMatrix_vtype
	return ((VLLine*)&_data[bl].data[num])->v;
#else
	return &_data[bl].data[num];
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//inline int NewGetData(){_dblock=0; _dnum=0; return 0;}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetData(DMEMMatrix&dlm){//int &block, int &num
nb:	if( _data[dlm._dblock].datas>dlm._dnum) 
#ifdef DConnectsLine_isdelete
		if(!_data[dlm._dblock].datadel[dlm._dnum]) {dlm._dnum++; goto nb;} else 
#endif
	{dlm=Ret(dlm._dblock,dlm._dnum++); return 1;}
	dlm._dblock++; dlm._dnum=0;
	if(dlm._dblock>=_gblocks) {dlm=(D_MEMMatrix*)0; return 0;} goto nb;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetPData(DMEMMatrix&dlm){//int &block, int &num
nb:	if(dlm._dnum>=0)
#ifdef DConnectsLine_isdelete
	if(!_data[dlm._dblock].datadel[dlm._dnum]) {dlm._dnum--; goto nb;} else 
#endif
	{dlm=Ret(dlm._dblock,dlm._dnum--); return 1;}
	dlm._dblock--; dlm._dnum=_data[dlm._dblock].datas-1;
	if(dlm._dblock<0) {dlm=(D_MEMMatrix*)0; return 0;} goto nb;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPos(D_MEMMatrix*dl, DMEMMatrix&dlm){
for(unsigned int i=0; i<_gblocks; i++){
if(dl>_data[i].data && dl<_data[i].data+_data[i].databls){
 dlm=dl; dlm._dblock=i; dlm._dnum=(unsigned short)(dl-_data[i].data); return 1;
}}dlm.UnLock(); dlm.New(); return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int DeleteData(D_MEMMatrix*dl){
	DMEMMatrix dlm(this); if(GetPos(dl, dlm)) return DeleteData(dl, dlm._dblock, dlm._dnum);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int DeleteData(DMEMMatrix&dlm){
	unsigned int block=dlm._dblock; unsigned short num=dlm._dnum; D_MEMMatrix*dta=dlm; // add foi
		
	while(block>=0){
		if(Ret(block,num)==dta){return DeleteData(dta, block, num);}
		num--; if(num<0) {block--; num=_data[block].datas-1;}
	}

	block=dlm._dblock, num=dlm._dnum;
	while(block<_gblocks){
		if(Ret(block,num)==dta){return DeleteData(dta, block, num);}
		num++; if(num>=_data[block].datas) {block++; num=0;}
	}
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
inline int DeleteData(D_MEMMatrix*dta, unsigned int &_dblock, unsigned short &_dnum){_sz--; _sunc=1;
#ifdef DMEMMatrix_destructor
	_data[_dblock].data[_dnum].~D_MEMMatrix();
#endif
#ifdef DMEMMatrix_isdelete
	_data[_dblock].datadel.set(_dnum,0); _data[_dblock].datadelbls++;
#else
	return RealDelete(dta, _dblock, _dnum);
#endif
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
inline int RealDelete(D_MEMMatrix*dta, unsigned int &block, unsigned short &num){
	_data[block].data=(D_MEMMatrix*)DeleteVDataNCM(_data[block].data, _data[block].databls*DMEMMatrix_dsz, num*DMEMMatrix_dsz, DMEMMatrix_dsz);
	_data[block].databls--; _data[block].datas--;
	// //if(block==_dblock && _dnum>num)_dnum--;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
int Clear(int e=1){
	for(unsigned int i=0; i<_gblocks; i++){
#ifdef DMEMMatrix_destructor
	for(int ii=0; ii<_data[i].datas; ii++){
#ifdef DMEMMatrix_isdelete
	if(_data[i].datadel[ii])
#endif
	_data[i].data[ii].~D_MEMMatrix();
	}
#endif
	DelBufferNCM(_data[i].data);
#ifdef DMEMMatrix_isdelete
	DelBufferNCM(_data[i].datadel);
#endif
	}
#ifdef DMEMMatrix_isdelete
	//DeleteBuffer(_delblocks); DeleteBuffer(_datadel);
#endif
	_deln(_data);
	if(e) Init(); 
return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
	//~MEMMatrix(){Clear(0);}
};

void DMEMMatrix::Lock(MEMMatrix*lm){if(lmatrix) {lmatrix->_cursors--; if(lmatrix->_cursors==0) lmatrix->Sunc();} lmatrix=lm; dmatrix=0; if(lmatrix) {lmatrix->_cursors++; _dblock=0; _dnum=0;}}
void DMEMMatrix::UnLock(){ if(lmatrix) {lmatrix->_cursors--; if(lmatrix->_cursors==0) lmatrix->Sunc();} lmatrix=0; dmatrix=0;}
inline bool DMEMMatrix::GetData(){return lmatrix->GetData(*this);}

/////////////////////////////////////////////////////////////////////////////////////////////////////
*/