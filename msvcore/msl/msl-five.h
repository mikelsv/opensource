// My Server Language - Five
Versions msl_fv_version[]={
	"5.0.0.1", "28.09.2015 23:34",
};

// msl-five:
// docode - code -> pseudocode
// runcode - pseudocode -> result
// create - pseudocode -> c++ code

// Stack for Push Pop operation
class msl_five_stack{
	MString data;
	int size;

public:
	msl_five_stack(){
		data.Reserv(S1M);
		size = 0;
	}

	int GetPos(){
		return size;
	}

	void SetPos(int v){
		if(v >= 0 && v <= data.sz)
			size = v;
	}

	void* PushI(int sz){
		unsigned char *r = data.data + size;
		size += sz;
		return r;
	}

	VString PushV(VString val){
		unsigned char *r = data.data + size;
		memcpy(r, val, val);
		size += val;
		return VString(r, val);
	}

	void Clear(){
		size = 0;
	}

};

#define msl_ce_lexp_d msl_five_lexp_d
#define msl_five_lexp_d msl_five_lexp

// MSL Explode
/*
struct msl_five_lexp_d{
	union{
		msl_five_lexp_d *_p;
		msl_five_lexp_d *prev;
	};

	union{
		msl_five_lexp_d *_n;
		msl_five_lexp_d *next;
	};

	msl_five_lexp_d /**_p, *_n,*/ /**prev, *next,* / *up, *end;
	//msl_five_lexp_d *&prev = _p; //, *&next = _n;

	int id; int ext; VString text;

	//int IsUp(){ return id==MCCT_FUNC || id==MCCT_UP || id==MCCT_PAR; }
};*/

class msl_five_lexp : public OMatrixT<msl_five_lexp_d>{
	//msl_five_lexp_d *end;
//	int size;

public:

	union{
		msl_five_lexp_d *_p;
		msl_five_lexp_d *prev;
	};

	union{
		msl_five_lexp_d *_n;
		msl_five_lexp_d *next;
	};

	//msl_five_lexp_d /**_p, *_n,*/ /**prev, *next,*/ *up, *end;

	int id;
	int ext;
	VString text;

public:

	msl_five_lexp(){
		//size = 0;
	}

	// Get
	msl_five_lexp_d *GetFirst(){ return _a; }
	msl_five_lexp_d *GetEnd(){ return _e; }
	
	msl_five_lexp_d *GetEnd(msl_five_lexp_d *el){
		if(!el)
			return 0;

		while(el->_n)
			el = el->_n;

		return el;	
	}

	int Size(){
		msl_five_lexp_d *p = _a;
		int size = 0;

		while(p){
			p = p->_n;
			size ++;
		}

		return size;
	}

	int Size(msl_five_lexp_d *fr, msl_five_lexp_d *to){
		int s=0;

		while(fr && fr->id){
			s++;

			if(fr == to)
				break;

			fr = fr->_n;			
		}
		return s;
	}


	// Add
	void Add(msl_five_lexp_d *p){
		p->_a = 0;
		p->_n = 0;

		OMAdd(p);

//		p->up = 0;
//		p->end = 0;

		return ;
	}


	//msl_five_lexp_d* Add(){


	//	if(end){
	//		if(end->_n){
	//		end=end->_n; usz++;
	//		return end;
	//		}
	//	}
	//	else if(_a){
	//		end=_a; usz++;
	//		return end;
	//	}

	//	end=OMNew(); usz++;
	//	return end;
	//}

	void Clear(){
		_a = 0;
		_e = 0;

		_p = 0;
		_n = 0;
//		size = 0;
	}

};

// Lexp operations
#define MCCT_TEXT		1 // "text"
#define MCCT_VAL		2 // $name
#define MCCT_SVAL		3 // stack value
//#define MCCT_PVAL		35 // pointer value
#define MCCT_OP			4 // +, -, ...
#define MCCT_UP			5 // ()
#define MCCT_FUNC		6 // name
#define MCCT_EXTFUNC	16 // name + {} + else {}
#define MCCT_PAR		7 // ,
#define MCCT_COMMENT	8 // comment as in code
#define MCCT_NEWLINE	9 // name
#define MCCT_CB			10 // {} code block
#define MCCT_NOUSE		100 // no use, del it

#define MCCTV_ALL		S3G // stack
#define MCCTV_STACK		0 // stack
#define MCCTV_PVAL		S1G // pointer
#define MCCTV_DATA		S2G // data

#define MCCT_OP_MOV		1 // =
#define MCCT_OP_CONE	2 // .=
#define MCCT_OP_ADDE	3 // +=
#define MCCT_OP_SUBE	4 // -=
#define MCCT_OP_MULE	5 // *=
#define MCCT_OP_DIVE	6 // /=
#define MCCT_OP_MODE	7 // %=

#define MCCT_OP_CON		8 // .
#define MCCT_OP_ADD		9 // +
#define MCCT_OP_SUB		10 // -
#define MCCT_OP_MUL		11 // *
#define MCCT_OP_DIV		12 // /
#define MCCT_OP_MOD		13 // %
#define MCCT_OP_NO		14 // !
#define MCCT_OP_EQ		15 // ==
#define MCCT_OP_NEQ		16 // !=
#define MCCT_OP_OR		17 // ||
#define MCCT_OP_AND		18 // &&

#define MCCT_OP_NOT		19 // ~
#define MCCT_OP_BOR		20 // |
#define MCCT_OP_BAND	21 // &
#define MCCT_OP_BXOR	22 // ^

#define MCCT_OP_IADD	23 // ++
#define MCCT_OP_ISUB	24 // --

#include "msl-five-func.h"
#include "msl-five-exec.h"

// Do Create Options
class msl_five_docreate_opt{
public:
	msl_five_docreate_opt(){
		level=0;
		ispar=0;
	}

	int level, ispar;
};


// Process DoCode data
struct msl_five_proc_docode{
	msl_five_proc_docode *prev;

	VString file;
};


class msl_five_asm{
	VString code, data;	
	msl_ce_values *values;

public:
	msl_five_asm(){ values=0; }

	void set(VString c, VString d, msl_ce_values *v){ code=c; data=d; values=v; }

	_msl_five_asm_data* getop(unsigned int op){
		for(int i=0; i<sizeof(msl_five_asm_data)/sizeof(_msl_five_asm_data); i++){
			if(msl_five_asm_data[i].code==op)
				return &msl_five_asm_data[i];
		}
		return 0;
	}

	unsigned char* next(unsigned char *ln){
		if(!ln)
			return code;

		if(ln+4<code.endu()){
			unsigned int id=*(unsigned int*)ln;
			_msl_five_asm_data *d=getop(id);
			if(!d)
				return 0;
			
			if(d->sz == -1){

				if(id == MCCA_FUNC){
					unsigned int pars=*(unsigned int*)(ln+4+sizeof(void*));
					ln+=4+sizeof(void*)+sizeof(int)+sizeof(int)+sizeof(int)*pars;
				}

				if(id == MSLA_COMMENT){
					int t = *(unsigned int*)(ln + 4);
					t += *(unsigned int*)(ln + 8 + t);
					ln += 12 + t;
				}
				
			} else 
				ln+=4+d->sz;

			if(ln<code.endu())
				return ln;
		}

		return 0;
	}

