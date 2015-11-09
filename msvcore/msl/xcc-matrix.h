#ifdef XTEMPLATE
ACT: TemplateConstructor
UID: 687906dc0bee68391f215630f101c0b1:c7f460c0bc7cd8d687c97fa5ba6b97a6:7a91689dba2f5b92045584f53beb3778
KEY:
file ../IMatrix.msvtcd
IMatrix XCCLEXPLine
IMData XCCLEXP
InputData <<<EOF
EOF;
IMATRIX_CONSTR
IMATRIX_DESTR
IMATRIX_ISDEL
#endif

#define XCCLEXPLine_minel S1K

#ifdef XTEMPLATE
ACT: TemplateConstructor
UID: 687906dc0bee68391f215630f101c0b1:230e96067738219fa66996608de7b1c3:ada9eb09290b505eea6ffdd1dd451e46
KEY:
file ../IMatrix.msvtcd
IMatrix XCCDefLine
IMData XCCDef
InputData <<<EOF
EOF;
IMATRIX_CONSTR
IMATRIX_DESTR
IMATRIX_ISDEL
#endif

#define XCCDefLine_minel 128

#ifdef XTEMPLATE
ACT: TemplateConstructor
UID: 687906dc0bee68391f215630f101c0b1:a70c11b6fddabebe9a8b6796f1d19ece:2f797d8f699bf80af71034fb52debd88
KEY:
file ../IMatrix.msvtcd
IMatrix XCCDefVirtLine
IMData XCCDefVirt
InputData <<<EOF
EOF;
IMATRIX_CONSTR
IMATRIX_DESTR
IMATRIX_ISDEL
#endif

#define XCCDefVirtLine_minel 64

#ifdef XTEMPLATE
ACT: TemplateConstructor
UID: 687906dc0bee68391f215630f101c0b1:9454b3720ac74f51b5d064d568c736f5:afc75e95ea3c52f11a150c100e1fbb43
KEY:
file ../IMatrix.msvtcd
IMatrix XCCDefInLine
IMData XCCDefIn
InputData <<<EOF
inline XCCDefIn& p(){ return data[size-1]; }
EOF;
IMATRIX_CONSTR
IMATRIX_DESTR
IMATRIX_ISDEL
#endif

#ifdef XTEMPLATE
ACT: TemplateConstructor
UID: 48bb978b67ece9d1a3084e8b81f333a2:c39b29ead27123a48524be5aa63d7892:579e69e894e5fda5c19df7904b364c1c
KEY:
file ../UMatrix.msvtcd
UMatrix XCCFileLine
UMData XCCFile
InputData <<<EOF
EOF;
UMATRIX_CONSTR
UMATRIX_DESTR
UMATRIX_ISDEL
#endif

void* GetBuffer(int sz){
	return malloc(sz);
}

void* AddVData(void *p, int sz, int asz){
	void *n = malloc(sz + asz);
	memcpy(n, p, sz);
	free(p);
	return n;
}

void DelBuffer(void *p){
	return free(p);
}

#define XTEMPLATE_RET_SOF
// XTemplate v.0.2.0.1 (03.08.2011 16:35) Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##MD5: 687906dc0bee68391f215630f101c0b1:c7f460c0bc7cd8d687c97fa5ba6b97a6:7a91689dba2f5b92045584f53beb3778 : 11.08.06 14:35:20


// NEW IDEAL MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XCCLEXPLine{ // NEW Ideal Matrix
#ifndef XCCLEXPLine_minel
#define XCCLEXPLine_minel 16
#endif
unsigned int allsize;
public:
XCCLEXP *data;
unsigned int size;

XCCLEXPLine(){ data=0; size=0; allsize=0; }
~XCCLEXPLine(){ Clear(); }

bool Add(unsigned int add=XCCLEXPLine_minel){ if(!add) return 1;
data=(XCCLEXP*)AddVData(data, allsize*sizeof(XCCLEXP), add*sizeof(XCCLEXP));
 allsize+=add;
return 1;
}
inline bool A(){ if(size==allsize) return Add(); return 0; }
inline bool A(int sz){ if(size==allsize) return Add(sz); return 0; }

inline XCCLEXP& operator [](unsigned int i){ return data[i]; }
inline unsigned int asize(){ return allsize; }
/// Input Data

