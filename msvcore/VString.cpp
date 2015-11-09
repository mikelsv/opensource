#include "msvdefine.h"
#include "VString.h"

// Compile
#include "crossplatform/prmf.cpp"
#include "crossplatform/rts.cpp"
#include "crossplatform/memory.cpp"

// class VString {

// --------------------------------------------------- Constructor  Конструктор

VString::VString(){ data=0; sz=0; } // MSVMEMORYCONTROLC
VString::~VString(){ } // MSVMEMORYCONTROLD

// --------------------------------------------------- Create object  Создание обьекта

VString::VString(const VString &line){ data=line.data; sz=line.sz;  }
VString::VString(const char *line){ data=(unsigned char*)line; if(line) sz=strlen((const char*)line); else sz=0; }
VString::VString(const unsigned char *line){ data=(unsigned char*)line; if(line) sz=strlen((const char*)line); else sz=0; }
VString::VString(const char *line, const unsigned int s){ data=(unsigned char*)line; if(s<S2G) sz=s; else sz=0; }
VString::VString(const unsigned char *line, const unsigned int s){ data=(unsigned char *)line; sz=s; }

// --------------------------------------------------- Set	Установка

void VString::set(VString line){ *this=line; }
void VString::set(const char* line, const unsigned int size){ sz=size; data=(unsigned char*)line; return ;}
void VString::setu(const unsigned char* line, const unsigned int size){ sz=size; data=(unsigned char*)line; return ;}
void VString::delq(){ if(sz>1 && *data=='"' && *(data+sz-1)=='"'){ data++; sz-=2; } }

//---------------------------------------------------- Operators   Операторы

bool VString::operator!()const{ if (sz>0) return 0; return 1; }
VString::operator void*()const { return data; }
inline VString::operator char*()const{ return (char*)data; }
inline VString::operator unsigned char*()const { return data; }
VString::operator int()const{ if(data) return sz; return 0; }
char VString::operator[](int index) const{ if(!sz || (unsigned int)index>=sz) return 0; return *(data+index); }
char VString::operator[](unsigned int index) const{ if(!sz || index>=sz) return 0; return *(data+index); }

//---------------------------------------------------- Index   Указатели

char * VString::rchar() const{ return (char*)data; }
unsigned char * VString::uchar() const{ return (unsigned char*)data;}
char* VString::pos(int p) const{ return (char*)(data+p); }
unsigned char* VString::upos(int p) const{ return data+p; }
char * VString::end() const{ return (char*)data+sz; }
unsigned char * VString::endu() const{ return data+sz; }
unsigned char VString::endo() const{ if(data && sz) return *(data+sz-1); return 0; }
//unsigned char VString::enduc() const { if(!sz || !data) return 0; return data[sz-1]; }
//char VString::goc(unsigned int index) const{ if(index<0 || index>=sz) return 0; return *(data+index); }

//---------------------------------------------------- Operations  Операции

bool VString::is()const{ return sz>0; }
bool VString::isempty()const{ return !data || !sz; }
unsigned int VString::size()const{ return data ? sz : 0; }

int VString::toi() const { return stoi((char*)data, sz); }
unsigned int VString::toui() const { return stoui((char*)data, sz); }
int64 VString::toi64() const { return stoi64((char*)data, sz); }
//uint64 VString::toui64() const { return stoui64((char*)data, sz); }
double VString::tod() const { return stod((char*)data, sz); }
//----------------------------------------------------------------------Part	Часть

VString VString::str(int pos) const{
	if(!data) return VString();
	if(pos<0) pos=sz+pos;
	if((int)sz<=pos) return VString();
	return VString(data+pos, sz-pos);
}

VString VString::str(int pos, int size) const{
	if(!data) return VString(); if(pos<0) pos+=sz;	if((unsigned int)pos>=sz) return VString();
	if(size<0) size+=sz;
	if((unsigned int)(size+pos)>sz) size=sz-pos; if(size<0) return VString();
	return VString(data+pos, size);
}

//----------------------------------------------------------------------Compare	Сравнение

bool VString::compare(const char *string, const unsigned int tsz) const{ return ::compare((char*)data, sz, (char*)string, tsz); }
bool VString::compareu(const char *string, const unsigned int tsz) const{ return ::compareu((char*)data, sz, (char*)string, tsz); }
bool VString::compareu(const VString &string) const{ return ::compareu((char*)data, sz, (char*)string.data, string.sz); }
int VString::comparesz(char *string, unsigned int tsz) const{ return ::comparesz((char*)data, sz, string, tsz); }
bool VString::incompare(const VString &line) const{ return ::incompare(data, sz, line.data, line.sz); }
bool VString::incompareu(const VString &line) const{ return ::incompareu(data, sz, line.data, line.sz); }

bool VString::morecompare(const VString& string)const{
	int size= sz>string.sz ? string.sz : sz;

	for (int i=0; i<size; i++){
		if(data[i]>string.data[i]) return 1;
		else if(data[i]<string.data[i]) return 0;
	}
	return sz>string.sz ? 1 : 0;
}

