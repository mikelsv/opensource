GLock senddatalock;
// #define GLOBAL_USE_ONESENDDATALOCK

struct SendDataLD{
	SendDataLD *n;
	unsigned int asz, usz, ssz; // all size, use size, send size;
	unsigned char data[0];
}; // 10, 12, 16. [align 1, 4, 8]

//unsigned short sz[CNT_SDPSZ], asz[CNT_SDPSZ], ssz[CNT_SDPSZ]; // size, all size, send size
//char*data[CNT_SDPSZ]; BYTE tpos, spos;

class SendDataL{ public:
	SendDataLD *a, *e;
	unsigned int asz, usz, dsz; // all size, use size, del size(sended)
#ifdef GLOBAL_USE_ONESENDDATALOCK
	GLock senddatalock;
#endif
	
	SendDataL(){ memset(this, 0, sizeof(SendDataL)); }
	//New(int sz=S4K){ data=mcnew(sz); if() }
/////
	void Clear(){
		UGLOCK(senddatalock);
		SendDataLD *p=a, *d=p;
		while(p){
			d=p; p=p->n;
			free(d);
		}
		a=0; e=0; asz=0; usz=0; dsz=0;
_test();
	}
/////
	SendDataLD* New(unsigned int ns){ // prev lock
		//UGLOCK(senddatalock);
		unsigned int s=S2K;
		if(asz>144*S1K) s=S64K-1;
		else if(asz>=80*S1K) s=S32K;
		else if(asz>=48*S1K) s=S16K;
		else if(asz>=S16K) s=S8K;
		else if(asz>=S8K) s=S4K;
		if(ns>s/2 && s<=S32K) s*=2;
_test();		
		SendDataLD *d=(SendDataLD*)malloc(s); //mcnew(s);
		d->asz = s - sizeof(SendDataLD);
		d->n=0;
		d->usz=0;
		d->ssz=0;

		if(e){ e->n=d; e=d; }
		else { a=d; e=d; }		
		
		asz += d->asz;
_test();
		//print("New ", itos(s), "\r\n");
		return d;
	}
/////
	void DelA(){ // prev lock
		//UGLOCK(senddatalock);
		if(!a) return ;
		SendDataLD *d=a; a=a->n;
		if(!a) e=0;
		asz-=d->asz; usz-=d->usz; dsz-=d->usz-d->ssz;
_test();		
		free(d); //mcdel((char*)d);
		//("Del \r\n");
	}

	void _test(){
		// return ;

		SendDataLD *d=this->a;
		int a=0, u=0, s=0;

		while(d){
			a+=d->asz;
			u+=d->usz;
			s+=d->ssz;
			d=d->n;

			if(d && d->n && d==d->n->n) break;
		}

		if(a!=asz || u!=usz || dsz+s!=usz)
			int warn=911;

	}

//	BYTE Is(){ return fsz/S4K+((fsz%S4K)>S2K); }
	unsigned int Size(){ return dsz; }
	// >10 a? >20 xmm?  >50 oh! >100 wtf?

/////
	unsigned int Set(const VString &line){ return Set(line, line); }
	unsigned int Set(const void*dta, unsigned int s){ if(!s || !dta) return 1;
		UGLOCK(senddatalock);
		unsigned int ef, se=0;
_test();
		if(e){
			if(e->ssz==e->usz){ usz-=e->usz; e->usz=0; e->ssz=0; }
			ef=e->asz-e->usz;
		}else ef=0;
_test();

		while(s){
			if(ef){
				if(ef>s) ef=s;
				memcpy(e->data+e->usz, dta, ef);
				dsz+=ef; usz+=ef; e->usz+=ef; dta=(char*)dta+ef; s-=ef; se+=ef;
_test();
			} if(!s) return se;

			New(s);
_test();
			ef=e->asz;
		}
		return se;
	}

	//unsigned int Set(const SendDataL &sd){ }

/////
	void* Get(unsigned int &se){
		UGLOCK(senddatalock);
		if(!a || !dsz){ se=0; return 0; }		
		se=a->usz-a->ssz;
		//se=a->usz-a->ssz > se ? se : a->usz-a->ssz;
_test();
		//print("Se: ", itos(se), "\r\n");
		return a->data+a->ssz;
	}

