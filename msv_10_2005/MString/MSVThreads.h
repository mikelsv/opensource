//class MSVThread;
typedef DWORD (*LPTHREAD_METHOD)(LPVOID pParam);

// Структура параметров для статической функции.
typedef struct STARTUP_PARAM
{
   // MSVThread*        pClass;
    LPTHREAD_METHOD    pMethod;
    LPVOID            pParam;
} *LPSTARTUP_PARAM;
  

    // Функция, которая создает новый поток.
    HANDLE    StartThread        (LPTHREAD_METHOD pMethod, LPVOID pParam, 
                             LPDWORD pdwThreadID = NULL, 
                             LPSECURITY_ATTRIBUTES pSecurity = NULL , 
                             DWORD dwStackSize = 0 , 
                             DWORD dwFlags = 0);

    // Статическая функция, которая запустит метод.
    static  DWORD StartFunc    (LPSTARTUP_PARAM pStartup);
//};


HANDLE StartThread(LPTHREAD_METHOD pMethod, LPVOID pParam, 
                             LPDWORD pdwThreadID /* = NULL */, 
                             LPSECURITY_ATTRIBUTES pSecurity /* = NULL */, 
                             DWORD dwStackSize /* = 0 */, 
                             DWORD dwFlags /* = 0 */)
{
    // Создаем структуру и упаковываем данные для статической функции.
    LPSTARTUP_PARAM pStartup = new STARTUP_PARAM;
//    pStartup->pClass    = 0;
    pStartup->pMethod    = pMethod;
    pStartup->pParam    = pParam;

    // Создаем новый поток.
    return CreateThread(pSecurity, dwStackSize, (LPTHREAD_START_ROUTINE)StartFunc, pStartup, dwFlags, pdwThreadID);
}

// В новом потоке вначале вызывается функция CMyClass::StartFunc(...)
// А она запускает наш метод.

DWORD StartFunc(LPSTARTUP_PARAM pStartup)
{
    // Распаковываем данные в новом потоке.
    // Получаем указатель на класс и на метод класса.
//    MSVThread* pClass        = pStartup->pClass;
    LPTHREAD_METHOD pMethod = pStartup->pMethod;
    LPVOID pParam            = pStartup->pParam;

    // Запускаем метод класса в новом потоке.
    DWORD dwResult = (*pMethod)(pParam);

    // Удаляем временные данные и возвращаем код возврата из нового потока.
    delete pStartup;
    return dwResult;
}

  

HANDLE StartThread(
    LPTHREAD_METHOD pMethod, LPVOID pParam, 
    LPDWORD pdwThreadID /* = NULL */, 
    LPSECURITY_ATTRIBUTES pSecurity /* = NULL */, 
    DWORD dwStackSize /* = 0 */, 
    DWORD dwFlags /* = 0 */
);