class msl_five_func_call{
public:
	// call
	VString name;

	// params
	msl_value **vals;
	int valsz;

	msl_value *ret;

	// out
	HLString0 *out_text;

	// Functions
	void SetOutput(VString line){
		*out_text + line;
	}

};


typedef int (*MSLFIVE_CALL)(msl_five_func_call &opt);

class msl_five_functions_d{
public:
	msl_five_functions_d *_p, *_n;
	VString name;
	MSLFIVE_CALL func;
	int valsz, valsz2;	
};

class msl_five_functions : OMatrixT<msl_five_functions_d>{
	HLString0 data;

public:
	msl_five_functions(){
	}

	void Add(VString name, MSLFIVE_CALL func, int valsz, int valsz2){
		msl_five_functions_d *p = (msl_five_functions_d*)data.Add(0, sizeof(msl_five_functions_d), 1);

		p->name = name;
		p->func = func;
		p->valsz = valsz;
		p->valsz2 = valsz2;

		OMAdd(p);

		return ;
	}

	msl_five_functions_d* Get(VString name, int valsz){
		msl_five_functions_d *p = _a;

		while(p){
			if(p->name == name && p->valsz <= valsz && p->valsz2 >= valsz)
				return p;

			p = p->_n;
		}

		return 0;
	}

} msl_five_functions_list;

class msl_five_functions_add{
public:
	msl_five_functions_add(VString name, MSLFIVE_CALL func, int valsz, int valsz2){
		msl_five_functions_list.Add(name, func, valsz, valsz2);
	}
};


int msl_print(msl_five_func_call &opt){
	for(int i = 0; i < opt.valsz; i++){
		opt.SetOutput(opt.vals[i]->val);
	}

	return 1;
}

#define MSLFIVE_REGFUNC(name, func, valsz, valsz2) msl_five_functions_add mffa_ ## func(name, func, valsz, valsz2);

MSLFIVE_REGFUNC("print", msl_print, 1, 999);