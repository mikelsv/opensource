struct pinfo{
u32 source, dest; // ip
u16 ps, pd; // ports
u64	sz; u32 pz;
};// 24b

#define PINFOSZ		S128K

struct _uius{
	u32 a; u16 b;
};

 int sniff_nosave_text=0, sniff_nosave_data=0;

class pinfod{
pinfo *data;
int sz, asz;

pinfo wbuf[128]; int wbufsz;

public:
	pinfod(){ data=0; sz=0; asz=0; wbufsz=0;  }
	~pinfod(){ Clean(); }

#define _pinfod_mov(l){ memcpy(l+1, l, (to-l)*sizeof(pinfo)); }
#define _pinfod_add(l){ l->sz+=s; l->pz+=pz; }
#define _pinfod_set(l){ l->source=source; l->dest=dest; l->ps=ps; l->pd=pd; l->sz=s; l->pz=pz; sz++; }

	void test(){// return ;
		pinfo *l=data, *to=data+sz;
		for(l; l<to-1; l++){
			if(l->source>(l+1)->source ||
				l->source==(l+1)->source && l->ps>(l+1)->ps ||
				l->source==(l+1)->source && l->ps==(l+1)->ps && l->dest>(l+1)->dest ||
				l->source==(l+1)->source && l->ps==(l+1)->ps && l->dest==(l+1)->dest && l->pd>=(l+1)->pd)
				globalerror("TRAFF_TEST");
				//int et=456;
		}
	}

	int addl(u32 source, u32 dest, u16 ps, u16 pd, u64 s, int pz=1){
		return add(ntohl(source), ntohl(dest), ntohs(ps), ntohs(pd), s, pz);
	}


#define sniffifmin(p) (source<p->source || source==p->source && ps<p->ps) || \
			(source<p->source || source==p->source && ps==p->ps) && \
			(dest<p->dest || dest==p->dest && pd<p->pd)

#define sniffifmax(p) (source>p->source || source==p->source && ps>p->ps) || \
			(source>p->source || source==p->source && ps==p->ps) && \
			(dest>p->dest || dest==p->dest && pd>p->pd)

#define sniffifcmp(p) (source==p->source && ps==p->ps && dest==p->dest && pd==p->pd)
#define sniffifcmpm(p, source, dest, ps, pd) (source==p->source && ps==p->ps && dest==p->dest && pd==p->pd)

	int add(u32 source, u32 dest, u16 ps, u16 pd, u64 s, int pz=1){// _uius _s={source, ps}, _d={dest, pd};
		/*if(sz==asz){ //add size
			pinfo *l=data; data=(pinfo*)GetBuffer(sizeof(pinfo)*(asz+PINFOSZ));
			if(!data){ data=l; return 0; }
			memcpy(data, l, asz*sizeof(pinfo));// memset(data+asz*sizeof(pinfo), 0, PINFOSZ*sizeof(pinfo));
			DelBuffer(l); asz+=PINFOSZ;
		}*/

		if(wbufsz>=sizeof(wbuf)/sizeof(pinfo)){
			OneBlock();
			if(wbufsz) return 0;
		}

		pinfo*a=findh(data, data+sz, source, dest, ps, pd);
		if(a){ a->sz+=s; a->pz+=pz; return 1; }
		a=find(wbuf, &wbuf[wbufsz], source, dest, ps, pd);
		if(sniffifcmp(a)){ a->sz+=s; a->pz+=pz; return 1; }
		else { pinfo *to=&wbuf[wbufsz]; _pinfod_mov(a); _pinfod_set(a); sz--; wbufsz++; return 2; }

		return 0;

		//pinfo *l=data, *to=data+sz, *p=l, *t=to;
		pinfo *l=wbuf, *to=&wbuf[wbufsz], *p=l, *t=to;
		if(l<to){
			test();
			if(sniffifmin(p)){ test(); _pinfod_mov(l); _pinfod_set(l); test(); return 2; }
			p=t-1;
			if(sniffifmax(p)){ test(); _pinfod_set(t); test(); return 2; }
		} else if(l==to){ _pinfod_set(l); return 2; }

		p=l+((to-l)/2);

		while(t-l){
			if(sniffifmin(p)){ if(t==p) t--; else t=p; }
			else if(sniffifmax(p)){ if(l==p) l++; else l=p; }
			else if(sniffifcmp(p)){ _pinfod_add(p); return 1; }
			else { p++; _pinfod_mov(p); _pinfod_set(p); return 2; }

			/*/if(source==p->source && ps==p->ps){  }
			if((source<p->source || source==p->source && ps<p->ps) ||
				(source<p->source || source==p->source && ps==p->ps) &&
				(dest<p->dest || dest==p->dest && pd<p->pd)){ if(t==p) t--; else t=p; }
			else if((source>p->source || source==p->source && ps>p->ps) ||
				(source>p->source || source==p->source && ps==p->ps) &&
				(dest>p->dest || dest==p->dest && pd>p->pd)){ if(l==p) l++; else l=p; }
            else if(source==p->source && ps==p->ps && dest==p->dest && pd==p->pd){ _pinfod_add(p); return 1; }
			else { p++; _pinfod_mov(p); _pinfod_set(p); return 2; } */

			p=l+((t-l)/2);
		}

		test();
		_pinfod_mov(t); _pinfod_set(t);
		test();
		return 2;
	}

