// intercept malloc & free
// DELETED #define USEMSV_INTERCEPT_MALLOC_LOCK - use critical section
// #define USEMSV_INTERCEPT_MALLOC_DELETE - use operator delete. Set /FORCE:MULTIPLE to linker options
// #define USEMSV_INTERCEPT_MALLOC_PRINT 30 - print data, 30 sec
// #define USEMSV_INTERCEPT_MALLOC_CONSOLE - new console

#ifndef USEMSV_INTERCEPT
	#error Use: #define USEMSV_INTERCEPT
#endif

#ifdef NNOOOOOOOOOOOOOO

// malloc, heap, local, global, virt, virtex,
#define MSVCOREIM_LIST malloc, free, HeapAlloc, HeapFree, LocalAlloc, LocalFree, GlobalAlloc, GlobalFree, VirtualAlloc, VirtualFree, VirtualAllocEx, VirtualFreeEx
#define MSVCOREIM_MULTI(func, q, w, e, r, t, y, u, i, o, p, a, s) func(q) func(w) func(e) func(r) func(t) func(y) func(u) func(i) func(o) func(p) func(a) func(s)
#define MSVCOREIM_OT_MULTI(type, name, q, w, e, r, t, y, u, i, o, p, a, s) type name ## q, name ## w, name ## e, name ## r, name ## t, name ## y, name ## u, name ## i, name ## o, name ## p, name ## a, name ## s;

#ifdef MSVXCCTEMPLATE

#define MSVCORE_RN	"\r\n"
#define MSVCOREIM_DEFINE1(name, ret, ...) :#define MSVCOREIMF_ ## name ## _r ret
#define MSVCOREIM_DEFINE2(name, ret, ...) :#define MSVCOREIMF_ ## name ## _p __VA_ARGS__
#define MSVCOREIM_DEFINE(name, ret, ...) MSVCOREIM_DEFINE1(name, ret, __VA_ARGS__) ::::: MSVCOREIM_DEFINE2(name, ret, __VA_ARGS__) :::::

// Defines
#define MSVCOREIM_N ,
#define MSVCOREIM_DEFINE(name, ret, ...) :#define MSVCOREIM_ ## name ret MSVCOREIM_N __VA_ARGS__ ::::: typedef ret (WINAPI *MSVCOREC_ ## name)(__VA_ARGS__); ::::: ret _msvcore_my_ ## name (__VA_ARGS__); :::::
#define MSVCOREIM_CON(name) MSVCOREC_ ## name call_ ## name; //MSVCORE_ ## name call_ ## name;
#define MSVCOREIM_INJ(name) ::::: call_ ## name = (MSVCOREC_ ## name) InterceptFunction((unsigned char*)name, (unsigned char*)_msvcore_my_ ## name);

MSVCOREIM_DEFINE(malloc, void*, int sz)
MSVCOREIM_DEFINE(free, void, void *v)

