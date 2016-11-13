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


MSVCF::MSVCF(){isopen=0; isread=0;}
	
MSVCF::~MSVCF(){Release(); }

	void MSVCF::Release(){if (IsHandle(hfile)) CloseHandle(hfile); conf.clear(); isread=0; isopen=0;}

	
	int MSVCF::Read(MString file){return Open(file);}
	
	int MSVCF::Read(bool noclear, bool renew){
		if(!noclear) conf.clear();

	unsigned long ult;
	int sizefile = GetFileSize(hfile, NULL);
	MString readfile; readfile.Reserv(sizefile);

	ReadFile(hfile, readfile, sizefile, &ult, NULL);
	//DXLine*ins=&conf;
	CXLine cxl((MMatrixX*)&conf);

	int pos=0, lpos=0; int level=0;
	while(pos<sizefile){

		if (rts(readfile, 13, pos)) {
				insert:
				MString tm[2];
				explode(tm, readfile.Mid(lpos, pos-lpos), " ", 1, 1);
				tm[0]=dspace(tm[0],3); tm[1]=dspace(tm[1],2);
				if(!tm[0] && !tm[1]) {goto skip;}
			{
				if(tm[0].Left(1)=="{") {level=1; tm[0]=tm[0].Mid(1);}
				if(tm[1]) {if(tm[1].Right(1)=="{") {level=1; tm[1]=tm[1].Mid(0, tm[1].size()-1);}}
				else if(tm[0].Right(1)=="{") {level=1; tm[0]=tm[0].Mid(0, tm[0].size()-1);}

				if(tm[0].Left(1)=="}") {level=-1; tm[0]=tm[0].Mid(1);}
				if(tm[1]) {if(tm[1].Right(1)=="}") {level=-1; tm[1]=tm[1].Mid(0, tm[1].size()-1);}}
				else if(tm[0].Right(1)=="}") {level=-1; tm[0]=tm[0].Mid(0, tm[0].size()-1);}

				if(!tm[0] && !tm[1] && !level) {goto skip;}

				if(renew){
				DXLine*xl=(DXLine*)cxl.xline;
				DXLine*xli=xl->Find(tm[0]);
				if(xli) xl->del(xli);
				}
				DXLine*xl=(DXLine*)cxl.xline; xl->insert(tm[0], tm[1]);
				if(level>0){level=0; cxl.EUp();}
				if(level<0){level=0; cxl.Down();}
			}
			skip:
				pos++;						 
				if (readfile[pos]==10) pos++;
				lpos=pos; 
		}		
		else goto insert;

	}
	
	isread=1;
	return 1;
	}





	int MSVCF::Write(){
	SetFilePointer(hfile, 0, NULL, FILE_BEGIN);
	
	unsigned long ult;
//	int sizefile= GetFileSize(hfile, NULL);
//	MString readfile; readfile.Reserv(sizefile);

	DXLine *dtl=conf._a;

	while(dtl){

	WriteFile(hfile, dtl->s, dtl->s.size(), &ult, NULL);
	if (dtl->t) WriteFile(hfile, " "+dtl->t, dtl->t.size()+1, &ult, NULL);
	if (dtl->_n) WriteFile(hfile, MString(13)+MString(10), 2, &ult, NULL);

	dtl=dtl->_n;
	}	
	
	SetEndOfFile(hfile);
	
	return 1;	
	}





	////////////////////////////////////
	int MSVCF::Open(MString file, int isread){
		isopen=0;

	SECURITY_ATTRIBUTES Sec;
	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = false;
	Sec.bInheritHandle = false;


	hfile = CreateFile(file, FILE_READ_DATA|FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &Sec, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//hfile = CreateFile(file, FILE_READ_DATA|FILE_WRITE_DATA, FILE_SHARE_READ, &Sec, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!IsHandle(hfile)) return 0;

	isopen=1; cfile=file;
	if (!isread) return 1;

	return Read();
	}


	MString MSVCF::GetConf(const MString &line){
		if(!isopen || !isread) return "";
		DXLine *dtl=conf.Find(line);
		return dtl ? dtl->t : MString();
	}

	MString MSVCF::GetConf(const MString &root, const MString &line){
		if(!isopen || !isread) return "";
		DXLine *dtl=conf.Find(root);
		if(!dtl) return ""; dtl=dtl->Find(line);
		return dtl ? dtl->t : MString();
	}

	int MSVCF::SetConf(const MString &line, const MString &param){
	if(!isopen) return 0;
	DXLine *dtl=conf.Find(line);
	if (dtl) dtl->t=param;
	else conf.insert(line, param);

	return 1;
	}

	bool MSVCF::IsOpen(){if(isopen)return 1; return 0;}


	int MSVCF::Create(){
	SECURITY_ATTRIBUTES Sec;
	Sec.nLength = sizeof(SECURITY_ATTRIBUTES);
	Sec.lpSecurityDescriptor = false;
	Sec.bInheritHandle = false;
	hfile = CreateFile(cfile, FILE_READ_DATA|FILE_WRITE_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, &Sec, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!IsHandle(hfile)) return 0;
	isopen=1;
	return 1;
	}