	MString getlisting_val(unsigned int pos){Itos it; 
		switch(pos&MCCTV_ALL){
			case MCCTV_STACK:{
				msl_ce_values_d *val = values->GetByPos(pos);
				if(val)
					it.Add("$", val->key);
				else
					it.Format("@%d", pos);
				return it.ret;
			break; }

			case MCCTV_PVAL:
				it.Format("pval %d", pos & ~MCCTV_ALL);
				return it.ret;
			break;

			case MCCTV_DATA:{
					msl_ce_code_datavalue *dv=(msl_ce_code_datavalue*)(data.data+(pos&~MCCTV_DATA));
					it.Format("'%s'", VString(dv->data, dv->sz));
					return it.ret;
			break;}

			default:
				it.set("unknown type");
				return it.ret;
			break;
		}

		it.set("???");
		return it.ret;
	}

	VString GetOpName(int op){
		switch(op){
		case MCCA_ADD:
			return "+";
		case MCCA_SUB:
			return "-";
		case MCCA_MUL:
			return "*";
		case MCCA_DIV:
			return "/";
		case MCCA_MOD:
			return "%";		
		case MCCA_CON:
			return ".";
		}

		return "???";
	}

	void getlisting_el(unsigned char *ln, _msl_five_asm_data *el, HLString &ls){
		switch(el->code){
		case MSLA_COMMENT:{
			VString key = VString(ln + 4, *(unsigned int*)(ln ));
			VString val = VString(ln + 8 + key.sz, *(unsigned int*)(ln + 4 + key.sz));
			ls + "comment: " + key + "=" + val;
			ln + 0;
		break; }

		case MSLA_COMMENT_LINE:
			ls + *(unsigned int*)(ln) + ">";
		break;

		case MCCA_NOP:
			ls + "nop";
		break;

		case MCCA_ADD: case MCCA_SUB: case MCCA_MUL: case MCCA_DIV: case MCCA_MOD: case MCCA_CON: 
			ls+getlisting_val(*(unsigned int*)ln) + " = " + getlisting_val(*(unsigned int*)(ln+4)) + " " + GetOpName(el->code) + " " + getlisting_val(*(unsigned int*)(ln+8)) + "; "; // + el->str;
		break;

		case MCCA_MOV:
			ls+getlisting_val(*(unsigned int*)ln) + " = " + getlisting_val(*(unsigned int*)(ln+4)) + "; "; // + el->str;
		break;

		case MCCA_FUNC:{
			//msl_functions_d *f=(msl_functions_d*)*(unsigned long*)(ln); ln+=sizeof(void*);
			//unsigned int pars=*(unsigned int*)(ln); ln+=sizeof(int);

			//ls + getlisting_val(*(unsigned int*)ln) + " = " + f->name + "("; ln+=sizeof(int);
			//for(int i=0; i<pars; i++){
			//	if(i) ls+", ";
			//	ls + getlisting_val(*(unsigned int*)ln);
			//	ln+=sizeof(int);
			//}

			ls+"); ";
		break; }

		case MCCA_VUP:{
			ls + getlisting_val(*(unsigned int*)ln) + " = " + getlisting_val(*(unsigned int*)(ln+4)); ln+=sizeof(int)*2;
			unsigned int pars=*(unsigned int*)ln; ln+=4;

			for(int i=0; i<pars; i++){
				ls+"[" + getlisting_val(*(unsigned int*)ln) + "]";
				ln+=sizeof(int);
			}			
		break;}


		case MCCA_JMP:
			ls + "jmp " + itos(*(unsigned int*)(ln)) + "; " + el->str;
		break;

		case MCCA_AJMP:
			ls + "ajmp " + *(unsigned int*)(ln) + "; " + el->str;
		break;

		case MCCA_EJMP:
			ls+"jmp ["+getlisting_val(*(unsigned int*)ln) + "==1] " + itos(*(unsigned int*)(ln+4)) + "; " + el->str;
		break;

		case MCCA_NEJMP:
			ls+"jmp ["+getlisting_val(*(unsigned int*)ln) + "==0] " + itos(*(unsigned int*)(ln+4)) + "; " + el->str;
		break;

		case MCCA_UADD:
			ls+" + "+getlisting_val(*(unsigned int*)ln) + "; " + el->str;
		break;

		case MCCA_USUB:
			ls+" - "+getlisting_val(*(unsigned int*)ln) + "; " + el->str;
		break;

		case MCCA_UNO:
			ls+" ! "+getlisting_val(*(unsigned int*)ln) + "; " + el->str;
		break;

		case MCCA_UNOT:
			ls+" ~ "+getlisting_val(*(unsigned int*)ln) + "; " + el->str;
		break;

		case MCCA_IADD:
			ls+" ++ "+getlisting_val(*(unsigned int*)ln) + "; " + el->str;
		break;

		case MCCA_ISUB:
			ls+" -- "+getlisting_val(*(unsigned int*)ln) + "; " + el->str;
		break;

		case MCCA_EQ:
			ls+getlisting_val(*(unsigned int*)ln) + " == " + getlisting_val(*(unsigned int*)(ln+4)) + "; " + el->str;
		break;

		case MCCA_NEQ:
			ls+getlisting_val(*(unsigned int*)ln) + " != " + getlisting_val(*(unsigned int*)(ln+4)) + "; " + el->str;
		break;

		default:
			ls+el->str;
		break;

		}

		return ;
	}

	MString getlisting(VString code, VString data, msl_ce_values &values){
		HLString ls;
		set(code, data, &values);

		unsigned char *ln=0;
		while(ln=next(ln)){
			_msl_five_asm_data *d=getop(*(unsigned int*)ln);
			if(d){
				getlisting_el(ln+4, d, ls);
				ls+"\r\n";
			}else
				ls+"undefined code\r\n";
		}

		return VString(ls.oneline(), ls.size());
	}
};




class msl_five_docode{

	// Defines
#define MSLFV_PUSHINT(val) int old_ ## val = val;
#define MSLFV_PUSHCHR(val) unsigned char *old_ ## val = val;
#define MSLFV_PUSHSTR(cls, val) cls old_ ## val = val;

#define MSLFV_PUSHINT2(val, nval) int old_ ## val = val; val = nval;
#define MSLFV_PUSHCHR2(val, nval) unsigned char *old_ ## val = val; val = nval;
#define MSLFV_PUSHSTR2(cls, val, nval) cls old_ ## val = val; val = nval;

#define MSLFV_POPINT(val) val = old_ ## val;
#define MSLFV_POPCHR(val) val = old_ ## val;
#define MSLFV_POPSTR(cls, val) val = (cls)old_ ## val;

#define MSLFV_EXPUSHPOS()				int _tmp_push_ex_pos = proc_stack.GetPos();
#define MSLFV_EXPUSHCLS(cls, val)		cls *val = (cls*)proc_stack.PushI(sizeof(cls));
#define MSLFV_EXPUSHSTR(to, from)		to = proc_stack.PushV(from);

#define MSLFV_EXPOPPOS()				proc_stack.SetPos(_tmp_push_ex_pos);

