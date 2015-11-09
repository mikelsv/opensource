#define PROJECTNAME "MsvXcc"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#define USEMSV_ITOS
#define USEMSV_XDATACONT
#define USEMSV_DATIXCONT
#define USEMSV_MLIST
#define USEMSV_ILIST
#define USEMSV_OLIST
#define USEMSV_ULIST
#define USEMSV_CPPXCC

//#define USEMSV_INJECTDLL
//#define USEMSV_INTERCEPT
//#define USEMSV_INTERCEPT_MALLOC

#ifdef MSVXCCTEMPLATE

#define AA(a) int a;
#define B(b) AA(b)

#define A 1, 2, 3
#define B(a, b, ...) printf(a, __VA_ARGS__ );
	B("%d %d %d\r\n", 1, 2, 3);
	B("%d %d %d\r\n", ::A);

	#define B fail
	#define BB(a, b, c) a c
	#define AB(B) BB(B)
	AB(A);

	#define BC(a, b, c) a c
	#define AC(B) BC(::B)

	AC(A);
#else

#endif

#include "../../opensource/msvcore/msvcore.cpp"

Versions PROJECTVER[]={
	"0.0.0.1", "04.11.2015 21:40",
};

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link);

	print(PROJECTNAME, " v.", PROJECTVER[0].ver," (", PROJECTVER[0].date, ").\r\n");

	// operations
	VString comm = link.file, t, l = PartLine(link.GetArg(), t);	
	int ishelp = 0, isreplace = 0, isprint = 0;

	VString files[S1K];
	int filesz = 0;

	while(l){
		if(l == "--help" || l == "-h" || l == "/?"){ ishelp = 1; }
		else if(l == "--replace" || l == "-r"){ isreplace = 1; }
		else if(l == "--print" || l == "-p"){ isprint = 1; }
		else{
			if(l[0] == '-'){
				print("Error: unknown options: '",  l, "'\r\n");
				ishelp = 1;
				break;
			}

			if(filesz >= S1K){
				print("Error: max files 1024!\r\n");
				ishelp = 1;
				break;
			}
			files[filesz++] = l;
		}

		l = PartLine(link.GetArg(), t);
	}

	if(!filesz || ishelp){
		print("Usage: msvxcc [options] file1 [file2] [file3...]" "\r\n"
			"--help, -h       print this help" "\r\n"
			"--replace, -r    replace MSVXCCTEMPLATE and save to file.new" "\r\n"
			"--print, -p      print logs" "\r\n"
			);
		return 0;
	}

	CppXcc xcc;
	xcc.SetPrint(isprint ? MSVXCC_PRINT_ALL : 0);
	xcc.SetEnableReplace(isreplace);

	for(int i = 0; i < filesz; i++){
		xcc.AnalysFile(files[i], 0);
	}

	xcc.CheckFiles();

	VString err = xcc.GetError();
	if(err)
		print("Xcc errors: \r\n", err, "\r\n");

	return 0;
}