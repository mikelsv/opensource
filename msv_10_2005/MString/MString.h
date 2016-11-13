/*
 * Company:	MSV (MikelSV) code name Vyris
 * This: MString - My String class
 *		 My version &CString class
 *
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MString version 1.4.3 10/April/2005

	MString : Copyright (C) 2004 - 2005, Mishael Senin


				
	Covered code is provided under this license on an "as is" basis, without warranty
	of any kind, either expressed or implied, including, without limitation, warranties
	that the covered code is free of defects, merchantable, fit for a particular purpose
	or non-infringing. The entire risk as to the quality and performance of the covered
	code is with you. Should any covered code prove defective in any respect, you (not
	the initial developer or any other contributor) assume the cost of any necessary
	servicing, repair or correction. This disclaimer of warranty constitutes an essential
	part of this license. No use of any covered code is authorized hereunder except under
	this disclaimer.

	Permission is hereby granted to use, copy, modify, and distribute this
	source code, or portions hereof, for any purpose, including commercial applications,
	freely and without fee, subject to the following restrictions: 

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.

  --------------------------------------------------------------------------------


  --------------------------------------------------------------------------------
 */

#define		MSTRING_VER "1.4.3"

// My Tupes data

#ifndef NULL
#define NULL	0
#endif

#define uint	unsigned int

class MString
{
protected:
			  
	char * data;
	unsigned int sz;
	unsigned int rsz;

	void Init();

public:

	MString();
	~MString();

//	MString(const MString& string){ // new 10%
//	Init();
//	ASSERT(string.sz!=0);
//  
//	sz=string.sz;
//	data=new char[sz+1];
//	memcpy(data, string.data, sz);
//	data[sz]=0; 
//	}


	
	// ---------------------------------------- Create empty object  Создание пустого обьекта


//	MString(LPCSTR string);
//	MString(LPCTSTR string);
	MString(const char string);
	MString(const char* string);
	MString(const unsigned char* string);
	MString(const int code);
	MString(const MString& string);

//	MString::MString(LPCSTR lpsz){
//	sz=strlen(lpsz); LPTSTR odata=data;
//	data=new char[sz+1];
//	memcpy(data, lpsz, sz);
//	data[sz]=0;//	if (strlen(odata)) delete odata;
//	return ;
//	}



//	LPCTSTR


	// --------------------------------------------------------------------- Равно
	const MString& operator=(const MString& string);
	const MString& operator=(const char* string); // 100%
	const MString& operator=(char string);


	/*
#ifdef _UNICODE
	const MString& operator=(char ch);
#endif
	// copy string content from ANSI string (converts to TCHAR)
	const MString& operator=(LPCSTR lpsz);
	// copy string content from UNICODE string (converts to TCHAR)
	const MString& operator=(LPCWSTR lpsz);
	// copy string content from unsigned chars
	const MString& operator=(const unsigned char* psz);
  */




//	const MString& operator=(MString string);

	// ----------------------------------------------------------------- Add    Добавление	

//	friend MString& operator=+(const MString& string);
	const MString& operator+=(const MString& string);


	const MString& operator+=(const char string);


//	operator LPCTSTR() const{
//		return data;
//	}

	//---------------------------------------------------------------------  Move   Перемещение

	void move (MString& string, MString& is);

	MString& operator-=(MString& string);


	//----------------------------------------------------------------- Operators   Операторы
	
	bool operator!()const;

//	operator bool() const{
//	if (sz) return 1;
//	return 0;
//	}

	//operator bool(){
	//	return sz? 1:0;
	//}

	operator int();

//	bool operator!();
	
	//char operator[](int index) const{ char x; return x;};
//	MString operator[](int index) const;
	char operator[](int index)const;

//	operator void*(){
//	return data;
//	}

	char schar(){return data[0];}

	operator char*();




	//------------------------------------------------------------------- Index   Указатели

	char* pos(int p) const;


