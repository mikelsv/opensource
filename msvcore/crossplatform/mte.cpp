/*
 * Company:	MSV (MikelSV) code name Vyrix
 * This: MTE - MultiType class
 *		 My version variant class
 *
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MTE version 1.0.0 22/04/2007
	MTE : Copyright (C) 2007, Mishael Senin

*/

#define MTE_VER "0.1.0.0"

// Data Type
/*
#define DTX_DATATYPE_STR 0 // String ++ 1-4 bytes (next 8bytes), 0 is 4 bytes// Unsigned Char
#define DTX_DATATYPE_CHAR 1 // Char
#define DTX_DATATYPE_BYTE 2 // BYTE
#define DTX_DATATYPE_USHORT 3 // Unsigned Short
#define DTX_DATATYPE_SHORT 4 // Short
#define DTX_DATATYPE_UINT 5 // Unsigned Int
#define DTX_DATATYPE_INT 6 // Int
#define DTX_DATATYPE_UINT64 7 // Unsigned Int64
#define DTX_DATATYPE_INT64 8 // Int64
#define DTX_DATATYPE_UFLOAT 9 // Unsigned Float
#define DTX_DATATYPE_FLOAT 10 // Float
#define DTX_DATATYPE_UDOUBLE 11 // Double
#define DTX_DATATYPE_DOUBLE 12 // Unsigned Double
#define DTX_DATATYPE_SZ 13*/

#define DATATYPE_STR 0 // String ++ // Unsigned Char
#define DATATYPE_CHAR 1 // Char
#define DATATYPE_BYTE 2 // BYTE
#define DATATYPE_USHORT 3 // Unsigned Short
#define DATATYPE_SHORT 4 // Short
#define DATATYPE_UINT 5 // Unsigned Int
#define DATATYPE_INT 6 // Int
//#define DATATYPE_UINT64 7 // Unsigned Int64
#define DATATYPE_INT64 7 // Int64
//#define DATATYPE_UFLOAT 9 // Unsigned Float
#define DATATYPE_FLOAT 8 // Float
//#define DATATYPE_UDOUBLE 11 // Unsigned Double
#define DATATYPE_DOUBLE 9 // Double
#define DATATYPE_RGB 10 // RGB
#define DATATYPE_STRING 11

#define DATATYPE_SZ 11
#define DATATYPE_CALL_SZ 13

#define DATATYPE_VOID 12
#define DATATYPE_BOOL 13

// bcont
#define DATATYPE_UINT64	14
#define DATATYPE_VSTRING 15

struct DATATYPE{
BYTE type; BYTE size; BYTE ts;
MString name;
};

struct DATATYPED{
	VString name; BYTE size; BYTE type;
};

//#ifndef WIN32
//typedef DWORD   COLORREF;
//#define RGB(r,g,b)	((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
//typedef struct RGBQUAD {
//        BYTE    rgbBlue;
//        BYTE    rgbGreen;
//        BYTE    rgbRed;
//        BYTE    rgbReserved;
//}RGB;
//#define GetRValue(rgb)      ((BYTE)(rgb))
//#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
//#define GetBValue(rgb)      ((BYTE)((rgb)>>16))
//#endif

DATATYPE datatype[DATATYPE_SZ]={ // textsize
	DATATYPE_STR, 1, 1, "string", // 1
	DATATYPE_CHAR, 1, 1, "char", // 1
	DATATYPE_BYTE, 1, 2, "byte", // 2
	DATATYPE_USHORT, 2, 3, "ushort", // 3
	DATATYPE_SHORT, 2, 3, "short", // 3
	DATATYPE_UINT, 4, 6, "uint", // 6
	DATATYPE_INT, 4, 6,"int", // 6
	DATATYPE_INT64, 8, 11, "int64", // 11
	DATATYPE_FLOAT, 4, 7, "float", // L(64), val 6
	DATATYPE_DOUBLE, 8, 12, "double", // L(64), val 11
	DATATYPE_RGB, 3, 6, "rgb" // 6(0xFF)
};

