#include "msvdefine.h"
#include "VString.h"
#include "MString.h"
//#include "SString.h"
#include "TString.h"
#include "msvdefine_m.h"

//#include "SString.cpp"
#include "TString.cpp"

// + global function
TString LoadFile(VString file);
bool IsFile(VString file);
unsigned int SaveFile(VString file, VString data);

// Compile
#include "crossplatform/strint.cpp"
#include "crossplatform/ilink.cpp"
#include "crossplatform/confline.cpp"
#include "crossplatform/time.cpp"
//#include "msvil.cpp"

/*
// compile
#include "msvil.h"
#include "msvtl.h"
#include "crossplatform-time.h"
//#include "crossplatform-iheader.h"
#include "crossplatform-ilink.h"
#include "crossplatform-confline.h"
*/



// Class MString: public VString {
// --------------------------------------------------- Constructor  Конструктор

MString::MString(){ data=0; Init(); } // MSVMEMORYCONTROLC
MString::~MString(){ _deln(data); } // MSVMEMORYCONTROLD
void MString::Init(){ _deln(data); sz=0; }

// --------------------------------------------------- Create object  Создание обьекта
void MString::MStringNew(const void*line){
	if(!sz){ data=0; return ; }
	data=_newn(sz+1);
	if(!data){ sz=0; return ; }
	if(line) memcpy(data, line, sz);
	data[sz]=0;
	return ;
}

MString::MString(const char string){ sz=sizeof(string); MStringNew(&string); return ; }
MString::MString(const char *line){ if(line){ sz=strlen((const char*)line); MStringNew(line); } else { data=0; sz=0; } }
MString::MString(const unsigned char *line){ if(line){ sz=strlen((const char*)line); MStringNew(line); } else { data=0; sz=0; } }
MString::MString(const char *line, const unsigned int s){ sz=s; MStringNew(line); }
MString::MString(const unsigned char *line, const unsigned int s){ sz=s; MStringNew(line); }

MString::MString(const short string){ sz=sizeof(string); MStringNew(&string); return ; }
// short
MString::MString(const short *string, int size){
	if(!size){ data=0; return ; }
	MString ret=stoc((unsigned short*)string, size);
	data=ret.data; ret.data=0;
	sz=ret.sz; ret.sz=0;
	return ;
}
// int 
MString::MString(const int code){ sz=1; MStringNew(&code); return ; }
MString::MString(const unsigned int code){ sz=1; MStringNew(&code); return ; }
MString::MString(const unsigned int code, const unsigned int size){ sz=size; MStringNew(0); memset(data, code, sz); return ; }

MString::MString(const VString &line){ sz=line.sz; MStringNew(line); }
MString::MString(const MString &line){ sz=line.sz; MStringNew(line); }

#ifdef WIN32
MString::MString(MString &&line){ data = line.data; sz = line.sz; line.data = 0; line.sz = 0; }
#endif

// --------------------------------------------------- Set	Установка

const MString& MString::operator=(const char string){ unsigned char* odata=data; sz=1; MStringNew(&string); _del(odata); return *this; }
const MString& MString::operator=(const char* line){ unsigned char *odata=data; sz=line ? strlen((const char*)line) : 0; MStringNew(line); _deln(odata); return *this; }
const MString& MString::operator=(const unsigned char* line){ unsigned char *odata=data; sz=line ? strlen((const char*)line) : 0; MStringNew(line); _deln(odata); return *this; }
const MString& MString::operator=(const VString& line){ unsigned char *odata=data; sz=line; MStringNew(line); _deln(odata); return *this; }
const MString& MString::operator=(const MString& line){ unsigned char *odata=data; sz=line; MStringNew(line); _deln(odata); return *this; }

