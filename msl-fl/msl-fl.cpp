#define PROJECTNAME "msl-fl"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

//#define USEMSV_XMLCONT
//#define USEMSV_DATIXCONT

#define USEMSV_ITOS
#define USEMSV_OLIST
#define USEMSV_MLIST

// Add MySQL support
//#define USEMSV_MYSQL

#ifndef USE_MSLFL_VER
#define USEMSV_MSL_FL
//#define USE_MSLFL_VER 8
//#include "../../opensource/msvcore/msl-fl/msl-fl.h"
#endif

#include "../../opensource/msvcore/msvcore.cpp"

// use: comment this #define for last version, or: 1, 2...
//#define USE_MSLFL_VER 1


#if USE_MSLFL_VER==1
#include "old/msl-fl_v1.h"
#endif

#if USE_MSLFL_VER==2
#include "old/msl-fl_v2.h"
#endif

#if USE_MSLFL_VER==3
#include "old/msl-fl_v3.h"
#endif

#if USE_MSLFL_VER==4
#include "old/msl-fl_v4.h"
#endif

#if USE_MSLFL_VER==5
#include "old/msl-fl_v5.h"
#endif

#if USE_MSLFL_VER==6
#include "old/msl-fl_v6.h"
#endif

#if USE_MSLFL_VER==7
#include "old/msl-fl_v7.h"
#endif

int TestMSL();

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link);

	msl_fl msl;

	// run on cgi
	VString scr = getenv("PATH_TRANSLATED");
	//scr="test.msl";
	if(scr){

		if(!IsFile(scr)){
			print("Content-type: text/html\r\n\r\n");
			print("File not found.");
			return 1;
		}

		MString f = LoadFile(scr);
		scr = f;
		
		if(scr.incompare("#!/"))
			scr = PartLineOT(scr, "\n");
		
		msl.Do(scr);
		
		// headers
		print("Content-type: text/html\r\n\r\n");
		// output
		print(msl.GetOutput());

		return 0;
	}//else{
	//	TestMSL();
	//}

	// analys command line
	VString l = "GO", t; int lto = 2, ltop = 1; 
	int ishelp = 0, isver = 0;
	//VString scr;

	while(l){
		if(!ltop && !lto) lto=1; ltop=lto;
		if(lto==1){ l=t; t=link.GetArg(); lto=0; }
		else if(lto==2){ l=link.GetArg(); t=link.GetArg(); lto=0; }		
		if(!l) break;

		if(l=="--help"){ lto=1; ishelp=1; }
		else if(l=="--ver"){ lto=1; isver=1; }
		else if(lto=usemsv_iskey(l)){}		

		else if(!scr){ lto=1; scr=l; }
		else { print(Lang("Invalid options: unknown key"), " '", l, "'\r\n"); return 1; }
	}

	if(!scr || isver)
		print("msl-fl v.", msl_fl_version[0].ver," (", msl_fl_version[0].date, ").\r\n\r\n");

	if(isver){
		VersionsPrint(msl_fl_version, sizeof(msl_fl_version)/sizeof(Versions));
		return 0;
	}

	if(!scr || ishelp){
		print(Lang("Usage: msl-fl filename"), "\r\n");
		print(Lang(" ## msv options: use --usemsv --help"), "\r\n");
		print(Lang(" ## info"), "\r\n");
		print("--help", "\t\t", Lang("print this help"), "\r\n");
		print("--ver", "\t\t", Lang("print versions list"), "\r\n");
		return 0;
	}

	if(!IsFile(scr)){
		print("File not found.");
		return 1;
	}

	MString f = LoadFile(scr);
	scr = f;

	if(scr.incompare("#!/")) 
		scr = PartLineOT(scr, "\n");
	msl.Do(scr);
		
	// output
	print(msl.GetOutput());

	return 0;
}

int TestMemory(){
/*	msl_value_matrix mx;


	tbtime

	MString arr; arr.Reserv(S4M*sizeof(void*));
	msl_value **am=(msl_value **)arr.data;

	for(int i=0; i<S128K; i++){
		//am[i]=new msl_value;// am[i]=m;
		am[i]=mx.New();
	}

	for(int i=0; i<S128K; i++){
		//delete am[i];
		mx.Del(am[i]);
	}

	tetime
*/
	return 1;
}


int TestMSL(){
	msl_fl msl;

	//{
	//msl.Do(LoadFile("test.msl"));
	//MString res=msl.GetOutput();
	//print(res);
	//}
	
#if USE_MSLFL_VER==1
	// this code not work yet
	msl.Do("<? print('Hello World!'); ?>");
	MString res=msl.GetResult();
	print(res);
#endif

#if USE_MSLFL_VER==2
	// this code is work
	msl.Do("<? print('Hello World!', '!!!'); ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

#if USE_MSLFL_VER==3
	// this code is work
	msl.Do("<? $hello='Hello World!'; print($hello); ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

#if USE_MSLFL_VER==4
	// this code is work
	msl.Do("<? if(1) print('true '); else print('false '); ?>");
	msl.Do("<? $hello='Hello code!'; if($hello) print($hello); else print('Sorry, code hate you.'); ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

#if USE_MSLFL_VER==5
	// this code is work
	msl.Do("<? print('Hello World!!!'); $i=55; while($i){ $i--; print('1'); } ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

#if USE_MSLFL_VER==6
	// this code is work
	msl.Do("<? print('Hello World!!!'); for($i=55; $i; $i--){ print('1'); } ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

#if USE_MSLFL_VER==7
	// this code is work
	msl.Do("<? $i=13; $j=15; $i++; --$j; print($i, ' . ', $j); ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

#if USE_MSLFL_VER==8
	// this code is work
	msl.Do("<? function test($k, $v){ print('Key: '.$k.' and val: '.$v); } test('box', 'gold'); ?>");
	MString res=msl.GetOutput();
	print(res);
#endif

	return 0;
}