///
inline XCCLEXP& n(){ return data[size]; }
void Clear(){
	 DelBuffer(data); data=0; size=0; allsize=0;
 }
};// Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##EMD5:687906dc0bee68391f215630f101c0b1:c7f460c0bc7cd8d687c97fa5ba6b97a6:7a91689dba2f5b92045584f53beb3778
// XTemplate v.0.2.0.1 (03.08.2011 16:35) Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##MD5: 687906dc0bee68391f215630f101c0b1:230e96067738219fa66996608de7b1c3:ada9eb09290b505eea6ffdd1dd451e46 : 11.08.06 14:35:20


// NEW IDEAL MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XCCDefLine{ // NEW Ideal Matrix
#ifndef XCCDefLine_minel
#define XCCDefLine_minel 16
#endif
unsigned int allsize;
public:
XCCDef *data;
unsigned int size;

XCCDefLine(){ data=0; size=0; allsize=0; }
~XCCDefLine(){ Clear(); }

bool Add(unsigned int add=XCCDefLine_minel){ if(!add) return 1;
data=(XCCDef*)AddVData(data, allsize*sizeof(XCCDef), add*sizeof(XCCDef));
 allsize+=add;
return 1;
}
inline bool A(){ if(size==allsize) return Add(); return 0; }
inline bool A(int sz){ if(size==allsize) return Add(sz); return 0; }

inline XCCDef& operator [](unsigned int i){ return data[i]; }
inline unsigned int asize(){ return allsize; }
/// Input Data

///
inline XCCDef& n(){ return data[size]; }
void Clear(){
	 DelBuffer(data); data=0; size=0; allsize=0;
 }
};// Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##EMD5:687906dc0bee68391f215630f101c0b1:230e96067738219fa66996608de7b1c3:ada9eb09290b505eea6ffdd1dd451e46
// XTemplate v.0.2.0.1 (03.08.2011 16:35) Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##MD5: 687906dc0bee68391f215630f101c0b1:a70c11b6fddabebe9a8b6796f1d19ece:2f797d8f699bf80af71034fb52debd88 : 11.08.08 09:07:26


// NEW IDEAL MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XCCDefVirtLine{ // NEW Ideal Matrix
#ifndef XCCDefVirtLine_minel
#define XCCDefVirtLine_minel 16
#endif
unsigned int allsize;
public:
XCCDefVirt *data;
unsigned int size;

XCCDefVirtLine(){ data=0; size=0; allsize=0; }
~XCCDefVirtLine(){ Clear(); }

bool Add(unsigned int add=XCCDefVirtLine_minel){ if(!add) return 1;
data=(XCCDefVirt*)AddVData(data, allsize*sizeof(XCCDefVirt), add*sizeof(XCCDefVirt));
 allsize+=add;
return 1;
}
inline bool A(){ if(size==allsize) return Add(); return 0; }
inline bool A(int sz){ if(size==allsize) return Add(sz); return 0; }

inline XCCDefVirt& operator [](unsigned int i){ return data[i]; }
inline unsigned int asize(){ return allsize; }
/// Input Data

///
inline XCCDefVirt& n(){ return data[size]; }
void Clear(){
	 DelBuffer(data); data=0; size=0; allsize=0;
 }
};// Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##EMD5:687906dc0bee68391f215630f101c0b1:a70c11b6fddabebe9a8b6796f1d19ece:2f797d8f699bf80af71034fb52debd88
// XTemplate v.0.2.0.1 (03.08.2011 16:35) Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##MD5: 687906dc0bee68391f215630f101c0b1:9454b3720ac74f51b5d064d568c736f5:afc75e95ea3c52f11a150c100e1fbb43 : 11.08.06 14:35:20


// NEW IDEAL MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XCCDefInLine{ // NEW Ideal Matrix
#ifndef XCCDefInLine_minel
#define XCCDefInLine_minel 16
#endif
unsigned int allsize;
public:
XCCDefIn *data;
unsigned int size;

XCCDefInLine(){ data=0; size=0; allsize=0; }
~XCCDefInLine(){ Clear(); }

bool Add(unsigned int add=XCCDefInLine_minel){ if(!add) return 1;
data=(XCCDefIn*)AddVData(data, allsize*sizeof(XCCDefIn), add*sizeof(XCCDefIn));
 allsize+=add;
return 1;
}
inline bool A(){ if(size==allsize) return Add(); return 0; }
inline bool A(int sz){ if(size==allsize) return Add(sz); return 0; }

inline XCCDefIn& operator [](unsigned int i){ return data[i]; }
inline unsigned int asize(){ return allsize; }
/// Input Data