MSVCOREIM_DEFINE(HeapAlloc, LPVOID, HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
MSVCOREIM_DEFINE(HeapFree, BOOL, HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)

MSVCOREIM_DEFINE(LocalAlloc, HLOCAL, UINT uFlags, SIZE_T uBytes)
MSVCOREIM_DEFINE(LocalFree, HLOCAL, HLOCAL hMem)

MSVCOREIM_DEFINE(GlobalAlloc, HGLOBAL, UINT uFlags, SIZE_T dwBytes)
MSVCOREIM_DEFINE(GlobalFree, HGLOBAL, HGLOBAL hMem)

MSVCOREIM_DEFINE(VirtualAlloc, LPVOID, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
MSVCOREIM_DEFINE(VirtualFree, BOOL, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)

MSVCOREIM_DEFINE(VirtualAllocEx, LPVOID, HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
MSVCOREIM_DEFINE(VirtualFreeEx, BOOL, HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)

// Constructor
MSVCOREIM_MULTI(MSVCOREIM_CON, ::MSVCOREIM_LIST);
MSVCOREIM_MULTI(MSVCOREIM_INJ, ::MSVCOREIM_LIST);
MSVCOREIM_OT_MULTI(int, count_, ::MSVCOREIM_LIST);



// Calls
//#define MSVCOREIM_TYPEDEF(name, ret, ...) typedef ret (WINAPI *MSVCORE_ ## name)(__VA_ARGS__); 

//MSVCOREIM_TYPEDEF(::MSVCOREIM_malloc)

	//MSVCOREIM_DEFINE(malloc, ::MSVCOREIMC_malloc);
//	MSVCOREIM_MULTI(MSVCOREIM_DEFINE, MSVCOREIM_malloc)
//	MSVCOREIM_MULTI(MSVCOREIM_DEFINE, MSVCOREIM_free)
	

	//MSVCOREIM_DEFINE2(malloc, ::MSVCOREIMC_malloc);
	//#define MSVCOREIM_CON(name) int call_ ## name; //MSVCORE_ ## name call_ ## name;
	//#define MSVCOREIM_DEFINE(ret, name, pars) #define MSVCOREIMF_ ## malloc ## _r ret
	//#define MSVCOREIMF_malloc_p int sz
	//MSVCOREIM_DEFINE(void, malloc, int sz);

#else
  #define MSVCOREIM_malloc void *  , int sz 
 typedef void * ( WINAPIV  * MSVCOREC_malloc ) ( int sz ) ; 
 void* _msvcore_my_malloc ( int sz );
 #define MSVCOREIM_free void  , void * v 
 typedef void (  WINAPIV * MSVCOREC_free ) ( void * v ) ; 
 void _msvcore_my_free ( void * v );
 #define MSVCOREIM_HeapAlloc LPVOID  , HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes 
 typedef LPVOID ( WINAPI  * MSVCOREC_HeapAlloc ) ( HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes ) ; 
 LPVOID _msvcore_my_HeapAlloc ( HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes );
 #define MSVCOREIM_HeapFree BOOL  , HANDLE hHeap , DWORD dwFlags , LPVOID lpMem 
 typedef BOOL ( WINAPI  * MSVCOREC_HeapFree ) ( HANDLE hHeap , DWORD dwFlags , LPVOID lpMem ) ; 
 BOOL _msvcore_my_HeapFree ( HANDLE hHeap , DWORD dwFlags , LPVOID lpMem );
 #define MSVCOREIM_LocalAlloc HLOCAL  , UINT uFlags , SIZE_T uBytes 
 typedef HLOCAL ( WINAPI  * MSVCOREC_LocalAlloc ) ( UINT uFlags , SIZE_T uBytes ) ; 
 HLOCAL _msvcore_my_LocalAlloc ( UINT uFlags , SIZE_T uBytes );
 #define MSVCOREIM_LocalFree HLOCAL  , HLOCAL hMem 
 typedef HLOCAL ( WINAPI  * MSVCOREC_LocalFree ) ( HLOCAL hMem ) ; 
 HLOCAL _msvcore_my_LocalFree ( HLOCAL hMem );
 #define MSVCOREIM_GlobalAlloc HGLOBAL  , UINT uFlags , SIZE_T dwBytes 
 typedef HGLOBAL ( WINAPI  * MSVCOREC_GlobalAlloc ) ( UINT uFlags , SIZE_T dwBytes ) ; 
 HGLOBAL _msvcore_my_GlobalAlloc ( UINT uFlags , SIZE_T dwBytes );
 #define MSVCOREIM_GlobalFree HGLOBAL  , HGLOBAL hMem 
 typedef HGLOBAL ( WINAPI  * MSVCOREC_GlobalFree ) ( HGLOBAL hMem ) ; 
 HGLOBAL _msvcore_my_GlobalFree ( HGLOBAL hMem );
 #define MSVCOREIM_VirtualAlloc LPVOID  , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect 
 typedef LPVOID ( WINAPI  * MSVCOREC_VirtualAlloc ) ( LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ) ; 
 LPVOID _msvcore_my_VirtualAlloc ( LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect );
 #define MSVCOREIM_VirtualFree BOOL  , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType 
 typedef BOOL ( WINAPI  * MSVCOREC_VirtualFree ) ( LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ) ; 
 BOOL _msvcore_my_VirtualFree ( LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType );
 #define MSVCOREIM_VirtualAllocEx LPVOID  , HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect 
 typedef LPVOID ( WINAPI  * MSVCOREC_VirtualAllocEx ) ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ) ; 
 LPVOID _msvcore_my_VirtualAllocEx ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect );
 #define MSVCOREIM_VirtualFreeEx BOOL  , HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType 
 typedef BOOL ( WINAPI  * MSVCOREC_VirtualFreeEx ) ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ) ; 
 BOOL _msvcore_my_VirtualFreeEx ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType );
    
#endif


#ifdef NOO
// Count
int _msvcore_int_malloc_count = 0;
int _msvcore_int_free_count = 0;
int _msvcore_int_hmalloc_count = 0;
int _msvcore_int_hfree_count = 0;
int _msvcore_int_vmalloc_count = 0;
int _msvcore_int_vfree_count = 0;
int _msvcore_int_emalloc_count = 0;
int _msvcore_int_efree_count = 0;

// Print Last Time
int _msvcore_lock_lprint = 0;

// Console
int _msvcore_lock_console = 0;