// MULTITYPE CLASS EXTENDED
typedef class MultiType{ // MULTI TYPES EXTENDED
protected:
	int64 data;
	BYTE type;
	int*setv;
	BYTE block;
	BYTE setval;

public: MultiType(){memset(this, 0, sizeof(MultiType)); type=-1;}
	~MultiType(){Clear();}

void Block(int tp, int*s=0, BYTE addval=1){
	MultiType mt=*this; Clear(); type=tp;
	Blocked(mt, tp); block=1; setv=s;
	setval=addval;
	return ;
}

void Blocked(const MultiType&mt, BYTE tp){
	switch(tp){
	case DATATYPE_STR: (*(MString*)&data)=(MString &)mt; break;
	case DATATYPE_BYTE: (*(unsigned char*)&data)=mt; break;
	case DATATYPE_USHORT: (*(unsigned short*)&data)=mt; break;
	case DATATYPE_SHORT: (*(short*)&data)=mt; break;
	case DATATYPE_UINT: (*(unsigned int*)&data)=mt; break;
	case DATATYPE_INT: (*(int*)&data)=mt; break;
	case DATATYPE_INT64: (*(int64*)&data)=mt; break;
	case DATATYPE_FLOAT: (*(float*)&data)=mt; break;
	case DATATYPE_DOUBLE: (*(double*)&data)=mt; break;
	case DATATYPE_RGB: (*(RGBQUAD*)&data)=mt; break;
	} return ;
}

const void operator=(const MString& v){if(block) {Blocked(MultiType(v), type); *setv|=setval; return ;} Clear(); type=DATATYPE_STR; (*(MString*)&data)=v; return ; }
const void operator=(const VString& v){if(block) {Blocked(MultiType(v), type); *setv|=setval; return ;} Clear(); type=DATATYPE_STR; (*(MString*)&data)=v; return ; }

const void operator=(const unsigned char v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_BYTE; (*(unsigned char*)&data)=v; return ; }
const void operator=(const char v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_INT; (*(char*)&data)=v; return ; }
const void operator=(const unsigned short v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_USHORT; (*(unsigned short*)&data)=v; return ; }
const void operator=(const short v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_SHORT; (*(short*)&data)=v; return ; }
const void operator=(const unsigned int v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_UINT; (*(unsigned int*)&data)=v; return ; }
const void operator=(const int v){if(block) { Blocked(v, type); *setv|=setval; return ;} Clear(); type=DATATYPE_INT; (*(int*)&data)=v; return ; }
//const void operator=(const unsigned int64 v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_UINT64; (*(unsigned int64*)&data)=v; return ; }
const void operator=(const int64 v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_INT64; (*(int64*)&data)=v; return ; }
//const void operator=(const unsigned float v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_UFLOAT; (*(unsigned float*)&data)=v; return ; }
const void operator=(const float v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_FLOAT; (*(float*)&data)=v; return ; }
//const void operator=(const unsigned double v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_UDOUBLE; (*(unsigned double*)&data)=v; return ; }
const void operator=(const double v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_DOUBLE; (*(double*)&data)=v; return ; }
const void operator=(const RGBQUAD v){if(block) {MultiType mt=v; Blocked(mt, type); *setv|=setval; return ;} Clear(); type=DATATYPE_RGB; (*(RGBQUAD*)&data)=v; return ; }


MultiType(const MString& v){memset(this, 0, sizeof(MultiType)); type=DATATYPE_STR; (*(MString*)&data)=v; return ;}
MultiType(const VString& v){memset(this, 0, sizeof(MultiType)); type=DATATYPE_STR; (*(MString*)&data)=v; return ;}
MultiType(const unsigned char v){*this=v;}
MultiType(const char v){*this=v;}
MultiType(const unsigned short v){*this=v;}
MultiType(const unsigned int v){*this=v;}
MultiType(const int v){if(block) {block=0; *this=v; block=1;} else *this=v; }
MultiType(const int64 v){*this=v;}
MultiType(const float v){*this=v;}
MultiType(const double v){*this=v;}
MultiType(const RGBQUAD v){*this=v;}

MultiType(const MultiType &mte){
	type=mte.type; data=0;
	if(type<2) {(*(MString*)&data)=(*(MString*)&mte.data); }
	else data=mte.data;
}

const MultiType& operator=(const MultiType &mte){
	type=mte.type;
	if(type<2) {data=0; (*(MString*)&data)=(*(MString*)&mte.data); }
	else data=mte.data;
	return *this;
}


inline void Clear(){
if(type==DATATYPE_STR || type==DATATYPE_CHAR) {(*(MString*)&data).Clean();}
else data=0; type=-1;
return ;
}

void NDClear(){ type=0; data=0; setv=0; setval=0; block=0; }
	
int Type() const {return type;}
void* VData(){return &data;}

operator MString() const{
	switch(type){
	case DATATYPE_STR: return (*(MString*)&data); break;
	case DATATYPE_BYTE: return itos(*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return itos(*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return itos(*(short*)&data); break;
	case DATATYPE_UINT: return itos(*(unsigned int*)&data); break;
	case DATATYPE_INT: return itos(*(int*)&data); break;
	case DATATYPE_INT64: return itos(*(int64*)&data); break;
	case DATATYPE_FLOAT: return dtos(*(float*)&data); break;
	case DATATYPE_DOUBLE: return dtos(*(double*)&data); break;
	case DATATYPE_RGB: return itos((*(RGBQUAD*)&data).rgbRed, 16)+ itos((*(RGBQUAD*)&data).rgbGreen, 16)+itos((*(RGBQUAD*)&data).rgbBlue, 16); break;
	}
return "";
}

operator VString() const{ if(!data) return VString();
if(type==DATATYPE_STR || type==DATATYPE_CHAR || type==DATATYPE_BYTE) return (*(VString*)&data);

return VString();
}
/*
operator MString() const{
	switch(type){
	case DATATYPE_STR: return (*(MString*)&data); break;
	case DATATYPE_BYTE: return itos(*(unsigned char*)&data, 10, 1); break;
	case DATATYPE_USHORT: return itos(*(unsigned short*)&data, 10, 2); break;
	case DATATYPE_SHORT: return itos(*(short*)&data, 10, 2); break;
	case DATATYPE_UINT: return itos(*(unsigned int*)&data, 10, 4); break;
	case DATATYPE_INT: return itos(*(int*)&data, 10, 4); break;
	case DATATYPE_INT64: return itos64(*(int64*)&data, 10, 8); break;
	case DATATYPE_FLOAT: return dtos(*(float*)&data); break;
	case DATATYPE_DOUBLE: return dtos(*(double*)&data); break;
	}
return "";
}*/

operator char() const{
	switch(type){
	case DATATYPE_STR: return stoi(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(char*)&data); break;
	case DATATYPE_USHORT: return (char)(*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (char)(*(short*)&data); break;
	case DATATYPE_UINT: return (char)(*(unsigned int*)&data); break;
	case DATATYPE_INT: return (char)(*(int*)&data); break;
	case DATATYPE_INT64: return (char)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (char)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (char)(*(double*)&data); break;
	}
return 0;
}

operator unsigned char() const{
	switch(type){
	case DATATYPE_STR: return stoi(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (unsigned char)(*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (unsigned char)(*(short*)&data); break;
	case DATATYPE_UINT: return (unsigned char)(*(unsigned int*)&data); break;
	case DATATYPE_INT: return (unsigned char)(*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned char)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned char)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned char)(*(double*)&data); break;
	}
return 0;
}

operator unsigned short() const{
	switch(type){
	case DATATYPE_STR: return stoi(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned short)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned short)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned short)(*(double*)&data); break;
	}
return 0;
}

operator short() const{
	switch(type){
	case DATATYPE_STR: return stoi(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (short)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (short)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (short)(*(double*)&data); break;
	}
return 0;
}

operator unsigned int() const{
	switch(type){
	case DATATYPE_STR: return stoi(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned int)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned int)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned int)(*(double*)&data); break;
	case DATATYPE_RGB: return RGB(((RGBQUAD&)*&data).rgbRed, ((RGBQUAD&)*&data).rgbGreen, ((RGBQUAD&)*&data).rgbBlue); break;
	}
return 0;
}

operator int() const{
	switch(type){
	case DATATYPE_STR: return stoi(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (int)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (int)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (int)(*(double*)&data); break;
	}
return 0;
}

operator int64() const{
	switch(type){
	case DATATYPE_STR: return stoi64(*(VString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (*(int64*)&data); break;
	case DATATYPE_FLOAT: return (int64)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (int64)(*(double*)&data); break;
	}
return 0;
}

operator float() const{
	switch(type){
	case DATATYPE_STR: return (float)stod(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (float)(*(unsigned int*)&data); break;
	case DATATYPE_INT: return (float)(*(int*)&data); break;
	case DATATYPE_INT64: return (float)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (*(float*)&data); break;
	case DATATYPE_DOUBLE: return (float)(*(double*)&data); break;
	}
return 0;
}

operator double() const{
	switch(type){
	case DATATYPE_STR: return stod(*(MString*)&data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (double)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (*(float*)&data); break;
	case DATATYPE_DOUBLE: return (*(double*)&data); break;
	}
return 0;
}

operator RGBQUAD() const{ RGBQUAD ret={0,0,0}; char*ln;
	switch(type){
	case DATATYPE_STR:
		 ln=*(MString*)&data;
		 ret.rgbRed=((*ln>47 && *ln<58) ? *ln-48 : *ln-55)*16; ln++;
		 ret.rgbRed+=((*ln>47 && *ln<58) ? *ln-48 : *ln-55); ln++;
 		 ret.rgbGreen=((*ln>47 && *ln<58) ? *ln-48 : *ln-55)*16; ln++;
		 ret.rgbGreen+=((*ln>47 && *ln<58) ? *ln-48 : *ln-55); ln++;
		 ret.rgbBlue=((*ln>47 && *ln<58) ? *ln-48 : *ln-55)*16; ln++;
		 ret.rgbBlue+=((*ln>47 && *ln<58) ? *ln-48 : *ln-55); ln++;
	break;
//	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
//	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
//	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: ret.rgbRed=GetRValue(*(unsigned int*)&data); ret.rgbGreen=GetGValue(*(unsigned int*)&data); ret.rgbBlue=GetBValue(*(unsigned int*)&data);
//	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: ret.rgbRed=GetRValue(*(int64*)&data); ret.rgbGreen=GetGValue(*(int64*)&data); ret.rgbBlue=GetBValue(*(int64*)&data);
//	case DATATYPE_FLOAT: return (*(float*)&data); break;
//	case DATATYPE_DOUBLE: return (*(double*)&data); * / break;	
	case DATATYPE_RGB: return (*(RGBQUAD*)&data);
	}	
	return ret;
}		 


int ToType(VString v, BYTE tp){
MString ret; Clear();

if(tp==DATATYPE_STR || tp==DATATYPE_STRING){ type=DATATYPE_STR; *(MString*)&data=v; return 1; }
bool minus=0; if(v[0]=='-') { v.data++; v.sz--; minus=1;}

if(tp==DATATYPE_CHAR){ *this=(char)stoi(v); }
if(tp==DATATYPE_BYTE){ *this=(unsigned char)stoi(v); }
if(tp==DATATYPE_USHORT){ *this=(unsigned short)stoi(v); }
if(tp==DATATYPE_SHORT){ *this=(minus ? -1 : 1)*(short)stoi(v); }
if(tp==DATATYPE_UINT){ *this=(unsigned int)stoi(v); }
if(tp==DATATYPE_INT){ *this=(minus ? -1 : 1)*(int)stoi(v); }
//if(type==DATATYPE_UINT64){ i64=stoi64(str); if(minus) i64*=-1; ret.Reserv(8); *(int64*)ret.pos(0)=i64; return ret; }
if(tp==DATATYPE_INT64){ *this=(minus ? -1 : 1)*(int64)stoi64((VString)v); }
//if(type==DATATYPE_UFLOAT){ uf=stod(str); ret.Reserv(4); *(unsigned float*)ret.pos(0)=uf; return ret; }
if(tp==DATATYPE_FLOAT){ *this=(minus ? -1 : 1)*(float)stod(v); }
//if(type==DATATYPE_UDOUBLE){ ud=stod(str); ret.Reserv(8); *(unsigned double*)ret.pos(0)=ud; return ret; }
if(tp==DATATYPE_DOUBLE){ *this=(minus ? -1 : 1)*(double)stod(v); }
return 1;
}

int ToTypeBin(const char* v, unsigned int size, BYTE tp){
MString ret; Clear();

if(tp==DATATYPE_STR || tp==DATATYPE_CHAR) {type=DATATYPE_STR; /*if(!data) return 1;*/ *(MString*)&data=v; return 1; }
//bool minus=0; if(v[0]=='-') { v=v.str(1); minus=1;}

int sz=0; if(size<datatype[tp].size) sz=size; else sz=datatype[tp].size;
memcpy(&data, (void*)v, sz); type=tp;
return 1;
}

}MultiType;


// MULTITYPE CLASS EXTENDED
class MTEDTX{ // MULTI TYPES EXTENDED
protected:
	int64 data; 
	BYTE type;

public: MTEDTX(){memset(this, 0, sizeof(MTEDTX)); type=-1;}
	~MTEDTX(){Clear();}

const void operator=(const MString& v){ Clear(); type=DATATYPE_STR; (*(MString*)&data)=v; return ; }
const void operator=(const unsigned char v){Clear(); type=DATATYPE_BYTE; (*(unsigned char*)&data)=v; return ; }
const void operator=(const char v){Clear(); type=DATATYPE_INT; (*(char*)&data)=v; return ; }
const void operator=(const unsigned short v){Clear(); type=DATATYPE_USHORT; (*(unsigned short*)&data)=v; return ; }
const void operator=(const short v){Clear(); type=DATATYPE_SHORT; (*(short*)&data)=v; return ; }
const void operator=(const unsigned int v){Clear(); type=DATATYPE_UINT; (*(unsigned int*)&data)=v; return ; }
const void operator=(const int v){Clear(); type=DATATYPE_INT; (*(int*)&data)=v; return ; }
//const void operator=(const unsigned int64 v){Clear(); type=DATATYPE_UINT64; (*(unsigned int64*)&data)=v; return ; }
const void operator=(const int64 v){Clear(); type=DATATYPE_INT64; (*(int64*)&data)=v; return ; }
//const void operator=(const unsigned float v){Clear(); type=DATATYPE_UFLOAT; (*(unsigned float*)&data)=v; return ; }
const void operator=(const float v){Clear(); type=DATATYPE_FLOAT; (*(float*)&data)=v; return ; }
//const void operator=(const unsigned double v){Clear(); type=DATATYPE_UDOUBLE; (*(unsigned double*)&data)=v; return ; }
const void operator=(const double v){Clear(); type=DATATYPE_DOUBLE; (*(double*)&data)=v; return ; }
const void operator=(const MTEDTX v){ Clear(); if(v.type==DATATYPE_STR) (*(MString*)&data)=(MString &)v;
 else data=v.data; type=v.type; }


MTEDTX(const MString& v){*this=v;}
MTEDTX(const unsigned char v){*this=v;}
MTEDTX(const char v){*this=v;}
MTEDTX(const unsigned short v){*this=v;}
MTEDTX(const unsigned int v){*this=v;}
MTEDTX(const int v){*this=v;}
MTEDTX(const int64 v){*this=v;}
MTEDTX(const float v){*this=v;}
MTEDTX(const double v){*this=v;}
MTEDTX(const MTEDTX&mte){ Clear(); if(mte.type==DATATYPE_STR) (*(MString*)&data)=(MString &)mte; else data=mte.data; type=mte.type;}

operator MString() const{
	if(type==DATATYPE_STR || type==DATATYPE_CHAR) return (*(MString*)&data);
	else if(type>1) {
		MString ret; ret.Reserv(datatype[type].size); memcpy(ret.rchar(), (void*)&data, datatype[type].size); return ret;
	}
return "";
}

operator char() const{unsigned char t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=1 ? (*(MString*)&data).size() : 1)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (unsigned char)(*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (unsigned char)(*(short*)&data); break;
	case DATATYPE_UINT: return (unsigned char)(*(unsigned int*)&data); break;
	case DATATYPE_INT: return (unsigned char)(*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned char)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned char)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned char)(*(double*)&data); break;
	}
return 0;
}

operator unsigned char() const{unsigned char t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=1 ? (*(MString*)&data).size() : 1)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (unsigned char)(*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (unsigned char)(*(short*)&data); break;
	case DATATYPE_UINT: return (unsigned char)(*(unsigned int*)&data); break;
	case DATATYPE_INT: return (unsigned char)(*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned char)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned char)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned char)(*(double*)&data); break;
	}
return 0;
}

