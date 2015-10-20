/*
 * Company:	MSV (MikelSV) code name Vyris(x)
 * This: MString - My String class
 *		 My version &CString class
 *
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MString version 1.4.5 10/April/2005
	MString version 0.1.5.0 24/Febrary/2007
	MString version 0.2.0.0 25/November/2012

	MString : Copyright (C) 2004 - 2100, Mishael Senin
  --------------------------------------------------------------------------------
  --------------------------------------------------------------------------------
 */

#define		MSTRING_VER "0.2.0.0"


extern VString _vstringnull_;
//extern MString _mstringnull_;

#ifdef USEMSV_GENERALCPP
//VString _vstringnull_;
#endif


class MString: public VString{
	// --------------------------------------------------- Data	 Данные
protected:
//	unsigned char *data;
//	unsigned int sz;

	// --------------------------------------------------- Constructor  Конструктор
public:	
	void Init();
	MString();
	~MString();
	
protected:
	void MStringNew(const void*line);

	// --------------------------------------------------- Create object  Создание обьекта
public:
	MString(const char string);
	MString(const char *line);
	MString(const unsigned char *line);
	MString(const char *line, const unsigned int s);
	MString(const unsigned char *line, const unsigned int s);

	MString(const short string);
	MString(const short *line, int size);
	MString(const int code);
	MString(const unsigned int code);
	MString(const unsigned int code, const unsigned int sz);
	
	MString(const VString &line);
	MString(const MString &line);
	
	// --------------------------------------------------- Set	Установка

// + const MString& operator=... ??
	const MString& operator=(char string);
	const MString& operator=(const char* line);
	const MString& operator=(const unsigned char* line);
	const MString& operator=(const VString& line);
	const MString& operator=(const MString& line);
	//MString& operator=(const MString& line){ sz=line; unsigned char *odata=data; MStringNew(line); _deln(odata); return *this; }
	
	MString& set(const VString line);
	MString& set(const char* line, const unsigned int size);
	MString& set(const unsigned char* line, const unsigned int size);
	MString& setu(const unsigned char* line, const unsigned int size);

	// --------------------------------------------------- Add  Добавление	

	//const MString& operator+=(const MString& string);
	const MString& operator+=(const VString& line);
	const MString& operator+=(const char line);
	MString& Add(VString o, VString t, VString th=VString(), VString f=VString(), VString fi=VString());
	MString& Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7=VString(), VString l8=VString(), VString l9=VString());
	MString& Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9,
		VString l10, VString l11=VString(), VString l12=VString(), VString l13=VString(), VString l14=VString(), VString l15=VString(), VString l16=VString(), VString l17=VString(), VString l18=VString(), VString l19=VString());
	MString& AddR(VString &l1, VString &l2=_vstringnull_, VString &l3=_vstringnull_, VString &l4=_vstringnull_, VString &l5=_vstringnull_, VString &l6=_vstringnull_, VString &l7=_vstringnull_, VString &l8=_vstringnull_, VString &l9=_vstringnull_);

	//---------------------------------------------------- Move   Перемещение

	MString& operator-=(MString& line);
	void move (MString& line);
		
	//---------------------------------------------------- Operators  Операторы

	// use VString operators

	//---------------------------------------------------- Index  Указатели

	// use VString index

	//---------------------------------------------------- Association Обьединение

	friend MString operator+(const MString& ln1, const MString& ln2);
	friend MString operator+(const MString& ln1, const VString& ln2);
 	friend MString operator+(const MString& ln1, const char* ln2);
	friend MString operator+(const char* ln1, const MString& ln2);

	//---------------------------------------------------- Part  Часть

//	MString Mid(int first) const;
//	MString Mid(int first, int size) const;
//	MString Left(int size) const;
//	MString Right(int size) const;

//	MString str(const int p) const;
//	MString str(int pos, int size) const;

	//---------------------------------------------------- Operations  Операции

	MString& Reserv(int size, int p=0);
	//MString& ReservM(int size, int p=0); // Reserv More
	void CleanData(int p);
	void RClean(int sz, int p);
	//void RSize(int size, int p=0);

	int getbit(int b){ if(!data || (int)sz<(b+7)/8) return 0; return data[b/8]&1<<(b%8);  }
	int setbit(int b, int v){
		if(!data || (int)sz<(b+7)/8) return 0;
		if(v) data[b/8]|=1<<(b%8);
		else data[b/8]&=~(1<<(b%8));
		return v;
	}

	//void UseLeak(); // set data=0; memory leak!
	void Clean();
	
	// Danger functions!
	//int rewrite(const MString& string, int pos);
	//unsigned int resize(const unsigned int size); //{return sz=size;}
	//void newsize(unsigned int s);

	//---------------------------------------------------- Functions  Функции

	//int toi();
	//void* tox();
};

