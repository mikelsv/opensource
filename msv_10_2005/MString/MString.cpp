// MString.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"
#include "MString.h"


typedef int	 BOOL;
typedef void *HANDLE;

/*
#define DLL_PROCESS_ATTACH 1    
#define DLL_THREAD_ATTACH  2    
#define DLL_THREAD_DETACH  3    
#define DLL_PROCESS_DETACH 0

#define TRUE true
#define FALSE false 

BOOL __stdcall DllMain( HANDLE hModule, 
                       unsigned int  ul_reason_for_call, 
                       void* lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

	
			*/



// MString.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"
//#include "windef.h"
//#include "winnt.h"
#include <string.h>
#include <math.h>
//#include "MSVSL.h"
#include "MSVTL.h"
#include "MSVIL.h"
#include "MSVBL.h"
#include "MMatrix.h"
#include "MMatrixCore.h"
#include "MMatrixSP.h"
#include "MSVILink.h"
//#include "MSVCF.h"
//#include "MSVLinkL.h"



/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
} */


MString::MString(){data=0; Init();}

MString::~MString(){
	Delete(data);
}


void MString::Init(){
	Delete(data);
	sz=0; rsz=0; data=NULL;
}



// ---------------------------------------- Create empty object  Создание пустого обьекта


/*
MString::MString(LPCSTR lpsz){
sz=strlen(lpsz); LPTSTR odata=data;
data=new char[sz+1];
memcpy(data, lpsz, sz);
data[sz]=0;//	if (strlen(odata)) delete odata;
return ;
}*/

MString::MString(const char string){
sz=sizeof(string);//	 LPTSTR odata=data;
if (!sz) {data=0; return ;}
data=new char[sz+1];
memcpy(data, &string, sz);
data[sz]=0;//	delete odata;
return ;
} 

MString::MString(const char* string){
sz=strlen(string);//	 LPTSTR odata=data;
if (!sz) {data=0; return ;}
data=new char[sz+1];
memcpy(data, string, sz);
data[sz]=0;//	delete odata;
return ;
} 

MString::MString(const unsigned char* string){
sz=sizeof(string);//	 LPTSTR odata=data;
if (!sz) {data=0; return ;}
data=new char[sz+1];
memcpy(data, string, sz);
data[sz]=0;//	delete odata;
return ;

}
MString::MString(const int code){
sz=1;//	 LPTSTR odata=data;
data=new char[2];
memcpy(data, &code, sz);
data[sz]=0;//	delete odata;
return ;
}

MString::MString(const MString& string){
sz=string.sz;//	 LPTSTR odata=data;
data=new char[sz+1];
memcpy(data, string.data, sz);
data[sz]=0;//	delete odata;
return ;
}

	// --------------------------------------------------------------------- Равно

const MString& MString::operator=(const char* string){ // 99%
sz=strlen(string); char* odata=data;
data=new char[sz+1];
memcpy(data, string, sz);
data[sz]=0;   if (odata) delete odata;
return *this;
}

const MString& MString::operator=(char string){	//90%
sz=1; char* odata=data;
data=new char[2];
memcpy(data, &string, 1);
data[1]=0;  if (odata) delete odata;
return *this;
}

const MString& MString::operator=(const MString& string){ // 99%
sz=string.sz;	char* odata=data;
data=new char[sz+1];
memcpy(data, string.data, sz);
data[sz]=0; if (odata) delete odata;
return *this;
}

	// ----------------------------------------------------------------- Add    Добавление	

const MString& MString::operator+=(const MString& string){
int szo=sz;
int szt=string.sz;

sz=szo+szt;		char* odata=data;
char* ndata=new char[sz+1];
memcpy(ndata, data, szo);
memcpy(ndata+szo, string.data, szt);
ndata[sz]=0; 
data=ndata;  if (odata) delete odata;
return *this;
}


const MString& MString::operator+=(const char string){
int szo=sz;
int szt=sizeof(string);

sz=szo+szt;	char* odata=data;
char* ndata=new char[sz+1];
memcpy(ndata, data, szo);
memcpy(ndata+szo, &string, szt);
ndata[sz]=0; 
data=ndata; if (odata) delete odata;
return *this;
}


	//---------------------------------------------------------------------  Move   Перемещение


void MString::move (MString& string, MString& is){
	if (string.data) delete string.data;
	string.sz=is.sz;
	string.data=is.data;
	is.data=0;
	return ;
}

