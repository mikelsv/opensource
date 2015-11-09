/*
 * Company:	MSV (MikelSV) code name Sumbios
 * This: MSL - My Server Language (My version internet language)
 * Company:	MSV (MikelSV) code name Archangel
 * This: MCC - My Compiler 
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MSL version 1.1.3		6/Apr/2005
	MSL version 2.0.3		17/Jun/2005
	MSL version 2.0.4		17/Jul/2005
	MSL version 2.0.5		19/Jul/2005

	MCC version 0.0.0.1		22/10/2007	
	MCC version 0.0.9.0		04/11/2009 14:30

	
	MSL : Copyright (C) 2004 - 2006, Mishael Senin aka MikelSV
	MCC : Copyright (C) 2007 - this, Mishael Senin aka MikelSV
*/
#define	MCC_VER "0.0.0.1"

#define MSL_VER	"0.3.2.2"
#define MSL_DAT "14.06.2009"

#define MSL_VER_3	"0.3.2.0"
#define MSL_DAT_3 "04.06.2009"

#define MSL_VER_2	"0.3.1.0"
#define MSL_DAT_2 "27.05.2009"

#define MSL_VER_1 "0.3.0.0"
#define MSL_DAT_1 "22.05.2009"

Versions xcc_version[]={
	"0.0.0.2", "05.11.2015 18:51",
	"0.0.0.1", "05.08.2011 14:27",
	"0.0.0.0", "04.08.2011 17:02"
};

// LEXP Types
//#define MCCT_NULL		0 // NULL
//#define MCCT_NUM		1 // 921
//#define MCCT_TEXT		2 // "text"
//#define MCCT_VAL		3 // $name
//#define MCCT_FUNC		4 // name
//#define MCCT_NVAL		5 // ,
//#define MCCT_EFUNC		6 // null
//#define MCCT_UP			7 // (), func
//#define MCCT_OP			8 // +, -, ...
//#define MCCT_END		9 // ;
//#define MCCT_ARR		10 // []
//#define MCCT_CB			11 // {} code block
//#define MCCT_EXFUNC		12 // if(), whie(), for()
//#define MCCT_SVAL		13 // stack val
//#define MCCT_UNDEF	14 // undefine unactive code   MCCT_UNDEFA - end of block.
enum mcc_lexp_type{ MCCT_NULL, MCCT_NUM, MCCT_TEXT,	MCCT_VAL, MCCT_DEF, MCCT_FUNC, MCCT_NVAL, MCCT_EFUNC, MCCT_UP, MCCT_OP, 
MCCT_END, MCCT_ARR, MCCT_CB, MCCT_EXFUNC, MCCT_SVAL, MCCT_UNDEF, MCCT_UNDEFA };


// MCC Operations
#define MCCOP_CMP	1	// val [=] element.
#define MCCOP_ADD	2	// val + val
#define MCCOP_ADDE	3	// val += val
#define MCCOP_MIN	4	// val - val
#define MCCOP_MINE	5	// val -= val
#define MCCOP_MULT	6	// val * val 
#define MCCOP_MULTE	7	// val *= val 
#define MCCOP_DIV	8	// val / val
#define MCCOP_DIVE	9	// val /= val
#define MCCOP_MOD	10	// val % val
#define MCCOP_MODE	11	// val %= val
#define MCCOP_JMP	12	// jump to code [addr]
#define MCCOP_JMPI	13	// jump to code if [addr], el
#define MCCOP_JMPE	14	// jump to code no if [addr], el
#define MCCOP_MORE	15	// more >
#define MCCOP_LESS	16	// less <
#define MCCOP_MOREC	17	// less cmp >=
#define MCCOP_LESSC	18	// less cmp <
#define MCCOP_CMPC	19	// compare ==
#define MCCOP_CMPNC	20	// no compare !=
#define MCCOP_ADDO	21	// ++
#define MCCOP_MINO	22	// --
#define MCCOP_MULTO	23	// *-1
#define MCCOP_LNEG	24	// !
#define MCCOP_BNEG	25	// ~
#define MCCOP_AND	26	// &
#define MCCOP_ANDE	27	// &=
#define MCCOP_OR	28	// |
#define MCCOP_ORE	29	// |=
#define MCCOP_XOR	30	// ^
#define MCCOP_XORE	31	// ^=
#define MCCOP_FUNC	32	// int func(a, b, c);

#define MCCOP_DAND	33	// &&
#define MCCOP_DOR	34	// ||

#define MCCOP_DEF	38 // #
#define MCCOP_DEFADD 35 // ##
#define MCCOP_DEFSUB 36 // ::
#define MCCOP_USEDEF 37 // :#


#include "cpp-lexp.h"

// analys code element
class XCCFile{
public:
	MString name;
	MString rdata;
	VString data;

	// save
	HLString0 moddata, modcode;
	int moddata_pos;
	//VString fdata; // first pos data, for modyfy lexp[].data

	CppLexpD *lexp;
	HLString0 lexpdata;

	// info
	int load_size; // size on load
	int end_size; // size after preprocessor
	int lexp_size; // lexp block count
	int code_size; // code size
	int code_lines; // lines of code

	void ClearInfo(){ /*load_size=0;*/ moddata_pos = 0; end_size=0; lexp_size=0; code_size=0; /*code_lines=0;*/ }
};

// Array: lexp_id -> *file. // effect: lexp elements * sizeof(void*) [memory]

class XCCLEXP{ public:
	VString data; // value
	int tp;

	// file, pos in file.
	XCCFile *file;
	unsigned int fpos, cln;
};

class XCCDef{ public:
	VString name;
	VString param;
	VString data;
};

#define XCCDEFIN_ACT	1
#define XCCDEFIN_ELSE	2

class XCCDefIn{ public:
	int act;
	int opt_replace;
};

class XCCDefVirt{ public:
	VString key, val;
};

class XCCSaveDataPointer{ public:
	XCCFile *file;
	VString data;

	XCCSaveDataPointer(){ file=0; }
	XCCSaveDataPointer(XCCFile *f, VString d){ file=f; data=d; }
/////
	void Save(XCCFile *f, VString d){ file=f; data=d; }
/////
	void Res(VString &d){
		if(file->data.data!=data.data && data.data<=d.data && d.data<data.endu()){
			d.data=file->data.data+(d.data-data.data);
			//if(data.sz!=file->data.sz){ d.sz+=file->data.sz-data.sz; }
		}
	}
/////
	void ResD(VString &d){
		if(file->data.data!=data.data && data.data<=d.data && d.data<data.endu()){
			d.data=file->data.data+(d.data-data.data);
			if(data.sz!=file->data.sz){ d.sz+=file->data.sz-data.sz; }
		}
	}
/////
	void Res(unsigned char *&d){ 
		if(file->data.data!=data.data && data.data<=d && d<data.endu()){
			d=file->data.data+(d-data.data);
		}
	}

};

#include "xcc-matrix.h"

// error
#define MCCERR_NOENDED_TL	100
#define MCCERR_NOENDED_K	101
#define MCCERR_NOENDED_DK	102

