// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:ef937b6bb9da0dcc8187bda35ec94e07 : 2010.12.22 14:58:58

//////////////// NEW OBJECT MATRIX //////////////////////////////////////////////////
class ProcStructProxy{ // lists & free
OMatrix data; int datasz;
int maxsz, newsz, allsz;
listen_proxy *lists, *elists; int listssz;
listen_proxy *free, *efree; int freesz;
unsigned int freetime;// free time , ltime;



public:
//ProcStructProxy(){ memset(this, 0, sizeof(ProcStructProxy)); maxsz=S2GM; newsz=LSLISTSIZE; }
//ProcStructProxy(int max){ memset(this, 0, sizeof(ProcStructProxy)); maxsz=max; newsz=LSLISTSIZE; }
ProcStructProxy(int max=OMATRIXMAXELS, int nsz=OMATRIXNEWSZ){ memset(this, 0, sizeof(ProcStructProxy)); maxsz=max; newsz=nsz; }
listen_proxy*GetFirst(){ return lists; }

listen_proxy*New(){// noblock & use Lock Prev

	if(!free){ if(listssz>=maxsz) return 0; 
		OMatrix*nwo=(OMatrix*)GetBuffer(sizeof(OMatrix)+sizeof(listen_proxy)*newsz); if(!nwo) return 0;	
		listen_proxy *nw=(listen_proxy*)(nwo+1); data.Add(nwo); datasz++; //  new listen_proxy[LSLISTSMAX]; 
		allsz+=newsz;
		listen_proxy *f=free; if(!f) free=nw;
		for(int i=0; i<newsz; i++){
			if(f) f->_n=&nw[i]; nw[i]._p=f; f=&nw[i];		
		} efree=f; freesz+=newsz;
	}

	// free to lists
listen_proxy *el=free; free=(listen_proxy*)free->_n; if(!free) efree=0; else free->_p=0; freesz--; listssz++;
 new(el)listen_proxy; 
if(!lists){ lists=el; elists=el; el->_p=0; el->_n=0; bool b=el->block(); el->Clear(); el->block(b); }
else { elists->_n=el; el->_p=elists; el->_n=0; elists=el; }
el->use(1);
//if(!lists) { lists=el; elists=el; el->_p=0; el->_n=0; el->Clear(); }
//elists->_n=el; el->_p=elists; el->_n=0; elists=el;



return el;
}

inline void dell(listen_proxy*el){ // !el - crash
	if(el->_n) el->_n->_p=el->_p; else if(el==elists) elists=(listen_proxy*)el->_p;
	if(el->_p) el->_p->_n=el->_n; else if(el==lists) lists=(listen_proxy*)el->_n;
	el->_n=0; el->_p=0;
	listssz--;// freesz++;
}

inline void delf(listen_proxy*el){ // !el - crash
	if(el->_n) el->_n->_p=el->_p; else if(el==elists) elists=(listen_proxy*)el->_p;
	if(el->_p) el->_p->_n=el->_n; else if(el==lists) lists=(listen_proxy*)el->_n;
	el->_n=0; el->_p=0;
	freesz--;// listssz++;
}

bool Del(listen_proxy*el){// noblock & use Lock Prev
if(!el) return 0; // out lists


dell(el); freesz++;


 el->~listen_proxy(); 
	// in free & is delete
	if(!free) { free=el; efree=el; el->_p=0; el->_n=0; return 1;}
	if(allsz-listssz>OMFREE && freetime+OMFREET>=time()){
		listen_proxy *f=efree, *tf, *fe, *ef=f; int fs;
		while(f){
			if(data.isfirst(f)){ tf=f; fe=f+newsz; fs=0;
				while(ef){ if(ef>=f && ef<fe) fs++; ef=(listen_proxy*)ef->_n; }
			//	for(fe; tf<fe; tf++){ if(tf->isuse()) break; }
				if(fs==newsz){ tf=f; // delete block
					for(fe; tf<fe; tf++){ delf(tf); }
                    data.Del((OMatrix*)f-1); datasz--; DelBuffer(f); freetime=time(); return 1; // deleted
				}
			} f=(listen_proxy*)f->_n;
		}
		freetime=time();
	}
	// add to free
	if(el>efree-maxsz){ efree->_n=el; el->_p=efree; el->_n=0; efree=el; }
	else { free->_p=el; el->_n=free; el->_p=0; free=el; } // if(el<free+LSLISTSMAX)

return 1;
}

};
//////////////// NEW OBJECT MATRIX //////////////////////////////////////////////////
class ProcStructHTTP{ // lists & free
OMatrix data; int datasz;
int maxsz, newsz, allsz;
LISTEN_HTTP *lists, *elists; int listssz;
LISTEN_HTTP *free, *efree; int freesz;
unsigned int freetime;// free time , ltime;



public:
//ProcStructHTTP(){ memset(this, 0, sizeof(ProcStructHTTP)); maxsz=S2GM; newsz=LSLISTSIZE; }
//ProcStructHTTP(int max){ memset(this, 0, sizeof(ProcStructHTTP)); maxsz=max; newsz=LSLISTSIZE; }
ProcStructHTTP(int max=OMATRIXMAXELS, int nsz=OMATRIXNEWSZ){ memset(this, 0, sizeof(ProcStructHTTP)); maxsz=max; newsz=nsz; }
LISTEN_HTTP*GetFirst(){ return lists; }

LISTEN_HTTP*New(){// noblock & use Lock Prev

	if(!free){ if(listssz>=maxsz) return 0; 
		OMatrix*nwo=(OMatrix*)GetBuffer(sizeof(OMatrix)+sizeof(LISTEN_HTTP)*newsz); if(!nwo) return 0;	
		LISTEN_HTTP *nw=(LISTEN_HTTP*)(nwo+1); data.Add(nwo); datasz++; //  new LISTEN_HTTP[LSLISTSMAX]; 
		allsz+=newsz;
		LISTEN_HTTP *f=free; if(!f) free=nw;
		for(int i=0; i<newsz; i++){
			if(f) f->_n=&nw[i]; nw[i]._p=f; f=&nw[i];		
		} efree=f; freesz+=newsz;
	}

	// free to lists
LISTEN_HTTP *el=free; free=(LISTEN_HTTP*)free->_n; if(!free) efree=0; else free->_p=0; freesz--; listssz++;
 new(el)LISTEN_HTTP; 
if(!lists){ lists=el; elists=el; el->_p=0; el->_n=0; bool b=el->block(); el->Clear(); el->block(b); }
else { elists->_n=el; el->_p=elists; el->_n=0; elists=el; }
el->use(1);
//if(!lists) { lists=el; elists=el; el->_p=0; el->_n=0; el->Clear(); }
//elists->_n=el; el->_p=elists; el->_n=0; elists=el;



return el;
}

inline void dell(LISTEN_HTTP*el){ // !el - crash
	if(el->_n) el->_n->_p=el->_p; else if(el==elists) elists=(LISTEN_HTTP*)el->_p;
	if(el->_p) el->_p->_n=el->_n; else if(el==lists) lists=(LISTEN_HTTP*)el->_n;
	el->_n=0; el->_p=0;
	listssz--;// freesz++;
}

inline void delf(LISTEN_HTTP*el){ // !el - crash
	if(el->_n) el->_n->_p=el->_p; else if(el==elists) elists=(LISTEN_HTTP*)el->_p;
	if(el->_p) el->_p->_n=el->_n; else if(el==lists) lists=(LISTEN_HTTP*)el->_n;
	el->_n=0; el->_p=0;
	freesz--;// listssz++;
}

bool Del(LISTEN_HTTP*el){// noblock & use Lock Prev
if(!el) return 0; // out lists


dell(el); freesz++;


 el->~LISTEN_HTTP(); 
	// in free & is delete
	if(!free) { free=el; efree=el; el->_p=0; el->_n=0; return 1;}
	if(allsz-listssz>OMFREE && freetime+OMFREET>=time()){
		LISTEN_HTTP *f=efree, *tf, *fe, *ef=f; int fs;
		while(f){
			if(data.isfirst(f)){ tf=f; fe=f+newsz; fs=0;
				while(ef){ if(ef>=f && ef<fe) fs++; ef=(LISTEN_HTTP*)ef->_n; }
			//	for(fe; tf<fe; tf++){ if(tf->isuse()) break; }
				if(fs==newsz){ tf=f; // delete block
					for(fe; tf<fe; tf++){ delf(tf); }
                    data.Del((OMatrix*)f-1); datasz--; DelBuffer(f); freetime=time(); return 1; // deleted
				}
			} f=(LISTEN_HTTP*)f->_n;
		}
		freetime=time();
	}
	// add to free
	if(el>efree-maxsz){ efree->_n=el; el->_p=efree; el->_n=0; efree=el; }
	else { free->_p=el; el->_n=free; el->_p=0; free=el; } // if(el<free+LSLISTSMAX)

return 1;
}

};
// END TEMPLATE 
// Do Not Write To This File, All Data Well Be Lost!
// ##EMD5:ef937b6bb9da0dcc8187bda35ec94e07