	void resize(unsigned int size);


//	friend CString AFXAPI operator+(const CString& string1,
//			const CString& string2);
//	friend CString AFXAPI operator+(const CString& string, TCHAR ch);
//	friend CString AFXAPI operator+(TCHAR ch, const CString& string);
//#ifdef _UNICODE
//	friend CString AFXAPI operator+(const CString& string, char ch);
//	friend CString AFXAPI operator+(char ch, const CString& string);
//#endif
//	friend CString AFXAPI operator+(const CString& string, LPCTSTR lpsz);
//	friend CString AFXAPI operator+(LPCTSTR lpsz, const CString& string);


	//----------------------------------------------------------------- Association Обьединение

	friend MString operator+(const MString& string, const MString& stringt);
 	friend MString operator+(const MString& string, char* lpsz);
	friend MString operator+(char* lpsz, const MString& string);
	
	//friend CString AFXAPI operator+(LPCTSTR lpsz, const CString& string);


	//-------------------------------------------------------------------------- Сравнение

//	bool MSTI(MString &string, MString &stringt);

	bool compare(const char *string, const unsigned int isz)const;
	bool moreless(MString string)const;
	
	bool incompare(MString string)const;

	/*
	friend bool operator==(const MString &string, const MString &stringt){
	if (string.sz!=stringt.sz) return 0;

	for (UINT i=0; i<string.sz; i++){
	if (string.data[i]!=stringt.data[i]) return 0;
	}
	return 1;
	}

	friend bool operator==(MString &string, MString &stringt){
	if (string.sz!=stringt.sz) return 0;

	for (UINT i=0; i<string.sz; i++){
	if (string.data[i]!=stringt.data[i]) return 0;
	}
	return 1;
	}

	friend bool operator==(const MString &string, const char* &stringt){
	if (string.sz!=strlen(stringt)) return 0;

	for (UINT i=0; i<string.sz; i++){
	if (string.data[i]!=stringt[i]) return 0;		   // asm++
	}
	return 1;
	}

	
	friend bool operator==(const char* &stringt, const MString &string){
	if (string.sz!=strlen(stringt)) return 0;					   // strlen - ok!

	for (UINT i=0; i<string.sz; i++){
	if (string.data[i]!=stringt[i]) return 0;		   // asm++
	}
	return 1;
	}

 */

//	int operator&&(const int into, const int intt){
//	if (into!=0 && intt!=0) return 1;
//	return 0;
//	}
 /*
	friend bool operator!=(const MString& string, const MString& stringt){
	if (string.sz!=stringt.sz) return 1;					   // strlen - ok!

	for (UINT i=0; i<string.sz; i++){
	if (string.data[i]!=stringt[i]) return 1;		   // asm++
	}
	return 0;
	}
   */

	//-------------------------------------------------------------------- Parametrs   Параметры


	int GetLength() const;

	int size() const;


	//-------------------------------------------------------------------------- Part   Часть

	MString Mid(int first, int size) const;
	MString Mid(int first) const;
	MString Left(int size) const;
	MString Right(int size) const;

	//---------------------------------------------------------------------- Operations  Операции

	bool IsEmpty() const;

	//void Empty(){
	//sz=0;
	//return;
	//}

	void Reserv(int size);

	void Clean(int p);

	void RClean(int sz, int p);

	void Empty();


	//---------------------------------------------------------------------- Functions	Функции

	int stoi();



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
	char * rchar() const;
	
	
	MString tosm();



//	operator char*(){
	//MString ret;
	//return data;
//	return data;
//	}		

//	operator LPTSTR(){
//	LPTSTR dat;
//	sz=strlen(data);
//	dat=new char[sz+1];
//	memcpy(dat, data, sz);
//	dat[sz]=0;
//	return dat;
//	}

//	int ok();

	//operator int(){
	//if (sz>0) return 1; else return 0;
	//}




//	LPTSTR MSLPC(){
//	char dat[strlen(data)+1];
//	memcpy(dat, data, sz+1);
//	return data;
// 	}
};

 
bool operator==(const MString& string, const MString& stringt);
bool operator==(const MString& string, const char* stringt);
bool operator==(const char* stringt, const MString& string);
bool operator==(const char stringt, const MString& string);
bool operator==(const MString& string, const char stringt);
bool operator!=(const MString& string, const MString& stringt);
bool operator!=(const MString& string, const char* stringt);
bool operator!=(const char* stringt, const MString& string);
bool operator!=(const char stringt, const MString& string);
bool operator!=(const MString& string, const char stringt);

