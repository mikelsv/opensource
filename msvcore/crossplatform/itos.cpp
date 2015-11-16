template<int ITOS_DEFSZ>
class ItosX{
char data[ITOS_DEFSZ];
	MString dt;
	//bool GetBuffer(unsigned int s);
public:
	VString ret;

	VString GetBuffer(unsigned int s){
		if(s<ITOS_DEFSZ) ret.set((char*)data, s);
		else if(dt.size()>=s) ret.set(dt.rchar(), s); else { dt.Reserv(s); ret.set(dt.rchar(), s);}
		if(ret.data) ret.data[s]=0;
	return ret;
	}

//public:
	//ItosX();
	//ItosX(VString o);
	ItosX(){ data[0]=0; ret.data=(unsigned char*)data; ret.sz=0; }
	ItosX(VString o){
		if(!GetBuffer(o.sz)) return ;
		memcpy(ret, o, o); *(ret.rchar()+o.sz)=0; return ;
	}

	operator unsigned char*(){ return ret.data; }
	operator char*(){ return (char*)ret.data; }
	operator void*(){ return ret.data; }

	VString operator=(const VString &line){
		if(!GetBuffer(line.sz)) return ret;
		memcpy(ret, line, line); *(ret.rchar()+line.sz)=0; return ret;
	}

	//VString uptime(int tm){		
		//Reserv(prmf_uptime(0, 0, tm));
		//prmf_uptime(ret, ret, tm));
	//	return ret;
	//}

	//VString itos(int val, int radix=10, int null=0);
	// n 100%		  // re create on 10 and any radix and add tos
/*	VString itos(int val, int radix=10, int null=0){
		 if(null<1) null=1; int mins=val, bls=0;
		if (mins<0) {mins=mins*-1; bls++;}
		while(mins>0){ mins/=radix; bls++; }
		if(null>bls) {GetBuffer(null); memset(ret, '0', null); bls=null;} else GetBuffer(bls);
		bls--; if (val<0) {mins=1; val=val*-1; *(ret.rchar())='-';} else mins=0;
		int stp=1; int tmp;
		while(val>0){ tmp=val%radix;
		*(char*)(ret.rchar()+bls)=cb16[tmp];
		//if (tmp>=0 && tmp<=9) *(char*)(ret.rchar()+bls)=char(48+tmp);
		//else if (tmp>=10 && tmp<=33) *(char*)(ret.rchar()+bls)=char(55+tmp);
		val/=radix; bls--;
		} return ret;
	}

	//VString itos(unsigned int val, int radix=10, int null=0);
	VString itos(unsigned int val, int radix=10, int null=0){// n 100%
		if(null<1) null=1; unsigned int mins=val, bls=0;
		while(mins>0){ mins/=radix; bls++; }
		if((unsigned int)null>bls) {GetBuffer(null); memset(ret, '0', null); bls=null;} else GetBuffer(bls);
		bls--;
		int stp=1; int tmp;
		while(val>0){ tmp=val%radix;
		*(char*)(ret.rchar()+bls)=cb16[tmp];
		//if (tmp>=0 && tmp<=9) *(char*)(ret.rchar()+bls)=char(48+tmp);
		//else if (tmp>=10 && tmp<=33) *(char*)(ret.rchar()+bls)=char(55+tmp);
		val/=radix; bls--;
		} return ret;
	}
*/

	VString itos(int64 val, int radix = 10, int null = 0){
		Reserv(prmf_itos(0, 0, val, radix, null));
		prmf_itos(ret, ret, val, radix, null);
		return ret;
	}