#define MCCERR_WTF_IT	200
#define MCCERR_WTF_MAO	201
#define MCCERR_WTF_SE	202
#define MCCERR_WTF_SNF	203
#define MCCERR_WTF_MENO	204
#define MCCERR_WTF_MOP	205

#define MCCERR_LEXP_INCLSP 300
#define MCCERR_LEXP_INCNOFILE 301
#define MCCERR_LEXP_INCFREEFILE 302

#define MCCERR_LEXP_INTERNAL_ERROR	500


#define XCC_CISNAME(ln) (ln>='a' && ln<='z' || ln>='A' && ln<='Z' || ln>='0' && ln<='9' || ln=='_')
#define XCC_CISNUM(ln) (ln>='0' && ln<='9')

//
// Список файлов XCCFileLine files содержит название и данные используемых файлов. 
// Список элементов XCCLEXPLine lexp содержит список обработанных элементов: данные, тип, указатель на их XCCFile и позицию в нем.

// AnalysFile - запускает процесс анализа файла с помощью функции Lexp.
// Lexp - разбирает переданные данные в lexp[].

// + XCCFile fdata служит для сохранения старой позиции данных в памяти (first data). Добавлена для рассчета новых позиций данных в lexp[].data.

// Препроцессор.
// #include вызовет AnalysFile, после чего продолжит разбирать код.
// #define добавит элемент в XCCDefLine define.
// #ifdef должен игнорироваться, если он выполняется. Иначе выделяется новый блок памяти, куда вносятся изменения.
// #if - тоже самое, #else тоже, #endif игнорируется.
// * Новый блок памяти может быть больше, чтобы не потребовалось перевыделять память.
// ! После выделения нового блока в lexp[] вносятся изменения, все lexp[].data входившие в старый блок должны изменить адреса на новые.

#define XCC_USE

#define MSVXCC_PRINT_ANALYSFILE		1
#define MSVXCC_PRINT_ALL			S2GM

// XCC
class XCC{
	XCCLEXPLine lexp; // analys code
	XCCFileLine files; // used files
	XCCDefLine define; // used files
	//HLString lexp_data; // buffer for modify data

	// Errors text, code, count
	MString err, warn;
	int ierr, iwarn;
	int errors, warnings;

	// Options
	int opt_print;
	int opt_replace_enable;	
	int opt_replace_do;

	union{
	XCCFile *proc_file;
	XCCFile *tmp_file; // for: lexp
	};

	// temp values
	
	unsigned int tmp_cln; // global count line [for AddDefine]
//	char *tmp_lexpchar;
	int tmp_notmind; // no call tmind forcode
	int tmp_nodefine; // no call define insert
	//int tmp_lexpint;

	// temp define 
	int tmp_def_in; // in defines count
	int tmp_def_act; // this define is active?
	XCCDefInLine define_in;

	// return
	HLString ret;

	// info
public:
	int load_size; // size on load
	int end_size; // size after preprocessor
	int lexp_size; // lexp block count
	int code_size; // code size
	int code_lines; // lines of code

	int res_code_lines;

public:
	XCC(){
		opt_print = 0;
		opt_replace_do = 0;
		opt_replace_enable = 0;

		res_code_lines = 0;
	}

	~XCC(){ }

public:

// options
	int GetEnableReplace(){
		return opt_replace_enable;
	}

	void SetEnableReplace(int val = 1){
		if(val)
			AddDefine("MSVXCCTEMPLATE", "", "");
		else
			DelDefine("MSVXCCTEMPLATE");
		opt_replace_enable = val;
	}

	void SetPrint(int val){
		opt_print = val;
	}

// analys
	int AnalysFile(VString file);
	int AnalysFile(VString file, VString data);
	int AnalysFile(XCCFile *file);

// lexp. analys code
	int Lexp(VString data, unsigned char f=0);
	int Lexp(VString data, unsigned char f, unsigned char *&rln, unsigned int &rcln);
	int LexpPrepro(VString data, unsigned char *&ln, unsigned int &rcln);
	int LexpDefine(XCCDef *def, VString &data, unsigned char *&ln, unsigned char *&lln, unsigned char *&to, unsigned int &cln);
	//int LexpPreproInIf(VString data, unsigned char *&ln);
	// int Lexp(VString data, char f=0, char *&rln=msldefchar, int &ncln=msldefint);
	void Lexp_fpos(unsigned int &fpos, unsigned char *ln);
	int Lexp_defop(VString c, VString p);

	int _skipspace(unsigned char *&ln, unsigned char *to);

	TString GetResultLine(VString line);

// Op
	int GetOpId(VString op);

// lind. analys codeline
	int lind(int fid, int eid);

// files
	XCCFile* GetFile(VString file);
	XCCFile* AddFile(VString file, VString data);

// define
	int AddDefine(VString name, VString param, VString data);
	XCCDef* GetDefine(VString name);
	int DelDefine(VString name);
	void GoDefine(int v);
	void ElseDefine();
	void EndDefine();
	void IsActiveDefine();
	
// error
	int SetError(VString e, int ie);
	int SetWarning(VString e, int ie);

	int SetError(int i, int ln);
	int SetWarning(int i, int ln);

	VString EList(int i, int d=0);
	VString EListRU(int i);

	// Clear
	void Clear(){
		// error
		err.Clean(); warn.Clean();
		ierr=0; iwarn=0;
		errors=0; warnings=0;

		// tmp
		tmp_file=0; tmp_notmind=0; tmp_nodefine=0;
		return ;
	}

	// Info
	void RecheckFileInfo();
};


// analys
int XCC::AnalysFile(VString file){
	//if(!IsFile(file)){ Error(HLString()+"#include \""+file+"\" not found.", 0); return 0;  }
	MString data = LoadFile(file);
	return AnalysFile(file, data);
}

int XCC::AnalysFile(VString file, VString data){
	if(opt_print & MSVXCC_PRINT_ANALYSFILE){
		int is = IsFile(file);
		print(HLString() + "XCC Analys file: " + file + (is ? " OK " : " FAIL(File not found)") + "\r\n");
	}
	
	XCCFile*fl = GetFile(file);
	if(fl)
		return 0; // allready used
	
	fl = AddFile(file, data);
	if(!fl)
		return 0; // add fail

	// Analys
	fl->load_size = fl->data;
	return AnalysFile(fl);
}

int XCC::AnalysFile(XCCFile *file){
	if(!file)
		return 0;

	XCCFile *lf = tmp_file;
	tmp_file = file;

	IsActiveDefine();
	unsigned char *ln=file->data; unsigned int cln=1;
	int r=Lexp(file->data, 0, ln, cln);
	file->code_lines=cln;
	tmp_file=lf;
	return r;
}

// lexp. analys code
int XCC::Lexp(VString data, unsigned char f){
	unsigned char *ln=data; unsigned int cln=1;
	return Lexp(data, f, ln, cln /*, tmp_lexpint*/);
}