operator unsigned short() const{unsigned short t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=2 ? (*(MString*)&data).size() : 2)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned short)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned short)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned short)(*(double*)&data); break;
	}
return 0;
}

operator short() const{short t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=2 ? (*(MString*)&data).size() : 2)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (short)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (short)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (short)(*(double*)&data); break;
	}
return 0;
}

operator unsigned int() const{unsigned int t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=4 ? (*(MString*)&data).size() : 4)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (unsigned int)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (unsigned int)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (unsigned int)(*(double*)&data); break;
	}
return 0;
}

operator int() const{ int t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=4 ? (*(MString*)&data).size() : 4)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (int)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (int)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (int)(*(double*)&data); break;
	}
return 0;
}

operator int64() const{int64 t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=8 ? (*(MString*)&data).size() : 8)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (*(int64*)&data); break;
	case DATATYPE_FLOAT: return (int64)(*(float*)&data); break;
	case DATATYPE_DOUBLE: return (int64)(*(double*)&data); break;
	}
return 0;
}

operator float() const{ float t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=4 ? (*(MString*)&data).size() : 4)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (float)(*(unsigned int*)&data); break;
	case DATATYPE_INT: return (float)(*(int*)&data); break;
	case DATATYPE_INT64: return (float)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (*(float*)&data); break;
	case DATATYPE_DOUBLE: return (float)(*(double*)&data); break;
	}
