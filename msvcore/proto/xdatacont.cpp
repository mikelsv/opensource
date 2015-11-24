// Don't include! use #define USEMSV_XDATACONT 

#define USEMSV_XDATACONT_USED
//#define XMLIT

//#ifndef USEMSV_DATIXCONT_USED
//#error Please define: #define USEMSV_DATIXCONT
//#endif

class XMLel{ public:
VString key;
VString val;
#ifndef XMLIT // XML int type
XMLel *r, *p, *n, *a, *l; // special, prev, next, a(up), line(up).
#define XMLN	0
#else
short r, p, n, a, l;
#define XMLN	-1 // -1
#endif

#ifndef XMLIT // CFL int type
inline XMLel* get(XMLel *cf) { return cf; }
inline XMLel* set(XMLel *cf) { return cf; }
#else
inline XMLel * get(XMLel * cf){ return cf; }
inline unsigned int ps(int cf){ return cf; }
#endif

short type; // text, tag

	XMLel* FindX(XMLel*el, VString line){
		for(el; el; el=get(el->n)){
			if(el->key==line) return el;
		}
	return 0;
	}

	XMLel* Get(VString line){
		XMLel *el=this; if(!this) return 0;
		VString o, t; int l=0;

		while(el && line.is()){
			o=PartLineO(line, "."); t=PartLineO(line, ",");
			if(o.data && o.sz<t.sz){ o=PartLine(line, line, "."); l=0; } else{ o=PartLine(line, line, ","); l=1; }

			el=el->FindX(el, o);
			if(el && line) el= !l ? el->a : el->l;
			if(line.sz==1 && *line.data==':') return el;
		}
		return el;
	}

	VString GetVal(VString line){ XMLel *el=Get(line); if(el) return el->val; return VString(); }

	XMLel* fa(){ if(this) return a; return 0; }

/*
	XMLel * operator[](VString line){
		XMLel *el=this; if(!this) return 0;
		el=el->a;
		while(el && line.is()){
			VString o=PartLine(line, line, ".");
			el=el->FindX(el, o);
			if(el && line) el=el->a; if(line.sz==1 && *line.data==':') return el;
		}
		return el;
	}
	*/

};

#include "UMatrixXML.h"