MString& MString::set(const VString line){ unsigned char *odata=data; sz=line.sz; MStringNew(line); _deln(odata); return *this; }
MString& MString::set(const char* line, const unsigned int size){ unsigned char *odata=data; sz=size;  MStringNew(line); _deln(odata); return *this; }
MString& MString::set(const unsigned char* line, const unsigned int size){ unsigned char *odata=data; sz=size;  MStringNew(line); _deln(odata); return *this; }
MString& MString::setu(const unsigned char* line, const unsigned int size){ unsigned char *odata=data; sz=size; MStringNew(line); _deln(odata); return *this; }

// --------------------------------------------------- Add  Добавление	

const MString& MString::operator+=(const VString& string){
	if(!string)
		return *this;

	int szo=sz; int szt=string.sz;
	sz=szo+szt; unsigned char* odata=data;
	MStringNew(0);
	memcpy(data, odata, szo);
	memcpy(data+szo, string.data, szt);
	if(sz) data[sz]=0; 
	if(odata) _del(odata);
	return *this;
}

const MString& MString::operator+=(const char string){
	int szo=sz; int szt=sizeof(string);

	sz=szo+szt; unsigned char* odata=data;
	MStringNew(0);
	memcpy(data, odata, szo);
	memcpy(data+szo, &string, szt);
	if(sz) data[sz]=0; 
	if(odata) _del(odata);
	return *this;
}

MString& MString::Add(VString o, VString t, VString th, VString f, VString fi){
	sz=o.size()+t.size()+th.size()+f.size()+fi.size(); if(!sz){ _deln(data); return *this;}
	unsigned char* odata=data;
	MStringNew(0); int p=0;
	memcpy(data, o, o); p+=o.size();
	memcpy(data+p, t, t); p+=t.size();
	memcpy(data+p, th, th); p+=th.size();
	memcpy(data+p, f, f); p+=f.size();
	memcpy(data+p, fi, fi); p+=fi.size();
	data[sz]=0; _del(odata);
return *this;
}

MString& MString::Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9){
	sz=l1.size()+l2.size()+l3.size()+l4.size()+l5.size()+l6.size()+l7.size()+l8.size()+l9.size();
	if(!sz){ _deln(data); return *this;}
	unsigned char* odata=data, *p;
	MStringNew(0); p=data;
	memcpy(p, l1, l1); p+=l1.sz;
	memcpy(p, l2, l2); p+=l2.sz;
	memcpy(p, l3, l3); p+=l3.sz;
	memcpy(p, l4, l4); p+=l4.sz;
	memcpy(p, l5, l5); p+=l5.sz;
	memcpy(p, l6, l6); p+=l6.sz;
	memcpy(p, l7, l7); p+=l7.sz;
	memcpy(p, l8, l8); p+=l8.sz;
	memcpy(p, l9, l9); p+=l9.sz;
	data[sz]=0; _del(odata);
return *this;
}

MString& MString::Add(VString l1, VString l2, VString l3, VString l4, VString l5, VString l6, VString l7, VString l8, VString l9,
	VString l10, VString l11, VString l12, VString l13, VString l14, VString l15, VString l16, VString l17, VString l18, VString l19){
	sz=l1.size()+l2.size()+l3.size()+l4.size()+l5.size()+l6.size()+l7.size()+l8.size()+l9.size()+
		l10.size()+l11.size()+l12.size()+l13.size()+l14.size()+l15.size()+l16.size()+l17.size()+l18.size()+l19.size();
	if(!sz){ _deln(data); return *this;}
	unsigned char* odata=data, *p;
	MStringNew(0); p=data;
	memcpy(p, l1, l1); p+=l1.sz;
	memcpy(p, l2, l2); p+=l2.sz;
	memcpy(p, l3, l3); p+=l3.sz;
	memcpy(p, l4, l4); p+=l4.sz;
	memcpy(p, l5, l5); p+=l5.sz;
	memcpy(p, l6, l6); p+=l6.sz;
	memcpy(p, l7, l7); p+=l7.sz;
	memcpy(p, l8, l8); p+=l8.sz;
	memcpy(p, l9, l9); p+=l9.sz;
	memcpy(p, l10, l10); p+=l10.sz;
	memcpy(p, l11, l11); p+=l11.sz;
	memcpy(p, l12, l12); p+=l12.sz;
	memcpy(p, l13, l13); p+=l13.sz;
	memcpy(p, l14, l14); p+=l14.sz;
	memcpy(p, l15, l15); p+=l15.sz;
	memcpy(p, l16, l16); p+=l16.sz;
	memcpy(p, l17, l17); p+=l17.sz;
	memcpy(p, l18, l18); p+=l18.sz;
	memcpy(p, l19, l19); p+=l19.sz;
	data[sz]=0; _del(odata);
return *this;
}