return 0;
}

operator double() const{double t=0;
	switch(type){
	case DATATYPE_STR: memcpy(&t, (*(MString*)&data).rchar(), ((*(MString*)&data).size()<=8 ? (*(MString*)&data).size() : 8)); return t; break;
	case DATATYPE_BYTE: return (*(unsigned char*)&data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)&data); break;
	case DATATYPE_SHORT: return (*(short*)&data); break;
	case DATATYPE_UINT: return (*(unsigned int*)&data); break;
	case DATATYPE_INT: return (*(int*)&data); break;
	case DATATYPE_INT64: return (double)(*(int64*)&data); break;
	case DATATYPE_FLOAT: return (*(float*)&data); break;
	case DATATYPE_DOUBLE: return (*(double*)&data); break;
	}
return 0;
}

int ToType(const VString& v, BYTE tp){
Clear(); // MString ret;

if(tp==DATATYPE_STR || tp==DATATYPE_CHAR) {type=DATATYPE_STR; *(MString*)&data=v; return 1; }
//bool minus=0; if(v[0]=='-') { v=v.str(1); minus=1;}

int sz=0; if(v.size()<datatype[tp].size) sz=v.size(); else sz=datatype[tp].size;
memcpy(&data, (void*)v.rchar(), sz); type=tp;
return 1;
}

