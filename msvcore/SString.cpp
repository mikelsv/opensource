//class SString : public VString{
//public:

template<int stacksize>
SStringX<stacksize>::SStringX(){ data = 0; sz = 0; }

template<int stacksize>
SStringX<stacksize>::~SStringX(){ data = 0; sz = 0; }


template<int stacksize>
void SStringX<stacksize>::Init(){ StringDel(data); data = 0; sz = 0; }

template<int stacksize>
void SStringX<stacksize>::StringNew(const void *line){
	if(!sz){
		data = 0;
		return ;
	}

	if(sz + 1 <= stacksize)
		data = sdata;
	else
		data = mdata.Reserv(sz);

	if(!data){
		sz = 0;
		return ;
	}

	if(line)
		memcpy(data, line, sz);

	data[sz] = 0;

	return ;
}

template<int stacksize>
void SStringX<stacksize>::StringDel(unsigned char *line){
	if(data != line)
		return ;

	Clean();

	if(mdata)
		mdata.Clean();

	return ;
}

	// --------------------------------------------------- Create object  Создание обьекта

// int 
template<int stacksize>
SStringX<stacksize>::SStringX(const int code){ sz = 1; StringNew(&code); return ; }

template<int stacksize>
SStringX<stacksize>::SStringX(const unsigned int code){ sz = 1; StringNew(&code); return ; }

template<int stacksize>
SStringX<stacksize>::SStringX(const unsigned int code, const unsigned int size){ sz = size; StringNew(0); memset(data, code, sz); return ; }

// char
template<int stacksize>
SStringX<stacksize>::SStringX(const char string){ sz = sizeof(string); StringNew(&string); return ; }

template<int stacksize>
SStringX<stacksize>::SStringX(const char *line){ if(line){ sz = strlen((const char*)line); StringNew(line); } else { data = 0; sz = 0; } }

template<int stacksize>
SStringX<stacksize>::SStringX(const unsigned char *line){ if(line){ sz = strlen((const char*)line); StringNew(line); } else { data = 0; sz = 0; } }

template<int stacksize>
SStringX<stacksize>::SStringX(const char *line, const unsigned int s){ sz = s; StringNew(line); }

template<int stacksize>
SStringX<stacksize>::SStringX(const unsigned char *line, const unsigned int s){ sz = s; StringNew(line); }

template<int stacksize>
SStringX<stacksize>::SStringX(const short string){ sz = sizeof(string); StringNew(&string); return ; }

// short
template<int stacksize>
SStringX<stacksize>::SStringX(const short *string, int size){
	if(!size){
		data = 0;
		return ;
	}

	MString ret = stoc((unsigned short*)string, size);
	sz = ret; StringNew(ret);

	return ;
}

// string
template<int stacksize>
SStringX<stacksize>::SStringX(const VString &line){ sz = line.sz; StringNew(line); }

template<int stacksize>
SStringX<stacksize>::SStringX(const SStringX &line){ sz = line.sz; StringNew(line); }
//SString::SString(const MString &line){ sz = line.sz; StringNew(line); }

	// --------------------------------------------------- Set	Установка

template<int stacksize>
const SStringX<stacksize>& SStringX<stacksize>::operator=(const char string){ unsigned char* odata = data; sz = 1; StringNew(&string); StringDel(odata); return *this; }

template<int stacksize>
const SStringX<stacksize>& SStringX<stacksize>::operator=(const char* line){ unsigned char *odata = data; sz = line ? strlen((const char*)line) : 0; StringNew(line); StringDel(odata); return *this; }

template<int stacksize>
const SStringX<stacksize>& SStringX<stacksize>::operator=(const unsigned char* line){ unsigned char *odata=data; sz = line ? strlen((const char*)line) : 0; StringNew(line); StringDel(odata); return *this; }

