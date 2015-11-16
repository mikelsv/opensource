#define CXXLEXPT_CODELINE		1	// codeblock
#define CXXLEXPT_DEF			10	// #define
#define CXXLEXPT_DEFP			100	// #define (params)
#define CXXLEXPT_NAME			11	// int or value or any name
#define CXXLEXPT_INT			12	// ineger 111 or 0x111
#define CXXLEXPT_CHAR			13	// 'c' har
#define CXXLEXPT_TEXT			14	// "text"
#define CXXLEXPT_UP				15	// ( up )
#define CXXLEXPT_BLOCK			16	// { code }
#define CXXLEXPT_ARR			17	// [ index ]
//#define CXXLEXPT_TEMPLATE		18	// < template >
#define CXXLEXPT_OP				19	// operation: + - * / ...
#define CXXLEXPT_NVAL			20  // ,
#define CXXLEXPT_END			21	// ;
#define CXXLEXPT_COMMENT		22	// /* comment */

// CXXLEXPT_DEF(CXXLEXPT_DEFP) CXXLEXPT_BLOCK

#define MSVXCC_PRINT_ANALYSFILE		1
#define MSVXCC_PRINT_MODDEFINES		2
#define MSVXCC_PRINT_SAVEFILES		4
#define MSVXCC_PRINT_ALL			S2GM


Versions CppXccVer[]={
	"0.0.0.1", "07.11.2015 05:59",
};

class CppLexpD : public OMatrixT<CppLexpD>{

public:
	CppLexpD *_p, *_n;

	int type;
	int ext;
	VString val;

	int tmp;

public:

	CppLexpD(){
		//size = 0;
	}

	// Get
	CppLexpD *GetFirst(){ return _a; }
	CppLexpD *GetEnd(){ return _e; }
	
	CppLexpD *GetEnd(CppLexpD *el){
		if(!el)
			return 0;

		while(el->_n)
			el = el->_n;

		return el;	
	}

	int Size(){
		CppLexpD *p = _a;
		int size = 0;

		while(p){
			p = p->_n;
			size ++;
		}

		return size;
	}

	int Size(CppLexpD *fr, CppLexpD *to){
		int s = 0;

		while(fr && fr->type){
			s++;

			if(fr == to)
				break;

			fr = fr->_n;			
		}
		return s;
	}

	// Add
	void Add(CppLexpD *p){
		p->_a = 0;
		p->_n = 0;

		OMAdd(p);

		return ;
	}

	// Get
	CppLexpD* a(){
		if(!this)
			return 0;
		return _a;
	}

	CppLexpD* n(){
		if(!this)
			return 0;
		return _n;
	}

	void Clear(){
		_a = 0;
		_e = 0;

		_p = 0;
		_n = 0;
	}

};


class CppLexp{
	// Defines
	#define CPPLEXP_PUSHINT(val) int old_ ## val = val;
	#define CPPLEXP_POPINT(val) val = old_ ## val;

	#define CPPLEXP_PUSHSTR(cls, val) cls old_ ## val = val;
	#define CPPLEXP_POPSTR(cls, val) val = (cls)old_ ## val;

	// File
	VString filename;
	VString filedata;

	// Data
	CppLexpD *lexp_first, *lexp_this;
	HLString0 lexpdata;

	// Options
	int do_opt_stopit;		// stop it
	int do_opt_line_count;	// line count
	int do_opt_ifdef;		// #if in def

	// Errors
	HLString0 warning, error;

public:
	CppLexp(){
		lexp_first = 0;
		lexp_this = 0;
	}

	CppLexpD* GetFirstLexp(){
		return lexp_first;
	}

	int Analys(VString name, VString data){
		filename = name;
		filedata = data;

		do_opt_stopit = 0;
		do_opt_line_count = 0;
		do_opt_ifdef = 0;

		lexp_first = 0;
		lexp_this = 0;
		lexpdata.Clear();

		return AnalysMulti();
	}

protected:

	int AnalysMulti(){
		unsigned char *ln = filedata, *to = filedata.endu(), *lln;
		return AnalysMulti(ln, to);
	}

	int AnalysMulti(unsigned char *&ln, unsigned char *to, unsigned char ecode = 1){
		unsigned char *lln;

		if(!lexp_first){
			lexp_first = LexpAddFirst();
			lexp_this = lexp_first;
		}

		while(ln < to){
			CPPLEXP_PUSHSTR(CppLexpD*, lexp_this);

			CppLexpD * lexp = LexpAddLine();
			lexp_this = lexp;
			lln = ln;

			if(!AnalysLine(ln, to, ';', ecode))
				return 0;

			if(ln < to && *ln ==';')
				LexpAdd(CXXLEXPT_END, VString(ln, 1));

			ln ++;

			CPPLEXP_POPSTR(CppLexpD*, lexp_this);

			LexpUpdLine(lexp, VString(lln, ln - lln));

			if(*(ln - 1) == ecode){
				ln --;
				return 1;
			}
		}

		if(do_opt_stopit)
			return 0;

		return 1;
	}

