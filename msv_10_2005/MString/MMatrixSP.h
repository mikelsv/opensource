// MMatrix SPecific


////////////////////////////////	NEW STANDART
// NEW MATRIX Y-format
class TRINT; // your class Index


// new class
class DTRINT
{
public:
DTRINT *_p,*_n;		// class data
int i;			// add your data
int di;
int ti;
};


class TRINT
{
public:
DTRINT *_a, *_e;	// class cell
unsigned int _sz;
	TRINT (){_a=0; _e=0; _sz=0;}	//new
	
	DTRINT* insert(){ //insert null
	return (DTRINT*)_minsert((MMatrixR*)&_a, (MMatrixD*)new DTRINT);
	}

	DTRINT* insert(int i, int di, int ti){ //insert	witch param
	DTRINT*ind=(DTRINT*)_minsert((MMatrixR*)&_a, (MMatrixD*)new DTRINT);
	ind->i=i; ind->di=di; ind->ti=ti;
	return ind;
	}

	void del(DTRINT * ind){ //delete
		_mdelete((MMatrixR*)&_a, (MMatrixD*)ind);
		return ;
	
	 }

	void clear(){ //delete
		_mdeleteall((MMatrixR*)&_a, (MMatrixD*)this->_a);
		return ;
	}

	DTRINT* Find(int i){
	if(_sz==0) return 0;
	DTRINT*ar=_a;
	while(ar){
	if (ar->i==i) return ar;
	ar=ar->_n;
	}
	return 0;
	}
	
	int dtake(){
	if (_a){
	int ms=_a->i;
	del(_a);
	return ms;
	}
	return int();
	}

	~TRINT(){ clear();}
};

/////////////////////////