inline XCCDefIn& p(){ return data[size-1]; }
///
inline XCCDefIn& n(){ return data[size]; }
void Clear(){
	 DelBuffer(data); data=0; size=0; allsize=0;
 }
};// Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##EMD5:687906dc0bee68391f215630f101c0b1:9454b3720ac74f51b5d064d568c736f5:afc75e95ea3c52f11a150c100e1fbb43
// XTemplate v.0.2.0.1 (03.08.2011 16:35) Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##MD5: 48bb978b67ece9d1a3084e8b81f333a2:c39b29ead27123a48524be5aa63d7892:579e69e894e5fda5c19df7904b364c1c : 11.08.06 14:35:20


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XCCFileLine; // your class Index

// Defines 
//#define DXCCFileLine_sz sizeof(DData) // General data size



#define XCCFileLine_minmem	1024
#define XCCFileLine_minel	16

//class XCCFileLine_um{ XCCFile*n; unsigned int sz; XCCFile data[0]; };

class XCCFileLine{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
XCCFile *data, *edata, *cdata; unsigned int cblockfn, esz;
const XCCFile nullel;
bool isdel;
public:
XCCFileLine(unsigned int buf=XCCFileLine_minmem) : nullel(){ Init(); bufsize=buf; }
~XCCFileLine(){Clear();}



void Init(){ memset(this, 0, sizeof(XCCFileLine));  }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(XCCFile)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(XCCFile); }
unsigned int size()const {return usedsize;}

//const XCCFileLine& operator+=(XCCFile *s){ return add(s, strlen(s)/sizeof(XCCFile));}
//const XCCFileLine& operator+=(MString &s){ return add(s, s);}

const XCCFile& operator[](const unsigned int e){ return getelement(e); }
const XCCFile& operator[](const int e){ return getelement(e); }

XCCFile& el(const unsigned int e){ return getelement(e); }

XCCFile& getelement(unsigned int e){
int s; if(!cdata || e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-UMSZ);
		if(e<cblockfn+s)  return cdata[e-cblockfn];
		//cdata=(XCCFile*)(char*)((cdata)-4);
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-UMCSZ); cblockfn+=s;
	}
return (XCCFile&)nullel;
}

inline const unsigned int add(const XCCFile &s){ return add(&s, sizeof(XCCFile)); }
//inline XCCFile& inserta(){return (XCCFile&)*insert();}

inline const unsigned int add(const XCCFile *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=XCCFileLine_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s) memcpy(edata+esz-frs, s, ws*sizeof(XCCFile)); usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(XCCFile); if(ws<1) ws=1; if(sz>ws) ws=sz;
	char *ndata=(char*)GetBuffer(ws*sizeof(XCCFile)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(XCCFile)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(XCCFile)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	*(unsigned int*)((char*)ndata)=ws; esz=ws; ndata+=UMSZ;
	if(edata){ *(unsigned int*)((char*)edata-UMCSZ)=*(unsigned int*)&ndata; }
	edata=(XCCFile*)((char*)ndata); if(!data) data=edata;
	if(s) memcpy(edata, s+frs, (sz-frs)*sizeof(XCCFile));
return ret;
}

inline XCCFile& inserta(){return (XCCFile&)getelement(insert()); }
inline XCCFile* vinsert(){ return (XCCFile*)&getelement(insert()); }
inline XCCFile& repaira(){return (XCCFile&)getelement(repair()); }
inline XCCFile* vrepair(){return (XCCFile*)&getelement(repair());}

inline unsigned int insert(){ char tm[sizeof(XCCFile)]; memset(&tm, 0, sizeof(XCCFile)); unsigned int ed=add((XCCFile*)&tm, 1); XCCFile*e=&getelement(ed); memset(e, 0, sizeof(*e)); new(e)XCCFile; return ed;}

bool del(unsigned int e){
int s; if(!cdata || e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-UMSZ);
		if(e<cblockfn+s){
				#ifdef DXCCFileLine_constructor
			cdata[e-cblockfn].~XCCFile();
				#endif
			 if(isdel) ((mbool*)(cdata+s))->set(e-cblockfn, 0);
			 return 1;
		}
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-UMCSZ); cblockfn+=s;
	}
return 0;
}

unsigned int repair(){ if(!isdel) return insert();
int s; mbool *mb; if(!cdata || 0<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-UMSZ);
		int i=0; mb=(mbool*)(cdata+s);

		for(i; i<s; i++){ if(i%8==0 && mb->isbyte(i/8)==255) {i+=8; continue;}
		if(!mb->get(i)){// if(cblockfn+i<usedsize) return insert();
		 mb->set(i, 1);
#ifdef DXCCFileLine_constructor
		new(cdata+i)XCCFile;
#endif
		return cblockfn+i;
	}}
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-UMCSZ); cblockfn+=s;
	}
 return insert();
}

