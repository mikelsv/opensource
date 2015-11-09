// My Server Language - Fast Line
Versions msl_fl_version[]={
	"0.0.1.1", "12.06.2015 12:06",
	"0.0.1.0", "30.11.2013 15:12",
	"0.0.0.8", "19.10.2013 21:29",
	"0.0.0.7", "18.10.2013 21:48",
	"0.0.0.6", "17.10.2013 23:13",
	"0.0.0.5", "16.10.2013 05:06",
	"0.0.0.4", "15.10.2013 17:24",
	"0.0.0.3", "14.10.2013 16:22",
	"0.0.0.2", "14.10.2013 01:08",
	"0.0.0.1", "13.10.2013 21:05"	
};

#ifndef USEMSV_MLIST
#error Please #define USEMSV_MLIST
#endif

#ifndef USEMSV_ITOS
#error Please #define USEMSV_ITOS
#endif

TLock msl_value_test_loc;
int msl_value_test_count;

#include "../msl/msl-value-def.h";

#ifdef USEMSV_MSL_FASTVAL
#include "../msl/msl-val-fast.h"
#else
#define msl_fvalue msl_value
#endif

#ifdef USEMSV_MSL_TMPVAL
#include "../msl/msl-val-tmp.h"
#else
#define msl_tvalue msl_value
#endif

#define msl_value msl_value_template<msl_value_base>



// Function Arguments
class msl_fl_farg{
public:
	// in
	msl_value val, *ppval, *pval;
	// ($key=val)
	VString fkey, fval;
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
	~msl_fl_fargs(){ Clear(); }

	// vals[id];
	msl_fl_farg &operator[](const int i){
		if(i>usz){ globalerror("msl_fl_fargs epic fail"); }
		return args[i]; 
	}

	// add value
	msl_fl_farg* Add(msl_value &val, msl_value *pval, msl_value *ppval){
		if(usz>=asz){ if(!UpSize()) return 0; }
		// add
		args[usz].val.Move(val);
		args[usz].pval=pval;
		args[usz].ppval=ppval;
		return &args[usz++];
	}

	// add value
	msl_fl_farg* AddF(VString k, VString v){
		if(usz>=asz){ if(!UpSize()) return 0; }
		// add
		args[usz].fkey=k;
		args[usz].fval=v;
		args[usz].pval=0;
		return &args[usz++];
	}

	int Sz(){ return usz; }
	int MSz(){
		for(int i=0; i<usz; i++){
			if(args[i].fval.data) return i;
		}
		return usz;
	}

	void Clear(){
		for(int i=0; i<usz; i++){
			args[i].val.ClearFull();
		}

		args = 0;
		asz = 0;
		usz = 0;

		return ;
	}
	
};

class msl_function_d{
public:
	msl_function_d(){ use=0; }

	msl_function_d *_p, *_n;
	VString name;
	msl_fl_fargs args, globals;
	MString code;
	int use;

	void UseIt(int v=1){ use=v; }
};

class msl_function : public OMatrixT<msl_function_d>{

public:
	msl_function(){}
	~msl_function(){ Clear(); }

	msl_function_d* New(){
		msl_function_d*p=new msl_function_d;
		OMAdd(p);
		return p;
	}

	msl_function_d *Find(VString name, int args){
		for(msl_function_d *p=_a; p; p=p->_n){
			if(p->use && p->name==name && args>=p->args.MSz() && args<=p->args.Sz()) return p;
		}
		return 0;
	}

	void Del(msl_function_d *p){
		OMRealDel(p);
	}

	void Clear(){ OMClear(); }
};

class msl_globals_d{
public:
	msl_globals_d *_p, *_n;
	VString name;
	msl_value *glob;
	msl_value *used;
};

class msl_globals : public OMatrixT<msl_globals_d>{
public:
	msl_globals(){}
	~msl_globals(){ Clear(); }

	msl_globals_d* New(){
		msl_globals_d *p = new msl_globals_d;
		OMAdd(p);
		return p;
	}

	msl_globals_d *Find(msl_value *val){
		for(msl_globals_d *p = _a; p; p = p->_n){
			if(p->glob == val)
				return p;
		}
		return 0;
	}

	void Del(msl_globals_d *p){
		OMRealDel(p);
	}

	void Clear(){ OMClear(); }
};

//#include "omatrix-msl_value.h"

// Hints:
// msl_value. ћеханизм балансировки возвращаемых значений.  „тобы не копировать лишний раз структуру с данными.


//
class msl_fl_extfunc{
public:
	virtual int DoCodeFunctionExec(VString name, msl_fl_fargs &args, msl_value &val){
		// exec
		//if(name=="print" || name=="echo"){
		//	//for(int i=0; i<args.Sz(); i++){
		//		//SetOutput(args[i].val.val);
		//		//print(args[i].val.val);
		//	//}
		//	//return 1;
		//}

		if(name=="testextfunc"){
			val.val="ok";
			return 1;
		}

		return 0;
	}
};

class msl_cursor_d{
public:
	msl_value *val, *cur;
};

int msl_cursor_find(msl_cursor_d &d, msl_value *&val){
	return d.val==val;
}

class msl_cursor : public MList<msl_cursor_d>{

public:

	void reset(msl_value *val){
		if(!val) return ;
		UGLOCK(this);
		msl_cursor_d *d=Find((void*)msl_cursor_find, val);
		if(d) d->cur=0;
		return ;
	}

	msl_cursor_d* Get(msl_value* val){
		msl_cursor_d *d=Find((void*)msl_cursor_find, val);
		if(!d){ d=New(); d->val=val; d->cur=0; }
		if(d && !d->cur) d->cur=d->val->_a;
		return d;
	}

	msl_value* current(msl_value *val){
		if(!val) return 0;
		UGLOCK(this);
		msl_cursor_d *d=Get(val);
		if(d){
			if(val->GetV(d->cur)){
				return d->cur;
			}
		}
		return 0;
	}

	msl_value* prev(msl_value *val){
		if(!val) return 0;
		UGLOCK(this);
		msl_cursor_d *d=Get( val);
		if(d){
			if(val->GetV(d->cur)){
				if(!d->cur->_p) return 0;
				d->cur=d->cur->_p;
				return d->cur;
			}
		}
		return 0;
	}

	msl_value* next(msl_value *val){
		if(!val) return 0;
		UGLOCK(this);
		msl_cursor_d *d=Get(val);
		if(d){
			if(val->GetV(d->cur)){
				if(!d->cur->_n) return 0;
				d->cur=d->cur->_n;
				return d->cur;
			}
		}
		return 0;
	}

	msl_value* end(msl_value *val){
		if(!val) return 0;
		UGLOCK(this);
		msl_cursor_d *d=Get(val);
		if(d){ d->cur=val->_e; return d->cur;  }
		return 0;
	}
};

#ifdef USEMSV_MYSQL
class msl_extension_mysql_con{
public:
	int cid;
	MySQLCon con;
};

class msl_extension_mysql{
	OList<msl_extension_mysql_con> cons;
	int cids;

public:
	
	msl_extension_mysql(){
		cids = 100 + (rand() % 100000);
	}

	msl_extension_mysql_con* GetConnection(int cid){
		msl_extension_mysql_con *con = 0;

		while(con = cons.Next(con)){
			if(con->cid == cid)
				return con;
		}

		return 0;
	}

