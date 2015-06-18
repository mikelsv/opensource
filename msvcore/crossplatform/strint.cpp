//MSL Int Library + Code library: char, short, utf8
/*
int stoi(char *line, unsigned int size){
	if(!line) return 0; if(!size) size=(unsigned int)strlen(line); if(!size) return 0;

	char *to=line+size; int ret=0; bool min=0; 

	while(line<to){
		if(*line=='-'){ min=1; line++; break; }
		else if(*line>='0' && *line<='9') break;
		line++;
	}

	while(line<to){
		if(*line>='0' && *line<='9') ret=ret*10+(*line)-'0';
		else break;
		line++;
	}

	if(min) ret*=-1;
	return ret;
}*/

int64 stoi64(VString val, int radix){ return stoi64((char*)val.data, val.sz, radix); }
int stoih(const VString &val) {return stoi(val, val, 16);}
int stoi(char *line, unsigned int size, int radix){ return (int)stoi64(line, size, radix); }
unsigned int stoui(char *line, unsigned int size, int radix){ return (unsigned int)stoi64(line, size, radix); }

int64 stoi64(char*line, unsigned int size, int radix){
int64 ret=0; if(!line) return 0; if(!size) size=(unsigned int)strlen(line);
if(!size) return 0;
char *to=line+size; bool min=0; int t;

while(line<to){
if(*line=='-'){min=1; line++; break;}
if(*line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z') break;
line++;
}
while(line<to){
	if(*line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z'){
		t=((*line>='0' && *line<='9') ? *line-48 : (*line>='a' && *line<='z') ? *line-'a'+10 : *line-'A'+10 );
		if(t>=radix) break;
		ret=ret*radix+t;
	}
else break; line++;
}

if(min) ret*=-1;
return ret;
}

double stod(VString val, int radix){ return stod((char*)val.data, val.sz, radix); }

double stod(char*line, unsigned int size, int radix){
	char *ln = line, *to = line + size;

	for(ln; ln < to; ln++){
		if(*ln == '.')
			break;
	}

	double ret = stoi64(line, ln - line, radix);
	
	if(ln + 1 < to){
		double pw = pow((double)radix, to - ln - 1);
		double dret = stoi64(ln + 1, to - ln - 1, radix);
		ret = ret + dret / pw;
	}

	return ret;

#ifdef NOOOOOO
double ret=0; if(!line) return 0; if(!size) size=(unsigned int)strlen(line);
if(!size) return 0;
char *to=line+size; bool min=0, dd=0; int d=0;

while(line<to){
if(*line=='-'){min=1; line++; break;}
if(*line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z') break;
line++;
}
while(line<to){ if(dd) d++;
	if(*line=='.'){ dd=1; }
	else
	if(*line>='0' && *line<='9' || *line>='a' && *line<='z' || *line>='A' && *line<='Z')
		ret=ret*radix+((*line>='0' && *line<='9') ? *line-48 : (*line>='a' && *line<='z') ? *line-'a'+10 : *line-'A'+10 );
else break; line++;
}

while(d){ ret/=radix; d--;}

if(min) ret*=-1;
return ret;
#endif
}


MString itos(int64 val, int radix, int null){
	MString ret; ret.Reserv(prmf_itos(0, 0, val, radix, null)); prmf_itos(ret, ret, val, radix, null); return ret;
}

MString htob(VString line){
	MString ret; ret.Reserv(prmf_htob(0, 0, line, line)); prmf_htob(ret, ret, line, line); return ret;
}

MString btoh(VString line){
	MString ret; ret.Reserv(prmf_btoh(0, 0, line, line)); prmf_btoh(ret, ret, line, line); return ret;
}

MString itob(unsigned int val, int null){ // recreate it
	MString ret;
	int stp=1, tmp;

	while(val>0){
		tmp=val%256; val/=256;
		ret+=MString(tmp);	//pows -1
	}
	if (null>0 && (unsigned int)null>ret.size()){ MString s; s.RClean(null-ret.size(),0); ret+=s; }
	return ret;
}


// SHORT to CHAR
unsigned char stoc(const unsigned short s){
	if(s>1039 && s<1104) return char(s-1040+192);
	if(s==1025) return 168; if(s==1105) return 184; // Ё, ё.
	return (unsigned char)s;
} // short to char

MString stoc(const unsigned short *s, const int sz){	// short line to MString
	MString ret; ret.Reserv(sz); char *r=ret;
	for(int i=0; i<sz; i++){
		*r=stoc(*s); s++; r++;
	}
	return ret;
}

char stocdos(const short s){
	if(s<128) return (char)s;
	if (s>1039 && s<1072) return char(s-1040+128);
	if (s>=1072 && s<1072+16) return char(s-1040+160-32);
	if (s>=1072+16 && s<1104) return char(s-1040+224-32-16);	
	if (s==1105) return 241;
	if (s==1025) return 240;
	return -1;
} // short to char dos

char stoclin(const short s){
	if (s>1039 && s<1072) return char(s-1040+128);
	if (s>=1072 && s<1104) return char(s-1040+224-32);
	//if (s>=1072+16 && s<1104) return char(s-1040+224-32-16);	
	return -1;
} // short to char linux

MString stocdos(const short *s, const int sz){	// short line to MString
	MString ret; ret.Reserv(sz); char *r=ret;
	for(int i=0; i<sz; i++){
		*r=stocdos(*s); s++; r++;
	}
	return ret; 
}

// CHAR to SHORT
unsigned short ctos(const unsigned char s){
	if(s>=192/* && s<255*/) return (unsigned short)s+1040-192; // if(v>='а' && v<'я') return v-'а'+32+1040;
	if(s==168) return 1025; if(s==184) return 1105; // Ё, ё
	return s;
} // char to short

MString ctos(const unsigned char*s, const int sz){	// short line to MString
	MString ret; ret.Reserv(sz*2); char *r=ret;
	for(int i=0; i<sz; i++){
		*(unsigned short*)r=ctos(*s); s++; r+=2;
	}
	return ret;
}

MString ctos(VString line){ return ctos(line, line); }

unsigned short dosctos(unsigned char v){ // dos char to short
	if(v>=128 && v<=159) return v-128+1040; // if(v>='а' && v<'я') return v-'а'+32+1040;
	else if(v>=160 && v<=175) return v-160+1040+32;
	else if(v>=224 && v<=239) return v-224+1040+32+16;
	return v;
}

// UTF to SHORT
unsigned short utftos(unsigned short v){
	return (((v>>8)&31)<<6)+v&63;
}

/////////////////////////////////////////////////////////////////////////////////////
MString utftos(const VString ln){
	unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar(); int lss=0;
	for(line; line<to; line++){
		if(*line<128) lss++;
		else if((*line&224)==192){ lss++; line++; } // twobyte
		else if((*line&240)==224){ line+=2; } // three byte
		else if((*line&248)==240){ line+=3; } // four byte
	}

	MString _ls; _ls.Reserv(lss*2); unsigned short *ls=(unsigned short *)_ls.rchar(); to=ln.uchar()+ln.size(); line=ln.uchar(); lss=0; 
	for(line; line<to; line++){
		if(*line<128){ *ls++=*line; }
		else if((*line&224)==192){
			*ls++=((*line&31)<<6)+(*(line+1)&63);
			line++;
		} // twobyte
		else if((*line&240)==224){ line+=2; } // three byte
		else if((*line&248)==240){ line+=3; } // four byte
	}
	return _ls;
}



// SHORT to UTF
unsigned short stoutf(unsigned short v){ // short to utf
	if(v<128) return v;
	return ((192)+((v>>6)&31))+256*((128)+(v&63));
}

MString stoutf(VString line){ unsigned short *p=(unsigned short*)line.data, *to=p+line.sz/2; int ns=0;
	while(p<to){ // short to utf
		if(*p<128){ ns++; }
		else{ ns+=2; }
		p++;
	}

	MString ret; ret.Reserv(ns); 
	unsigned char *w=ret;
	p=(unsigned short*)line.data;
	
	while(p<to){ // short to utf
		if(*p<128){ *w++=(unsigned char)*p; }
		else{
			*(unsigned short*)w=((192)+((*p>>6)&31))+256*((128)+(*p&63));
			w+=2;
		}
		p++;
	}
	return ret;
}

// UTF to CHAR

MString utftotr(const MString ln);
/////////////////////////////////////////////////////////////////////////////////////
MString utftoc(const VString ln, int sys){ if(sys==4) return utftotr(ln);
	unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar(); unsigned int r; int lss=0;
	for(line; line<to; line++){
		if(*line<128) lss++;
		else if((*line&224)==192){ lss++; line++; } // twobyte
		else if((*line&240)==224){ line+=2; } // three byte
		else if((*line&248)==240){ line+=3; } // four byte
	}

	MString _ls; _ls.Reserv(lss); char *ls=_ls; to=ln.uchar()+ln.size(); line=ln.uchar(); lss=0; 
	for(line; line<to; line++){
		if(*line<128){ *(ls+lss)=*line; lss++; }
		else if((*line&224)==192){
			r=((*line&31)<<6)+(*(line+1)&63);
			if(!sys) *(ls+lss)=stoc(r);
			else if(sys==1) *(ls+lss)=stocdos(r);
			else *(ls+lss)=stoclin(r);			
			line++; lss++;
		} // twobyte
		else if((*line&240)==224){ line+=2; } // three byte
		else if((*line&248)==240){ line+=3; } // four byte
	}
	return _ls;
}

//														'zh'																					// 'ch' 'sh'							'yu'	'ya'
unsigned short _lntrans[]={'a', 'b', 'v', 'g', 'd', 'e', 31336, 'z', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'f', 'h', 'c', 25448, 29544, 'w', '\'', 'y', '\'', 'e', 31093, 31073};

/////////////////////////////////////////////////////////////////////////////////////
MString utftotr(const MString ln){
	unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar(); unsigned int r; int lss=0;
	for (line; line<to; line++){
		if(*line<128) lss++;
		else if((*line&224)==192){ r=((*line&31)<<6)+(*(line+1)&63); r=stoc(r);
		if(r>=224 && r<=255) {if(_lntrans[r-224]<256) lss++; else lss+=2; }
		else if(r>=192 && r<=223) {if(_lntrans[r-223]<256) lss++; else lss+=2; }
		else lss++; line++;  } // twobyte
		else if((*line&240)==224) { line+=2; } // three byte
		else if((*line&248)==240) { line+=3;} // four byte
	}

	MString _ls; _ls.Reserv(lss); char *ls=_ls; to=ln.uchar()+ln.size(); line=ln.uchar();lss=0;

	for (line; line<to; line++){
		if(*line<128) { *(ls+lss)=*line; lss++;}
		else if((*line&224)==192){r=((*line&31)<<6)+(*(line+1)&63); r=stoc(r);
		if(r>=224 && r<=255){
			if(_lntrans[r-224]<256){ *(ls+lss)=(unsigned char)_lntrans[r-224]; lss++;}
			else  {*(ls+lss)=_lntrans[r-224]/256; lss++; *(ls+lss)=_lntrans[r-224]%256; lss++;}
		}
		else if(r>=192 && r<=223){
			if(_lntrans[r-192]<256){ *(ls+lss)=upd((unsigned char)_lntrans[r-192]); lss++;}
			else  {*(ls+lss)=upd(_lntrans[r-192]/256); lss++; *(ls+lss)=_lntrans[r-192]%256; lss++;}
		}
		else *(ls+lss)=stoc(r); line++;} // twobyte
		else if((*line&240)==224){ line+=2; } // three byte
		else if((*line&248)==240){ line+=3;} // four byte
	}
	return _ls;
}

// CHAR to UTF

MString ctoutf(const VString ln){
	unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar(); int lss=0;
	for(line; line<to; line++){
		if(ctos(*line)<128) lss++;
		else{ lss+=2; } // twobyte
	}

	MString _ls; _ls.Reserv(lss); char *ls=_ls; to=ln.uchar()+ln.size(); line=ln.uchar();lss=0; 
	for (line; line<to; line++){
		if(ctos(*line)<128){ *(ls+lss)=(unsigned char)ctos(*line); lss++; }
		else { *(unsigned short*)(ls+lss)=stoutf(ctos(*line)); lss+=2; } // twobyte
	}
	return _ls;
}

// WIN to DOS

MString ctodos(const VString ln){
	unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar();
	MString _ls; _ls.Reserv(ln); char *ls=_ls; to=ln.uchar()+ln.size(); line=ln.uchar();
	
	for (line; line<to; line++){
		*ls++=stocdos(ctos(*line));
	}
	return _ls;
}



// Base 64 help functions
BYTE cbgetpos(unsigned char c){ if(c>='A' && c<='Z') return c-'A';
	if(c>='a' && c<='z') return c-'a'+26; if(c>='0' && c<='9') return c-'0'+52;
	if(c=='+') return 62; if(c=='/') return 63; return 0;
}

BYTE cbgetpost(unsigned char c){ if(c>='A' && c<='Z') return 1;
	if(c>='a' && c<='z') return 1; if(c>='0' && c<='9') return 1;
	if(c=='+') return 1; if(c=='/') return 1; return 0;
}

// Base 64
MString Base64::btos(const VString line){
		MString ret; ret.Reserv(line.size()*6/8+2, 0);
		unsigned char*ln=line.uchar(), *to=ln+line.size(), *r=ret.uchar(); int i=0; unsigned short s=0;
		while(to>ln && *(to-1)=='=') to--;

		while(ln<to){
			//*((short*)(r+i/8))+= GetBpos(*ln) << (i%8); i+=6; ln++;
			s+=cbgetpos(*ln) << (( (i%8<=6) ? 8-6-(i%8) : 0)+8);
			if(i%8+6>=8) {*(r+i/8)=*((char*)&s+1); s=s<<8;} i+=6; ln++;
        }
		ret.sz = i / 8;
		//ret.resize(i/8);
		return ret;
	}

	MString Base64::stob(const VString line){
		MString ret; ret.Reserv(line.size()*8/6+3, 0);
		unsigned char*l=line.uchar(), *ln=l, *to=ln+line.size(), *r=ret.uchar(); int i=0; unsigned short s=0, t;

		while(ln<to){
			//*r+= ((*(unsigned short*)ln) >> (i%8))&63;
			//s+= (((*(unsigned short*)ln) >> (i%8))&63) << ( (i%8<=6) ? 8-6-(i%8) : 0)+8;

			*((char*)&t+1)=*ln; *((char*)&t)=*(ln+1);
			*r= (t >> (8 + ((i%8<=6) ? 8-6-(i%8) : 0))&63); //&(63<<8) ;//<< ( +8
			//*r=*((char*)&s+1); s=s<<8;
			if(i%8+6>=8) ln++; r++; i+=6;
		} //if(i%8) r--; if(i%8) to++;

		to=r; r=ret.uchar();
		while(r<to){ *r=cb64[*r]; r++; }
		if(i%8==4) {*r='='; r++; *r='='; r++; } if(i%8==2) {*r='='; r++;}

		//ret.resize(r-ret.uchar());
		ret.sz = r-ret.uchar();
		return ret;
	}

MString Base64::mbtos(const VString line){
MString ret; ret.Reserv(line.size()*6/8+2, 0);
unsigned char*ln=line.uchar(), *to=ln+line.size(), *r=ret.uchar(); int i=0;
		
while(ln<to){
	*((short*)(r+i/8))+=cbgetpos(*ln) << (i%8); i+=6; ln++;
}
//ret.resize(i/8);
ret.sz = i / 8;
return ret;
}

MString Base64::mstob(const VString line){
MString ret; ret.Reserv(line.size()*8/6+3, 0);
unsigned char*l=line.uchar(), *ln=l, *to=ln+line.size(), *r=ret.uchar(); int i=0;

	while(ln<to){
		*r+= ((*(unsigned short*)ln) >> (i%8))&63;
		if(i%8+6>=8) ln++; r++; i+=6;
	}

	to=r; r=ret.uchar(); while(r<to){ *r=cb64[*r]; r++; }
	//ret.resize(r-ret.uchar());
	ret.sz = r-ret.uchar();
	return ret;
}

// bsize
int bsize(unsigned int val){
	if(!val) return 0;
	if(val<=255) return 1;
	if(val<=S64K-1) return 2;
	if(val<=S16M-1) return 3;
	return 4;
}

int bsizem(unsigned int val){
	if(!val) return 0;
	if(val<=255) return 0;
	if(val<=S64K-1) return 1;
	if(val<=S16M-1) return 2;
	return 3;
}

int bsize16(int64 val){
	if(!val) return 0;

	int i=0;
	
	while(val){
		val/=16;
		i++;
	}	

return i;
}

int bsize64(int64 val){
if(!val) return 0;
if(val<=255) return 1;
if(val<=S64K-1) return 2;
if(val<=S16M-1) return 3;
if(val<=S4GM) return 4;
if(val<=S4G*256-1) return 5;
if(val<=S4G*S64K-1) return 6;
if(val<=S4G*S16M-1) return 7;
return 8;
}

int dsize(unsigned int val){
if(!val) return 0;
if(val<10) return 1;
if(val<100) return 2;
if(val<1000) return 3;
if(val<10000) return 4;
if(val<100000) return 5;
if(val<1000000) return 6;
if(val<10000000) return 7;
if(val<100000000) return 8;
if(val<1000000000) return 9;
//if(val<=10000000000ull) return 10;
//if(val<=100000000000ull) return 11;
//if(val<=1000000000000ull) return 12;
return 13;
}


// md5
#include "../crypto/md5.c"
#include "../crypto/rmd160.c"
#include "../crypto/sha1.c"
#include "../crypto/sha224-256.c"

MString md5(VString line){
	md5_state_t state;
	MString ret;
	ret.Reserv(16);

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)line.rchar(), line.size());
	md5_finish(&state, (md5_byte_t*)ret.rchar());
	
	return ret;
}
		   