	int AnalysLine(unsigned char *&ln, unsigned char *to, unsigned char ecode = 1, unsigned char ecodet = 1){
		unsigned char *lln = ln;

		while(ln < to){
			// Name
			if(*ln >= 'a' && *ln <= 'z' || *ln >= 'A' && *ln <= 'Z' || *ln == '_' || *ln == '$' /* || *ln == '#' && !do_opt_ifdef*/){
				lln = ln ++;

				while(ln < to && (*ln >= 'a' && *ln <= 'z' || *ln >= 'A' && *ln <= 'Z' || *ln == '_' || *ln == '$' || *ln >= '0' && *ln <= '9'))
					ln ++;

				if(VString(lln, ln - lln) == "GetCurrentThread")
					int rty = 67;

				CppLexpD *lxp = LexpAdd(CXXLEXPT_NAME, VString(lln, ln - lln));
				continue;
			}
			// Comments
			else if(*ln == '/' && ln + 1 < to && ( *(ln + 1) == '/' || *(ln + 1) == '*')){
				int t = *(ln + 1) == '*';
				lln = ln + 2;

				if(!t)
					while(ln < to && *ln != '\n')
						ln ++;
				else
					while(ln < to){
						if(*ln == '*' && ln + 1 < to && *(ln + 1) == '/')
							break;
						ln ++;
					}

				LexpAdd(CXXLEXPT_COMMENT, VString(lln, ln - lln));

				if(t)
					ln += 2;

				continue;
			}
			// Special code
			else if(*ln == ecode || *ln == ecodet){
				return 1;
			}
			// Define #
			else if(*ln == '#' && !do_opt_ifdef){
				CppLexpD *lxp = LexpAdd(CXXLEXPT_DEF, VString(lln, ln - lln));
				
				CPPLEXP_PUSHSTR(CppLexpD*, lexp_this);
				lexp_this = lxp;

				do_opt_ifdef = 1;

				if(!AnalysLine(++ln, to, '\n'))
					return 0;

				do_opt_ifdef = 0;

				CPPLEXP_POPSTR(CppLexpD*, lexp_this);

				if(ln < to && *ln == '\n' && ecode == ';')
					return 1;

			}
			// Operations
			else if(*ln == '+' || *ln == '-' || *ln == '*' || *ln == '/' || *ln == '.' || *ln == '!' || *ln == '<' || *ln == '>' || *ln == '=' || *ln == '|' || *ln == '&' || *ln == '%' || *ln == '~' || *ln == '^' || *ln == '#' || *ln == ':' || *ln == '?'){
				lln = ln ++;

				while(ln < to && (*ln == '+' || *ln == '-' || *ln == '*' || *ln == '/' || *ln == '.' || *ln == '!' || *ln == '<' || *ln == '>' || *ln == '=' || *ln == '|' || *ln == '&' || *ln == '%' || *ln == '~' || *ln == '^' || *ln == '#' || *ln == ':'))
					ln ++;

				if(ln >= to){
					SetError("EOF");
					return 0;
				}

				LexpAddOp(VString(lln, ln - lln));
				continue;
			}
			// int
			else if(*ln >= '0' && *ln <= '9'){
				lln = ln ++;

				if(ln < to && *ln == 'x')
					ln ++;

				while(ln < to && *ln >= '0' && *ln <= '9')
					ln ++;

				LexpAdd(CXXLEXPT_INT, VString(lln, ln - lln));
				continue;
			}
			// char
			else if(*ln == '\''){
				ln ++;
				
				if(ln < to && *ln == '\\')
					ln ++;

				if(ln + 1 >= to || *(ln + 1) != '\''){
					SetError("No close '");
					return 0;
				}

				LexpAdd(CXXLEXPT_CHAR, VString(ln, 1));

				ln ++;
			}
			// text
			else if(*ln == '"'){
				lln = ++ ln;

				while(ln < to){
					if(*ln == '\\')
						ln ++;
					else if(*ln == '"')
						break;
					ln ++;
				}

				LexpAdd(CXXLEXPT_TEXT, VString(lln, ln - lln));
			}
			// Up
			else if(*ln == '('){
				CPPLEXP_PUSHSTR(CppLexpD*, lexp_this);
				lexp_this = LexpAdd(CXXLEXPT_UP, VString(ln, 1));


				if(!AnalysLine(++ln, to, ')'))
					return 0;

				//LexpAddUpEnd();
				CPPLEXP_POPSTR(CppLexpD*, lexp_this);

				if(ln >= to || *ln != ')'){
					SetError("Close ')' not found");
					return 0;
				}
			}
			// Code block
			else if(*ln == '{'){
				CPPLEXP_PUSHSTR(CppLexpD*, lexp_this);
				lexp_this = LexpAdd(CXXLEXPT_BLOCK, VString(ln, 1));
				lln = ln;

				if(!AnalysMulti(++ln, to, '}'))
					return 0;

				//LexpAddUpEnd();
				CPPLEXP_POPSTR(CppLexpD*, lexp_this);

				if(ln >= to || *ln != '}'){
					SetError("Close '}' not found");
					return 0;
				}
			}
			// Arr []
			else if(*ln == '['){

				CPPLEXP_PUSHSTR(CppLexpD*, lexp_this);
				lexp_this = LexpAdd(CXXLEXPT_ARR, VString(ln, 1));

				if(!AnalysLine(++ln, to, ']'))
					return 0;

				//LexpAddUpEnd();
				CPPLEXP_POPSTR(CppLexpD*, lexp_this);

				if(ln >= to || *ln != ']'){
					SetError("Close ']' not found");
					return 0;
				}
			}
			//// Define #
			//else if(*ln == '#'){
			//	lln = ++ ln;

			//	while(ln < to){
			//		if(*ln == '\r' || *ln == '\n')
			//			break;
			//		ln ++;
			//	}

			//	LexpAddVal(CXXLEXPT_DEF, VString(ln, lln - ln));
			//}
			// Free space
			else if(*ln == ' ' || *ln == '\r' || *ln == '\n' || *ln =='\t'){
				if(*ln == '\n')
					do_opt_line_count ++;
			}
			// Next line
			else if(*ln == '\\'){
				lln = ++ln;

				while(ln + 1 < to){
					if(*ln == '\n')
						do_opt_line_count ++;
					else if(*ln != '\r')
						break;
					ln ++;
				}

				continue;
			}
			// Next value
			else if(*ln == ','){
				LexpAdd(CXXLEXPT_NVAL, VString(ln, 1));
			}
			// End code
			else if(*ln == ';'){
				LexpAdd(CXXLEXPT_END, VString(ln, 1));
			}

			// Special sumbol in WinNt.h
			else if(*ln == 12){
				// ignore
			}
			// Error
			else{
				SetError(HLString() + "Unknown sumbol: '" + VString(ln, 1) + "'.");
				return 0;
			}

			ln ++;
		}

		return 1;
	}