void XCC::Lexp_fpos(unsigned int &fpos, unsigned char *ln){
	if(!tmp_file){ fpos=-1; return ; }

	if(tmp_file->data.data<=ln && tmp_file->data.endu()>ln)
		fpos=ln-tmp_file->data.data;
	else
		fpos=-1;

	return ;
}
//
//int XCC::Lexp_defop(VString c, VString p){
//	int l=0, r=0;
//	if(c=="+" || c=="-") l=1;
//	else if(c=="*") l=2; else if(c=="/") l=3;
//	else if(c==">" || c=="<" || c==">=" || c=="<=") l=3;
//
//
//}

int XCC::Lexp(VString data, unsigned char f, unsigned char *&rln, unsigned int &rcln){
	//if(rln==msldefchar) lexp.Clear();

#define iflln			if(ln!=lln)	{lexp.A(); lexp.n().data.setu(lln, ln-lln); lexp.n().tp=type; lexp.n().cln=cln; Lexp_fpos(lexp.n().fpos, (unsigned char*)ln); lexp.n().file=tmp_file; lexp.size++; lln=ln; et=0; type=0; }
#define ifllnp(ln, lln)	if(ln!=lln) {lexp.A(); lexp.n().data.setu(lln, ln-lln); lexp.n().tp=type; lexp.n().cln=cln; Lexp_fpos(lexp.n().fpos, (unsigned char*)ln); lexp.n().file=tmp_file; lexp.size++; lln=ln; et=0; type=0; }
#define ifllnl						{lexp.A(); lexp.n().data.setu(0, 0);		lexp.n().tp=type; lexp.n().cln=cln; Lexp_fpos(lexp.n().fpos, (unsigned char*)ln); lexp.n().file=tmp_file; lexp.size++; lln=ln; et=0; type=0; }

#define ifllnadd(ln, sz) lexp.A(); lexp.n().data.setu(ln, sz); lexp.n().tp=type; lexp.n().cln=cln; Lexp_fpos(lexp.n().fpos, (unsigned char*)ln); lexp.n().file=tmp_file; lexp.size++; et=0; type=0;

	unsigned char*ln = rln, *lln = ln, *to = data.endu();
	int type = MCCT_NULL, et = 0;

	// tmind
	int tmid = lexp.size; unsigned int cln = rcln; // int cln=1; if(rln) cln=ncln; // count line^W count pos in file
	int r; // tmp
	int openup=0; // open UP '(';
	int eol = 0;

	while(ln < to){
		// 
		if(*ln == '('){
			type = MCCT_UP;
			ifllnadd(ln, 1);
		}
		else{
			SetError((HLString() + "Unknown sumbol: '" + VString(ln, 1) + "'.").String(), cln);
			break;
		}
	}


	rcln = cln;
	return 1;

while(ln<to){
		// skip space 
		cln += _skipspace(ln, to);

	switch(*ln){
		//case '(': case ')': iflln if(*ln=='(') type=MCCT_FUNC; else type=MCCT_EFUNC;
		//break;
		case '/':
			if(type==MCCT_OP && *(ln-1)=='/'){ while(ln<to){ if(*ln=='\n'){ /*cln++;*/ break;} ln++; } lln=ln; et=0; continue; }
		 case '*':
			 if(type==MCCT_OP && *(ln)=='*' && *(ln-1)=='/'){
				  while(ln<to){ if(*ln=='*' && *(ln+1)=='/') break; else if(*ln=='\n') cln++; ln++; }
				  lln=++ln+1; et=0;
		break; }
		case '+': case '-':  case '%': case '=': case '<': case '>': case '.': case '|': case '&': case '^': case '!': case '~':
			if(type!=MCCT_OP || (*ln=='!' || *ln=='~') && ln-lln) iflln type=MCCT_OP; et=1;
		break;

		case '(': case ')': iflln if(*ln==f && !openup){ rln=ln; rcln=cln; return 1; }
				  type=MCCT_UP; et=1;// ln++; iflln // + '(' 				  //r=Lexp(data, 
				  if(*ln=='(') openup++; else openup--;
		break;
		case '[': case ']': iflln type=MCCT_ARR; et=1; break;
		case '{': { iflln 
			if(!lexp.size || lexp[lexp.size-1].tp==MCCT_END){
				ln++;
				VString vs(ln, to-ln); unsigned char *rln=ln; //int cnl
				et=Lexp(vs, '}', rln, cln); lln=(ln=rln)+1;
				if(!et) return 0; et=0;
			} else { type=MCCT_CB; et=1; }
		} break;
		case '}': iflln if(*ln==f){ rln=ln; rcln=cln; return 1; } type=MCCT_CB; et=1; break;

		case '$': iflln lln=ln+1; type=MCCT_VAL; break;

		/*
		case '\n': cln++;
				   {
						unsigned char *x=ln+1, *tx;// if(x+1<to) x++;
						for(tx=x; tx<x+50 && tx<to; tx++){ if(*tx=='\n' || *tx=='\r') break; }
						//print(itos(cln), ". ", VString(x, tx-x), "...\r\n");
				   }
		*/
		
		case '\r': case ' ': case '\t': 
			iflln type=MCCT_NULL; lln=ln+1;
		break;

		case ',': iflln type=MCCT_NVAL; et=1; break;

		case ';': iflln type=MCCT_END; et=1;  break;

		case '#': iflln type=MCCT_NULL;
			// include define ifdef else endif if
			if(!LexpPrepro(data, ln, cln))
				type = MCCT_TEXT;
			else{
				type = MCCT_DEF;
				eol = 1;
			}
			break;

		case '`': 
			iflln 
			for(ln; ln<to; ln++){ if (*ln=='`') break; if(*ln=='\n') cln++; } // err=5;
			if(*ln!='`') { SetError(MCCERR_NOENDED_TL, cln); return 0; }
			type=MCCT_TEXT; if(ln==lln) { ifllnl } iflln; lln++;
		break;
		
		case '\'': 
			iflln ln++; lln=ln; et=cln; 
			// one sumbol, for c++.    fail, short may be 2 sumbols. :( use msl variant
			//if(ln+1<to && *ln!='\\' && *(ln+1)=='\''){ type=MCCT_TEXT; lln=ln; ln++; iflln; ln++; }
			//else if(ln+2<to && *ln=='\\' && *(ln+2)=='\''){ type=MCCT_TEXT; lln=++ln; ln++; iflln; ln++; }
			//else { Error("error in '<this>'.", cln); return 0; }

			// multi. for msl
			for (ln; ln<to; ln++){
				if(*ln=='\\'){ ln++; continue; }
				if(*ln=='\''){ break; }
				if(*ln=='\n') cln++;
			}// err=4;
			if(*ln!='\'') { SetError(MCCERR_NOENDED_K, et); return 0; }
			//print("T:'", VString(lln, ln-lln), "' ET\r\n");
			type=MCCT_TEXT; iflln; lln=ln+1;
		break;

		case '"': { MString t; int wp=0;
			iflln ln++; lln=ln; unsigned char*isn=ln;
			for (ln; ln<to; ln++){
				if (*ln=='"') {break;} if(*ln=='\n') cln++;
			
				if (*ln=='\\'){// write point
					if(wp){ { type=MCCT_OP; unsigned char *lln=(unsigned char*)".", *ln=lln+1; iflln wp=0; }}// if write point
					
					type=MCCT_TEXT;
					et=1; t.Reserv(t.size()+ln-lln+1); memcpy(t.data+t.sz-(ln-lln+1), lln, ln-lln);
				switch(*(ln+1)){
					case 'n': *(t.end()-1)=10; break;
					case 'r': *(t.end()-1)=13; break;
					case 't': *(t.end()-1)=9; break;
					default : *(t.end()-1)=*(ln+1);
				} lln=++ln +1; }

				//if(*ln=='{' && ln+1<to && *(ln+1)=='$'){// write point
				//	if(wp){ { type=MCCT_OP; unsigned char *lln=(unsigned char*)".", *ln=lln+1; iflln wp=0; }}// if write point

				//	type=MCCT_TEXT;// add text
				//	//if(ln-lln) {t.RSize(t.size()+ln-lln+1); memcpy(t.data+t.sz-(ln-lln+1), lln, ln-lln); lln=ln+1;}
				//	if(ln-lln) { t.RSize(t.size()+ln-lln); memcpy(t.data+t.sz-(ln-lln), lln, ln-lln); lln=ln+1; }
				//	if(t){ unsigned char *ln=(t.data+t.sz), *lln=t.data; iflln }
				//	if(t){ type=MCCT_OP; unsigned char *lln=(unsigned char*)".", *ln=lln+1; iflln } t.Empty();
				//	
				//	*ln++;
				//	VString vs(ln, to-ln); unsigned char *rln=ln; //int cnl
				//	et=Lexp(vs, '}', rln, cln); rcln=cln; lln=(ln=rln)+1;
				//	if(!et) return 0; wp=1;
				//	// if next
				//	//type=MCCT_OP; if(t){ char *ln=(char*)(t.data+t.sz), *lln=(char*)t.data; iflln }
				//}
		
				}// err=4;
			if(wp){ if(ln-lln){ type=MCCT_OP; unsigned char *lln=(unsigned char*)".", *ln=lln+1; iflln } } // if write point
			if(!t.is() && ln-lln) {t.Reserv(t.size()+ln-lln); memcpy(t.data+t.sz-(ln-lln), lln, ln-lln); lln=ln+1;}
			if(*ln!='"') { SetError(MCCERR_NOENDED_DK, cln); return 0; }
			//if(t) if(ln-lln) {t.Renew(t.size()+ln-lln+1); memcpy(t.data+t.sz-(ln-lln+1), lln, ln-lln); lln=ln+1;}
			type=MCCT_TEXT; if(t){ unsigned char *ln=(t.data+t.sz), *lln=t.data; iflln } else iflln;
			if(isn==ln) ifllnl lln=ln+1;
			//print("T:'", t, "' ET\r\n");
			}break;

		//case '"':
		//	iflln ln++;
		//	for (ln; ln<to; ln++){ if (*ln=='"') if(*ln=='"') ln++; else break; }// err=4;
		//	type=MCCT_TEXT; iflln;
		//break;

		default: if(et){ iflln } break;
	}

	// if text && ln+1!=text -> analys, define?
	if(!type && ln>lln && ln+1<data.endu() && !XCC_CISNAME(*(ln+1))){
		XCCDef*def=GetDefine(VString(lln, ln-lln+1));
		if(def && !tmp_nodefine){ ++ln;
			int r=LexpDefine(def, data, ln, lln, to, cln);
			if(def->name=="MAKEDIRSLASH")
		int ert=1;
			//--ln;
			if(!r) return 0;
		}
	}


	if(*ln==';' || eol){
		iflln
		if(!lind(tmid, lexp.size))
			return 0;

		tmid = lexp.size;
		lln = ln;
		eol = 0;
	}

	//ln++;
	if(ln == to){
		iflln
		if(lln > data.data){
			type = MCCT_END;
			lln--;
			iflln
		}

		if(!lind(tmid, lexp.size))
			return 0;

		tmid = lexp.size;

		break;
	}
}
//
//for(unsigned int i=0; i<lexp.size; i++){
//	WriteFile(1, lexp[i].data, lexp[i].data); print("\r\n");
//}

	rcln=cln;
	return 1;
}

