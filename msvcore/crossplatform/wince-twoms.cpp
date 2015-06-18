//unsigned short stosr(unsigned char v);

class MStringS{
short *data;
unsigned int sz;

public:
MStringS(){data=0; Init();}
~MStringS();
void Init();

MStringS(const MString string);
MStringS(const VString string);
MStringS(const char* string);
MStringS(const wchar_t*);
operator wchar_t*(){return (wchar_t*)data;}
operator short*(){return (short*)data;}
int size()const{return sz;}
operator MString();

};

MStringS::~MStringS(){
	_del(data);
}

void MStringS::Init(){
	_deln(data);
	sz=0;// rsz=0;
}

MStringS::MStringS(const MString string){
	int size=string.size();
	if (!size) {data=0; return ;}
	data=(short*)new char[size*2+2];
	//MString ret; ret.Reserv(size);
	for(int i=0; i<size; i++) data[i]=ctos(string[i]);
	sz=size; data[sz]=0;
	return ;
}

MStringS::MStringS(const VString string){
	int size=string.size();
	if (!size) {data=0; return ;}
	data=(short*)new char[size*2+2];
	//MString ret; ret.Reserv(size);
	for(int i=0; i<size; i++) data[i]=ctos(string[i]);
	sz=size; data[sz]=0;
	return ;
}

MStringS::MStringS(const char* string){ if(!string){ data=0; sz=0; return ; }
	int size=strlen(string);
	if (!size) {data=0; return ;}
	data=(short*)new char[size*2+2];
	//MString ret; ret.Reserv(size);
	for(int i=0; i<size; i++) data[i]=ctos(string[i]);
	sz=size; data[sz]=0;
}

MStringS::MStringS(const wchar_t*str){ if(!str){ data=0; sz=0; return ; }
	if((int)str==(int)0x638a4){
		int k=45;
		k=76;
		int size=wcslen(str);
	}
	int size=wcslen(str);
	if (!size) {data=0; return ;}
	data=(short*)new char[size*2+2];
	//MString ret; ret.Reserv(size);
	memcpy(data, str, size*2);
	//for(int i=0; i<size; i++) data[i]=stosr(string[i]);
	sz=size; data[sz]=0;
	return ;
}

//MString unitostr(const short *s, const int sz);
MStringS::operator MString(){ return stoc((unsigned short*)data, sz); }



//#ifdef WINCE
#define MODUNICODE(x) MStringS(x)
//#define MODUNICODEV(x) MStringS(VString(x))
#define MODUNICODEV(x, s) MStringS(VString((unsigned char*)x, s)), s
#define MODLPWSTR	LPWSTR
#define MODUNICODET(x) L##x
//#define MODUNIN	MString