MString& MString::operator-=(MString& string){
if (data) delete data;
sz=string.sz;
data=string.data;
string.data=0;
string.sz=0;
return *this;
}

	//----------------------------------------------------------------- Operators   Операторы
  

bool MString::operator!()const{
if (sz>0) return 0;
return 1;
}

MString::operator int(){
	if (data) return sz;
	return 0;
}	
	
char MString::operator[](int index) const{
return *(data+index);
}

/*
MString MString::operator[](const int index)const{
MString ret;
ret.sz=1;
ret.data=new char[2];
memcpy(ret.data, data+index, 1);
ret.data[1]=0;
return ret;
} */


MString::operator char*(){
return data;
}


	//------------------------------------------------------------------- Index   Указатели


char* MString::pos(int p) const{
return data+p;
}


void MString::resize(unsigned int size){
if (sz>size+1) sz=size;
data[size]=0;
return ;
}			



	MString operator+(const MString& string, const MString& stringt){
	int szo=string.sz;
	int szt=stringt.sz;

	MString ret;
	ret.sz=szo+szt;
	ret.data=new char[ret.sz+1];
	memcpy(ret.data, string.data, szo);
	memcpy(ret.data+szo, stringt.data, szt);
	ret.data[ret.sz]=0;

	return ret;
	}


	MString operator+(const MString& string, char* lpsz){
	int szo=string.sz;
	int szt=strlen(lpsz);

	MString ret;
	ret.sz=szo+szt;
	ret.data=new char[ret.sz+1];
	memcpy(ret.data, string.data, szo);
	memcpy(ret.data+szo, lpsz, szt);
	ret.data[ret.sz]=0;
	return ret;
	}

	MString operator+(char* lpsz, const MString& string){
	int szo=string.sz;
	int szt=strlen(lpsz);

	MString ret;
	ret.sz=szo+szt;
	ret.data=new char[ret.sz+1];
	memcpy(ret.data, lpsz, szt);
	memcpy(ret.data+szt, string.data, szo);
	ret.data[ret.sz]=0;
	return ret;
	}

				 





	//-------------------------------------------------------------------------- Сравнение


bool MString::compare(const char *string, const unsigned int isz)const{
	if (sz!=isz) return 0;

	for (unsigned int i=0; i<sz; i++){
	if (data[i]!=string[i]) return 0;		   // asm++
	}
	return 1;
	}
	
bool MString::incompare(MString string) const{
		if (sz>string.sz){
		for (unsigned int i=0; i<string.sz; i++){
		if (data[i]!=string.data[i]) return 0;		   // asm++
	}}
		else{
		if (sz>string.sz){
		for (unsigned int i=0; i<sz; i++){
		if (data[i]!=string.data[i]) return 0;		   // asm++
	}}}
	return 1;
	}

bool MString::moreless(MString string)const{
	int size= sz>string.sz ? sz : string.sz;

	for (int i=0; i<size; i++){
	if (*pos(i)>*string.pos(i)) return 1;
	}

return 0;
}


	//-------------------------------------------------------------------- Parametrs   Параметры


	int MString::GetLength() const{
	return sz;
	}

	int MString::size() const{
	return sz;
	}


	//-------------------------------------------------------------------------- Part   Часть

	MString MString::Mid(int first, int size) const{	// re 99%
	if(!data) return "";
	MString ret;
	ret.data=new char[size+1];
	ret.sz=size;
	memcpy(ret.data, data+first, size);
	ret.data[ret.sz]=0;
	return ret;
	}


	MString MString::Mid(int first) const{	// re 99%
	if(!data) return "";
	MString ret;
	ret.data=new char[sz-first+1];
	ret.sz=sz-first;
	memcpy(ret.data, data+first, sz-first);
	ret.data[ret.sz]=0;
	return ret;
	}

	MString MString::Left(int size) const{ // re 99%
	if(!data) return "";
	MString ret;
	ret.data=new char[size+1];
	ret.sz=size;
	memcpy(ret.data, data, size);
	ret.data[ret.sz]=char(0);
	return ret;
	}

	MString MString::Right(int size) const{ // re 99%
	if(!data) return "";
	MString ret;
	ret.data=new char[size+1];
	ret.sz=size;
	memcpy(ret.data, data+sz-size, size);
	ret.data[ret.sz]=0;
	return ret;

	}

	//---------------------------------------------------------------------- Operations  Операции

	bool MString::IsEmpty() const{
	if (sz==0) return 1; else return 0;
 	}

	//void Empty(){
	//sz=0;
	//return;
	//}

	void MString::Reserv(int size){
	delete data;
	sz=rsz=size;
	data=new char[size+1];
	data[size]=0;
	}

	void MString::Clean(int p){
	memset(data, p, sz);
	}

	void MString::RClean(int sz, int p){
	Reserv(sz);
	Clean(p);
	}

	void MString::Empty()
	{
	Init();
	}