int XCC::LexpPrepro(VString data, unsigned char *&ln, unsigned int &rcln){
	unsigned char*to=data.endu(), *l=ln-1; VString df; unsigned char *f=0;

	// no data prev #
	for(l; l>data.data; l--){
		if(*l=='\n'){ /*rcln++;*/ break; }
		if(*l!=' ' && *l!='\t'){
			if(incompare(VString(ln, to-ln), "#include")){ SetError(MCCERR_LEXP_INCLSP, 0); }
			return 0;
		}
	}

	VString k=VString(ln, to-ln);
	if(incompare(k, "#include")) df="#include";
	if(incompare(k, "#define")) df="#define";
	if(incompare(k, "#undef")) df="#undef";
	if(incompare(k, "#ifdef")) df="#ifdef";
	if(incompare(k, "#ifndef")) df="#ifndef";
	if(incompare(k, "#if") && !df) df="#if";
	if(incompare(k, "#else")) df="#else";
	if(incompare(k, "#endif")) df="#endif";

	if(!df || k.data+df.sz<data.endu() && ( // k+<space or \t or \n or \n>
		*(k.data+df.sz)!=' ' && *(k.data+df.sz)!='\t' && *(k.data+df.sz)!='\r' && *(k.data+df.sz)!='\n' )
		&&(df!="#if" || df=="#if" && *(k.data+df.sz)!='(')) return 0;

	//if(!(incompare(, "#include") || incompare(ln, to-ln, "#if", 3) || incompare(ln, to-ln, "#ifdef", 6) ||
	//	incompare(ln, to-ln, "#ifndef", 7) || incompare(ln, to-ln, "#else", 5) || incompare(ln, to-ln, "#endif", 6) ))
	//	return 0;
	

	// #def<to this>, to #def <space> <to this>.
	ln+=df.sz;
	for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t'){ f=ln; break; } }
	
	//for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t' && *ln!='\r' || *ln!='\n'){ f=ln; break; } }

// #include
	if(df=="#include"){ /*
		ln+9<to && cmp(ln, "#include", 8) && (*(ln+8)==' ' || *(ln+8)=='\t') ){
		if(ln>(char*)data.data && *(ln-1)!='\n'){ Error(MCCERR_LEXP_INCLSP, 0); }
		else{
			char *f=0; ln+=8;
			for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t'){ f=ln; ln++; break; } }*/
		if(f){
			ln++;
			for(ln; ln<to; ln++){ if(*f=='"' && *ln=='"' || *f=='<' && *ln=='>'){ break; } }
		}

		if(f && ln<to){
			VString file; file.setu(f+1, ln-f-1);
			if(!tmp_file){
				 SetError(MCCERR_LEXP_INTERNAL_ERROR, 0);
			}
			else if(file){
				ILink ilink; ilink.Link(tmp_file->name);
				if(tmp_def_act)
					AnalysFile(HLString()+ilink.GetProtoDomainPath()+file);						
			}
			else SetError(MCCERR_LEXP_INCNOFILE, 0);
		} else { SetError(MCCERR_LEXP_INCFREEFILE, 0); }
	}

//#define
	if(df=="#define"){
	//if(ln+8<to && cmp(ln, "#define", 7)  && (*(ln+7)==' ' || *(ln+7)=='\t') ){
		 /*char *f=0; ln+=7;*/ 
	//	for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t'){ f=ln; ln++; break; } }

		VString name, param, data; int nline=0;

		// name
		for(ln; ln<to; ln++){ if(!XCC_CISNAME(*ln)){ break; } }
		if(/*ln<to &&*/ f){ name.setu(f, ln-f); } else SetError("#define <null>", 0);

		// params
		if(ln<to && *ln=='('){ f=ln+1;
			for(ln; ln<to; ln++){
				if(*ln==')'){ param.setu(f, ln-f); ln++; break; }				
			}
			if(!param.data){ SetError("#define( <this error>", 0); }
		}
		
		for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t'){ f=ln; break; } }
		// data
		f=ln;
		for(ln; ln<to; ln++){
			if(*ln=='\r' || *ln=='\n'){
				if(*ln=='\n') rcln++;
				if(!nline) break;
			} else if(*ln=='\\') nline=1; else { nline=0; }
		}
		data.setu(f, ln-f);

		// insert define
		if(tmp_def_act){
			tmp_cln=rcln;
			AddDefine(name, param, data);
		}
	}

	if(df=="#undef"){
		VString name, param, data; int nline=0;
		// name
		for(ln; ln<to; ln++){ if(!XCC_CISNAME(*ln)){ break; } }
		if(ln<to && f){ name.setu(f, ln-f); } else SetError("#undef <null>", 0);

		// delete define
		if(tmp_def_act){
			tmp_cln=rcln;
			DelDefine(name);
		}
	}

	if(df=="#ifdef" || df=="#ifndef"){
	//if(ln+7<to && cmp(ln, "#ifdef", 6) && (*(ln+6)==' ' || *(ln+6)=='\t') ){
		// char *f=0; ln+=8;
		//for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t'){ f=ln; ln++; break; } }
		
		VString name; tmp_cln=rcln;
		// value				// (*ln>='a' && *ln<='z' || *ln>='A' && *ln<='Z' || *ln=='_')
		for(ln; ln<to; ln++){ if(!XCC_CISNAME(*ln)){ break; } }
		if(ln<to && f){ name.setu(f, ln-f); } else SetError("#ifdef <null>", tmp_cln);

		XCCDef *def = GetDefine(name);
		if(def/* && def->data*/){
			GoDefine(df == "#ifdef");
		} else{
			GoDefine(df != "#ifdef");
		}

		if(tmp_def_act && name == "MSVXCCTEMPLATE" && GetEnableReplace()){
			if(opt_replace_do){
				SetError("Double MSVXCCTEMPLATE! Please Don't it.", tmp_cln);
				return 0;
			}

			VString fdata = proc_file->data;
			int tpos = (ln - fdata.data) - proc_file->moddata_pos;
			fdata = fdata.str(proc_file->moddata_pos, tpos);
			proc_file->moddata_pos += tpos;

			proc_file->moddata.Add(fdata, fdata);

			opt_replace_do = define_in.size;
			define_in.p().opt_replace = 1;
		}

		if(!tmp_def_act){
			lexp.A();
			lexp.n().tp = MCCT_UNDEF;
		}
		//if(!tmp_def_act){ return LexpPreproInIf(); }
	}

	if(df=="#if"){
		//for(ln; ln<to; ln++){ if(*ln!=' ' && *ln!='\t'){ break; } }
		VString val;

		for(ln; ln<to; ln++){ if(*ln=='\r' || *ln=='\n'){ break; } }
		if(ln<to && f){ val.setu(f, ln-f); } else SetError("#if <null>", tmp_cln);

		// analys.
		unsigned char *p, *lp, *tp;
		ItosX <S1K> it, rt; it.set(val);
		while(1){
			VString l, c, r; int cid=-1, ctmp;			
			p=it.ret; lp=p; tp=it.ret.endu();
			unsigned char *lb=lp, *rb=tp; int b=0, bb=0;

			// find box
			for(p; p<tp; p++){
				if(*p=='('){ b++; if(bb<b){ lb=p+1; bb=b; rb=0; } }
				if(*p==')'){ b--; if(lb && !rb) rb=p;  }
			} // box - ( lb, rb. )

			if(lb && !rb){ SetError("#if. not found ')'", tmp_cln); return 0; }
			if(b){ SetError("#if. not found '('", tmp_cln); return 0; }


if(rcln==150)
	int erte=564; // test

			while(1){
				p=lb; lp=p; tp=rb; int t=0; // 0 "T" 1 "t..." 1 " " 2 + 3 " " 4 T 5   + && + ||
				for(p; p<tp; p++){
					if(*p=='>' || *p=='<' || *p=='=' || *p == '|' || *p == '&'){
						if(!t) lp=p; t=1;
						//if(p+1<tp && ( *(p+1)=='>' || *p=='<' || *p=='=')
					}
					else{ if(t){ ctmp=GetOpId(VString(lp, p-lp)); if(ctmp>cid){ c.setu(lp, p-lp); } t=0; } }
				} // ok operation found

				if(!c){
					t=0;
					for(p=lb; p<rb; p++){ if(*p!=' ' || *p!='\t') break; } lp=p; // skip space
					for(p; p<rb; p++){ if(!(XCC_CISNAME(*p))){ break; }} // skip val
					l.setu(lp, p-lp);
					for(p; p<rb; p++){ if(*p!=' ' || *p!='\t') break; } // skip space
					if(p!=rb){ SetError((HLString()+"#if bad block: '"+VString(lb, rb-lb)+"'.").String(), rcln); return 0; }

					// 
					XCCDef *def=GetDefine(l); VString ret;
					if(l[0]>='0' && l[0]<='9') ret=l; else if(def) ret=def->data; else ret="0";
					if(it.ret.data==lb && it.ret.endu()==rb){
						GoDefine(ret && ret!="0");
						if(!tmp_def_act){ lexp.A(); lexp.n().tp=MCCT_UNDEF; }
						return 1;
					}

					rt.Add(VString(it.ret.data, lb-it.ret.data-1), ret, VString(rb+1, it.ret.endu()-rb-1)); it=rt;
					break;
				}

				if(c){
					// left val
					for(p=c.data-1; p>=lb; p--){ if(*p!=' ' && *p!='\t') break; } lp=p; lp++; // skip space
					for(p; p>=lb; p--){ if(!(XCC_CISNAME(*p))){ break; }} // skip val
					if(p<lb) p++; l.setu(p, lp-p);

					// right val
					for(p=c.endu(); p<rb; p++){ if(*p!=' ' && *p!='\t') break; } lp=p; // skip space
					for(p; p<rb; p++){ if(!(XCC_CISNAME(*p))){ break; }} // skip val
					r.setu(lp, p-lp);
//				}

				// result
//				if(c){
					VString ret;

					if(!l || !r){ SetError("#if parameters error.", rcln); return 0; }
					if(!XCC_CISNUM(l[0])){
						XCCDef *def=GetDefine(l); l= def ? def->data : "0";
					}

					if(!XCC_CISNUM(r[0])){
						XCCDef *def=GetDefine(r); r= def ? def->data : "0";
					}

					// op
					if(c==">"){ int il=l.toi(), ir=r.toi(); ret= il>ir ? "1" : "0"; }
					else if(c=="<"){ int il=l.toi(), ir=r.toi(); ret= il<ir ? "1" : "0"; }
					else if(c==">="){ int il=l.toi(), ir=r.toi(); ret= il>=ir ? "1" : "0"; }
					else if(c=="<="){ int il=l.toi(), ir=r.toi(); ret= il<=ir ? "1" : "0"; }
					else if(c=="=="){ int il=l.toi(), ir=r.toi(); ret= il==ir ? "1" : "0"; }
					else if(c=="&&"){ int il=l.toi(), ir=r.toi(); ret= il && ir ? "1" : "0"; }
					else if(c=="||"){ int il=l.toi(), ir=r.toi(); ret= il || ir ? "1" : "0"; }
					else { SetError((HLString()+"#if operation: '"+c+"' not accessible. sorry.").String(), rcln); return 0; }

					rt.Add(VString(it.ret.data, lb-it.ret.data), ret, VString(rb+1, it.ret.endu()-rb)); it=rt;
					break; // or create normal lb and rb, else it crash.
				}





					//if(XCC_CISNAME(*p)){
					//	if(t==3){ ctmp=GetOpId(VString(lp, p-pl)); if(ctmp>cid){ c.set(lp, p-pl); t=4; } }
					//	if(t==0 || t==3){ lp=p; t=1; }
					//	if(t==4){ lp=p; t=5; }
					//}
					//else if(*p=='>' || *p=='<'){
					//	if(t==1){ l.set(lp, p-pl); }
					//	if(t!=3){ t=3; lp=p; }					
					//}
					//else if(*p==' ' || *p=='\t'){
					//	if(t==1){ l.set(lp, p-pl); }
					//	if(t==3){ c.set(lp, p-pl); }
					//	if(t==5){ r.set(lp, p-pl); }
					//	lp=p+1;
					//}







			}

		}


		
	}

	if(df=="#else"){
		if(opt_replace_do && opt_replace_do == define_in.size){
			VString fdata = proc_file->data;
			int tpos = (ln - fdata.data) - proc_file->moddata_pos;
			fdata = fdata.str(proc_file->moddata_pos, tpos);
			proc_file->moddata_pos += tpos;

			proc_file->moddata.Add(fdata, fdata);

			proc_file->moddata + proc_file->modcode;
			proc_file->modcode.Clear();

			opt_replace_do = -1;
		}

		 ElseDefine();
		 lexp.A(); lexp.n().tp = tmp_def_act ? MCCT_UNDEFA : MCCT_UNDEF;
		 //if(!tmp_def_act){ return LexpPreproInIf(); }
		 return 1;
	}

	if(df=="#endif"){
		if(define_in.p().opt_replace){
			proc_file->moddata_pos = ln - data.data;
			opt_replace_do --;
		}

		EndDefine();
		lexp.A(); lexp.n().tp=tmp_def_act ? MCCT_UNDEFA : MCCT_UNDEF;
		return 1;
	}

	return 1;
}

