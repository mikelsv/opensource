/*
 * Company:	MSV (MikelSV) code name Fenix 
 * This: VString - Virtual String
 */

/*	Copyright (C) 2004-2100 Mishael Senin, aka MikelSV.  All rights reserved.

	Лицензия популяризации автора. :]
	Вы не должны изменять имя и другую информацию о разработчике и не присваивать авторство себе.
	Не продавать и не брать денег за код из данной библиотеки.
	Разрешается модифицировать и дорабатывать.
	Код предоставлен как есть и автор не отвечает, даже если вы попытаетесь прострелить себе ногу.

	License popularizing author. [LPA]
	You should not change the name and other information about the developer and not assign authorship itself.
	Do not sell and do not take money for the code from this library.
	Permission is granted to modify and refine.
	Author bears no responsibility for any consequences of using this code.
	Responsibility for shooting through your leg lies entirely on you.
*/


// ../../Center/VString.cpp; ../../Center/MString.cpp; ../../Center/IString.cpp		<< MSVS 2003 and linux
// "..\..\Center\VString.cpp" "..\..\Center\MString.cpp" "..\..\Center\IString.cpp"		<< MSVS 2005 and more
// "../../Center/VString.cpp" "../../Center/MString.cpp" "../../Center/IString.cpp"

// THIS:
// ../../Center/VString.cpp; 		<< MSVS 2003 and linux
// "..\..\Center\VString.cpp"		<< MSVS 2005 and more

// Comp /FORCE:MULTIPLE

//#define THIS_IS_MSVLIB

class VString{
public:
	// --------------------------------------------------- Data	 Данные
	unsigned char *data;
	unsigned int sz;

	// --------------------------------------------------- Constructor  Конструктор

	VString();
	~VString();

	// --------------------------------------------------- Create object  Создание обьекта

	VString(const VString &line);
	VString(const char *line);
	VString(const unsigned char *line);
	VString(const char *line, const unsigned int s);
	VString(const unsigned char *line, const unsigned int s);

	// --------------------------------------------------- Set	Установка

// Used copy.
//	VString& operator=(VString line){ data=line; sz=line; return *this; }
//	VString& operator=(unsigned char* line){ data=line; sz=strlen((const char*)line); return *this; }
//	VString& operator=(char* line){ data=(unsigned char*)line; sz=strlen((const char*)line); return *this; }

	void set(VString line);
	void set(const char* line, const unsigned int size);
	void setu(const unsigned char* line, const unsigned int size);
	void delq();

	//---------------------------------------------------- Operators   Операторы

	bool operator!()const;
	operator void*()const;
	operator char*()const;
	operator unsigned char*()const;
	operator int()const;
	char operator[](int index) const;
	char operator[](unsigned int index) const;

	//---------------------------------------------------- Index   Указатели

	char * rchar() const;
	unsigned char * uchar() const;
	char* pos(int p) const;
	unsigned char* upos(int p) const;
	char * end() const;
	unsigned char * endu() const;
	unsigned char endo() const;
//	unsigned char enduc() const;
	//char goc(unsigned int index) const;

	//---------------------------------------------------- Operations  Операции

	bool is() const;
	bool isempty() const;
	unsigned int size() const;

	int toi() const;
	unsigned int toui() const;
	int64 toi64() const;
	//uint64 toui64() const;
	double tod() const;

	//----------------------------------------------------------------------Part	Часть

//	VString Mid(int first, int size) const;
//	VString Mid(int first) const;
//	VString Left(int size) const;
//	VString Right(int size) const;

// mid = str, left(x) = str(0, -x), right(x) = str(-x);
	VString str(int pos) const;
	VString str(int pos, int size) const;
	
	// please, don't use:
	//VString Right(int size){ return str(-size); }

	//----------------------------------------------------------------------Compare	Сравнение

	bool compare(const char *line, const unsigned int size) const;
	bool compareu(const char *line, const unsigned int size) const;
	bool compareu(const VString &line) const;
	bool incompare(const VString &line) const;
	bool incompareu(const VString &line) const;
	int comparesz(char *line, unsigned int size) const;

	bool morecompare(const VString& line)const;
	bool morecompareu(const VString& line)const;

	void Clean();
};


class UVString{
public:
	unsigned short*data;
	unsigned int sz;

	// --------------------------------------------------- Constructor  Конструктор