	pinfo* find(pinfo*l, pinfo*to, u32 source, u32 dest, u16 ps, u16 pd){
		pinfo *p=l, *t=to;
		if(l<to){
			if(sniffifmin(p)){ return p; }
			p=t-1;
			if(sniffifmax(p)){ return p+1; }
		} else if(l==to){ return p; }

		p=l+((to-l)/2);
		while(t-l){
			if(sniffifmin(p)){ if(t==p) t--; else t=p; }
			else if(sniffifmax(p)){ if(l==p) l++; else l=p; }
			else if(sniffifcmp(p)){ return p; }
			else { return p+1; }
			p=l+((t-l)/2);
		}
		return t;
	}

	pinfo* findh(pinfo*l, pinfo*to, u32 source, u32 dest, u16 ps, u16 pd){
		pinfo *p=l, *t=to;
		if(l<to){
			if(sniffifmin(p)){ return 0; }
			p=t-1;
			if(sniffifmax(p)){ return 0; }
		} else if(l==to){ return 0; }

		p=l+((to-l)/2);
		while(t-l){
			if(sniffifmin(p)){ if(t==p) t--; else t=p; }
			else if(sniffifmax(p)){ if(l==p) l++; else l=p; }
			else if(sniffifcmp(p)){ return p; }
			else { return 0; }
			p=l+((t-l)/2);
		}
		return 0;
	}

	void OneBlock(){
		if(sz+wbufsz>=asz){ //add size
			pinfo *l=data; data=(pinfo*)malloc(sizeof(pinfo)*(asz+PINFOSZ)); memset(data, 0, sizeof(pinfo)*(asz+PINFOSZ));
			if(!data){ data=l; return ; }
			memcpy(data, l, asz*sizeof(pinfo));// memset(data+asz*sizeof(pinfo), 0, PINFOSZ*sizeof(pinfo));
			free(l); asz+=PINFOSZ;
		}

		if(sz+wbufsz>=asz){ return ; }

		pinfo *x[sizeof(wbuf)/sizeof(pinfo)]; int i, im=0;

		for(i=0; i<wbufsz; i++){
			x[i]=find(data, data+sz, wbuf[i].source, wbuf[i].dest, wbuf[i].ps, wbuf[i].pd);
			//if(sniffifcmp(x[i], wbuf[i].source, wbuf[i].dest, wbuf[i].ps, wbuf[i].pd)
		}

	//	for(i=0; i<wbufsz; i++){
	//		x[i]+=im; im++;
	//	}

		pinfo *l=data, *to=data+sz+wbufsz, *p=l, *t=to-wbufsz;
		for(i=wbufsz-1; i>=0; i--){
			if(t>x[i]){ memcpy(x[i]+1+i, x[i], (char*)t-(char*)x[i]); }
			t=x[i];
			*(x[i]+i)=wbuf[i];
		}

		test();
		sz+=wbufsz; wbufsz=0;
		return ;
	}


