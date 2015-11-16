// Auto List
// 11.11.2015 12:50

enum AListOpt{ AListCon = 1, AListDes = 2, AListClear = 4 };

// Malloc / Free
template<class T, int OT = AListCon | AListDes | AListClear>
class AListAllocDef{

public:
	AListAllocDef(){
	}

	T* AllocNew(){
		T *t = (T*)malloc(sizeof(T));

		if(OT & AListCon)
			new(t)T;
		else
			memset(t, 0, sizeof(T));

		return t;
	}

	void AllocDel(T *t){
		if(OT & AListDes)
			t->~T();
		free(t);
		return ;
	}

};

//#ifdef USEMSV_ULIST
// UList
template<class T, int OT = AListCon | AListDes | AListClear>
class AListAllocUList{
	UList<T, OT & 3, 1> list;

public:
	AListAllocUList(){
	}

	T* AllocNew(){
		T *t = list.Add();
		return t;
	}

	void AllocFree(T *t){
		//list.Del(t);
		return ;
	}

};

template<class T, int OT = 0>
class AListAllocHLString{
	HLString0 list;

public:
	AListAllocHLString(){
	}

	T* AllocNew(){
		T *t = (T*)list.Add(0, sizeof(T), 1);
		if(OT & AListCon)
			new(t)T;
		return t;
	}

	void AllocFree(T *t){
		//list.Del(t);
		return ;
	}
};

//#endif

#ifdef USEMSV_OLIST
// OList
template<class T, int OT = AListCon | AListDes | AListClear>
class AListAllocOList{
	OList<T, OT & 3> list;

public:
	AListAllocOList(){
	}

	T* AllocNew(){
		T *t = list.New();
		return t;
	}

	void AllocFree(T *t){
		list.Del(t);
		return ;
	}

};
#endif


// AutoList 1d
template<class TAlloc, class T, int OT = AListCon | AListDes | AListClear>
class AList : public TAlloc, public OMatrixT<T>{

public:
	// Constructor
	AList(){

	}

	~AList(){
		Clear();
	}

	// Memory operations
	T* New(){
		T *t = this->AllocNew();
		OMAdd(t);
		return t;
	}

	void Free(T *t){
		OMDel(t);
		return this->AllocFree(t);
	}

	// Get
	T* GetFirst(){
		return this->_a;
	}

	OMatrixT<T>* GetThis(){
		return this;
	}

	T* GetLast(){
		return this->_e;
	}

	// Next
	T* Next(T *t){
		if(!t)
			return this->_a;

		return t->_n;
	}

	// Find
	template<typename TN>
	T* Find(TN &val){
			T *t = this->_a;

		while(t){
			if(t->FindFound(val))
				return t;

			t = t->_n;
			
		}

		return 0;
	}

	// Clear
	void Clear(){
		T *t = this->_a, *d = t;

		while(t){
			t = t->_n;
			this->Free(d);
		}

		return ;
	}
};

// AutoList 2d
template<class TAlloc, class T, int OT = AListCon | AListDes | AListClear>
class AList2d : public TAlloc, public OMatrixT<T>{

public:
	// Constructor
	AList2d(){

	}

	~AList2d(){
		Clear();
	}

	// Memory operations
	T* New(T *parent = 0){
		T *t = this->AllocNew();
		if(!parent)
			OMAdd(t);
		else
			OMAddP(parent->_a, parent->_e, t);
		return t;
	}

	void Free(T *parent, T *t){
		OMDelP(parent->_a, parent->_e, t);
		return this->AllocFree(t);
	}

	// Get
	T* GetFirst(){
		return this->_a;
	}

	OMatrixT<T>* GetThis(){
		return this;
	}

	T* GetLast(){
		return this->_e;
	}

	// Next
	T* Next(T *t){
		if(!t)
			return this->_a;

		return t->_n;
	}

	// Find
	//template<class F, typedef N>
	//T* Find(F &val){
	//		T *t = this->_a;

	//	while(t){
	//		if(t->N == val)
	//			return t;

	//		t = t->_n;
	//		
	//	}

	//	return 0;
	//}

	// Clear
	void Clear(T *parent = 0){
		T *t = !parent ? this->_a : parent->_a, *d = t;

		while(t){
			if(t->_a)
				Clear(t);
			t = t->_n;
			this->Free(parent, d);
		}

		return ;
	}
};



class TestAList{
public:
	TestAList * _p, *_n;

	int i;
};


#ifdef MSVXCCTEMPLATE



#else


#endif



//AList<AListAllocUList<TestAList>, TestAList> testalist;
//
//class A{
//
//public:
//	A(){
//
//		testalist.New()->i = 1;
//
//		int i = 1;
////		TestAList *t = testalist.Find<int, i>(i);
//
//
//
//	}
//
//} aaa;




