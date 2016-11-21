// Msl Assembly
#define MSLA_COMMENT	1 // comment: VString key, val;
#define MSLA_COMMENT_LINE	2 // comment: int line;
#define MCCA_NOP		99 // no operaion
#define MCCA_ADD		100 // add [space]$a [space]$b => $c
#define MCCA_SUB		101 // sub [space]$a [space]$b => $c
#define MCCA_MUL		102 // mul [space]$a [space]$b => $c
#define MCCA_DIV		103 // div [space]$a [space]$b => $c
#define MCCA_MOD		104 // mod [space]$a [space]$b => $c
#define MCCA_CON		105 // com [space]$a [space]$b => $c

#define MCCA_BOR		106 // $c = $a | $b
#define MCCA_BAND		107 // $c = $a & $b
#define MCCA_BXOR		108 // $c = $a ^ $b

#define MCCA_COPY		114 // copy [space]$to [space]$from
#define MCCA_MOV		115 // mov [space]$to [space]$from

#define MCCA_JMP		120 // jmp: +bytes
#define MCCA_EJMP		121 // equalent jmp: if $val==1, +bytes 
#define MCCA_NEJMP		122 // non equalent jmp: if $val==0, +bytes 

#define MCCA_AJMP		123 // ajmp: pos

#define MCCA_FUNC		130	// exec msl_func*, int pars, [space]$ret, [space]$params...;
#define MCCA_VUP		131	// val up, $res, $val, int pars, [$par1, $par2, ... ]

#define MCCA_EQ			150	// eq $a == $b
#define MCCA_NEQ		151	// neq $a != $b

#define MCCA_UADD		200 // unary add
#define MCCA_USUB		201 // unary sub
#define MCCA_UNO		202 // unary !
#define MCCA_UNOT		203 // unary ~
#define MCCA_IADD		204 // ++$a or $a++ 
#define MCCA_ISUB		205 // --$a or $a-- 

// spaces: 0 - data section, 1 - value section

// msl assembler
struct _msl_five_asm_data{
	int code, sz;
	VString str;
};

_msl_five_asm_data msl_five_asm_data[]={
	MSLA_COMMENT, -1, "comment",
	MSLA_COMMENT_LINE, 4, "line",
	MCCA_NOP, 0, "nop",
	MCCA_ADD, 12, "add $ = $ + $",
	MCCA_SUB, 12, "sub $ = $ - $",
	MCCA_MUL, 12, "mul $ = $ * $",
	MCCA_DIV, 12, "div $ = $ / $",
	MCCA_MOD, 12, "mod $ = $ % $",
	MCCA_CON, 12, "con $ = $ . $",
	MCCA_BOR, 12, "$ = $ | $",
	MCCA_BAND, 12, "$ = $ & $",
	MCCA_BXOR, 12, "$ = $ ^ $",
	MCCA_COPY, 8, "copy $ = $",
	MCCA_MOV, 8, "mov $ = $",
	MCCA_FUNC, -1, "call $ = func $...",
	MCCA_VUP, -1, "$[$...]",
	MCCA_JMP, 4, "jmp int",
	MCCA_AJMP, 4, "ajmp int",
	MCCA_EJMP, 8, "jmp ($ == 1) int",
	MCCA_NEJMP, 8, "jmp ($ == 0) int",
	MCCA_UADD, 4, "uadd $",
	MCCA_USUB, 4, "usub $",
	MCCA_UNO, 4, "!$",
	MCCA_UNOT, 4, "~$",
	MCCA_IADD, 4, "++$",
	MCCA_ISUB, 4, "--$",
	MCCA_EQ, 12, "eq $ == $",
	MCCA_NEQ, 12, "eq $ != $",
};

// stack values data
struct msl_ce_values_d{
	msl_ce_values_d *_p, *_n;

	MString key; int pos;
};

