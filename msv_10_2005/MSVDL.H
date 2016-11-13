#define MSVDL_VER "0.0.0"; // MSV Double Line 
// channel 

////////////////////////////////
// NEW MATRIX 
class Tandex; // your class Index


// new class
class DMSVDL
{
public:
DMSVDL *_p,*_n;		// class data
int id;			// add your data
int act;
HWND hwnd;
ILine mess;
};


class MSVDL
{
public:
DMSVDL *_a, *_e;	// class cell
unsigned int _sz;
	MSVDL (){_a=0; _e=0; _sz=0;}	//new
	
	DMSVDL* insert(){ //insert
	DMSVDL*ind=(DMSVDL*)_minsert((MMatrixR*)&_a, (MMatrixD*)new DMSVDL); //
	//(XLine*)_minsertx((MMatrixX*)&_a, );
	//ind->func=func;
	return ind;
	}

	int Reg(int i, HWND hw){
		if(!Find(i)) {DMSVDL*dl=insert(); dl->id=i; dl->act=1; dl->hwnd=hw; return 1;}
	else return 0;
	}


	int SetWind(int i, HWND hw){
	DMSVDL*dl=Find(i);
	if(dl){ dl->hwnd=hw; return 1;}
	else return 0;
	}

	bool Stop(int i){
	DMSVDL*dl=Find(i);
	if(dl) {dl->act=0;}
	else return 0;
	}

	bool Run(int i){
	DMSVDL*dl=Find(i);
	if(dl) {dl->act=1;}
	else return 0;
	}

	int GetMess(int i, MString &ms, int &code){
	DMSVDL*dl=Find(i);
	if(dl){
		if(dl->mess.sz && dl->mess.a){
		ms=dl->mess.a->s;
		code=dl->mess.a->i;
		dl->mess.del(dl->mess.a);
		return 1;
		}
	}
	return 0;
	}

	int SendMess(int i, int code, MString ms){
	DMSVDL*dl=Find(i);
	if(dl){
		if(!dl->act) return 0;
	dl->mess.insert(ms, code);
	}

	return 0;
	}


	void del(DMSVDL * ind){ //delete
		_mdelete((MMatrixR*)&_a, (MMatrixD*)ind);
		return ;
	
	 }

	void clear(){ //delete
		_mdeleteall((MMatrixR*)&_a, (MMatrixD*)this->_a);
		return ;
	}

	DMSVDL* Find(int i){
	if(_sz==0)
    return 0;
	DMSVDL*ar=_a;
	while(ar){
	if (ar->id==i) return ar;
	ar=ar->_n;
	}
	return 0;
	}


	~MSVDL(){ clear();}
};

/////////////////////////





