// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:68758ffd406fec9ee00f01c16a0d5420 : 2013.10.11 22:08:35


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XDataLine; // your class Index

// Defines 
//#define DXDataLine_sz sizeof(DData) // General data size



#ifndef XDataLine_minmem
#define XDataLine_minmem	1024
#endif
#define XDataLine_minel	16

//class XDataLine_um{ XDataEl*n; unsigned int sz; XDataEl data[0]; };

class XDataLine{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
UMX_data *data, *edata, *cdata; unsigned int cblockfn, esz;
const XDataEl nullel;
bool isdel;
public:
XDataLine(unsigned int buf=XDataLine_minmem) : nullel(){ Init(); bufsize=buf; }
~XDataLine(){Clear();}



void Init(){ memset(this, 0, sizeof(XDataLine)); }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(XDataEl)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(XDataEl); }
unsigned int size()const {return usedsize;}

//const XDataLine& operator+=(XDataEl *s){ return add(s, strlen(s)/sizeof(XDataEl));}
//const XDataLine& operator+=(MString &s){ return add(s, s);}

const XDataEl& operator[](const unsigned int e){ return getelement(e); }
const XDataEl& operator[](const int e){ return getelement(e); }

XDataEl& el(const unsigned int e){ return getelement(e); }

XDataEl& getelement(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz) return *((XDataEl*)(&cdata->data)+(e-cblockfn));
		cblockfn+=cdata->sz; cdata=cdata->n;
	}
return (XDataEl&)nullel;
}

inline const unsigned int add(const XDataEl &s){ return add(&s, sizeof(XDataEl)); }
//inline XDataEl& inserta(){return (XDataEl&)*insert();}

inline const unsigned int add(const XDataEl *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=XDataLine_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s){ memcpy((XDataEl*)&edata->data+esz-frs, s, ws*sizeof(XDataEl)); }		
		usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(XDataEl); if(ws<1) ws=1; if(sz>ws) ws=sz;
	UMX_data *ndata=(UMX_data*)GetBuffer(ws*sizeof(XDataEl)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(XDataEl)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(XDataEl)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	ndata->sz=ws; esz=ws; //*(unsigned int*)((char*)ndata)=ws; esz=ws;// ndata+=UMSZ;
	if(edata){ edata->n=ndata; }
	edata=ndata; if(!data) data=edata;
	if(s) memcpy(edata->data, s+frs, (sz-frs)*sizeof(XDataEl));
return ret;
}

inline XDataEl& inserta(){ return (XDataEl&)getelement(insert()); }
inline XDataEl* vinsert(){ return (XDataEl*)&getelement(insert()); }
inline XDataEl& repaira(){ return (XDataEl&)getelement(repair()); }
inline XDataEl* vrepair(){ return (XDataEl*)&getelement(repair()); }

inline unsigned int insert(){ char tm[sizeof(XDataEl)]; memset(&tm, 0, sizeof(XDataEl)); unsigned int ed=add((XDataEl*)&tm, 1); XDataEl*e=&getelement(ed); memset(e, 0, sizeof(*e)); return ed; }

bool del(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
				#ifdef DXDataLine_constructor
			((XDataEl*)cdata.data+(e-cblockfn))->~XDataEl();
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
#ifdef DXDataLine_constructor
		new((XDataEl*)cdata->data+i)XDataEl;
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
			 return ((mbool*)((XDataEl*)cdata->data+cdata->sz))->get(e-cblockfn);
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int getpos(const XDataEl *pos){
	cdata=data; cblockfn=0;
	while(cdata){
		if((XDataEl*)&cdata->data<=pos && pos<(XDataEl*)&cdata->data+cdata->sz){
			 return cblockfn+(pos-(XDataEl*)(&cdata->data));
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
unsigned int GetFreeBuffer(XDataEl**data){
	if(allsize==usedsize){ add((XDataEl*)&nullel, 1); usedsize--; }
	unsigned int fs=allsize-usedsize;
	*data=(XDataEl*)&edata->data+esz-fs;
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
		memcpy(ret.pos(pos), &dt->data, sz*sizeof(XDataEl)); pos+=sz;
		dt=dt->n;	
	}
	return ret;
}

XDataEl* pos(){ if(!data) return 0; return (XDataEl*)data->data; }
operator XDataEl*(){ if(!data) return 0; return (XDataEl*)data->data; }
XDataEl* nomov(){ return oneline(); }

XDataEl* oneline(bool delfree=1){
	if(!data) return 0; if(data==edata) return (XDataEl*)data->data;
	if(delfree) esz=usedsize; else esz=allsize;
	UMX_data *ndata=(UMX_data*)GetBuffer(esz*sizeof(XDataEl)+UMSZ);
	//*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	ndata->sz=esz;
	unsigned int pos=0, sz;

	UMX_data *dt=data, *dd;
	while(dt){
		if(dt->sz+pos<usedsize) sz=dt->sz; else{ sz=usedsize-pos; }
		memcpy(ndata->data+pos*sizeof(XDataEl), dt->data, sz*sizeof(XDataEl)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
//*(long*)(char*)&dt=*(long*)((char*)dt-UMCSZ);
 		dd=dt; dt=dt->n; DelBuffer(dd);
	}
	data=ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
	return (XDataEl*)data->data;
}

void Clear(){ if(!data) return ;
	UMX_data *dt=data, *dd;
	unsigned int pos=0, dsz, msz;

	while(dt){
		dsz=dt->sz; msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
#ifdef DXDataLine_destructor
		for(unsigned int i=0; i<msz; i++){
			((XDataEl*)dt->data+i)->~XDataEl();
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
// ##EMD5:68758ffd406fec9ee00f01c16a0d5420

