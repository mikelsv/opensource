// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:0a67e022602b6d2706ca79ba8b874f9a : 2007.09.26 23:03:45


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW LIVE MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class MTLine; class D_MTLine; class VDMTLine; class MTLineD; class DMTLine; // your class Index // DDS

// Defines 
#define DMTLine_sz sizeof(D_MTLine) // Data matrix size
#define DMTLine_gsz sizeof(MTLine) // Global data size
#define DMTLine_dsz sizeof(MTLineD) // Matrix Data size

 #define DMTLine_isdelete 1 // + 8 byte 

 #define DMTLine_destructor 1 
#define DMTLine_min 10
#define DMTLine_max 65535





class D_MTLine{public: // data class // add your data

	MultiType key, val;		
	MTLine*up;	
	~D_MTLine(){_deln(up);}	
};

//class VDMTLine{ public: D_MTLine *v; }; // virtual class // add your data

class MTLineD{ public:
D_MTLine*data;
unsigned short datas;
unsigned short databls;
#ifdef DMTLine_isdelete
mbool datadel;
unsigned short datadelbls;
#endif
};

class DMTLine{
public:
MTLine*lmatrix; D_MTLine*dmatrix;
int _dblock; int _dnum;

DMTLine(MTLine&lm){lmatrix=0; Lock(&lm);} DMTLine(MTLine*lm){lmatrix=0; Lock(lm);}// DMTLine(void*lm){}
DMTLine& operator=(int lm){dmatrix=0; return *this;}
~DMTLine(){UnLock();}
void operator=(DMTLine&dlm){ Lock(dlm.lmatrix); memcpy(this, &dlm, sizeof(DMTLine)); }
//void operator=(MTLine*lm){Lock(lm);} void operator=(MTLine&lm){Lock(&lm);}
void operator=(D_MTLine*dm){dmatrix=dm;}
inline D_MTLine* operator->()const{return dmatrix;} operator D_MTLine* (){return dmatrix;}

inline void New(){_dblock=0; _dnum=0;} void Lock(MTLine*lm); void UnLock(); inline bool GetData();
};

class MTLine{
protected:
#ifdef DMTLine_extrime
public:
#endif
MTLineD*_data;
unsigned int _gblocks; /* global blocks size*/  unsigned int _gbfi;		// global block for insert

unsigned int _sz;
//unsigned int _dblock; unsigned short _dnum;
unsigned short _mode;
public: unsigned short _cursors; char _sunc;
// special //unsigned int _ft; // fasttime //unsigned int _st; // smalltime

// isuni : 1 - full wr, 2 - wr only; isruni: 1- full ro

public:
/////////////////////////////////////////////////////////////////////////////////////////////////////
MTLine(unsigned short mode=0){Init(); if(mode) _mode=mode; }
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Lock(){} void UnLock(){} void RLock(){} void RUnLock(){}
/////////////////////////////////////////////////////////////////////////////////////////////////////
class MTLineLock{ bool locktype; MTLine * lm;
public:
MTLineLock(MTLine*lm, int full=1){}
~MTLineLock(){ } }; /////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Init(){ memset(this, 0, DMTLine_gsz); _mode=50; NewData();  }
inline unsigned int size()const{return _sz;}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMode(int m){_mode=m;}

	DMTLine Find(MString s){ DMTLine dl(this); while(GetData(dl)){ if(cmpuc(dl->key.operator MString(),s)) return dl; } return 0; }
	DMTLine Find(MString s, MTLine *ts){ DMTLine dl(ts); while(ts->GetData(dl)){ if(cmpuc(dl->key.operator MString(),s)) return dl; } return 0; }