#ifdef USEMSV_INTERCEPT_MALLOC_DELETE
void * operator new(std::size_t sz) throw(std::bad_alloc){
	return malloc(sz);
}
void operator delete(void * p) throw(){
	return free(p);
}
#endif

 #define MSVCOREIM_malloc void *  , int sz 
 typedef void * (   * MSVCOREC_malloc ) ( int sz ) ; 
 void * _msvcore_my_malloc ( int sz ) { } 
 #define MSVCOREIM_free void  , void * v 
 typedef void (   * MSVCOREC_free ) ( void * v ) ; 
 void _msvcore_my_free ( void * v ) { } 
 #define MSVCOREIM_HeapAlloc LPVOID  , HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes 
 typedef LPVOID (   * MSVCOREC_HeapAlloc ) ( HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes ) ; 
 LPVOID _msvcore_my_HeapAlloc ( HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes ) { } 
 #define MSVCOREIM_HeapFree BOOL  , HANDLE hHeap , DWORD dwFlags , LPVOID lpMem 
 typedef BOOL (   * MSVCOREC_HeapFree ) ( HANDLE hHeap , DWORD dwFlags , LPVOID lpMem ) ; 
 BOOL _msvcore_my_HeapFree ( HANDLE hHeap , DWORD dwFlags , LPVOID lpMem ) { } 
 #define MSVCOREIM_LocalAlloc HLOCAL  , UINT uFlags , SIZE_T uBytes 
 typedef HLOCAL (   * MSVCOREC_LocalAlloc ) ( UINT uFlags , SIZE_T uBytes ) ; 
 HLOCAL _msvcore_my_LocalAlloc ( UINT uFlags , SIZE_T uBytes ) { } 
 #define MSVCOREIM_LocalFree HLOCAL  , HLOCAL hMem 
 typedef HLOCAL (   * MSVCOREC_LocalFree ) ( HLOCAL hMem ) ; 
 HLOCAL _msvcore_my_LocalFree ( HLOCAL hMem ) { } 
 #define MSVCOREIM_GlobalAlloc HGLOBAL  , UINT uFlags , SIZE_T dwBytes 
 typedef HGLOBAL (   * MSVCOREC_GlobalAlloc ) ( UINT uFlags , SIZE_T dwBytes ) ; 
 HGLOBAL _msvcore_my_GlobalAlloc ( UINT uFlags , SIZE_T dwBytes ) { } 
 #define MSVCOREIM_GlobalFree HGLOBAL  , HGLOBAL hMem 
 typedef HGLOBAL (   * MSVCOREC_GlobalFree ) ( HGLOBAL hMem ) ; 
 HGLOBAL _msvcore_my_GlobalFree ( HGLOBAL hMem ) { } 
 #define MSVCOREIM_VirtualAlloc LPVOID  , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect 
 typedef LPVOID (   * MSVCOREC_VirtualAlloc ) ( LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ) ; 
 LPVOID _msvcore_my_VirtualAlloc ( LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ) { } 
 #define MSVCOREIM_VirtualFree BOOL  , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType 
 typedef BOOL (   * MSVCOREC_VirtualFree ) ( LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ) ; 
 BOOL _msvcore_my_VirtualFree ( LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ) { } 
 #define MSVCOREIM_VirtualAllocEx LPVOID  , HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect 
 typedef LPVOID (   * MSVCOREC_VirtualAllocEx ) ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ) ; 
 LPVOID _msvcore_my_VirtualAllocEx ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ) { } 
 #define MSVCOREIM_VirtualFreeEx BOOL  , HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType 
 typedef BOOL (   * MSVCOREC_VirtualFreeEx ) ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ) ; 
 BOOL _msvcore_my_VirtualFreeEx ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ) { } 
   

// Malloc
void* _msvcore_my_malloc(int sz){
	UGLOCK(_msvcore_lock_mallock);
	_msvcore_int_malloc_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_malloc)(sz);
}

void* _msvcore_my_free(void *v){
	UGLOCK(_msvcore_lock_free);
	_msvcore_int_free_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_free)(v);
}

// Heap
LPVOID (WINAPI _msvcore_HeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes){
	UGLOCK(_msvcore_lock_mallock);

	//if(_msvcore_lock_mallock.Count() == 1){
		_msvcore_int_hmalloc_count++;
		_msvcore_my_malloc_print();
	//}

	return (_msvcore_int_hmalloc)(hHeap, dwFlags, dwBytes);
}

BOOL (WINAPI _msvcore_HeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem){
	UGLOCK(_msvcore_lock_free);
	_msvcore_int_hfree_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_hfree)(hHeap, dwFlags, lpMem);
}

// Virtual
LPVOID (WINAPI _msvcore_VirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect){
	UGLOCK(_msvcore_lock_mallock);
	_msvcore_int_vmalloc_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_vmalloc)(lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL (WINAPI _msvcore_VirtualFree)(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType){
	UGLOCK(_msvcore_lock_free);
	_msvcore_int_vfree_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_vfree)(lpAddress, dwSize, dwFreeType);
}