MString md5h(VString line){
	md5_state_t state;
	MString ret;
	ret.Reserv(32);

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)line.rchar(), line.size());
	md5_finish(&state, (md5_byte_t*)ret.rchar() + 16);

	unsigned char * ln = ret.uchar() + 16, *to = ret.endu(), *r = ret;
	while(ln < to){
		*r++ = (*ln / 16) < 10 ? (*ln / 16) + '0' : (*ln / 16) + 'a' - 10;
		*r++ = (*ln % 16) < 10 ? (*ln % 16) + '0' : (*ln % 16) + 'a' - 10;
		ln ++;
	}
	
	return ret;
}

//class MHash{
//	int type;
//public:

	MHash::MHash(int t){ type=t; Init(); }
	int MHash::Type(int t){ type=t; Init(); return type;  }
	int MHash::Type(){ return type; }

	void MHash::Init(){ int ad=0;
#ifndef USEMSV_MHASHT_GETALL		
	int t=type; while(t){ if(t&1 && t>1){ globalerror("#define USEMSV_MHASHT_GETALL or this is you problem!"); break; } t>>=1; }
#endif

		if(type&MHASHT_MD5){ md5_init(&md5); ad=16; }
		if(type&MHASHT_RMD160){ RMD160Init(&rmd); ad=20; }
		if(type&MHASHT_SHA1){ SHA1Reset(&sha1); ad=SHA1HashSize; }
		if(type&MHASHT_SHA256){ SHA256Reset(&sha256); ad=SHA256HashSize; }
		
		if(ad){ ret.setu(data, ad); reth.setu(data+ad, ad*2); }
	}

	void MHash::Add(VString line){
		if(type&MHASHT_MD5) md5_append(&md5, line, line.size());
		if(type&MHASHT_RMD160) RMD160Update(&rmd, line, line.size());
		if(type&MHASHT_SHA1) SHA1Input(&sha1, line, line.size());
		if(type&MHASHT_SHA256) SHA256Input(&sha256, line, line.size());
	
	}

	void MHash::Finish(int t){
		type=t; Finish();
	}

	void MHash::Finish(){
		if(type==MHASHT_MD5){ md5_finish(&md5, ret); ret.sz=16;  }
		if(type==MHASHT_RMD160){ RMD160Final(ret, &rmd); ret.sz=20; }
		if(type==MHASHT_SHA1){ SHA1Result(&sha1, ret); ret.sz=20; }
		if(type==MHASHT_SHA256){ SHA256Result(&sha256, ret); ret.sz=32; }

		reth.sz=ret.sz*2;
		prmf_btoh(reth, reth, ret, ret);	
	}

	VString MHash::Get(){ return ret; }
	VString MHash::GetH(){ return reth; }
