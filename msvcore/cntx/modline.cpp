#define MODLINE_SAFE_SQL	1

#define SafeSQL(val) ModLine(val, MODLINE_SAFE_SQL)

class ModLine{
protected:
	unsigned char sdata[S2K];
	MString data;
	VString ret;

public:

	ModLine(ModLine &val){
		if(val.ret.data == val.sdata){
			memcpy(sdata, sdata, val.ret.sz);
			ret.data = sdata;
			ret.sz = val.ret.sz;
		} else{
			data = val.data;
			ret = data;
		}

		return ;
	}

	ModLine(VString line, int op){
		if(op == MODLINE_SAFE_SQL)
			ModLineSafeSQL(line);
		//else if(op == MODLINE_SAFE_CGI)
		//	ModLineSafeCGI(line);
	}

	ModLine& ModLineSafeSQL(VString line){
		unsigned char *ln = line, *lln = ln, *to = line.endu();
		int cnt = 0;

		for(ln; ln < to; ln++)
			if(*ln == '\'')
				cnt++;

		if(!Reserv(line.sz + cnt))
			return *this;

		unsigned char *tn = ret;
		ln = lln;

		for(ln; ln < to; ln++){
			if(*ln == '\''){
				memcpy(tn, lln, ln-lln);
				tn += ln-lln;
				*tn++ = '\\';
				lln = ln;
			}
		}

		if(lln != ln){
			memcpy(tn, lln, ln-lln);
		}

		return *this;
	}

	int Reserv(int sz){
		if(sz <= sizeof(sdata))
			ret.setu(sdata, sz);
		else
			ret = data.Reserv(sz);

		if(ret.sz != sz)
			return 0;

		return 1;		
	}

	static int TestText(VString text){
		unsigned char *ln = text, *to = text.endu();
		while(ln < to){
			if(ln < ' ')
				return 0;
			ln ++;
		}

		return 1;
	}

	operator VString(){
		return ret;
	}

};