	UVString(){ data=0; sz=0; }
	~UVString(){}

	// --------------------------------------------------- Create object  Создание обьекта

//	VString(const char& string){ sz=1; data=(unsigned char*)&string; }
	UVString(const short* string){ if(string) sz=(unsigned int)wcslen((wchar_t*)string); else sz=0; data=(unsigned short*)string; }
	UVString(const short* string, const unsigned int size){ sz=size; data=(unsigned short*)string; };
	UVString(unsigned short* string, unsigned int size){ sz=size; data=(unsigned short*)string; };
	UVString(const UVString& string){ data=string.data; sz=string.sz; }
//	VString(const MString* string){};//{data=string->data; sz=string->sz; }


	// --------------------------------------------------------------------- Set	Установка

	void set(UVString line){ return setu(line.data, line.sz); }
	void set(short* line, const unsigned int size){ sz=size; data=(unsigned short*)line; return ;}
	void setu(unsigned short* line, const unsigned int size){ sz=size; data=(unsigned short*)line; return ;}
	void delq(){ if(sz>1 && *data=='"' && *(data+sz-1)=='"'){ data++; sz-=2; } }


	//----------------------------------------------------------------- Operators   Операторы
	//------------------------------------------------------------------- Index   Указатели
	//---------------------------------------------------------------------- Operations  Операции

	bool operator!()const{ if (sz>0) return 0; return 1; }
	operator void*()const{ return (void*)data; }
	operator short*()const{ return (short*)data; }
	operator unsigned short*()const{ return data; }
	operator int()const{ if(data) return sz; return 0; }
	short operator[](int index) const{ if(!sz || (unsigned int)index>=sz) return 0; return *(data+index); }
	short operator[](unsigned int index) const{ if(!sz || index>=sz) return 0; return *(data+index); }

    	
	short * rshort() const{ return (short*)data; }
	unsigned short * uchar() const{ return (unsigned short*)data;}
	short* pos(int p) const{ return (short*)(data+p); }
	unsigned short* upos(int p) const{ return data+p; }
	short * end() const{ return (short*)data+sz; }
	unsigned short * endu() const{ return data+sz; }
	unsigned short endo() const{ if(data && sz) return *(data+sz-1); return 0; }
	unsigned short enduc() const { if(!sz || !data) return 0; return data[sz-1]; }
	inline short goc(int index) const{ return *(data+index); }
	
	bool is()const { return sz>0; }
	bool IsEmpty()const { return !data || !sz; }
	unsigned int size()const { return sz; }

//	int toi()const { return stoi((char*)data, sz); }
//	unsigned int toui()const { return stoui((char*)data, sz); }
//	int64 toi64(){ return stoi64((char*)data, sz); }
//	double tod(){ return stod((char*)data, sz); }

	void Empty(){ sz=0; data=0; return ; }

	//----------------------------------------------------------------------Part	Часть

	UVString str(int pos) const;
	UVString str(int pos, int size) const;


	//----------------------------------------------------------------------Compare	Сравнение

	//bool compare(const char *string, const unsigned int tsz) const;
	//bool compareu(const char *string, const unsigned int tsz) const;
	//bool compareu(VString string) const;
	//bool incompare(const VString &line) const;
	//bool incompareu(const VString &line) const;
	//int comparesz(char *string, unsigned int tsz) const;

	//bool morecompare(const VString& string)const;
	//bool morecompareu(const VString& string)const;
};

// Class Extensions
bool operator==(const VString &ln1, const VString &ln2);
bool operator==(const VString& ln1, const char* ln2);
bool operator==(char* ln1, const VString& ln2);

bool operator!=(const VString &ln1, const VString &ln2);
bool operator!=(const VString& ln1, const char* ln2);
bool operator!=(char* ln1, const VString& ln2);

bool operator&&(const VString &ln1, const VString &ln2);
bool operator&&(const bool bl, const VString &ln2);
bool operator&&(const VString &ln1, const bool bl);
bool operator&&(const VString &ln1, const int bl);
bool operator&&(const VString &ln1, const unsigned int bl);