	// Values Section

	// Process
	msl_five_proc_docode *proc_docode;
	msl_five_stack proc_stack;

	// Lexp
	msl_five_lexp *lexp;

	// Values
	msl_ce_values section_values, section_cvalues;

	// Process options
	int do_opt_stopit;
	int do_opt_line_count;
	unsigned char *do_opt_line_pos;

	// jumps
	int do_opt_ret, do_opt_con, do_opt_brk;

	union{
	int do_opt_stack_size;
	int section_stack;
	};

	// Output
	HLString0 out_text, out_code, out_data;

	// Code Section

public:

	msl_five_docode(){
		proc_docode = 0;
		do_opt_stack_size = 0;
	}

	void Clear(){
		proc_docode = 0;
		proc_stack.Clear();

		do_opt_stack_size = 0;

		out_code.Clear();
		out_text.Clear();
	}

	int Do(VString file){
		do_opt_ret = 0;
		do_opt_con = 0;
		do_opt_brk = 0;

		AddCodeInt(MCCA_JMP);
		AddCodeInt(8);
		AddCodeInt(MCCA_JMP);
		do_opt_ret = DoCreateAddInt(0);

		int r = DoFile(file);

		int p = DoCreateAddInt(MCCA_NOP);
		DoCreateUpdInt(p - do_opt_ret, do_opt_ret);

		return r;
	}

	int DoFile(VString file){
		// Push file info
		//int stack_pos = proc_stack.GetPos();
		//msl_five_proc_docode *pdc = (msl_five_proc_docode*)proc_stack.PushI(sizeof(msl_five_proc_docode));
		//pdc->prev = proc_docode;
		//pdc->file = proc_stack.PushV(file);
		//proc_docode = pdc;
		
		MSLFV_EXPUSHPOS();
		MSLFV_EXPUSHCLS(msl_five_proc_docode, pdc);
		MSLFV_EXPUSHSTR(pdc->file, file);
		
		pdc->prev = proc_docode;
		proc_docode = pdc;

		MSLFV_PUSHINT2(do_opt_stopit, 0);
		MSLFV_PUSHINT2(do_opt_line_count, 0);
		MSLFV_PUSHCHR2(do_opt_line_pos, 0);

		// Load file
		MString data = LoadFile(file);

		AddCodeComment("@file", file);
		AddCodeCommentLine(0);
		
		// Do code
		int ret = DoCode(data);

		// Pop file info
		proc_docode = pdc->prev;
		//proc_stack.SetPos(stack_pos);
		MSLFV_EXPOPPOS();

		MSLFV_POPINT(do_opt_stopit);
		MSLFV_POPINT(do_opt_line_count);
		MSLFV_POPCHR(do_opt_line_pos);

		// return ;
		return ret;
	}

	int DoCode(VString code){
		unsigned char *line = code, *pline = line, *to = code.endu();

		while(line < to && !do_opt_stopit){
			if(*line == '<' && line + 1 < to && *(line + 1) == '?'){
				// set result
				SetOutput(VString(pline, line - pline));
				// *line='<?...' skip this 2 sumbols
				line += 2;

				if(line + 3 < to && *(line + 0) == 'm' && *(line + 1) == 's' && *(line + 2) == 'l')
					line += 3;

				//msl_value outval;
				DoAnalysMulti(line, to); // , outval);

				// if(line='?>')
				if(line + 1 >= to || *line != '?' || *(line + 1) != '>'){
					// oh no
					SetError("No find '?>'");
					// exit
					return 0;
				} else{
					// *line='?>...' skip this 2 sumbols
					line += 2;
					pline = line + 1;
				}
			}else
				if(*line == '\n'){
					do_opt_line_count ++;
					do_opt_line_pos = line + 1;
					AddCodeCommentLine(do_opt_line_count);
				}

			line++;
		}

		// set result
		SetOutput(VString(pline, line-pline));

		return !do_opt_stopit;
	}

	// do code; code; code
	void DoAnalysMulti(unsigned char *&line, unsigned char *to, unsigned char ecode = 1){ //, msl_value &outval,){
		MSLFV_EXPUSHPOS();

		MSLFV_EXPUSHCLS(msl_five_lexp, nlexp);
		MSLFV_PUSHSTR2(msl_five_lexp*, lexp, nlexp);
		//lexp = nlexp;		

		while(!do_opt_stopit){
			// do msl

			MSLFV_EXPUSHPOS();
			lexp->Clear();

			DoAnalys(line, to, /*outval,*/ ';', ecode);
			DoCreate();

			MSLFV_EXPOPPOS();

			//DoCompile();			

			if(line >= to || *line != ';' || ecode == ';')
				break;

			line ++;
		}

		//lexp = nlexp;
		MSLFV_POPSTR(msl_five_lexp*, lexp);
		MSLFV_EXPOPPOS();
		return ;
	}