	int mysql_connect(VString host, VString user, VString pass, int port){
		msl_extension_mysql_con * con = cons.New();
		con->cid = cids;
		cids += 1 + (rand() % 5);
		if(con->con.Connect(host, user, pass, "", port ? port : 3306))
			return con->cid;

		cons.Del(con);
		return 0;
	}

	int mysql_select_db(int cid, VString db){
		msl_extension_mysql_con *con = GetConnection(cid);
		if(con)
			return con->con.SelectDB(db);
		return 0;		
	}

	int mysql_query(int cid, VString query){
		msl_extension_mysql_con *con = GetConnection(cid);
		if(con)
			return con->con.Query(query);
		return 0;
	}

	int mysql_fetch_array(int cid, msl_value &val){
		msl_extension_mysql_con *con = GetConnection(cid);
		if(con){
			if(con->con.GetRow()){
				for(int i = 0; i < con->con.GetNumFields(); i++){
					val.Add(con->con.GetFieldName(i), con->con.GetRowVal(i));
				}
			}
		}
		return 0;
	}

	MString mysql_error(int cid){
		msl_extension_mysql_con *con = GetConnection(cid);
		if(con)
			return con->con.GetError();
		return "connection id is fail";
	}
	
	~msl_extension_mysql(){
		// auto clean
	}

	virtual int DoCodeFunctionExec(VString name, msl_fl_fargs &args, msl_value &val){
		if(name == "mysql_connect" && (args.Sz() == 3 || args.Sz() == 4)){
			val.val = itos(mysql_connect(args[0].val.val, args[1].val.val, args[2].val.val, args.Sz() == 4 ? args[3].val.val.toi() : 0));
		}

		else if(name == "mysql_select_db" && args.Sz() == 2){
			val.val = mysql_select_db(args[1].val.val.toi(), args[0].val.val) ? "1" : "0";
		}

		else if(name == "mysql_query" && args.Sz() == 2){
			val.val = mysql_query(args[1].val.val.toi(), args[0].val.val) ? "1" : "0";
		}

		else if(name == "mysql_fetch_array" && args.Sz() == 1){
			mysql_fetch_array(args[0].val.val.toi(), val);
		}

		else if(name == "mysql_error" && args.Sz() == 1){
			val.val = mysql_error(args[0].val.val.toi());
		}

		else
			return 0;

		return 1;
	}


};

#endif

// msl mathematical operations
void msl_math_ops(int op, VString a, VString b, msl_value *ret){
	Itos it;
	double da = stod(a, a);
	double db = stod(b, b);
	double dr;

	switch(op){
		case '+': dr = da + db; break;
		case '-': dr = da - db; break;
		case '*': dr = da * db; break;
		case '/': dr = da / db; break;
		case '%': dr = (int64)da % (int64)db; break;
		default: dr = 0; break;
	}

	ret->val = it.dtos(dr);
}


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
#define MSLCBR_DIE	4

class msl_fl{
	// result
	HLString output;

	// options
	int do_opt, do_opt_stopit, do_opt_ifw, do_opt_cbr, do_opt_active;

	// positions line count, char *line, char this pos in line
	int do_line_count, do_line_count_ignore; unsigned char *do_line, *do_line_t;

	// values
	msl_value global_value;
	msl_value *local_value;
	msl_value null_value;

	// functions
	msl_function functions;
	// used flag
	msl_value functions_uf;
	//IList <msl_value*> functions_global;

	// globals
	msl_globals globals;

	msl_fl_extfunc* extfunc;

	// cursor reset(), prev(), current(), next(), end();
	msl_cursor cursor;
	msl_value *cursor_value, *cursor_value_p;

	// this path
	VString file_path;

	// extensions
#ifdef USEMSV_MYSQL
	msl_extension_mysql extension_mysql;
#endif

	HLString temp_data;

	public:

	// init
	msl_fl(){
		do_opt=0; do_opt_stopit=0; do_opt_cbr=0; local_value=&global_value; extfunc=0;
#ifdef MSL_FL_EXTFUNC_DEFAULT
		extfunc = MSL_FL_EXTFUNC_DEFAULT;
#endif	
	}

	~msl_fl(){	}
	
	void DoFile(VString file){
		file_path = file;

		if(IsFile(file))
			Do(LoadFile(file));
		else
			SetError(HLString() + "include('" + file + "') file not found");

		return ;
	}

