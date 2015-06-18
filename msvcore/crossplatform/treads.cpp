typedef DWORD (*LPTHREAD_METHOD)(LPVOID pParam);

#ifdef USEMSV_TUPLE
#ifndef USEMSV_ANDROID
typedef DWORD (*LPTHREAD_METHOD_T)(MTuple &_tuple);
#endif
#endif

// Статическая функция, которая запустит метод.
//static  DWORD StartFunc (LPSTARTUP_PARAM pStartup);

// Структура параметров для статической функции.
typedef struct STARTUP_PARAM {
    LPTHREAD_METHOD		pMethod;
    LPVOID				pParam;
	HANDLE				hdl;
} *LPSTARTUP_PARAM;

DWORD StartFunc(LPSTARTUP_PARAM pStartup){
	// Распаковываем данные в новом потоке.
	LPTHREAD_METHOD pMethod	= pStartup->pMethod;
	LPVOID pParam			= pStartup->pParam;

#if(MSV_PROC_STAT)
_msvps_thread_cnt++;
_msvps_thread_is++;
#endif
    // Запускаем функцию в новом потоке.
    DWORD dwResult = (*pMethod)(pParam);
#if(MSV_PROC_STAT)
_msvps_thread_is--;
#endif

    // Удаляем временные данные и возвращаем код возврата из нового потока.
#ifdef WIN32
	CloseHandle(pStartup->hdl);
#endif
	delete pStartup;
    return dwResult;
}

#ifndef WIN32
//#include "pthread.h"
#define LPSECURITY_ATTRIBUTES pthread_attr_t*
#endif

HANDLE StartThread(LPTHREAD_METHOD pMethod, LPVOID pParam=0, LPDWORD pdwThreadID=0, const LPSECURITY_ATTRIBUTES pSecurity=0, DWORD dwStackSize=0, DWORD dwFlags=0){
    // Создаем структуру и упаковываем данные для статической функции.
    LPSTARTUP_PARAM pStartup = new STARTUP_PARAM;
    pStartup->pMethod	= pMethod;
    pStartup->pParam	= pParam;

    // Создаем новый поток.
#ifdef WIN32
	 pStartup->hdl=CreateThread(pSecurity, dwStackSize, (LPTHREAD_START_ROUTINE)StartFunc, pStartup, dwFlags, pdwThreadID);
	 if(!ishandle(pStartup->hdl)){ delete pStartup; return 0; }
	 return pStartup->hdl;
#else
	pthread_t restrict; if(pdwThreadID) *pdwThreadID=0;
	if(pthread_create(&restrict, pSecurity, (void* (*)(void*))StartFunc, pStartup)){ return 1; }
	if(pdwThreadID) *pdwThreadID=1;
	pthread_detach(restrict);
	return 0;
#endif
}


#ifdef USEMSV_TUPLE
#ifndef USEMSV_ANDROID
/////////////////////////////////////////////// TUPLE
typedef struct STARTUP_PARAM_T {
    LPTHREAD_METHOD_T		pMethod;
    MTuple				pParam;
	HANDLE				hdl;
} *LPSTARTUP_PARAM_T;

DWORD StartFuncT(LPSTARTUP_PARAM_T pStartup){
	// Распаковываем данные в новом потоке.
	LPTHREAD_METHOD_T pMethod	= pStartup->pMethod;
	MTuple &pParam			= pStartup->pParam;

#if(MSV_PROC_STAT)
_msvps_thread_cnt++;
_msvps_thread_is++;
#endif
    // Запускаем функцию в новом потоке.
    DWORD dwResult = (*pMethod)(pParam);
#if(MSV_PROC_STAT)
_msvps_thread_is--;
#endif

    // Удаляем временные данные и возвращаем код возврата из нового потока.
#ifdef WIN32
	CloseHandle(pStartup->hdl);
#endif
	delete pStartup;
    return dwResult;
}

HANDLE StartThreadT(LPTHREAD_METHOD_T pMethod, MTuple &pParam, LPDWORD pdwThreadID=0, const LPSECURITY_ATTRIBUTES pSecurity=0, DWORD dwStackSize=0, DWORD dwFlags=0){
    // Создаем структуру и упаковываем данные для статической функции.
    LPSTARTUP_PARAM_T pStartup = new STARTUP_PARAM_T;
    pStartup->pMethod	= pMethod;
    pStartup->pParam	= pParam;

    // Создаем новый поток.
#ifdef WIN32
	 pStartup->hdl=CreateThread(pSecurity, dwStackSize, (LPTHREAD_START_ROUTINE)StartFuncT, pStartup, dwFlags, pdwThreadID);
	 if(!ishandle(pStartup->hdl)){ delete pStartup; return 0; }
	 return pStartup->hdl;
#else
	pthread_t restrict; if(pdwThreadID) *pdwThreadID=0;
	if(pthread_create(&restrict, pSecurity, (void* (*)(void*))StartFuncT, pStartup)){ return 1; }
	if(pdwThreadID) *pdwThreadID=1;
	pthread_detach(restrict);
	return 0;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////
#endif
#endif



// COT
class MSVCOT;

DWORD MSVCOT_RunCot(LPVOID lp);

struct MSVCOTD{
LPVOID lp;
MSVCOT*cot;
};

class MSVCOT{
protected:
	MSVCOT(){ cottid=0; }
public:
	DWORD cottid, cotuse;

public:
	virtual DWORD COT(LPVOID lp){
		//while(cotuse) Sleep(1);
		return 0;
	}

	void StopCot(){
		if(cottid) cotuse=0; while(cottid) Sleep(2);
	}

	//void KillCot(int wl=20){
	//	DWORD ct=cottid;
	//	if(cottid && cottid!=0xdeaddead) cottid=0xdeaddead;
	//	if(cottid) Sleep(wl);
	//	if(cottid) KillThread(ct);
	//}

	int COTS(LPVOID lp=0){
		MSVCOTD*cot=new MSVCOTD;
		cot->cot=this; cot->lp=lp;
		StopCot(); cot->cot->cotuse=1;
		StartThread(MSVCOT_RunCot, cot, &cottid, 0, 0, 0);
	return 1;
	}

};


DWORD MSVCOT_RunCot(LPVOID lp){
	MSVCOTD*cot=(MSVCOTD*)lp;
	cot->cot->COT(cot->lp); cot->cot->cottid=0;
	delete cot; return 0;
}


// Multi COT
class MSVMCOT;

DWORD MSVMCOT_RunCot(LPVOID lp);

struct MSVMCOTD{
	LPVOID lp;
	MSVMCOT*cot;
};

class MSVMCOT{
protected:
	MSVMCOT(){ cottid=0; }
public:
	DWORD cottid;

public:
	virtual DWORD MCOT(LPVOID lp){
		//while(cotuse) Sleep(1);
		return 0;
	}

	void StopCot(){}

	//void KillCot(int wl=20){
	//	DWORD ct=cottid;
	//	if(cottid && cottid!=0xdeaddead) cottid=0xdeaddead;
	//	if(cottid) Sleep(wl);
	//	if(cottid) KillThread(ct);
	//}

	int MCOTS(LPVOID lp=0){
		MSVMCOTD*cot=new MSVMCOTD;
		cot->cot=this; cot->lp=lp;
		StartThread(MSVMCOT_RunCot, cot, &cottid, 0, 0, 0);
	return 1;
	}

};


DWORD MSVMCOT_RunCot(LPVOID lp){
	MSVMCOTD*cot=(MSVMCOTD*)lp;
	cot->cot->MCOT(cot->lp); cot->cot->cottid=0;
	delete cot; return 0;
}