	// Lexp
	CppLexpD* LexpAddFirst(){
		CppLexpD *lexp = (CppLexpD*)lexpdata.Add(0, sizeof(CppLexpD), 1);
		return lexp;
	}

	CppLexpD* LexpAdd(){
		CppLexpD *lexp = (CppLexpD*)lexpdata.Add(0, sizeof(CppLexpD), 1);
		lexp_this->Add(lexp);
		return lexp;
	}

	CppLexpD* LexpAddLine(){
		CppLexpD *lexp = LexpAdd();
		lexp->type = CXXLEXPT_CODELINE;
		return lexp;
	}

	void LexpUpdLine(CppLexpD* l, VString line){
		l->val = line;
		return ;
	}

	CppLexpD* LexpAdd(int type, VString val = VString()){
		CppLexpD *lexp = LexpAdd();
		lexp->type = type;
		lexp->val = val;
		return lexp;
	}

	//CppLexpD* LexpAddUp(int type = CXXLEXPT_UP){
	//	CppLexpD *lexp = LexpAdd();
	//	lexp->type = type;
	//	return lexp;
	//}

	void LexpAddOp(VString op){
		CppLexpD *lexp = LexpAdd();
		lexp->type = CXXLEXPT_OP;
		lexp->val = op;
		lexp->ext = GetOpId(op);
	}

	// Op
	#define MCCOPTID(val, id) if(op==val) return id;
	int GetOpId(VString op){
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

		MCCOPTID("##", MCCOP_DEFADD);
		MCCOPTID("::", MCCOP_DEFSUB);
		MCCOPTID(":#", MCCOP_USEDEF);

		return -1;
	}

public:

	// Error
	VString GetError(){
		return error.String();
	}

	void SetWarning(VString line){
		// add error line to result
		warning + "CPP-LEXP Warning: '" + line + "' in '" + filename + "' on " + (do_opt_line_count + 1) + " line\r\n";
		return ;
	}

	void SetError(VString line){
		// add error line to result
		error + "CPP-LEXP Error: '" + line + "' in '" + filename + "' on " + (do_opt_line_count + 1) + " line\r\n";
		// stop
		do_opt_stopit = 1;
		return ;
	}

};


// File
class CppXccFile{
public:
	// Data
	MString filename;
	MString filedata;
	int filesize;

	// Lexp
	CppLexp lexp;

	// Replace
	HLString moddata; // Save to file
	HLString modcode; // Temp for #else
	int modpos;

	// Options
	int pragma_once;

	CppXccFile(){
		filesize = 0;
		modpos = 0;
		pragma_once = 0;
	}

};

// Define
class CppXccDefine{
public:
	MString name;
	CppLexpD *param;
	CppLexpD *value;
};

class CppXcc{
	// Lists
	UList<CppXccFile, 3, 1> files;
	UList<CppXccDefine, 3, 1> defines;

	// Options
	int opt_print;
	int opt_replace_enable;
	VString opt_flist; // full listing
	VString opt_ipath; // include path

	// Process
	CppXccFile * this_file;
	int proc_replace_do; // replace it
	//int proc_replace_level; // set level to end replace
	int proc_active_code; // active code & defines
	int proc_ifdef_level; // ifdef level
	int proc_ifdef_activate; // ifdef level for proc_active_code => 1
	int proc_ifdef_dend; // double end

	HLString0 proc_flist;

	// Error
	HLString0 error, warning;

public:

	// Result
	int res_code_lines;

	CppXcc(){
		opt_print = 0;
		opt_replace_enable = 0;

		InitProc();
	}

	void InitProc(){
		proc_replace_do = 0;
		proc_active_code = 1;
		proc_ifdef_level = 0;
		proc_ifdef_dend = 0;

		res_code_lines = 0;
	}

	// Options
	int GetEnableReplace(){
		return opt_replace_enable;
	}

	void SetEnableReplace(int val = 1){
		if(val)
			SetDefine("MSVXCCTEMPLATE");
		else
			DelDefine("MSVXCCTEMPLATE");
		opt_replace_enable = val;
	}

	void SetEnableFullList(VString file = VString()){
		opt_flist = file;
	}

	void SetIncludePath(VString path = VString()){
		opt_ipath = path;
	}

	void SetPrint(int val){
		opt_print = val;
	}

	int AnalysFile(VString file, int first = 1){
		if(first)
			InitProc();

		int is = IsFile(file);

		if(opt_print & MSVXCC_PRINT_ANALYSFILE)			
			print(HLString() + "CppXcc Analys file: " + file + (is ? " OK " : " FAIL(File not found)") + "\r\n");

		if(opt_flist){
			proc_flist + "// CppXcc Analys file: "+ file + "\r\n";
			//if(!is)
				//proc_flist + "// NOT FOUND" "\r\n" + "#include <" + file + ">\r\n\r\n";
		}

		CppXccFile* fl = GetFile(file);
		int ret;

		if(!fl){
			fl = AddFile(file);
			fl->filedata = LoadFile(file);
			fl->filesize = fl->filedata;
			ret = AnalysFile(fl);
		} else{
			if(fl->pragma_once)
				return 1;
			ret = AnalysFile(fl, 1);
		}


		if(first && proc_ifdef_level){
			if(proc_ifdef_level)
				SetError("EOF. Not found #endif");
			return 0;
		}

		if(first)
			CheckFiles();

		return is ? ret : -1;
	}

	int AnalysFile(CppXccFile *fl, int again = 0){
		this_file = fl;

		if(!again)
		if(!fl->lexp.Analys(fl->filename, fl->filedata)){
			SetError(HLString() + "CppXcc Lexp:" + fl->lexp.GetError());
			return 0;
		}

		int ret = AnalysLexp(fl->lexp.GetFirstLexp());

		return ret;
	}

