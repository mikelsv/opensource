inline unsigned int mfsbs(unsigned int val){
unsigned int ret=0; while(val){ val/=2; ret++; } return ret;
}

// Сортировка построением цепочки.

class mfsstack{public:
unsigned int *data;
unsigned int sz, *pos, *fpos, *lpos;
mfsstack(){memset(this, 0, sizeof(mfsstack));}
~mfsstack(){ _deln(data);}
//inline unsigned int& operator[](unsigned int i){return data[i];}
//inline int& fpos(unsigned int i){return fpos[i];}
//inline int& lpos(unsigned int i){return lpos[i];}
void New(unsigned int size, unsigned int base, unsigned int basesize){
	_deln(data); sz=size; data=(unsigned int*)_newn((sz+base*2)*sizeof(int)+sz*basesize);
	pos=(unsigned int*)((char*)data+sz*basesize); fpos=pos+sz; lpos=fpos+base; memset(fpos, 255, base*2*sizeof(int)); return ;
}
};

void myfastsort(unsigned int size, unsigned int *arr, unsigned int base){
unsigned int bits=mfsbs(base-1), bit, k, p, j;
mfsstack st; st.New(size, base, sizeof(int));

for(unsigned int i=0; i<sizeof(int)*8; i+=bits){
	memset(st.data, 0, size*sizeof(int)+size*sizeof(int)); memset(st.fpos, 255, base*2*sizeof(int));
    bit=(base-1)<<i;
    for(j=0; j<size; j++) {
        k=arr[j]&bit; k=k>>i;
		if(st.fpos[k]==-1){ st.fpos[k]=j; } else { st.pos[st.lpos[k]]=j; }
		st.data[j]=arr[j]; st.lpos[k]=j;
    }k=0;

	for(j=0; j<base; j++) {
		p=st.fpos[j]; 
		if(p!=-1) while(1){
			arr[k]=st.data[p]; p=st.pos[p];	k++; if(!p) break;
		}
	}
} return ;
}

void myfastsort_sm(unsigned int size, unsigned int *arr, unsigned int ssz){ // save memory
//unsigned int bits=mfsbs(base-1), bit, k, p, j;
//mfsstack st; st.New(size, base, sizeof(int));
unsigned int *buf=new unsigned int[size];
unsigned int *bufp=new unsigned int[256];
//unsigned int *bufs=new unsigned int[ssz];

for(unsigned int i=0; i<ssz; i++){
	memset(bufp, 0, 256*sizeof(int));// memset(bufs, 0, ssz*sizeof(int));

	unsigned int bit=(255)<<i*8;
	for(unsigned int j=0; j<size; j++){
		unsigned int byte=(arr[j]&bit)>>i*8;
		bufp[byte]++;
	}

	unsigned int s, p=0;
	for(unsigned int j=0; j<256; j++){
		s=bufp[j]; bufp[j]=p; p+=s;
	}

	for(unsigned int j=0; j<size; j++){
		unsigned int byte=(arr[j]&bit)>>i*8;
		buf[bufp[byte]++]=arr[j];
	}
	memcpy(arr, buf, size*sizeof(int));
}

return ;
}