int XCC::LexpDefine(XCCDef *def, VString &data, unsigned char *&ln, unsigned char *&lln, unsigned char *&to, unsigned int &cln){
	unsigned char *p=ln, *lp, *defb=lln, *defe=ln;
	int lel=lexp.size, tel=lel, l2el;

	for(p; p<to; p++){ if(*p!=' ' && *p!='\t' && *p!='\r' && *p!='\n') break; }

	if(*p!='(' && def->param.data) return 1;

	if(def->name=="XCC_CISNAME")
		int ert=1; // test
	
	if(p<to && *p=='(' && def->param.data){
		p++; lp=p;
		tmp_notmind++; // no analys code in tmind
		tmp_nodefine++; // no use define
		int r=Lexp(data, ')', p, cln);
		// result??? modify

		tmp_notmind--; tmp_nodefine--; tel=lexp.size;
		if(def->param && lel==tel || !def->param && lel!=tel){ SetError("define param != vslue params", 0); return 0; }
	}

	// #define def param.  [in code] key(key param).  vline.key=def param, .val=key param
	XCCDefVirtLine vline; HLString vlined;

	// vline.val
	HLString ls; MString tmp; int vcnt=0, kcnt=0; // count
	l2el=lel;
			
	if(def->param.data){ // if param ==> #define(a,b,c) (a,b,c)  else #define data
		int openup=0;
		for(int el=lel; el<tel; el++){
			if(lexp[el].tp==MCCT_UP){
				if(lexp[el].data=="(") openup++; else openup--;
			}

			if(lexp[el].tp!=MCCT_NVAL || openup){
				if(ls.size()) ls+" "; ls+lexp[el].data;
				if(el+1==tel) ++el;
			}

			if(lexp[el].tp==MCCT_NVAL && !openup || el==tel){ // block
				if(lel==tel){ SetError("no data in define.", cln); }
				ls.String(tmp); ls.Clear();
				lel=el+1; vcnt++;
				vline.A(); vline.n().val=vlined.addnfr(tmp); vline.size++;
			}
		}

		// vline.key // test param
		unsigned char *vk=def->param, *lvk=vk, *tvk=def->param.endu(); int t=0; // 0 - no, 1 - text, 2 - free.
		for(vk; vk<tvk; vk++){
			//  *vk>='a' && *vk<='z' || *vk>='A' && *vk<='Z' || *vk=='_'
			if(XCC_CISNAME(*vk)){ t=1; lvk=vk; }
			else if(*vk==' ' || *vk=='\t'){ if(t==1) t=2; }
			
			if(*vk==',' || vk+1==tvk && ++vk){
				vline.A(); vline[kcnt].key.setu(lvk, vk-lvk);
				if(vline.size<kcnt) vline.size++;
				kcnt++; t=0;
			}
		}

		if(kcnt!=vcnt){ SetError("#define param != define(param).", cln); return 0; }

		// replace #define data.
		ls.Clear(); vk=def->data; lvk=vk; tvk=def->data.endu();

		for(vk; vk<=tvk; vk++){
			if(vk<tvk && ( *vk>='a' && *vk<='z' || *vk>='A' && *vk<='Z' || *vk=='_')){
				if(!t){ ls+VString(lvk, vk-lvk); lvk=vk; t=1; }						
			}
			else{
				if(t){
					VString k(lvk, vk-lvk);
					for(int i=0; i<kcnt; i++){ if(k==vline[i].key){ ls+vline[i].val; t=0; break; }}
					if(t){ ls+VString(lvk, vk-lvk); t=0; }
					lvk=vk;
				} else{
					ls+VString(lvk, vk-lvk); lvk=vk;
				}
			}
		}
	}// if param
	else { ls+def->data; }

	VString lfile=tmp_file->data; ls.String(tmp); ls.Clear();

	if(tmp_file->data+defb-p<=tmp_file->rdata && 0){
		// fast correct if memory
		memcpy(defb+(p-defb), defb, p-defb);
		*defb=' ';
		memcpy(defb+1, tmp, tmp);
		tmp_file->data.sz+=p-defb;
		ln=defb; lln=ln; to=data.endu();
	}else{
		// ret in ls
		XCCSaveDataPointer dp(tmp_file, tmp_file->data);
		tmp_file->rdata.Add(VString(tmp_file->data.data, defb-tmp_file->data.data), " ", tmp, VString(p, tmp_file->data.endu()-p)
			, tmp_file->data.sz > S16K ? tmp_file->data : tmp_file->data.str(0, S16K)); // reserv memory for fast correct
		
		int rs=(defb-tmp_file->data.data)+1+tmp.sz+(tmp_file->data.endu()-p);
		tmp_file->data.data=tmp_file->rdata.data; tmp_file->data.sz=rs;
		memset(tmp_file->data.endu(), 0, tmp_file->rdata.sz-tmp_file->data.sz);

		// update 
		dp.ResD(data); to=data.endu();
		ln=defb; dp.Res(ln); lln=ln;

		// update lexp
		lexp.size=l2el;
		
		for(int i=0; i<lexp.size; i++){ dp.Res(lexp[i].data); } // end update lexp

		// update define
		for(int i=0; i<define.size; i++){
			dp.Res(define[i].name); dp.Res(define[i].param); dp.Res(define[i].data);
		} // end update define
	}
	return 1;
}

