// My Server Language - Fast Line
Versions msl_fl_version[]={
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
// msl_value. ћеханизм балансировки возвращаемых значений.  „тобы не копировать лишний раз структуру с данными.


// spaces
// Do() - all process operations
// Set () - set
// Get () - get

//#define MSL_DOOPT_ERROR 0
//#define MSL_DOOPT_STOPIT (MSL_DOOPT_ERROR)

class msl_fl{
	HLString output; // result

	int do_opt, do_opt_stopit;

	msl_value global;

	public:

	// init
	msl_fl(){ do_opt=0; do_opt_stopit=0; }

	~msl_fl(){	}
	
	// process
	void Do(VString code){
		unsigned char *line=code, *pline=line, *to=code.endu();
		// stop it
		do_opt_stopit=0;
		// value;
		msl_value outval;

		while(line<to && !do_opt_stopit){
			if(*line=='<' && line+1<to && *(line+1)=='?'){
				// set result
				SetOutput(VString(pline, line-pline));
				// *line='<?...' skip this 2 sumbols
				line+=2;

				while(!do_opt_stopit){
					// do msl
					DoCode(line, to, outval, ';');
					if(line>=to || *line!=';') break;
					line++;
				}

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
		// set result
		SetOutput(VString(pline, line-pline));
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
		msl_value *val=global.SGet(VString(pline, line-pline));

		// skip space
		while(1){
			while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			if(line<to && *line=='['){
				msl_value dval;
				DoCode(++line, to, dval, ']');

				// next []
				val=val->SGet(dval.val);
				line++;
			}
			else return val;
		}

		return 0;
	}

	void DoCodeFunction(unsigned char*&line, unsigned char *to){
		VString name; unsigned char *pline=line; msl_fl_fargs args;

		while(line<to){
			// normal name
			if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_'){}
			else{
				name.setu(pline, line-pline);
				while(line<to){
					if(*line=='('){
						line++;
						
						while(!do_opt_stopit){
							msl_value val;
							DoCode(line, to, val, ',', ')');
							//if(!DoCodeFunctionArgs(line, to, args)) return ;
							args.Add(val);
							if(line>=to){ SetError("not found ')'. EOF"); return ; }
							if(*line!=',') break;
							line++;
						}
						line++;

						if(!DoCodeFunctionExec(name, args)) return ;
						line++;
						return ;
					}
					else if(!(*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')){
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

/*	int DoCodeFunctionArgs_DEL(unsigned char*&line, unsigned char *to, msl_fl_fargs &args){
		unsigned char *pline;
		// skip space 
		while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

		pline=line;

		if(*line=='\''){
			line++;
			while(line<to && *line!='\'') line++;
			if(!DoCodeFunctionAddArg(pline+1, line, to, args)) return 0;
			line++;
		}else	
		if(*line=='"'){
			line++;
			while(line<to && *line!='"') line++;
			if(!DoCodeFunctionAddArg(pline+1, line, to, args)) return 0;
			line++;
		} else
		if(*line=='$'){
			while(line<to && (*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_')) line++;
			if(!DoCodeFunctionAddArg(pline, line, to, args)) return 0;
		}
		else{
			if(line>=to){
				SetError("EOF");
				return 0;
			}else{
				SetError(HLString()+"Unknown data: '"+VString(line, 1)+"'.");
				return 0;
			}
		}

		// skip space 
		while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

		// Lol recursion :)
		if(line<to && *line==','){
			line++; 
			return DoCodeFunctionArgs(line, to, args);
		}
		
		// if line=')' ok
		if(line>=to || *line!=')'){
			SetError(HLString()+"')' not foud.");
			return 0;
		}

		// line=')', skip
		line++;

		return 1;
	}

	int DoCodeFunctionAddArg(unsigned char *pline, unsigned char *line, unsigned char *to, msl_fl_fargs &args){
		if(line>=to){
			SetError(HLString()+"end "+VString(pline, 1)+" not found");
			return 0;
		} else{
			if(!args.Add(VString(pline, line-pline))){
				SetEpic("add value");
				return 0;
			}
		}
		return 1;
	}*/

	int DoCodeFunctionExec(VString name, msl_fl_fargs &args){
		if(name=="print" || name=="echo"){
			for(int i=0; i<args.Sz(); i++){
				print(args[i].val.val);
			}
			return 1;
		}

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