	void DoAnalys(unsigned char *&line, unsigned char *to, /*msl_value &outval,*/ unsigned char ecode = 1, unsigned char ecodet = 1){
//		msl_value *value=0, *pdvalue=0, *dvalue=0, *pvalue; msl_value valueline;
		unsigned char *pline = 0;// int msl_do_inc=0; // 1 - ++, 2 - --

		while(line<to && !do_opt_stopit){

			// skip space
			_skipspace(line, to);
			//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

		
			// $value
			if(*line=='$'){
				DoAnalysValue(++line, to);
				continue;
			}else
			// function
			if(*line >= 'a' && *line <= 'z' || *line >= 'A' && *line <= 'Z' || *line == '_'){
				msl_value val; int df=0;
				DoAnalysFunction(line, to, val, df);
//				if(!df){
//					msl_value *v=valueline.New();
//					v->Move(val); v->key="t";
//				}
				continue;
			}
			// ecode
			else if(*line == ecode || *line == ecodet){
				return ;
			}
		// string
			else if(*line=='"' || *line=='\''){
				pline=++line; 
				if(*(line-1)=='"')
					while(line<to){
						if(*line=='"'){
							if(pline<line && *(line-1)=='\\'){
								LexpAdd(MCCT_TEXT, 0, VString(pline, line-pline-1));
								LexpAdd(MCCT_OP, MCCT_OP_CON);
								//valueline.Add("t", VString(pline, line-pline-1));
								//valueline.Add("o", ".");
								pline=line+1; line++; continue;
							}
							break;
						}

						if(*line=='{' && line+1<to && *(line+1)=='$'){
							msl_value val;
							//valueline.Add("t", VString(pline, line-pline));
							//valueline.Add("o", ".");
							LexpAdd(MCCT_TEXT, 0, VString(pline, line-pline));
							LexpAdd(MCCT_OP, MCCT_OP_CON);

							line ++;

							DoAnalys(line, to, /*val,*/ '}');							
							//valueline.Add("t", val.val);

							//valueline.Add("o", ".");
							LexpAdd(MCCT_OP, MCCT_OP_CON);
							pline = line + 1;
						}
						line ++;
					}
				else
					while(line < to && *line != '\'')
						line++;

				if(line >= to){
					SetError("closed \" or ' not found");
					return ;
				}
				//outval.val=VString(pline, line-pline);
				LexpAdd(MCCT_TEXT, 0, VString(pline, line-pline)); //valueline.Add("t", VString(pline, line-pline));
				line ++;
				continue;
			}
			// numbers
			else if(*line >= '0' && *line <= '9'){
				pline = line;

				while(line < to && *line >= '0' && *line <= '9')
					line++;

				//outval.val=VString(pline, line-pline);
				//valueline.Add("t", VString(pline, line-pline));
				LexpAdd(MCCT_TEXT, 0, VString(pline, line-pline));
				continue;
			}
			// comments
			else if(*line == '/' && line + 1 < to && ( *(line + 1) == '/' || *(line + 1) == '*')){
				pline = line;
				line += 2;

				if(*(line - 1) == '/'){
					while(line < to && ( *line != '\r' && *line != '\n')) 
						line ++;
				} else{
					while(line < to && ( *line != '/' || *(line - 1) != '*')){
						if(*line == '\n')
							do_opt_line_count ++;
						line ++;
					}

					line ++;
				}

				LexpAdd(MCCT_COMMENT, 0, VString(pline, line - pline));
				continue;
			}
			// operators
			else if(*line=='+' || *line=='-' || *line=='*' || *line=='/' || *line=='.' || *line=='!' || *line=='<' || *line=='>' || *line=='=' || *line=='|' || *line=='&' || *line=='%' || *line=='~' || *line=='^'){
				pline = line++;

				while(*line=='+' || *line=='-' || *line=='*' || *line=='/' || *line=='.' || *line=='!' || *line=='<' || *line=='>' || *line=='=' || *line=='|' || *line=='&' || *line=='%' || *line=='~' || *line=='^')
					line++;

				if(line >= to){
					SetError("EOF");
					return ;
				}
				
				VString name(pline, line - pline);
				if(line - pline > 1 && *pline == '=' && *(pline + 1) != '='){
					name.sz = 1;
					line -= line - pline - 1;
				}

				LexpAdd(MCCT_OP, GetOptId(name));
				continue;
			}
			// ()
			else if(*line=='('){
				msl_value val;
				msl_ce_lexp_d *lup = LexpAdd(MCCT_UP);
				DoAnalys(++line, to, /*val,*/ ')');
				LexpEnd(lup);
				
				if(line >= to || *line != ')'){
					SetError("Close ')' not found");
					return ;
				}
				line ++;

				//valueline.Add("t", val.val);
				continue;
			}
			// '?>' end do code
			else if(*line == '?' && line + 1 < to && *(line + 1) == '>'){
				return ;
			}
			// what the sumbol?
			else{
				SetError(HLString() + "Unknown sumbol: '" + VString(line, 1) + "'.");
				return ;
			}


			// skip space
			//_skipspace(line, to);
			//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			line++;
		}

		SetError("EOF");
		return ;
	}

	int GetOptId(VString name){
		int id=0;

		if(name.sz==1){
			switch(*name.data){
			case '.': id = MCCT_OP_CON; break;
			case '+': id = MCCT_OP_ADD; break;
			case '-': id = MCCT_OP_SUB; break;
			case '*': id = MCCT_OP_MUL; break;
			case '/': id = MCCT_OP_DIV; break;
			case '%': id = MCCT_OP_MOD; break;
			case '!': id = MCCT_OP_NO; break;
			case '=': id = MCCT_OP_MOV; break;
			case '~': id = MCCT_OP_NOT; break;
			case '|': id = MCCT_OP_BOR; break;
			case '&': id = MCCT_OP_BAND; break;
			case '^': id = MCCT_OP_BXOR; break;
			}
		}
		else if(name.sz == 2){

			switch(*(name.data + 1)){
			case '=':
				switch(*name.data){
				case '=': id = MCCT_OP_EQ; break;
				case '!': id = MCCT_OP_NEQ; break;
				}
			break;
			case '|':
				if(*name.data == '|')
					id = MCCT_OP_OR;
			break;
			case '&':
				if(*name.data == '&')
					id = MCCT_OP_OR;
			break;
			}

			if(!id){
				if(name == "++")
					id = MCCT_OP_IADD;
				else if(name == "--")
					id = MCCT_OP_ISUB;
			}
		}

		if(!id)
			SetError(HLString() + "operator '" + name + "' not found");

		return id;
	}


	msl_value* DoAnalysValue(unsigned char*&line, unsigned char *to){
		unsigned char *pline=line;

		while(line<to && *line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line>='0' && *line<='9' || *line=='_') line++;
		if(line>=to){
			SetError(HLString()+"EOF.");
			return 0;
		}

		//msl_five_lexp_d *lup=lexp->Add(MCCT_VAL, 0, VString(pline, line-pline));
		msl_five_lexp_d *lup = LexpAdd(MCCT_VAL, 0, VString(pline, line-pline));
		
		while(1){
			// skip space 
			_skipspace(line, to);

			if(line<to && *line=='['){
				msl_value dval;
				msl_five_lexp_d *lup = LexpAdd(MCCT_PAR);
				//DoAnalys(++line, to, dval, ']');
				LexpEnd(lup);

				if(line<to && *line==']')
					line++;
			}else
				break;
		}

		LexpEnd(lup);

		return 0;
	}