class TString;



// Int library
//int stoi(char *line, unsigned int size=0);
TString itos(int64 val, int radix=10, int null=1);
TString itob(unsigned int val, int null=0);
TString dtos(double val, int ml=5);
//MString htob(VString line);
TString gditos(double i);

TString htob(VString line);
TString btoh(VString line);

// unicode string short
// short to char
unsigned char stoc(const unsigned short s);
MString stoc(const unsigned short *s, const int sz);
char stocdos(const short s);
char stoclin(const short s);
MString stocdos(const short *s, const int sz);
// CHAR to SHORT
unsigned short ctos(const unsigned char s);
MString ctos(const unsigned char*s, const int sz);
MString ctos(VString line);
unsigned short dosctos(unsigned char v);
// UTF to SHORT
unsigned short utftos(unsigned short v);
MString utftos(const VString ln);
// SHORT to UTF
unsigned short stoutf(unsigned short v);
MString stoutf(VString line);
// UTF to CHAR
MString utftoc(const VString ln, int sys=0);
MString utftotr(const MString ln);
// CHAR to UTF
MString ctoutf(const VString ln);
// WIN to DOS
MString ctodos(const VString ln);

// Replace
TString Replace(VString line, VString fr, VString to, unsigned int cnt=0);
//MString sons(MString line);

// ILink
struct DUMS{ MString key, val; };
struct DUVS{ VString key, val; };
struct DUMSi{ MString key, val; unsigned int i; };

class ILink{
public:
	//MString data;
	const VString proto, domain, path, file, iquest, target;
	DUVS *ilinkq;
	MString mdata; // << link + ilinkq
	//TLine ilinkq;
	unsigned short port, largs, args; BYTE portsz; bool ltype;

	ILink();
	~ILink();

	void Clean();
	//void Ilink(const MString &ilink){ return Ilink(VString(ilink)); }
		
	void Link(const VString &ilink, int tp=0);
	void MoveData(char*ln, int mo, int pr, int dom, int pa, int fi, int iq, int tg);
	void AnalysIQuest(const VString &_iquest);
	void AnalysIQuestSet(const VString &_iquest);

	// Get
	VString name();
	VString ext();
	// Find
	DUVS* Find(const VString &line);

	char* ProtoPos();
	char* DomainPos();
	char* PathPos();
	char* FilePos();
	char* QuestPos();

	void SetProto(const VString nproto);
	void SetDomain(const VString &nfile);
	void SetFile(const VString &nfile);
	void SetPath(const VString &npath);
	void SetQuest(const VString &nquest);


	//VString GetFullPath();
	VString GetFullPath();
	VString GetOnlyPath();
	VString GetPathFile();
	VString GetProtoDomainPath();
	VString GetPathFileQuest();

	void FirstArg();
	VString GetArg();
	VString GetLastArg();
	VString GetArg(VString ret);
	//VString GetArgI(VString key);
	int GetArgSz();
};

// Text Library
int wcslash(MString &val);
void cslash(MString &val);
void cslash(MString &val, int i);

// Base64
class Base64{
public:
	static MString btos(const VString line);
	static MString stob(const VString line);
	static MString mbtos(const VString line);
	static MString mstob(const VString line);
};

// MD5 - 6.12.05 1.0.0 MSVSPL Specila Library
TString md5(VString line);
TString md5h(VString line);
//TString md5b(VString line);

#include "crypto/md5.h"
#include "crypto/rmd160.h"
//#include "msv/sha1.h"
#include "crypto/sha.h"

#define MHASHT_MD5		1
#define MHASHT_RMD160	2
#define MHASHT_SHA1		4
#define MHASHT_SHA256	8
#define MHASHT_ALL		255

#define MHASHT_MAXDATA	32
//#define USEMSV_MHASHT_GETALL	// for generate all

class MHash{
	unsigned char data[MHASHT_MAXDATA*3]; int type;
#ifndef USEMSV_MHASHT_GETALL
	union{
#endif
		md5_state_t md5;  
		RMD160_CTX rmd;
		SHA1Context sha1;
		SHA256Context sha256;
#ifndef USEMSV_MHASHT_GETALL	
	};
#endif

public:
	VString ret, reth;

	MHash(int t);
	int Type(int t);
	int Type();

	void Init();

	void Add(VString line);
	void Finish();
	void Finish(int t); // finish for type
	VString Get();
    VString GetH();
	VString FastH(int type, VString line);
};