	//VString dtos(double val, int ml=0, int radix=10, int null=1);
	VString dtos(double val, int ml=0, int radix=10, int null=1){// n 100%
		Reserv(prmf_dtos(0, 0, val));
		prmf_dtos(ret, ret, val);
		return ret;

		if(null<1) null=1; unsigned int valt; double vald; bool mins=0, point=0; int bls=0, mls=ml;
		if(val<0){ mins=1; val=val*-1; bls++; } valt=(unsigned int)val;
		
		// left
		if(valt==0) bls++; while(valt>0){ valt/=radix; bls++; }
		// right
		vald=val-(unsigned int)val; if(vald>0 && ml){ bls++; point=1; }
		while(vald>0 && ml){ vald*=radix; vald=vald-(unsigned int)vald; bls++; ml--; } mls=mls-ml;

		if(null>bls){ GetBuffer(null); memset(ret.data, '0', null); bls=null; } else GetBuffer(bls);
		bls--; if(mins){ mins=1; *(ret.rchar())='-'; } else mins=0;

		char*blp=ret.end()-1;
		//vald=val-(unsigned int)val; ml=mls; while(ml--) vald*=radix;

		int tmp; vald=val-(unsigned int)val;
		for(ml=0; ml<mls; ml++){
			vald*=radix; tmp=(unsigned int)vald;
			*(char*)(blp-mls+ml*2+1)=cb16[tmp];
			vald=vald-(unsigned int)vald;
			blp--;
		}
		if(point) *(char*)(blp--)='.';
		valt=(unsigned int)val;
		if(!valt) *(char*)(blp--)='0';
		while(valt){
			tmp=valt%radix; *(char*)(blp--)=cb16[tmp]; valt/=radix;
		}
		return ret;
	}


	//VString itos64(int64 val, int radix=10, int null=1);
	// n 100%		  // re create on 10 and any radix and add tos
	VString itos64(int64 val, int radix=10, int null=1){
	if(null<1) null=1; int bls=0; int64 mins=val;
	if (mins<0) {mins=mins*-1; bls++;}
	while(mins){ mins/=radix; bls++; }
	if(null>bls) {GetBuffer(null); memset(ret, '0', null); bls=null;} else GetBuffer(bls);
	bls--; if (val<0) {mins=1; val=val*-1; *(ret.rchar())='-';} else mins=0;
	int stp=1; int64 tmp;
	while(val){ tmp=val%radix; if(tmp<0) tmp*=-1;
	*(char*)(ret.rchar()+bls)=cb16[tmp];
	//if (tmp>=0 && tmp<=9) *(char*)(ret.rchar()+bls)=char(48+tmp);
	//else if (tmp>=10 && tmp<=33) *(char*)(ret.rchar()+bls)=char(55+tmp);
	val/=radix; bls--;
	} return ret;
	}

	//VString ipitos(unsigned int val);
	VString ipitos(unsigned int val){
		BYTE b=*(BYTE*)&val; int sz=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&val+1); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&val+2); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&val+3); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		sz+=3;
		GetBuffer(sz); char *p=ret;

		b=*((BYTE*)&val+3); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
		b=*((BYTE*)&val+2); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
		b=*((BYTE*)&val+1); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
		b=*(BYTE*)&val; if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; }// *p++='.';// sz++;
	return ret;
	}

	VString ip6itos(IP6_ADDRESS ip){ // 1:2:3:4:5:6:7:8
	unsigned char data[7+8*4];
	//MString ret; ret.Reserv(7+8*4);
	unsigned short *i=(unsigned short *)&ip, *t=i+8;
	unsigned char *l=data;

	int n=0, mn=0; unsigned short *mp=0;
	for(i; i<t-1; i++){
		if(!*i) n++; else n=0;
		if(n>mn){ mn=n; mp=i-n+1; }
	}

	i=(unsigned short *)&ip; 
	for(i; i<t; i++){
		if(l!=data){ *l=':'; l++; }
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
	GetBuffer((unsigned int)((char*)l-(char*)data));
	memcpy(ret, data, (char*)l-(char*)data);
	return ret;
}

	//VString ipitosftp(unsigned int ip, unsigned short port);
	VString ipitosftp(unsigned int val, unsigned short port){
		BYTE b=*(BYTE*)&val; int sz=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&val+1); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&val+2); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&val+3); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&port+0); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		b=*((BYTE*)&port+1); sz+=b<10 ? 1 : b<100 ? 2 : 3;
		sz+=5;
		GetBuffer(sz); char *p=ret;

		b=*((BYTE*)&val+3); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++=',';// sz++;
		b=*((BYTE*)&val+2); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++=',';// sz++;
		b=*((BYTE*)&val+1); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++=',';// sz++;
		b=*(BYTE*)&val; if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++=',';// sz++;
		b=*((BYTE*)&port+1); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++=',';// sz++;
		b=*((BYTE*)&port+0); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++=',';// sz++;
	return ret;
	}

	VString macitos(MACADDR_S mac){
	GetBuffer(17);
	char*l=ret; unsigned char c; 

	for(int i=0; i<6; i++){
		c=mac.mac[i]/16; *l=((c<10) ? c+48 : c+55); l++;
		c=mac.mac[i]%16; *l=((c<10) ? c+48 : c+55); l++;
		if(i<5) *l=':';
		l++;
	}

	return ret;
}

	VString macitos(int64 _mac){
	GetBuffer(17); 
	unsigned char *mac=(unsigned char*)&_mac;
	char*l=ret; unsigned char c; 

	for(int i=0; i<6; i++){
		c=*mac/16; *l=((c<10) ? c+48 : c+55); l++;
		c=*mac%16; *l=((c<10) ? c+48 : c+55); l++;
		if(i<5) *l=':';
		l++; mac++;
	}

	return ret;
}

	
	//MString ms();
	MString ms(){ return ret; }

	//VString set(VString o);
	VString set(VString o){
		if(!GetBuffer(o.size())) return VString();
		memcpy(ret, o, o); *(ret.rchar()+o.sz)=0; return ret;	
	}

	//operator VString();
	operator VString(){ return ret; }