TString XCC::GetResultLine(VString line){
	unsigned char *ln = line, *lln = ln, *to = line.endu();
	//TString ret;
	HLString ls;

	while(ln){
		// name
		while(ln < to && (*ln >= 'a' && *ln <= 'z' || *ln >= 'A' && *ln <= 'Z' || *ln == '_'))
			ln ++;

		VString name(lln, ln - lln);
		lln = ln;

		// params

		// replace
		XCCDef *def = GetDefine(name);
		if(def){
			ls + def->data;
		} else{
			ls + name + VString(lln, ln - lln);
			lln = ln;
		}
	}	

	return ls.String();
}

int XCC::_skipspace(unsigned char *&ln, unsigned char *to){
	//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' && (line+1>=to || *(line+1)=='\n' || line++ && ++do_line_count && (do_line=line+1))
	//	|| (*line=='\n' && ++do_line_count && (do_line=line+1) ))) line++;
	int cln = 0;

	while(ln < to){
		if(*ln == ' ' || *ln == '\t')
			ln ++;
		else if(*ln == '\r'){
			//if(ln + 1 < to || *(ln + 1) == '\n'){ if(!do_line_count_ignore) ++do_line_count; do_line=line+2; }
			ln ++;
		}
		else if(*ln == '\n'){
			ln ++; // if(!do_line_count_ignore) ++do_line_count; do_line=line;
			cln ++;
		}
		else
			break;
	}
	return cln;
}