//};

//class MD5Code{
//    public:
//	md5_state_t state;
//    MString ret; 
    
	MD5Code::MD5Code(){ md5_init(&state); ret.Reserv(16);} 
    void MD5Code::Add(VString line){md5_append(&state, (const md5_byte_t *)line.rchar(), line.size());}
    void MD5Code::Finish(){md5_finish(&state, (md5_byte_t*)ret.rchar());}
    MString MD5Code::Get(){return ret;}
    MString MD5Code::GetH(){
		MString r; r.RClean(32, 0); unsigned char *l=r, *c=ret;
		for(int i=0; i<16; i++){
			*l++=((unsigned char)*c/16 + (unsigned char)(*c/16<10 ? 48 : 87));
			*l++=((unsigned char)*c%16 + (unsigned char)(*c%16<10 ? 48 : 87));
			c++;
		}
		return r;
	}
	/*MString Geth(){
        MString r;
        for(int i=0; i<16; i++){
        r+=MString((unsigned char)ret[i]/16 + ((unsigned char)ret[i]/16<10 ? 48 : 87))+MString((unsigned char)ret[i]%16 + ((unsigned char)ret[i]%16<10 ? 48 : 87));
        }
    return ret;
    }*/
//};


int getbbsz(int i){ return i/8+(i%8>0);  }  // get bit > byte sz