	unsigned int Get(void *dt, unsigned int se){
		if(!a || !dsz){ return 0; }
		UGLOCK(senddatalock);
		SendDataLD *d=a; unsigned int s=0, t;
		while(d && se){
			t=d->usz-d->ssz > se ? se : d->usz-d->ssz;
			memcpy(dt, d->data+d->ssz, t); s+=t; dt=(char*)dt+t; se-=t;
			d=d->n;
		}
_test();
if(s>usz || s>dsz)
	int dsf=5454;
		return s;
	}

/////
	void Del(unsigned int se){
		if(!a || !dsz || !se){ se=0; return ; }
		UGLOCK(senddatalock);
		unsigned int t;
_test();
if(se>usz || se>dsz)
	int dsf=5454;
		//while(a && se>=a->usz-a->ssz && a->usz>a->asz/2){ se-=a->usz-a->ssz; DelA(); }
		while(a && se && se>=a->usz-a->ssz){
			se-=a->usz-a->ssz;  dsz-=a->usz-a->ssz; a->ssz=a->usz;
			if(a->asz==a->usz) DelA();
			//if(a->n)
			//else break;
		}

_test();

		//t= a ? min(se, a->usz-a->ssz) : 0;
		t= a ? (((se) < (a->usz-a->ssz)) ? (se) : (a->usz-a->ssz)) : 0;
		if(a && se && t){ a->ssz+=t; dsz-=t;  }
//			if(a){ a->ssz+=se; dsz-=se; }
_test();
		return ;
	}

/////
	void oneline(MString &ln){
		ln.Reserv(dsz);
		Set(ln, ln);
	}

};

class RecvDataD{
public:
	MString data;
	unsigned int sz;

	RecvDataD(){
		sz=0;
	}

	void Clear(){
		data.Clean();
		sz=0;
		return ;
	}

	~RecvDataD(){
		Clear();
	}

};

class RecvData{
public:
	RecvDataD *data;
	VString abuff, buff;
	int dsz;

	RecvData(){
		data=0;
		dsz=0;
	}

	~RecvData(){
		End();
	}

	int Read(RecvDataD &d, VString b, VString read){
		data=&d;
		buff=abuff=b;

		if(!data || data->sz+read.sz>buff)
			return 0;

		memcpy(buff, data->data, data->sz);
		memcpy(buff.data+data->sz, read, read);

		buff.sz=data->sz+read.sz;
		return 1;
	}

	int Del(int s){
		dsz+=s;
		return 1;
	}

	int Add(int s){
		if(buff.sz+s<=abuff.sz)
			buff.sz+=s;
		else
			buff.sz=abuff.sz;
		return 1;
	}

	int End(){
		if(dsz>=buff)
			buff.Clean();
		else{
			buff.data+=dsz;
			buff.sz-=dsz;
		}
		dsz=0;

		if(data->data.sz<buff){
			data->data.Reserv(buff.sz+S1K-buff.sz%1024);
		}
		
		memcpy(data->data, buff, buff);
		data->sz=buff;
		return 1;
	}

	VString GetBuffer(){
		return buff.str(dsz);
	}

	VString GetFree(){
		return VString(buff.endu(), abuff.sz-buff);
	}
};


// SendDataGLock - send data [one global lock]. Rebuild SendDataL class.
// Created 21.10.2016 16:55
#ifndef MSVCORE_SENDDATA_BUFSZ
	#define MSVCORE_SENDDATA_BUFSZ S8K
#endif

TLock SendDataGLockLock;

struct SendDataGLockD{
	SendDataGLockD *n;
	unsigned int asz, usz, ssz; // all size, use size, send size;
	unsigned char data[0];
}; // 10, 12, 16. [align 1, 4, 8]

//unsigned short sz[CNT_SDPSZ], asz[CNT_SDPSZ], ssz[CNT_SDPSZ]; // size, all size, send size
//char*data[CNT_SDPSZ]; BYTE tpos, spos;

class SendDataGLock{
public:
	SendDataGLockD *a, *e;
	unsigned int asz, usz, dsz; // all size, use size, del size(sended)
	
	SendDataGLock(){
		memset(this, 0, sizeof(SendDataGLockD));
	}


	void* Get(unsigned int &se){
		UGLOCK(SendDataGLockLock);

		if(!a || !dsz){
			se = 0;
			return 0;
		}

		se = a->usz - a->ssz;
		
		return a->data + a->ssz;
	}

	unsigned int Get(void *dt, unsigned int se){
		if(!a || !dsz){
			return 0;
		}

		UGLOCK(SendDataGLockLock);

		SendDataGLockD *d = a;
		unsigned int s = 0, t;

		while(d && se){
			t = d->usz - d->ssz > se ? se : d->usz - d->ssz;
			memcpy(dt, d->data + d->ssz, t);
			s += t;
			dt = (char*)dt + t;
			se -= t;
			d = d->n;
		}

		return s;
	}
	
protected:
	SendDataGLockD* New(unsigned int ns){ // prev lock
		if(ns % MSVCORE_SENDDATA_BUFSZ)
			ns = ns + MSVCORE_SENDDATA_BUFSZ - ns % MSVCORE_SENDDATA_BUFSZ;
			
		SendDataGLockD *d = (SendDataGLockD*)malloc(ns); //mcnew(s);
		d->asz = ns - sizeof(SendDataGLockD);
		d->n = 0;
		d->usz = 0;
		d->ssz = 0;

		if(e){
			e->n = d;
			e = d;
		}
		else {
			a = d;
			e = d; 
		}		
		
		asz += d->asz;

		return d;
	}

public:
	unsigned int Set(const VString &line){
		return Set(line, line);
	}

