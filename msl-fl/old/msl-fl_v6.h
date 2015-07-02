// My Server Language - Fast Line
Versions msl_fl_version[]={
	"0.0.0.6", "17.10.2013 23:13",
	"0.0.0.5", "16.10.2013 05:06",
	"0.0.0.4", "15.10.2013 17:24",
	"0.0.0.3", "14.10.2013 16:22",
	"0.0.0.2", "14.10.2013 01:08",
	"0.0.0.1", "13.10.2013 21:05"	
};



// Values
class msl_value : public OMatrixT<msl_value>{
public:
	// prev, next, up first, up end
	msl_value *_p, *_n;//, *_a, *_e;
	MString key, val;

	msl_value(){ _p=0; _n=0; }

	msl_value* New(){
		msl_value *p=new msl_value;
		if(!p) return 0;

		OMAdd(p);
		return p;
	}

	msl_value* Find(VString key){
		if(!this) return 0;
		
		for(msl_value*p=_a; p; p=p->_n){
			if(p->key==key) return p;
		}

		return 0;
	}

	msl_value* Set(VString key, VString val){
		msl_value *p=Find(key);
		if(p){
			p->val=val;
		} else{
			p=New();
			if(p){
				p->key=key; p->val=val;
			}
		}
		return p;
	}

	VString Get(VString key){
		msl_value *p=Find(key);
		if(p)
			return p->val;
		else
			return VString();
	}

	msl_value* SGet(VString key){
		msl_value *p=Find(key);
		if(!p){
			p=New();
			if(p){
				p->key=key; p->val.sz=0xffffffff;
			}
		}
		return p;
	}

	void Del(VString key){
		msl_value *p=Find(key);
		if(p){
			OMDel(p); delete p;
		}
		return ;
	}

	void Copy(msl_value *val){
		Clear();
		this->val=val->val;
		
		for(msl_value*p=val->_a; p; p=p->_n){
			Set(p->key, p->val)->Copy(p);			
		}
		return ;
	}

	void Move(msl_value &val){
		Clear();
		_a=val._a; _e=val._e; val._a=0; val._e=0;
		this->val-=val.val;		
		return ;
	}

	~msl_value(){ Clear(); }
	void Clear(){ OMClear(); }

};

// Function Arguments
class msl_fl_farg{
public:
	msl_value val;
	//VString val;
};

class msl_fl_fargs{
	MString _args; // memory buffer
	msl_fl_farg *args; // 
	int asz, usz; // all sz & use sz

	int UpSize(){
		// reserv memory
		_args.Reserv(sizeof(msl_fl_farg)*(asz+16));
		// if error
		if(!_args){
			asz=0; usz=0;
			return 0;
		}
		// update
		asz+=16;
		args=(msl_fl_farg*)_args.data;

		return 1;
	}

public:
	// constructor & destructor
	msl_fl_fargs(){ args=0; asz=0; usz=0; }
	~msl_fl_fargs(){ }

	// vals[id];
	msl_fl_farg &operator[](const int i){
		if(i>usz){ globalerror("msl_fl_fargs epic fail"); }
		return args[i]; 
	}

	// add value
	msl_fl_farg* Add(msl_value &val){
		if(usz>=asz){ if(!UpSize()) return 0; }
		// add
		args[usz].val.Move(val);
		return &args[usz++];
	}

	int Sz(){ return usz; }
	
};

//#include "omatrix-msl_value.h"

// Hints:
// msl_value. Механизм балансировки возвращаемых значений.  Чтобы не копировать лишний раз структуру с данными.


// spaces
// Do() - all process operations
// Set () - set
// Get () - get

//#define MSL_DOOPT_ERROR 0
//#define MSL_DOOPT_STOPIT (MSL_DOOPT_ERROR)
#define MSLCBR_NULL	0
#define MSLCBR_CONT	1
#define MSLCBR_BREAK 2
#define MSLCBR_RET	3

class msl_fl{
	// result
	HLString output;

	// options
	int do_opt, do_opt_stopit, do_opt_ifw, do_opt_cbr, do_opt_active;

	// values
	msl_value global;
	msl_value _nullvalue;


	public:

	// init
	msl_fl(){ do_opt=0; do_opt_stopit=0; do_opt_cbr=0; }
	~msl_fl(){	}
	