bool operator>(const MString& string, const MString& stringt);
bool operator<(const MString& string, const MString& stringt);

MString operator&&(const MString& string, const MString& stringt);
MString operator||(const MString& string, const MString& stringt);


bool operator==(const MString& string, const int in);
bool operator!=(const MString& string, const int in);

bool operator>(const MString& string, const int in);
bool operator<(const MString& string, const int in);

bool operator>(const int in, const MString& string);
bool operator<(const int in, const MString& string);


//bool operator if(const int in, const MString& string);

//operator int(const MString string){int f=0; return f;}
	  
//bool operator==(MString& string, MString& stringt);
//bool operator==(MString& string, char* stringt);
//bool operator==(char* stringt, MString& string);
//bool operator!=(MString& string, MString& stringt);
//bool operator!=(MString& string, char* stringt);
//bool operator!=(char* stringt, MString& string);
		



// MSV Text Library for MString (MSVLibrary)
// MSVTL 1.12.04 (build 12)

int rts(const MString &line, MString fr, int &pos); //read to submol
int rtns(const MString &line, MString fr, int &pos); //read to not submol
int rrts(const MString &line, MString fr, int &pos); //reserv read to sumbol
int rrtns(const MString &line, MString fr, int &pos); //reserv read to no sumbol
int rtst(const MString &line, MString fr, int &pos, int to, bool res=0); //read to sumbol to
int rtms(const MString &line, MString fr, int &pos, bool res=0); //read to sumbol to
int rtmst(const MString &line, MString fr, int &pos, int to, bool res=0); //read to sumbol to
int rta(const MString &line, int o, int t, int &pos); // read to array
int rtna(const MString &line, int o, int t, int &pos); // read to no array
int rrtna(const MString &line, int o, int t, int &pos);	// reserv read to no array
int explode(MString *ret, MString line, MString fr, int c=0, bool a=0, bool r=0);
//int explodem(D &ret, MString line, MString fr, int c=0, bool a=0, bool r=0);

int zrout(MString ret, MString buf, int p, int t);
int GetParam(int *ret, MString param);
	// convert
MString itos(int val, int radix=10, int null=0);
int stoi(MString val);
MString ftos(float val, int ml=-1);
float stof(MString val);
MString itob(unsigned int val, int null=0);
int btoi(MString val);
MString xtos(void*x);
void* stox(MString line);
_int64 stoi64(MString val, int radix=10);
MString itos64(_int64 val, int radix=10, int null=0);

char stos(short s);	// short to char
int isnum(MString val); //is
MString Replace(const MString &line, MString fr, MString to);
MString replace(const MString &line, MString fr, MString to);
MString sons(MString line, int s, bool d, int dc);
MString ants(MString string, int n);
int tsor(MString &o, MString &t);
int wcslash(MString &val);
int cslash(MString &val);
MString dspace(MString line, int s); // удаление пробелов s - начало конец;
int dspacea(MString &line, int s); // удаление пробелов s - начало конец;  1,2,3
MString dinc(MString &line);
void tos(MString &line, int s);
int umn(int val);
int getpl(int val);

// is
bool ishandle(void* h);
bool IsHandle(void* h);




template <class D>
__inline void Delete(D *v){ if (v) delete v; return ;}

// MSV Link Library for MString (MSVLibrary)//////////////////////////////////////////////////////
// MSVLinkL 22.01.05 (build 13)///////////////////////////////////////////////////////////////////

//void Ilink(ILink &ret, MString link);
//int IlinkQ(ILinkQ *ret, MString line);
//MString RetQ(ILinkQ *lq, MString key, int slq);
//int Lexplode(ILinkQ &ret, const MString &line);
//MString HTI(const MString &line);


// MSV Bylte Library 4.2.6 (build 3)	//////////////////////////////////////////////////////////
int tobit(int *ret, int val);
int fobit(int *bit);
int tobit(bool *ret, int val);
int fobit(bool *bit);
