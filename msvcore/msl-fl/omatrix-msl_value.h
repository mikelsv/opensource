// MikelSV (MSV) Template Constructor v.0.1.0.1
// Do Not Write To This File, All Data Well Be Lost!
// ##MD5:b1a3c92f3ab6d3bb40577c2219b8832e : 2013.10.14 11:02:16

//////////////// NEW OBJECT MATRIX v2 ///////////////////////////////////////////////


class msl_value_matrix{
OMatrixT<OmatrixBlock> data; int datasz;
#ifdef msl_value_matrix_USE_L
#ifndef msl_value_matrix_NOUSE_L
#define msl_value_matrix_USE_R
OMatrixT<msl_value> use;//, free;
#endif
#endif
int maxsz, newsz; // info
int usesz, allsz;
unsigned int ltime;

// fast
OmatrixBlock *fre;



public:
msl_value_matrix(int max=OMATRIXMAXELS, int nsz=OMATRIXNEWELS/sizeof(msl_value)){
	memset(this, 0, sizeof(msl_value_matrix)); maxsz=max; newsz=nsz;
}
~msl_value_matrix(){
	OmatrixBlock*p=data._a;
	while(p){
		DelBuffer(p->data);
	}
#ifdef msl_value_matrix_USE_R
	use.OMClearS();
#endif
	data.OMClearS();
}

msl_value*GetFirst(){
#ifdef msl_value_matrix_USE_R
	return use._a;
#else 
        return 0;
#endif
}

int size(){ return usesz; }

msl_value*New(){
	

	if(!fre || fre->u==fre->a){
		OmatrixBlock*nw=data._a, *d=0;
		unsigned int s=newsz+1; fre=0;
		for(nw; nw; nw=nw->_n){
			if(s>nw->a-nw->u && nw->u!=nw->a){ fre=nw; s=nw->a-nw->u; }
			if(!nw->u) d=nw;
		}

		if(d && d!=fre && ltime+30<time()){
			datasz--; allsz-=d->a;			
			data.OMDel(d); DelBuffer(d);			
			ltime=time();
		}

		if(!fre){
			if(allsz>=maxsz || newsz<1) return 0; 
			nw=(OmatrixBlock*)GetBuffer(sizeof(OmatrixBlock)+sizeof(msl_value)*newsz+((newsz+7)/8));
			if(!nw) return 0;
			nw->u=0; nw->a=newsz; nw->sz=sizeof(msl_value)*newsz;
			data.OMAdd(nw); datasz++; allsz+=newsz;
			fre=nw; ltime=time();
		}
	}

	// free to lists
	if(!fre){ globalerror("OMATRIX2 !fre"); return 0; }
	int num=fre->getfree(); usesz++; if(num<0 || num>=newsz){ globalerror("OMATRIX2 num>maxsz"); return 0; }
	msl_value *el=((msl_value*)(fre+1))+num;
	fre->u++; fre->set(num, 1); 
	 new(el)msl_value; 
#ifdef msl_value_matrix_USE_R
	use.OMAdd(el); // list
#endif

	return el;
}


bool Del(msl_value*el){
	if(!el) return 0; // out lists
	

	OmatrixBlock*nw=data._a; int num;
	for(nw; nw; nw=nw->_n){
		if(nw->ismy(el)){ break; }
	} if(!nw) return 0;

	num=nw->getn(el); if(!nw->get(num)) globalerror("OMATRIX2 DEL FREE");
	nw->set(num, 0); nw->u--; usesz--;
	 el->~msl_value(); 

	if(nw->u>=nw->a) globalerror("OMATRIX2");

#ifdef msl_value_matrix_USE_R
	use.OMDel(el); // if list
#endif
return 1;
}


};// END TEMPLATE 
// Do Not Write To This File, All Data Well Be Lost!
// ##EMD5:b1a3c92f3ab6d3bb40577c2219b8832e