	// process
	void Do(VString code){
		unsigned char *line=code, *pline=line, *to=code.endu();
		// stop it & active
		do_opt_stopit=0; do_opt_active=1; do_opt_ifw=0; do_opt_cbr=0;
		// value;
		msl_value outval;

		while(line<to && !do_opt_stopit){
			if(*line=='<' && line+1<to && *(line+1)=='?'){
				// set result
				SetOutput(VString(pline, line-pline));
				// *line='<?...' skip this 2 sumbols
				line+=2;

				//while(!do_opt_stopit){
					// do msl
				//	DoCode(line, to, outval, ';');
				//	if(line>=to || *line!=';') break;
				//	line++;
				//}
				DoCodeMulti(line, to, outval);

				// if(line='?>')
				if(line+1>=to || *line!='?' || *(line+1)!='>'){
					// oh no
					SetError("No find '?>'");
					// exit
					return ;
				} else{
					// *line='?>...' skip this 2 sumbols
					line+=2; pline=line;
				}
			}
			line++;
		}

		if(do_opt_cbr){
			if(do_opt_cbr==MSLCBR_CONT) SetError("for/while for continue; not found");
			if(do_opt_cbr==MSLCBR_BREAK) SetError("for/while for break; not found");
			if(do_opt_cbr==MSLCBR_RET) do_opt_cbr=MSLCBR_NULL;
		}

		// set result
		SetOutput(VString(pline, line-pline));
		return ;
	}

	// do code; code; code
	void DoCodeMulti(unsigned char*&line, unsigned char *to, msl_value &outval, unsigned char ecode=1){
		while(!do_opt_stopit){
			// do msl
			DoCode(line, to, outval, ';', ecode);
			if(line>=to || *line!=';') break;
			line++;
		}
		return ;
	}

	void DoCode(unsigned char*&line, unsigned char *to, msl_value &outval, unsigned char ecode=1, unsigned char ecodet=1){
		msl_value *value=0;
		unsigned char *pline=0;


		while(line<to && !do_opt_stopit){

			// skip space 
			while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

			// $value
			if(*line=='$'){
				if(value){ SetError("double $value"); return ; }
				value=DoCodeValue(++line, to);
				//outval.val=value->val;
				continue;
			}else
			// function
			if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_'){
				
				DoCodeFunction(line, to);
				continue;
			}
			// ecode
			else if(*line==ecode || *line==ecodet){
				if(value) outval.val=value->val;
				return ;
			}
			// string
			else if(*line=='"' || *line=='\''){
				pline=++line; 
				if(*line=='"')
					while(line<to && *line!='"') line++;
				else
					while(line<to && *line!='\'') line++;
				if(line>=to){ SetError("closed \" or ' not found"); }
				outval.val=VString(pline, line-pline);
				line++;
				continue;
			}
			// numbers
			else if(*line>='0' && *line<='9'){
				pline=line; 
				while(line<to && *line>='0' && *line<='9') line++;
				outval.val=VString(pline, line-pline);
				continue;				
			}
			// operator=
			else if(*line=='='){
				if(!value){ SetError("need $value="); return ; }
				
				msl_value val;
				DoCode(++line, to, val, ecode, ecodet);
				
				// move
				value->Clear();
				value->val=val.val;
				value->_a=val._a; value->_e=val._e; val._a=0; val._e=0;
				// set outval
				outval.Copy(value);
				continue;			
			}
			// operator--
			else if(*line=='-' && line+1<to && *(line+1)=='-'){
				if(!value){ SetError("need $value--"); return ; }
				// $val--
				value->val=itos(value->val.toi()-1);
				// set outval
				outval.Copy(value);
				line+=2;
				continue;
			}
			// '?>' end do code
			else if(*line=='?' && line+1<to && *(line+1)=='>'){
				return ;
			}
			// what the sumbol?
			else{				
				SetError(HLString()+"Unknown sumbol: '"+VString(line, 1)+"'.");
				return ;
			}

			// skip space
			while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			line++;
		}