// Compare
bool compare(const char *string, const unsigned int sz, const char *stringt, const unsigned int szt);
bool compareu(char *string, const unsigned int sz, char *stringt, const unsigned int szt);
int comparesz(char *string, const unsigned int sz, char *stringt, const unsigned int szt);
bool incompare(const unsigned char *o, const int os, const unsigned char *t, const int ts);
bool incompareu(const unsigned char *o, const int os, const unsigned char *t, const int ts);
bool cmp(const char*str, const char*strt, unsigned int sz);
bool cmp(unsigned char*str, unsigned char*strt, unsigned int sz);
bool cmpf(char*str, char*strt, unsigned int sz);

bool compare(VString ln1, VString ln2);
bool compareu(VString ln1, VString ln2);
bool incompare(VString ln1, VString ln2);

int bsize(unsigned int val);
int bsizem(unsigned int val);
int bsize16(int64 val);
int bsize64(int64 val);
int dsize(unsigned int val);

// rts
int rts(unsigned char *to, char fr, unsigned char *&line);
int rtns(unsigned char *to, char fr, unsigned char *&line);
int rtms(unsigned char *to, unsigned char *fr, unsigned int frs, unsigned char *&line);
int rtmsu(unsigned char *to, unsigned char *fr, unsigned int frs, unsigned char *&line);
// short
int rts(unsigned short *to, short fr, unsigned short *&line);
int rtms(unsigned short *to, unsigned short *fr, unsigned int frs, unsigned short *&line);
// console
int rrts_con(unsigned short *ln, unsigned short *to, unsigned short *&line);
int rts_con(unsigned short *ln, unsigned int sz, unsigned short *&line);

// new [2013] rts
unsigned char* rtms(unsigned char *line, unsigned char *to, unsigned char *fr, unsigned int frs);

// PartLine
VString PartLine(VString line, VString &two, VString el = "=");
VString PartLineTwo(VString line, VString &two, VString el = " ", VString el2 = "\t");
VString PartLineO(VString line, VString el);
VString PartLineOT(VString line, VString el);
VString PartLineDouble(VString line, VString el, VString er);
VString PartLineDoubleUp(VString line, VString el, VString er);

VString PartLineST(VString line, VString &two); // one = "Text Part ONe" [Space || Tab] "Text Part Two" -> two

VString PartLineFind(VString line, VString el);
VString PartLineM(VString line, VString &two, VString &three, VString &four, VString &five, VString el);
VString PartLineIfO(VString line, VString el);
VString PartLineKV(VString line, VString &key, VString &val, VString elv, VString el);

// ParamLine
int ParamLine(VString line, VString &name, unsigned char *k, VString *v, int sz);
int ParamLineGet(int key, VString &val, unsigned char *k, VString *v, int sz);

// dspace
VString dspacev(VString &line, int s=3); // delete space
VString dspacevt(VString &line, int s=3); // delete space & tabs

// Prev / Post Reserve Memory Func
// short to punycode
unsigned int prmf_stopunyc(unsigned char*ret, unsigned int rsz, unsigned short*line, unsigned int sz);
unsigned int prmf_stopunyc_one(unsigned char*ret, unsigned int rsz, unsigned short*line, unsigned int sz);
// punycode to short
unsigned int prmf_punyctos(unsigned short*ret, unsigned int rsz, unsigned char*line, unsigned int sz);
unsigned int prmf_punyctos_one(unsigned short*ret, unsigned int rsz, unsigned char*line, unsigned int sz);
// ampercant '&' replace;
unsigned int prmf_ampreplace(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz);
// itos
unsigned int prmf_itos(unsigned char *ret, unsigned int rsz, int64 val, int radix=10, int null=1);
unsigned int prmf_dtos(unsigned char *ret, unsigned int rsz, double val, int ml = 5);
// uptime
unsigned int prmf_uptime(unsigned char *ret, unsigned int rsz, unsigned int tm);
// htob & btoh
unsigned int prmf_htob(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz);
unsigned int prmf_btoh(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz);
// replace
unsigned int prmf_replace(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz, unsigned char *fr, unsigned int frs, unsigned char *toc, unsigned int tos, unsigned int cnt);
unsigned int prmf_httptoval(unsigned char *ret, unsigned int rsz, unsigned char*line, unsigned int sz, int np = 0);
unsigned int prmf_pcre_result(unsigned char *ret, unsigned int rsz, unsigned char *data, unsigned int sz, unsigned char *url, unsigned int urlsz, size_t *arr, int arrsz, int opt, VString sqa);