#define MCCOPTID(val, id) if(op==val) return id;
int XCC::GetOpId(VString op){
	MCCOPTID("=", MCCOP_CMP);
	MCCOPTID("+", MCCOP_ADD);
	MCCOPTID("+=", MCCOP_ADDE);
	MCCOPTID("-", MCCOP_MIN);
	MCCOPTID("-=", MCCOP_MINE);
	MCCOPTID("*", MCCOP_MULT);
	MCCOPTID("*=", MCCOP_MULTE);
	MCCOPTID("/", MCCOP_DIV);
	MCCOPTID("/=", MCCOP_DIVE);
	MCCOPTID("%", MCCOP_MOD);
	MCCOPTID("%=", MCCOP_MODE);
	MCCOPTID(">", MCCOP_MORE);
	MCCOPTID("<", MCCOP_LESS);
	MCCOPTID(">=", MCCOP_MOREC);
	MCCOPTID("<=", MCCOP_LESSC);
	MCCOPTID("==", MCCOP_CMPC);
	MCCOPTID("!=", MCCOP_CMPNC);
	MCCOPTID("++", MCCOP_ADDO);
	MCCOPTID("--", MCCOP_MINO);
	MCCOPTID("*-", MCCOP_MULTO);
	MCCOPTID("!", MCCOP_LNEG);
	MCCOPTID("~", MCCOP_BNEG);
	MCCOPTID("&", MCCOP_AND);
	MCCOPTID("&=", MCCOP_ANDE);
	MCCOPTID("|", MCCOP_OR);
	MCCOPTID("|=", MCCOP_ORE);
	MCCOPTID("^", MCCOP_XOR);
	MCCOPTID("^=", MCCOP_XORE);

	MCCOPTID("&&", MCCOP_DAND);
	MCCOPTID("||", MCCOP_DOR);

	return -1;
}

//int XCC::LexpPreproInIf(VString data, unsigned char *&ln){
//	char*to=data.end();
//
//	for(ln; ln<to; ln++){
//		if(*ln=='#'){
//		}
//	}
//	return 1;
//}

// lind. analys codeline
int XCC::lind(int fid, int eid){
	int rep = opt_replace_do > 0 && lexp[fid].tp != MCCT_DEF;

	int t;
	for(int i=fid; i<eid; i++){
		if(!lexp[i].tp){
			if(isnum(lexp[i].data, lexp[i].data)) {lexp[i].tp=MCCT_TEXT; continue; }
			else if(lexp[i].data=="true") {lexp[i].data="1"; lexp[i].tp=MCCT_TEXT; continue; }
			else if(lexp[i].data=="false") {lexp[i].data="0"; lexp[i].tp=MCCT_TEXT; continue; }
			//else if(!USENEW && (t=TestFunc(lexp[i].data, 0))) {lexp[i].tp= (t==1 ? MCCT_FUNC : MCCT_EXFUNC); continue; }
			//else if(!USENEW){ Error(HLString()+EList(MCCERR_WTF_IT)+lexp[i].data, lexp[i].cln); return 0; }

		// is ext num
		// is func
		// is define
		// is normal value
		// is struct & class
		}  // analys code
	}
return 1;
}



// files
XCCFile* XCC::GetFile(VString file){
	for(int i = 0; i < files.size(); i++){
		if(files[i].name == file)
			return &files.el(i);
	}
	return 0;
}

XCCFile* XCC::AddFile(VString file, VString data){
	XCCFile *fl = files.vinsert();

	if(fl){
		fl->name = file;
		fl->rdata = data;
		fl->data = fl->rdata;
	}

	return fl;
}

