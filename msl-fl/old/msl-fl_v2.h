// My Server Language - Fast Line
Versions msl_fl_version[]={
	"0.0.0.2", "14.10.2013 1:08",
	"0.0.0.1", "13.10.2013 21:05"
};


// Function Arguments
class msl_fl_farg{
public:
	VString val;
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
	int Add(VString val){
		if(usz>=asz){ if(!UpSize()) return 0; }
		// add
		args[usz].val=val; usz++;
		return 1;
	}

	int Sz(){ return usz; }
	
};

// spaces
// Do() - all process operations
// Set () - set
// Get () - get

//#define MSL_DOOPT_ERROR 0
//#define MSL_DOOPT_STOPIT (MSL_DOOPT_ERROR)

class msl_fl{
	HLString output; // result

	int do_opt, do_opt_stopit;

	public:

	// init
	msl_fl(){ do_opt=0; do_opt_stopit=0; }

	~msl_fl(){	}
	
	// process
	void Do(VString code){
		unsigned char *line=code, *pline=line, *to=code.endu();
		// stop it
		do_opt_stopit=0;

		while(line<to && !do_opt_stopit){
			if(*line=='<' && line+1<to && *(line+1)=='?'){
				// set result
				SetOutput(VString(pline, line-pline));
				// *line='<?...' skip this 2 sumbols
				line+=2;
				// do msl
				DoCode(line, to);
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

	void DoCode(unsigned char*&line, unsigned char *to){

		while(line<to && !do_opt_stopit){

			if(*line=='$'){
				// value
			}else if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_'){
				// function
				DoCodeFunction(line, to);
			} else if(*line==' ' || *line=='\t' || *line=='\r' || *line=='\n'){
				// ignore
			}else if(*line=='?' && line+1<to && *(line+1)=='>'){
				// '?>' end do code
				return ;
			}else{
				// what the sumbol?
				SetError(HLString()+"Unknown sumbol: '"+VString(line, 1)+"'.");
				return ;
			}

			line++;
		}

		return ;
	}

	void DoCodeFunction(unsigned char*&line, unsigned char *to){
		unsigned char *pline=line;
		VString name; msl_fl_fargs args;

		while(line<to){
			// normal name
			if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_'){}
			else{
				name.setu(pline, line-pline);
				while(line<to){
					if(*line=='('){
						line++;
						if(!DoCodeFunctionArgs(line, to, args)) return ;
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

	int DoCodeFunctionArgs(unsigned char*&line, unsigned char *to, msl_fl_fargs &args){
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
	}

	int DoCodeFunctionExec(VString name, msl_fl_fargs &args){
		if(name=="print" || name=="echo"){
			for(int i=0; i<args.Sz(); i++){
				print(args[i].val);
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