MString& MString::AddR(VString &l1, VString &l2, VString &l3, VString &l4, VString &l5, VString &l6, VString &l7, VString &l8, VString &l9){
	sz=l1.size()+l2.size()+l3.size()+l4.size()+l5.size()+l6.size()+l7.size()+l8.size()+l9.size();
	if(!sz){ _deln(data); return *this;}
	unsigned char* odata=data, *p;
	MStringNew(0); p=data;
	memcpy(p, l1, l1); l1.data=p; l1.sz=l1.sz; p+=l1.sz;
	memcpy(p, l2, l2); l2.data=p; l2.sz=l2.sz; p+=l2.sz;
	memcpy(p, l3, l3); l3.data=p; l3.sz=l3.sz; p+=l3.sz;
	memcpy(p, l4, l4); l4.data=p; l4.sz=l4.sz; p+=l4.sz;
	memcpy(p, l5, l5); l5.data=p; l5.sz=l5.sz; p+=l5.sz;
	memcpy(p, l6, l6); l6.data=p; l6.sz=l6.sz; p+=l6.sz;
	memcpy(p, l7, l7); l7.data=p; l7.sz=l7.sz; p+=l7.sz;
	memcpy(p, l8, l8); l8.data=p; l8.sz=l8.sz; p+=l8.sz;
	memcpy(p, l9, l9); l9.data=p; l9.sz=l9.sz; p+=l9.sz;
	data[sz]=0; _del(odata);
return *this;
}

//---------------------------------------------------- Move   Перемещение

void MString::move(MString& string){
	_del(data);
	sz=string.sz; data=string.data;
	string.data=0; string.sz=0;
	return ;
}

MString& MString::operator-=(MString& string){
	_del(data);
	sz=string.sz; data=string.data;
	string.data=0; string.sz=0;
	return *this;
}

//void MString::newsize(unsigned int s){ sz=s; return ; }

//---------------------------------------------------- Operators  Операторы
//---------------------------------------------------- Index  Указатели
//---------------------------------------------------- Association Обьединение

MString operator+(const MString& string, const MString& stringt){
	int szo=string.sz; int szt=stringt.sz;

    MString ret; ret.sz=szo+szt;
	ret.MStringNew(0); if(!ret.data) return 0;
	memcpy(ret.data, string.data, szo);
	memcpy(ret.data+szo, stringt.data, szt);
	ret.data[ret.sz]=0;
	return ret;
}

MString operator+(const MString& string, const VString& stringt){
	int szo=string.sz; int szt=stringt.sz;

    MString ret; ret.sz=szo+szt;
	ret.MStringNew(0); if(!ret.data) return 0;
	memcpy(ret.data, string.data, szo);
	memcpy(ret.data+szo, stringt.data, szt);
	ret.data[ret.sz]=0;
	return ret;
}


MString operator+(const MString& string, const char* lpsz){
	int szo=string.sz; if(!lpsz) return string;
	int szt=(unsigned int)strlen(lpsz);

	MString ret; ret.sz=szo+szt;
	ret.MStringNew(0);
	memcpy(ret.data, string.data, szo);
	memcpy(ret.data+szo, lpsz, szt);
	ret.data[ret.sz]=0;
	return ret;
}

