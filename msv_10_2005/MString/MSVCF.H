/*
 * Company:	MSV (MikelSV) code name Vyris
 * This: MSV CF - Configuration file
 */
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	CF version 1.0.0 4/Mai/2005

	MSV CF : Copyright (C) 2005, Mishael Senin



	Covered code is provided under this license on an "as is" basis, without warranty
	of any kind, either expressed or implied, including, without limitation, warranties
	that the covered code is free of defects, merchantable, fit for a particular purpose
	or non-infringing. The entire risk as to the quality and performance of the covered
	code is with you. Should any covered code prove defective in any respect, you (not
	the initial developer or any other contributor) assume the cost of any necessary
	servicing, repair or correction. This disclaimer of warranty constitutes an essential
	part of this license. No use of any covered code is authorized hereunder except under
	this disclaimer.

	Permission is hereby granted to use, copy, modify, and distribute this
	source code, or portions hereof, for any purpose, including commercial applications,
	freely and without fee, subject to the following restrictions: 

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.

  --------------------------------------------------------------------------------


  --------------------------------------------------------------------------------
 */

#define		MSVCF_VER	"1.0.0"

class MSVCF{

	MString cfile;
	bool isopen;
	HANDLE hfile;

	TLine conf;
//	TLine conf;

public:
	
	MSVCF(){isopen=0;}
	
	~MSVCF(){if (IsHandle(hfile)) CloseHandle(hfile);}

	
	int Read(MString file){return Open(file);}
	
	int Read(){
		conf.delall();

	
	unsigned long ult;
	int sizefile= GetFileSize(hfile, NULL);
	MString readfile; readfile.Reserv(sizefile);

	ReadFile(hfile, readfile, sizefile, &ult, NULL);

	int pos=0, lpos=0;
	while(pos<sizefile){

		if (rts(readfile, 13, pos)) {
				insert:
				MString tm[2];
				explode(tm, readfile.Mid(lpos, pos-lpos), " ", 1, 1);
				conf.insert(tm[0], tm[1]);
				pos++;
				if (readfile[pos]==10) pos++;
				lpos=pos;
		}		
		else goto insert;

	}
	
	
	//MString tmp;
	//for(int i=0; i<sizefile; i++){
	//	if (tmp==13 && tmp){
	//		MString tm[2];
	//		explode(tm, tmp, " ", 1, 1);
	//		conf.insert(tm[0], tm[1]);
	//		tmp.Empty();
	//		if ()

	//	}
	//tmp
	//}


	return 1;
	}





	int Write(){
	SetFilePointer(hfile, 0, NULL, FILE_BEGIN);
	
	unsigned long ult;
//	int sizefile= GetFileSize(hfile, NULL);
//	MString readfile; readfile.Reserv(sizefile);

	DTLine *dtl=conf.a;

	while(dtl){

	WriteFile(hfile, dtl->s, dtl->s.size(), &ult, NULL);
	if (dtl->t) WriteFile(hfile, " "+dtl->t, dtl->t.size()+1, &ult, NULL);
	if (dtl->n) WriteFile(hfile, MString(13)+MString(10), 2, &ult, NULL);

	dtl=dtl->n;
	}	
	
	SetEndOfFile(hfile);

	
/*
	int pos=0, lpos=0;
	while(pos<sizefile){

		if (rts(readfile, 13, pos)) {
				insert:
				MString tm[2];
				explode(tm, readfile.Mid(lpos, pos-lpos), " ", 1, 1);
				conf.insert(tm[0], tm[1]);
				pos++;
				if (readfile[pos]==10) pos++;
				lpos=pos;
		}		
		else goto insert;

	}

		*/
	
	
	return 1;	
	}





	////////////////////////////////////
	int Open(MString file, int isread=1){
		isopen=0;

	SECURITY_ATTRIBUTES Sec;
	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = false;
	Sec.bInheritHandle = false;


	hfile = CreateFile(file, FILE_READ_DATA|FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &Sec, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//hfile = CreateFile(file, FILE_READ_DATA|FILE_WRITE_DATA, FILE_SHARE_READ, &Sec, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!IsHandle(hfile)) return 0;

	if (!isread) return 1;

	return Read();
	}




	MString GetConf(const MString &line){
		DTLine *dtl=conf.Find(line);
		return dtl ? dtl->t : MString();
	}

	int SetConf(const MString &line, const MString &param){
	DTLine *dtl=conf.Find(line);
	if (dtl) dtl->t=param;
	else conf.insert(line, param);

	return 1;
	}



};