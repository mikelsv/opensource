bool InjectDll(DWORD procID, VString dll){
    //Find the address of the LoadLibrary api, luckily for us, it is loaded in the same address for every process
    HMODULE hLocKernel32 = GetModuleHandle("Kernel32");
    FARPROC hLocLoadLibrary = GetProcAddress(hLocKernel32, "LoadLibraryA");
	FARPROC hLocFreeLibrary = GetProcAddress(hLocKernel32, "FreeLibraryA");
    
    //Adjust token privileges to open system processes
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
    }

    //Open the process with all access
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

    //Allocate memory to hold the path to the Dll File in the process's memory
//    dll += '\0';
    LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, dll.size(), MEM_COMMIT, PAGE_READWRITE);

    //Write the path to the Dll File in the location just created
    DWORD numBytesWritten;
    WriteProcessMemory(hProc, hRemoteMem, dll, dll.size(), &numBytesWritten);

    //Create a remote thread that starts begins at the LoadLibrary function and is passed are memory pointer
	DWORD ThreadID;
    HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, hRemoteMem, 0, &ThreadID);

    //Wait for the thread to finish
    bool res = false;
    if (hRemoteThread)
        res = (bool)WaitForSingleObject(hRemoteThread, 10000) != WAIT_TIMEOUT;

    //Free the memory created on the other process
    VirtualFreeEx(hProc, hRemoteMem, dll.size(), MEM_RELEASE);

    //Release the handle to the other process
    CloseHandle(hProc);

    return res;
}

// Inject Dll Loader
typedef HMODULE (WINAPI *LpLoadLibrary)(LPCSTR);

typedef struct InjectDllData {
  char        dllpath[S4K];
  LpLoadLibrary  MyLpLoadLibrary;
};

static DWORD WINAPI InjectDllMain(LPVOID lpParams){  
	InjectDllData *info = (InjectDllData*)lpParams;
	info->MyLpLoadLibrary(info->dllpath);
	return 0;
}

bool InjectDllLoader(DWORD procID, VString dll){
    //Find the address of the LoadLibrary api, luckily for us, it is loaded in the same address for every process
    HMODULE hLocKernel32 = GetModuleHandle("Kernel32");
	HINSTANCE userHinstance = LoadLibrary(("user32.dll"));
    //FARPROC hLocLoadLibrary = GetProcAddress(hLocKernel32, "LoadLibraryA");
	//FARPROC hLocFreeLibrary = GetProcAddress(hLocKernel32, "FreeLibraryA");

	InjectDllData injectData;

	injectData.MyLpLoadLibrary = (LpLoadLibrary) GetProcAddress(hLocKernel32, "LoadLibraryA");
	memcpy(injectData.dllpath, dll, dll);
	injectData.dllpath[dll.sz] = 0;

    //Adjust token privileges to open system processes
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, 0, &tkp, sizeof(tkp), NULL, NULL);
    }

    //Open the process with all access
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

    //Allocate memory to hold the path to the Dll File in the process's memory
//    dll += '\0';
    LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, dll.size(), MEM_COMMIT, PAGE_READWRITE);

   
	LPVOID lpProc  = VirtualAllocEx(hProc, NULL, 2*2048, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	LPVOID lpParams = VirtualAllocEx(hProc, NULL, sizeof(injectData), MEM_COMMIT, PAGE_READWRITE );
	DWORD dwWritten;
	if(WriteProcessMemory(hProc, lpProc, InjectDllMain, 2*2048, &dwWritten ) == 0) {
	  //addLogMessage("WriteProcessMemory error", GetLastError());
	  return 0;
	}
	if(WriteProcessMemory( hProc, lpParams, &injectData, sizeof(injectData), &dwWritten ) == 0) {
	  //addLogMessage("WriteProcessMemory error", GetLastError());
	  return 0;
	}

    //Create a remote thread that starts begins at the LoadLibrary function and is passed are memory pointer
	DWORD ThreadID;
    HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)lpProc, lpParams, 0, &ThreadID);

//    std::cout << hRemoteThread << std::endl;

    //Wait for the thread to finish
    bool res = false;
    if (hRemoteThread)
        res = (bool)WaitForSingleObject(hRemoteThread, 10000) != WAIT_TIMEOUT;

    //Free the memory created on the other process
    VirtualFreeEx(hProc, hRemoteMem, dll.size(), MEM_RELEASE);

    //Release the handle to the other process
    CloseHandle(hProc);

    return res;
}