	int AnalysLexp(CppLexpD *lexp){
		//print("+: ", lexp->val, "\r\n");

		while(lexp){
			// Analys define
			if(lexp->type == CXXLEXPT_DEF){
				if(!AnalysDefine(lexp))
					return 0;
			}else{
				if(proc_active_code && (proc_replace_do || opt_flist)){
					int rc = SelReplaceCode(lexp);

					if(lexp->_a){
						AnalysLexp(lexp->_a);
					}

					if(lexp->type == CXXLEXPT_UP || lexp->type == CXXLEXPT_BLOCK || lexp->type == CXXLEXPT_ARR){
						SelReplaceCodeClose(lexp);
					}					

					if(rc == 2){
						lexp = lexp->n()->n();
						continue;
					}

				}
				else
					if(lexp->_a)
						AnalysLexp(lexp->_a);
			}

			lexp = lexp->_n;
		}

		return 1;
	}

	// Analys Defines
	int AnalysDefine(CppLexpD *plexp){
		VString dname = plexp->_a ? plexp->_a->val : "";
		CppLexpD *lexp = plexp->a()->n();

		if((!lexp || !lexp->val) && dname !="else" && dname != "endif"){
			SetLexpError(HLString() + "#" + dname + " not found name", lexp);
			return 0;
		}

		if(dname == "define"){
			return AnalysDefineDefine(plexp, lexp);
		} else if(dname == "undef"){
			if(!proc_active_code)
				return 1;

			if(opt_print && MSVXCC_PRINT_MODDEFINES)
				print("#undef ", lexp->val, "\r\n");

			DelDefine(lexp->val);

		} else if(dname == "if"){
			return AnalysDefineIf(plexp, lexp);
		} else if(dname == "ifdef" || dname == "ifndef"){
			return AnalysDefineIfdef(plexp, lexp, dname == "ifdef");
		} else if(dname == "else"){
			return AnalysDefineElse(plexp, plexp);
		} else if(dname == "endif"){
			return AnalysDefineEndif(plexp, plexp);
		} else if(dname == "elif"){
			proc_ifdef_dend = proc_ifdef_level;
			return AnalysDefineElse(plexp, lexp) && AnalysDefineIf(plexp, lexp);
		} else if(dname == "include"){

			if(!proc_active_code)
				return 1;

			CPPLEXP_PUSHSTR(CppXccFile*, this_file);

			ILink ilink;
			HLString ls;

			// Get file name
			CppLexpD *l = lexp;
			while(l){
				if(!(l->type == CXXLEXPT_OP && ( l->ext == MCCOP_MORE || l->ext == MCCOP_LESS) || l->type == CXXLEXPT_COMMENT))
					ls + l->val;
				l = l->_n;
			}

			ilink.Link(this_file->filename);
			TString path = DefIncludeGetPath(ilink, ls.String());
			
			// Analys
			int af = AnalysFile(path, 0);

			CPPLEXP_POPSTR(CppXccFile*, this_file);

			if(opt_flist){
				if(af == -1){
					proc_flist + "// NOT FOUND" "\r\n" + "#include <" + ls.String() + ">\r\n\r\n";
				}
				proc_flist + "\r\n// Return to: '" + this_file->filename + "'\r\n\r\n";
			}
		} else if(dname == "error"){
			if(!proc_active_code)
				return 1;

			SetError(HLString() + "#error " + lexp->val);

		} else if(dname == "pragma"){
			// ignore

			if(lexp && lexp->val == "once")
				this_file->pragma_once = 1;

		} else{
			SetLexpError(HLString() + "Unknown define: " + dname, lexp);
			return 0;
		}

		return 1;
	}

	TString DefIncludeGetPath(ILink &link, VString p){
		TString t;
		
		if(IsFile(p))
			return p;

		if(IsFile(t.Add(link.GetProtoDomainPath(), p)))
			return t;
		
		VString line = opt_ipath;
		while(line){
			VString l = PartLine(line, line, ";");

		if(IsFile(t.Add(l, "/", p)))
			return t;
		}

		return p;
	}

	int AnalysDefineDefine(CppLexpD *plexp, CppLexpD *lexp){
		if(!proc_active_code)
				return 1;

		VString name = lexp->val;
		CppLexpD *par, *val;
			
		if(*name.endu() == '(' && lexp->n() && lexp->n()->type == CXXLEXPT_UP){
			par = lexp->n();
			val = lexp->n()->n();
		} else{
			par = 0;
			val = lexp->n();
		}

		if(opt_print && MSVXCC_PRINT_MODDEFINES)
			print("#define ", lexp->val, "\r\n");
			
		SetDefine(name, par, val);

		return 1;
	}
	
	int AnalysDefineIf(CppLexpD *plexp, CppLexpD *lexp){
		proc_ifdef_level ++;

		if(!proc_active_code)
			return 1;

		if(AnalysDefineIf(lexp)){
			proc_active_code = 1;
		} else{
			proc_active_code = 0;
		}

		proc_ifdef_activate = proc_ifdef_level;

		return 1;
	}

	int AnalysDefineIfdef(CppLexpD *plexp, CppLexpD *lexp, int iftrue){
		proc_ifdef_level ++;

		if(!proc_active_code)
			return 1;

		CppXccDefine *def = GetDefine(lexp->val);
		if((def != 0) == iftrue){
			proc_active_code = 1;
		} else{
			proc_active_code = 0;
		}

		proc_ifdef_activate = proc_ifdef_level;

		// Replace
		if(opt_replace_enable && lexp->val == "MSVXCCTEMPLATE"){
			ReplaceBegin(lexp);
		}

		return 1;
	}

	int AnalysDefineElse(CppLexpD *plexp, CppLexpD *lexp){
		// proc_ifdef_level nomod;
		if(proc_ifdef_level == proc_ifdef_activate && !proc_active_code)
			proc_active_code = 1;
		else if(proc_active_code){
			proc_active_code = 0;
			proc_ifdef_activate = proc_ifdef_level;
		}

		if(proc_replace_do == proc_ifdef_level){
			ReplaceElse(plexp);
		}

		return 1;
	}