class msl_ce_values : public OMatrixT<msl_ce_values_d>{

public:
	// constructor
	msl_ce_values(){ }
	~msl_ce_values(){ OMClear(); }

	msl_ce_values_d* Add(VString key, unsigned int pos=0){
		msl_ce_values_d*el=OMNew();
		el->key=key; el->pos=pos;
		return el;
	}

	msl_ce_values_d* Get(VString key){
		msl_ce_values_d *el=_a;
		while(el){
			if(el->key==key)
				return el;
			el=el->_n;
		}
		return 0;
	}

	msl_ce_values_d* GetByPos(int pos){
		if(!this)
			return 0;

		msl_ce_values_d *el=_a;
		while(el){
			if(el->pos==pos)
				return el;
			el=el->_n;
		}
		return 0;
	}

};

class msl_ce_code_datavalue{
public:
	int sz; unsigned char data[1];
};

struct msl_ce_getpointer{ msl_value *val; };


class msl_five_exec{

	// Input
	VString code;
	VString data;
	msl_ce_values *values;

	// Process
	MString stack;

	int do_opt_exec;
	int do_opt_asm;

	// Output
	HLString0 out_text;
	HLString0 out_asm;

public:
	msl_five_exec(){
		values = 0;

		do_opt_exec = 1;
		do_opt_asm = 0;	
	}

	// Options
	int GetOptExec(){
		return do_opt_exec;
	}

	int GetOptAsm(){
		return do_opt_asm;
	}

	void SetOptExec(int v){
		do_opt_exec = v;
	}

	void SetOptAsm(int v){
		do_opt_asm = v;
	}

	// Execute
	int Execute(VString ncode, VString ndata, msl_ce_values *nval = 0){
		code = ncode;
		data = ndata;
		values = nval;

		stack.Reserv(S1M);

		return ExecuteCode();
	}

	int ExecuteCode(){
		unsigned char *ln = 0;
		int csize;
		_msl_five_asm_data *op;

		while(ln = NextCode(ln, csize, op)){
			if(do_opt_asm)
				if(!AsmOne(ln, csize, op))
					return 0;

			if(do_opt_exec)
				if(!ExecuteOne(ln, csize, op))
					return 0;
		}

		//if(ln != code.endu())
		//	return 0;

		return 1;
	}