// Virtual Ex
LPVOID (WINAPI _msvcore_VirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect){
	UGLOCK(_msvcore_lock_mallock);
	_msvcore_int_emalloc_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_emalloc)(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL (WINAPI _msvcore_VirtualFreeEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType){
	UGLOCK(_msvcore_lock_free);
	_msvcore_int_efree_count++;
	_msvcore_my_malloc_print();

	return (_msvcore_int_efree)(hProcess, lpAddress, dwSize, dwFreeType);
}

#endif


	// Function Call
	MSVCOREC_malloc call_malloc ;
	MSVCOREC_free call_free ;
	MSVCOREC_HeapAlloc call_HeapAlloc ;
	MSVCOREC_HeapFree call_HeapFree ;
	MSVCOREC_LocalAlloc call_LocalAlloc ;
	MSVCOREC_LocalFree call_LocalFree ;
	MSVCOREC_GlobalAlloc call_GlobalAlloc ;
	MSVCOREC_GlobalFree call_GlobalFree ;
	MSVCOREC_VirtualAlloc call_VirtualAlloc ;
	MSVCOREC_VirtualFree call_VirtualFree ;
	MSVCOREC_VirtualAllocEx call_VirtualAllocEx ;
	MSVCOREC_VirtualFreeEx call_VirtualFreeEx ;   

	// Count
	int64 count_malloc , count_free , count_HeapAlloc , count_HeapFree , count_LocalAlloc , count_LocalFree , count_GlobalAlloc , count_GlobalFree , count_VirtualAlloc , count_VirtualFree , count_VirtualAllocEx , count_VirtualFreeEx ;

	//
	int lprint;

// Do
class _msvcore_init_malloc_do{ // : public TLock{

public:
	_msvcore_init_malloc_do(){
		//memset(this, 0, sizeof(*this));
		//UGLOCK(this);

		//call_malloc = ( MSVCOREC_malloc ) InterceptFunction ( ( unsigned char * ) malloc , ( unsigned char * ) _msvcore_my_malloc ) ;   
		//call_free = ( MSVCOREC_free ) InterceptFunction ( ( unsigned char * ) free , ( unsigned char * ) _msvcore_my_free ) ;   
		//call_HeapAlloc = ( MSVCOREC_HeapAlloc ) InterceptFunction ( ( unsigned char * ) HeapAlloc , ( unsigned char * ) _msvcore_my_HeapAlloc ) ;   
		//call_HeapFree = ( MSVCOREC_HeapFree ) InterceptFunction ( ( unsigned char * ) HeapFree , ( unsigned char * ) _msvcore_my_HeapFree ) ;   
		//call_LocalAlloc = ( MSVCOREC_LocalAlloc ) InterceptFunction ( ( unsigned char * ) LocalAlloc , ( unsigned char * ) _msvcore_my_LocalAlloc ) ;   
		//call_LocalFree = ( MSVCOREC_LocalFree ) InterceptFunction ( ( unsigned char * ) LocalFree , ( unsigned char * ) _msvcore_my_LocalFree ) ;   
		//call_GlobalAlloc = ( MSVCOREC_GlobalAlloc ) InterceptFunction ( ( unsigned char * ) GlobalAlloc , ( unsigned char * ) _msvcore_my_GlobalAlloc ) ;   
		//call_GlobalFree = ( MSVCOREC_GlobalFree ) InterceptFunction ( ( unsigned char * ) GlobalFree , ( unsigned char * ) _msvcore_my_GlobalFree ) ;   
		//call_VirtualAlloc = ( MSVCOREC_VirtualAlloc ) InterceptFunction ( ( unsigned char * ) VirtualAlloc , ( unsigned char * ) _msvcore_my_VirtualAlloc ) ;   
		//call_VirtualFree = ( MSVCOREC_VirtualFree ) InterceptFunction ( ( unsigned char * ) VirtualFree , ( unsigned char * ) _msvcore_my_VirtualFree ) ;   
		//call_VirtualAllocEx = ( MSVCOREC_VirtualAllocEx ) InterceptFunction ( ( unsigned char * ) VirtualAllocEx , ( unsigned char * ) _msvcore_my_VirtualAllocEx ) ;   
		//call_VirtualFreeEx = ( MSVCOREC_VirtualFreeEx ) InterceptFunction ( ( unsigned char * ) VirtualFreeEx , ( unsigned char * ) _msvcore_my_VirtualFreeEx ) ;

#ifdef USEMSV_INTERCEPT_MALLOC_CONSOLE
		 MsvAllocConsole();
#endif
	}

	/*
	~_msvcore_init_malloc_do(){
		_msvcore_my_malloc_print();
	}

void _msvcore_my_malloc_print(){
#ifdef USEMSV_INTERCEPT_MALLOC_PRINT
	if(lprint + USEMSV_INTERCEPT_MALLOC_PRINT > time(0))
		return ;

	lprint = time(0);

	Itos it;
	int upt = time(0) - _proc_upped;

#define MSVCOREIM_PRINT(name) print(it.Format("%s:	%d / %d / %d\r\n", #name, count_ ## name ## Alloc, count_ ## name ## Alloc - count_ ## name ## Free, count_ ## name ## Free));

	print("MSV Memory Control Result: (",  it.itos(upt), "s)\r\n");
	print(it.Format("Malloc: %d / %d / %d\r\n", count_malloc, count_malloc - count_free, count_free));
	MSVCOREIM_PRINT(Heap);
	MSVCOREIM_PRINT(Local);
	MSVCOREIM_PRINT(Global);
	MSVCOREIM_PRINT(Virtual);
	print(it.Format("Virtual: %d / %d / %d\r\n", count_VirtualAllocEx, count_VirtualAllocEx - count_VirtualFreeEx, count_VirtualFreeEx));
	print("\r\n");

#endif
	return ;
}
*/


#ifdef IAM_YOU_CODE___NOOOOOOO
	unsigned char* getpos(unsigned char* func){ // if jmp
		int i=7;

		// jmp?
		while(*func == 0xE9 && i){ i--;
			func = func + *(unsigned int*)(func + 1) + 5;
		}
		
		if(!i)
			return 0;

		return func;
	}

	void* intercept_it(void *func, void* mfunc){ // import table or any jmp.
		if(!func || !mfunc) return 0;
		void *ret = 0;

		if(*(unsigned char*)func!=0xE9 /*|| *(unsigned char*)mfunc!=0xE9*/) return 0; // not jmp

#ifdef WIN32
		unsigned long op;
		VirtualProtect(func, 5, PAGE_READWRITE, &op); // PAGE_READWRITE
#endif

		unsigned int m=(char*)mfunc-(char*)func-5;

		*(unsigned int*)func = *(unsigned int*)func;
		*(unsigned int*)&ret = *(unsigned int*)((char*)func+1);
		//memcpy(&ret, ((char*)func+1), sizeof(void*));
		*(unsigned int*)((char*)func+1) = *(unsigned int*)&m;
		//memcpy(((char*)func+1), &m, sizeof(m));

		ret=(char*)ret+(unsigned int)func+5;

#ifdef WIN32
		VirtualProtect(func, 5, op, &op);
#endif
		return ret;
	}

	void* intercept_hp(unsigned char *from, unsigned char* to){  // hot patching
		if(!from || !to)
			return 0;
		
		from = getpos(from);

		if(!from)
			return 0;

		if(!(*(from - 5) == 0x90 && *(from - 4) == 0x90 && *(from - 3) == 0x90 && *(from - 2) == 0x90 && *(from - 1) == 0x90 && *(from + 0) == 0x8B && *(from + 1) == 0xFF))
			return 0;

#ifdef WIN32
		unsigned long op;
		VirtualProtect(from - 5, 7, PAGE_READWRITE, &op); // PAGE_READWRITE
#endif

		//add E9 ED 32 07 00 + EB FF-2-7            jmp         71A9620E
		unsigned char *buf = from - 5;
		buf[0] = 0xE9; // jmp
		*(unsigned long*)&buf[1] = (unsigned long)to - (unsigned long)from; // to
		*(unsigned short*)&buf[5] = 0xF9EB; // jmp -7

#ifdef WIN32
		VirtualProtect(from - 5, 7, op, &op);
#endif
		return from + 2;
	}

#endif

}_msvcore_init_malloc_do;

#ifdef NOOOOOOOOOOOOOOOOO

_msvcore_init_malloc_do *_msvcore_im = new _msvcore_init_malloc_do;


#define MSVCOREIM_COUNT(name)
//UGLOCK(_msvcore_im); count_ ## name ++; _msvcore_im->_msvcore_my_malloc_print();

void* _msvcore_my_malloc(int sz){
	MSVCOREIM_COUNT(malloc);

	return call_malloc(sz);
}

void _msvcore_my_free(void *v){
	MSVCOREIM_COUNT(free);

	return call_free(v);
}

LPVOID _msvcore_my_HeapAlloc(HANDLE hHeap , DWORD dwFlags , SIZE_T dwBytes){
	MSVCOREIM_COUNT(HeapAlloc);

	return call_HeapAlloc(hHeap , dwFlags , dwBytes);
}

BOOL _msvcore_my_HeapFree ( HANDLE hHeap , DWORD dwFlags , LPVOID lpMem ){
	MSVCOREIM_COUNT(HeapFree);

	return call_HeapFree( hHeap , dwFlags , lpMem );
}

HLOCAL _msvcore_my_LocalAlloc ( UINT uFlags , SIZE_T uBytes ){
	MSVCOREIM_COUNT(LocalAlloc);

	return call_LocalAlloc(uFlags , uBytes);
}

HLOCAL _msvcore_my_LocalFree ( HLOCAL hMem ){
	MSVCOREIM_COUNT(LocalFree);

	return call_LocalFree(hMem);
}

HGLOBAL _msvcore_my_GlobalAlloc ( UINT uFlags , SIZE_T dwBytes ){
	MSVCOREIM_COUNT(GlobalAlloc);

	return call_GlobalAlloc( uFlags , dwBytes);
}

HGLOBAL _msvcore_my_GlobalFree ( HGLOBAL hMem ){
	MSVCOREIM_COUNT(GlobalFree);

	return call_GlobalFree(hMem);
}

LPVOID _msvcore_my_VirtualAlloc ( LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ){
	MSVCOREIM_COUNT(VirtualAlloc);

	return call_VirtualAlloc(lpAddress , dwSize , flAllocationType , flProtect);
}

BOOL _msvcore_my_VirtualFree ( LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ){
	MSVCOREIM_COUNT(VirtualFree);

	return call_VirtualFree(lpAddress , dwSize , dwFreeType );
}

LPVOID _msvcore_my_VirtualAllocEx ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ){
	MSVCOREIM_COUNT(VirtualAllocEx);

	return call_VirtualAllocEx(hProcess , lpAddress , dwSize , flAllocationType , flProtect);
}