int ToType(const char* v, unsigned int size, BYTE tp){
MString ret; Clear();

if(tp==DATATYPE_STR || tp==DATATYPE_CHAR) {type=DATATYPE_STR; /*if(!data) return 1;*/ *(MString*)&data=v; return 1; }
//bool minus=0; if(v[0]=='-') { v=v.str(1); minus=1;}

int sz=0; if(size<datatype[tp].size) sz=size; else sz=datatype[tp].size;
memcpy(&data, (void*)v, sz); type=tp;
return 1;
}

inline void Clear(){
if(type==DATATYPE_STR || type==DATATYPE_CHAR) {(*(MString*)&data).Clean();}
else data=0; type=-1;
return ;
}

int Type() const {return type;}
void* VData(){
	if(type==DATATYPE_STR || type==DATATYPE_CHAR) { return (*(VString*)&data).rchar();}
	return &data;
}

int ModType(int nt){
if(type==nt) return 1;
	switch(nt){
	case DATATYPE_STR:{ MString ms=*this; ToType(ms, DATATYPE_STR); break;}
	case DATATYPE_BYTE:{ unsigned char c=*this; ToType((char*)&c, 1, DATATYPE_BYTE); break;}
	case DATATYPE_USHORT:{ unsigned short c=*this; ToType((char*)&c, 2, DATATYPE_USHORT); break;}
	case DATATYPE_SHORT:{ short c=*this; ToType((char*)&c, 2, DATATYPE_SHORT); break;}
	case DATATYPE_UINT:{ unsigned int c=*this; ToType((char*)&c, 4, DATATYPE_UINT); break;}
	case DATATYPE_INT:{ int c=*this; ToType((char*)&c, 4, DATATYPE_INT); break;}
	case DATATYPE_INT64:{ int64 c=*this; ToType((char*)&c, 8, DATATYPE_INT64); break;}
	case DATATYPE_FLOAT:{ float c=*this; ToType((char*)&c, 8, DATATYPE_FLOAT); break;}
	case DATATYPE_DOUBLE:{ double c=*this; ToType((char*)&c, 8, DATATYPE_DOUBLE); break;}
	default: return 0;
	}
	type=nt;
return 1;
}

void NDClear(){ type=0; data=0; }

};



// MULTITYPE CLASS EXTENDED VIRTUAL
#define MTEV_VER "0.1.0.0"
#define MTEV_DTE "12.11.2007 16:04"