template<int stacksize>
const SStringX<stacksize>& SStringX<stacksize>::operator=(const VString& line){ unsigned char *odata = data; sz = line; StringNew(line); StringDel(odata); return *this; }
//const SString& SString::operator=(const MString& line){ unsigned char *odata=data; sz=line; MStringNew(line); _deln(odata); return *this; }

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::set(const VString line){ unsigned char *odata = data; sz = line.sz; StringNew(line); StringDel(odata); return *this; }

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::set(const char* line, const unsigned int size){ unsigned char *odata = data; sz = size;  StringNew(line); StringDel(odata); return *this; }

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::set(const unsigned char* line, const unsigned int size){ unsigned char *odata = data; sz = size;  StringNew(line); StringDel(odata); return *this; }

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::setu(const unsigned char* line, const unsigned int size){ unsigned char *odata = data; sz = size; StringNew(line); StringDel(odata); return *this; }

	// --------------------------------------------------- Add  Добавление	

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::Add(VString l1, VString l2, VString l3, VString l4, VString l5){
	MSTRING_TEMPALTE_ADD5;
}

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9){
	MSTRING_TEMPALTE_ADD9;
}

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9,
	VString l10, VString l11, VString l12, VString l13, VString l14, VString l15, VString l16, VString l17, VString l18, VString l19){
		MSTRING_TEMPALTE_ADD19;
}

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::AddR(VString &l1, VString &l2, VString &l3, VString &l4, VString &l5, VString &l6, VString &l7, VString &l8, VString &l9){
	MSTRING_TEMPALTE_ADDR9;
}

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::Format(VString line, MTEVA v1, MTEVA v2, MTEVA v3, MTEVA v4, MTEVA v5,
		MTEVA v6, MTEVA v7, MTEVA v8, MTEVA v9, MTEVA v10){
		unsigned char*ln=line.data, *to=line.endu(), *ls=0; int ll=0;
		MTEVA _z, *a[11]={&v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &v10, &_z};
		MTEVA *l = a[ll++], *tl = l + 10;
		
		SStringX<128> it;
		int p1, p2;

		while(ln<to){
			if(*ln=='%' && ln+1<to){ ln++;
				if(ll>10){ ln++; continue; }

				p1=0; p2=0;
				while(ln<to && (*ln>='0' && *ln<='9' || *ln=='.')){
					if(*ln!='.'){
						p1=p1*10+*ln-'0'; p2=1;
					}
					ln++;
				}

				if(*ln=='%'){ ls++; ln++; continue; }
				else if(*ln=='d'){ it.itos(*l, 10, p1); ls+=it; }				
				else if(*ln=='h'){ it.itos(*l, 16, p1); ls+=it; }
				else if(*ln=='f'){ if(!p2) p1=99; it.dtos(*l, p1); ls+=it; }
				else if(*ln=='s'){ VString v=*l; ls+=v; }
				else if(ln+3<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='!'){ ln+=2; it.ipitos(*l); ls+=it; }
				else if(ln+4<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='6' && *(ln+3)=='!'){ ln+=3; it.ip6itos(*l); ls+=it; }
				else if(ln+4<=to && *ln=='m' && *(ln+1)=='a' && *(ln+2)=='c' && *(ln+3)=='!'){ ln+=3; ls+=17; }
				else{
					ls ++;
					continue;
				}
				//else if(*ln=='i'){ it.itos64(*l, 10, p1); ls+=it; }
				
				l=a[ll++]; ln++;
			}
			else{ ls++; ln++; }
		}

		if(!Reserv((unsigned int)(ls-(unsigned char*)0)))
			return *this;

		ll=0; l=a[ll++]; ln=line.data; ls = data;
		while(ln<to){
			if(*ln=='%' && ln+1<to){ ln++;
				if(ll>10){ ln++; continue; }

				p1=0; p2=0;
				while(ln<to && (*ln>='0' && *ln<='9' || *ln=='.')){
					if(*ln!='.'){
						p1=p1*10+*ln-'0'; p2=1;
					}
					ln++;
				}

				if(*ln=='%'){ *ls++ = '%'; ln++; continue; }
				else if(*ln=='d'){ it.itos(*l, 10, p1); memcpy(ls, it, it); ls+=it; }
				else if(*ln=='h'){ it.itos(*l, 16, p1); memcpy(ls, it, it); ls+=it; }
				else if(*ln=='f'){ if(!p2) p1=99; it.dtos(*l, p1); memcpy(ls, it, it); ls+=it; }
				else if(*ln=='s'){ VString v=*l; memcpy(ls, v, v); ls+=v; }
				//else if(cmp((char*)ln, "ip!", 3)){ ln+=2; it.ipitos(*l); memcpy(ls, it.ret, it); ls+=it; }
				else if(ln+3<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='!'){ ln+=2; it.ipitos(*l); memcpy(ls, it, it); ls+=it; }
				else if(ln+4<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='6' && *(ln+3)=='!'){ ln+=3; it.ip6itos(*l); memcpy(ls, it, it); ls+=it; }
//				else if(ln+4<=to && *ln=='m' && *(ln+1)=='a' && *(ln+2)=='c' && *(ln+3)=='!'){ ln+=3; it.macitos(*l); memcpy(ls, it, it); ls+=17; }
				//else if(*ln=='i'){ it.itos64(*l, 10, p1); memcpy(ls, it.ret, it); ls+=it; }
				else{
					*ls = '%';
					ls ++;
					continue;
				}

				l=a[ll++]; ln++;
			}
			else *ls++=*ln++;
		}

		return *this;
	}