	void DoAnalysFunction(unsigned char *&line, unsigned char *to, msl_value &val, int &df){
		VString name; unsigned char *pline = line;
		unsigned char *code, *ecode;

		// skip space 
		_skipspace(line, to);

		pline = line;
		while(*line >= 'a' && *line <= 'z' || *line >= 'A' && *line <= 'Z' || *line == '_' || *line >= '0' && *line <= '9')
			line++;

		if(line == pline){
			// line='functionname'EOF
			SetError("bad function name");
			return ;
		}

		name.setu(pline, line - pline);

		//if(name=="function"){
//			 DoCodeFunctionFunction(line, to); df=1; return ;
		//}

		// skip space 
		_skipspace(line, to);

		if(line<to && *line == '('){
			msl_ce_lexp_d *lup = LexpAdd(MCCT_FUNC, 0, name);

			MSLFV_PUSHSTR2(msl_five_lexp*, lexp, lup);

			if(!DoCodeFunctionArgs(line, to, name == "for"))
				return ;

			MSLFV_POPSTR(msl_five_lexp*, lexp);
			LexpEnd(lup);

			if(name == "if"){
				lup->id = MCCT_EXTFUNC;

				DoCreate();

				// Add jmp
				DoCreateAddInt(MCCA_NEJMP);
				DoCreateAddValue(lexp->_a->_a);
				int jto = DoCreateAddInt(0);

				// Add Code {}
				if(!DoCodeFunctionBlock(line, to))
					return ;

				// else
				unsigned char *tline = line;
				if(tline < to && *tline == ';'){
					tline ++;
				}
					
				_skipspace(tline, to);
				if(tline + 4 < to && *tline == 'e' && *(tline + 1) == 'l' && *(tline + 2) == 's' && *(tline + 3) == 'e')
					line = tline + 4;
				else
					tline = 0;


				if(tline){
					DoCreateAddInt(MCCA_JMP);
					int jto2 = DoCreateAddInt(0);

					// Add end if
					int jend = DoCreateAddInt(MCCA_NOP);
					DoCreateUpdInt(jend - jto, jto);

					if(!DoCodeFunctionBlock(line, to))
						return ;

					int jend2 = DoCreateAddInt(MCCA_NOP);
					DoCreateUpdInt(jend2 - jto2, jto2);
				} else {
					// Add end
					int jend = DoCreateAddInt(MCCA_NOP);
					DoCreateUpdInt(jend - jto, jto);
				}

				LexpClear();
				return ;
			}
			else if(name == "while"){
				lup->id = MCCT_EXTFUNC;

				DoCreateAddInt(MCCA_JMP);
				DoCreateAddInt(8);
				DoCreateAddInt(MCCA_JMP);
				int jtoend = DoCreateAddInt(0);

				int jloop = DoCreateAddInt(MCCA_NOP);

				DoCreate();

				// Push con & break;
				MSLFV_PUSHINT2(do_opt_con, jloop);
				MSLFV_PUSHINT2(do_opt_brk, jtoend - 4);

				// Add jmp
				DoCreateAddInt(MCCA_NEJMP);
				DoCreateAddValue(lexp->_a->_a);
				int jtoend2 = DoCreateAddInt(0);

				// Add Code {}
				if(!DoCodeFunctionBlock(line, to))
					return ;

				// Pop con & break;
				MSLFV_POPINT(do_opt_con);
				MSLFV_POPINT(do_opt_brk);

				int p = DoCreateAddInt(MCCA_JMP);
				DoCreateAddInt(jloop - p - 8);

				int jend = DoCreateAddInt(MCCA_NOP);
				DoCreateUpdInt(jend - jtoend, jtoend);
				DoCreateUpdInt(jend - jtoend2, jtoend2);

				LexpClear();
				return ;
			} else if(name == "for"){
				lup->id = MCCT_EXTFUNC;

				if(lexp->_a->Size() != 3){
					SetError("Bad for parameters");
					return ;
				}

				msl_five_lexp_d *fr = lexp->_a->GetFirst();

				msl_five_docreate_opt opt;
				DoCreate(fr->_a, fr->_a, opt);
				fr = fr->next->next;

				DoCreateAddInt(MCCA_JMP);
				int jskip = DoCreateAddInt(0);

				DoCreateAddInt(MCCA_JMP);
				int jtoend = DoCreateAddInt(0);

				int jloop = DoCreateAddInt(MCCA_NOP);

				DoCreate(fr->_a, fr->_a, opt);
				fr = fr->prev;

				int jp = DoCreateAddInt(MCCA_NOP);
				DoCreateUpdInt(jp - jskip, jskip);

				DoCreate(fr->_a, fr->_a, opt);

				// Push con & break;
				MSLFV_PUSHINT2(do_opt_con, jloop);
				MSLFV_PUSHINT2(do_opt_brk, jtoend - 4);

				// Add jmp
				DoCreateAddInt(MCCA_NEJMP);
				DoCreateAddValue(fr->_a);
				int jfail = DoCreateAddInt(0);

				// Add Code {}
				if(!DoCodeFunctionBlock(line, to))
					return ;

				// Pop con & break;
				MSLFV_POPINT(do_opt_con);
				MSLFV_POPINT(do_opt_brk);

				int p = DoCreateAddInt(MCCA_JMP);
				DoCreateAddInt(jloop - p - 8);

				int jend = DoCreateAddInt(MCCA_NOP);
				DoCreateUpdInt(jend - jfail, jfail);
				DoCreateUpdInt(jend - jtoend, jtoend);

				LexpClear();
				return ;
			}

			//LexpEnd(lup);
			if(lup->_a){ lup->ext = lexp->Size(lup->_a, lup->_e)/2; }
		} else{
			if(name == "continue"){
				if(!do_opt_con){
					SetError("continue error");
					return ;
				}

				DoCreateAddJmp(do_opt_con);
			}
			else if(name == "break"){
				if(!do_opt_brk){
					SetError("break error");
					return ;
				}

				DoCreateAddJmp(do_opt_brk);
			}
			else if(name == "return"){
				DoCreateAddJmp(do_opt_ret);
			}
			else
				SetError("open ( not found");
		}

		return ;
	}

	int DoCodeFunctionBlock(unsigned char *&line, unsigned char *to){
		unsigned char endcode;
		// skip space
		_skipspace(line, to);
		
		if(line >= to){
			SetError("if(...) EOF");
			return 0;
		}

		if(*line == '{'){
			endcode = '}';
			line ++;
		} else
			endcode=';';

		//msl_value val;
		msl_ce_lexp_d *lup = LexpAdd(MCCT_PAR);
		MSLFV_PUSHSTR2(msl_five_lexp*, lexp, lup);

		//DoAnalys(line, to, /*val,*/ endcode);
		DoAnalysMulti(line, to, /*val,*/ endcode);

		MSLFV_POPSTR(msl_five_lexp*, lexp);

		LexpEnd(lup);


		//DoAnalys(line, to, /*val,*/ endcode);

		if(endcode != ';')
			line ++;

		return 1;
	}

	int DoCodeFunctionArgs(unsigned char *&line, unsigned char *to, int isfor){
		line++;
		// skip space
		_skipspace(line, to);
		// no data
		if(line < to && *line == ')'){
			line ++;
			return 1;
		}

		while(!do_opt_stopit){
			// skip space
			_skipspace(line, to);			

			msl_value val;
			msl_ce_lexp_d *lup = LexpAdd(MCCT_PAR);
			MSLFV_PUSHSTR2(msl_five_lexp*, lexp, lup);

			DoAnalys(line, to, /*val,*/ !isfor ? ',' : ';', ')');

			MSLFV_POPSTR(msl_five_lexp*, lexp);

			LexpEnd(lup);

			if(line >= to){
				SetError("not found ')'. EOF");
				return 0;
			}

			if(*line != (!isfor ? ',' : ';'))
				break;
			
			line ++;
		}

		if(line < to && *line == ')'){
			line ++;
			return 1;
		}

		SetError("Function parameters fail.");
		return 0;
	}

	// Lexp
	msl_five_lexp_d* LexpAdd(int id, int ext = 0, VString val = VString()){
		MSLFV_EXPUSHCLS(msl_five_lexp_d, lup);
		//lup->_a = 0;
		//lup->_e = 0;
		//lup->_p = 0;
		//lup->_n = 0;

		lexp->Add(lup);

		lup->id = id;
		lup->ext = ext;
		MSLFV_EXPUSHSTR(lup->text, val);

		return lup;
	}

	void LexpEnd(msl_five_lexp_d *el){
		if(!el->_n)
			return ;

		el->_a = el->_n;
		el->_e = el->GetEnd(el); //lexp->GetEnd();

		lexp->_e = el;

		if(el->_n)
			el->_n->_p = 0;
		el->_n = 0;

		return ;
	}

	void LexpDel(msl_five_lexp_d *el){
		if(!el)
			return ;

		lexp->OMDel(el);

		return ;
	}

	void LexpClear(){

		lexp->_a = 0;
		lexp->_e = 0;

		return ;
	}