	// process
	void Do(VString code){
		unsigned char *line=code, *pline=line, *to=code.endu();
		// stop it & active
		do_opt_stopit=0; do_opt_active=1; do_opt_ifw=0; do_opt_cbr=0; do_line_count=0; do_line_count_ignore = 0; do_line=line; do_line_t=line;
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

				if(line + 3 < to && *(line + 0) == 'm' && *(line + 1) == 's' && *(line + 2) == 'l')
					line += 3;

				DoCodeMulti(line, to, outval);

				// if(line='?>')
				if(line+1>=to || *line!='?' || *(line+1)!='>'){
					// oh no
					if(!do_opt_stopit)
						SetError("No find '?>'");
					// exit
					return ;
				} else{
					// *line='?>...' skip this 2 sumbols
					line+=2; pline=line+1;
				}
			}else if(*line=='\n'){ do_line_count++; do_line=line+1; }
			line++;
		}

		if(!DoTestCBR()) return ;


		// set result
		SetOutput(VString(pline, line-pline));
		return ;
	}

	// test continue, break, return
	int DoTestCBR(){
		if(do_opt_cbr){
			if(do_opt_cbr==MSLCBR_CONT){ SetError("for/while for continue; not found"); return 0; }
			if(do_opt_cbr==MSLCBR_BREAK){ SetError("for/while for break; not found"); return 0; }
			if(do_opt_cbr==MSLCBR_RET) do_opt_cbr = MSLCBR_NULL;
		}
		return 1;
	}

	// do code; code; code
	void DoCodeMulti(unsigned char*&line, unsigned char *to, msl_value &outval, unsigned char ecode = 1){
		msl_value val;
		int opt_ret = 0;

		while(!do_opt_stopit){
			// do msl
			DoCode(line, to, val, ';', ecode);
			if(do_opt_cbr == MSLCBR_RET && !opt_ret){
				outval.Move(val);
				opt_ret = 1;
			}

			if(line >= to || *line != ';' || ecode == ';')
				break;

			line++;
		}
		return ;
	}

	void DoCode(unsigned char*&line, unsigned char *to, msl_value &outval, unsigned char ecode=1, unsigned char ecodet=1){
		msl_value *value=0, *pdvalue=0, *dvalue=0, *pvalue; msl_value valueline;
		unsigned char *pline=0; int msl_do_inc=0; // 1 - ++, 2 - --
		int opt_ret = 0;

		while(line<to && !do_opt_stopit){

			// skip space 
			_skipspace(line, to);
			//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

			// $value
			if(*line=='$'){
				if(dvalue){ pdvalue->Del(dvalue); dvalue=0; }
				if(value){ SetError("double $value"); return ; }
				value=DoCodeValue(++line, to, pdvalue, dvalue, pvalue);
				if(!value) return ;
				
				if(msl_do_inc){
					value->val=itos(value->val.toi()+ (msl_do_inc==1 ? 1 : -1));	
					msl_do_inc=0;
				}

				valueline.Add("t", value);

				cursor_value=value;
				cursor_value_p=pvalue;

				//outval.val=value->val;
				continue;
			}else
			// function
			if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_'){
				msl_value val; int df=0;
				DoCodeFunction(line, to, val, df);
				if(!df){
					msl_value *v=valueline.New();
					v->Move(val); v->key="t";
				}
				else
					if(do_opt_cbr == MSLCBR_RET && !opt_ret){
					outval.Move(val);
					opt_ret = 1;
				}

				continue;
			}
			// ecode
			else if(*line==ecode || *line==ecodet){
				if(msl_do_inc){
					SetError("Increment found!");
					return ;
				}

				if(dvalue){
					pdvalue->Del(dvalue);
					if(cursor_value == dvalue)
						cursor_value = 0;
					dvalue=0;
				}

				if(!opt_ret)
					DoCodeOneValue(valueline, outval);
				//if(value) outval.val=value->val;
				return ;
			}
			// string
			else if(*line=='"' || *line=='\''){
				pline=++line; 
				if(*(line-1)=='"')
					while(line<to){
						if(*line=='"'){
							if(pline<line && *(line-1)=='\\'){
								valueline.Add("t", VString(pline, line-pline-1));
								valueline.Add("o", ".");
								pline=line+1; line++; continue;
							}
							break;
						}

						if(*line=='{' && line+1<to && *(line+1)=='$'){
							msl_value val;
							valueline.Add("t", VString(pline, line-pline));
							valueline.Add("o", ".");

							line++;
							DoCode(line, to, val, '}');							
							valueline.Add("t", val.val);

							valueline.Add("o", ".");
							pline=line+1;
						}
						line++;
					}
				else
					while(line<to && *line!='\'') line++;
				if(line>=to){ SetError("closed \" or ' not found"); return ; }
				//outval.val=VString(pline, line-pline);
				valueline.Add("t", VString(pline, line-pline));
				line++;
				continue;
			}
			// numbers
			else if(*line>='0' && *line<='9'){
				pline=line; 
				while(line<to && (*line>='0' && *line<='9' || *line == '.')) line++;
				//outval.val=VString(pline, line-pline);
				valueline.Add("t", VString(pline, line-pline));
				continue;
			}
			// comments
			else if(*line=='/' && line+1<to && ( *(line+1)=='/' || *(line+1)=='*')){
				line+=2;
				if(*(line-1)=='/'){
					while(line<to && ( *line!='\r' && *line!='\n')) line++;
				} else{
					while(line<to && ( *line!='/' || *(line-1)!='*')){
						if(*line=='\n') ++do_line_count;
						line++;
					}
					line++;
				}

				continue;
			}
			// EOF
			else if(*line == '<' && line + 6 < to && compare((char*)line, 6, "<<<EOF", 6)){
				line += 6;
				pline = line;

				while(line < to){
					if(*line == '\n' && line + 3 < to && *(line + 1) == 'E' && *(line + 2) =='O' && *(line + 3) =='F'){
						unsigned char *l = line + 3;

						if(*(line - 1) != '\r')
							line++;

						valueline.Add("t", VString(pline, line-pline));
						line = l;
						break;
					}

					else if(*line=='{' && line+1<to && *(line+1)=='$'){
							msl_value val;
							valueline.Add("t", VString(pline, line-pline));
							valueline.Add("o", ".");

							line++;
							DoCode(line, to, val, '}');							
							valueline.Add("t", val.val);

							valueline.Add("o", ".");
							pline=line+1;
					}

					line ++;
				}
			}
			// operators
			else if(*line=='+' || *line=='-' || *line=='*' || *line=='/' || *line=='.' || *line=='!' || *line=='<' || *line=='>' || *line=='=' || *line=='|' || *line=='&' || *line=='%'){
				pline=line++;
				while(*line=='+' || *line=='-' || *line=='*' || *line=='/' || *line=='.' || *line=='!' || *line=='<' || *line=='>' || *line=='=' || *line=='|' || *line=='&' || *line=='%') line++;
				if(line>=to){ SetError("EOF"); return ; }
				
				VString name(pline, line-pline);
				if(line-pline>1 && *(pline+1)=='-' && *pline=='='){ name.sz=1; line-=line-pline-1; }

				// =
				if(*(line-1)=='=' && !((*(pline)=='=' || *(pline)=='>' || *(pline)=='<' || *(pline)=='!' ) && line-pline==2)){
					msl_value val;
					DoCode(line, to, val, ecode, ecodet);
					
					if(!value || valueline._e->key!="t"){
						SetError("lvalue for = not set");
						return ;
					}
					
					if(name=="="){
						value->Move(val);
					}else if(name=="+="){
						msl_math_ops('+', value->val, val.val, value);
						//value->val = itos(value->val.toi() + val.val.toi());
					}else if(name=="-="){
						msl_math_ops('-', value->val, val.val, value);
						//value->val = itos(value->val.toi() - val.val.toi());
					}else if(name=="*="){
						msl_math_ops('*', value->val, val.val, value);
						//value->val = itos(value->val.toi() * val.val.toi());
					}else if(name=="/="){
						if(!val.val.toi()){
							SetWarning("Divide by zero");
							value->val="0";
						} else
							msl_math_ops('/', value->val, val.val, value);
							//value->val = itos(value->val.toi() / val.val.toi());
					}else if(name==".="){
						value->val.Add(value->val, val.val);
					}

					if(dvalue){ dvalue=0; }

					valueline._e->Copy(value);
					value=0;
					continue;
				}

				if(name=="++" || name=="--"){
					if(msl_do_inc){
						SetError("Increment found!");
						return ;
					}

					if(value){
						value->val=itos(value->val.toi()+ (name=="++" ? 1 : -1));
						valueline._e->val=value->val;

						value=0; dvalue=0;
						continue;
					}

					msl_do_inc = name=="++" ? 1 : 2;
					value=0;
					continue;
				}

				valueline.Add("o", name);
				value=0;
				
				//DoCodeOperation(VString(pline, line-pline), value, 
				continue;
			}
			// ()
			else if(*line=='('){
				msl_value val;
				DoCode(++line, to, val, ')');
				if(line>=to || *line!=')'){
					SetError("Close ')' not found");
					return ;
				} line++;
				valueline.Add("t", val.val);
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
			_skipspace(line, to);
			//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			line++;
		}

		if(!do_opt_stopit)
			SetError("EOF");
		return ;
	}

	void DoCodeInclude(VString file){
		// save
		int _do_line_count = do_line_count;
		VString old_file_path = file_path;

		ILink il;
		il.Link(file_path);
		il.SetFile(file);

		if(IsFile(il.GetPathFile())){
			// do
			Do(LoadFile(il.GetPathFile()));
		} else {
			SetError(HLString() + "include('" + il.GetPathFile() + "') file not found");
		}

		// load
		do_line_count = _do_line_count;
		file_path = old_file_path;
		return ;
	}

	void DoCodeOneValue(msl_value &line, msl_value &outval){
		int sz=line.Size();

		for(int i=0; i<8; i++){
			msl_value *val=line._a;
			for(val; val && sz!=1; val=val->_n){
				
				switch(i){
				case 0:
					if(val->key=="o" && val->val=="!"){
						if(!val->_n || val->_n->key!="t"){
							SetError("Operator ! no have value");
							return ;
						}
						val->key="t"; val->val= (val->_n->val && val->_n->val!="0" || val->_n->_a)==0 ? "1" : "0";
						line.Del(val->_n); sz--;
					}
					if(val->key=="o" && val->val=="-" && (!val->_p || val->_p->key!="t")){
						if(!val->_n || val->_n->key!="t"){
							SetError("Operator - no have value");
							return ;
						}
						val->key="t"; val->val.Add("-", val->_n->val);
						line.Del(val->_n); sz--;
					}
					break;
				case 1: case 2: case 3: case 4: case 5: case 6: case 7:
					if(val->key=="o"){
						if(!val->_p || val->_p->key!="t" || !val->_n || val->_n->key!="t"){
							SetError(HLString()+"Operator'"+val->val+"'no have value");
							return ;
						}

						// */%
						if(i==1 && (val->val=="*" || val->val=="/" || val->val=="%")){
							if(val->val=="/" && !val->_n->val.toi()){
								SetWarning("Divide by zero");
								val->val="0";
							}else{
								if(val->val=="*") msl_math_ops('*', val->_p->val, val->_n->val, val); //val->val= itos(val->_p->val.toi()*val->_n->val.toi());
								else if(val->val=="/") msl_math_ops('/', val->_p->val, val->_n->val, val); //val->val= itos(val->_p->val.toi()/val->_n->val.toi());
								else if(val->val=="%") msl_math_ops('%', val->_p->val, val->_n->val, val); //val->val= itos(val->_p->val.toi()%val->_n->val.toi());
							}
						} else
						// +-
						if(i==2 && (val->val=="+" || val->val=="-" || val->val==".")){
							if(val->val=="+") msl_math_ops('+', val->_p->val, val->_n->val, val); //val->val = itos(val->_p->val.toi()+val->_n->val.toi());
							else if(val->val=="-") msl_math_ops('-', val->_p->val, val->_n->val, val); //val->val= itos(val->_p->val.toi()-val->_n->val.toi());
							else if(val->val==".") val->val.Add(val->_p->val, val->_n->val);

							//val->val=itos("+" ? val->_p->val.toi()+val->_n->val.toi() : val->_p->val.toi()-val->_n->val.toi() );
						}else
						// >> <<
						if(i==3 && (val->val==">>" || val->val=="<<")){
							val->val=itos(">>" ? val->_p->val.toi()>>val->_n->val.toi() : val->_p->val.toi()<<val->_n->val.toi() );
						}else
						//< <= > >=
						if(i==4 && (val->val==">" || val->val=="<" || val->val==">=" || val->val=="<=")){
							if(val->val==">") val->val= itos(val->_p->val.toi()>val->_n->val.toi());
							else if(val->val=="<")val->val= itos(val->_p->val.toi()<val->_n->val.toi());
							else if(val->val==">=") val->val= itos(val->_p->val.toi()>=val->_n->val.toi());
							else if(val->val=="<=") val->val= itos(val->_p->val.toi()<=val->_n->val.toi());
						} else
						// == !=
						if(i==5 && (val->val=="==" || val->val=="!=")){
							val->val= (val->val=="==" ? val->_p->val==val->_n->val : val->_p->val!=val->_n->val) ? "1" : "0";
						}else
						// & ^ |
						if(i==6 && (val->val=="&" || val->val=="^" || val->val=="|")){
							if(val->val=="&") val->val= itos(val->_p->val.toi()&val->_n->val.toi());
							else if(val->val=="^") val->val= itos(val->_p->val.toi()^val->_n->val.toi());
							else if(val->val=="|") val->val= itos(val->_p->val.toi()|val->_n->val.toi());
						} else
						// && ||
						if(i==7 && (val->val=="&&" || val->val=="||")){
							int l=val->_p->val && val->_p->val!="0";
							int r=val->_n->val && val->_n->val!="0";
							val->val= (val->val=="&&" ? l && r : l || r) ? "1" : "0";
						}else{
							continue;
						}

						val->key="t"; //val->val= val->val==itos("+" ? val->_p->val.toi()+val->_n->val.toi() : val->_p->val.toi()-val->_n->val.toi() );
						line.Del(val->_n); line.Del(val->_p); val=line._a; sz-=2;
					}
				break;
				}

			}
		}
		
		if(sz>1 || sz && line._a->key!="t"){
			SetError("One value. Bad code");
			return ;
		}

		if(sz && line._a){
			outval.Move(*line._a);
			//outval.val=line._a->val;
		}

		return ;
	}

	msl_value* DoCodeValue(unsigned char*&line, unsigned char *to, msl_value *&pnval, msl_value *&nval, msl_value *&pval){
		unsigned char *pline=line; int isnew=0; nval=0;

		while(line<to && *line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line>='0' && *line<='9' || *line=='_') line++;
		if(line>=to){
			SetError(HLString()+"EOF.");
			return 0;
		}
		
		// Get existing or create new
		msl_value *val, *lval=0;
		
		if(do_opt_active){
			val=local_value->SGet(VString(pline, line-pline), isnew); pval=local_value;
			if(isnew){ pnval=local_value; nval=val; }
		}
		else val=&null_value;

		lval=val;

		while(1){
			// skip space 
			_skipspace(line, to);
			//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

			if(line<to && *line=='['){
				msl_value dval;
				DoCode(++line, to, dval, ']');

				// next []
				if(do_opt_active){
					pval=val; val=val->SGet(dval.val, isnew);
					if(isnew && !nval){ pnval=lval; nval=val; }
					lval=val;
				}else val=&null_value;
				line++;
			}
			else return val;
		}

		return 0;
	}

	void DoCodeFunction(unsigned char*&line, unsigned char *to, msl_value &val, int &df){
		VString name; unsigned char *pline=line; msl_fl_fargs args;
		unsigned char *code, *ecode;

		while(line<to){
			// normal name
			if(*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line >= '0' && *line <= '9' || *line=='_'){}
			else{
				name.setu(pline, line-pline);

				if(name=="function"){
					 DoCodeFunctionFunction(line, to); df=1; return ;
				}

				_skipspace(line, to);

				if(*line==';'){
					if(name=="continue"){ if(do_opt_active) do_opt_cbr=MSLCBR_CONT; do_opt_active=0; return ; }
					if(name=="break"){ if(do_opt_active) do_opt_cbr=MSLCBR_BREAK; do_opt_active=0; return ; }
					if(name=="return"){ if(do_opt_active) do_opt_cbr=MSLCBR_RET; do_opt_active=0; return ; }
					if(name=="die"){ if(do_opt_active) do_opt_cbr=MSLCBR_DIE; do_opt_active=0; return ; }
				}

				//while(line<to){
					//if(*line=='('){
						unsigned char simple;

						if(*line == '('){
							line ++;
							simple = 0;
						}
						else
							simple = 1;

						code = line;

						if(name=="for"){
							 DoCodeFunctionFor(line, to); df=1; return ;
						}

						if(!DoCodeFunctionArgs(line, to, args, simple)) return ;
						ecode = line;
						
						if(!simple)
							line++;

						// Exec function
						if(!DoCodeFunctionExec(name, args, val)) return ;
						//line++;

						// if, while, for functions
						if(do_opt_ifw){
							if(name=="if")
								DoCodeFunctionIf(line, to, val);
							else if(name=="while")
								DoCodeFunctionWhile(line, to, code, ecode);
							//else if(name=="for")
								//DoCodeFunctionFor(line, to);
							df=1;
						}

						return ;
					//}
					/*else if(!(*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')){
						if(*line==';'){
							if(name=="continue"){ if(do_opt_active) do_opt_cbr=MSLCBR_CONT; do_opt_active=0; return ; }
							if(name=="break"){ if(do_opt_active) do_opt_cbr=MSLCBR_BREAK; do_opt_active=0; return ; }
							if(name=="return"){ if(do_opt_active) do_opt_cbr=MSLCBR_RET; do_opt_active=0; return ; }
							if(name=="die"){ if(do_opt_active) do_opt_cbr=MSLCBR_DIE; do_opt_active=0; return ; }
						}

						SetError(HLString()+"function '"+name+"' open '(' not found.");
						return ;
					}*/

					//line++;
				//}
			}
			line++;
		}

		// line='functionname'EOF
		SetError(HLString()+"end of function name: '"+VString(pline, line-pline)+"'");
		return ;
	}

	int DoCodeFunctionArgs(unsigned char *&line, unsigned char *to, msl_fl_fargs &args, int simple = 0){
		while(!do_opt_stopit){

			// skip space
			_skipspace(line, to);
			// no data
			if(line<to && *line==')') break;

			msl_value val;
			if(!simple)
				DoCode(line, to, val, ',', ')');
			else
				DoCode(line, to, val, ';');
			//if(!DoCodeFunctionArgs(line, to, args)) return ;
			args.Add(val, cursor_value, cursor_value_p);
			if(line>=to){ SetError("not found ')'. EOF"); return 0; }
			if(*line!=',') break;
			line++;
		}
		return 1;
	}

	void DoCodeFunctionIf(unsigned char*&line, unsigned char *to, msl_value &val){
		unsigned char endcode; unsigned char *pline;
		// save values
		int old_ifw=do_opt_ifw, old_active=do_opt_active; do_opt_ifw=0;

		// skip space 
		_skipspace(line, to);
		//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

		// single or {multi}
		if(line>=to){ SetError("if(...) EOF"); return ; }
		
		// set active
		do_opt_active=old_active && old_ifw==2;
		// do if(){ code }
		if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
		DoCodeMulti(line, to, val, endcode); pline=line++; // if(line<to && *line!=';') line++;

		// skip space 
		_skipspace(line, to);
		//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

		// test on 'else'
		if(line+4<to && *line=='e' && *(line+1)=='l' && *(line+2)=='s' && *(line+3)=='e'){
			// skip 'else'
			line+=4;
			// skip space
			_skipspace(line, to);
			//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;
			// set active
			do_opt_active=old_active && old_ifw==1;
			// do else{ code }
			if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
			DoCodeMulti(line, to, val, endcode); if(endcode!=';') line++;
		}
		else if(endcode==';') line=pline;

		// load old value
		do_opt_active=old_active; do_opt_ifw=0;

		// if cbr
		if(do_opt_cbr){
			do_opt_active=0;
		}

		return ;
	}

	void DoCodeFunctionWhile(unsigned char *&line, unsigned char *to, unsigned char *code, unsigned char *ecode){
		msl_value val; unsigned char *lline, *elline, *tline; unsigned char endcode;
		
		// save values
		int old_ifw=do_opt_ifw, old_active=do_opt_active; do_opt_ifw=0;
		// skip space
		_skipspace(line, to);
		//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' || *line=='\n')) line++;

		if(line>=to){ SetError("if(...) EOF"); return ; }

		// set active
		do_opt_active=old_active && old_ifw==2; lline=line;
		// do while(){ code }
		if(*(line)=='{'){ endcode='}'; line++; } else endcode=';';
		DoCodeMulti(line, to, val, endcode); line++;
		elline=line;

		// while
		while(!do_opt_stopit && old_active && old_ifw==2){
			msl_value val;
			// do while( it code )
			tline=code;
			DoCode(tline, ecode+1, val, ')');
			
			// cbr
			if(do_opt_cbr){
				if(do_opt_cbr==MSLCBR_CONT){ do_opt_cbr=MSLCBR_NULL; }
				if(do_opt_cbr==MSLCBR_BREAK){ do_opt_cbr=MSLCBR_NULL; break; }
				if(do_opt_cbr==MSLCBR_RET){ break; }
				if(do_opt_cbr==MSLCBR_DIE){ break; }
			}
			
			// result
			if((!val.val || val.val == "0") && !val.a())
				break;

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

			while(!do_opt_stopit){
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
					if(do_opt_cbr==MSLCBR_DIE){ break; }
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

	int DoCodeFunctionFunction(unsigned char *&line, unsigned char *to){
		unsigned char *lline; VString name, key, val; msl_value value;
		int def_value=0;

		// skip space
		_skipspace(line, to);

		// find name
		lline=line;
		while(line<to && (*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line >= '0' && *line <= '9' || *line=='_')) line++;

		// add functions
		msl_function_d *func=functions.New();
		//func->name.setu(lline, line-lline);
		func->name = temp_data.addnfr(VString(lline, line-lline));

		// '('
		if(line>=to || *line!='('){
			SetError("function '(' not found"); return 0;
		} line++;

		while(1){
			_skipspace(line, to);
			if(line<to && *line=='$'){
				lline=++line;
				while(line<to && (*line>='a' && *line<='z' || *line>='A' && *line<='Z' || *line=='_')) line++;
				if(lline==line){ SetError("$(no_name)"); return 0; }
				key.setu(lline, line-lline);

				_skipspace(line, to);
				if(line<to && *line=='='){
					def_value=1; line++; val.Clean();

					if(_skipnum(line, to, val)){ }
					else if(_skipquote(line, to, val)){ line++; }
					else{ SetError("$key= unknow"); return 0; }
				}else
					if(def_value){
						SetError("$key=default value, $key= no default value"); return 0;
				}
				
				key = temp_data.addnfr(key);
				val = temp_data.addnfr(val);
				func->args.AddF(key, val);

				// ,
				_skipspace(line, to);
				if(line<to){
					if(*line==','){ line++; continue; }
					if(*line==')') break;
				}
				SetError("bad value"); return 0;

			}else if(line<to && *line==')'){ break; }
			else { SetError("bad parameter"); return 0; }
		}

		if(line>=to || *line!=')'){ SetError("Fubction ')' not found"); return 0; }
		line++;

		_skipspace(line, to);
		if(line>=to || *line!='{'){ SetError("Fubction '{' not found"); return 0; }
		
		// skip {
		++line;

		// global values
		_skipspace(line, to);
		if(VString(line, to - line).incompare("global")){
			Itos it;
			line += 6;

			while(line < to){
				_skipspace(line, to);
				if(*line == '$'){
					lline = ++line;
					while(line < to && (*line >= 'a' && *line <= 'z' || *line >= 'A' && *line <= 'Z' || *line == '_'))
						line++;

					if(lline == line){
						SetError("$(no_name)");
						return 0;
					}

					key.setu(lline, line-lline);
					it.Add(it, it ? "." : "", key);
					continue;
				}
				else if(*line == ','){
					if(key){
						func->globals.AddF(temp_data.addnfr(it), "");
						it.Clean();
					} else
						SetError("Bad global in symbol ','");
				}
				else if(*line == ';'){
					line ++;
					func->globals.AddF(temp_data.addnfr(it), "");
					break;
				} else{
					SetError("Bad global line");
					break;
				}
		
				line ++;
			}
		}

		lline = line;

		// save values
		int old_active=do_opt_active; do_opt_active=0;

		// Get Function Code
		DoCodeMulti(line, to, value, '}');

		if(line>=to || *line!='}'){
			SetError("function '}' not found"); return 0;
		}
		line++;

		func->code.setu(lline, line-lline);

		// activate function
		func->UseIt();

		// load old value
		do_opt_active=old_active; do_opt_ifw=0;
		return 1;
	}

	void _skipspace(unsigned char *&line, unsigned char *to){
		//while(line<to && (*line==' ' || *line=='\t' || *line=='\r' && (line+1>=to || *(line+1)=='\n' || line++ && ++do_line_count && (do_line=line+1))
		//	|| (*line=='\n' && ++do_line_count && (do_line=line+1) ))) line++;
		while(line<to){
			if(*line==' ' || *line=='\t') line++;
			else if(*line=='\r'){
				if(line+1<to || *(line+1)=='\n'){ if(!do_line_count_ignore) ++do_line_count; do_line=line+2; }
				line += 2;
			}
			else if(*line=='\n'){
				line++; if(!do_line_count_ignore) ++do_line_count; do_line=line;
			}
			else break;
		}
		return ;
	}

	int _skipnum(unsigned char *&line, unsigned char *to, VString &val){
		if(line<to && *line>'0' && *line<'9'){
			val.data=line;
			while(line<to && *line>'0' && *line<'9') line++;
			val.sz=line-val.data;
			return 1;
		}
		return 0;
	}

	int _skipquote(unsigned char *&line, unsigned char *to, VString &val){
		if(line>=to) return 0;

		if(*line=='\'' || *line=='"'){
			unsigned char q=*line;
			val.data=++line;
			while(line<to && *line!=q) line++;
			val.sz=line-val.data;
			return 1;
		}
		return 0;
	}


	void msv_func_print_r(msl_value *v, HLString &ls){
		int f=1;
		ls+"{";
		while(v){
			if(!f) ls+", "; else f=0;
			ls+"'"+v->key+"' => '"+v->val+"'";
			if(v->_a)
				msv_func_print_r(v->_a, ls);
			v=v->_n;
		}
		ls+"}";
		return ;
	}

	int DoCodeFunctionExec(VString name, msl_fl_fargs &args, msl_value &val){
		// all sections

		if((name=="if" || name=="while") && args.Sz()==1){
			do_opt_ifw=args[0].val.val && args[0].val.val!="0" || args[0].val.a(); do_opt_ifw++;
			return 1;
		}

		// if active
		if(!do_opt_active) return 1;

		if(extfunc){
			int r=extfunc->DoCodeFunctionExec(name, args, val);
			if(r)
				return 1;
		}

		int r=DoCodeFunctionExecDefault(name, args, val);
		if(r)
			return 1;

		//
		msl_function_d *func=functions.Find(name, args.Sz());
		if(func){
			// save and new
			msl_value *_local_value=local_value, func_value; local_value=&func_value;
			// save values
			int old_active  =do_opt_active;
			int old_do_line_count_ignore = do_line_count_ignore;
			// output value
			msl_value outval;

			do_line_count_ignore = 1;

			for(int i=0; i<func->args.Sz(); i++){
				local_value->Set(func->args[i].fkey, &args[i].val); //(args.Sz()>=i+1) ? args[i].val.val : func->args[i].fval);
			}

			msl_value *global_uf[256]; // max function globals

			for(int i = 0; i < func->globals.Sz(); i++){
				msl_value *gv, *lv;
				VString name = func->globals[i].fkey;
				gv = global_value.SGet(name);
				lv = local_value->Set(name, "");
				if(lv->a()){
					SetError("global $(value) allready defined.");
				}

				if(gv->_a == &functions_uf){
					global_uf[i] = gv->_e;
					lv->_a = gv->_e->_a;
					lv->_e = gv->_e->_e;
					lv->val -= gv->_e->val;
				}else{
					lv->_a = gv->_a;
					lv->_e = gv->_e;
					lv->val -= gv->val;

					global_uf[i] = gv;
					gv->_a = &functions_uf;
					gv->_e = lv;
				}
			}

			unsigned char *fline=func->code, *eline=func->code.endu(); 
			
			DoCodeMulti(fline, eline, outval, '}');

			for(int i = 0; i < func->globals.Sz(); i++){
				msl_value *gv, *lv;
				VString name = func->globals[i].fkey;
				gv = global_value.GetV(name);
				lv = local_value->GetV(name);

				if(!lv && gv){
					global_uf[i]->_a = 0;
					global_uf[i]->_e = 0;
					//gv->_a = 0;
					//gv->_e = 0;
				} else if(lv && gv){
					global_uf[i]->_a = lv->_a;
					global_uf[i]->_e = lv->_e;
					global_uf[i]->val -= lv->val;
					//gv->_a = lv->_a;
					//gv->_e = lv->_e;
					//gv->val -= lv->val;

					lv->_a = 0;
					lv->_e = 0;					
				}
			}
			
			// test continue, break, return
			DoTestCBR();

			// load old value
			do_opt_active=old_active; do_opt_ifw=0;
			do_line_count_ignore = old_do_line_count_ignore;
			// load
			local_value=_local_value;
			// return output value
			val.Move(outval);

			return 1;
		}

		//
		SetError(HLString()+"Function: '"+name+"' not found");
		return 0;
	}

	int DoCodeFunctionExecDefault(VString name, msl_fl_fargs &args, msl_value &val){
		Itos it;

		// exec
		if(name=="print" || name=="echo"){
			for(int i=0; i<args.Sz(); i++){
				SetOutput(args[i].val.val);
				//print(args[i].val.val);
			}
		}

		else if(name=="print_r" && args.Sz()==1){
			SetOutput(args[0].val.val);
			msl_value *v=args[0].val._a;
			HLString ls;

			msv_func_print_r(v, ls);
			SetOutput(ls);
		}

		else if(name=="array"){
			Itos it;

			int n=0;
			for(int i=0; i<args.Sz(); i++){
				val.Set(args[i].val.key ? args[i].val.key : it.itos(n++), args[i].val.val);
			}
		}

		else if(name=="include" && args.Sz()==1){
			DoCodeInclude(args[0].val.val);
		}

		// text
		else if(name=="substr" && args.Sz()==2){
			val.val=args[0].val.val.str(args[1].val.val.toi());
		}
		
		else if(name=="substr" && args.Sz()==3){
			val.val=args[0].val.val.str(args[1].val.val.toi(), args[2].val.val.toi());
		}

		// time & data
		else if(name=="clock" && args.Sz()==0){
			val.val=itos(clock());		
		}

		else if(name=="time" && args.Sz()==0){
			val.val=itos(time());
		}

		else if(name=="date" && args.Sz()==1){
			MTime mt; val.val=mt.date(args[0].val.val);
		}

		else if(name=="date" && args.Sz()==2){
			MTime mt; val.val=mt.date(args[0].val.val, args[1].val.val.toi());
		}

		else if(name == "microtime" && args.Sz() == 0){
			timeb tb; ftime(&tb);
			val.val = it.Format("%d 0.%3d", (int)tb.time, tb.millitm);
		}

		// size
		else if(name=="sizeof" && args.Sz()==1){
			if(args[0].pval){
				val.val=itos(args[0].pval->Size());
			}else
				SetWarning("sizeof(need $value);");
		}

		// cursor
		else if(name=="reset" && args.Sz()==1){
			if(args[0].pval){
				cursor.reset(args[0].pval);
			}else
				SetWarning("reset(need $value);");
		}

		else if(name=="prev" && args.Sz()==1){
			if(args[0].pval){
				msl_value *v=cursor.prev(args[0].pval);
				if(v)
					val.val=v->val;
			} else
				SetWarning("prev(need $value);");
		}

		else if(name=="current" && args.Sz()==1){
			if(args[0].pval){
				msl_value *v=cursor.current(args[0].pval);
				if(v)
					val.val=v->val;
			} else
				SetWarning("current(need $value);");
		}

		else if(name=="next" && args.Sz()==1){
			if(args[0].pval){
				msl_value *v=cursor.next(args[0].pval);
				if(v)
					val.val=v->val;
			} else
				SetWarning("next(need $value);");
		}

		else if(name=="end" && args.Sz()==1){
			if(args[0].val._a && args[0].pval){
				msl_value *v=cursor.end(args[0].pval);
				if(v)
					val.val=v->val;
			} else
				SetWarning("end(need $value);");
		}

		// value
		else if(name=="kprev" && args.Sz()==1){
			if(args[0].pval){
				msl_value *v=cursor.prev(args[0].pval);
				if(v)
					val.val=v->key;
			} else
				SetWarning("prev(need $value);");
		}

		else if(name=="kcurrent" && args.Sz()==1){
			if(args[0].pval){
				msl_value *v=cursor.current(args[0].pval);
				if(v)
					val.val=v->key;
			} else
				SetWarning("current(need $value);");
		}

		else if(name=="knext" && args.Sz()==1){
			if(args[0].pval){
				msl_value *v=cursor.next(args[0].pval);
				if(v)
					val.val=v->key;
			} else
				SetWarning("next(need $value);");
		}

		else if(name=="kend" && args.Sz()==1){
			if(args[0].val._a && args[0].pval){
				msl_value *v=cursor.end(args[0].pval);
				if(v)
					val.val=v->key;
			} else
				SetWarning("end(need $value);");
		}

		// isset
		else if(name=="isset" && args.Sz()==1){
			val.val = args[0].pval ? "1" : "0";				
		}

		// unset
		else if(name=="unset" && args.Sz()==1){
			if(args[0].pval){
				args[0].ppval->Del(args[0].pval);
			} else
				SetWarning("unset(need $value);");
		}

		// explode
		else if(name=="explode" && args.Sz()==2){
			VString line = args[1].val.val, sep = args[0].val.val;
			int cnt = 0;

			while(line){
				VString o = PartLine(line, line, sep);
				val.Add(it.itos(cnt++), o);
			}
		}

		// replace
		else if(name=="str_replace" && args.Sz()==3){
			val.val = Replace(args[2].val.val, args[0].val.val, args[1].val.val);
		}		

		// return
		else if(name == "return" && args.Sz() <= 1){
			if(do_opt_active)
				do_opt_cbr = MSLCBR_RET;
			do_opt_active = 0;
			if(args.Sz() == 1)
				val.Move(args[0].val);
		}

		// die
		else if(name == "die" && args.Sz() == 0){
			do_opt_cbr = MSLCBR_DIE;
			do_opt_active = 0;
		}

		// info
		else if(name == "msl_info"){
			val.val = it.Format("MSL: My Server Lanruage %s(%s).", msl_fl_version[0].ver, msl_fl_version[0].date);
		}

#ifdef USEMSV_MYSQL
		// mysql
		else if(extension_mysql.DoCodeFunctionExec(name, args, val)){}
#endif

		//else if(name == 

		else 
			return 0;

		return 1;
	}

	void SetExtFunc(msl_fl_extfunc *f){
		extfunc=f;
	}

	// HTTP input
	void SetHttpGet(VString line){
		msl_value *data = SetValue("_GET", "");
		while(line){
			VString key = PartLine(line, line, "&"), val;
			key = PartLine(key, val, "=");
			data->Set(key, HttpToVal(val));
		}
	}

	void SetHttpPost(VString line){
		msl_value *data = SetValue("_POST", "");
		while(line){
			VString key = PartLine(line, line, "&"), val;
			key = PartLine(key, val, "=");
			data->Set(key, HttpToVal(val));
		}
	}

	void SetHttpCookie(VString line){
		msl_value *data = SetValue("_COOKIE", "");
		while(line){
			VString key = PartLine(line, line, ";"), val;
			if(key[0] == ' '){
				key.data++; key.sz--;
			}
			key = PartLine(key, val, "=");
			data->Set(key, val);
		}

		return ;
	}

	// global value
	msl_value* SetValue(VString key, VString val){
		return local_value->Set(key, val);		
	}

	VString GetValue(VString key){
		return local_value->Get(key);		
	}
	
	msl_value* GetVValue(VString key){
		return local_value->GetV(key);		
	}


	// get output
	MString GetOutput(){
		// return result;
		return MString(output.oneline(), output.size());
	}

	int _getlinecount(){ return do_line_count; }
	int _getlinesz(){ if(do_line>do_line_t) return 0; return do_line_t-do_line; }

protected:

	// set output
	void SetOutput(VString line){
		// add line to result
		output+line;
		return ;
	}

	void SetWarning(VString line){
		// add error line to result
		output + "MSL-FL Warning: '" + line + "' in '" + file_path + "' on " + itos(_getlinecount() + 1) + " line " + itos(_getlinesz()) + " row\r\n";
		return ;
	}

	void SetError(VString line){
		// add error line to result
		output + "MSL-FL Error: '" + line + "' in '" + file_path + "' on " + itos(_getlinecount() + 1) + " line " + itos(_getlinesz()) + " row\r\n";
		// stop
		do_opt_stopit = 1;
		return ;
	}

	void SetEpic(VString line){
		// add error line to result
		output + "MSL-FL Epic Fail: '" + line + "' in '" + file_path + "' on " + itos(_getlinecount() + 1) + " line " + itos(_getlinesz()) + " row\r\n";
		// stop
		do_opt_stopit = 1;
		return ;
	}

};

#ifdef USEMSV_XDATACONT

void JsonToMsl(XDataEl *el, msl_value &val, int clear=1){
	if(clear)
		val.Clear();

	el=el->a(); int i=0;
	while(el){
		if(!el->a())
			val.Set(el->key ? el->key : itos(i++), el->val);
		else{
			JsonToMsl(el, *val.Set(el->key ? el->key : itos(i++), ""), clear);
		}
		el=el->n();
	}
	return ;
}

void JsonToMslMulti(XDataEl *el, msl_value &val, int clear=1){
	if(clear)
		val.Clear();

	//el=el->a();
	int i=0;
	while(el){
		if(!el->a())
			val.Set(el->key ? el->key : itos(i++), el->val);
		else{
			JsonToMsl(el, *val.Set(el->key ? el->key : itos(i++), ""), clear);
		}
		el=el->n();
	}
	return ;
}

//void MslToJson(msl_value &val, HLString &ls){
//	msl_value *el=val._a; int f=1;
//	ls+"{";
//		
//	while(el){
//		if(!f) ls+","; else f=0;
//		if(!el->_a)
//			ls+"\""+el->key+"\":\""+el->val+"\"";
//		else{
//			ls+"\""+el->key+"\":";
//			MslToJson(*el, ls);
//		}
//		el=el->_n;
//	}
//	ls+"}";
//	return ;
//}

/*
void GetLine(msl_value &val, HLString &ls);

MString GetLine(msl_value &val){
	if(!val._a) return MString();
	HLString ls;
	GetLine(val, ls);
	return ls;
}

void GetLine(msl_value &val, HLString &ls){
	int a=val.a()!=0 && val.a()->key, f=1;
	if(a)
		ls+"{";
	else
		ls+"[";

	msl_value *v=val._a;
	while(v){
		if(!f) ls+","; else f=0;
		if(v->_a){
			if(a)
				ls+"\""+v->key+"\":";
			GetLine(*v, ls);
		} else{
			if(v->key)
				ls+"\""+v->key+"\":";
			if(!v->val) ls+"null";
			else ls+"\""+v->val+"\"";
		}
		//ls+v->key+":"+v->val;
		v=v->_n;
	}

	if(a)
		ls+"}";
	else
		ls+"]";
	return ;
}
*/


// msl_value_template
template<class B>
void JsonToMsl(XDataEl *el, msl_value_template<B> &val, int clear = 1){
	if(clear)
		val.Clear();

	int i = 0;
	Itos it;
	el = el->a();
	
	while(el){
		if(!el->a())
			val.Set(el->key ? el->key : it.itos(i++), el->val);
		else{
			JsonToMsl(el, *val.Set(el->key ? el->key : it.itos(i++), ""), clear);
		}
		el = el->n();
	}
	return ;
}


template<class B>
unsigned int GetLineCount(msl_value_template<B> &val){
	int a = val.a() != 0 && val.a()->k(), f = 1;
	int sz = 0;

	msl_value_template<B> *v = val._a;
	while(v){
		if(!f)
			sz ++;
		else
			f = 0;

		if(v->_a){
			if(a)
				sz += 3 + v->k().sz;
			sz += GetLineCount(*v);
		} else{
			if(v->k())
				sz += 3 + v->k().sz;

			if(!v->v())
				sz += 4;
			else
				sz += 2 + v->v().sz;
		}

		v = v->_n;
	}

	sz +=2;

	return sz;
}

template<class B>
unsigned char* GetLine(msl_value_template<B> &val, unsigned char *to){
	int a = val.a() != 0 && val.a()->k(), f = 1;
	if(a)
		*to ++ = '{';
	else
		*to ++ = '[';

	msl_value_template<B> *v = val._a;
	while(v){
		if(!f)
			*to ++ = ',';
		else
			f = 0;

		if(v->_a){
			if(a){
				*to ++ = '"';
				memcpy(to, v->k(), v->k());
				to += v->k();
				*to ++ = '"';
				*to ++ = ':';
			}
			to = GetLine(*v, to);
		} else{
			if(v->k()){
				//ls + "\"" + v->k() + "\":";
				*to ++ = '"';
				memcpy(to, v->k(), v->k());
				to += v->k();
				*to ++ = '"';
				*to ++ = ':';
			}

			if(!v->v()){
				memcpy(to, "null", 4);
				to += 4;
				//ls + "null";
			}
			else{
				//ls + "\"" + v->v() + "\"";
				*to ++ = '"';
				memcpy(to, v->v(), v->v());
				to += v->v();
				*to ++ = '"';
			}
		}
		//ls+v->key+":"+v->val;
		v = v->_n;
	}

	if(a)
		*to ++ = '}';
	else
		*to ++ = ']';

	return to;
}

template<class B>
TString GetLine(msl_value_template<B> &val){
	if(!val._a)
		return TString();

	TString ret;
	ret.Reserv(GetLineCount(val));

	GetLine(val, ret);

	//HLString ls;
	//GetLine(val, ls);

	return ret;
}

template<class B>
void GetLineL(msl_value_template<B> &val, HLString &ls){
	int a = val.a() != 0 && val.a()->k(), f = 1;
	if(a)
		ls + "{";
	else
		ls + "[";

	msl_value_template<B> *v = val._a;
	while(v){
		if(!f)
			ls + ",";
		else
			f = 0;

		if(v->_a){
			if(a)
				ls + "\"" + v->k() + "\":";
			GetLine(*v, ls);
		} else{
			if(v->k())
				ls + "\"" + v->k() + "\":";
			if(!v->v())
				ls + "null";
			else
				ls + "\"" + v->v() + "\"";
		}
		//ls+v->key+":"+v->val;
		v = v->_n;
	}

	if(a)
		ls + "}";
	else
		ls + "]";
	return ;
}

template<class B>
TString GetLinePost(msl_value_template<B> &val){
	if(!val._a)
		return TString();

	HLString ls;

	msl_value_template<B> *p = val._a;
	while(p){
		if(ls)
			ls + "&";
		ls + p->k() + "=" + p->v();

		p = p->_n;
	}

	return ls;
}


#endif


#ifdef USEMSV_CJX

template<class B>
unsigned int CjxGetLineCount(msl_value_template<B> &val){
	int a = val.a() != 0 && val.a()->k();
	int sz = 0;

	CjxProtoData d;

	msl_value_template<B> *v = val._a;
	while(v){
		if(v->_a){
			sz += d.CountUp(v->k());
			sz += CjxGetLineCount(*v);
		} else
			sz += d.Count(v->k(), v->v());

		v = v->_n;
	}

	sz += d.CountDown();

	return sz;
}

template<class B>
unsigned char* CjxGetLine(msl_value_template<B> &val, unsigned char *to){
	int a = val.a() != 0 && val.a()->k(), f = 1;
	
	msl_value_template<B> *v = val._a;
	while(v){
		CjxProtoData &d = (CjxProtoData*)to;

		if(v->_a){
			to += d.SetLineUp(v->k());
			to = CjxGetLine(*v, to);
		} else
			to += d.SetLine(v->k(), v->v());

		v = v->_n;
	}

	CjxProtoData &d = (CjxProtoData*)to;
	to += d.SetLineDown();

	return to;
}

template<class B>
TString CjxGetLine(msl_value_template<B> &val){
	if(!val._a)
		return TString();

	TString ret;
	unsigned int rsz = CjxGetLineCount(val);

	ret.Reserv(sizeof(CjxProtoHead) + rsz);

	CjxProtoHead &head = (CjxProtoHead*) ret.data;
	unsigned char *line = ret.data + sizeof(CjxProtoHead);

	CjxGetLine(val, line);

	head.Init(rsz);

	return ret;
}

#ifdef IAM_YOU_CODE___NOOOOOOO


void GetCJXLineD(int &els, msl_value &val, HLString &ls);

MString GetCJXLine(msl_value &val){
	HLString ls;
	MString ret;
	int els=0;

	if(!val._a){
		ret.Reserv(sizeof(CJXParseH));
		CJXParseH *h=(CJXParseH*)ret.data;
		if(!h)
			return MString();
		
//		h->ver=1;

		return ret;
	}

	CJXParseH *h=(CJXParseH*)ls.addnfv(sizeof(CJXParseH));
	if(!h)
		return MString();
	
	GetCJXLineD(els, val, ls);

//	h->els=els;
	h->size=ls.size()-sizeof(CJXParseH);

	return ls;
}

void GetCJXLineD(int &els, msl_value &val, HLString &ls){
	// count
	int sz=val.Size(), t;
	if(sz>127){
		t=bsize(sz)|128;
		ls.add((char*)&t, 1);
		ls.add((char*)&sz, t&127);
	}else{
		ls.add((char*)&sz, 1);
	}

	msl_value *v=val._a;
	while(v){
		CJXParseL l;
		l.up=v->_a!=0;

		l.keysz = v->key.sz>15 ? 15 : v->key.sz;

		if(v->val.sz>511){
			l.valsz=bsize(v->val.sz)|512;
			ls.add((char*)&l, sizeof(l));
			ls.add((char*)&v->val.sz, bsize(v->val.sz));
		}else{
			l.valsz=v->val.sz;
			ls.add((char*)&l, sizeof(l));
		}

		ls.add(v->key, l.keysz);
		ls.add(v->val, v->val);

		els++;
		v=v->_n;
	}

	return ;
}

#endif
#endif