	unsigned int Set(const void*dta, unsigned int s){
		if(!s || !dta)
			return 1;

		UGLOCK(SendDataGLockLock);
		unsigned int ef, se = 0;

		// Use last element
		if(e){
			if(e->ssz == e->usz){
				usz -= e->usz;
				e->usz = 0;
				e->ssz = 0;
			}
			ef = e->asz - e->usz;
		}else
			ef = 0;

		while(s){
			if(ef){
				if(ef > s)
					ef = s;

				memcpy(e->data + e->usz, dta, ef);
				dta = (char*)dta + ef;
				dsz += ef;
				usz += ef;
				e->usz += ef;				
				s -= ef;
				se += ef;
			}
			if(!s)
				return se;

			if(!New(s))
				return se;

			ef = e->asz;
		}

		return se;
	}

	void Del(unsigned int se){
		if(!a || !dsz || !se){
			se = 0;
			return ;
		}

		UGLOCK(SendDataGLockLock);
		unsigned int t;

		if(se > usz || se > dsz){
			while(a && se && se >= a->usz - a->ssz){
				se -= a->usz - a->ssz;
				dsz -= a->usz - a->ssz;
				a->ssz = a->usz;
				if(a->asz == a->usz)
					DelA();
			}
		}

		t= a ? (((se) < (a->usz - a->ssz)) ? (se) : (a->usz - a->ssz)) : 0;
		if(a && se && t){
			a->ssz += t;
			dsz -= t;
		}

		return ;
	}

	void DelA(){ // prev lock
		if(!a)			
			return ;

		SendDataGLockD *d = a;
		if(!a)
			return;

		a = a->n;

		if(!a)
			e = 0;

		asz -= d->asz;
		usz -= d->usz;
		dsz -= d->usz - d->ssz;
		
		free(d);

		return ;
	}

	unsigned int Size(){
		return dsz;
	}

	void Clean(){
		SendDataGLockD *p, *d;

		{
			UGLOCK(SendDataGLockLock);

			p = a;
			d = p;

			a = 0;
			e = 0;
			asz = 0;
			usz = 0;
			dsz = 0;
		}

		while(p){
			d = p;
			p = p->n;
			free(d);
		}

		return ;
	}

};

TLock SendDataRingLock;

#define SENDDATALOCK_BUFSZ	S8K

class SendDataRing{
	MString buf;
	int pread, pwrite;
	
public:
	SendDataRing(){
		pread = 0;
		pwrite = 0;
	}

	// Max Read
	int IsRead(){
		return pwrite >= pread ? pwrite - pread : buf.sz - pread + pwrite;
		//return (buf.sz + pread - pread) % buf.sz;
	}

	int IsFree(){
		return buf.sz ? buf.sz - IsRead() : SENDDATALOCK_BUFSZ;
	}

	int Read(VString b){
		UGLOCK(SendDataRingLock);
		if(!b.sz)
			return 0;

		int ms = pwrite >= pread ? pwrite - pread : buf.sz - pread;
		int s = minel(ms, b.sz);
		memcpy(b, buf.data + pread, s);

		if(pwrite < pread && b.sz > ms){
			int ns = minel(pwrite, b.sz - s);
			memcpy(b.data + s, buf.data, ns);
			s+= ns;
		}

		return s;
	}
	
	VString ReadData(){
		UGLOCK(SendDataRingLock);
		if(pwrite >= pread)
			return VString(buf.data + pread, pwrite - pread);
		else
			return VString(buf.data + pread, buf.sz - pread);
	}

	int Readed(int sz){
		UGLOCK(SendDataRingLock);
		if(sz > IsRead())
			sz = IsRead();

		if(sz && buf.sz)
			pread = (pread + sz) % buf.sz;

		return 1;
	}

	int Write(VString b){
		UGLOCK(SendDataRingLock);
		if(!buf)
			Reserv();

		int as = 0;

		while(1){
			int f = buf.sz - IsRead();
			int s = minel(f, b.sz - as);
			if(!s)
				break;

			memcpy(buf.data + pwrite, b.data + as, s);

			pwrite = (pwrite + s) % buf.sz;
			as += s;
		}

		return as;
	}

	int Reserv(int sz = SENDDATALOCK_BUFSZ){
		UGLOCK(SendDataRingLock);
		return buf.Reserv(sz);
	}

	void Clean(){
		UGLOCK(SendDataRingLock);
		pread = 0;
		pwrite = 0;
		buf.Clean();
	}

	~SendDataRing(){
		UGLOCK(SendDataRingLock);
		buf.Clean();
	}

};