bool is(unsigned int e){ if(!isdel) return 1;
int s; if(!cdata || e<cblockfn) {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-UMSZ);
		if(e<cblockfn+s){
			 return ((mbool*)(cdata+s))->get(e-cblockfn);
		}
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-UMCSZ); cblockfn+=s;
	}
return 0;
}

unsigned int getpos(const XCCFile *pos){
int s; {cdata=data; cblockfn=0;}
	while(cdata){
		s=*(unsigned int*)((char*)cdata-UMSZ);
		if(cdata<=pos && pos<cdata+s){
			 return cblockfn+(pos-cdata);
		}
		*(unsigned int*)(char*)&cdata=*(unsigned int*)((char*)cdata-UMCSZ); cblockfn+=s;
	}
return -1;
}

// SP
unsigned int GetFreeBuffer(XCCFile**data){
if(allsize==usedsize) {add((XCCFile*)&nullel, 1); usedsize--;}
unsigned int fs=allsize-usedsize;
*data=edata+esz-fs;
return fs;
}

void SetFreeBuffer(unsigned int add){usedsize+=esz-add; }

//const LString& operator=(char *s){Clear(); return add(s, strlen(s));}
//const LString& operator=(MString s){Clear(); return add(s, s);}

operator MString(){
	MString ret; ret.Reserv(usedsize*sizeof(char));
	unsigned int pos=0, sz, dsz;
XCCFile*dt=(XCCFile*)data;

if(data) while(dt){
dsz=*(unsigned int*)((char*)dt-UMSZ);
if(dsz+pos<ret.size()) sz=dsz;
else {sz=ret.size()-pos;}
memcpy(ret.pos(pos), dt, sz*sizeof(XCCFile)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-UMCSZ);
}
return ret;
}

XCCFile* pos(){return data;}
operator XCCFile*(){return data;}
XCCFile* nomov(){return oneline();}

XCCFile* oneline( bool delfree=1){ if(!data) return 0; if(data==edata) return data;
	if(delfree) esz=usedsize; else esz=allsize;
	char *ndata=(char*)GetBuffer(esz*sizeof(XCCFile)+UMSZ);
	*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	unsigned int pos=0, sz, dsz;
XCCFile*dt=data, *dd;
while(dt){
dd=(XCCFile*)((char*)dt-UMSZ); dsz=*(unsigned int*)((char*)dd);
if(dsz+pos<usedsize) sz=dsz; else {sz=usedsize-pos;}
memcpy(ndata+pos*sizeof(XCCFile), dt, sz*sizeof(XCCFile)); pos+=dsz;
//memcpy(&dd, (char*)(dt-4), 4);
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-UMCSZ);
DelBuffer(dd);
}
data=(XCCFile*)ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
return data;
}

void Clear(){ if(!data) return ;
XCCFile*dt=(XCCFile*)data, *dd;
#ifdef DXCCFileLine_destructor
unsigned int pos=0, dsz, msz;
while(dt){
dsz=*(unsigned int*)((char*)dt-UMSZ); msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
dd=(XCCFile*)((char*)dt-UMSZ); // ((char*)(dt))-8;
//dt=(char*)*(unsigned int*)((char*)dd+4); //(void*)((char*)dd+4);// (void*)dd+4;
for(unsigned int i=0; i<msz; i++){
dt[i].~XCCFile();
}
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-UMCSZ);
DelBuffer(dd);
}
#else if
while(dt){
dd=(XCCFile*)((char*)dt-UMSZ); // ((char*)(dt))-8;
//dt=(char*)*(unsigned int*)((char*)dd+4); //(void*)((char*)dd+4);// (void*)dd+4;
*(unsigned int*)(char*)&dt=*(unsigned int*)((char*)dt-UMCSZ);
DelBuffer(dd);
}
#endif
data=0; edata=0; cdata=0; allsize=0; usedsize=0; 
}
};// Do Not Write To This Block, All Data Well Be Rewrite On Update!
// ##EMD5:48bb978b67ece9d1a3084e8b81f333a2:c39b29ead27123a48524be5aa63d7892:579e69e894e5fda5c19df7904b364c1c
#define XTEMPLATE_RET_EOF