//	operator const VString&(){ return ret; }
	operator int(){ return ret.size(); }


	//VString Add(VString o, VString t);
	//VString Add(VString o, VString t, VString th, VString f=VString(), VString fi=VString());

	VString Add(VString o, VString t){
		if(!GetBuffer(o.size()+t.size())) return VString();
		memcpy(ret, o, o); memcpy(ret.rchar()+o.size(), t, t); *(ret.rchar()+o.sz+t.sz)=0; return ret;	
	}

	VString Add(VString l1, VString l2, VString l3, VString l4=VString(), VString l5=VString(),
		VString l6=VString(), VString l7=VString(), VString l8=VString(), VString l9=VString(), VString l10=VString()){
		if(!GetBuffer(l1.sz+l2.sz+l3.sz+l4.sz+l5.sz+l6.sz+l7.sz+l8.sz+l9.sz+l10.sz)) return VString();
		unsigned char*l=ret.data;
		memcpy(l, l1, l1); l+=l1.sz;
		memcpy(l, l2, l2); l+=l2.sz;
		memcpy(l, l3, l3); l+=l3.sz;
		memcpy(l, l4, l4); l+=l4.sz;
		memcpy(l, l5, l5); l+=l5.sz;
		memcpy(l, l6, l6); l+=l6.sz;
		memcpy(l, l7, l7); l+=l7.sz;
		memcpy(l, l8, l8); l+=l8.sz;
		memcpy(l, l9, l9); l+=l9.sz;
		memcpy(l, l10, l10); l+=l10.sz;
		*l=0;
		return ret;	
	}

	VString Add(VString l1, VString l2, VString l3, VString l4, VString l5,
		VString l6, VString l7, VString l8, VString l9, VString l10
		, VString l11, VString l12=VString(), VString l13=VString(), VString l14=VString()
		, VString l15=VString(), VString l16=VString(), VString l17=VString(), VString l18=VString()
		, VString l19=VString(), VString l20=VString(), VString l21=VString(), VString l22=VString()){
		if(!GetBuffer(l1.sz+l2.sz+l3.sz+l4.sz+l5.sz+l6.sz+l7.sz+l8.sz+l9.sz+l10.sz+l11.sz+l12.sz+l13.sz+l14.sz+l15.sz+l16.sz+l17.sz+l18.sz+l19.sz+l20.sz+l21.sz+l22.sz)) return VString();
		unsigned char*l=ret.data;
		memcpy(l, l1, l1); l+=l1.sz;
		memcpy(l, l2, l2); l+=l2.sz;
		memcpy(l, l3, l3); l+=l3.sz;
		memcpy(l, l4, l4); l+=l4.sz;
		memcpy(l, l5, l5); l+=l5.sz;
		memcpy(l, l6, l6); l+=l6.sz;
		memcpy(l, l7, l7); l+=l7.sz;
		memcpy(l, l8, l8); l+=l8.sz;
		memcpy(l, l9, l9); l+=l9.sz;
		memcpy(l, l10, l10); l+=l10.sz;
		memcpy(l, l11, l11); l+=l11.sz;
		memcpy(l, l12, l12); l+=l12.sz;
		memcpy(l, l13, l13); l+=l13.sz;
		memcpy(l, l14, l14); l+=l14.sz;
		memcpy(l, l15, l15); l+=l15.sz;
		memcpy(l, l16, l16); l+=l16.sz;
		memcpy(l, l17, l17); l+=l17.sz;
		memcpy(l, l18, l18); l+=l18.sz;
		memcpy(l, l19, l19); l+=l19.sz;
		memcpy(l, l20, l20); l+=l20.sz;
		memcpy(l, l21, l21); l+=l21.sz;
		memcpy(l, l22, l22); l+=l22.sz;
		*l=0;
		return ret;	
	}

	VString AddR(VString &l1, VString &l2 = _vstringnull_, VString &l3 = _vstringnull_, VString &l4 = _vstringnull_, VString &l5 = _vstringnull_, VString &l6 = _vstringnull_, VString &l7 = _vstringnull_, VString &l8 = _vstringnull_, VString &l9 = _vstringnull_){
		if(!GetBuffer(l1.sz+l2.sz+l3.sz+l4.sz+l5.sz+l6.sz+l7.sz+l8.sz+l9.sz)) return VString();

		unsigned char*l=ret.data;
		memcpy(l, l1, l1); l1.data=l; l1.sz=l1.sz; l+=l1.sz;
		memcpy(l, l2, l2); l2.data=l; l2.sz=l2.sz; l+=l2.sz;
		memcpy(l, l3, l3); l3.data=l; l3.sz=l3.sz; l+=l3.sz;
		memcpy(l, l4, l4); l4.data=l; l4.sz=l4.sz; l+=l4.sz;
		memcpy(l, l5, l5); l5.data=l; l5.sz=l5.sz; l+=l5.sz;
		memcpy(l, l6, l6); l6.data=l; l6.sz=l6.sz; l+=l6.sz;
		memcpy(l, l7, l7); l7.data=l; l7.sz=l7.sz; l+=l7.sz;
		memcpy(l, l8, l8); l8.data=l; l8.sz=l8.sz; l+=l8.sz;
		memcpy(l, l9, l9); l9.data=l; l9.sz=l9.sz; l+=l9.sz;
		
		*l = 0;
		return ret;
	}

	VString Format(VString line, MTEVA v1=MTEVA(), MTEVA v2=MTEVA(), MTEVA v3=MTEVA(), MTEVA v4=MTEVA(), MTEVA v5=MTEVA(),
		MTEVA v6=MTEVA(), MTEVA v7=MTEVA(), MTEVA v8=MTEVA(), MTEVA v9=MTEVA(), MTEVA v10=MTEVA()){
		unsigned char*ln=line.data, *to=line.endu(), *ls=0; int ll=0;
		MTEVA _z, *a[11]={&v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &v10, &_z};
		MTEVA *l=a[ll++], *tl=l+10; ItosX<128> it;
		//unsigned char*sz=0; print(itos(&v2-&v1), " ");
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
				else if(*ln=='d'){ it.itos64(*l, 10, p1); ls+=it; }				
				else if(*ln=='h'){ it.itos64(*l, 16, p1); ls+=it; }
				else if(*ln=='f'){ if(!p2) p1=99; it.dtos(*l, p1); ls+=it; }
				else if(*ln=='s'){ VString v=*l; ls+=v; }
				else if(ln+3<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='!'){ ln+=2; it.ipitos(*l); ls+=it; }
				else if(ln+4<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='6' && *(ln+3)=='!'){ ln+=3; it.ip6itos(*l); ls+=it; }
//				else if(ln+4<=to && *ln=='m' && *(ln+1)=='a' && *(ln+2)=='c' && *(ln+3)=='!'){ ln+=3; ls+=17; }
				else{
					ls ++;
					continue;
				}
				//else if(*ln=='i'){ it.itos64(*l, 10, p1); ls+=it; }
				
				l=a[ll++]; ln++;
			}
			else{ ls++; ln++; }
		}

		if(!GetBuffer((unsigned int)(ls-(unsigned char*)0))) return VString();
		ll=0; l=a[ll++]; ln=line.data; ls=ret;
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
				else if(*ln=='d'){ it.itos64(*l, 10, p1); memcpy(ls, it.ret, it); ls+=it; }
				else if(*ln=='h'){ it.itos64(*l, 16, p1); memcpy(ls, it.ret, it); ls+=it; }
				else if(*ln=='f'){ if(!p2) p1=99; it.dtos(*l, p1); memcpy(ls, it.ret, it); ls+=it; }
				else if(*ln=='s'){ VString v=*l; memcpy(ls, v, v); ls+=v; }
				//else if(cmp((char*)ln, "ip!", 3)){ ln+=2; it.ipitos(*l); memcpy(ls, it.ret, it); ls+=it; }
				else if(ln+3<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='!'){ ln+=2; it.ipitos(*l); memcpy(ls, it.ret, it); ls+=it; }
				else if(ln+4<=to && *ln=='i' && *(ln+1)=='p' && *(ln+2)=='6' && *(ln+3)=='!'){ ln+=3; it.ip6itos(*l); memcpy(ls, it.ret, it); ls+=it; }