	operator pinfo*()const { return data; }
	int size()const { return sz; }

	void operator-=(pinfod &pf){ move(pf); }
	void move(pinfod &pf){ Clean(); pf.OneBlock(); data=pf.data; sz=pf.sz; asz=pf.asz; pf.data=0; pf.sz=0; pf.asz=0; }
	void move(MString &ms){ Clean(); data=(pinfo*)ms.rchar(); sz=ms.size()/sizeof(pinfo); asz=sz; ms.data = 0; ms.sz = 0; }

	void Clean(){ free(data); data=0; sz=0; asz=0; wbufsz=0; }

	// Save options
	int SaveTxtLog(const MString &fl, unsigned int tfr, unsigned int tto){ if(sniff_nosave_text) return 0;
		HFILE hf=CreateFile(fl, O_RDWR|O_CREAT, S_IREAD | S_IWRITE);
		if(!ishandle(hf)) return 0;
		// add sort!!
		MTime mt; Itos it, itp;
		HLString ls; ls+"Logs. "+mt.date("y.m.d H:i:s", tfr)+" - "+mt.date("y.m.d H:i:s", tto)+"\r\n";
		ls+"source ip:port, dest ip:port, bytes, packages\r\n";

		pinfo *l=data, *to=data+sz;
		for(l; l<to; l++){
			ls+it.ipitos(l->source)+":"+itp.itos(l->ps)+"  "; 
			ls+it.ipitos(l->dest)+":"+itp.itos(l->pd)+"  ";
			ls+it.itos64(l->sz)+"  "+itp.itos(l->pz)+"\r\n";
		}

		WriteFile(hf, ls.oneline(), ls.size());
		CloseHandle(hf);
		return 1;
	}

	int SaveData(const MString &fl){ if(sniff_nosave_data) return 0;
		HFILE hf=CreateFile(fl, O_RDWR|O_CREAT, S_IREAD | S_IWRITE);
		if(!ishandle(hf)) return 0;
		WriteFile(hf, (char*)data, sz*sizeof(pinfo));
		CloseHandle(hf);
		return 1;
	}

};

pinfod t_tcp, t_udp, t_oth, dt_tcp, dt_udp, dt_oth;
pinfod st_tcp, st_udp, st_oth, sdt_tcp, sdt_udp, sdt_oth;

u64 atcp=0, audp=0, aoth=0;

DWORD TraffixSaveLogs(LPVOID){ if(sniff_nosave_text && sniff_nosave_data) return 0;
	MkDir("logs/"); MkDir("logs/traffix/"); MTime mt; mt.mtime(time());
	int tm=time(); tm-=tm%3600;
	MString flp=HLString()+"logs/traffix/"+mt.date("y-m-d/", tm-1); MkDir(flp);
	MString flh=HLString()+flp+mt.date("y-m-d-H-", tm-1);
	MString fld=HLString()+flp+mt.date("y-m-d-", tm-1);

	if(st_tcp.size()){ st_tcp.SaveTxtLog(flh+"tcp.log", tm-3600, tm); st_tcp.SaveData(flh+"tcp.dta"); st_tcp.Clean(); }
	if(st_udp.size()){ st_udp.SaveTxtLog(flh+"udp.log", tm-3600, tm); st_udp.SaveData(flh+"udp.dta"); st_udp.Clean();}
	if(st_oth.size()){ st_oth.SaveTxtLog(flh+"oth.log", tm-3600, tm); st_oth.SaveData(flh+"oth.dta"); st_oth.Clean();}	

	
	if(sdt_tcp.size()){ sdt_tcp.SaveTxtLog(fld+"tcp.log", tm-86400, tm); sdt_tcp.SaveData(fld+"tcp.dta"); sdt_tcp.Clean(); }
	if(sdt_udp.size()){ sdt_udp.SaveTxtLog(fld+"udp.log", tm-86400, tm); sdt_udp.SaveData(fld+"udp.dta"); sdt_udp.Clean(); }
	if(sdt_oth.size()){ sdt_oth.SaveTxtLog(fld+"oth.log", tm-86400, tm); sdt_oth.SaveData(fld+"oth.dta"); sdt_oth.Clean(); }

return 0;
}