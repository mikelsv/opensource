// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:46df2008f34215b25da9cbff6d68f154 : 2012.11.30 14:28:28


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW UNLIMITED MATRIX///////////////////////////////////////////////////////////////////////////////////////////////////////////
class XMLLine; // your class Index

// Defines 
//#define DXMLLine_sz sizeof(DData) // General data size



#ifndef XMLLine_minmem
#define XMLLine_minmem	1024
#endif
#define XMLLine_minel	16

//class XMLLine_um{ XMLel*n; unsigned int sz; XMLel data[0]; };

class XMLLine{// Unlimited Matrix
protected:
unsigned int allsize, usedsize, bufsize, blocks;
UMX_data *data, *edata, *cdata; unsigned int cblockfn, esz;
const XMLel nullel;
bool isdel;
public:
XMLLine(unsigned int buf=XMLLine_minmem) : nullel(){ Init(); bufsize=buf; }
~XMLLine(){Clear();}



void Init(){ memset(this, 0, sizeof(XMLLine)); }

void NewBlockSz(unsigned int buf){ if(buf>=sizeof(XMLel)) bufsize=buf; }
void NewBlockESz(unsigned int buf){ if(buf>0) bufsize=buf*sizeof(XMLel); }
unsigned int size()const {return usedsize;}

//const XMLLine& operator+=(XMLel *s){ return add(s, strlen(s)/sizeof(XMLel));}
//const XMLLine& operator+=(MString &s){ return add(s, s);}

const XMLel& operator[](const unsigned int e){ return getelement(e); }
const XMLel& operator[](const int e){ return getelement(e); }

XMLel& el(const unsigned int e){ return getelement(e); }

XMLel& getelement(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz) return *((XMLel*)(&cdata->data)+(e-cblockfn));
		cblockfn+=cdata->sz; cdata=cdata->n;
	}
return (XMLel&)nullel;
}

inline const unsigned int add(const XMLel &s){ return add(&s, sizeof(XMLel)); }
//inline XMLel& inserta(){return (XMLel&)*insert();}

inline const unsigned int add(const XMLel *s, const unsigned int sz=1){ if(!sz) return -1;
	if(!bufsize) bufsize=XMLLine_minmem; unsigned int ret=usedsize;
	unsigned int frs=allsize-usedsize, ws;
	if(frs && edata){ ws=sz; if(ws>frs) ws=frs;
		if(s){ memcpy((XMLel*)&edata->data+esz-frs, s, ws*sizeof(XMLel)); }		
		usedsize+=ws;
		if(ws==sz) return ret;
	}
	ws=bufsize/sizeof(XMLel); if(ws<1) ws=1; if(sz>ws) ws=sz;
	UMX_data *ndata=(UMX_data*)GetBuffer(ws*sizeof(XMLel)+UMSZ+(isdel ? (sizeof(mbool)+ws/8 + (ws%8>0 ? 1 : 0)) : 0)); blocks++;
	if(isdel) {*((mbool*)((char*)ndata)+ws*sizeof(XMLel)+UMSZ)=(bool*)(((char*)ndata)+ws*sizeof(XMLel)+UMSZ+sizeof(mbool));}
	allsize+=ws; usedsize+=sz-frs; //ws=bufsize; if(sz-frs>ws) ws=sz-frs;
	ndata->sz=ws; esz=ws; //*(unsigned int*)((char*)ndata)=ws; esz=ws;// ndata+=UMSZ;
	if(edata){ edata->n=ndata; }
	edata=ndata; if(!data) data=edata;
	if(s) memcpy(edata->data, s+frs, (sz-frs)*sizeof(XMLel));
return ret;
}

inline XMLel& inserta(){ return (XMLel&)getelement(insert()); }
inline XMLel* vinsert(){ return (XMLel*)&getelement(insert()); }
inline XMLel& repaira(){ return (XMLel&)getelement(repair()); }
inline XMLel* vrepair(){ return (XMLel*)&getelement(repair()); }

inline unsigned int insert(){ char tm[sizeof(XMLel)]; memset(&tm, 0, sizeof(XMLel)); unsigned int ed=add((XMLel*)&tm, 1); XMLel*e=&getelement(ed); memset(e, 0, sizeof(*e)); return ed; }

bool del(unsigned int e){
if(!cdata || e<cblockfn){ cdata=data; cblockfn=0; }
	while(cdata){
		if(e<cblockfn+cdata->sz){
				#ifdef DXMLLine_constructor
			((XMLel*)cdata.data+(e-cblockfn))->~XMLel();
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
#ifdef DXMLLine_constructor
		new((XMLel*)cdata->data+i)XMLel;
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
			 return ((mbool*)((XMLel*)cdata->data+cdata->sz))->get(e-cblockfn);
		}
		cdata=cdata->n; cblockfn+=cdata->sz;
	}
return 0;
}

unsigned int getpos(const XMLel *pos){
	cdata=data; cblockfn=0;
	while(cdata){
		if((XMLel*)&cdata->data<=pos && pos<(XMLel*)&cdata->data+cdata->sz){
			 return cblockfn+(pos-(XMLel*)(&cdata->data));
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
unsigned int GetFreeBuffer(XMLel**data){
	if(allsize==usedsize){ add((XMLel*)&nullel, 1); usedsize--; }
	unsigned int fs=allsize-usedsize;
	*data=(XMLel*)&edata->data+esz-fs;
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
		memcpy(ret.pos(pos), &dt->data, sz*sizeof(XMLel)); pos+=sz;
		dt=dt->n;	
	}
	return ret;
}

XMLel* pos(){ if(!data) return 0; return (XMLel*)data->data; }
operator XMLel*(){ if(!data) return 0; return (XMLel*)data->data; }
XMLel* nomov(){ return oneline(); }

XMLel* oneline(bool delfree=1){
	if(!data) return 0; if(data==edata) return (XMLel*)data->data;
	if(delfree) esz=usedsize; else esz=allsize;
	UMX_data *ndata=(UMX_data*)GetBuffer(esz*sizeof(XMLel)+UMSZ);
	//*(unsigned int*)((char*)ndata)=esz; ndata+=UMSZ;
	ndata->sz=esz;
	unsigned int pos=0, sz;

	UMX_data *dt=data, *dd;
	while(dt){
		if(dt->sz+pos<usedsize) sz=dt->sz; else{ sz=usedsize-pos; }
		memcpy(ndata->data+pos*sizeof(XMLel), dt->data, sz*sizeof(XMLel)); pos+=sz;
//memcpy(&dd, (char*)(dt-4), 4);
//*(long*)(char*)&dt=*(long*)((char*)dt-UMCSZ);
 		dd=dt; dt=dt->n; DelBuffer(dd);
	}
	data=ndata; edata=data; cdata=0; blocks=1; allsize=usedsize;
	return (XMLel*)data->data;
}

void Clear(){ if(!data) return ;
	UMX_data *dt=data, *dd;
	unsigned int pos=0, dsz, msz;

	while(dt){
		dsz=dt->sz; msz=dsz; if(dsz+pos>usedsize) msz=usedsize-pos;
#ifdef DXMLLine_destructor
		for(unsigned int i=0; i<msz; i++){
			((XMLel*)dt->data+i)->~XMLel();
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
// ##EMD5:46df2008f34215b25da9cbff6d68f154

