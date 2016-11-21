// Interception Call. Only Win32, sorry.

#ifndef WIN32
#include <sys/mman.h>
#endif

#ifdef WIN32

int InterceptFunctionErr=0;

#define INTFUNCERR_NO_HEADER	1
#define INTFUNCERR_NO_IDATA		2
#define INTFUNCERR_NO_FUNC		3
#define INTFUNCERR_NO_FIDATA	4
#define INTFUNCERR_NO_WRITE		5

// Эта функция ищет в таблице импорта - .idata нужный адрес и меняет на адрес процедуры-двойника
// Модифицированная версия с http://www.rsdn.ru/article/baseserv/IntercetionAPI.xml

// Возвратит адрес для вызова в новой функции.
// или 0 и ошибку в InterceptFunctionErr.
void* InterceptFunction(char *module, char *name, void *call){
	// Начало отображения в памяти процесса
	BYTE *pimage = (BYTE*)GetModuleHandle(NULL); 
	BYTE *pidata;

	// Стандартные структуры описания PE заголовка
	IMAGE_DOS_HEADER *idh;
	IMAGE_OPTIONAL_HEADER *ioh;
	IMAGE_SECTION_HEADER *ish;
	IMAGE_IMPORT_DESCRIPTOR *iid;
	DWORD *isd;  //image_thunk_data dword

	// Получаем указатели на стандартные структуры данных PE заголовка
	idh = (IMAGE_DOS_HEADER*)pimage;
	ioh = (IMAGE_OPTIONAL_HEADER*)(pimage + idh->e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER));
	ish = (IMAGE_SECTION_HEADER*)((BYTE*)ioh + sizeof(IMAGE_OPTIONAL_HEADER));

	//если не обнаружен магический код, то у этой программы нет PE заголовка
	if(idh->e_magic!=0x5A4D){ InterceptFunctionErr=INTFUNCERR_NO_HEADER; return 0; }

	//ищем секцию .idata
	int i;
	for(i=0; i<16; i++){
		if(strcmp((char*)((ish+ i)->Name) , ".idata") == 0) break;
	}

	if(i==16){ InterceptFunctionErr=INTFUNCERR_NO_IDATA; return 0; }

	// Получаем адрес секции .idata(первого элемента IMAGE_IMPORT_DESCRIPTOR)
	iid = (IMAGE_IMPORT_DESCRIPTOR*)(pimage + (ish +i)->VirtualAddress );
  
	// Получаем абсолютный адрес функции для перехвата
	DWORD func = (DWORD)GetProcAddress(GetModuleHandleA(module), name);
	if(!func){ InterceptFunctionErr=INTFUNCERR_NO_FUNC; return 0; }

	// В таблице импорта ищем соответствующий элемент для 
	// библиотеки user32.dll -> module
	while(iid->Name){  //до тех пор пока поле структуры не содержит 0
		if(strcmp((char*)(pimage + iid->Name), module) ==0 ) break;
		iid++;
	}

	// Ищем в IMAGE_THUNK_DATA нужный адрес
	isd = (DWORD*)(pimage + iid->FirstThunk);
	while(*isd!=func && *isd!=0)  isd++;
	if(*isd == 0){ InterceptFunctionErr=INTFUNCERR_NO_FIDATA; return 0; }
  
	// Заменяем адрес на свою функцию  
	DWORD buf = (DWORD)&call;
	DWORD op, written=0;
  
	// Обычно страницы в этой области недоступны для записи
	// поэтому принудительно разрешаем запись
	VirtualProtect((void*)(isd), 4, PAGE_READWRITE, &op);
  
	// Пишем новый адрес
	WriteProcessMemory(GetCurrentProcess(), (void*)(isd), (void*)&buf, 4, &written);

	//восстанавливаем первоначальную защиту области по записи
	VirtualProtect((void*)(isd), 4, op, &op);

	//если записать не удалось – увы, все пошло прахом…
	if(written!=4){ InterceptFunctionErr=INTFUNCERR_NO_WRITE; return 0; }

	return (void*)func;
}

#endif

/////////////////////////////////////////////////////////////////////////////////////
unsigned char* GetFunctionPos(unsigned char* func){ // if jmp
	int i=7;

	// jmp?
	while(*func==0xE9 && i){ i--;
		func=func+*(unsigned int*)(func+1)+5;
	} if(!i) return 0;

	return func;
}