	// Do Create
	void DoCreate(){
		msl_five_lexp_d *fr = lexp->GetFirst(), *to = lexp->GetEnd();
		if(!lexp->GetFirst())
			return ;
		
		msl_five_docreate_opt opt;
		DoCreate(fr, to, opt);
		return ;
	}

	int DoCreate(msl_five_lexp_d *&fr, msl_five_lexp_d *&to, msl_five_docreate_opt &opt){
		if(!fr)
			return 0;

		// find =
		/*for(int i=eps-1; i<eps; i--){
			if(ep[i]->id==MCCT_OP && (ep[i]->ext>=MCCT_OP_MODE && ep[i]->ext<=MCCT_OP_MODE)){
				int neps=eps-i-1;
				DoCreate(ep+i+1, neps);
				if(neps!=1) SetError("CreateCode, fail.");
			}
		}*/

		// update options
		opt.level ++;

		unsigned int jmpend = -1, jmpstack = 0; 


		msl_five_lexp_d *el, *tel;

		for(int op = 0; op < 10; op++){
			for(el = fr; el; el = el->_n){		



//  <<<<<<<<
	if(op == 0){
		// MCCT_NOUSE MCCT_COMMENT MCCT_NEWLINE
		while(el->id == MCCT_NOUSE || el->id == MCCT_COMMENT || el->id == MCCT_NEWLINE){
			tel=el; el=el->prev;
			if(fr==tel) fr=tel->next; if(to==tel) to=el; if(!el) el=fr; if(!fr) return 0;
			LexpDel(tel);
			if(!el)
				break;
		}

		if(el->id==MCCT_OP && (el->ext==MCCT_OP_OR || el->ext==MCCT_OP_AND)){
			if(!el->prev){
				SetError("data no found prev || or &&");
				return 0;
			}

			// Values
			unsigned int t;

			// Add code
			if(jmpend==-1){
				jmpstack = section_stack;
				section_stack += sizeof(msl_value);

				DoCreateAddInt(MCCA_JMP);
				DoCreateAddInt(8);
				DoCreateAddInt(MCCA_JMP);
				jmpend = DoCreateAddInt(0);
			}

			// left ||
			if(lexp->Size(fr, el->prev) != 1 || ( el->prev->id != MCCT_SVAL && el->prev->id != MCCT_TEXT) )
				DoCreate(fr, el->prev, opt);

			if(lexp->Size(fr, el->prev) != 1  || ( el->prev->id != MCCT_SVAL && el->prev->id != MCCT_TEXT)){
				SetError("left part of || or && is bad");
				return 0;
			}

			// Nop
			t = DoCreateAddInt(MCCA_NOP) + 4;
			
			// Mov
			DoCreateAddInt(MCCA_MOV);
			DoCreateAddInt(jmpstack);
			DoCreateAddInt(el->prev->ext);

			// Jmp
			DoCreateAddInt(el->ext == MCCT_OP_OR ? MCCA_EJMP : MCCA_NEJMP);
			DoCreateAddInt(jmpend - t);

			el=el->next; fr=el;
			if(!el){
				SetError("right part of || or && is bad");
				return 0;
			}

			LexpDel(el->prev);
			LexpDel(el->prev);
		}
		
		if(el->id==MCCT_OP && el->ext==MCCT_OP_MOV){
			if(!DoCreate(el->next, to, opt))
				return 0;

			if(el->prev->id!=MCCT_VAL){
				SetError("DoCreate $value not found prev =");
				return 0;
			}

			// Open Values
			DoCreateOpenValue(el->prev, el->prev);
			DoCreateOpenValue(el->next, el->next);

			// Add code
			DoCreateAddInt(MCCA_MOV);
			DoCreateAddInt(el->prev->ext);
			DoCreateAddInt(el->next->ext);
			//int s=DoCreateAddValue(el->prev);
			//DoCreateAddValue(el->next);

			// Update
			el->id=MCCT_SVAL; el->ext=el->prev->ext;

			// Del
			if(fr==el->prev) fr=el; if(to==el->next) to=el;
			LexpDel(el->prev); LexpDel(el->next);
		}
	}
	else if(op==1){

		// MCCT_OP +, -, !, ~
		if(el->id==MCCT_OP){
			if((!el->prev || el->prev->id==MCCT_OP) && ( el->ext==MCCT_OP_ADD || el->ext==MCCT_OP_SUB || el->ext==MCCT_OP_NO || el->ext==MCCT_OP_NOT)){
				if(!el->next){
					SetError("bad value"); return 0;
				}

				DoCreateOpenValue(el->next, el->next);
				
				// Add code
				DoCreateAddInt(DoCreateGetUnOp(el->ext));
				int s=DoCreateAddStackValue(el->prev, el->next);
				DoCreateAddInt(s);
				DoCreateAddValue(el->next); 
					
				// Del
				if(to==el->next) to=el;
				LexpDel(el->next);
					
				// Update
				el->id=MCCT_SVAL; el->ext=s;
			}

			if(el->ext == MCCT_OP_IADD || el->ext == MCCT_OP_ISUB){				
				if(el->prev && DoCreateIsValue(el->prev)){
					int s = DoCreateAddStackValue(el->prev, el->next);

					// Add MOV
					DoCreateAddInt(MCCA_COPY);
					DoCreateAddInt(s);
					DoCreateAddValue(el->prev);

					// ADD INC
					DoCreateAddInt(DoCreateGetUnOp(el->ext));
					DoCreateAddInt(el->prev->ext);
					
					el->ext = s;
					
					// Del
					if(fr == el->prev)
						fr = el;
					LexpDel(el->prev);

				} else if(el->next && DoCreateIsValue(el->next)){
					
					// Add INC
					DoCreateAddInt(DoCreateGetUnOp(el->ext));
					DoCreateAddValue(el->next);
					
					el->ext=el->next->ext;

					// Del
					if(to==el->next) to=el;
					LexpDel(el->next);					
				}

				// Update
				el->id=MCCT_SVAL;
			}

		}

		// MCCT_OP | &
		if(el->id==MCCT_OP && (el->ext==MCCT_OP_BOR || el->ext==MCCT_OP_BAND || el->ext==MCCT_OP_BXOR)){
			DoCreateAddOp(el, fr, to);
		}

	}else if(op==2){
		// MCCT_OP *, /, %
		if(el->id==MCCT_OP && (el->ext==MCCT_OP_MUL || el->ext==MCCT_OP_DIV || el->ext==MCCT_OP_MOD)){
			DoCreateAddOp(el, fr, to);
		}
	}else if(op==3){
		// MCCT_OP +, -, ., <<, >>, <, <=, >, >=, ==, !=, &, ^, |
		if(el->id==MCCT_OP && (el->ext==MCCT_OP_ADD || el->ext==MCCT_OP_SUB || el->ext==MCCT_OP_CON || el->ext==MCCT_OP_EQ || el->ext==MCCT_OP_NEQ)){
			DoCreateAddOp(el, fr, to);
		}
	}else if(op == 4){

		// MCCT_UP
		if(el->id == MCCT_UP){
			DoCreate(el->_a, el->_e, opt);
			unsigned int p = DoCreateGetValue(el->_a);
			el->id = MCCT_SVAL;
			el->ext = p;
		}

		// MCCT_VAL
		if(el->id == MCCT_VAL){
			DoCreateOpenValue(el, el);
		}

		// MCCT_TEXT
		if(el->id == MCCT_TEXT){
			el->ext = DoCreateGetValue(el);
			el->id = MCCT_SVAL;
		}

		// MCCT_FUNC
		if(el->id == MCCT_FUNC){
			opt.ispar = 1;
			DoCreate(el->_a, el->_e, opt);
			opt.ispar = 0;

			int cnt = 0;
			tel = el->_a;
			while(tel){
				cnt ++;
				tel = tel->next;
			}

			el->ext = cnt;
					
			//msl_functions_d *f=functions.Get(el->text, el->ext);
			msl_five_functions_d *f = msl_five_functions_list.Get(el->text, el->ext);

			if(!f){
				SetError(HLString()+"DoCreate function '" + el->text + "' not found");
				return 0;
			}

			// Add op & param size
			DoCreateAddInt(MCCA_FUNC);
			DoCreateAddVoid(f);
			DoCreateAddInt(el->ext);
						
			// Add return
			unsigned int s = DoCreateAddStackValue(el->prev, el->next);
			DoCreateAddInt(s);

			// Add params
			tel = el->_a;
			while(tel){
				DoCreateAddInt(DoCreateGetValue(tel));
				tel = tel->next;
			}

			el->id = MCCT_SVAL;
			el->ext = s;
		}

		// MCCT_EXFUNC
		if(el->id == MCCT_EXTFUNC){
			opt.ispar = 1;
			DoCreate(el->_a, el->_e, opt);
			opt.ispar = 0;

			/*
			int cnt = 0;
			tel = el->_a;
			while(tel){
				cnt ++;
				tel = tel->next;
			}

			el->ext = cnt;

			if(el->text == "if"){
				// Add jmp
				DoCreateAddInt(MCCA_NEJMP);
				DoCreateAddValue(el->_a);
				int jto = DoCreateAddInt(0);

				// Add Block
				//DoCreate(el->_e->next->_a, el->_e->next->_e, opt);
				DoCreate(el->_a->next->_a, el->_a->next->_e, opt);

				// Add code
				int jend = DoCreateAddInt(MCCA_NOP);
				DoCreateUpdInt(jend - jto, jto);

				if(el->_a->_n->_n && el->_a->_n->_n->id == MCCT_CB && el->_a->_n->_n->text == "else"){
					DoCreateAddInt(MCCA_JMP);
					int jto2 = DoCreateAddInt(0);

					int jend = DoCreateAddInt(MCCA_NOP);
					DoCreateUpdInt(jend - jto, jto);

					DoCreate(el->_a->_n->_n->_a, el->_a->_n->_n->_e, opt);

					int jend2 = DoCreateAddInt(MCCA_NOP);
					DoCreateUpdInt(jend2 - jto2, jto2);
				} else {
					int jend = DoCreateAddInt(MCCA_NOP);
					DoCreateUpdInt(jend - jto - 4, jto);
				}

				el->id = el->_a->id;
				el->ext = el->_a->ext;
			}*/
		}

		// MCCT_PAR
		if(el->id==MCCT_PAR){
			DoCreate(el->_a, el->_e, opt);
			el->id=el->_a->id; el->ext=el->_a->ext; el->text=el->_a->text;
		}

		// MCCT_OP
		if(el->id==MCCT_OP){
			if(el->ext==MCCT_OP_ADD || el->ext==MCCT_OP_CON){
				if(!el->prev || !el->next){
					SetError("bad value");
					return 0;
				}

				// Add code
				DoCreateAddInt(DoCreateGetOp(el->ext));
				DoCreateAddValue(el->prev);
				DoCreateAddValue(el->next); 
				int s=DoCreateAddStackValue(el->prev, el->next);
				DoCreateAddInt(s);
					
				// Del
				if(fr==el->prev) fr=el; if(to==el->next) to=el;
				LexpDel(el->prev); LexpDel(el->next);
					
				// Update
				el->id=MCCT_SVAL; el->ext=s;
			}
		}
	}
//  >>>>>>>>
			}
		
			// end of || or && code
			if(op == 0 && jmpend != -1){
				if(lexp->Size(fr, to) != 1 || (fr->id != MCCT_SVAL && fr->id != MCCT_TEXT))
					DoCreate(fr, to, opt);

				if(el)
				if(lexp->Size(fr, el->prev) != 1 || fr->id != MCCT_SVAL){
					SetError("left part of || or && is bad");
					return 0;
				}

				// Mov
				DoCreateAddInt(MCCA_MOV);
				DoCreateAddInt(jmpstack);
				DoCreateAddInt(fr->ext);

				fr->id=MCCT_SVAL; fr->ext=jmpstack;
				break;	
			}
		}		

		if(!opt.ispar)
			//if(lexp->Size(fr, to) != 1 || ( lexp->_a->id != MCCT_VAL && lexp->_a->id != MCCT_SVAL)){
			if(lexp->Size(fr, to) != 1 || fr->_a && ( fr->_a->id != MCCT_VAL && fr->_a->id != MCCT_SVAL)){
				SetError("DoCreate code_size!=1");
				return 0;
			}

		// update options
		opt.level--;

		return 1;
	}