void myfastsort_duints_ipsz(DUINTS *arr, unsigned int size, unsigned int base){
unsigned int bits=mfsbs(base-1), bit, k, p, j;
mfsstack st; st.New(size, base, sizeof(DUINTS));

for(unsigned int i=0; i<sizeof(int)*8; i+=bits){
	memset(st.data, 0, size*sizeof(int)+size*sizeof(DUINTS)); memset(st.fpos, 255, base*2*sizeof(int));
    bit=(base-1)<<i;
    for(j=0; j<size; j++) {
        k=arr[j].pos&bit; k=k>>i;
		if(st.fpos[k]==-1){ st.fpos[k]=j; } else { st.pos[st.lpos[k]]=j; }
		((DUINTS*)st.data)[j]=arr[j]; st.lpos[k]=j; arr[j].pos=0;
    }k=0;

	for(j=0; j<base; j++){	p=st.fpos[j]; 
	if(p!=-1) while(1){ arr[k]=((DUINTS*)st.data)[p]; p=st.pos[p]; k++; if(!p) break; }}
	if(k!=size) globalerror("SORT ERROR");
}

for(unsigned int i=0; i<sizeof(int)*8; i+=bits){
	memset(st.data, 0, size*sizeof(int)+size*sizeof(DUINTS)); memset(st.fpos, 255, base*2*sizeof(int));
    bit=(base-1)<<i;
    for(j=0; j<size; j++) {
        k=arr[j].size&bit; k=k>>i;
		if(st.fpos[k]==-1){ st.fpos[k]=j; } else { st.pos[st.lpos[k]]=j; }
		((DUINTS*)st.data)[j]=arr[j]; st.lpos[k]=j; arr[j].pos=0;
    }k=0;

	for(j=base-1; j>-1; j--) { p=st.fpos[j]; 
		if(p!=-1) while(1){ arr[k]=((DUINTS*)st.data)[p]; p=st.pos[p]; k++; if(!p) break;
	}} if(k!=size) globalerror("SORT ERROR");
}
return ;
}

#ifdef IntStrILine
void myfastsort_duints_intstr(IntStrILine& isi, int base=256){ IntStr *arr=isi.data; int size=isi.size;
unsigned int bits=mfsbs(base-1), bit, k, p; int j;
mfsstack st; st.New(size, base, sizeof(IntStr));

for(unsigned int i=0; i<sizeof(int)*8; i+=bits){
	memset(st.data, 0, size*sizeof(int)+size*sizeof(IntStr)); memset(st.fpos, 255, base*2*sizeof(int));
    bit=(base-1)<<i;
    for(j=0; j<size; j++) {
        k=arr[j].i&bit; k=k>>i;
		if(st.fpos[k]==-1){ st.fpos[k]=j; } else { st.pos[st.lpos[k]]=j; }
		((IntStr*)st.data)[j].i=arr[j].i; ((IntStr*)st.data)[j].s-=arr[j].s; st.lpos[k]=j;
    }k=0;

	for(j=base-1; j>-1; j--){ p=st.fpos[j]; 
		if(p!=-1) while(1){ arr[k].s-=((IntStr*)st.data)[p].s; arr[k].i=((IntStr*)st.data)[p].i; p=st.pos[p]; k++; if(!p) break;
	}} if(k!=size) globalerror("SORT ERROR");
}

return ;
}
#endif


void myfastsort_VSiLine(VSiLine& isi){ VSi *arr=isi.data; int size=isi.size;
unsigned int base=256, bits=mfsbs(base-1), k, p; int j; 
mfsstack st; st.New(size, base, sizeof(VSi));

unsigned int maxs=0; int i;
for(i=0; i<size; i++){ if(maxs<arr[i].key.size()) maxs=arr[i].key; }

for(i=maxs-1; i>=0; i--){
	memset(st.data, 0, size*sizeof(int)+size*sizeof(VSi)); memset(st.fpos, 255, base*2*sizeof(int));
  //  bit=(base-1)<<i;
    for(j=0; j<size; j++){
        k=upd(arr[j].key[i]); // k=k>>i;
		if(st.fpos[k]==-1){ st.fpos[k]=j; } else { st.pos[st.lpos[k]]=j; }
		((VSi*)st.data)[j].key=arr[j].key; ((VSi*)st.data)[j].stt=arr[j].stt; st.lpos[k]=j;
    } k=size-1;

	for(j=base-1; j>=0; j--){ p=st.fpos[j]; 
		if(p!=-1) while(1){ arr[k].key=((VSi*)st.data)[p].key; arr[k].stt=((VSi*)st.data)[p].stt; p=st.pos[p]; k--;
		if(!p) break;
	}}
	if(k!=-1) globalerror("SORT ERROR");
}

return ;
}