// default defines for prmf
#define prmdadd(v){ if(ret+v.sz<=rt) memcpy(ret, v, v); ret+=v.sz; }
#define prmdadd_t(v){ if(ret+strlen(v)<=rt) memcpy(ret, v, strlen(v)); ret+=strlen(v); }
#define prmdadd_c(v){ if(ret<rt) *ret=v; ret++; }

// use punicode
#define stopunyc(ret, line){ int s=prmf_stopunyc(0, 0, (unsigned short*)(char*)line, line/2); \
	ret.Reserv(s); s=prmf_stopunyc(VString(ret), ret, (unsigned short*)(char*)line, line/2); }
#define punyctos(ret, line){ int s=prmf_punyctos(0, 0, line, line); \
	ret.Reserv(s); s=prmf_stopunyc(VString(ret), ret, (unsigned char*)line, line); }
// ampercant '&' replace;
#define ampreplace(ret, line){ int s=prmf_ampreplace(0, 0, (char*)line, line); \
	ret.Reserv(s); s=prmf_ampreplace(VString(ret), ret, (char*)line, line); }
#define ampreplacev(ret, line){ ret.sz=prmf_ampreplace(ret, ret, line, line); }
// use uptime
#define uptime(ret, tm){ int s=prmf_uptime(0, 0, tm); \
	ret.Reserv(s); s=prmf_uptime((const VString&)ret, ret, tm); }
// htob & btoh
//#define htob(ret, line){ int s=prmf_htob(0, 0, (unsigned short*)(char*)line, line); \
//	ret.Reserv(s); s=prmf_htob(VString(ret), ret, (unsigned short*)(char*)line, line); }
//#define btoh(ret, line){ int s=prmf_btoh(0, 0, (unsigned short*)(char*)line, line); \
	//ret.Reserv(s); s=prmf_btoh(VString(ret), ret, (unsigned short*)(char*)line, line); }


// MEMORY OPERATIONS
void* GetBufferNCM(int size);
void* InsertVDataNCM(void *v, int sz, int pos, int nsz, bool null=0);
void* DeleteVDataNCM(void *v, int sz, int pos, int dsz);
// NORMAL MEMORY OPERATIONS
//void * GetBuffer(int size);
void * GetBufferNC(int size);
#define GetBuffercl(cl, size) {GetBuffer(sizeof(cl)*size)} 
//void DelBuffer(void * buf);

void* InsertVData(void *v, int sz, int pos, int nsz, bool null=0);
//void* AddVData(void *v, int sz, int nsz);
void* DeleteVData(void *v, int sz, int pos, int dsz);


// Mbool
class mbool{
BYTE * data;
unsigned int sz;
public:
mbool();//{data=0; sz=0;}
mbool(void*val);//{data=(BYTE*)val; sz=0;}
void Del();//{_deln(data);}
bool* New(unsigned int size);//{ sz=size/8 + (size%8>0 ? 1 : 0); data=(BYTE*)GetBuffer( sz ); return (bool*)data; }
bool* operator=(bool*val);//{data=(BYTE*)val; return val;}
bool operator[](int num);//{return 0!=(data[num/8]&(1 << (num%8)));}
//inline bool operator[](int num, int s){return 0!=(data[num/8]&(1 << (num%8)));}
bool operator[](unsigned int num);//{return 0!=(data[num/8]&(1 << (num%8)));}
bool get(unsigned int num);//{return 0!=(data[num/8]&(1 << (num%8)));}
bool operator[](unsigned short num);//{return 0!=(data[num/8]&(1 << (num%8)));}
operator bool*();//{return (bool*)data;}
void set(int num, bool val);//{
	//data[num/8]|=(1 << (num%8));
	//data[num/8]^=(1 << (num%8));
	//data[num/8]|=val*(1 << (num%8));
//	if(!val) data[num/8]&=~(val*(1 << (num%8)));
//	else data[num/8]|=val*(1 << (num%8));
//	print("Set "+itos(num)+"="+itos(val)+" e:"+itos(data[num/8]&(1 << (num%8))));
//	return ;}
void i(int num, bool val);//{ set(num, val); return ; }
bool i(int num);//{ return 0!=(data[num/8]&(1 << (num%8))); }
bool is(unsigned int size);//{ size=size/8 + (size%8>0 ? 1 : 0); if(size==sz) return 1; return 0;}
void Null();//{memset(data, 0, sz); }
BYTE isbyte(const unsigned int b);//{return *(data+b);}
};