	// Do Create Add code
	unsigned int DoCreateAddInt(unsigned int op){
		return out_code.addnf((unsigned char*)&op, sizeof(op));
	}

	unsigned int DoCreateAddJmp(unsigned int pos){
		DoCreateAddInt(MCCA_AJMP);
		pos = DoCreateAddInt(pos);

		return pos;
	}

	unsigned int  DoCreateAddVoid(void *op){
		return out_code.addnf((unsigned char*)&op, sizeof(op));
	}

	unsigned int DoCreateAddValue(msl_five_lexp_d *el){
		unsigned int p=DoCreateGetValue(el);
		DoCreateAddInt(p);
		return p;
	}

	unsigned int DoCreateUpdInt(unsigned int op, int pos){
		if(out_code.Size() < pos + sizeof(op))
			return 0;

		*(unsigned int*)out_code.Get(pos) = op;

		return 1;
	}

	int DoCreateAddOp(msl_five_lexp_d *el, msl_five_lexp_d *&fr, msl_five_lexp_d *&to){
		if(!el->prev || !el->next){
			SetError("bad value"); return 0;
		}

		// Add code
		DoCreateAddInt(DoCreateGetOp(el->ext));
		int s=DoCreateAddStackValue(el->prev, el->next);
		DoCreateAddInt(s);
		DoCreateAddValue(el->prev);
		DoCreateAddValue(el->next);
					
		// Del
		if(fr==el->prev) fr=el; if(to==el->next) to=el;
		LexpDel(el->prev); LexpDel(el->next);
					
		// Update
		el->id=MCCT_SVAL; el->ext=s;
		return 1;
	}

	int DoCreateIsValue(msl_ce_lexp_d *el){
		if(el->id == MCCT_VAL || el->id == MCCT_SVAL) //  || el->id==MCCT_FUNC
			return 1;
		return 0;
	}