class XMLCont{
public:
XMLLine conf;
//HLString data;
int l;
int sz;

#ifndef XMLIT // CFL int type
inline XMLel* get(XMLel *cf) { return cf; }
inline XMLel* set(XMLel *cf) { return cf; }
inline unsigned int ps(XMLel * cf){ return conf.getpos(cf); }
#else
inline XMLel * get(short cf){ if(cf<0 || conf.size()<=cf) return 0; return &conf.el(cf); }
inline XMLel * get(XMLel * cf){ return cf; }
inline short set(XMLel *cf){ return conf.getpos(cf); }
inline unsigned int ps(int cf){ return cf; }
#endif

XMLCont(){sz=0;}
// add deletelist

operator XMLel*(){ return &conf.el(0); }

void Release(){conf.Clear(); return ; }

// {"chat":{"s":63485400402818,"i":7898,"r":["m.63","a.17027"]},"n":{"n":1},"t":1349812896171,"u":1349810379282,"q":[143,141,138,74],"g":1944889,"r":2979}

int ReadJson(unsigned char*data, unsigned int size, bool noclear=0){ // return pos;
	if(!noclear) {conf.Clear(); sz=0; }

	XMLel *arr[S1K], *te=0, *ta=0, *tg=0; memset(arr, 0, sizeof(arr));// , *err[S1K] memset(err, 0, sizeof(arr));
	int /*l=0,*/ il=0; l=0; arr[l]=0; //&conf.el(0);

	//if(data && size && data[0]=='{' && data[size-1]=='}'){ data++; size--; }

	unsigned char*pos=data, *lpos=data, *to_=data+size/*, *t*/; int ld=0;
	unsigned char*line=data, *pline=data, *to=data+size; int cl=0, ll=0, q=0;//, nt=0;
	int k=0;

	pline=0;
	
	for(line; line<to; line++){

		if(l>255 || l<0)
			int etey=456456;


		// key
		if(!k){
			if(q==1){ if(*line=='"') q=-1; }
			if(!pline){ pline=line; if(*line=='"') q=1; }

			//if(*line=='}' || *line==']'){
			//	if(l<S1K) arr[l]=0; l--; pline=0; if(line<to && *(line+1)==',') line++; continue;
			//}

			
			if(*line=='}' || *line==']' || *line==','){
				if(q && line-pline>1){
					if(!te){
						te=&conf.inserta();
					
						if(l>=0 && l<S1K) if(l>0){
							if(!arr[l-1]->a){
								arr[l-1]->a=te;
							}else{
								if(!arr[l-1]->a->n){ arr[l-1]->a->n=te; arr[l-1]->a->r=te; te->p=arr[l-1]->a; te->r=te->p; }
								else{ te->p=arr[l-1]->a->r; te->p->n=te; te->r=arr[l-1]->a; te->p->r=te->r; arr[l-1]->a->r=te; }
							}
						}
					}
					if(*(line-1)=='"'){ te->val.setu(pline+1, line-pline-2); } else{ te->val.setu(pline, line-pline); }
					k=0; q=0; pline=0; te=0;
				}			

				if(*line=='}' || *line==']'){
					if(l>=0 && l<S1K) arr[l]=0; l--; pline=0;
					if(line<to && *(line+1)==','){ line++; continue; }
				}				
				//if(l<S1K) arr[l]=0; l--; if(line<to && *(line+1)==',') line++; continue;
			}

			if((*line==':' || *line=='{' || *line=='[') && q<1){
				if(*line==':') k=1;
				XMLel*el=&conf.inserta();
				if(q && line-pline>1 && *(line-1)=='"'){ el->key.setu(pline+1, line-pline-2); }else { el->key.setu(pline, line-pline); }
				pline=0; q=0; te=el;

				if(l>=0 && l<S1K) if(l>0){
					if(!arr[l-1]->a){
						arr[l-1]->a=te;
					}else{
						if(!arr[l-1]->a->n){ arr[l-1]->a->n=te; arr[l-1]->a->r=te; te->p=arr[l-1]->a; te->r=te->p; }
						else{ te->p=arr[l-1]->a->r; te->p->n=te; te->r=arr[l-1]->a; te->p->r=te->r; arr[l-1]->a->r=te; }
					}
				}

				if(*line=='{' || *line=='['){ if(l>=0 && l<S1K) arr[l]=el; l++; te=0; }
				if(line+1<to && ( *(line+1)=='{' || *(line+1)=='[')){
					if(l>=0 && l<S1K) arr[l]=el; l++; k=0; line++; te=0;
				}				
			}
		}
		// val
		else{
			if(q==1){ if(*line=='"') q=-1; }
			if(!pline){ pline=line; if(*line=='"') q=1; }

			if((*line==',' || *line=='}' || *line==']') && q<1){
				if(q && line-pline>1 && *(line-1)=='"'){ te->val.setu(pline+1, line-pline-2); }else { te->val.setu(pline, line-pline); }
				k=0; q=0; pline=0; te=0;
			
				if(*line=='}' || *line==']'){
					if(l>=0 && l<S1K) arr[l]=0; l--;
					if(line<to && *(line+1)==',') line++;
				}
			}
		}
	}

	return 1;
}

int Read(unsigned char*data, unsigned int size, bool noclear=0){ // return pos;
	if(!noclear) {conf.Clear(); sz=0; }

	XMLel *arr[256], *err[256], *ta=0, *tg=0; memset(arr, 0, sizeof(arr)); memset(err, 0, sizeof(arr));
	int /*l=0,*/ il=0; l=0; arr[l]=0; //&conf.el(0);

unsigned char*pos=data, *lpos=data, *to_=data+size/*, *t*/; int ld=0;
unsigned char*line=data, *pline=data, *to=data+size; int cl=0, ll=0, q=0;//, nt=0;
//if(*pos!='<') { return 0; } pos++; lpos=pos; if(*(to-1)=='>') to--;

//if(!rts(data, size, ' ', pos)) return 0;
//ttl.set((char*)lpos, pos-lpos); lpos=++pos;


///new
for(line; line<to; line++){
	if(q){
		if(q==1 && *line=='>'){ q=0; if(line>data && *(line-1)=='/') cl=1; if(line>data &&( *(line-1)=='?' || *(line-1)=='-' )) cl=2; }
		else if(q==2){ if(*line=='\'') q=1; } // '
		else if(q==3){ if(*line=='"') q=1; } // ;
		else if(*line=='\'') q=2; else if(*line=='"') q=3;

		if(!ll && (*line==' ' || !q)){ ll++;
			// insert first element
			//XMLel*el=&conf.inserta();
			//el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;
			//if(l>0 && !arr[l-1]){ arr[l-1]=set(el); }
			//arr[l]=el;
			if(tg){
				tg->key.setu(pline, line-pline);
				if(tg->key=="stream:stream") cl=1;
			}
			pline=line+1;
			
			//if(l>0 && arr[l-1]->a==XMLN){ arr[l-1]->a=set(el); }
			//if(!arr[l]) arr[l]=el; el->key.setu(pline, line-pline);
			//el->r=set(arr[l]); if(arr[l]!=el) {el->p=arr[l]->r; get(arr[l]->r)->n=set(el);} // set prev
			//el->r=set(arr[l]); arr[l]->r=set(el); arr[l+1]=el; pline=line+1; // r=first; a-r=end;
		}

		else
		if(q==1 && (*line=='=' || *line==' ' || (line-pline && *line=='/') /*|| (line-pline && (*line=='\'' || *line=='"'))*/) || !q){
			if(ta && (*line==' ' || *line=='/' || !q)){
				if(ta) {
					if(*pline=='\'' && line>pline && *(line-1)=='\'' || *pline=='"' && line>pline && *(line-1)=='"'){
						ta->val.setu(pline+1, line-pline-2); }
					else ta->val.setu(pline, line-pline);
				ta=0;
			}} else if(line-pline){
				XMLel*el=&conf.inserta(); ll++;
				el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN; el->r=set(el);
				//if(l>0 && arr[l-1]->a==XMLN){ arr[l-1]->a=set(el); }
				//if(!arr[l]) arr[l]=el;
				el->key.setu(pline, line-pline);
				if(tg->l==XMLN){ tg->l=set(el); }
				el->r=tg->l; if(get(tg->l)!=el) {
					el->p=get(tg->l)->r; get(tg->l)->r=set(el); el->r=tg->l; get(el->p)->n=set(el); }
				//el->r=set(arr[l]); 
				//el->r=set(arr[l]); arr[l]->r=set(el);

				ta=el;
			}
			pline=line+1;
		}

		if(!q){
			pline=line+1; tg=0;
		}

	} else{
		if(line==pline && *line==10 || *line==13) pline++;

		if(*line=='<') { q=1; ll=0; // qtype & level-eLement = 0
		if(cl) {l--; if(!l) cl=0; if(l<0) l=0; }
		
		if(line-pline && !isspace(line, pline)){ // set text
			XMLel*el=&conf.inserta();
			el->r=XMLN; el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;
			el->val.setu(pline, line-pline);

			if(!arr[l]){
				arr[l]=el;
				if(l>0) err[l-1]->a=set(el);
			}else{
				//if(get(arr[l]->r)) el->p=arr[l]->r; else el->p=set(arr[l]); get(el->p)->n=set(el); //<p, n>
				arr[l]->r=set(el); el->r=set(arr[l]); // <r>

				if(!err[l]){
					if(l>0) err[l-1]->a=set(el);
				}else{
					err[l]->r=set(el); err[l]->n=set(el); el->p=set(err[l]);
				}
				//el->p=arr[l]->a;
			} // =copy
			err[l]=el; err[l+1]=0; arr[l+1]=0;// arr[l]=el;

		}
		pline=line+1;

		cl=0; if(line+1<to && *(line+1)=='/') cl=1;

		if(!cl){
		XMLel*el=&conf.inserta(); tg=el;
		el->r=XMLN; el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;

			if(!arr[l]){
				arr[l]=el;
				if(l>0) err[l-1]->a=set(el);
			}else{
			//if(get(arr[l]->r)) el->p=arr[l]->r; else el->p=set(arr[l]); get(el->p)->n=set(el); //<p, n>
				arr[l]->r=set(el); el->r=set(arr[l]); // <r>

				if(!err[l]){
					if(l>0) err[l-1]->a=set(el);
				}else{
					err[l]->r=set(el); err[l]->n=set(el); el->p=set(err[l]);
				}
				//el->p=arr[l]->a;
			} // =copy
			err[l]=el; err[l+1]=0; arr[l+1]=0; // arr[l]=el;
		}

		if(!cl) l++;
		//if(nt){ l--; nt=0; }

	//	el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;
	//	if(l>0 && arr[l-1]->a==XMLN){ arr[l-1]->a=set(el); }
	//	el->r=set(arr[l]); if(arr[l]!=el) {el->p=arr[l]->r; get(arr[l]->r)->n=set(el);}
	//	el->r=set(arr[l]); arr[l]->r=set(el);
			
		}
	}
	//if(!q) && *line=='<'){
} if(cl){ l--; }


// return 1;
return 1;
}

int isspace(unsigned char *line, unsigned char *pline){
	while(pline<line){
		if(*pline++!=' ') return 0;
	}
	return 1;
}


VString Find(XMLel*el, VString line){
	for(el; el; el=get(el->n)){
		if(el->key==line) return el->val;
	}
return VString();
}


VString FindA(XMLel*el, VString line){
	for(el; el; el=get(el->n)){
		if(el->key==line && get(el->a)) return get(el->a)->key;
	}
return VString();
}

XMLel* FindX(XMLel*el, VString line){
	for(el; el; el=get(el->n)){
		if(el->key==line) return el;
	}
return 0;
}

XMLel* FindX(VString line){
	if(!conf.size()) return 0;
	XMLel*el=&conf.el(0);
	for(el; el; el=get(el->n)){
		if(el->key==line) return el;
	}
return 0;
}


void Clear(){ conf.Clear(); sz=0; }

/*
bool Add(FString k, MTEV v){
int s;
	s=k.size(); ls.add((char*)&s, 4); ls.add(k, k);
	s=sizeof(MTEV); ls.add((char*)&s, 4);
	if(v.size()>1) ls.add(v.VData(), datatype[DATATYPE_SZ].size*v.size());
return 1;
}

void Clear(){ ls.Clear(); sz=0; }

int Read(unsigned char*data, unsigned int size){ sz=0;
	// read title
unsigned char*pos=data, *lpos=data, *to=data+size;
if(*pos!='<') { return 0; } pos++; lpos=pos; if(*(to-1)=='>') to--;

if(!rts(data, size, ' ', pos)) return 0;
ttl.set((char*)lpos, pos-lpos); lpos=++pos;

while(1){
int er=rts(data, size, ' ', pos);
}
return sz;
}*/

/*
MTEV operator [](unsigned int id){
if(id>=sz) return MTEV();
return val[id];
}*/
/*
MTEV operator [](const VString& k){
for(int i=0; i<sz; i++){ if(key[i]==k) return val[i]; }
return MTEV();
}

MTEV operator [](const char* _k){ VString k(_k);
for(int i=0; i<sz; i++){ if(key[i]==k) return val[i]; }
return MTEV();
}

operator char*(){return ls.nomov();}
operator unsigned int() {return ls.size();}
*/
};