#define GETFUNCSZ_CC	1
#define GETFUNCSZ_FULL	2

/////////////////////////////////////////////////////////////////////////////////////
unsigned char* GetFunctionSize(void * func, unsigned int &sz, unsigned int &opt){
	int i=7; unsigned int maxsz=128*1024; unsigned int inopt=opt; opt=0;

	// jmp?
	while(*(unsigned char*)func==0xE9 && i){ i--;
		func=(char*)func+*(unsigned int*)((char*)func+1)+5;
	} if(!i) return 0;

	unsigned char *f=(unsigned char*)func, *opcode;
	int fs=0;

	asmlexp asml;
#ifdef GENERALCPP
	print("GetFunctionSize: 0x", itos((unsigned int)f, 16), "\r\n");
#endif

//	VString mem(f, 1024);
//	SaveFile("_mem.bin", mem);
	X86_INST x; int r=0; unsigned long fb=(unsigned long)f, ft=0;//, fts=0;

	while(f-(unsigned char*)func<maxsz){
		int s=SizeOfCode(f, x); 
		//int s=asml.next(f, f+10);
#ifdef GENERALCPP
		if(!s){ print("Fail: ", itos(*x.op), "\r\n"); exit(0); }
		//else print("ASM 0x", itos(*x.op, 16), " ", itos(s), "b\r\n");
#else 
		if(!s)
			exit(0);
#endif

		// return
		if((*x.op==0xC3 || *x.op==0xC2) && !r){
			r=1; if(ft<x.offset_pos+s){
				if(!ft){ ft=(unsigned long)f+s; break; }
				ft=(unsigned long)f+s; print("ret: 0x", itos(ft, 16), "\r\n");
			}
		}
		if(*x.op==0xCC){
			if((unsigned long)f>=ft){ break; }
			opt|=GETFUNCSZ_CC;
		}

		if(x.is_rjmp /*&& (inopt&GETFUNCSZ_FULL || x.offset_size<4)*/){
			if(x.offset_pos>=(unsigned long)func && x.offset_pos<(unsigned long)f /*&& ft<x.offset_pos*/){
				ft=(unsigned long)f+s; print("jmp: up 0x", itos(ft, 16), "\r\n");
			}

		if(x.offset_pos>=(unsigned long)func && x.offset_pos<(unsigned long)f+256 && ft<x.offset_pos){
			ft=x.offset_pos; print("jmp: down 0x", itos(ft, 16), "\r\n");
		}

			//if(ft<x.offset_pos){
			//	ft=x.offset_pos; print("jmp: 0x", itos64(ft, 16), "\r\n");
			//}
			//if(x.offset_size<4 && fts<x.offset_pos) fts=x.offset_pos+s;
		}

		//if(x.is_rj){
		//	if(ft<x.offset_pos) ft=x.offset_pos;
		//}

		f+=s;

		if(r && ft+128<=(unsigned long)f) break;

		//if(*f==0xCC){ fs++;
		//	if((unsigned long)f%8==7) break;
		//}else fs=0;
		//f++;
	}

//#ifdef WIN32
//	VirtualProtect(func, maxsz, op, &op);
//#endif

	if(f-(unsigned char*)func==maxsz) return 0;

	//sz=f-(unsigned char*)func;
	sz=ft-(unsigned long)func;

	return (unsigned char*)func;
}