	// Value -> Stack Value
	void DoCreateOpenValue(msl_ce_lexp_d *el, msl_ce_lexp_d *to){
		while(el){
			if(el->id==MCCT_VAL){
				// Get Value
				msl_ce_values_d *val=section_values.Get(el->text);
				if(!val){
					val=section_values.Add(el->text, section_stack);
					section_stack+=sizeof(msl_value);
				}

				// Is Up
				if(el->_a){
					msl_five_docreate_opt opt; opt.ispar=1;
					DoCreate(el->_a, el->_e, opt);

					// Add Code: MCCA_VUP (Value Up)
					DoCreateAddInt(MCCA_VUP);
					DoCreateAddInt(section_stack|MCCTV_PVAL);
					DoCreateAddInt(val->pos);
					DoCreateAddInt(lexp->Size(el->_a, el->_e));

					// Add Up Keys
					msl_ce_lexp_d *up=el->_a;
					while(up){
						if(up->id==MCCT_SVAL)
							DoCreateAddInt(up->ext);
						else
							SetError("DoCreateOpenValue() non stask value");
						up=up->next;
					}

					el->ext=section_stack|MCCTV_PVAL;
					section_stack+=sizeof(msl_value);
				}
				else{
					 el->ext=val->pos;
				}

				el->id=MCCT_SVAL;
			}

			if(el==to) break;
			el=el->next;			
		}
		return ;
	}

	// Text or Value or Stack Value -> Stack Value
	unsigned int DoCreateGetValue(msl_ce_lexp_d *el){
		if(el->id==MCCT_SVAL){
			return el->ext;
		}else 
			if(el->id==MCCT_VAL){
				DoCreateOpenValue(el, el);
				return el->ext;
				//SetEpic("DoCreateAddValue() disable operation");
		}
		else if(el->id == MCCT_TEXT){
			int p = out_data.Size();
			out_data.add((char*)&el->text.sz, 4);			
			out_data + el->text;
			//DoCreateAddInt(p);
			return MCCTV_DATA | p;
		}else
			SetEpic("DoCreateAddValue() bad value type");
		return 0;
	}

	int DoCreateAddStackValue(msl_ce_lexp_d *l, msl_ce_lexp_d *r){
		//if(l->id==MCCT_SVAL) return l->ext;
		//if(r->id==MCCT_SVAL) return r->ext;
		
		int p=section_stack;
		section_stack+=sizeof(msl_value);
		return p;
	}

	int DoCreateTestValue(msl_ce_lexp_d *el){
		if(el->id!=MCCT_SVAL && el->id!=MCCT_VAL && el->id!=MCCT_TEXT) return 0;
		return 1;
	}

	int DoCreateGetOp(int op){
		switch(op){
			case MCCT_OP_ADD: return MCCA_ADD; break;
			case MCCT_OP_SUB: return MCCA_SUB; break;
			case MCCT_OP_MUL: return MCCA_MUL; break;
			case MCCT_OP_DIV: return MCCA_DIV; break;
			case MCCT_OP_MOD: return MCCA_MOD; break;
			case MCCT_OP_CON: return MCCA_CON; break;
			case MCCT_OP_EQ: return MCCA_EQ; break;
			case MCCT_OP_NEQ: return MCCA_NEQ; break;
			case MCCT_OP_BOR: return MCCA_BOR; break;
			case MCCT_OP_BAND: return MCCA_BAND; break;
			case MCCT_OP_BXOR: return MCCA_BXOR; break;
		}
		return 0;
	}

	int DoCreateGetUnOp(int op){
		switch(op){
			case MCCT_OP_ADD: return MCCA_UADD; break;
			case MCCT_OP_SUB: return MCCA_USUB; break;
			case MCCT_OP_NO: return MCCA_UNO; break;
			case MCCT_OP_NOT: return MCCA_UNOT; break;	
			case MCCT_OP_IADD: return MCCA_IADD; break;
			case MCCT_OP_ISUB: return MCCA_ISUB; break;
		}
		return 0;
	}

	
	// Add Code Prime
	void AddCodeInt(int i){
		out_code + VString((char*)&i, 4);
	}

	void AddCodeString(VString line){
		out_code + VString((char*)&line.sz, 4) + line;
	}

	// Add Code 
	void AddCodeComment(VString key, VString val){
		AddCodeInt(MSLA_COMMENT);
		AddCodeString(key);
		AddCodeString(val);
		return ;
	}

	void AddCodeCommentLine(int line){
		AddCodeInt(MSLA_COMMENT_LINE);
		AddCodeInt(line);
		
		return ;
	}

	// Simple Functions
	void _skipspace(unsigned char *&line, unsigned char *to){
		//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' && (line+1>=to || *(line+1)=='\n' || line++ && ++do_line_count && (do_line=line+1))
		//	|| (*line=='\n' && ++do_line_count && (do_line=line+1) ))) line++;

		int mod = 0;

		while(line<to){
			if(*line == ' ' || *line == '\t' || *line == '\r')
				line ++;

			//else if(*line == '\r'){
			//	if(line + 1 < to || *(line + 1) == '\n'){
			//		do_opt_line_count ++;
			//		do_opt_line_pos = line + 2;
			//		mod = 1;
			//	}

			//	line ++;
			//}
			else if(*line == '\n'){
				line ++;
				do_opt_line_count ++;
				do_opt_line_pos = line;
				mod = 1;
			}
			else
				break;
		}

		if(mod)
			AddCodeCommentLine(do_opt_line_count);

		return ;
	}


	// Get output
	VString GetOutput(){
		return VString(out_text.oneline(), out_text.size());
	}

	VString GetOutputCode(){
		return VString(out_code.oneline(), out_code.size());
	}

	VString GetOutputData(){
		return VString(out_data.oneline(), out_data.size());
	}

	MString GetCodeAsm(){
		msl_five_asm masm;
		return masm.getlisting(VString(out_code.oneline(), out_code.size()), VString(out_data.oneline(), out_data.size()), section_values);
	}

	msl_ce_values* GetValues(){
		return &section_values;
	}

	//MString GetCompileValues(){
	//	HLString ls; int f=1;
	//	msl_ce_values_d *el=section_cvalues._a;
	//	while(el){
	//		if(f){ ls+"msl_value "; f=0; } else ls+", ";
	//		ls+"$"+el->key;
	//		el=el->_n;
	//	}

	//	if(!f) ls+";";

	//	return ls;
	//}


	protected:

	// Output functions
	void SetOutput(VString line){
		out_text + line;
	}

	void SetOutputCode(VString line){
		out_text + line;
	}

	void SetWarning(VString line){
		// add error line to result
		out_text + "MSL-FV Warning: '" + line + "' in '" + proc_docode->file + "' on " + (do_opt_line_count + 1) + " line\r\n";
		return ;
	}

	void SetError(VString line){
		// add error line to result
		out_text + "MSL-FV Error: '" + line + "' in '" + proc_docode->file + "' on " + (do_opt_line_count + 1) + " line\r\n";
		// stop
		do_opt_stopit = 1;
		return ;
	}

	void SetEpic(VString line){
		// add error line to result
		out_text + "MSL-FV Epic Fail: '" + line + "' in '" + proc_docode->file + "' on " + (do_opt_line_count + 1) + " line\r\n";
		// stop
		do_opt_stopit = 1;
		return ;
	}

};