XMLel* XMLGet(VString line, XMLel *el){
	if(!el) return 0;
	while(el && line.is()){
		VString o=PartLine(line, line, ".");
		el=el->FindX(el, o);
		if(el) el=el->a;
	}
	return el;
}

/*
void XMLtoCont(XMLel *el, Cont &ct){
	if(!el) return ;
	//el=el->a;
	
	while(el){
		ct.Add(el->key, el->a ? el->a->val : "");
		el=el->n;
	}
	//ct.Read(ct.ls);
	ct.Read(VString(ct.ls.oneline(), ct.ls.size()));
	return ;
}*/


// eXtended Data Element
class XDataEl{
public:
	XDataEl *_p, *_n, *_a, *_e, *_l;
	VString key, val;

	XDataEl* n()const{ if(this) return _n; return 0; }
	XDataEl* a()const{ if(this) return _a; return 0; }

	VString k()const{ if(this) return key; return VString(); }
	VString v()const{ if(this) return val; return VString(); }

	VString GetVal(VString line){
		XDataEl*el=Get(line);
		if(el) return el->val;
		return VString();
	}

	VString GetLVal(VString line){
		XDataEl*el=GetL(line);
		if(el) return el->val;
		return VString();
	}

	XDataEl* GetByPos(int pos){
		XDataEl*el=this; if(el) el=el->_a; if(!el) return 0;
		while(el && pos){
			el=el->n(); pos--;
		}

		return el;
	}

	XDataEl* GetByKey(VString key, VString val){
		XDataEl*el=this; if(el) el=el->_a; if(!el) return 0;
		while(el){
			if(el->GetVal(key)==val)
				return el;
			el=el->n();
		}

		return el;
	}


	// find Find(_a-> ->_n ... )
	XDataEl* Get(VString line){
		XDataEl *el=this; if(el) el=el->_a; if(!el) return 0;
		VString o, t; int l=0;

		while(el && line.is()){
			o=PartLineO(line, "."); t=PartLineO(line, ",");
			if(o && o.sz<t.sz){ o=PartLine(line, line, "."); l=0; } else{ o=PartLine(line, line, ","); l=1; }

			if(o.sz==1 && *o.data==':'){
				el=el->a();
				continue;
			}

			el=el->FindX(el, o);
			if(el && line) el= !l ? el->_a : el->_l;
			if(line.sz==1 && *line.data==':') return el;
		}
		return el;
	}

	XDataEl* FindX(XDataEl*el, VString line){
		for(el; el; el=el->_n){
			if(el->key==line) return el;
		}
		return 0;
	}

	XDataEl* GetL(VString line){
		if(!this)
			return 0;

		XDataEl*el;
		for(el=_l; el; el=el->_n){
			if(el->key==line)
				return el;
		}
		return 0;
	}

	int GetSize()const{
		int sz=0;
		for(XDataEl *el=_a; el; el=el->_n)
			sz++;

	return sz;
	}

	void AddToList(XDataEl *el){
		if(!_a){
			_a = el;
			_e = el;
			el->_p = 0;
			el->_n = 0;
		} else{
			_e->_n = el;
			el->_p = _e;
			_e = el;
			el->_n = 0;
		}

		return ;
	}

};

class XDataPEl{
	XDataEl *el;

public:

	XDataPEl(){
		el=0;
	}

	XDataPEl(XDataEl *e){
		el=e;
	}

	XDataPEl p(){
		if(!el)
			return 0;
		return XDataPEl(el->_p);
	}

	XDataPEl n(){
		if(!el)
			return 0;
		return XDataPEl(el->_n);
	}

	XDataEl* a(){
		if(!el)
			return 0;
		return el->_a;
	}

	operator bool(){
		return el!=0;
	}

	operator XDataEl*(){
		return el;
	}

	VString key(){
		if(!el)
			return VString();
		return el->key;
	}

	VString val(){
		if(!el)
			return VString();
		return el->val;
	}

	int Size(){
		if(el)
			return el->GetSize();
		return 0;
	}

	VString operator [](const VString key){
		if(!el)
			return VString();
		return el->GetVal(key);
	}

	VString operator [](const char *key){
		if(!el)
			return VString();
		return el->GetVal(key);
	}

	XDataEl* GetByPos(const int pos){
		return el->GetByPos(pos);
	}

	XDataPEl GetByKey(const VString key, VString val){
		return el->GetByKey(key, val);
	}

	XDataEl* operator ()(const VString key){
		if(!el)
			return 0;
		return el->Get(key);
	}


};

#include "UMatrixXData.h"

// XData Control
class XDataCon{

public:
	XDataEl* XDataAddFirst(XDataLine &data){
		XDataEl *el=&data.inserta();
		return el;
	}

	XDataEl* XDataAdd(XDataLine &data, XDataEl *parent=0){
		XDataEl *el=&data.inserta();

		if(!parent) return el;
		if(!parent->_a){
			parent->_a=el; parent->_e=el;
		}else{
			parent->_e->_n=el; el->_p=parent->_e;
			parent->_e=el;
		}

		return el;
	}

	// Test
	void XDataPrint(XDataEl *el, int l=0){
		if(!el) return ;
		print("\r\n"); for(int i=0; i<l; i++){ print(" "); }
		print("{");
		while(el){
			print("\r\n"); for(int i=0; i<l; i++){ print(" "); }
			print("'", el->key, "' => '", el->val, "'");
			if(el->_a){
				XDataPrint(el->_a, l+1);
			}
			el=el->_n;
			if(el) print(", ");
		}
		print("\r\n"); for(int i=0; i<l; i++){ print(" "); }
		print("}");
		return ;
	}


};

// eXtended Data Container
class XDataCont{
	//XDataLine data;
	UList<XDataEl, 0, 1> data;

	int sz;

public:
	XDataCont(){ sz=0; }
	~XDataCont(){ Clear(); }

	XDataCont(VString line){
		sz=0;
		Read(line);
		return ;
	}