// MSV text Library

int wcslash(MString &val){
	if(val[val.size()-1]!='/')
	val+="/";
	return 1;
}

void cslash(MString &val){
	if(!val) return ;
#ifdef WIN32
	if(val[val.size()-1]!='\\' && val[val.size()-1]!='/') val+="\\";
#else
	if(val[val.size()-1]!='/') val+='/';
#endif
return ;
}


void cslash(MString &val, int i){
if (!i){ if(val[val.size()-1]!='\\') val+="\\";}
else if(val[val.size()-1]!='/') val+='/';
return ;
}

BYTE rts_lntype(unsigned short line){
BYTE lnt; if(line==32) lnt=0; else if(line>='0' && line<='9') lnt=2; else if(line<65) lnt=3; else lnt=4; return lnt;
}

int rts_con(unsigned short *ln, unsigned int sz, unsigned short *&line){ //read to symbol
unsigned short *to=ln+sz; if(!line || !ln) return 0;
BYTE lnt=rts_lntype(*line); line++;
for(line; line<to; line++){ if(lnt!=rts_lntype(*line)) return 1;} return 0;
}

int rrts_con(unsigned short *ln, unsigned short *to, unsigned short *&line){ //read to symbol
if(!line || !ln) return 0;
BYTE lnt=rts_lntype(*line); line--;
for (line; ln<line; line--){ if(lnt!=rts_lntype(*line)) return 1;} return 0;
}