class MD5Code{
    public:
	md5_state_t state;
    MString ret; 
    
    MD5Code();
    void Add(VString line);
    void Finish();
    MString Get();
    MString GetH();
};


// MyTime
unsigned int time();//{ timeb tb; ftime(&tb); return tb.time; }
double timeg();
timeb alltime();//{ timeb tb; ftime(&tb); return tb; }
int timemi(timeb&o, timeb&t);//{ return (o.time-t.time)*1000+o.millitm-t.millitm; };
int timemit(timeb&t);
int64 sectime();

struct timebm{
	unsigned int time; unsigned short millitm; short timezone; short dstflag; 
	void operator=(const timeb &tm){ time=(unsigned int)tm.time; millitm=tm.millitm; timezone=tm.timezone; dstflag=tm.dstflag; return ; }
};

extern VString mtmonths[];

class MTime{
public:
	int year;
	int month;
	int day;
	int doy;
	int hour;
	int minute;
	int sec;
	unsigned short millitm;
	short timezone;
	short dstflag;

	int ttime;

	MTime();
	int timemit(timebm&t);
	int64 timemitx(timebm&t);
	int timemits(timebm&t);
	int mtime(int tm, bool gmt=0);
	int time(int cl=0);
	int mktime(bool gmt=0);

	int weekday();
	int monthdays(int month=0);

	int gmttotime(VString line);

	MString date(const VString line, unsigned int tm=-1, bool gmt=0, bool eng=0, BYTE c=0);
	unsigned int rpmf_date(unsigned char *ret, unsigned int rsz, const VString line, unsigned int tm, bool gmt, bool eng, BYTE c);
	unsigned int mktime(int y, int m=0, int d=0, int h=0, int mi=0, int s=0, bool gmt=0);
};

//typedef MTime64 MTime;

// Timer
class MTimer{
	int64 freq;
public:
	MTimer();
	MTimer(int noinit);

	bool Init();
	double dtime();
	int64 time();
};

class MProfiler{
public:
	uint64 _ctime, _ktime, _utime;
	uint64 create, kernel, user;

	MProfiler(){ _ctime=0; _ktime=0; _utime=0; create=0; kernel=0; user=0; }

	void Thread(){
		FILETIME c, e, k, u;
		GetThreadTimes(GetCurrentThread(), &c, &e, &k, &u);
		Set(c, e, k, u);
		//ct=0; kt=0; ut=0;
		return ;
	}

	void Set(FILETIME &c, FILETIME &e, FILETIME &k, FILETIME &u){
		_ctime=ToInt(c);
		//etime=ToInt(e);
		_ktime=ToInt(k);
		_utime=ToInt(u);

	}

	uint64 ToInt(const FILETIME &t){
		return (((uint64)t.dwHighDateTime<<32)+t.dwLowDateTime);
	}

	void Time(){
		if(!_ctime)
			return Thread();

		FILETIME c, e, k, u;
		GetThreadTimes(GetCurrentThread(), &c, &e, &k, &u);
		create+=ToInt(c)-_ctime;
		//et+=ToInt(e)-etime;
		kernel+=ToInt(k)-_ktime;
		user+=ToInt(u)-_utime;
		
		Set(c, e, k, u);
		return ;
	}
};


class MAutoProfiler{
	MProfiler *profiler;
public:
	MAutoProfiler(MProfiler &p){
		profiler=&p;
	}

	~MAutoProfiler(){
		profiler->Time();
	}
};

// Conf Line
class ConfLine{
public:
	MString data, file;

public:
	ConfLine();
	ConfLine(VString file);

	int Is();
    
	VString Load(VString fl);
	VString LoadData(VString fl, VString dt);
	VString Reload();
	int Save();
	int Save(VString fl);
	VString operator[](VString line);
	VString Get(VString line);
	VString Set(VString line, VString set);
	VString Set(VString line, int64 set);
	VString GetLine(unsigned int &p);
	VString GetData();
	unsigned int GetLine(VString &line, unsigned int p);
	void Clear();
	void ClearData();
};




#define MSTRING_TEMPALTE_ADD5										\
	sz = l1.size() + l2.size() + l3.size() + l4.size() + l5.size(); \
	if(!sz){ StringDel(data); data = 0; return *this; }				\
	unsigned char *odata = data;									\
	StringNew(0);													\
	unsigned char *ndata = data;									\
	memcpy(ndata, l1, l1); ndata += l1.size();						\
	memcpy(ndata, l2, l2); ndata += l2.size();						\
	memcpy(ndata, l3, l3); ndata += l3.size();						\
	memcpy(ndata, l4, l4); ndata += l4.size();						\
	memcpy(ndata, l5, l5); ndata += l5.size();						\
	*ndata = 0;														\
	StringDel(odata);												\
	return *this;