bool VString::morecompareu(const VString& string)const{
	int size= sz>string.sz ? string.sz : sz;

	for (int i=0; i<size; i++){
		if (upd(data[i])>upd(string.data[i])) return 1;
		else if(data[i]<string.data[i]) return 0;
	}
	return sz>string.sz ? 1 : 0;
}

void VString::Clean(){ sz=0; data=0; return ; }

// }; // end VString


bool operator==(const VString &string, const VString &stringt){
	return compare(string, string.sz, stringt, stringt.sz);
 }
bool operator==(const VString& string, const char* stringt){
	return compare(string, string.sz, (char*)stringt, (unsigned int)strlen(stringt));
}
bool operator==(char* stringt, const VString& string){
	return compare(string, string.sz, stringt, (unsigned int)strlen(stringt));
}

bool operator!=(const VString &string, const VString &stringt){
	return !compare(string, string.sz, stringt, stringt.sz);
}

bool operator!=(const VString& string, const char* stringt){
	return !compare(string, string.sz, (char*)stringt, (unsigned int)strlen(stringt));
}
bool operator!=(char* stringt, const VString& string){
	return !compare(string, string.sz, stringt, (unsigned int)strlen(stringt));
}

bool operator&&(const VString &string, const VString &stringt){ return string.is() && stringt.is(); }
bool operator&&(const bool bl, const VString &stringt){ return bl && stringt.is(); }
bool operator&&(const VString &string, const bool bl){ return string.is() && bl; }
bool operator&&(const VString &string, const int bl){ return string.is() && bl; }
bool operator&&(const VString &string, const unsigned int bl){ return string.is() && bl; }

// Compare
bool compare(const char *string, const unsigned int sz, const char *stringt, const unsigned int szt){
	if (sz!=szt) return 0; const char* to=string+sz;
	for (string; string<to; string++){ if ((unsigned char)*string!=(unsigned char)*stringt) return 0; stringt++; // asm++
	} return 1;
}

int comparesz(char *string, const unsigned int sz, char *stringt, const unsigned int szt){
	char* to=string+ (sz<szt ? sz : szt), *s=string;
        for (string; string<to; string++){ if ((unsigned char)*string!=(unsigned char)*stringt) break; stringt++;
	} return string-s; // asm++
}

bool compareu(char *string, const unsigned int sz, char *stringt, const unsigned int szt){
	if (sz!=szt) return 0; char* to=string+sz;
	for (string; string<to; string++){ if (upd(*string)!=upd(*stringt)) return 0; stringt++;  // asm++
	} return 1;
}

bool cmp(const char*str, const char*strt, unsigned int sz){return compare(str, sz, strt, sz); }
//bool cmpu(const VString &o, const VString &t){return o.sz==t.sz && compareu((char*)o.data, o.sz, (char*)t.data, t.sz); }
bool cmp(unsigned char*str, unsigned char*strt, unsigned int sz){return compare((char*)str, sz, (char*)strt, sz); }
bool cmpf(char*str, char*strt, unsigned int sz){if(strlen(str)>sz) return 0; return compare(str, strlen(str), strt, strlen(str)); }

bool incompare(const unsigned char *o, const int os, const unsigned char *t, const int ts){
	if(os>=ts){ const unsigned char *e=t+ts;
	for(t; t<e; t++){ if(*o!=*t) return 0; o++; } return 1;
	} return 0;
}

bool incompareu(const unsigned char *o, const int os, const unsigned char *t, const int ts){
	if(os>=ts){ const unsigned char *e=t+ts;
	for(t; t<e; t++){ if(upd(*o)!=upd(*t)) return 0; o++; } return 1;
	} return 0;
}

// Compare
bool compare(VString string, VString stringt){
	if (string.sz!=stringt.sz) return 0; unsigned char* to=string.data+stringt.sz;
	while(string.data<to){
		if (*string.data!=*stringt.data) return 0;  // asm++
		string.data++; stringt.data++;
	} return 1;
}

bool compareu(VString string, VString stringt){
	if (string.sz!=stringt.sz) return 0; unsigned char* to=string.data+stringt.sz;
	while(string.data<to){
		if (upd(*string.data)!=upd(*stringt.data)) return 0;  // asm++
		string.data++; stringt.data++;
	} return 1;
}

bool incompare(VString string, VString stringt){
		if (string.sz>=stringt.sz){
		for (unsigned int i=0; i<stringt.sz; i++){
		if (*string.data!=*stringt.data) return 0;
		string.data++; stringt.data++;
		} return 1; } return 0;
}

// PartLine
VString PartLine(VString line, VString &two, VString el){
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ two.Clean(); return line; }
	two.setu(pos+el.sz, line.endu()-pos-el.sz);
	return VString(line.uchar(), pos-line.uchar());
}

VString PartLineTwo(VString line, VString &two, VString el, VString el2){
	unsigned char *pos = line, *pos2 = pos;
	
	if(!rtms(line.endu(), el, el, pos))
		pos = line.endu();
	
	if(!rtms(line.endu(), el2, el2, pos2))
		pos2 = line.endu();

	pos = pos < pos2 ? pos : pos2;

	if(pos == line.endu()){
		 two.Clean();
		 return line;
	}

	two.setu(pos+el.sz, line.endu()-pos-el.sz);
	return VString(line.uchar(), pos-line.uchar());
}