	DMTLine Find(int s){ DMTLine dl(this); while(GetData(dl)){ if(dl->key.operator int()==s) return dl; } return 0; }
	DMTLine Find(int s, MTLine*ts){ DMTLine dl(ts); while(ts->GetData(dl)){ if(dl->key.operator int()==s) return dl; } return 0; }


/////////////////////////////////////////////////////////////////////////////////////////////////////
int NewData(){
	if(_gbfi+1<_gblocks) {_gbfi++; return 1;} Lock();
	_data=(MTLineD*)InsertVData(_data, DMTLine_dsz*_gblocks, _gblocks*DMTLine_dsz, DMTLine_dsz, 1);
	int nbls;
	_gblocks++; nbls=_sz*_mode/100; if(nbls<DMTLine_dsz*2/DMTLine_sz) nbls=DMTLine_dsz*2/DMTLine_sz;
	if(nbls<DMTLine_min) nbls=DMTLine_min; else if(nbls>DMTLine_max) nbls=DMTLine_max;
	_gbfi=_gblocks-1; //_sz+=nbls;
	_data[_gbfi].data=(D_MTLine*)GetBuffer(nbls*DMTLine_sz);
	_data[_gbfi].databls=nbls; _data[_gbfi].datas=0;
#ifdef DMTLine_isdelete
	int t=(nbls)/8+((nbls)%8>0);
	_data[_gbfi].datadel=(bool*)GetBuffer(t);
	memset(_data[_gbfi].datadel, 255, t);
#endif

#ifdef MTLineDebug
	float prs=_sz*DMTLine_sz; float prb=_gblocks*DMTLine_dsz+DMTLine_gsz+nbls*DMTLine_sz;
	unsigned int allsize=0; for(int i=0; i<_gblocks; i++){allsize+=_data[i].databls*DMTLine_sz;}
#ifdef DMTLine_isdelete
	for(int i=0; i<_gblocks; i++){allsize+=_data[i].databls*t;}
#endif
	allsize+=DMTLine_dsz*_gblocks;
	//printf("MTLine: add %d blocks, otn: %f / %f = %f , Allsize: %d \r\n", nbls, prs, prb, prs/prb, allsize);
#endif
	UnLock(); return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Sunc(){ return ; // ??? 
if(!_sunc && !_cursors){
for(unsigned int i=0; i<_gblocks; i++){
if(_data[i].databls==0
#ifdef DMTLine_isdelete
 || _data[i].databls==_data[i].datas && _data[i].databls==_data[i].datadelbls
#endif
){_data[i].~MTLineD();
_data=(MTLineD*)DeleteVData(_data, _gblocks*DMTLine_dsz, i*DMTLine_dsz, DMTLine_dsz);
 }}
}}

/////////////////////////////////////////////////////////////////////////////////////////////////////
D_MTLine* Insert(){ _sz++;
	if(_data[_gbfi].datas>=_data[_gbfi].databls) { Lock();
#if DMTLine_repair
#if DMTLine_isdelete
if(!(_gbfi+1<_gblocks)){
for(int i=0; i<_gblocks; i++){
if(_data[i].datadelbls){
for(int k=0; k<_data[i].databls; k++){
if(!_data[i].datadel[k]){memset(&_data[i].data[k], 0, DMTLine_dsz);
_data[i].datadel.set(k,1); _data[i].datadelbls--; UnLock();
eturn Ret(i, k); 
 }} UnLock(); return 0;
}} UnLock(); return 0;}
#endif
#endif
NewData();} Lock();
#ifdef DMTLine_constructor
	D_MTLine*dl=Ret(_gbfi, _data[_gbfi].datas++);
	UnLock();
	return new(dl)D_MTLine;
#else
	UnLock();
	return Ret(_gbfi, _data[_gbfi].datas++);
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
inline D_MTLine *Ret(unsigned int bl, unsigned int num){
#ifdef DMTLine_vtype
	return ((VLLine*)&_data[bl].data[num])->v;
#else
	return &_data[bl].data[num];
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//inline int NewGetData(){_dblock=0; _dnum=0; return 0;}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetData(DMTLine&dlm){MTLineLock l_(this,0); //int &block, int &num
nb:	if( _data[dlm._dblock].datas>dlm._dnum) 
#ifdef DConnectsLine_isdelete
		if(!_data[dlm._dblock].datadel[dlm._dnum]) {dlm._dnum++; goto nb;} else 
#endif
	{dlm=Ret(dlm._dblock,dlm._dnum++); return 1;}
	dlm._dblock++; dlm._dnum=0;
	if(dlm._dblock>=_gblocks) {dlm=(D_MTLine*)0; return 0;} goto nb;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetPData(DMTLine&dlm){MTLineLock l_(this,0); //int &block, int &num
nb:	if(dlm._dnum>=0)
#ifdef DConnectsLine_isdelete
	if(!_data[dlm._dblock].datadel[dlm._dnum]) {dlm._dnum--; goto nb;} else 
#endif
	{dlm=Ret(dlm._dblock,dlm._dnum--); return 1;}
	dlm._dblock--; dlm._dnum=_data[dlm._dblock].datas-1;
	if(dlm._dblock<0) {dlm=(D_MTLine*)0; return 0;} goto nb;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetEData(DMTLine&dlm){MTLineLock l_(this,0); //int &block, int &num
dlm._dblock=_gblocks-1;	dlm._dnum=_data[dlm._dblock].datas-1;
return GetPData(dlm);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPos(D_MTLine*dl, DMTLine&dlm){MTLineLock l_(this,0);
for(unsigned int i=0; i<_gblocks; i++){
if(dl>_data[i].data && dl<_data[i].data+_data[i].databls){
 dlm=dl; dlm._dblock=i; dlm._dnum=dl-_data[i].data; RUnLock(); return 1;
}}dlm.UnLock(); dlm.New(); return 0;
}

#if DMTLine_mmatrix
inline bool GetData(D_MTLine*&dlm){MTLineLock l_(this,0);//int &block, int &num
if(!dlm) dlm=_a; else dlm=dlm->_n;
return dlm;
}

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////
int DeleteData(D_MTLine*dl){
	DMTLine dlm(this); if(GetPos(dl, dlm)) return DeleteData(dl, dlm._dblock, dlm._dnum);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int DeleteData(DMTLine&dlm){Lock(); 
	int block=dlm._dblock; int num=dlm._dnum; D_MTLine*dta=dlm; // add foi
		
	while(block>=0){
		if(Ret(block,num)==dta){UnLock(); return DeleteData(dta, block, num);}
		num--; if(num<0) {block--; num=_data[block].datas-1;}
	}

	block=dlm._dblock, num=dlm._dnum;
	while(block<_gblocks){
		if(Ret(block,num)==dta){UnLock(); return DeleteData(dta, block, num);}
		num++; if(num>=_data[block].datas) {block++; num=0;}
	}
	UnLock(); return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
inline int DeleteData(D_MTLine*dta, int &_dblock, int &_dnum){MTLineLock l_(this); _sz--; _sunc=1;
#ifdef DMTLine_destructor
	_data[_dblock].data[_dnum].~D_MTLine();
#endif
#ifdef DMTLine_isdelete
	_data[_dblock].datadel.set(_dnum,0); _data[_dblock].datadelbls++;
#else
	return RealDelete(dta, _dblock, _dnum);
#endif
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
inline int RealDelete(D_MTLine*dta, int &block, int &num){
	_data[block].data=(D_MTLine*)DeleteVData(_data[block].data, _data[block].databls*DMTLine_dsz, num*DMTLine_dsz, DMTLine_dsz);
	_data[block].databls--; _data[block].datas--;
	// //if(block==_dblock && _dnum>num)_dnum--;
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
int Clear(int e=1){
	for(unsigned int i=0; i<_gblocks; i++){
#ifdef DMTLine_destructor
	for(int ii=0; ii<_data[i].datas; ii++){
#ifdef DMTLine_isdelete
	if(_data[i].datadel[ii])
#endif
	_data[i].data[ii].~D_MTLine();
	}
#endif
	DelBuffer(_data[i].data);
#ifdef DMTLine_isdelete
	DelBuffer(_data[i].datadel);
#endif
	}
#ifdef DMTLine_isdelete
	//DeleteBuffer(_delblocks); DeleteBuffer(_datadel);
#endif
	_deln(_data);
	if(e) Init(); 
return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
	~MTLine(){Clear(0);}
};

void DMTLine::Lock(MTLine*lm){if(lmatrix) {lmatrix->_cursors--; if(lmatrix->_cursors==0) lmatrix->Sunc();} lmatrix=lm; dmatrix=0; if(lmatrix) {lmatrix->_cursors++; _dblock=0; _dnum=0;}}
void DMTLine::UnLock(){ if(lmatrix) {lmatrix->_cursors--; if(lmatrix->_cursors==0) lmatrix->Sunc();} lmatrix=0; dmatrix=0;}
inline bool DMTLine::GetData(){return lmatrix->GetData(*this);}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW LIVE MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class LLine; class D_LLine; class VDLLine; class LLineD; class DLLine; // your class Index // DDS

// Defines 
#define DLLine_sz sizeof(D_LLine) // Data matrix size
#define DLLine_gsz sizeof(LLine) // Global data size
#define DLLine_dsz sizeof(LLineD) // Matrix Data size

 #define DLLine_isdelete 1 // + 8 byte 

 #define DLLine_destructor 1 
#define DLLine_min 10
#define DLLine_max 65535





class D_LLine{public: // data class // add your data

MString s;
};

//class VDLLine{ public: D_LLine *v; }; // virtual class // add your data

class LLineD{ public:
D_LLine*data;
unsigned short datas;
unsigned short databls;
#ifdef DLLine_isdelete
mbool datadel;
unsigned short datadelbls;
#endif
};

class DLLine{
public:
LLine*lmatrix; D_LLine*dmatrix;
int _dblock; int _dnum;

DLLine(LLine&lm){lmatrix=0; Lock(&lm);} DLLine(LLine*lm){lmatrix=0; Lock(lm);}// DLLine(void*lm){}
DLLine& operator=(int lm){dmatrix=0; return *this;}
~DLLine(){UnLock();}
void operator=(DLLine&dlm){ Lock(dlm.lmatrix); memcpy(this, &dlm, sizeof(DLLine)); }
//void operator=(LLine*lm){Lock(lm);} void operator=(LLine&lm){Lock(&lm);}
void operator=(D_LLine*dm){dmatrix=dm;}
inline D_LLine* operator->()const{return dmatrix;} operator D_LLine* (){return dmatrix;}

inline void New(){_dblock=0; _dnum=0;} void Lock(LLine*lm); void UnLock(); inline bool GetData();
};

class LLine{
protected:
#ifdef DLLine_extrime
public:
#endif
LLineD*_data;
unsigned int _gblocks; /* global blocks size*/  unsigned int _gbfi;		// global block for insert

unsigned int _sz;
//unsigned int _dblock; unsigned short _dnum;
unsigned short _mode;
public: unsigned short _cursors; char _sunc;
// special //unsigned int _ft; // fasttime //unsigned int _st; // smalltime

// isuni : 1 - full wr, 2 - wr only; isruni: 1- full ro

public:
/////////////////////////////////////////////////////////////////////////////////////////////////////
LLine(unsigned short mode=0){Init(); if(mode) _mode=mode; }
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Lock(){} void UnLock(){} void RLock(){} void RUnLock(){}
/////////////////////////////////////////////////////////////////////////////////////////////////////
class LLineLock{ bool locktype; LLine * lm;
public:
LLineLock(LLine*lm, int full=1){}
~LLineLock(){ } }; /////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Init(){ memset(this, 0, DLLine_gsz); _mode=50; NewData();  }
inline unsigned int size()const{return _sz;}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void SetMode(int m){_mode=m;}

DLLine Find(MString val){LLineLock l_(this, 2); DLLine dl(this); while(GetData(dl)){ if(dl->s==val) return dl; } return 0; }
MString Take(){LLineLock l_(this, 2); DLLine dl(this); if(GetData(dl)){ return dl->s; } return 0;}
MString DTake(){LLineLock l_(this, 2); DLLine dl(this); if(GetData(dl)){ MString ret=dl->s; DeleteData(dl); return ret; } return "";}

/////////////////////////////////////////////////////////////////////////////////////////////////////
int NewData(){
	if(_gbfi+1<_gblocks) {_gbfi++; return 1;} Lock();
	_data=(LLineD*)InsertVData(_data, DLLine_dsz*_gblocks, _gblocks*DLLine_dsz, DLLine_dsz, 1);
	int nbls;
	_gblocks++; nbls=_sz*_mode/100; if(nbls<DLLine_dsz*2/DLLine_sz) nbls=DLLine_dsz*2/DLLine_sz;
	if(nbls<DLLine_min) nbls=DLLine_min; else if(nbls>DLLine_max) nbls=DLLine_max;
	_gbfi=_gblocks-1; //_sz+=nbls;
	_data[_gbfi].data=(D_LLine*)GetBuffer(nbls*DLLine_sz);
	_data[_gbfi].databls=nbls; _data[_gbfi].datas=0;
#ifdef DLLine_isdelete
	int t=(nbls)/8+((nbls)%8>0);
	_data[_gbfi].datadel=(bool*)GetBuffer(t);
	memset(_data[_gbfi].datadel, 255, t);
#endif

#ifdef LLineDebug
	float prs=_sz*DLLine_sz; float prb=_gblocks*DLLine_dsz+DLLine_gsz+nbls*DLLine_sz;
	unsigned int allsize=0; for(int i=0; i<_gblocks; i++){allsize+=_data[i].databls*DLLine_sz;}
#ifdef DLLine_isdelete
	for(int i=0; i<_gblocks; i++){allsize+=_data[i].databls*t;}
#endif
	allsize+=DLLine_dsz*_gblocks;
	//printf("LLine: add %d blocks, otn: %f / %f = %f , Allsize: %d \r\n", nbls, prs, prb, prs/prb, allsize);
#endif
	UnLock(); return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void Sunc(){ return ; // ??? 
if(!_sunc && !_cursors){
for(unsigned int i=0; i<_gblocks; i++){
if(_data[i].databls==0
#ifdef DLLine_isdelete
 || _data[i].databls==_data[i].datas && _data[i].databls==_data[i].datadelbls
#endif
){_data[i].~LLineD();
_data=(LLineD*)DeleteVData(_data, _gblocks*DLLine_dsz, i*DLLine_dsz, DLLine_dsz);
 }}
}}

/////////////////////////////////////////////////////////////////////////////////////////////////////
D_LLine* Insert(){ _sz++;
	if(_data[_gbfi].datas>=_data[_gbfi].databls) { Lock();
#if DLLine_repair
#if DLLine_isdelete
if(!(_gbfi+1<_gblocks)){
for(int i=0; i<_gblocks; i++){
if(_data[i].datadelbls){
for(int k=0; k<_data[i].databls; k++){
if(!_data[i].datadel[k]){memset(&_data[i].data[k], 0, DLLine_dsz);
_data[i].datadel.set(k,1); _data[i].datadelbls--; UnLock();
eturn Ret(i, k); 
 }} UnLock(); return 0;
}} UnLock(); return 0;}
#endif
#endif
NewData();} Lock();
#ifdef DLLine_constructor
	D_LLine*dl=Ret(_gbfi, _data[_gbfi].datas++);
	UnLock();
	return new(dl)D_LLine;
#else
	UnLock();
	return Ret(_gbfi, _data[_gbfi].datas++);
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
inline D_LLine *Ret(unsigned int bl, unsigned int num){
#ifdef DLLine_vtype
	return ((VLLine*)&_data[bl].data[num])->v;
#else
	return &_data[bl].data[num];
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//inline int NewGetData(){_dblock=0; _dnum=0; return 0;}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetData(DLLine&dlm){LLineLock l_(this,0); //int &block, int &num
nb:	if( _data[dlm._dblock].datas>dlm._dnum) 
#ifdef DConnectsLine_isdelete
		if(!_data[dlm._dblock].datadel[dlm._dnum]) {dlm._dnum++; goto nb;} else 
#endif
	{dlm=Ret(dlm._dblock,dlm._dnum++); return 1;}
	dlm._dblock++; dlm._dnum=0;
	if(dlm._dblock>=_gblocks) {dlm=(D_LLine*)0; return 0;} goto nb;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetPData(DLLine&dlm){LLineLock l_(this,0); //int &block, int &num
nb:	if(dlm._dnum>=0)
#ifdef DConnectsLine_isdelete
	if(!_data[dlm._dblock].datadel[dlm._dnum]) {dlm._dnum--; goto nb;} else 
#endif
	{dlm=Ret(dlm._dblock,dlm._dnum--); return 1;}
	dlm._dblock--; dlm._dnum=_data[dlm._dblock].datas-1;
	if(dlm._dblock<0) {dlm=(D_LLine*)0; return 0;} goto nb;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool GetEData(DLLine&dlm){LLineLock l_(this,0); //int &block, int &num
dlm._dblock=_gblocks-1;	dlm._dnum=_data[dlm._dblock].datas-1;
return GetPData(dlm);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int GetPos(D_LLine*dl, DLLine&dlm){LLineLock l_(this,0);
for(unsigned int i=0; i<_gblocks; i++){
if(dl>_data[i].data && dl<_data[i].data+_data[i].databls){
 dlm=dl; dlm._dblock=i; dlm._dnum=dl-_data[i].data; RUnLock(); return 1;
}}dlm.UnLock(); dlm.New(); return 0;
}

#if DLLine_mmatrix
inline bool GetData(D_LLine*&dlm){LLineLock l_(this,0);//int &block, int &num
if(!dlm) dlm=_a; else dlm=dlm->_n;
return dlm;
}

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////
int DeleteData(D_LLine*dl){
	DLLine dlm(this); if(GetPos(dl, dlm)) return DeleteData(dl, dlm._dblock, dlm._dnum);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
int DeleteData(DLLine&dlm){Lock(); 
	int block=dlm._dblock; int num=dlm._dnum; D_LLine*dta=dlm; // add foi
		
	while(block>=0){
		if(Ret(block,num)==dta){UnLock(); return DeleteData(dta, block, num);}
		num--; if(num<0) {block--; num=_data[block].datas-1;}
	}

	block=dlm._dblock, num=dlm._dnum;
	while(block<_gblocks){
		if(Ret(block,num)==dta){UnLock(); return DeleteData(dta, block, num);}
		num++; if(num>=_data[block].datas) {block++; num=0;}
	}
	UnLock(); return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
inline int DeleteData(D_LLine*dta, int &_dblock, int &_dnum){LLineLock l_(this); _sz--; _sunc=1;
#ifdef DLLine_destructor
	_data[_dblock].data[_dnum].~D_LLine();
#endif
#ifdef DLLine_isdelete
	_data[_dblock].datadel.set(_dnum,0); _data[_dblock].datadelbls++;
#else
	return RealDelete(dta, _dblock, _dnum);
#endif
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
inline int RealDelete(D_LLine*dta, int &block, int &num){
	_data[block].data=(D_LLine*)DeleteVData(_data[block].data, _data[block].databls*DLLine_dsz, num*DLLine_dsz, DLLine_dsz);
	_data[block].databls--; _data[block].datas--;
	// //if(block==_dblock && _dnum>num)_dnum--;
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
int Clear(int e=1){
	for(unsigned int i=0; i<_gblocks; i++){
#ifdef DLLine_destructor
	for(int ii=0; ii<_data[i].datas; ii++){
#ifdef DLLine_isdelete
	if(_data[i].datadel[ii])
#endif
	_data[i].data[ii].~D_LLine();
	}
#endif
	DelBuffer(_data[i].data);
#ifdef DLLine_isdelete
	DelBuffer(_data[i].datadel);
#endif
	}
#ifdef DLLine_isdelete
	//DeleteBuffer(_delblocks); DeleteBuffer(_datadel);
#endif
	_deln(_data);
	if(e) Init(); 
return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
	~LLine(){Clear(0);}
};

void DLLine::Lock(LLine*lm){if(lmatrix) {lmatrix->_cursors--; if(lmatrix->_cursors==0) lmatrix->Sunc();} lmatrix=lm; dmatrix=0; if(lmatrix) {lmatrix->_cursors++; _dblock=0; _dnum=0;}}
void DLLine::UnLock(){ if(lmatrix) {lmatrix->_cursors--; if(lmatrix->_cursors==0) lmatrix->Sunc();} lmatrix=0; dmatrix=0;}
inline bool DLLine::GetData(){return lmatrix->GetData(*this);}
/////////////////////////////////////////////////////////////////////////////////////////////////////// END TEMPLATE 
// Do Not Write To This File, All Data Well Be Lost!
// ##EMD5:0a67e022602b6d2706ca79ba8b874f9a