		return ;
	}

	msl_value* DoCodeValue(unsigned char*&line, unsigned char *to){
		unsigned char *pline=line;

		while(line<to && *line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_') line++;
		if(line>=to){
			SetError(HLString()+"EOF.");
			return 0;
		}
		
		// Get existing or create new
		msl_value *val;
		
		if(do_opt_active)
			val=global.SGet(VString(pline, line-pline));
		else val=&_nullvalue;

		// skip space
		while(1){
			while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			if(line<to && *line=='['){
				msl_value dval;
				DoCode(++line, to, dval, ']');

				// next []
				if(do_opt_active) val=val->SGet(dval.val); else val=&_nullvalue;
				line++;
			}
			else return val;
		}

		return 0;
	}

	void DoCodeFunction(unsigned char*&line, unsigned char *to){
		VString name; unsigned char *pline=line; msl_fl_fargs args;
		unsigned char *code, *ecode;

		while(line<to){
			// normal name
			if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_'){}
			else{
				name.setu(pline, line-pline);
				while(line<to){
					if(*line=='('){
						line++;
						code=line;

						if(name=="for"){
							 DoCodeFunctionFor(line, to); return ;
						}

						if(!DoCodeFunctionArgs(line, to, args)) return ;
						ecode=line; line++;
						// Exec function
						if(!DoCodeFunctionExec(name, args)) return ;
						//line++;

						// if, while, for functions
						if(do_opt_ifw){
							if(name=="if")
								DoCodeFunctionIf(line, to);
							else if(name=="while")
								DoCodeFunctionWhile(line, to, code, ecode);
							//else if(name=="for")
								//DoCodeFunctionFor(line, to);
						}

						return ;
					}
					else if(!(*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')){
						if(*line==';'){
							if(name=="continue"){ do_opt_cbr=MSLCBR_CONT; do_opt_active=0; return ; }
							if(name=="break"){ do_opt_cbr=MSLCBR_BREAK; do_opt_active=0;  return ; }
							if(name=="return"){ do_opt_cbr=MSLCBR_RET; do_opt_active=0;  return ; }
						}

						SetError(HLString()+"function '"+name+"' open '(' not found.");
						return ;
					}

					line++;
				}
			}
			line++;
		}

		// line='functionname'EOF
		SetError(HLString()+"end of function name: '"+VString(pline, line-pline)+"'");
		return ;
	}

	int DoCodeFunctionArgs(unsigned char *&line, unsigned char *to, msl_fl_fargs &args){
		while(!do_opt_stopit){
			msl_value val;
			DoCode(line, to, val, ',', ')');
			//if(!DoCodeFunctionArgs(line, to, args)) return ;
			args.Add(val);
			if(line>=to){ SetError("not found ')'. EOF"); return 0; }
			if(*line!=',') break;
			line++;
		}
		return 1;
	}

	void DoCodeFunctionIf(unsigned char*&line, unsigned char *to){
		msl_value val; unsigned char endcode;
		// save values
		int old_ifw=do_opt_ifw, old_active=do_opt_active; do_opt_ifw=0;

		// skip space
		while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
		// single or {multi}
		if(line>=to){ SetError("if(...) EOF"); return ; }
		
		// set active
		do_opt_active=old_active && old_ifw==2;
		// do if(){ code }
		if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
		DoCodeMulti(line, to, val, endcode); line++;

		// skip space
		while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
		// test on 'else'
		if(line+4<to && *line=='e' && *(line+1)=='l' && *(line+2)=='s' && *(line+3)=='e'){
			// skip 'else'
			line+=4;
			// skip space
			while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			// set active
			do_opt_active=old_active && old_ifw==1;
			// do else{ code }
			if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
			DoCodeMulti(line, to, val, endcode); line++;
		}

		// load old value
		do_opt_active=old_active; do_opt_ifw=0;
		return ;
	}

	void DoCodeFunctionWhile(unsigned char *&line, unsigned char *to, unsigned char *code, unsigned char *ecode){
		msl_value val; unsigned char *lline, *elline, *tline; unsigned char endcode;
		
		// save values
		int old_ifw=do_opt_ifw, old_active=do_opt_active; do_opt_ifw=0;
		// skip space
		while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
		if(line>=to){ SetError("if(...) EOF"); return ; }

		// set active
		do_opt_active=old_active && old_ifw==2; lline=line;
		// do while(){ code }
		if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
		DoCodeMulti(line, to, val, endcode); line++;
		elline=line;

		// while
		while(old_active && old_ifw==2){
			msl_value val;
			// do while( it code )
			tline=code;
			DoCode(tline, ecode+1, val, ')');
			
			// cbr
			if(do_opt_cbr){
				if(do_opt_cbr==MSLCBR_CONT){ do_opt_cbr=MSLCBR_NULL; }
				if(do_opt_cbr==MSLCBR_BREAK){ do_opt_cbr=MSLCBR_NULL; break; }
				if(do_opt_cbr==MSLCBR_RET){ break; }
			}
			
			// result
			if(!val.val || val.val=="0") break;

			// do while(1){ it code }
			tline=lline;
			if(*(tline)=='{'){ endcode='}'; tline++; } else endcode=';';
			DoCodeMulti(tline, elline, val, endcode);

			// set active
			do_opt_active=old_active && old_ifw==2;
		}

		// load old value
		do_opt_active=old_active; do_opt_ifw=0;
		return ;
	}

	int DoCodeFunctionFor(unsigned char *&line, unsigned char *to){
		// save values
		int old_active=do_opt_active; do_opt_active=0;
		// for args
		VString a[4]; int as=0;

		while(!do_opt_stopit && as<4){
			msl_value val;
			a[as].data=line;
			DoCode(line, to, val, ';', ')');
			a[as].sz=line-a[as].data; as++;

			if(line>=to){ SetError("not found ')'. EOF"); return 0; }
			if(*line==')') break;
			line++;
		}
		line++;

		if(as!=3){
			SetError("for(args!=3)."); return 0;
		}

		// Do {}
		msl_value val; unsigned char *lline, *elline, *tline, *eline; unsigned char endcode;

		lline=line;
		if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
		DoCodeMulti(line, to, val, endcode); line++;
		elline=line;

		// load old value
		do_opt_active=old_active;

		// Go or Nooo...
		if(old_active){
			// do for(1)
			tline=a[0].data; eline=a[0].endu()+1;
			DoCode(tline, eline, val, ';');

			while(1){
				// do for(2), test
				tline=a[1].data; eline=a[1].endu()+1;
				DoCode(tline, eline, val, ';');
				if(!val.val || val.val=="0") break;
			
				// do {}
				tline=lline; eline=elline;
				if(*(tline)=='{'){ endcode='}'; tline++; } else endcode=';';				
				DoCodeMulti(tline, eline, val, endcode);

				// cbr
				if(do_opt_cbr){
					if(do_opt_cbr==MSLCBR_CONT){ do_opt_cbr=MSLCBR_NULL; }
					if(do_opt_cbr==MSLCBR_BREAK){ do_opt_cbr=MSLCBR_NULL; break; }
					if(do_opt_cbr==MSLCBR_RET){ break; }
				}

				// set value
				do_opt_active=old_active;

				// do for(3)
				tline=a[2].data; eline=a[2].endu()+1;
				DoCode(tline, eline, val, ')');				
			}
		}

		// load old value
		do_opt_active=old_active; do_opt_ifw=0;
		return 1;
	}

	int DoCodeFunctionExec(VString name, msl_fl_fargs &args){
		// all sections

		if((name=="if" || name=="while") && args.Sz()==1){
			do_opt_ifw=args[0].val.val && args[0].val.val!="0"; do_opt_ifw++;
			return 1;
		}

		// if active
		if(!do_opt_active) return 1;

		// exec
		if(name=="print" || name=="echo"){
			for(int i=0; i<args.Sz(); i++){
				print(args[i].val.val);
			}
			return 1;
		}

		//
		SetError(HLString()+"Function: '"+name+"' not found");
		return 0;
	}

	// global value
	void SetValue(VString key, VString val){
	

	}

	// get output
	MString GetOutput(){
		// return result;
		return MString(output.oneline(), output.size());
	}

protected:
	// set output
	void SetOutput(VString line){
		// add line to result
		output+line;
		return ;
	}

	void SetWarning(VString line){
		// add error line to result
		output+"MSL-FL Warning: "+line+"\r\n";
		return ;
	}

	void SetError(VString line){
		// add error line to result
		output+"MSL-FL Error: "+line+"\r\n";
		// stop
		do_opt_stopit=1;
		return ;
	}

	void SetEpic(VString line){
		// add error line to result
		output+"MSL-FL Epic Fail: "+line+"\r\n";
		// stop
		do_opt_stopit=1;
		return ;
	}

};