/////////////////////////////////////////////////////////////////////////////////////
int InterceptFunctionUpdate(unsigned char* func, unsigned int sz, unsigned char* real){// mfunc - func
	if(!func || !sz) return 0;

	X86_INST x; unsigned char *f=func, *ft=func+sz;
	while(f<ft){
		int s=SizeOfCode(f, x);

		if(x.is_rjmp){
			if((unsigned long)func<=x.offset_pos && (unsigned long)ft>x.offset_pos){// in
				// no mov
			}else{
				if(x.offset_size==4){ *(unsigned long*)(x.op+1)=*(unsigned long*)(x.op+1)-(unsigned long)func+(unsigned long)real; }
				else return 0;
			}
		}
		if(x.is_rcall){
			if((unsigned long)func<x.offset_pos && (unsigned long)ft>x.offset_pos){// in
				// no mov
			}else{
				if(x.offset_size==4){ *(unsigned long*)(x.op+1)=*(unsigned long*)(x.op+1)-(unsigned long)func+(unsigned long)real; }
				else return 0;
			}
		}
		f+=s;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
void* InterceptFunctionIT(void *func, void* mfunc){ // import table or any jmp.
	if(!func || !mfunc) return 0;
	void *ret;

	if(*(unsigned char*)func!=0xE9 /*|| *(unsigned char*)mfunc!=0xE9*/) return 0; // not jmp

#ifdef WIN32
	DWORD op;
	VirtualProtect(func, 4, PAGE_EXECUTE_READWRITE, &op);
#endif

	unsigned int a=*(unsigned int*)((char*)func+1);
	void* call=(char*)func+a;
	unsigned int m=(char*)mfunc-(char*)func-5;

	memcpy(&ret, ((char*)func+1), sizeof(void*));
	memcpy(((char*)func+1), &m, sizeof(m));

	ret=(char*)ret+(unsigned int)func+5;

#ifdef WIN32
	VirtualProtect(func, 4, op, &op);
#endif
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////
void* InterceptFunctionHP(unsigned char *func, unsigned char* mfunc){ // hot patching
	if(!func || !mfunc)
		return 0;

	void *ret;

	func=GetFunctionPos(func);
	if(!func)
		return 0; // not jmp

	if(!(*(func-5)==0x90 && *(func-4)==0x90 && *(func-3)==0x90 && *(func-2)==0x90 && *(func-1)==0x90
		&& *(func+0)==0x8B && *(func+1)==0xFF))
		return 0;


#ifdef WIN32
	DWORD op;
	VirtualProtect(func - 5, 7, PAGE_EXECUTE_READWRITE, &op); // PAGE_READWRITE
#endif

	//add E9 ED 32 07 00 + EB FF-2-7            jmp         71A9620E
	unsigned char *buf = func - 5;
	buf[0] = 0xE9; // jmp
	*(unsigned long*)&buf[1] = (unsigned long)mfunc - (unsigned long)func; // to
	*(unsigned short*)&buf[5] = 0xF9EB; // jmp -7

#ifdef WIN32
	VirtualProtect(func - 5, 7, op, &op);
#endif
	return func + 2;
}

/////////////////////////////////////////////////////////////////////////////////////
void* InterceptFunctionCopy(void *func, void* mfunc){
	if(!func || !mfunc) return 0;
	void *ret;

	// get void & size
	unsigned int fs, ms, opt=GETFUNCSZ_FULL;
	void *f=GetFunctionSize(func, fs, opt); if(!f || opt) return 0; // debug flag found
	//void *m=GetFunctionSize(mfunc, ms, opt);

#ifdef GENERALCPP
	print("Func: 0x", itos((unsigned int)f, 16), "\r\n");
	print("Sizes: ", itos(fs), "\r\n");
#endif
	
	// allocate & copy
#ifdef WIN32
	unsigned char* mem =(unsigned char*) VirtualAlloc(NULL, fs, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	unsigned char* mem =(unsigned char*) malloc(fs);
	mprotect(mem-(unsigned long)mem%4096, fs, PROT_READ|PROT_WRITE|PROT_EXEC);
#endif
	memcpy(mem, f, fs);
	//memset(mem, 0, fs);
	//WriteProcessMemory(processHandle, lpProc, InjectionMain, ProcSize, &dwWritten ) == 0){

	int r=InterceptFunctionUpdate(mem, fs, (unsigned char*)f);

	//asmlexp asml; unsigned char* ln=mem, *to=ln+fs;
	//while(ln<to){
	//	int s=asml.next(ln, to);

	//	if(*ln==0xE8 || *ln==0xE9){
	//		unsigned char *p=(unsigned char*)func+*(unsigned int*)(ln+1)+5+(ln-mem);
	//		*(unsigned int*)(ln+1)=p-ln-5;
	//		//*(unsigned int*)(ln+1)=*(unsigned int*)(ln+1)+(mem-(unsigned char*)func);
	//	}
	//	ln+=s;
	//}

	if(!r) return 0;
	
#ifdef WIN32
	DWORD op;
	VirtualProtect(f, 6, PAGE_READWRITE, &op);
#endif

	unsigned char jmp[6];
	jmp[0]=0x68; *(unsigned int*)&jmp[1]=(unsigned int)mfunc; jmp[5]=0xC3;
	memcpy(f, jmp, 6);

#ifdef WIN32
	VirtualProtect(f, 6, op, &op);
#endif

	return mem;
}

/////////////////////////////////////////////////////////////////////////////////////
void* InterceptFunctionDJ(void *func, void* mfunc){ // Double Jump
	if(!func || !mfunc) return 0;
	void *ret;

	// get void & size
	unsigned int fs, ms, opt=0;
	unsigned char *f=GetFunctionSize(func, fs, opt); if(!f || opt) return 0; // debug flag found
	//unsigned char *m=GetFunctionSize(mfunc, ms, opt);

#ifdef GENERALCPP
	print("Func: 0x", itos((unsigned int)f, 16), "\r\n");
	print("Sizes: ", itos(fs), "\r\n");
#endif

	asmlexp asml;
	unsigned char* ln=f, *to=ln+fs;
	//unsigned char *opcode;

	X86_INST x;

	while(ln<to){
		int s=SizeOfCode(ln, x);
		//int s=asml.next(ln, to);

//		if(x.is_rcall){ return 0; } // call
//		if(x.is_rjmp){ return 0; } // jmp
		if(*x.op==0xCC){ return 0; } // debugger

		//if(*opcode==0xE8 || *opcode==0xFF){ return 0; } // call
		//if(*opcode==0xE8) return 0; // jmp
		//if(*opcode==0xCC){ return 0; } // debugger!

		ln+=s;
		if(ln-f>=6) break;
	}

	ms=ln-f+6;

	// allocate & copy
#ifdef WIN32
	unsigned char* mem =(unsigned char*) VirtualAlloc(NULL, ms, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
	unsigned char* mem =(unsigned char*) malloc(ms);
	mprotect(mem-(unsigned long)mem%4096, ms, PROT_READ|PROT_WRITE|PROT_EXEC);
#endif

	// copy jmp
	unsigned char jmp[6];
	jmp[0]=0x68; *(unsigned int*)&jmp[1]=(unsigned int)ln; jmp[5]=0xC3;
	// copy first bytes & jmp to func
	memcpy(mem, f, ms-6); memcpy(mem+ms-6, jmp, 6);

	int r=InterceptFunctionUpdate(mem, ms-6, f);
	if(!r){ free(mem); return 0; }
	
#ifdef WIN32
	DWORD op;
	VirtualProtect(f, ms, PAGE_EXECUTE_READWRITE, &op);
#else
	int me=mprotect(f-(unsigned long)f%4096, ms, PROT_READ|PROT_WRITE|PROT_EXEC);
	if(me){ /*print("Me: ", itos(me), "\r\n");*/ free(mem); return 0; }
#endif
	jmp[0]=0x68; *(unsigned int*)&jmp[1]=(unsigned int)mfunc; jmp[5]=0xC3;
	memcpy(f, jmp, 6);
	if(ln-f>6) memset(f+6, 0xCC, ln-f-6);

#ifdef WIN32
	VirtualProtect(f, 6, op, &op);	
#endif

	return mem;
}

/////////////////////////////////////////////////////////////////////////////////////
void* InterceptFunction(void *func, void *mfunc){ 
	void *ret;
	//print("InterceptFunction\r\n");
	if(ret = InterceptFunctionIT(func, mfunc))
		return ret; // import table
	//print("import table fail\r\n");
	if(ret = InterceptFunctionHP((unsigned char*)func, (unsigned char*)mfunc))
		return ret; // hot patching
	//print("hot patching fail\r\n");
	if(ret = InterceptFunctionDJ(func, mfunc))
		return ret; // double jump
	//print("double jump fail\r\n");
	if(ret = InterceptFunctionCopy(func, mfunc))
		return ret; // copy func
	//print("copy func fail\r\n");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
// Get Kernel32.dll
HMODULE GetKernel(){
	__asm {
	mov eax, dword ptr fs:[30h]
	mov eax, dword ptr [eax+0ch]
	mov esi, dword ptr [eax+1ch]
	lodsd
	mov eax, dword ptr [eax+08h]
	}
}
#endif