//	MString operator[](int &ind){
//	MString ret;


//	return ret;
//	}
/*	friend MString operator[](MString string ,int index) const{
	MString ret;
//	ret.data=new char[2];
//	ret.sz=1;
//	memcpy(ret.data, data+index, size);
//	ret.data[2]=0;
	return ret;
	}
			 */
	char * MString::rchar() const{
	//MString ret;
	//return data;
	return data;
	}
	
	
	MString MString::tosm(){
	unsigned char *c= new unsigned char[sz];
	memcpy(&c, data, sz);
	//c[sz]=0;
	
	MString m;
	m.sz=sz;
	memcpy(data, &c, sz);
	data[sz]=0;
	return m;
	
	} 

	//---------------------------------------------------------------------- Functions	Функции

	int MString::stoi(){
		return ::stoi(*this);
 	}
	
	
	


// if ==
bool operator==(const MString& string, const MString& stringt){
	return string.compare(stringt.rchar(), stringt.size());
}  

bool operator==(const MString& string, const char* stringt){
	return string.compare(stringt, strlen(stringt));
}

bool operator==(const char* stringt, const MString& string){
	return string.compare(stringt, strlen(stringt));
}

bool operator==(const char stringt, const MString& string){
	return string.compare(&stringt, 1);
}

bool operator==(const MString& string, const char stringt){
	return string.compare(&stringt, 1);
}



// !=


bool operator!=(const MString& string, const MString& stringt){
	return !string.compare(stringt.rchar(), stringt.size());
}

bool operator!=(const MString& string, const char* stringt){
	return !string.compare(stringt, strlen(stringt));
}

bool operator!=(const char* stringt, const MString& string){
	return !string.compare(stringt, strlen(stringt));
}

bool operator!=(const char stringt, const MString& string){
	return !string.compare(&stringt, 1);
}

bool operator!=(const MString& string, const char stringt){
	return !string.compare(&stringt, 1);
}

// > <

bool operator>(const MString& string, const MString& stringt){
	return string.moreless(stringt);
}

bool operator<(const MString& string, const MString& stringt){
	return stringt.moreless(string);
}

// && ||

MString operator&&(const MString& string, const MString& stringt){
	return string.size() && stringt.size() ? MString(1) : MString();

}

MString operator||(const MString& string, const MString& stringt){
	return string.size() || stringt.size() ? MString(1) : MString();
}


// if   int

bool operator==(const MString& string, const int in){
if (*string.rchar()==in) return 1;
return 0;
}

bool operator!=(const MString& string, const int in){
if (*string.rchar()!=in) return 1;
return 0;
}


bool operator>(const MString& string, const int in){
if (*string.rchar()>in) return 1;
return 0;
}

bool operator<(const MString& string, const int in){
if (*string.rchar()<in) return 1;
return 0;
}

bool operator>(const int in, const MString& string){
if (*string.rchar()>in) return 1;
return 0;
}

bool operator<(const int in, const MString& string){
if (*string.rchar()<in) return 1;
return 0;
}


















//MString operator+(LPCSTR lpsz){
//sz=strlen(lpsz);
//data=new char[sz+1];
//memcpy(data, lpsz, sz);
//data[sz]=0;
//}

//MString::MString(CString string){
//sz=strlen(lpsz);
//data=new char[sz+1];
//memcpy(data, lpsz, sz);
//data[sz]=0;
//}

/*
bool MString::MSTI(MString &string, MString &stringt){
if (string.sz!=stringt.sz) return 0;

for (unsigned int i=0; i<string.sz; i++){
if (string.data[i]!=stringt.data[i]) return 0;

}


return 1;
}
  */


//int operator==(MString string, MString stringt){
//return string.MSTI(string, stringt);


//}



//const MString::MString =(char ch){

//}


//int MString::ok(){if (sz>0) return 1; else return 0;}



				 