BOOL _msvcore_my_VirtualFreeEx ( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ){
	MSVCOREIM_COUNT(VirtualFreeEx);

	return call_VirtualFreeEx(hProcess , lpAddress , dwSize , dwFreeType );
}
 







#endif
#endif

// typedef
typedef void * ( WINAPIV  *msvcore_im_malloc ) ( int sz ) ; 
typedef void ( WINAPIV *msvcore_im_free ) ( void * v ) ; 

typedef LPVOID (WINAPI *msvcore_im_HeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
typedef BOOL (WINAPI *msvcore_im_HeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
typedef HLOCAL (WINAPI  *msvcore_im_LocalAlloc)(UINT uFlags, SIZE_T uBytes) ; 
typedef HLOCAL (WINAPI  *msvcore_im_LocalFree)(HLOCAL hMem) ; 
typedef HGLOBAL (WINAPI  *msvcore_im_GlobalAlloc)(UINT uFlags , SIZE_T dwBytes) ; 
typedef HGLOBAL (WINAPI  *msvcore_im_GlobalFree)(HGLOBAL hMem) ; 
typedef LPVOID (WINAPI  *msvcore_im_VirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType , DWORD flProtect) ; 
typedef BOOL (WINAPI  *msvcore_im_VirtualFree)(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType) ; 
typedef LPVOID (WINAPI  *msvcore_im_VirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect) ; 
typedef BOOL (WINAPI  *msvcore_im_VirtualFreeEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType) ; 


// Data
TLock *msvcore_im_lock = new TLock;
int msvcore_im_lprint = 0;

msvcore_im_malloc msvcore_im_malloc_call = 0;
msvcore_im_free msvcore_im_free_call = 0;
msvcore_im_HeapAlloc msvcore_im_HeapAlloc_call = 0;
msvcore_im_HeapFree msvcore_im_HeapFree_call = 0;

msvcore_im_LocalAlloc msvcore_im_LocalAlloc_call = 0;
msvcore_im_LocalFree msvcore_im_LocalFree_call = 0;

msvcore_im_GlobalAlloc msvcore_im_GlobalAlloc_call = 0;
msvcore_im_GlobalFree msvcore_im_GlobalFree_call = 0;

msvcore_im_VirtualAlloc msvcore_im_VirtualAlloc_call = 0;
msvcore_im_VirtualFree msvcore_im_VirtualFree_call = 0;

msvcore_im_VirtualAllocEx msvcore_im_VirtualAllocEx_call = 0;
msvcore_im_VirtualFreeEx msvcore_im_VirtualFreeEx_call = 0;

int msvcore_im_malloc_count = 0;
int msvcore_im_free_count = 0;
int msvcore_im_HeapAlloc_count = 0;
int msvcore_im_HeapFree_count = 0;

int msvcore_im_LocalAlloc_count = 0;
int msvcore_im_LocalFree_count = 0;

int msvcore_im_GlobalAlloc_count = 0;
int msvcore_im_GlobalFree_count = 0;

int msvcore_im_VirtualAlloc_count = 0;
int msvcore_im_VirtualFree_count = 0;

int msvcore_im_VirtualAllocEx_count = 0;
int msvcore_im_VirtualFreeEx_count = 0;


// MSVCORE IM STACK ///
// Stack struct
struct msvcoreim_stack_item{
	unsigned int crc;
	unsigned int count;
	unsigned char buf[S1K];
	VString stack;
};

// Stack data
msvcoreim_stack_item *msvcoreim_stack_items = 0;
unsigned int msvcoreim_stack_itemsz = 0;
unsigned int msvcoreim_stack_itemasz = 0;

// Crc
unsigned int msvcoreim_stack_crc(){
	HANDLE process;
	void *stack[1024];
	unsigned short frames;
	unsigned int ret = 0;

	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = CaptureStackBackTrace(0, 100, stack, NULL);

	for(int i = 0; i < frames; i++)
		ret ^= (unsigned int)stack[i];

	return ret;
}

unsigned int msvcoreim_stack_crc_string(VString data, int ign = 0){
	HANDLE process;
	void *stack[1024];
	unsigned short frames;
	unsigned int ret = 0;
	unsigned char buf[sizeof(SYMBOL_INFO) + 256 * sizeof(char)];
	SYMBOL_INFO *symbol = (SYMBOL_INFO*)buf;

	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = CaptureStackBackTrace(0, 100, stack, NULL);

	for(int i = ign; i < frames; i++){
		SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
		//ret ^= (unsigned int)stack[i];
		if(data.sz != ret){
			int m = minel((data.sz - ret), strlen(symbol->Name));
			memcpy(data.data + ret, symbol->Name, m);
			ret += m;

			m = minel((data.sz - ret), 2);
			memcpy(data.data + ret, "\r\n", m);
			ret += m;

		} else
			break;
	}

	return ret;
}

// Get
msvcoreim_stack_item*  msvcoreim_stack_get(unsigned int crc){
	for(unsigned int i = 0; i < msvcoreim_stack_itemsz; i ++){
		if(msvcoreim_stack_items[i].crc == crc)
			return &msvcoreim_stack_items[i];
	}

	return 0;
}

// Set
msvcoreim_stack_item*  msvcoreim_stack_set(unsigned int crc){
	if(msvcoreim_stack_itemsz == msvcoreim_stack_itemasz){
		msvcoreim_stack_item *nitems = (msvcoreim_stack_item*)msvcore_im_malloc_call(sizeof(msvcoreim_stack_item) * (msvcoreim_stack_itemasz + S1K)); 
		memcpy(nitems, msvcoreim_stack_items, sizeof(msvcoreim_stack_item) * msvcoreim_stack_itemasz);
		
		Sleep(S8K);
		msvcore_im_free_call(msvcoreim_stack_items);
		msvcoreim_stack_items = nitems;
		msvcoreim_stack_itemasz += S1K;
	}

	msvcoreim_stack_items[msvcoreim_stack_itemsz].crc = crc;
	msvcoreim_stack_items[msvcoreim_stack_itemsz].count = 0;

	return &msvcoreim_stack_items[msvcoreim_stack_itemsz++];
}

void  msvcoreim_stack_print(int mincount = 0){
	print("Stack print:\r\n");
	SString ss;

	//UGLOCK(msvcore_im_lock);

	for(unsigned int i = 0; i < msvcoreim_stack_itemsz; i ++){
		if(msvcoreim_stack_items[i].count < mincount)
			continue;
		
		ss.Format("[%d] %d: %s\r\n", i, msvcoreim_stack_items[i].count, msvcoreim_stack_items[i].stack);
		print(ss);
	}

	return ;
}

// Alloc
void msvcoreim_stack_alloc(){
	unsigned int crc = msvcoreim_stack_crc();

	UGLOCK(msvcore_im_lock);
	msvcoreim_stack_item *item = msvcoreim_stack_get(crc);
	if(!item){
		item = msvcoreim_stack_set(crc);
		item->stack = VString(item->buf, sizeof(item->buf));
		item->stack.sz = msvcoreim_stack_crc_string(item->stack, 3);
	}

	item->count ++;

	return ;
}


#define MSVCOREIM_COUNT(name) msvcore_im_lock->Lock(); msvcore_im_ ## name ## _count ++; msvcore_im_lock->UnLock(); // msvcore_im_print();//  print("MCIM: ", #name, "\r\n");

void msvcore_im_print(){
#ifdef USEMSV_INTERCEPT_MALLOC_PRINT
	if(msvcore_im_lprint + USEMSV_INTERCEPT_MALLOC_PRINT > time(0))
		return ;

	msvcore_im_lprint = time(0);

	Itos it;
	int upt = time(0) - _proc_upped;

#define MSVCOREIM_PRINT(name) print(it.Format("%s:	%d / %d / %d\r\n", #name, msvcore_im_ ## name ## Alloc_count, msvcore_im_ ## name ## Alloc_count - msvcore_im_ ## name ## Free_count, msvcore_im_ ## name ## Free_count));


#ifdef USEMSV_INTERCEPT_MALLOC_STACKS_PRINT
	msvcoreim_stack_print();
#endif

	print("MSV Memory Control Result: (",  it.itos(upt), "s)\r\n");
	print(it.Format("Malloc: %d / %d / %d\r\n", msvcore_im_malloc_count, msvcore_im_malloc_count - msvcore_im_free_count, msvcore_im_free_count));
	MSVCOREIM_PRINT(Heap);
	MSVCOREIM_PRINT(Local);
	MSVCOREIM_PRINT(Global);
	MSVCOREIM_PRINT(Virtual);
	print(it.Format("Virtual: %d / %d / %d\r\n", msvcore_im_VirtualAllocEx_count, msvcore_im_VirtualAllocEx_count - msvcore_im_VirtualFreeEx_count, msvcore_im_VirtualFreeEx_count));
	print("\r\n");

#endif

	return ;
}

void* msvcore_im_my_malloc(int sz){
#ifndef USEMSV_INTERCEPT_MALLOC_STACKS
	MSVCOREIM_COUNT(malloc);
#else
	msvcore_im_lock->Lock();
	msvcore_im_malloc_count ++;
	msvcoreim_stack_alloc();
	msvcore_im_lock->UnLock();
#endif

	return (msvcore_im_malloc_call)(sz);
}

void msvcore_im_my_free( void * v ){
	MSVCOREIM_COUNT(free);

	return (msvcore_im_free_call)(v);
}

LPVOID WINAPI msvcore_im_my_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes){
	//MSVCOREIM_COUNT(HeapAlloc);
	msvcore_im_lock->Lock();
	if(dwFlags < S1K)
		msvcore_im_HeapAlloc_count ++;
	msvcore_im_lock->UnLock();

	return (msvcore_im_HeapAlloc_call)(hHeap, dwFlags, dwBytes);
}

BOOL WINAPI msvcore_im_my_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem){
	MSVCOREIM_COUNT(HeapFree);

	return (msvcore_im_HeapFree_call)(hHeap, dwFlags, lpMem);
}