	int ExecuteOne(unsigned char *&rln, int &csize, _msl_five_asm_data *op){
		// tmp
		MString tmp;
		msl_value tv0, tv1;

		unsigned char *ln = rln + 4;

		switch(op->code){
			case MSLA_COMMENT:
			case MSLA_COMMENT_LINE:
				break;

			case MCCA_ADD:
			case MCCA_SUB:
			case MCCA_CON:
			case MCCA_MUL:
			case MCCA_DIV:
			case MCCA_MOD:
			case MCCA_EQ:
			case MCCA_NEQ:

			case MCCA_BOR:
			case MCCA_BAND:
			case MCCA_BXOR:
				{
				msl_value &res=DoRunGetVal(*(unsigned int*)ln, tv1, data, stack); ln+=4;
				msl_value &l=DoRunGetVal(*(unsigned int*)ln, tv0, data, stack); ln+=4;
				msl_value &r=DoRunGetVal(*(unsigned int*)ln, tv1, data, stack); ln+=4;
				ExecuteMathOp(op->code, l.val, r.val, &res);
			break;}

			case MCCA_COPY:{
				msl_value &l = DoRunGetVal(*(unsigned int*)(ln), tv0, data, stack); ln += 4;
				msl_value &r = DoRunGetVal(*(unsigned int*)(ln), tv1, data, stack); ln += 4;
				l.Copy(r);
			break;}
			
			case MCCA_MOV:{
				msl_value &l = DoRunGetVal(*(unsigned int*)(ln), tv0, data, stack); ln += 4;
				msl_value &r = DoRunGetVal(*(unsigned int*)(ln), tv1, data, stack); ln += 4;
				l.Move(r);
			break;}

			case MCCA_IADD:
			case MCCA_ISUB:{
				msl_value &res = DoRunGetVal(*(unsigned int*)ln, tv1, data, stack); ln += 4;
				ExecuteMathOp(op->code, res.val, &res);
			break;}

			//case MCCA_UADD: case MCCA_USUB: case MCCA_UNO: case MCCA_UNOT:
			//	{
			//	msl_value &l=DoRunGetVal(*(unsigned int*)(ln), tv0, data, stack); ln+=4;
			//	msl_value &r=DoRunGetVal(*(unsigned int*)(ln), tv1, data, stack); ln+=4;
			//	if(id==MCCA_UADD) l.val=r.val;
			//	else if(id==MCCA_USUB) l.val.Add("-", r.val);
			//	else if(id==MCCA_UNO) l.val = (r.val && r.val!="0") ? "0" : "1";
			//	else if(id==MCCA_UNOT) l.val = itos(~r.val.toi());
			//break;}

			//case MCCA_IADD: case MCCA_ISUB:{
			//	msl_value &l=DoRunGetVal(*(unsigned int*)(ln), tv0, data, stack); ln+=4;
			//	if(id==MCCA_IADD) l.val=itos(l.val.toi()+1);
			//	else if(id==MCCA_ISUB) l.val=itos(l.val.toi()-1);
			//break;}

			//case MCCA_VUP:{
			//	unsigned int res=*(unsigned int*)ln; ln+=4;
			//	unsigned int val=*(unsigned int*)ln; ln+=4;
			//	unsigned int pars=*(unsigned int*)ln; ln+=4;

			//	msl_value *value=&DoRunGetVal(val, tv0, data, stack);

			//	for(int i=0; i<pars; i++){
			//		msl_value &d=DoRunGetVal(*(unsigned int*)(ln), tv1, data, stack);
			//		value=value->SGet(d.val);
			//		ln+=sizeof(int);
			//	}

			//	DoRunSetPVal(res, value, data, stack);					

			//break; }

			case MCCA_FUNC:{
				msl_five_functions_d *f = (msl_five_functions_d*)*(unsigned long*)(ln); ln += sizeof(void*);
				unsigned int pars = *(unsigned int*)(ln); ln += sizeof(int);

				msl_value vals[S1K];
				msl_value *pvals[S1K];
				for(int i=0; i<S1K; i++){ pvals[i]=&vals[i]; }

				msl_value &ret = DoRunGetVal(*(unsigned int*)ln, tv0, data, stack);
				ln += sizeof(void*);

				for(int i = 0; i < pars; i++){
					pvals[i] = &DoRunGetVal(*(unsigned int*)ln, vals[i], data, stack);
					ln += sizeof(int);
				}

				msl_five_func_call call;
				call.name = f->name;
				call.vals = pvals;
				call.valsz = pars;
				call.ret = &ret;
				call.out_text = &out_text;

				int r = (*f->func)(call);
					
				//msl_ce_efunc ef(f->name, output);

				//if(pars*sizeof(msl_value)<sizeof(msl_ce_efunc_s8)){
				//	msl_ce_efunc_s8 &s=*(msl_ce_efunc_s8*)pvals;
				//	ret=(*f->func)(ef, s);
				//}
				//else if(pars*sizeof(msl_value)<sizeof(msl_ce_efunc_s16)){
				//	msl_ce_efunc_s16 &s=*(msl_ce_efunc_s16*)pvals;
				//	ret=(*f->func)(ef, s);
				//} 
				//else if(pars*sizeof(msl_value)<sizeof(msl_ce_efunc_s32)){
				//	msl_ce_efunc_s32 &s=*(msl_ce_efunc_s32*)pvals;
				//	ret=(*f->func)(ef, s);
				//}
				//else if(pars*sizeof(msl_value)<sizeof(msl_ce_efunc_s64)){
				//	msl_ce_efunc_s64 &s=*(msl_ce_efunc_s64*)pvals;
				//	ret=(*f->func)(ef, s);
				//}
				//	else if(pars*sizeof(msl_value)<sizeof(msl_ce_efunc_s1k)){
				//	msl_ce_efunc_s1k &s=*(msl_ce_efunc_s1k*)pvals;
				//	ret=(*f->func)(ef, s);
				//}

				//if(pars==0) 
				//msl_value ret; unsigned int retp=*(unsigned int*)ln; //  =DoRunGetSVal(*(unsigned int*)ln, data, stack); 
				//msl_value &ret=DoRunGetSVal(*(unsigned int*)ln, data, stack);
					
				//if(pars==1) ret=(*f)(msl_ce_efunc(output), DoRunGetVal(*(unsigned int*)ln, data, stack));
				//if(pars==2) ret=(*f)(msl_ce_efunc(output), DoRunGetVal(*(unsigned int*)ln, data, stack), DoRunGetVal(*(unsigned int*)(ln+4), data, stack));
					
				//*retv=ret;
				//DoRunSetVal(retp, ret, stack);
					
					
			break;}

			case MCCA_JMP:
				rln += *(int*)(ln) + 8;
				csize = 0;
			break;

			case MCCA_AJMP:
				rln = code.data + *(unsigned int*)(ln);
				csize = 0;
			break;

			case MCCA_EJMP:
			case MCCA_NEJMP:
			{
				msl_value &l = DoRunGetVal(*(unsigned int*)(ln), tv0, data, stack);// ln += 4;
				if((l.val && l.val != "0") == (op->code == MCCA_EJMP)){
					rln += *(int*)(ln + 4) + 8;
					csize = 0;
				}
			break;}

			case MCCA_NOP: break;

			default:
				SetErrorEpic("DoRun operation not found");
				return 0;
			break;
		}

		return 1;
	}