class MTEV{ // MULTI TYPES EXTENDED
protected:
	char* data;
//#if (sizeof(data)==4)
	char *nouse;
//#endif
	unsigned int sz;
	BYTE type;

public: MTEV(){data=0; sz=0; type=-1;}
	~MTEV(){Clear();}

inline void SetData(char *d, unsigned int s, BYTE t){
	data=d; sz=s; type=t; return ;
}

inline void SetData(char *d){
	data=d; return ;
}

void MoveData(unsigned int d){ data+=d; return ; }

inline void set(char *d, unsigned int s, BYTE t){
	data=d; sz=s; type=t; return ;
}

inline void settype(BYTE t, unsigned int s){
	type=t; sz=s; return ;
}

inline void Clear(){
data=0; sz=0; type=-1;
return ;
}
	
int Type() const {return type;}
char* VData()const {return data;}
inline unsigned int size()const{return sz;}
bool Is(){if(type!=255) return 1; return 0;}

char* Mov(unsigned int p){return data+=p;}

const void operator=(const MString& v){ Clear(); type=DATATYPE_STR; data=(char*)v.rchar(); sz=v.size(); return ; }
const void operator=(const VString v){ Clear(); type=DATATYPE_STR; data=(char*)v.rchar(); sz=v.size(); return ; }
const void operator=(const unsigned char& v){Clear(); type=DATATYPE_BYTE; data=(char*)&v; sz=1; return ; }
const void operator=(const char& v){Clear(); type=DATATYPE_INT; data=(char*)&v; sz=1; return ; }
const void operator=(const unsigned short& v){Clear(); type=DATATYPE_USHORT; data=(char*)&v; sz=1; return ; }
const void operator=(const short& v){Clear(); type=DATATYPE_SHORT; data=(char*)&v; sz=1; return ; }
const void operator=(const unsigned int& v){Clear(); type=DATATYPE_UINT; data=(char*)&v; sz=1; return ; }
const void operator=(const int& v){Clear(); type=DATATYPE_INT; data=(char*)&v; sz=1; return ; }
//const void operator=(const unsigned int64 v){Clear(); type=DATATYPE_UINT64; (*(unsigned int64*)&data)=v; sz=1; return ; }
const void operator=(const int64& v){Clear(); type=DATATYPE_INT64; data=(char*)&v; sz=1; return ; }
//const void operator=(const unsigned float v){Clear(); type=DATATYPE_UFLOAT; (*(unsigned float*)&data)=v; sz=1; return ; }
const void operator=(const float& v){Clear(); type=DATATYPE_FLOAT; data=(char*)&v; sz=1; return ; }
//const void operator=(const unsigned double v){Clear(); type=DATATYPE_UDOUBLE; (*(unsigned double*)&data)=v; sz=1; return ; }
const void operator=(const double& v){Clear(); type=DATATYPE_DOUBLE; data=(char*)&v; sz=1; return ; }
//const void operator=(const MTEDTX v){ Clear(); if(v.type==DATATYPE_STR) (*(MString*)&data)=&v; else data=v.data; type=v.type; sz=1; }
//const void operator=(const MRGB& v){Clear(); type=DATATYPE_RGB; (*(RGBQUAD*)&data)=v; sz=1; return ; }

MTEV(const MString& v){*this=v;}
MTEV(const VString& v){*this=v;}
MTEV(const unsigned char& v){*this=v;}
MTEV(const char& v){*this=v;}
MTEV(const unsigned short& v){*this=v;}
MTEV(const unsigned int& v){*this=v;}
MTEV(const int& v){*this=v;}
MTEV(const int64& v){*this=v;}
MTEV(const float& v){*this=v;}
MTEV(const double& v){*this=v;}
//MTEV(const MRGB& v){*this=v;}

//MTEDTX(const MTEDTX&mte){ Clear(); if(mte.type==DATATYPE_STR) (*(MString*)&data)=mte; else data=mte.data; type=mte.type;}


operator MString() const{ if(!data) return MString();
if(type==DATATYPE_STR || type==DATATYPE_CHAR || type==DATATYPE_BYTE) return MString((unsigned char*)data, sz);

if(sz==1)
	switch(type){
//	case DATATYPE_STR:	return (*(MString*)&data); break;
//	case DATATYPE_CHAR: return itos(*(char*)&data); break;
	case DATATYPE_BYTE: return itos(*(unsigned char*)data); break;
	case DATATYPE_USHORT: return itos(*(unsigned short*)data); break;
	case DATATYPE_SHORT: return itos(*(short*)data); break;
	case DATATYPE_UINT: return itos(*(unsigned int*)data); break;
	case DATATYPE_INT: return itos(*(int*)data); break;
	case DATATYPE_INT64: return itos(*(int64*)data); break;
	case DATATYPE_FLOAT: return dtos (*(float*)data, 5); break;
	case DATATYPE_DOUBLE: return dtos(*(double*)data, 5); break;
	//case DATATYPE_RGB: return itos((*(RGBQUAD*)&data).rgbRed, 16)+ itos((*(RGBQUAD*)&data).rgbGreen, 16)+itos((*(RGBQUAD*)&data).rgbBlue, 16); break;
	case DATATYPE_RGB: return itos((*(RGBQUAD*)data).rgbRed*S64K+(*(RGBQUAD*)data).rgbGreen*256+(*(RGBQUAD*)data).rgbBlue, 16); break;
	}
else{
LString ret;
	switch(type){
//	case DATATYPE_STR:	ret+=(*(MString*)data); break;
//	case DATATYPE_CHAR: for(int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(char*)data); } return ret;
	case DATATYPE_BYTE: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(unsigned char*)data); } return ret;
	case DATATYPE_USHORT: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(unsigned short*)data); } return ret;
	case DATATYPE_SHORT: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(short*)data); } return ret;
	case DATATYPE_UINT: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(unsigned int*)data); } return ret;
	case DATATYPE_INT: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(int*)data); } return ret;
	case DATATYPE_INT64: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos(*(int64*)data); } return ret;
	case DATATYPE_FLOAT: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=dtos(*(float*)data); } return ret;
	case DATATYPE_DOUBLE: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=dtos(*(double*)data); } return ret;
	case DATATYPE_RGB: for(unsigned int i=0; i<sz; i++){if(i) ret+=" "; ret+=itos((*(RGBQUAD*)&data).rgbRed*S64K+(*(RGBQUAD*)&data).rgbGreen*256+(*(RGBQUAD*)&data).rgbBlue, 16); } return ret;
	}
}
return MString();
}


operator VString() const{ if(!data) return VString();
if(type==DATATYPE_STR || type==DATATYPE_CHAR || type==DATATYPE_BYTE) return VString((unsigned char*)data, sz);

return VString();
}