#define MSTRING_TEMPALTE_ADD9										\
	sz = l1.size() + l2.size() + l3.size() + l4.size() + l5.size() + l6.size() + l7.size() + l8.size() + l9.size(); \
	if(!sz){ StringDel(data); data = 0; return *this; }				\
	unsigned char *odata = data;									\
	StringNew(0);													\
	unsigned char *ndata = data;									\
	memcpy(ndata, l1, l1); ndata += l1.size();						\
	memcpy(ndata, l2, l2); ndata += l2.size();						\
	memcpy(ndata, l3, l3); ndata += l3.size();						\
	memcpy(ndata, l4, l4); ndata += l4.size();						\
	memcpy(ndata, l5, l5); ndata += l5.size();						\
	memcpy(ndata, l6, l6); ndata += l6.size();						\
	memcpy(ndata, l7, l7); ndata += l7.size();						\
	memcpy(ndata, l8, l8); ndata += l8.size();						\
	memcpy(ndata, l9, l9); ndata += l9.size();						\
	*ndata = 0;														\
	StringDel(odata);												\
	return *this;

#define MSTRING_TEMPALTE_ADDR9										\
	sz = l1.size() + l2.size() + l3.size() + l4.size() + l5.size() + l6.size() + l7.size() + l8.size() + l9.size(); \
	if(!sz){ StringDel(data); data = 0; return *this; }				\
	unsigned char *odata = data;									\
	StringNew(0);													\
	unsigned char *ndata = data;									\
	memcpy(ndata, l1, l1); l1.data = ndata; ndata += l1.size();		\
	memcpy(ndata, l2, l2); l2.data = ndata; ndata += l2.size();		\
	memcpy(ndata, l3, l3); l3.data = ndata; ndata += l3.size();		\
	memcpy(ndata, l4, l4); l4.data = ndata; ndata += l4.size();		\
	memcpy(ndata, l5, l5); l5.data = ndata; ndata += l5.size();		\
	memcpy(ndata, l6, l6); l6.data = ndata; ndata += l6.size();		\
	memcpy(ndata, l7, l7); l7.data = ndata; ndata += l7.size();		\
	memcpy(ndata, l8, l8); l8.data = ndata; ndata += l8.size();		\
	memcpy(ndata, l9, l9); l9.data = ndata; ndata += l9.size();		\
	*ndata = 0;														\
	StringDel(odata);												\
	return *this;

#define MSTRING_TEMPALTE_ADD19										\
	sz = l1.size() + l2.size() + l3.size() + l4.size() + l5.size() + l6.size() + l7.size() + l8.size() + l9.size()	\
		+ l10.size() + l11.size() + l12.size() + l13.size() + l14.size() + l15.size() + l16.size() + l17.size()		\
		+ l18.size() + l19.size();									\
	if(!sz){ StringDel(data); data = 0; return *this; }				\
	unsigned char *odata = data;									\
	StringNew(0);													\
	unsigned char *ndata = data;									\
	memcpy(ndata, l1, l1); ndata += l1.size();						\
	memcpy(ndata, l2, l2); ndata += l2.size();						\
	memcpy(ndata, l3, l3); ndata += l3.size();						\
	memcpy(ndata, l4, l4); ndata += l4.size();						\
	memcpy(ndata, l5, l5); ndata += l5.size();						\
	memcpy(ndata, l6, l6); ndata += l6.size();						\
	memcpy(ndata, l7, l7); ndata += l7.size();						\
	memcpy(ndata, l8, l8); ndata += l8.size();						\
	memcpy(ndata, l9, l9); ndata += l9.size();						\
	memcpy(ndata, l10, l10); ndata += l10.size();					\
	memcpy(ndata, l11, l11); ndata += l11.size();					\
	memcpy(ndata, l12, l12); ndata += l12.size();					\
	memcpy(ndata, l13, l13); ndata += l13.size();					\
	memcpy(ndata, l14, l14); ndata += l14.size();					\
	memcpy(ndata, l15, l15); ndata += l15.size();					\
	memcpy(ndata, l16, l16); ndata += l16.size();					\
	memcpy(ndata, l17, l17); ndata += l17.size();					\
	memcpy(ndata, l18, l18); ndata += l18.size();					\
	memcpy(ndata, l19, l19); ndata += l19.size();					\
	*ndata = 0;														\
	StringDel(odata);												\
	return *this;