	void ExecuteMathOp(int op, VString a, VString b, msl_value *ret){
		Itos it;
		double da = stod(a, a);
		double db = stod(b, b);
		double dr;

		switch(op){
			case MCCA_ADD: dr = da + db; break;
			case MCCA_CON: ret->val.Add(a, b); return; break;
			case MCCA_SUB: dr = da - db; break;
			case MCCA_MUL: dr = da * db; break;
			case MCCA_DIV: dr = da / db; break;
			case MCCA_MOD: dr = (int64)da % (int64)db; break;
			case MCCA_EQ: ret->val = a == b ? "1" : "0"; return; break;
			case MCCA_NEQ: ret->val = a == b ? "0" : "1"; return; break;
			case MCCA_BOR: ret->val = itos(a.toi() | b.toi()); return; break;
			case MCCA_BAND: ret->val = itos(a.toi() & b.toi()); return; break;
			case MCCA_BXOR: ret->val = itos(a.toi() ^ b.toi()); return; break;
		}


		ret->val = it.dtos(dr);
	}

	void ExecuteMathOp(int op, VString a, msl_value *ret){
		switch(op){
		case MCCA_IADD: ret->val = itos(a.toi() + 1); return; break;
		case MCCA_ISUB: ret->val = itos(a.toi() - 1); return; break;
		}
	}

		// Text or Stack Value or Pointeer Value -> msl_value&
	msl_value& DoRunGetVal(unsigned int l, msl_value &val, VString data, VString stack){
		switch(l&MCCTV_ALL){
			case MCCTV_STACK:
					return *(msl_value*)(stack.data+l);
				break;

			case MCCTV_PVAL:
				return *((msl_ce_getpointer*)(stack.data+(l&~MCCTV_ALL)))->val;
			break;

			case MCCTV_DATA:{
					msl_ce_code_datavalue *dv=(msl_ce_code_datavalue*)(data.data+(l&~MCCTV_DATA));
					val.val.set(VString(dv->data, dv->sz));
					return val;
			break;}

			default:
				SetErrorEpic("DoRunGetVal() unknown type");
			break;
		}
		return msl_value();
	}