template<int stacksize>
VString SStringX<stacksize>::itos(int64 val, int radix, int null){
	Reserv(prmf_itos(0, 0, val, radix, null));
	prmf_itos(data, sz, val, radix, null);
	return *this;
}

template<int stacksize>
VString SStringX<stacksize>::dtos(double val, int ml, int radix, int null){// n 100%
	Reserv(prmf_dtos(0, 0, val));
	prmf_dtos(data, sz, val);
	return *this;
}

template<int stacksize>
VString SStringX<stacksize>::ipitos(unsigned int val){
	BYTE b=*(BYTE*)&val; int sz=b<10 ? 1 : b<100 ? 2 : 3;
	b=*((BYTE*)&val+1); sz+=b<10 ? 1 : b<100 ? 2 : 3;
	b=*((BYTE*)&val+2); sz+=b<10 ? 1 : b<100 ? 2 : 3;
	b=*((BYTE*)&val+3); sz+=b<10 ? 1 : b<100 ? 2 : 3;
	sz+=3;
	Reserv(sz); unsigned char *p = data;

	b=*((BYTE*)&val+3); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
	b=*((BYTE*)&val+2); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
	b=*((BYTE*)&val+1); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
	b=*(BYTE*)&val; if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; }// *p++='.';// sz++;
	return *this;
}

template<int stacksize>
VString SStringX<stacksize>::ip6itos(IP6_ADDRESS ip){ // 1:2:3:4:5:6:7:8
	unsigned char bdata[7+8*4];
	//MString ret; ret.Reserv(7+8*4);
	unsigned short *i=(unsigned short *)&ip, *t=i+8;
	unsigned char *l=bdata;

	int n=0, mn=0; unsigned short *mp=0;
	for(i; i<t-1; i++){
		if(!*i) n++; else n=0;
		if(n>mn){ mn=n; mp=i-n+1; }
	}

	i=(unsigned short *)&ip; 
	for(i; i<t; i++){
		if(l!=bdata){ *l=':'; l++; }
		if(mp==i){
			if(mp==(unsigned short *)&ip){ *l++=':'; }
			i+=mn-1; continue;
		}

		unsigned char b, s=0;
		b=*i>>12;	if(b || s){ if(b<10) *l++='0'+b; else *l++=87+b; s=1; }
		b=*i>>8&15; if(b || s){ if(b<10) *l++='0'+b; else *l++=87+b; s=1; }
		b=*i>>4&15;	if(b || s){	if(b<10) *l++='0'+b; else *l++=87+b; s=1; }
		b=*i&15;	if(b<10) *l++='0'+b; else *l++=87+b;
	}
	Reserv((unsigned int)((char*)l-(char*)bdata));
	memcpy(data, bdata, (char*)l-(char*)bdata);
	return *this;
}

template<int stacksize>
VString SStringX<stacksize>::macitos(MACADDR_S mac){
	Reserv(17);
	char*l = data; unsigned char c; 

	for(int i=0; i<6; i++){
		c=mac.mac[i]/16; *l=((c<10) ? c+48 : c+55); l++;
		c=mac.mac[i]%16; *l=((c<10) ? c+48 : c+55); l++;
		if(i<5) *l=':';
		l++;
	}

	return *this;
}

//---------------------------------------------------- Move   Перемещение

//---------------------------------------------------- Operations  Операции

template<int stacksize>
SStringX<stacksize>& SStringX<stacksize>::Reserv(int size, int p){
	if(size == sz)
		return *this;
	
	if(size <= 0){
		StringDel(data);
		data = 0;
		sz = 0;
		return *this;
	}

	unsigned char *odata = data;
	int s = sz; sz = size;

	StringNew(0);
	int m = minel(s, size);
	memcpy(data, odata, m);
	memset(data + m, p, sz - m);
	data[size] = 0;

	StringDel(odata);
	return *this;
}

template<int stacksize>
void SStringX<stacksize>::RClean(int sz, int p){
	Reserv(sz);
	CleanData(p);
}

template<int stacksize>
void SStringX<stacksize>::CleanData(int p){
	memset(data, p, sz);
}

template<int stacksize>
void SStringX<stacksize>::Clean(){ Init(); }

//};