	// Get Data
	operator XDataEl*(){ if(data.Size()) return data.Get(0); return 0; }
	operator XDataPEl(){ if(data.Size()) return data.Get(0); return 0; }
	XDataEl* GetData(){ if(data.Size()) return data.Get(0); return 0; }

	// GetValue
	VString GetValue(VString line){ return GetData()->GetVal(line); }
	XDataEl* Get(VString line){ return GetData()->Get(line); }
	VString operator[](VString line){ return GetData()->GetVal(line); }

	XDataEl* operator ()(const VString key){
		return Get(key);
	}


	void Release(){ Clear(); return ; }
	
	// Read
	int Read(VString data, bool noclear=0){
		if(data[0]=='{' || data[0]=='[')
			return ReadJson(data, noclear);

		if(data[0]=='<')
			return ReadXML(data, noclear);

		Clear();
		return 0;
	}


	// Json
	int ReadJson(VString data, bool noclear=0){
		return ReadJson(data, data, noclear);
	}

	int ReadJson(unsigned char*data, unsigned int size, bool noclear=0){
		if(!noclear){ Clear(); }

		ReadJsonDo(0, data, data+size);

		return 1;
	}

	void ReadJsonDo(XDataEl*parent, unsigned char*&line, unsigned char*to){
		VString k, v; unsigned char *lline=line;
		XDataEl*fel=0, *eel=0, *el; int sh;

		while(line<to){

			// data "key" "val"
			if(*line=='"'){
				lline=++line; sh=0;
				while(line<to){
					if(*line=='"')
						break;
					if(*(line)=='\\'){						
						if(line+1<to)
							line++;
						sh=1;
					}
					line++;
				}
				v.setu(lline, line-lline);
				line++; continue;
			}
			else if(*line=='\''){
				lline=++line;  sh=0;
				while(line<to){
					if(*line=='\'')
						break;
					if(*(line)=='\\'){						
						if(line+1<to)
							line++;
						sh=1;
					}
					line++;
				}
				v.setu(lline, line-lline);
				line++; continue;
			}


			// data 0-9 or val
			if(*line=='-' || *line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z'){
				lline=+line; 
				while(line<to){ if(!(*line=='-' || *line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z')) break; line++; }
				v.setu(lline, line-lline);
				if(v=="null")
					v.Clean();
				continue;
			}

			// key:val
			if(*line==':'){ k=v; v.sz=0; line++; continue; }
		
			// space
			if(*line==' '){ line++; continue; }

			// next, up, down
			if(*line==',' || *line==']' || *line=='[' || *line=='{' || *line=='}'){
			
				if(k || v || *line=='{' || *line=='['){
					// add element
					el = data.Add(); // &data.inserta();
					el->key=k; el->val=v; k.sz=0; v.sz=0;

					if(fel){ // first element
						el->_p=eel; eel->_n=el; eel=el;
						if(parent)
							parent->_e=el;
					} else{
						if(parent){
							parent->_a=el; parent->_e=el; // update parent
						}
						fel=el; eel=el; // this is first
					}
				}

				// up, down
				if(*line==','){ line++; continue; }
				if(*line=='[' || *line=='{'){ line++; ReadJsonDo(el, line, to); continue; }
				if(*line==']' || *line=='}'){ line++; return ; }
			}

			int what_the_sumbol=1;

			// what the sumbol? ignore
			line++;
		}

		return ;
	}

	// Unknown Format. Hmm... T1 -> TF(irst)
	int ReadTF(VString data, bool noclear=0){
		return ReadTF(data, data, noclear);
	}

	int ReadTF(unsigned char*data, unsigned int size, bool noclear=0){
		if(!noclear){ Clear(); }

		ReadTFDo(0, data, data+size);

		return 1;
	}

	void ReadTFDo(XDataEl*parent, unsigned char*&line, unsigned char*to){
		VString k, v; unsigned char *lline=line;
		XDataEl*fel=0, *eel=0, *el;

		while(line<to){

			// data "key" "val"
			if(*line=='"'){
				lline=++line; 
				while(line<to){ if(*line=='"' && ( line<=lline || *(line-1)!='\\')) break; line++; }
				v.setu(lline, line-lline);
				line++; continue;
			}

			// data 0-9 or val
			if(*line=='-' || *line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z'){
				lline=+line; 
				while(line<to){ if(!(*line=='-' || *line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z')) break; line++; }
				v.setu(lline, line-lline);
				continue;
			}

			// key:val
			if(*line==':'){ k=v; v.sz=0; line++; continue; }
		
			// space
			if(*line==' '){ line++; continue; }

			// next, up, down
			if(*line==',' || *line==']' || *line=='[' || *line=='{' || *line=='}'){
			
				if(k || v || *line=='{' || *line=='['){
					// add element
					el = data.Add(); //&data.inserta();
					el->key=k; el->val=v; k.sz=0; v.sz=0;

					if(fel){ // first element
						el->_p=eel; eel->_n=el; eel=el;
						if(parent) parent->_e=el;
					} else{
						if(parent){
							parent->_a=el; parent->_e=el; // update parent
						}
						fel=el; eel=el; // this is first
					}
				}

				// up, down
				if(*line==','){ line++; continue; }
				if(*line=='[' || *line=='{'){ line++; ReadJsonDo(el, line, to); continue; }
				if(*line==']' || *line=='}'){ line++; return ; }	
			}

			int what_the_sumbol=1;

			// what the sumbol? ignore
			line++;
		}

		return ;
	}


	// XML
	int ReadXML(VString data, bool noclear=0){
		return ReadXML(data, data, noclear);
	}

	int ReadXML(unsigned char*data, unsigned int size, bool noclear=0){
		if(!noclear){ Clear(); }

		ReadXMLDo(0, data, data+size);

		return 1;
	}

	void ReadXMLDo(XDataEl*parent, unsigned char*&line, unsigned char*to){
		VString k, v; unsigned char *lline=line;
		XDataEl *pel=0, *fel=0, *eel=0, *el=0, *nppl = 0;

		int type=0;

		while(line<to && *line!='<'){ line++; }

		if(line != lline){
			pel = data.Add(); // &data.inserta();
			pel->val.setu(lline, line-lline);

			if(parent){
				if(parent->_a){
					pel->_p=parent->_e;
					parent->_e->_n=pel;
					parent->_e=pel;
					pel->_n=0;
				}else{
					parent->_a=pel;
					parent->_e=pel;
				}
			} else{
				if(nppl)
					nppl->_n = pel;
				nppl = pel;
			}
		}
		
		if(line>=to)
			return ;

		// <tag>text</tag>
		//if(line+1<to && *(line+1)=='/'){
		//	if(line-lline && parent)
		//		parent->val.setu(lline, line-lline);
		//	return ;
		//}

		type=0; lline=++line;
		
		while(line<to){
			if(*line==' ' || *line=='\t' || *line=='\r' || *line=='\n' || *line=='>' || *line=='/'){
				if(type==0){
					pel = data.Add(); // &data.inserta();
					pel->key.setu(lline, line-lline);

					if(parent){
						if(parent->_a){
							pel->_p=parent->_e;
							parent->_e->_n=pel;
							parent->_e=pel;
							pel->_n=0;
						}else{
							parent->_a=pel;
							parent->_e=pel;
						}
						//pel->_a=0;
						//pel->_e=0;
						//pel->_l=0;
					} else{
						if(nppl)
							nppl->_n = pel;
						nppl = pel;
					}

					type=1;
				}else if(type==1){
					if(line!=lline)
						v.setu(lline, line-lline);

					if(k && v){
						//ReadXMLDoSetKV();
						el = data.Add(); //&data.inserta();
						el->key=k; el->val=v;
						
						if(!pel->_l){
							pel->_l=fel=eel=el;
						}else{
							eel->_n=el; el->_p=eel; eel=el;
						}
						
						k.Clean(); v.Clean();
						//
					}
				}

				if(*line=='>'){
					if(line>lline && *(line-1)=='/')
						type=2;
					
					line++;
					break;
				}

				lline=line+1;
			}
			else if(*line=='='){
				if(line!=lline){
					v.setu(lline, line-lline); lline=line+1;
				}
				k=v;
				v.Clean();
			}
			else if(*line=='\''){
				lline=++line;
				while(line<to){
					if(*line=='\'' && (line<=lline || *(line-1)!='\\')){
						v.setu(lline, line-lline); lline=line+1;
						break;
					}
					line++;
				}
			}
			else if(*line=='"'){
				lline=++line;
				while(line<to){
					if(*line=='"' && (line<=lline || *(line-1)!='\\')){
						v.setu(lline, line-lline); lline=line+1;
						break;
					}
					line++;
				}
			}
			else if(*line=='>'){
				if(line!=lline)
					v.setu(lline, line-lline);

				if(k && v){
					//ReadXMLDoSetKV();
					el = data.Add(); //&data.inserta();
					el->key=k; el->val=v;
						
					if(!pel->_l){
						pel->_l=fel=eel=el;
					}else{
						eel->_n=el; el->_p=eel; eel=el;
					}
						
					k.Clean(); v.Clean();
					//
				}

				if(line>lline && *(line-1)=='/')
					type=2;

				line++;
				break;
			}

			// what the sumbol? ignore
			line++;
		}

		if(type==1 && pel)
			ReadXMLDo(pel, line, to);

		// </tag>

		while(line<to){
			if(*line=='>')
				break;
			line++;
		}

		return ;
	}


	int ReadHtml(VString data, bool noclear=0){
		return ReadHtml(data, data, noclear);
	}

	int ReadHtml(unsigned char *data, unsigned int size, bool noclear = 0){
		if(!noclear)
			Clear();

		XDataEl *el = this->data.Add();

		ReadHtmlDo(el, data, data + size);

		return 1;
	}

	XDataEl* ReadHtmlAdd(XDataEl *parent){
		XDataEl *el = data.Add();

		if(parent){
			if(!parent->_a){
				parent->_a = el;
				parent->_e = el;
				//el->_p = 0;
				//el->_n = 0;
				//el->_l = 0;
			} else{
				parent->_e->_n = el;
				el->_p = parent->_e;
				parent->_e = el;
				//el->_n = 0;
				//el->_l = 0;
			}
		}

		return el;
	}

	XDataEl* ReadHtmlAddL(XDataEl *parent, XDataEl *lparent){
		if(!parent)
			return 0;

		XDataEl *el = data.Add();

		if(!lparent){
			parent->_l = el;
		} else{
			lparent->_l = el;
		}

		lparent = el;

		return el;
	}

	struct ReadHtmlPrev{
		ReadHtmlPrev *prev;
		VString tag;
		int toit;
		//XDataEl *parent;

		ReadHtmlPrev(){
			prev = 0;
			toit = 0;
		}

		void GoTo(VString tag){
			ReadHtmlPrev *p = prev;

			while(p){
				if(p->tag == tag){
					p->toit = 1;
					return ;
				} else
					p->toit = 0;
			}

			if(prev)
				prev->toit = 1;
		}

	};

	void ReadHtmlDo(XDataEl *parent, unsigned char *&line, unsigned char *to, ReadHtmlPrev *prev = 0){
		XDataEl *el, *lel;
		VString k, v;
		unsigned char *lline = line;

		// Prev
		ReadHtmlPrev nprev;
		nprev.prev = prev;

		int opt_cl; // </tag>
		int opt_kv; // key(0) = val(1)

		while(line < to){
			lline = line;

			// Find <
			while(line < to && *line != '<')				
				line ++;
			
			if(line != lline){				
				el = ReadHtmlAdd(parent);
				el->val = VString(lline, line - lline);
				lline = line;
			}

			// <tag>
			line ++;
			
			// </tag>
			if(line < to && *(line) == '/'){
				line ++;
				opt_cl = 2;
			}
			else
				opt_cl = 0;

			lline = line;

			// tag name
			while(line < to && ( *line != ' ' && *line != '\t' && *line != '\n'
				 && *line != '\r' && *line != '/' && *line != '>'))
				line ++;

			if(opt_cl){
				nprev.GoTo(VString(lline, line - lline));
				line ++;
				break;
			}

			el = ReadHtmlAdd(parent);
			nprev.tag = VString(lline, line - lline);
			el->key = nprev.tag;
			lline = line;
			opt_kv = 0;

			lel = 0;
		
			// to end tag
			while(line < to && *line != '>'){

				// string
				if(*line >= 'a' && *line <= 'z' || *line >= 'A' && *line <= 'Z' || *line >= '0' && *line <= '9'){
					lline = line;
					while(line < to && (*line >= 'a' && *line <= 'z' || *line >= 'A' && *line <= 'Z' || *line >= '0' && *line <= '9'))
						line ++;

					if(!opt_kv){
						k.setu(lline, line - lline);
					} else {
						v.setu(lline, line - lline);
						opt_kv = 2;
					}
				}

				// "text"
				else if(*line == '"'){
					lline = ++line;

					while(line < to){
						if(*line == '"')
							break;

						if(*line == '\\'){						
							if(line + 1 < to)
								line ++;
	
						}
						line ++;
					}
					v.setu(lline, line - lline);
					if(opt_kv)
						opt_kv = 2;
					line ++;
					continue;
				}

				// 'text'
				else if(*line == '\''){
					lline = ++line;

					while(line < to){
						if(*line == '\'')
							break;

						if(*line == '\\'){						
							if(line + 1 < to)
								line ++;
	
						}
						line ++;
					}
					v.setu(lline, line - lline);
					if(opt_kv)
						opt_kv = 2;
					line ++;
					continue;
				}

				else if(*line == '=' && k){
					opt_kv = 1;
					line ++;
				}

				//if(*line != ' ' || *line != '\t' || *line != '\n'
				// || *line != '\r' || *line != '/' || *line != '>'){
				//	 lline = line;
				//}

				else
					line ++;

				if(opt_kv == 2){
					lel = ReadHtmlAddL(el, lel);
					lel->key = k;
					lel->val = v;
					opt_kv = 0;
					k.Clean();
				}
			}

			if(line < to){
				if(*(line - 1) == '/')
					opt_cl = 1;
				line ++;
			}

			if(opt_cl == 0){
				ReadHtmlDo(el, line, to, &nprev);
				if(!nprev.toit)
					return ;
			} /*else if(opt_cl == 2){
				nprev.GoTo();
				break;
			}*/
		}

		return ;
	}


	// XML Valid. bad: -1, EOF: 0, Ok: 1.
	static int ValidateXML(VString data){ return ValidateXML(data, data.sz); }

	static int ValidateXML(unsigned char *data, unsigned int sz){
		int l=0;

		unsigned char *line=data, *to=data+sz;
		while(line<to){
			if(*line=='<'){
				line++;
				if(line>=to) return 0;
				if(*line=='/') l--; else l++;

				while(line<to){
					if(*line=='>'){
						if(*(line-1)=='/') l--;
						line++;
						break;
					}
					line++;
				}

				if(l<0) return -1;
			}

			line++;
		}

		if(l<0) return -1;
		return l==0;
	}


	void Clear(){ data.Clear(); sz=0; }

};


// Fast XML Parser

struct XMLFCall{
public:
	virtual void start() = 0;
	virtual void end() = 0;

	virtual void tag(VString name) = 0;
	virtual void etag(VString name) = 0;

	virtual void attr(VString key, VString val) = 0;
	virtual void text(VString text) = 0;
};



// Class fast xml parser
#ifdef NOUSEIT
class XMLFCont{
public:
XMLLine conf;
int l;
int sz;

XMLFCont(){ sz=0; }
// add deletelist

operator XMLel*(){ return &conf.el(0); }

void Release(){conf.Clear(); return ; }

// {"chat":{"s":63485400402818,"i":7898,"r":["m.63","a.17027"]},"n":{"n":1},"t":1349812896171,"u":1349810379282,"q":[143,141,138,74],"g":1944889,"r":2979}

int ReadJson(unsigned char*data, unsigned int size, bool noclear=0){ // return pos;
	if(!noclear) {conf.Clear(); sz=0; }

	XMLel *arr[S1K], *err[S1K], *te=0, *ta=0, *tg=0; memset(arr, 0, sizeof(arr)); memset(err, 0, sizeof(arr));
	int /*l=0,*/ il=0; l=0; arr[l]=0; //&conf.el(0);

	//if(data && size && data[0]=='{' && data[size-1]=='}'){ data++; size--; }

	unsigned char*pos=data, *lpos=data, *to_=data+size/*, *t*/; int ld=0;
	unsigned char*line=data, *pline=data, *to=data+size; int cl=0, ll=0, q=0;//, nt=0;
	int k=0;

	pline=0;
	
	for(line; line<to; line++){

		if(l>255 || l<0)
			int etey=456456;


		// key
		if(!k){
			if(q==1){ if(*line=='"') q=-1; }
			if(!pline){ pline=line; if(*line=='"') q=1; }

			if(*line=='}' || *line==']'){
				if(l<S1K) arr[l]=0; l--; pline=0; if(line<to && *(line+1)==',') line++; continue;
			}
			

			if((*line==':' || *line=='{' || *line=='[') && q<1){
				if(*line==':') k=1;
				XMLel*el=&conf.inserta();
				if(q && line-pline>1 && *(line-1)=='"'){ el->key.setu(pline+1, line-pline-2); }else { el->key.setu(pline, line-pline); }
				pline=0; q=0; te=el;

				if(l<S1K) if(l>0){
					if(!arr[l-1]->a){
						arr[l-1]->a=te;
					}else{
						if(!arr[l-1]->a->n){ arr[l-1]->a->n=te; arr[l-1]->a->r=te; te->p=arr[l-1]->a; te->r=te->p; }
						else{ te->p=arr[l-1]->a->r; te->p->n=te; te->r=arr[l-1]->a; te->p->r=te->r; arr[l-1]->a->r=te; }
					}
				}

				if(*line=='{' || *line=='['){ if(l<S1K) arr[l]=el; l++; }
				if(line+1<to && ( *(line+1)=='{' || *(line+1)=='[')){
					if(l<S1K) arr[l]=el; l++; k=0; line++;
				}				
			}
		}
		// val
		else{
			if(q==1){ if(*line=='"') q=-1; }
			if(!pline){ pline=line; if(*line=='"') q=1; }

			if((*line==',' || *line=='}' || *line==']') && q<1){
				if(q && line-pline>1 && *(line-1)=='"'){ te->val.setu(pline+1, line-pline-2); }else { te->val.setu(pline, line-pline); }
				k=0; q=0; pline=0; te=0;
			
				if(*line=='}' || *line==']'){
					if(l<S1K) arr[l]=0; l--;
					if(line<to && *(line+1)==',') line++;
				}
			}
		}
	}

	return 1;
}

int Read(XMLFCall &ct, unsigned char*data, unsigned int size){ // return pos;

	XMLel *arr[256], *err[256], *ta=0, *tg=0; memset(arr, 0, sizeof(arr)); memset(err, 0, sizeof(arr));
	int /*l=0,*/ il=0; l=0; arr[l]=0; //&conf.el(0);
	VString var[S1K];
	VString key, val, tag;

unsigned char*pos=data, *lpos=data, *to_=data+size/*, *t*/; int ld=0;
unsigned char*line=data, *pline=data, *to=data+size; int cl=0, ll=0, q=0;//, nt=0;

ct.start();

///new
for(line; line<to; line++){
	if(q){
		if(q==1 && *line=='>'){ q=0; if(line>data && *(line-1)=='/') cl=1; if(line>data &&( *(line-1)=='?' || *(line-1)=='-' )) cl=2; }
		else if(q==2){ if(*line=='\'') q=1; } // '
		else if(q==3){ if(*line=='"') q=1; } // ;
		else if(*line=='\'') q=2; else if(*line=='"') q=3;

		if(!ll && (*line==' ' || !q)){ ll++;
			if(tg){
				//tg->key.setu(pline, line-pline);
				key.setu(pline, line-pline);
				if(/*tg->*/key=="stream:stream") cl=1;
				ct.tag(key); var[l-1]=key; key.Clean();
			}
			pline=line+1;
		}

		else
		if(q==1 && (*line=='=' || *line==' ' || (line-pline && *line=='/') /*|| (line-pline && (*line=='\'' || *line=='"'))*/) || !q){
			if(ta && (*line==' ' || *line=='/' || !q)){
				if(ta) {
					if(*pline=='\'' && line>pline && *(line-1)=='\'' || *pline=='"' && line>pline && *(line-1)=='"'){
						/*ta->*/val.setu(pline+1, line-pline-2); }
					else /*ta->*/val.setu(pline, line-pline);
					ct.attr(key, val); key.Clean(); val.Clean();
				ta=0;
			}} else if(line-pline){
				/*XMLel*el=&conf.inserta();*/ ll++;
				//el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN; el->r=set(el);
				//if(l>0 && arr[l-1]->a==XMLN){ arr[l-1]->a=set(el); }
				//if(!arr[l]) arr[l]=el;
				/*el->*/key.setu(pline, line-pline);
				//if(tg->l==XMLN){ tg->l=set(el); }
				//el->r=tg->l; if(get(tg->l)!=el) {
				//	el->p=get(tg->l)->r; get(tg->l)->r=set(el); el->r=tg->l; get(el->p)->n=set(el); }
				//el->r=set(arr[l]); 
				//el->r=set(arr[l]); arr[l]->r=set(el);

				ta=(XMLel*)1; //el;
			}
			pline=line+1;
		}

		if(!q){
			pline=line+1; tg=0;
		}

	} else{
		if(line==pline && *line==10 || *line==13) pline++;

		if(*line=='<') { q=1; ll=0; // qtype & level-eLement = 0
		if(cl) {l--; if(!l) cl=0; if(l<0) l=0; ct.etag(var[l]); }
		
		if(line-pline && !isspace(line, pline)){ // set text
			//XMLel*el=&conf.inserta();
			//el->r=XMLN; el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;
			/*el->*/val.setu(pline, line-pline); ct.text(val); val.Clean();

			//if(!arr[l]){
			//	arr[l]=el;
			//	if(l>0) err[l-1]->a=set(el);
			//}else{
			//	//if(get(arr[l]->r)) el->p=arr[l]->r; else el->p=set(arr[l]); get(el->p)->n=set(el); //<p, n>
			//	arr[l]->r=set(el); el->r=set(arr[l]); // <r>

			//	if(!err[l]){
			//		if(l>0) err[l-1]->a=set(el);
			//	}else{
			//		err[l]->r=set(el); err[l]->n=set(el); el->p=set(err[l]);
			//	}
			//	//el->p=arr[l]->a;
			//} // =copy
			//err[l]=el; err[l+1]=0; arr[l+1]=0;// arr[l]=el;

		}
		pline=line+1;

		cl=0; if(line+1<to && *(line+1)=='/') cl=1;

		if(!cl){
		/*XMLel*el=&conf.inserta();*/ tg=(XMLel*)1; //el;
		//el->r=XMLN; el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;

			//if(!arr[l]){
			//	arr[l]=el;
			//	if(l>0) err[l-1]->a=set(el);
			//}else{
			////if(get(arr[l]->r)) el->p=arr[l]->r; else el->p=set(arr[l]); get(el->p)->n=set(el); //<p, n>
			//	arr[l]->r=set(el); el->r=set(arr[l]); // <r>

			//	if(!err[l]){
			//		if(l>0) err[l-1]->a=set(el);
			//	}else{
			//		err[l]->r=set(el); err[l]->n=set(el); el->p=set(err[l]);
			//	}
			//	//el->p=arr[l]->a;
			//} // =copy
			//err[l]=el; err[l+1]=0; arr[l+1]=0; // arr[l]=el;
		}

		if(!cl) l++;
		//if(nt){ l--; nt=0; }

	//	el->a=XMLN; el->l=XMLN; el->p=XMLN; el->n=XMLN;
	//	if(l>0 && arr[l-1]->a==XMLN){ arr[l-1]->a=set(el); }
	//	el->r=set(arr[l]); if(arr[l]!=el) {el->p=arr[l]->r; get(arr[l]->r)->n=set(el);}
	//	el->r=set(arr[l]); arr[l]->r=set(el);
			
		}
	}
	//if(!q) && *line=='<'){
} if(cl){ l--; ct.etag(var[l]); }


ct.end();

// return 1;
return 1;
}

int isspace(unsigned char *line, unsigned char *pline){
	while(pline<line){
		if(*pline++!=' ') return 0;
	}
	return 1;
}

};

#endif


class XMLParse;

class XMLParseCont{
public:
	XMLLine conf;

	void DoXMLParse(XMLParse &xml);

	XMLel * Get(){ if(conf.size()) return &conf.el(0); return 0; }

	XMLel * Get(VString ln){ return Get()->Get(ln); }



	void Clear(){ conf.Clear(); }

};


//
#define XMLPARSE_USEMEM	1

#define XMLP_START	1
#define XMLP_END	2
#define XMLP_TAG	4
#define XMLP_CTAG	8
//#define XMLP_ETAG	16
#define XMLP_ATTR	16
#define XMLP_TEXT	32
#define XMLP_UTAG	64
#define XMLP_DTAG	128
//#define XMLP_SKIP	128
//#define XMLP_		128

#define XMLS_DO		1
//#define XMLS_TAG	2
//#define XMLS_ATTR	4
#define XMLS_CL		8
#define XMLS_ACL	16

class XMLParse{
	MString data; int data_sz, data_rd, data_pt;
	VString rdata;

	// opt
	int tag, qq; // tag: 0 - text, 1 - tag, 2 - arrt, 3 - attr_val. qq: 0 - no, 1 - ', 2 - ".

	VString tkey, ttag; // tmp key

//	unsigned char *ln, *to;

	int state;
public:
	XMLParse(){
		Clear();
		//data_sz=0; data_rd=0; data_pt=0; state=0; /*ln=0; to=0;*/
		//tag=0; qq=0;
	}

	void Clear(){
		data_sz=0; data_rd=0; data_pt=0; state=0;
		tag=0; qq=0; tkey.Clean();
		return ;
	}

protected:
	void AddData(VString line, int nocpy=0){
		if(tkey.data>=data.data && tkey.data<data.endu()){ tkey.data-=data.data-(unsigned char*)0; }
		else if(tkey.data>=rdata.data && tkey.data<rdata.endu()){ tkey.data-=rdata.data-(unsigned char*)0; }
		
		if(data_sz+line.sz>data.sz){ data.Reserv(data_sz+line.sz+S1K-(data_sz+line.sz)%S1K); }
		if(data_sz+line.sz<=data.sz){ memcpy(data.data+data_sz, line, line.sz); data_sz+=line.sz; }

		tkey.data+=data.data-(unsigned char*)0;
	}

	void SetVal(VString &val, unsigned char *pt, unsigned char *ln){
		if(!data.data || !rdata.data){ val.setu(pt, ln-pt); return ; }

		if(ln>=data.data && ln<data.data+data.sz && pt>=data.data && pt<data.data+data.sz
			|| ln>=rdata.data && ln<rdata.data+rdata.sz && pt>=rdata.data && pt<rdata.data+rdata.sz){
				val.setu(pt, ln-pt);
		}else{
			if(data_sz+ln-pt<=data.sz) memcpy(data.data+data_sz, pt, ln-pt);
			else{
				int s=data_sz; VString d; d.sz=ln-pt; AddData(d, 1); data_sz=s;
				memcpy(data.data+data_sz, pt, ln-pt);
			}
			val.setu(data.data+data_sz, ln-pt);
		}
		return ;
	}

	void SetVal(VString &tkey, VString &val, unsigned char *pt, unsigned char *ln){
		if(!data.data || !rdata.data){ val.setu(pt, ln-pt); return ; }

		int sz=tkey.sz+ln-pt;
		if(data_sz+sz>data.sz){ int s=data_sz; AddData(VString(data.data, ln-pt), 1); data_sz=s; }
		if(data_sz+sz<=data.sz){
			memcpy(data.data+data_sz, tkey.data, tkey.sz); tkey.data=data.data+data_sz;
			memcpy(data.data+data_sz+tkey.sz, pt, ln-pt); val.setu(tkey.data+tkey.sz, ln-pt);
		}
		return ;
	}

public:
	void Read(VString line, int opt=0){
		if(rdata){ AddData(rdata); rdata.Clean(); }
		
		if(opt&XMLPARSE_USEMEM) rdata=line;
		else{ AddData(line); }
		
		return ;
	}

	int Do(XMLParseCont &cont){
		cont.DoXMLParse(*this);
		return 1;	
	}

	int Do(VString &key, VString &val){
		int ret=0;//, tag=state&XMLS_TAG, q=0;
		if(!state){ state=XMLS_DO; ret=XMLP_START; }

		unsigned char *ln, *to, *pt;
		if(data_rd<data_sz){ ln=data.data+data_rd; to=data.data+data_sz; }
		else { ln=rdata.data+data_rd-data_sz; to=rdata.data+rdata.sz; }
		if(data_pt<data_sz){ pt=data.data+data_pt; } else { pt=rdata.data+data_pt-data_sz; }

		if(!tag){
			if(state&XMLS_CL) state&=~XMLS_CL;
			if(state&XMLS_ACL){ ret|=XMLP_CTAG; state&=~XMLS_ACL; SetVal(key, ttag.data, ttag.endu()); ttag.Clean(); return ret; }			
		}

		while(ln<to){
			// tag
			if(tag){
				if(*ln=='\''){ if(!qq) qq=1; else if(qq==1) qq=0; }
				else if(*ln=='"'){ if(!qq) qq=2; else if(qq==2) qq=0; }
				else if(qq){ ln++; continue; }
				else if((*ln=='?' || *ln=='!') && tag==1) state|=XMLS_ACL;
				else if(*ln==' ' || *ln=='=' || *ln=='\r' || *ln=='\n' || *ln=='\t' || *ln=='/' || *ln=='>'){

					if((*ln==' ' || *ln=='\r' || *ln=='\n' || *ln=='\t') && pt==ln){ pt=++ln; continue; }
					
					//if(*ln=='>'){}
					if(*ln=='/'){ if(tag==1){ state|=XMLS_CL; ret|=XMLP_DTAG; ln++; } else { state|=XMLS_ACL; } ln++; continue; }
					//if(*ln=='>') && !(state&XMLS_ACL)){ ret|=XMLP_CTAG; }
					if(*ln=='>' && !(state&(XMLS_ACL|XMLS_CL))) ret|=XMLP_UTAG;

					if(tag==1){
						if(*ln=='='){ tag=3; tkey.Clean(); } else if(*ln=='>') tag=0; else tag=2;
						SetVal(key, pt, ln); ttag.setu(pt, ln-pt); ln++; pt=ln; ret|=XMLP_TAG;
						//if(state&XMLS_CL){ state&=~XMLS_CL; ret|=XMLP_TAG; } 
						break;
					}

					if(tag==2){
						if(*ln=='='){
							tkey.setu(pt, ln-pt); pt=ln+1; tag=3;
						}else{
							SetVal(key, pt, ln); val.Clean(); ret|=XMLP_ATTR; tag= *ln=='>' ? 0:2; ln++; pt=ln;  break;							
						}
					}
					else if(tag==3){
						SetVal(tkey, val, pt, ln); key=tkey;

						if(val.sz>1)
							if(*val.data=='\'' && val.endo()=='\'' || *val.data=='"' && val.endo()=='"'){
								val.data++; val.sz-=2;
							}

							ret|=XMLP_ATTR; tag= *ln=='>' ? 0:2; ln++; pt=ln; break;
					}

				 }
			}
			// text
			else{
				if(*ln=='<'){
					tag=1;
					if(ln!=pt){
						SetVal(val, pt, ln);
						ln++; pt=ln; ret|=XMLP_TEXT; break;
					} pt=ln+1;
				}
			}

			ln++;
		}

		if(ln>=data.data && ln<data.data+data_sz){ data_rd=ln-data.data; }
		else{ data_rd=ln-rdata.data+data_sz; }
		if(pt>=data.data && pt<=data.data+data_sz){ data_pt=pt-data.data; } else { data_pt=pt-rdata.data+data_sz; }
		//if(tag) state|=XMLS_TAG; else state&=~XMLS_TAG;

		//if(state&XMLS_CL){ state&=~XMLS_CL; ret|=XMLP_ETAG; }

		if(ln==to) if(to==rdata.endu() || !rdata) ret|=XMLP_END; // else to=rdata.end();

		return ret;
	}

/*
	void Read(XMLFCall &xc, VString data){
		Read(data, XMLPARSE_USEMEM);
		VString key, val;

		while(1){
			int r=Do(key, val); if(r&XMLP_END){ xc.end(); break; }
			if(r&XMLP_START) xc.start();
			if(r&XMLP_TEXT) xc.text(val);
			if(r&XMLP_TAG) xc.tag(key);
			if(r&XMLP_ETAG) xc.etag(key);
			if(r&XMLP_ATTR) xc.attr(key, val);		
		}
		return ;
	}*/
};




void XMLParseCont::DoXMLParse(XMLParse &xml){
	XMLel *arr[S1K], *err[S1K], *ta=0, *tg=0; memset(arr, 0, sizeof(arr)); memset(err, 0, sizeof(arr));

		VString key, val; int l=0;

		while(1){
			int r=xml.Do(key, val);
			if(r&XMLP_START){  }
			//if(r&XMLP_TEXT) xc.text(val);
			if(r&XMLP_TAG){
				//if(r&XMLP_ETAG){ arr[l]=0; err[l]=0; l--; print("F\r\n"); r&=~XMLP_ETAG; }

				for(int i=0; i<l; i++) print("|"); print(key); print("\r\n");
				if(l>=0 || key[0]!='/'){					
					XMLel*el=&conf.inserta();
					if(!arr[l]){ arr[l]=el; el->r=el; if(l) err[l-1]->a=el; }
					else{
						el->p=err[l]; el->p->n=el;
						arr[l]->r=el; el->r=arr[l];					
					}
					err[l]=el; el->key=key;
				}
			}
			if(r&XMLP_ATTR){
				XMLel*el=&conf.inserta();
				if(!(err[l]->l)){ err[l]->l=el; el->r=el; }
				else{ el->p=err[l]->l->r; el->p->n=el; err[l]->l->r=el; el->r=err[l]->l; }
				el->key=key; el->val=val;
			}
			if(r&XMLP_CTAG){ print("C\r\n"); }
			if(r&XMLP_UTAG){ l++; print("U\r\n"); }
			if(r&XMLP_DTAG){ arr[l]=0; err[l]=0; l--; print("E\r\n"); }
			if(r&XMLP_END){ break; }
			//print(".\r\n");
		}
	return ;
}



// GetLine
void GetLine(XDataEl &val, HLString &ls);

MString GetLine(XDataEl &val){
	if(!val._a) return MString();
	HLString ls;
	GetLine(val, ls);
	return ls;
}

MString GetLine(XDataEl *val){
	if(!val) return MString();
	HLString ls;
	GetLine(*val, ls);
	return ls;
}

void GetLine(XDataEl *val, HLString &ls){
	if(!val) return ;
	GetLine(*val, ls);
	return ;
}

void GetLine(XDataEl &val, HLString &ls){
	int a=val.a() && val.a()->key, f=1;
	if(a)
		ls+"{";
	else
		ls+"[";

	XDataEl *v=val._a;
	while(v){
		if(!f) ls+","; else f=0;
		if(v->_a){
			if(a)
				ls+"\""+v->key+"\":";
			GetLine(*v, ls);
		} else{
			if(v->key)
				ls+"\""+v->key+"\":";
			if(!v->val) ls+"null";
			else ls+"\""+v->val+"\"";
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