	// Asm
	int AsmOne(unsigned char *ln, int csize, _msl_five_asm_data *op){
		HLString0 &ls = out_asm;
		
		ls + "[" + ( ln - code.data ) + "]  ";

		ln += 4;

		switch(op->code){
		case MSLA_COMMENT:{
			VString key = VString(ln + 4, *(unsigned int*)(ln ));
			VString val = VString(ln + 8 + key.sz, *(unsigned int*)(ln + 4 + key.sz));
			
			if(12 + key.sz + val.sz != csize)
				return SetErrorAsm("COMMENT bad size");
			else
				ls + "comment: " + key + "=" + val;
		break; }

		case MSLA_COMMENT_LINE:
			ls + *(unsigned int*)(ln) + ">";
		break;

		case MCCA_NOP:
			ls + "nop";
		break;

		case MCCA_ADD: case MCCA_SUB: case MCCA_MUL: case MCCA_DIV: case MCCA_MOD: case MCCA_CON:
		case MCCA_BOR: case MCCA_BAND: case MCCA_BXOR: 
			ls + getlisting_val(*(unsigned int*)ln) + " = " + getlisting_val(*(unsigned int*)(ln+4)) + " " + GetOpText(op->code) + " " + getlisting_val(*(unsigned int*)(ln+8)) + "; "; // + el->str;
		break;

		case MCCA_MOV:
			ls+getlisting_val(*(unsigned int*)ln) + " = " + getlisting_val(*(unsigned int*)(ln+4)) + "; "; // + el->str;
		break;

		case MCCA_COPY:
			ls+getlisting_val(*(unsigned int*)ln) + " = " + getlisting_val(*(unsigned int*)(ln+4)) + " [copy]; "; // + el->str;
		break;

		case MCCA_FUNC:{
			msl_five_functions_d *f = (msl_five_functions_d*)*(unsigned long*)(ln); ln += sizeof(void*);
			unsigned int pars = *(unsigned int*)(ln); ln += sizeof(int);

			ls + getlisting_val(*(unsigned int*)ln) + " = " + f->name + "("; ln += sizeof(int);
			for(int i = 0; i < pars; i++){
				if(i)
					ls+", ";

				ls + getlisting_val(*(unsigned int*)ln);
				ln += sizeof(int);
			}

			ls + "); ";
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
			ls + "jmp " + *(int*)(ln) + ";";
		break;

		case MCCA_AJMP:
			ls + "ajmp " + *(unsigned int*)(ln) + ";";
		break;

		case MCCA_EJMP:
			ls+"jmp ["+getlisting_val(*(unsigned int*)ln) + " == 1] " + itos(*(int*)(ln+4)) + ";";
		break;

		case MCCA_NEJMP:
			ls+"jmp ["+getlisting_val(*(unsigned int*)ln) + " == 0] " + itos(*(int*)(ln+4)) + ";";
		break;

		case MCCA_UADD:
			ls+" + "+getlisting_val(*(unsigned int*)ln) + ";";
		break;

		case MCCA_USUB:
			ls+" - "+getlisting_val(*(unsigned int*)ln) + ";";
		break;

		case MCCA_UNO:
			ls+" ! "+getlisting_val(*(unsigned int*)ln) + ";";
		break;

		case MCCA_UNOT:
			ls+" ~ "+getlisting_val(*(unsigned int*)ln) + ";";
		break;

		case MCCA_IADD:
			ls+" ++ "+getlisting_val(*(unsigned int*)ln) + ";";
		break;

		case MCCA_ISUB:
			ls+" -- "+getlisting_val(*(unsigned int*)ln) + ";";
		break;

		case MCCA_EQ:
			ls+getlisting_val(*(unsigned int*)ln) + " == " + getlisting_val(*(unsigned int*)(ln+4)) + ";";
		break;

		case MCCA_NEQ:
			ls+getlisting_val(*(unsigned int*)ln) + " != " + getlisting_val(*(unsigned int*)(ln+4)) + ";";
		break;

		default:
			//ls+el->str;
			return 0;
		break;

		}

		ls + "\r\n";

		return 1;
	}

	MString getlisting_val(unsigned int pos){
		switch(pos&MCCTV_ALL){
			case MCCTV_STACK:{
				msl_ce_values_d *val=values->GetByPos(pos);
				if(val)
					return MString()+"$"+val->key;
				else
					return MString()+"$"+itos(pos);
			break; }

			case MCCTV_PVAL:
				return MString()+"pval "+itos(pos&~MCCTV_ALL);
			break;

			case MCCTV_DATA:{
					msl_ce_code_datavalue *dv=(msl_ce_code_datavalue*)(data.data+(pos&~MCCTV_DATA));
					return MString()+"'"+(VString(dv->data, dv->sz))+"'";
			break;}

			default:
				return "unknown type";
			break;
		}
		return "???";
	}

	// Get Op
	_msl_five_asm_data* GetOp(unsigned int op){
		for(int i = 0; i < sizeof(msl_five_asm_data) / sizeof(_msl_five_asm_data); i++){
			if(msl_five_asm_data[i].code == op)
				return &msl_five_asm_data[i];
		}
		return 0;
	}

	VString GetOpText(int op){
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
		case MCCA_BOR:
			return "|";
		case MCCA_BAND:
			return "&";
		case MCCA_BXOR:
			return "^";
		}

		return "???";
	}