	int AnalysDefineEndif(CppLexpD *plexp, CppLexpD *lexp){
		if(proc_ifdef_level <= 0){
			SetLexpError("Not found #if for #endif", lexp);
			return 0;
		}

		if(proc_ifdef_level == proc_ifdef_activate && !proc_active_code)
			proc_active_code = 1;

		if(proc_replace_do == proc_ifdef_level){
			ReplaceEnd(plexp);
		}

		proc_ifdef_level --;

		if(proc_ifdef_dend && proc_ifdef_dend == proc_ifdef_level){
			proc_ifdef_dend = 0;
			return AnalysDefineEndif(plexp, lexp);
		}

		return 1;
	}

	int AnalysDefineIf(CppLexpD *lexp){
		int ret = AnalysDefineIfCheck(lexp);

		if(ret < 0)
			SetLexpError("#if bad value", lexp);

		return ret;
	}

	int AnalysDefineIfDef(CppLexpD *lexp){
		if(lexp->type == CXXLEXPT_NAME){
			CppXccDefine *def = GetDefine(lexp->val);
			if(def && def->value)
				return def->value->val;
			else
				return 0;
		} else if(lexp->type == CXXLEXPT_INT){
			return GetInt(lexp->val);
		} else
			return lexp->tmp;

		return 0;
	}

	int GetInt(VString val){
		if(val.incompare("0x"))
			return stoi(val.rchar() + 2, val.sz - 2, 16);
		else
			if(val.incompare("0"))
				return stoi(val, val, 8);
		return val.toi();
	}

	int AnalysDefineIfCheck(CppLexpD *plexp){
		for(int optype = 0; optype < 4; optype ++){
			CppLexpD *lexp = plexp;
			int tcount = 0, ret = -1;

			while(lexp){
				if(optype == 0){
					lexp->tmp = -1;

					if(lexp->type == CXXLEXPT_COMMENT)
						lexp->tmp = -2;
					

					if(lexp->_a){
						lexp->tmp = ret = AnalysDefineIfCheck(lexp->_a);
						if(ret < 0)
							return ret;
					}

					if(lexp->type == CXXLEXPT_NAME || lexp->type == CXXLEXPT_INT){
						if(lexp->val == "defined" && lexp->_n && lexp->_n->type == CXXLEXPT_UP && lexp->n()->a() && lexp->n()->a()->type == CXXLEXPT_NAME){
							CppXccDefine *def = GetDefine(lexp->n()->a()->val);
							lexp->tmp = ret = def != 0;
							lexp->_n->tmp = -2;

							tcount ++;
							lexp = lexp->n()->n();
							continue;
						} else{
							CppXccDefine *def = GetDefine(lexp->val);
							lexp->tmp = ret = def ? 1 : 0;
						}
					}
				}
				else if(optype == 1){
					if(lexp->tmp == -1 && lexp->type == CXXLEXPT_OP && lexp->ext == MCCOP_LNEG && lexp->_n && lexp->_n->tmp >= 0){
						lexp->tmp = ret = !lexp->_n->tmp;
						lexp->_n->tmp = -2;
					}
				} else if(optype == 2){
					if((lexp->type == CXXLEXPT_OP && lexp->tmp == -1) &&
					( lexp->ext == MCCOP_MORE || lexp->ext == MCCOP_LESS || lexp->ext == MCCOP_MOREC || lexp->ext == MCCOP_LESSC || lexp->ext == MCCOP_CMPC || lexp->ext == MCCOP_CMPNC)){
					
					ret = AnalysDefineIfCheckOp(lexp);
					if(ret < 0)
						return -1;

					tcount --;
					}
				} else if(optype == 3){
					if((lexp->type == CXXLEXPT_OP && lexp->tmp == -1) &&
					( lexp->ext == MCCOP_DAND || lexp->ext == MCCOP_DOR)){
					
					ret = AnalysDefineIfCheckOp(lexp);
					if(ret < 0)
						return -1;

					tcount --;
					}
				}

				if(lexp->tmp >= -1)
					tcount ++;

				lexp = lexp->_n;
			}

			if(tcount == 1 && ret >= 0)
				return ret;
		}

		return -1;

#ifdef IAM_YOU_CODE___NOOOOOOO
		while(lexp){

			// Clear state && count
			if(optype == 0){
				if(lexp->type == CXXLEXPT_NAME || lexp->type == CXXLEXPT_INT){
					if(lexp->val == "defined" && lexp->_n && lexp->_n->type == CXXLEXPT_UP && lexp->n()->a() && lexp->n()->a()->type == CXXLEXPT_NAME){
						CppXccDefine *def = GetDefine(lexp->n()->a()->val);
						lexp->tmp = ret = def != 0;
						lexp->_n->tmp = -2;

						count ++;
						tcount ++;
						lexp = lexp->n()->n();
						continue;
					} else{
						CppXccDefine *def = GetDefine(lexp->val);
						lexp->tmp = ret = def ? 1 : 0;
					}
				} else
					lexp->tmp = -1;
				count ++;
			}
			// !
			if(optype == 1){
				if(lexp->tmp == -1 && lexp->type == CXXLEXPT_OP && lexp->ext == MCCOP_LNEG && lexp->_n && lexp->_n->tmp >= 0){
					count --;
					lexp->tmp = ret = !lexp->_n->tmp;
					lexp->_n->tmp = -2;
				}
			}
			//  operations
			if(optype == 2){
				if((lexp->type == CXXLEXPT_OP && lexp->tmp == -1) &&
					( lexp->ext == MCCOP_MORE || lexp->ext == MCCOP_LESS || lexp->ext == MCCOP_MOREC || lexp->ext == MCCOP_LESSC || lexp->ext == MCCOP_CMPC || lexp->ext == MCCOP_CMPNC || lexp->ext == MCCOP_DAND || lexp->ext == MCCOP_DOR )){
					
				CppLexpD *l = lexp->_p, *r = lexp->_n;
				
				int64 ir, il;
				int ret;

				while(l){
					if(l->tmp >= 0)
						break;
					l = l->_p;
				}

				while(r){
					if(r->tmp >= 0)
						break;
					r = r->_n;
				}

				if(!l || !r){
					SetLexpError("#if bad operations", lexp);
					return 0;
				}

				il = AnalysDefineIfDef(l);
				ir = AnalysDefineIfDef(r);

				switch(lexp->ext){
					case MCCOP_MORE: ret = il > ir; break;
					case MCCOP_LESS: ret = il < ir; break;
					case MCCOP_MOREC: ret = il >= ir; break;
					case MCCOP_LESSC: ret = il <= ir; break;
					case MCCOP_CMPC: ret = il <= ir; break;
					case MCCOP_CMPNC: ret = il <= ir; break;
					case MCCOP_DAND: ret = il <= ir; break;
					case MCCOP_DOR: ret = il <= ir; break;
					default: SetLexpError("#if operaion not found", lexp); return 0; break;
				}

				count -= 2;
				l->tmp = -2;
				r->tmp = -2;
				lexp->tmp = ret;
				}
			}

			if(lexp->tmp >= -1)
				tcount ++;

			lexp = lexp->_n;
		}

		if(tcount == 1 && ret >= 0)
			return ret;

		return -1;
#endif
	}