//				else if(ln+4<=to && *ln=='m' && *(ln+1)=='a' && *(ln+2)=='c' && *(ln+3)=='!'){ ln+=3; it.macitos(*l); memcpy(ls, it.ret, it); ls+=17; }
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

		return ret;
	}


	//int Reserv(int sz, int p=0);
	int Reserv(int sz, int p=0){
		if(!GetBuffer(sz)) return 0;
		memset(ret, 0, ret);
		return sz;
	}

	void Clean(){ ret.Clean(); return ; }
	
	// text +, -, *, /, &. no.
};

typedef ItosX<128> Itos;


template<int ITOS_DEFSZ>
class ItosUX{
	unsigned short data[ITOS_DEFSZ];
	MString dt;
public:
	UVString ret;

// constr
	ItosUX(){ }
	ItosUX(unsigned short *s, unsigned int sz){
		if(!GetBuffer(sz)) return ;
		memcpy(ret, s, sz); return ;
	}

	operator unsigned short*(){ return ret.data; }


// buffer
	UVString GetBuffer(unsigned int s){
		if(s<ITOS_DEFSZ){ ret.data=data; ret.sz=s; }
		//else if(dt.size()>=s*2){ ret=(unsigned short*)dt.rchar(); retsz=s; }
		else { dt.Reserv(s*2+1); ret.data=(unsigned short*)dt.rchar(); ret.sz=s; }
		if(ret.data) ret.data[s]=0;
	return ret;
	}