	// Next Code
	unsigned char* NextCode(unsigned char *ln, int &csize, _msl_five_asm_data *&op){
		if(!ln)
			ln = code;
		else
			ln += csize;

		if(ln + 4 < code.endu()){
			unsigned int id = *(unsigned int*)ln;
			op = GetOp(id);

			if(!op)
				return 0;
			
			if(op->sz == -1){

				if(id == MCCA_FUNC){
					unsigned int pars = *(unsigned int*)(ln + 4 + sizeof(void*));
					csize = 4 + sizeof(void*) + sizeof(int) + sizeof(int) + sizeof(int) * pars;
				}

				if(id == MSLA_COMMENT){
					int t = *(unsigned int*)(ln + 4);
					t += *(unsigned int*)(ln + 8 + t);
					csize = 12 + t;
				}
				
			} else 
				csize = 4 + op->sz;

			if(ln + csize <= code.endu())
				return ln;
		}

		return 0;
	}

	// Error
	int SetErrorEpic(VString line){
		out_text + "Epic Error: " + line + "\r\n";
		return 0;
	}

	int SetErrorAsm(VString line){
		out_text + "ASM Error: " + line + "\r\n";
		return 0;
	}

	// Output
	VString GetOutput(){
		return VString(out_text.OneLine(), out_text.Size());
	}

	VString GetOutputAsm(){
		return VString(out_asm.OneLine(), out_asm.Size());
	}

	MString GetOutputVal(){
		if(!values)
			return MString();

		HLString ls;
		int f = 1;
		msl_ce_values_d *el = values->_a;

		while(el){
			if(f){
				//ls + "msl_value ";
				f = 0;
			}// else
			//	ls + ", ";
			ls + "$" + el->key + " = " + ((msl_value*)(stack.data + el->pos))->val + "\r\n";
			el = el->_n;
		}

		//if(!f)
		//	ls + ";";

		return ls;
	}

	MString GetOutputStackVal(){
		HLString ls;

		msl_value *v = (msl_value*)stack.data, *tv = (msl_value*)stack.endu();
		int pos = 0;

		while(v < tv){
			if(v->val){
				ls + "$" + pos +" = '" + v->val + "'" + "\r\n";
			}

			v ++;
			pos ++;
		}

		return ls;
	}

};