HLOCAL msvcore_im_my_LocalAlloc( UINT uFlags , SIZE_T uBytes ){
	//MSVCOREIM_COUNT(LocalAlloc);

	return msvcore_im_LocalAlloc_call(uFlags , uBytes);
}

HLOCAL msvcore_im_my_LocalFree ( HLOCAL hMem ){
	MSVCOREIM_COUNT(LocalFree);

	return msvcore_im_LocalFree_call(hMem);
}

HGLOBAL msvcore_im_my_GlobalAlloc ( UINT uFlags , SIZE_T dwBytes ){
	MSVCOREIM_COUNT(GlobalAlloc);

	return msvcore_im_GlobalAlloc_call( uFlags , dwBytes);
}

HGLOBAL msvcore_im_my_GlobalFree ( HGLOBAL hMem ){
	MSVCOREIM_COUNT(GlobalFree);

	return msvcore_im_GlobalFree_call(hMem);
}

LPVOID msvcore_im_my_VirtualAlloc ( LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ){
	MSVCOREIM_COUNT(VirtualAlloc);

	return msvcore_im_VirtualAlloc_call(lpAddress , dwSize , flAllocationType , flProtect);
}

BOOL msvcore_im_my_VirtualFree( LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ){
	MSVCOREIM_COUNT(VirtualFree);

	return msvcore_im_VirtualFree_call(lpAddress , dwSize , dwFreeType );
}