	int Reserv(int sz, int p=0){
		if(!GetBuffer(sz)) return 0;
		memset(ret, 0, ret.sz*2);
		return sz;
	}

	unsigned short operator [](unsigned int i){
		if(i>=ret.sz) return 0;
		return ret[i];
	}

// set
	void set(unsigned short *s, unsigned int sz){
		if(!GetBuffer(sz)) return ;
		memcpy(ret, s, sz*2); return ;
	}

	void set(VString ln){
		if(!GetBuffer(ln)) return ;
		unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar();
		unsigned short *r=ret;
		for(line; line<to; line++){
			*r=ctos(*line); r++;
		}
	}

	void setutf(VString ln){
		unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar(); unsigned int r; int lss=0;
		for(line; line<to; line++){
			if(*line<128) lss++;
			else if((*line&224)==192){ lss++; line++; } // twobyte
			else if((*line&240)==224){ line+=2; } // three byte
			else if((*line&248)==240){ line+=3; } // four byte
		}

        if(!GetBuffer(lss)) return ;
		unsigned short *rt=ret;
		line=ln.uchar();

		for(line; line<to; line++){
			if(*line<128){ *rt=*line; rt++; }
			else if((*line&224)==192){
				r=((*line&31)<<6)+(*(line+1)&63);
				*rt=r; rt++;
			} // twobyte
			else if((*line&240)==224){ line+=2; } // three byte
			else if((*line&248)==240){ line+=3; } // four byte
		}
	}

	int size(){
		return ret;
	}

	MString stoc(){
		return ::stoc(ret.data, ret.sz);
	}

};

typedef ItosUX<32> ItosU;