	int AnalysDefineIfCheckOp(CppLexpD *lexp){
		CppLexpD *l = lexp->_p, *r = lexp->_n;
		int64 ir, il;
		int ret;

		while(l){
			if(l->tmp >= 0)
				break;
			l = l->_p;
		}

		while(r){
			if(r->tmp >= 0)
				break;
			r = r->_n;
		}

		if(!l || !r){
			SetLexpError("#if bad operations", lexp);
			return -1;
		}

		il = AnalysDefineIfDef(l);
		ir = AnalysDefineIfDef(r);

		switch(lexp->ext){
			case MCCOP_MORE: ret = il > ir; break;
			case MCCOP_LESS: ret = il < ir; break;
			case MCCOP_MOREC: ret = il >= ir; break;
			case MCCOP_LESSC: ret = il <= ir; break;
			case MCCOP_CMPC: ret = il == ir; break;
			case MCCOP_CMPNC: ret = il != ir; break;
			case MCCOP_DAND: ret = il && ir; break;
			case MCCOP_DOR: ret = il || ir; break;
			default: SetLexpError("#if operaion not found", lexp); return -1; break;
		}

		l->tmp = -2;
		r->tmp = -2;
		lexp->tmp = ret;

		return ret;
	}

	// Replace
	void ReplaceBegin(CppLexpD *lexp){
		proc_replace_do = proc_ifdef_level;
	}

	void ReplaceElse(CppLexpD *lexp){
		unsigned char *ln = this_file->filedata.data + this_file->modpos;
		unsigned char *to = lexp->val.uchar();
	
		this_file->moddata + VString(ln, to - ln) + "\r\n" + "#else";
	}

	void ReplaceEnd(CppLexpD *lexp){
		MTime mt;

		proc_replace_do = 0;
		this_file->moddata + "\r\n" +

		"// MSV CppXcc v." + CppXccVer[0].ver + " (" + CppXccVer[0].date + ") Do Not Write To This Block, All Data Well Be Rewrite On Update!" "\r\n" \
		"// ##MD5: " + md5h(this_file->modcode.String()) + " : " + mt.date("d.m.y H:i:s");

		this_file->moddata + "\r\n" + this_file->modcode + "\r\n";
		this_file->modcode.Clear();
		this_file->modpos = lexp->val.uchar() - this_file->filedata.data;
	}


	int SelReplaceCode(CppLexpD *lexp){
		HLString ls;
		int ret = ReplaceCodeIfDef(lexp, ls);
		
		if(ret != -1){
			if(proc_replace_do)
				this_file->modcode + ls;
			if(opt_flist)
				proc_flist + ls.String();

			return ret;
		}

		if(lexp->type == CXXLEXPT_COMMENT){
		} else if(lexp->type == CXXLEXPT_CODELINE){

			if(proc_replace_do)
				if(this_file->modcode.Size() && *this_file->modcode[this_file->modcode.Size() - 1] != '\n')
					this_file->modcode + "\r\n";

			if(opt_flist)
				if(proc_flist.Size() && *proc_flist[proc_flist.Size() - 1] != '\n')
					proc_flist + "\r\n";

		} else if(lexp->type == CXXLEXPT_TEXT){
			if(proc_replace_do)
				this_file->modcode + "\"" + lexp->val + "\"";
			if(opt_flist)
				proc_flist + + "\"" + lexp->val + "\"";
		} else if(lexp->type == CXXLEXPT_CHAR){
			if(proc_replace_do)
				this_file->modcode + "'" + lexp->val + "'";
			if(opt_flist)
				proc_flist + + "'" + lexp->val + "'";
		} else {
			if(proc_replace_do)
				this_file->modcode + " " + lexp->val;
			if(opt_flist)
				proc_flist + " " + lexp->val;
		}

		if(lexp->val == "GetCurrentThread")
			int rety  = 567;

		return 1;
	}

	void SelReplaceCodeClose(CppLexpD *lexp){
		VString add;

		if(lexp->type == CXXLEXPT_UP)
			add = " )";
		else if(lexp->type == CXXLEXPT_BLOCK)
			add = " }";
		else if(lexp->type == CXXLEXPT_ARR)
			add = " ]";

		if(add){
			if(proc_replace_do)
				this_file->modcode + add;
			if(opt_flist)
				proc_flist + add;
		}
	}

class CxxReplaceData{
public:
	CxxReplaceData *_n;
	CppXccDefine *def;
	CppLexpD *pars;

	int opt_nodef;
	int opt_nos;

	//int *opt_newline;

	// Temp
	HLString ls;