LPVOID msvcore_im_my_VirtualAllocEx( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD flAllocationType , DWORD flProtect ){
	MSVCOREIM_COUNT(VirtualAllocEx);

	return msvcore_im_VirtualAllocEx_call(hProcess , lpAddress , dwSize , flAllocationType , flProtect);
}

BOOL msvcore_im_my_VirtualFreeEx( HANDLE hProcess , LPVOID lpAddress , SIZE_T dwSize , DWORD dwFreeType ){
	MSVCOREIM_COUNT(VirtualFreeEx);

	return msvcore_im_VirtualFreeEx_call(hProcess , lpAddress , dwSize , dwFreeType );
}

DWORD msvcore_im_thread(LPVOID){
	while(1){
		msvcore_im_print();


		Sleep(1000);
	}

	return 0;
}


class msvcore_im_con{

public:
	msvcore_im_con(){
		UGLOCK(msvcore_im_lock);
		time(0);

#define MSVCOREIM_INTFUNC(name) msvcore_im_ ## name ## _call = ( msvcore_im_ ## name ) InterceptFunction ( ( unsigned char * ) name , ( unsigned char * ) msvcore_im_my_ ## name ) ;   

		MSVCOREIM_INTFUNC(malloc);
		MSVCOREIM_INTFUNC(free);
		//MSVCOREIM_INTFUNC(HeapAlloc);
		//MSVCOREIM_INTFUNC(HeapFree);

		// Problem:
		//MSVCOREIM_INTFUNC(LocalAlloc);
		//MSVCOREIM_INTFUNC(LocalFree);
		//MSVCOREIM_INTFUNC(GlobalAlloc);
		//MSVCOREIM_INTFUNC(GlobalFree);
		//MSVCOREIM_INTFUNC(VirtualAlloc);
		//MSVCOREIM_INTFUNC(VirtualFree);
		//MSVCOREIM_INTFUNC(VirtualAllocEx);
		//MSVCOREIM_INTFUNC(VirtualFreeEx);

#ifdef USEMSV_INTERCEPT_MALLOC_CONSOLE
		 MsvAllocConsole();
#endif

#ifdef USEMSV_INTERCEPT_MALLOC_PRINT
		 StartThread(msvcore_im_thread);
#endif

#ifdef USEMSV_INTERCEPT_MALLOC_STACKS
		SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
#endif

	}

} msvcore_im_con;