operator unsigned char() const{
	switch(type){
	case DATATYPE_STR: return stoi(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (unsigned char)(*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (unsigned char)(*(short*)data); break;
	case DATATYPE_UINT: return (unsigned char)(*(unsigned int*)data); break;
	case DATATYPE_INT: return (unsigned char)(*(int*)data); break;
	case DATATYPE_INT64: return (unsigned char)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (unsigned char)(*(float*)data); break;
	case DATATYPE_DOUBLE: return (unsigned char)(*(double*)data); break;
	}
return 0;
}

operator bool() const{
	switch(type){
	//case DATATYPE_STR: return VString((unsigned char*)data, sz); break;
	case DATATYPE_STR: return data && sz; break;
	case DATATYPE_CHAR: return (*(char*)data)>0; break;
	case DATATYPE_BYTE: return (*(unsigned char*)data)>0; break;
	case DATATYPE_USHORT: return (*(unsigned short*)data)>0; break;
	case DATATYPE_SHORT: return (*(short*)data)>0; break;
	case DATATYPE_UINT: return (*(unsigned int*)data)>0; break;
	case DATATYPE_INT: return (*(int*)data)>0; break;
	case DATATYPE_INT64: return (*(int64*)data)>0; break;
	case DATATYPE_FLOAT: return (*(float*)data)>0; break;
	case DATATYPE_DOUBLE: return (*(double*)data)>0; break;
	}
return 0;
}

operator unsigned short() const{
	switch(type){
	case DATATYPE_STR: return stoi(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (*(unsigned int*)data); break;
	case DATATYPE_INT: return (*(int*)data); break;
	case DATATYPE_INT64: return (unsigned short)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (unsigned short)(*(float*)data); break;
	case DATATYPE_DOUBLE: return (unsigned short)(*(double*)data); break;
	}
return 0;
}

operator short() const{
	switch(type){
	case DATATYPE_STR: return stoi(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (*(unsigned int*)data); break;
	case DATATYPE_INT: return (*(int*)data); break;
	case DATATYPE_INT64: return (short)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (short)(*(float*)data); break;
	case DATATYPE_DOUBLE: return (short)(*(double*)data); break;
	}
return 0;
}

operator unsigned int() const{
	switch(type){
	case DATATYPE_STR: return stoi(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (*(unsigned int*)data); break;
	case DATATYPE_INT: return (*(int*)data); break;
	case DATATYPE_INT64: return (unsigned int)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (unsigned int)(*(float*)data); break;
	case DATATYPE_DOUBLE: return (unsigned int)(*(double*)data); break;
	case DATATYPE_RGB: return RGB((*(RGBQUAD*)data).rgbRed, (*(RGBQUAD*)data).rgbGreen, (*(RGBQUAD*)data).rgbBlue); break;
	}
return 0;
}

operator int() const{ //stoi(*(MString*)data)
	switch(type){
	case DATATYPE_STR: return stoi(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (*(unsigned int*)data); break;
	case DATATYPE_INT: return (*(int*)data); break;
	case DATATYPE_INT64: return (int)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (int)(*(float*)data); break;
	case DATATYPE_DOUBLE: return (int)(*(double*)data); break;
	}
return 0;
}

operator int64() const{
	switch(type){
	case DATATYPE_STR: return stoi64(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (*(unsigned int*)data); break;
	case DATATYPE_INT: return (*(int*)data); break;
	case DATATYPE_INT64: return (*(int64*)data); break;
	case DATATYPE_FLOAT: return (int64)(*(float*)data); break;
	case DATATYPE_DOUBLE: return (int64)(*(double*)data); break;
	}
return 0;
}

operator float() const{
	switch(type){
	case DATATYPE_STR: return (float)stod(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (float)(*(unsigned int*)data); break;
	case DATATYPE_INT: return (float)(*(int*)data); break;
	case DATATYPE_INT64: return (float)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (*(float*)data); break;
	case DATATYPE_DOUBLE: return (float)(*(double*)data); break;
	}
return 0;
}

operator double() const{
	switch(type){
	case DATATYPE_STR: return stod(VString((unsigned char*)data, sz)); break;
	case DATATYPE_CHAR: return (*(char*)data); break;
	case DATATYPE_BYTE: return (*(unsigned char*)data); break;
	case DATATYPE_USHORT: return (*(unsigned short*)data); break;
	case DATATYPE_SHORT: return (*(short*)data); break;
	case DATATYPE_UINT: return (*(unsigned int*)data); break;
	case DATATYPE_INT: return (*(int*)data); break;
	case DATATYPE_INT64: return (double)(*(int64*)data); break;
	case DATATYPE_FLOAT: return (*(float*)data); break;
	case DATATYPE_DOUBLE: return (*(double*)data); break;
	}
return 0;
}

void SetInData(MTEV mt){ if(!data) return ;
	switch(type){
	case DATATYPE_STR:	*(MString*)data=(VString)mt; break;
//	case DATATYPE_CHAR: *(char*)data=mt;
	case DATATYPE_BYTE: *(BYTE*)data=mt;
	case DATATYPE_USHORT: *(unsigned short*)data=mt; break;
	case DATATYPE_SHORT: *(short*)data=mt; break;
	case DATATYPE_UINT: *(unsigned int*)data=mt; break;
	case DATATYPE_INT: *(int*)data=mt; break;
	case DATATYPE_INT64: *(int64*)data=mt; break;
	case DATATYPE_FLOAT: *(float*)data=mt; break;
	case DATATYPE_DOUBLE: *(double*)data=mt; break;
	//case DATATYPE_RGB: *(RGBQUAD*)data=mt; break;
		break;
	}
return ;
}

};

class MTEVU{
	union{
		char *c;
		unsigned char *uc;
		short* s;
		unsigned short *us;
		unsigned int *ui;
		int *i;
		float *f;
		double *d;
		int64 *i6;
		VString *vs;
		MString *ms;
	};
	int tp;
public:
// Constructor
	MTEVU(){tp=-1; vs=0; }
	MTEVU(VString&mv) { vs=&mv; tp=DATATYPE_STR; }
	MTEVU(MString&mv) { ms=&mv; tp=DATATYPE_STR; }
	MTEVU(unsigned char &v) { uc=&v; tp=DATATYPE_BYTE; }
	MTEVU(unsigned short &v) { us=&v; tp=DATATYPE_USHORT; }
	MTEVU(short &v) { s=&v; tp=DATATYPE_SHORT; }
	MTEVU(unsigned int &v) { ui=&v; tp=DATATYPE_UINT; }
	MTEVU(int &v) { i=&v; tp=DATATYPE_INT; }
	MTEVU(int64 &v) { i6=&v; tp=DATATYPE_INT64; }
	MTEVU(float &v) { f=&v; tp=DATATYPE_FLOAT; }
	MTEVU(double &v) { d=&v; tp=DATATYPE_DOUBLE; }

// Options
	BYTE type()const{ return tp; }
	bool is(){ return c!=0; }

// ==
	void operator=(VString&mv){ vs=&mv; tp=DATATYPE_STR; }
	void operator=(MString&mv){ ms=&mv; tp=DATATYPE_STR; }

// Return
	char*GetData(){ return c; }
	operator VString*(){ if(tp==DATATYPE_STR) return vs; return 0; }
	operator MTEV(){
		MTEV m; if(tp!=DATATYPE_STR) m.set(c, 1, tp);
		else { if(vs) m.set(*vs, *vs, tp); else m.set(0,0,tp);  }
		 return m;
	}

// Clear
	bool ClearVData(){ if(!i) return 0;
	switch(tp){
	case DATATYPE_STR: vs->Clean(); break;// return stod(*(MString*)data); break;
	case DATATYPE_CHAR: c=0; break;
	case DATATYPE_BYTE: uc=0; break;
	case DATATYPE_USHORT: us=0; break;
	case DATATYPE_SHORT: s=0; break;
	case DATATYPE_UINT: *ui=0; break;
	case DATATYPE_INT: *i=0; break;
	case DATATYPE_INT64: *i6=0; break;
	case DATATYPE_FLOAT: *f=0; break;
	case DATATYPE_DOUBLE: *d=0; break;
	default: return 0;
	} return 1;
	}

	bool SetR(MTEV mt){ if(!is()) return 0;
		switch(tp){
			case DATATYPE_STR: *ms=(VString)mt; return 1;// return stod(*(MString*)data); break;
//			case DATATYPE_CHAR: *c=mt; return 1;
			case DATATYPE_BYTE: *uc=mt; return 1;
			case DATATYPE_USHORT: *us=mt; return 1;
			case DATATYPE_SHORT: *s=mt; return 1;
			case DATATYPE_UINT: *ui=mt; return 1;
			case DATATYPE_INT: *i=mt; return 1; //return (*(int*)data); break;
			case DATATYPE_INT64: *i6=mt; return 1; //return (double)(*(int64*)data); break;
			case DATATYPE_FLOAT: *f=mt; return 1;
			case DATATYPE_DOUBLE: *d=mt; return 1;
			default: return 0;
		} return 1;
	}

};


#ifdef IHAVE_USE_MTEVS

class MTEVS{
char data[32]; VString ret;
public:

MTEVS(const MTEV &mtev){
	if(mtev.Type()==DATATYPE_STR) ret=mtev;
	else { Itos it; void *d=mtev.VData();
		switch(mtev.Type()){
	//	case DATATYPE_STR:	return (*(MString*)&data); break;
	//	case DATATYPE_CHAR: return itos(*(char*)&data); break;
		case DATATYPE_BYTE: it.itos(*(unsigned char*)d); break;
		case DATATYPE_USHORT: it.itos(*(unsigned short*)d); break;
		case DATATYPE_SHORT: it.itos(*(short*)d); break;
		case DATATYPE_UINT: it.itos(*(unsigned int*)d); break;
		case DATATYPE_INT: it.itos(*(int*)d); break;
		case DATATYPE_INT64: it.itos64(*(int64*)d); break;
		case DATATYPE_FLOAT: it.dtos(*(float*)d, 5); break;
		case DATATYPE_DOUBLE: it.dtos(*(double*)d, 5); break;
		//case DATATYPE_RGB: return itos((*(RGBQUAD*)&data).rgbRed, 16)+ itos((*(RGBQUAD*)&data).rgbGreen, 16)+itos((*(RGBQUAD*)&data).rgbBlue, 16); break;
		case DATATYPE_RGB: it.itos((*(RGBQUAD*)d).rgbRed*S64K+(*(RGBQUAD*)d).rgbGreen*256+(*(RGBQUAD*)d).rgbBlue, 16); break;
		}
		memcpy(&data, it.ret, it<32 ? it : 32); ret.set((char*)&data, it<32 ? it : 32);
	}
}

operator VString(){ return ret; }


};

#endif


// New Mtev
#define MTEVAT_INT		1
#define MTEVAT_FLOAT	2
#define MTEVAT_STR		3

class MTEVA{ public:
	unsigned int tp, sz;
	union{ int64 i; double f; }; VString v;

	// Insert	
	MTEVA(){ tp=0; sz=0; i=0; }

	MTEVA(unsigned char v){ i=v; sz=1; tp=MTEVAT_INT; }
	MTEVA(unsigned short v){ i=v; sz=2; tp=MTEVAT_INT; }
	MTEVA(short v){ i=v; sz=2; tp=MTEVAT_INT; }
	MTEVA(unsigned int v){ i=v; sz=4; tp=MTEVAT_INT; }
	MTEVA(int v){ i=v; sz=4; tp=MTEVAT_INT; }
	MTEVA(int64 v){ i=v; sz=8; tp=MTEVAT_INT; }
	MTEVA(float v){ f=v; sz=4; tp=MTEVAT_FLOAT; }
	MTEVA(double v){ f=v; sz=8; tp=MTEVAT_FLOAT; }
	//MTEVA(VString s){ v=s; sz=8; tp=MTEVAT_STR; }
	MTEVA(const VString s){ v=s; sz=8; tp=MTEVAT_STR; }
	MTEVA(const char* s){ v=VString(s); sz=8; tp=MTEVAT_STR; }
	MTEVA(MACADDR_S s){ i=0; memcpy(&i, &s, 6); sz=8; tp=MTEVAT_INT; }
	MTEVA(IP6_ADDRESS &s){ v.data=(unsigned char*)&s; sz=16; tp=MTEVAT_STR; }

	// Return 
	operator unsigned int(){ return (unsigned int)(int64)*this; }
	operator int64(){
		switch(tp){
			case MTEVAT_INT: return i; break;
			case MTEVAT_FLOAT: return (int64)f; break;
			case MTEVAT_STR: return stoi64(v);
			default : return 0;
		}
	}

	operator double(){
		switch(tp){
			case MTEVAT_INT: return (double)i; break;
			case MTEVAT_FLOAT: return f; break;
			case MTEVAT_STR: return stod(v);
			default : return 0;
		}
	}

	operator VString(){
		switch(tp){
			case MTEVAT_INT: return VString(); break;
			case MTEVAT_FLOAT: return VString(); break;
			case MTEVAT_STR: return v;
			default : return VString();
		}
	}

	operator IP6_ADDRESS(){
		if(tp==MTEVAT_STR && sz==16){ return *(IP6_ADDRESS*)v.data; }
		IP6_ADDRESS ip6; memset(&ip6, 0,16); return ip6;
	}



};