// define
int XCC::AddDefine(VString name, VString param, VString data){

	if(name=="SET")
		int ert=356;

	// test param
	unsigned char *vk=param, *lvk=vk, *tvk=param.endu(); int t=0; // 0 - no, 1 - text, 2 - free.
	for(vk; vk<tvk; vk++){
		if(XCC_CISNAME(*vk)){ // *vk>='a' && *vk<='z' || *vk>='A' && *vk<='Z' || *vk=='_'
			if(t==2){ SetError("#define (text <??> text).", tmp_cln); return 0; }
			t=1;
		}
		else if(*vk==' ' || *vk=='\t'){ if(t==1) t=2; }
		else if(*vk==','){ if(!t){ SetError("#define (,<??>,).", 0); } t=0; }
		else if(*vk == '.'){
			// ...
		} else if(*vk == '/' && vk + 1 < tvk && *(vk + 1) == '*'){
			vk += 2;
			for(vk; vk<tvk; vk++){
				if(*vk == '*' && vk + 1 < tvk && *(vk + 1) == '/'){
					vk ++;
					break;
				}
			}

		}
		else{ SetError("uncnown sumbol in #define.", tmp_cln); return 0; }
	}

	int fid=-1;

	for(int i=0; i<define.size; i++){
		if(define[i].name==name){
			SetWarning((HLString()+" #define "+name+" already set.").String(), tmp_cln);
			return 0;
		}
		if(fid==-1 && !define[i].name) fid=i;
	}

	define.A();
	XCCDef &def= fid==-1 ? define.n() : define[fid];

	def.name=name; def.param=param; def.data=data; if(fid==-1) define.size++;
	return 1;
}

int XCC::DelDefine(VString name){
	for(int i=0; i<define.size; i++){
		if(define[i].name==name){
			define[i].name.Clean();
			return 1;
		}
	}
	return 0;
}

XCCDef* XCC::GetDefine(VString name){
	for(int i=0; i<define.size; i++){
		if(define[i].name==name){
			return &define[i];
		}
	}

	return 0;
}

void XCC::GoDefine(int v){
	define_in.A();
	define_in.n().act = v ? 1 : 0;
	define_in.n().opt_replace = 0;
	tmp_def_act = v ? 1 : 0;
	define_in.size ++;
}

void XCC::IsActiveDefine(){
	for(int i=0; i<define_in.size; i++){
		if(!(define_in[i].act&XCCDEFIN_ACT)){ tmp_def_act=0; return ; }
	}
	tmp_def_act=1;
}

void XCC::ElseDefine(){
	if(!define_in.size){ SetError("oh, #else? where's #if(n)def?", tmp_cln); return ; }
	if(define_in.p().act&XCCDEFIN_ELSE){ SetError("oh, #else? again?", tmp_cln); return ; }
	define_in.p().act=!(define_in.p().act&XCCDEFIN_ACT);
	define_in.p().act|=XCCDEFIN_ELSE;
	IsActiveDefine();
	return ;
}
void XCC::EndDefine(){
	if(!define_in.size){
		SetError("oh, #endif? where's #if(n)def?", tmp_cln);
		return ;
	}
	
	define_in.size --;
	//if(define_in.size && !(define_in.p().act&XCCDEFIN_ACT)) tmp_def_act=0; else tmp_def_act=1;
	IsActiveDefine();
	return ;
}

// error
int XCC::SetError(VString e, int ie){
	if(!tmp_def_act) return 0;
	ItosX<S2K> it; it.Add("ERROR: (", (tmp_file ? tmp_file->name : "unknown"), ") ", e, " (line: ", itos(ie), ")\r\n");	
	err=e; ierr=ie; errors++; ret+it.ret;
	print(it);
	return 1;
}
int XCC::SetWarning(VString e, int ie){
	if(!tmp_def_act) return 0;
	ItosX<S2K> it; it.Add("WARNING: (", (tmp_file ? tmp_file->name : "unknown"), ") ", e, " (line: ", itos(ie), ")\r\n");	
	err=e; ierr=ie; errors++; ret+it.ret;
	print(it);
	return 1;
}

int XCC::SetError(int i, int ln){ return SetError((MString)EList(i), ln); }
int XCC::SetWarning(int i, int ln){ return SetWarning((MString)EList(i), ln); }

VString XCC::EList(int i, int d){// if(!d) return EListRU(i);
	switch(i){
		case MCCERR_NOENDED_TL: return "no ended ` ";
		case MCCERR_NOENDED_K: return "no ended ' ";
		case MCCERR_NOENDED_DK: return "no ended \" ";
		
		case MCCERR_WTF_IT: return "What is element? ";
		case MCCERR_WTF_MAO: return "MAO: maximum operations! ";
		case MCCERR_WTF_SE: return "it's: ( empty )";
		case MCCERR_WTF_SNF: return "not full ( & )";
		case MCCERR_WTF_MENO: return "many elements, no find next operations";
		case MCCERR_WTF_MOP: return "MAO: Max all operations";

		case MCCERR_LEXP_INCLSP: return "space prev #include.";
		case MCCERR_LEXP_INCNOFILE: return "can't find file in #include";
		case MCCERR_LEXP_INCFREEFILE: return "no file name in #include";
		case MCCERR_LEXP_INTERNAL_ERROR: return "internal error.";
	}

	return VString();
}

VString XCC::EListRU(int i){
	switch(i){
		case MCCERR_NOENDED_TL: return "нет закрывающей ` (обратный апостроф) ";
		case MCCERR_NOENDED_K: return "нет закрывающей ' (кавычки) ";
		case MCCERR_NOENDED_DK: return "нет закрывающей \" (двойной кавычки) ";
		
		case MCCERR_WTF_IT: return "Этот элемент не определен: ";
		case MCCERR_WTF_MAO: return "MAO: превышен максимум операций! ";
		case MCCERR_WTF_SE: return "данные в скобках ( не найдены )";
		case MCCERR_WTF_SNF: return "не полные скобки ( )";
		case MCCERR_WTF_MENO: return "элементы есть, операций нет. так и живем :( ";
		case MCCERR_WTF_MOP: return "MAO: превышен максимум операций для всех скриптов!";

		case MCCERR_LEXP_INCLSP: return "#include должно начинаться с новой строки.";
		case MCCERR_LEXP_INCNOFILE: return "не найдено имя файла в #include.";
		case MCCERR_LEXP_INCFREEFILE: return "не указано имя файла в #include.";
		case MCCERR_LEXP_INTERNAL_ERROR: return "внутреняя ошибка.";
	}

	return EList(i, 1);
}


void XCC::RecheckFileInfo(){
	// clear info & clear file info
	load_size=0; end_size=0; lexp_size=0; code_size=0; code_lines=0;
	for(int i=0; i<files.size(); i++){
		//files.el(i).ClearInfo();
		files.el(i).code_size = 0;
	}

	// count lexp
	for(int i=0; i<lexp.size; i++){
		lexp[i].file->lexp_size++;
		lexp[i].file->code_size += lexp[i].data;
	}

	for(int i=0; i<files.size(); i++){
		files.el(i).end_size = files.el(i).data.size();

		if(files[i].moddata_pos){
			VString fdata = files[i].data.str(files[i].moddata_pos);
			files.el(i).moddata_pos = files[i].data;			
			files.el(i).moddata.Add(fdata, fdata);

			SaveFile(HLString() + files[i].name + ".new", VString(files.el(i).moddata.String()));
		}
		
		load_size+=files.el(i).load_size;
		end_size+=files.el(i).end_size;
		lexp_size+=files.el(i).lexp_size;
		code_size+=files.el(i).code_size;
		code_lines+=files.el(i).code_lines;
	}


	return ;
}