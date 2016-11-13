/*
 * Company:	MSV (MikelSV) code name Vyris
 * This: MString - My String class
 *		 My version &CString class
 *
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MString version 1.4.3 10/April/2005

	MString : Copyright (C) 2004 - 2005, Mishael Senin


				
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

#define		MSV_VER  "1.0.1"

int ReadDir(MString dir, ISLine &list);



#define		MSVCF_VER	"1.0.2"

class MSVCF{
										 
	MString cfile;
	bool isopen, isread;
	HANDLE hfile;

public:
	DXLine conf;
//	TLine conf;

public:

	MSVCF();
	~MSVCF();

	int Read(MString file);
	int Read(bool noclear=0, bool renew=0);
	int Write();
	int Open(MString file, int read=1);
	bool IsOpen();
	int Create();
	void Release();
	MString GetConf(const MString &line);
	MString GetConf(const MString &root, const MString &line);
	int SetConf(const MString &line, const MString &param);

};
   