	CppLexpD* NewLexp(){
		return (CppLexpD*)ls.Add(0, sizeof(CppLexpD), 1);
	}

};

	int ReplaceCodeIfDef(CppLexpD *lexp, /*CppLexpD *lpar,*/ HLString &ls, CxxReplaceData *rep = 0){
		if(lexp->type == CXXLEXPT_NAME){
			CppXccDefine *def = GetDefine(lexp->val);

			// if ()
			if(def && (!def->param || lexp->_n && lexp->_n->type == CXXLEXPT_UP))
				return ReplaceCodeDef(lexp, lexp->_n, def, ls, rep);
		}

		return -1;
	}

	int ReplaceCodeIfDef(CppLexpD *lexp, CppLexpD *lpar, HLString &ls, CxxReplaceData *rep = 0){
		if(lexp->type == CXXLEXPT_NAME){
			CppXccDefine *def = GetDefine(lexp->val);

			// if ()
			if(def && (!def->param || lpar && lpar->type == CXXLEXPT_UP))
				return ReplaceCodeDef(lexp, lpar, def, ls, rep);
		}

		return -1;
	}

	CppLexpD* ReplaceCodeDefPar(CxxReplaceData &rep, CppLexpD *lexp){
		CppLexpD *plexp = lexp;

		while(lexp){
			if(lexp->type == CXXLEXPT_OP && lexp->ext == MCCOP_DEFSUB)
				break;
			//else if(lexp->type == CXXLEXPT_NAME)
			//	if(GetDefine(lexp->val))
			//		break;

			lexp = lexp->_n;
		}

		if(!lexp)
			return plexp;

		CppLexpD *npar = rep.NewLexp();
		npar->type = CXXLEXPT_COMMENT;
		
		ReplaceCodeDefParAdd(rep, plexp, npar);

		return npar;
	}

	CppLexpD* ReplaceCodeDefParAdd(CxxReplaceData &rep, CppLexpD *lexp, CppLexpD *par){
		CppLexpD *n;

		if(!par)
			return 0;

		while(lexp){
			if(lexp->type == CXXLEXPT_OP && lexp->ext == MCCOP_DEFSUB){
				lexp = lexp->_n;

				if(!lexp)
					break;

				CppLexpD *fr, *to;
				int f = rep._n ? ReplaceCodeDefRGetVal(*rep._n, lexp, fr, to) : 0;

				CppXccDefine *def = GetDefine(!f ? lexp->val : fr->val);
				if(def){
					par = ReplaceCodeDefParAdd(rep, def->value, par);
					lexp = lexp->_n;
					continue;
				}
			}
			//else if(lexp->type == CXXLEXPT_NAME){
			//	CppXccDefine *def = GetDefine(lexp->val);
			//	if(def){
			//		par = ReplaceCodeDefParAdd(rep, def->value, par);
			//		lexp = lexp->_n;
			//		continue;
			//	}
			//}

			n = rep.NewLexp();
			if(par)
				par->_n = n;

			n->val = lexp->val;
			n->type = lexp->type;
			n->ext = lexp->ext;
			n->_a = lexp->_a;
			n->_e = lexp->_e;

			par = n;
			lexp = lexp->_n;
		}

		return par;
	}

	int ReplaceCodeDef(CppLexpD *lexp, CppLexpD *lpar, CppXccDefine *def, HLString &ls, CxxReplaceData *rep = 0){
		CppLexpD *l = 0;
		//if(lexp->_n && lexp->_n->type == CXXLEXPT_UP){
		//	lpar = lexp->_n->_a;
		//}

		if(lpar && lpar->_a)
			lpar = lpar->_a;

		ls + " ";

		CxxReplaceData nrep;
		nrep.def = def;
		nrep._n = rep;
		nrep.opt_nodef = 0;
		nrep.opt_nos = 0;
//		nrep.opt_newline = 0;

		//int opt_newline = 0;
		//if(rep)
		//	nrep.opt_newline = rep->opt_newline;
		//else
		//	nrep.opt_newline = &opt_newline;

		lpar = ReplaceCodeDefPar(nrep, lpar);
		nrep.pars = lpar;
		
		
		ReplaceCodeDefR(nrep, ls, l, 0, def->value); // lpar -> 0,  def->param->a() -> 0

		//this_file->modcode + " " + ls;

		if(lpar)
			return 2;

		return 1;
	}

	int ReplaceCodeDefRGetVal(CxxReplaceData &prep, CppLexpD *lexp, CppLexpD *&fr, CppLexpD *&to){
		if(lexp->type != CXXLEXPT_NAME)
			return 0;
		if(!prep.def->param->a())
			return 0;
		
		fr = 0; to = 0;

		VString fval;
		CxxReplaceData *rep = &prep;

		while(rep){
			CppLexpD *d = rep->def->param->a(); // , *p = rep._n->def->param->a();
			int pos = 0, va = 0;
		
			if(lexp->val == "__VA_ARGS__"){
				fval = "...";
				va = 1;
			} else if(lexp->val == "__VA_ALL_ARGS__"){
				va = 2;
			} else
				fval = lexp->val;

			// find value in #def params
			while(d){
				if(d->val == fval)
					break;
				else if(d->type == CXXLEXPT_NVAL)
					pos ++;

				d = d->_n;
			}

			if(va == 2){
				pos = 0;
				d = lexp;
			}

			if(!d){
				if(fr)
					return 1;
				return 0;
			}

			CppLexpD *p = rep->pars;

			// if found
			if(pos == 0)
				fr = p;
			to = 0;

			while(p){
				if(p->type == CXXLEXPT_NVAL){
					pos --;
					if(pos == 0)
						fr = p->_n;
					else if(pos == -1 && !va){
						to = p;
						break;
					}
				}

				p = p->_n;
			}

			if(!rep->_n || !fr)
				return 1;

			if(fr->_n != to && fr->_n->_n == to && fr->type == CXXLEXPT_COMMENT){
				fr = fr->_n;
			}
			
			lexp = fr;
			rep = rep->_n;
		}

		return 1;
	}