VString PartLineST(VString line, VString &two){ // one = "Text Part ONe" [Space || Tab] "Text Part Two" -> two
	unsigned char *ln = line, *to =line.endu();

	while(ln < to && (*ln != ' ' || *ln !='\t'))
		ln ++;

	if(ln == to){
		two.Clean();
		return line;
	}

	line = line.str(ln - line.data);

	while(ln < to && (*ln == ' ' || *ln =='\t'))
		ln ++;

	two.setu(ln, to - ln);

	return line;
}

VString PartLineFind(VString line, VString el){
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ return VString(); }
	return VString(pos, el);
}

VString PartLineM(VString line, VString &two, VString &three, VString &four, VString &five, VString el){
	VString l=PartLine(line, line, el);
	two=PartLine(line, line, el);
	three=PartLine(line, line, el);
	four=PartLine(line, five, el);
	return l;
}

VString PartLineO(VString line, VString el){ // Get One
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ return line; }
	return VString(line.uchar(), pos-line.uchar());
}

VString PartLineIfO(VString line, VString el){ // Get One If found
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ return VString(); }
	return VString(line.uchar(), pos-line.uchar());
}

VString PartLineOT(VString line, VString el){ // Get One, tvo part
	unsigned char *pos=line;
	if(!rtms(line.endu(), el, el, pos)){ return VString(); }
	return VString(pos+el.sz, line.endu()-pos-el.sz);
}

VString PartLineDouble(VString line, VString el, VString er){
	unsigned char *pos=line, *lpos;
	if(!rtms(line.endu(), el, el, pos)){ return VString(); }
	lpos=pos+=el.sz; //el.setu(pos+el.sz, line.endu()-pos-el.sz);
	if(!rtms(line.endu(), er, er, pos)){ return VString(); }
	return VString(lpos, pos-lpos);
}

VString PartLineDoubleUp(VString line, VString el, VString er){
	unsigned char *pos=line, *lpos;
	if(!rtmsu(line.endu(), el, el, pos)){ return VString(); }
	lpos=pos+=el.sz; //el.setu(pos+el.sz, line.endu()-pos-el.sz);
	if(!rtmsu(line.endu(), er, er, pos)){ return VString(); }
	return VString(lpos, pos-lpos);
}



VString PartLineKV(VString line, VString &key, VString &val, VString elv, VString el){ // return line, set
	key=PartLine(line, line, el);
	key=PartLine(key, val, elv);
	return line;
}

// ParamLine
int ParamLine(VString line, VString &name, unsigned char *k, VString *v, int sz){
	int count=0;

	VString t;
	unsigned char *ln=line, *lln=ln, *to=line.endu(), lk=0, f=1;

	while(ln<to+1){
		if(ln==to || *ln=='.' || *ln==',' || *ln=='=' || *ln=='!' || *ln=='*' || *ln=='/' || *ln=='+'/* || *ln=='-'*/ || *ln=='!' || *ln=='~' || *ln==':'){
			t.setu(lln, ln-lln); dspacev(t, 7);

			if(f){
				f=0;
				name=t;
			}
			else{
				if(count<sz){
					k[count]=lk;
					v[count]=t;
					count++;
				}
			}

			lk=*ln; lln=ln+1;
		}
		ln++;
	}

	return count;
}

int ParamLineGet(int key, VString &val, unsigned char *k, VString *v, int sz){
	for(int i=0; i<sz; i++){
		if(k[i]==key){
			val=v[i];
			return 1;
		}
	}
	return 0;
}


// dspace
VString dspacev(VString &line, int s){ // удаление пробелов s - начало конец вернуть;  1,2,4
	unsigned char *ln=line, *to=line.endu();
	if(s&1) for(ln; ln<to; ln++){ if(*ln!=' ') break; }
	if(s&2) for(to; ln<to; to--){ if(*(to-1)!=' ') break; }
	if(s&4) return line=VString(ln, to-ln);
	return VString(ln, to-ln);
}

VString dspacevt(VString &line, int s){ // удаление пробелов и табов s - начало конец вернуть;  1,2,4
	unsigned char *ln=line, *to=line.endu();
	if(s&1) for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t' ) break; }
	if(s&2) for(to; ln<to; to--){ if(*(to-1)!=' ' && *(to-1)!='\t') break; }
	if(s&4) return line=VString(ln, to-ln);
	return VString(ln, to-ln);
}

// up & low
int up(unsigned char *line, unsigned int sz){
unsigned char *to=line+sz;
for (line; line<to; line++){
*line=upd(*line);
} return 0;
}

int low(unsigned char *line, unsigned int sz){
unsigned char *to=line+sz;
for (line; line<to; line++){
*line=lowd(*line);
} return 0;
}

int isnum(unsigned char *line, unsigned int sz){
	if(!sz || !line)
		return 0;

	for(unsigned int i=0; i<sz; i++){
		if(!isnumd(line[i]))
			return 0;
	}

	return 1;
}

#ifdef HAVE_WIN32
#error error: HAHAHA!
#endif