// MikelSV Inject Dll




#ifndef PROJECTNAME
	#error Please set #define PROJECTNAME "you_project_name"
#endif


void ProcessAttach();

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved){
	//Injection();

	switch(Reason){
	case DLL_PROCESS_ATTACH:
		//asmcodesz.Use((unsigned char*)asmbinarycode, sizeof(asmbinarycode));

		//pOrigMBAddress = (pMessageBoxW) //Get MessageBoxW pointer
		//	GetProcAddress(GetModuleHandle("user32.dll"), "MessageBoxW");
		//if(pOrigMBAddress != NULL)
			//BeginRedirect(MyMessageBoxW);	//Valid? Redirect
		ProcessAttach();

		//Injection();

		break;
	case DLL_PROCESS_DETACH:
		//memcpy(pOrigMBAddress, oldBytes, SIZE);
	case DLL_THREAD_ATTACH:
		//BeginRedirect(MyMessageBoxW);
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

#define MSVINJECTDLL(ret, func, ...) typedef ret (WINAPI *func)(...);
//int WINAPI MyMessageBoxW(HWND, LPCWSTR, LPCWSTR, UINT);



/*
int globalerror(const char *line){


	return 0;
}


TString LoadFile(VString file){ return ""; }
int SaveFile(VString file, VString data){ return 1; }
*/