	void ReplaceCodeDefR(CxxReplaceData &rep, HLString &ls, CppLexpD *&par, CppLexpD *dpar, CppLexpD *lexp, CppLexpD *tolexp = 0){
		//int nos = 0; // no space
		//int ist = 0; // is text
		CppLexpD *ist = 0; // is text
		CppLexpD *fr, *to;

		while(lexp != tolexp){
			int wr = 0;

			if(lexp->type == CXXLEXPT_NAME){
				// is param
				if(!rep.opt_nodef && ReplaceCodeDefRGetVal(rep, lexp, fr, to)){
					int old_opt_nodef = rep.opt_nodef;
					//rep.opt_nodef = 1;

					CppLexpD *npar = lexp->_n != tolexp && lexp->_n->type == CXXLEXPT_UP ? lexp->_n : 0;

					ReplaceCodeDefR(rep, ls, npar, dpar, fr, to); // par -> 0
					
					rep.opt_nodef = old_opt_nodef;
					wr = 1;

					if(npar){
						if(lexp->_n == tolexp)
							break;
						
						lexp = lexp->n()->n();
						continue;
					}
				}

				if(!wr){
					// is define
					int ret = ReplaceCodeIfDef(lexp, par ? par : (lexp->_n != tolexp && lexp->_n->type == CXXLEXPT_UP ? lexp->_n : 0), ls, &rep);
					if(ret == 2){
						lexp = lexp->_n;
						if(lexp == tolexp)
							break;
						lexp = lexp->_n;
						continue;
					} else if(ret == -1){
						if(par)
							par = 0;					
					} else
						wr = 1;
				}
			}

			if(!wr && lexp->type == CXXLEXPT_OP){
				if(lexp->ext == MCCOP_DEF){
					ls + " \"";
					ist = lexp;
					rep.opt_nos = 1;
					wr = 1;
				} else if(lexp->ext == MCCOP_DEFADD){
					rep.opt_nos = 1;
					wr = 1;
				}
				else if(lexp->ext == MCCOP_USEDEF){
					ls + "#";
					wr = 1;
					rep.opt_nos = 1;
				}
			}

			if(!wr)
				WriteLexp(ls, lexp, rep.opt_nos);

			if(lexp->_a){
				ReplaceCodeDefR(rep, ls, par, dpar, lexp->_a);

				if(lexp->type == CXXLEXPT_UP)
					ls + " " + ")";
			}

			if(ist && ist != lexp){
				ls + "\"";
				ist = 0;
			}

			lexp = lexp->_n;
		}

		if(ist)
			ls + "\"";

		return ;
	}

	void WriteLexp(HLString &ls, CppLexpD *lexp, int &nos){
		if(nos)
			nos = 0;
		else
			ls + " ";

		if(lexp->type == CXXLEXPT_COMMENT){
			//ls + "//" + lexp->val + "\r\n;
		}
		else if(lexp->type == CXXLEXPT_TEXT)
			ls + "\"" + lexp->val + "\"";
		else if(lexp->type == CXXLEXPT_OP && lexp->val == ":::::"){
			ls + "\r\n";
		}
		else
			ls + lexp->val;
	}

	// Files
	CppXccFile* GetFile(VString path){
		for(int i = 0; i < files.Size(); i++){
			if(files[i]->filename == path)
				return files[i];
		}
		return 0;
	}

	CppXccFile* AddFile(VString path){
		CppXccFile *fl = files.Add();
		fl->filename = path;
		return fl;
	}

	void CheckFiles(){
		for(int i = 0; i < files.Size(); i++){
			if(files[i]->modpos){
				if(opt_print && MSVXCC_PRINT_SAVEFILES){
					print("CppXcc Save file: '", files[i]->filename, ".new'\r\n");
				}

				files[i]->moddata + files[i]->filedata.str(files[i]->modpos);
				files[i]->modpos = files[i]->filedata;

				SaveFile(HLString() + files[i]->filename + ".new", files[i]->moddata.String());
			}			
		}

		return ;
	}

	void SaveFullListing(){
		if(!opt_flist)
			return ;

		SaveFile(opt_flist, proc_flist.String());
	}

	// Defines
	CppXccDefine* GetDefine(VString name){
		for(int i = 0; i < defines.Size(); i++){
			if(defines[i]->name == name)
				return defines[i];
		}
		return 0;
	}
	
	CppXccDefine * SetDefine(VString name, CppLexpD *par = 0, CppLexpD *val = 0){
		CppXccDefine *def = GetDefine(name);

		if(name == "__crt_typefix" || def && def->name == "__crt_typefix")
			int ert = 4564;

		if(!def){
			def = defines.Add();
			def->name = name;
		}

		def->param = par;
		def->value = val;

		return def;
	}

	int DelDefine(VString name){
		CppXccDefine *def = GetDefine(name);
		if(def)
			def->name.Clean();
		return def != 0;
	}

	// Errors
	VString GetError(){
		return error.String();
	}

	void SetError(VString line){
		// add error line to result
		error + "CPPXCC Error: '" + line + "' in '" + this_file->filename + "' \r\n";
		// stop
		//do_opt_stopit = 1;
		return ;
	}

	int GetFileLine(CppLexpD *lexp){
		if(!lexp || !lexp->val)
			return -1;

		unsigned char *ln = this_file->filedata, *to = lexp->val;
		int line = 1;

		while(ln < to){
			if(*ln == '\n')
				line ++;
			ln ++;
		}

		return line;
	}

	void SetLexpError(VString line, CppLexpD *lexp){
		// add error line to result
		error + "CPPXCC Error: '" + line + "' in '" + this_file->filename + "' on " + GetFileLine(lexp) + " line\r\n";
		// stop
		//do_opt_stopit = 1;
		return ;
	}
	
	// Clear
	void Clear(){
		files.Clear();
		error.Clear();
		warning.Clear();
		return ;
	}

};