MString operator+(const char* lpsz, const MString& string){
	int szo=string.sz; int szt=(unsigned int)strlen(lpsz);

	MString ret; ret.sz=szo+szt;
	ret.MStringNew(0);
	memcpy(ret.data, lpsz, szt);
	memcpy(ret.data+szt, string.data, szo);
	ret.data[ret.sz]=0;
	return ret;
}

//---------------------------------------------------- Part  Часть
/*
MString MString::str(const int p) const{
	if((int)sz<=p) return MString();
	return MString(data+p, sz-p);
}

MString MString::str(int pos, int size) const{
	if(!data) return MString(); if(pos<0) pos+=sz;	if((unsigned int)pos>=sz) return MString();
	if(size<0) size+=sz;
	if((unsigned int)(size+pos)>sz) size=sz-pos; if(size<0) return MString();
	return MString(data+pos, size);
}

// Old functions, please use str.
MString MString::Mid(int first) const{	// re 99%
	if(!data || first>=sz) return "";
	MString ret; if (first<0) first+=sz; ret.sz=sz-first;
	ret.MStringNew(0);
	memcpy(ret.data, data+first, sz-first);
	ret.data[ret.sz]=0;
	return ret;
}

MString MString::Mid(int first, int size) const{	// re 99%
	if(!data || size<0) return "";
	MString ret; if (first<0) first+=sz; ret.sz=size;
	ret.MStringNew(0);
	memcpy(ret.data, data+first, size);
	ret.data[ret.sz]=0;
	return ret;
}

MString MString::Left(int size) const{ // re 99%
	if(!data || size<0) return "";
	MString ret; ret.sz=size;
	ret.MStringNew(0);
	memcpy(ret.data, data, size);
	ret.data[ret.sz]=char(0);
	return ret;
}

MString MString::Right(int size) const{ // re 99%
	if(!data || size<=0) return "";
	MString ret; ret.sz=size;
	ret.MStringNew(0);
	memcpy(ret.data, data+sz-size, size);
	ret.data[ret.sz]=0;
	return ret;
}*/

//---------------------------------------------------- Operations  Операции

MString& MString::Reserv(int size, int p){
	if(size==sz) return *this; if(size<=0){ _del(data); data=0; sz=0; return *this; }
	unsigned char*odata=data; int s=sz; sz=size;
	MStringNew(0); int m=minel(s, size);
	memcpy(data, odata, m); _del(odata);
	memset(data+m, p, sz-m);
	data[size]=0;
	return *this;
}
/*
MString& MString::ReservM(int size, int p){
	if(size==sz) return *this; if(!size){ _del(data); data=0; sz=0; return *this; }
	unsigned char*odata=data; int s=sz; sz=size;
	MStringNew(0);
	memcpy(data, odata, min(s, size)); _del(odata);
	if(s<size) memset(data+s, p, size-s);
	data[size]=0;
	return *this;
}

void MString::RSize(int size, int p){
	if(size==sz) return ; if(!size){ _del(data); data=0; sz=0; return ; }
	unsigned char*odata=data; int s=sz; sz=size;
	MStringNew();
	memcpy(data, odata, min(s, size)); _del(odata);
	if(s<size) memset(data+s, p, size-s);
	data[size]=0;
}*/

void MString::RClean(int sz, int p){
	Reserv(sz);
	CleanData(p);
}

void MString::CleanData(int p){
	memset(data, p, sz);
}

void MString::Clean(){ Init(); }

// Danger Functions
//unsigned int MString::resize(const unsigned int size){return sz=size;}
//void MString::UseLeak(){ data=0; sz=0; }

//---------------------------------------------------- Functions  Функции
/*
int MString::toi(){
	return ::stoi(*this);
}
	
void* MString::tox(){
	return stox(*this);
}*/


// }; end MString