MString Replace(VString line, VString fr, VString to, unsigned int cnt){
	MString ret;
	int s=prmf_replace(0, 0, line, line, fr, fr, to, to, cnt);
	ret.Reserv(s); s=prmf_replace(ret, ret, line, line, fr, fr, to, to, cnt);
	return ret;
}

MString dtos(double val, int ml){
MString ret=itos(val);
val=val-int64(val);
if(val<0) val*=-1;
if(!val) return ret;
ret+=".";
if(val!=val) return ret;
//val*=1000000;
while(val){
if(ml<=0) break;
val*=10; ret+=itos(val); val-=int64(val);
ml--;
}
return ret;
}

MString gditos(double i){
int rt=0;
if(i>1000) {rt=1; i/=1024; 
if(i>1000) {rt=2; i/=1024; 
if(i>1000) {rt=3; i/=1024; 
if(i>1000) {rt=4; i/=1024; }}}}
return dtos(i, 2)+ (rt ? (rt==1 ? "Kb" : (rt==2 ? "Mb" : ( rt==3 ? "Gb" : "Tb" )) ) : "b");
}

int64 gdstoi(const VString line){
char*l=line, *t=l+line.sz; char c=0;
for(l; l<t; l++){
	if(!isnumd(*l)){ c=*l; break; }
} t=l; l=line;

int64 ret=stoi(l, t-l);
if(!c || c=='b' || c=='B') return ret;
if(c=='k' || c=='K') return ret*S1K;
if(c=='m' || c=='M') return ret*S1M;
if(c=='g' || c=='G') return ret*S1G;
if(c=='t' || c=='T') return ret*S1G*S1K;
return ret;
}

VString dspace(VString line, int s){ // удаление пробелов s - начало конец;
	if(!line) return ""; int i=0, ii=line.size()-1;
	//if (s & 1) rtnms(line, ' ', i); if (s & 2) rrtnms(line, ' ', ii);
	if(s&1) while(line[0]==' '){ line.data++; line.sz--; }
	if(s&2) while(line.endo()==' '){ line.sz--; }
	//if(ii<i-1) ii=i;
	return line; // .str(i,ii-i+1)
}

/*
int dspacea(MString &line, int s){ // удаление пробелов s - начало конец;  1,2,3
int i=0, ii=line.size()-1; if (s & 1) { rtnms(line, ' ', i); }
if (s & 2) { rrtnms(line, ' ', ii);} if(ii<i) ii=i-1;
line=line.Mid(i,ii-i+1);
return  1;
}

VString dspacev(VString &line, int s){ // удаление пробелов s - начало конец;  1,2,3
int i=0, ii=line.size()-1; if (s & 1) { rtnms(line, ' ', i); }
if (s & 2) { rrtnms(line, ' ', ii);} if(ii<i) ii=i-1;
if(s&4) return line=line.str(i,ii-i+1);
return line.str(i,ii-i+1);
}*/