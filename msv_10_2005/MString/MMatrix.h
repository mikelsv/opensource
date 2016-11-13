/*
 * Company:	MSV (MikelSV) code name Vyris
 * This: MMatrix - My Matrix class
 *		 My version Array class
 */
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MMatrix version 1.2.3 7/Mai/2005

	MMatrix : Copyright (C) 2004 - 2005, Mishael Senin



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

#define		MMATRIX_VER "1.3.3"

struct MMatrixD;

struct MMatrixR{
MMatrixD *_a, *_e;
int _sz;
};

struct MMatrixD{
MMatrixD *_p, *_n;
};

class MMatrixXE{
public:
	virtual void destroy(void*){};
};

struct MMatrixX: public MMatrixXE{
public:
MMatrixX *_a, *_e, *_p, *_n;
int _sz;
};




//delete - next, prev	class delete
int _mdeletex(MMatrixX* root, MMatrixX* data, MMatrixXE*cls);

// delete all for all types		size delete all	 class delete
int _mdeleteallx(MMatrixX* root, MMatrixXE*cls);


//		MMATRIX 

// <- & ->	 DOUBLE	 <- & ->
//		-> prev next <-


// insert - prev, next
void* _minsert(MMatrixR* root, MMatrixD* data);


//delete - next, prev	automatic delete
int _mdelete(MMatrixR* root, MMatrixD* data);

//delete - next, prev	user delete
void* _mdelete_u(MMatrixR* root, MMatrixD* data);

// delete all for all types		automatic delete all
int _mdeleteall(MMatrixR* root, MMatrixD* data);

// delete all for all types		size delete all
int _mdeleteall(MMatrixR* root, MMatrixD* data, class *clr);


void* _minsertx(MMatrixX* root, MMatrixX* data);
int _mdeletex(MMatrixX* root, MMatrixX* data);
int _mdeleteallx(MMatrixX* root, MMatrixX* data);

int _mdeleteallcx(MMatrixX* root, class* data);


inline void* _mrap(void* v, int p){
int a=(int)v;
return (void*)(a+p);
}



// INSERT 

// next // prev - standart
template < class D, class M>
M* _minsert(D&t, M*m){

	if(t->sz<1){
        t->a=new M;
        t->a->p=NULL;
        t->a->n=NULL;
		t->e=t->a;
		t->sz=1;
        return (M*)t->a;
    }
    t->e->n=new M;
    t->e->n->p=t->e;
    t->e->n->n=NULL;
	t->e=t->e->n;
    t->sz++;
    return (M*)t->e;
}





// next	only
template < class D, class M>
M* _minsert_(D&t, M*m){

	if(t->sz<1){
        t->a=new M;
        t->a->n=NULL;
		t->e=t->a;
		t->sz=1;
        return (M*)t->a;
    }
    t->e->n=new M;
    t->e->n->n=NULL;
	t->e=t->e->n;
    t->sz++;
    return (M*)t->e;
}

//DELETE

//next prev
template <class D, class M>
int _mdelete(D &t, M*ind)
{
    if(t->sz==0)
        return 0;
	M *ni,*pi,*dl;
	if (ind->n) dl=ind->n->p;
	else if (ind->p) dl=ind->p->n;
	else {delete t->a; t->a=t->e=0; t->sz=0; return 1;}
    if (ind->n)  ni=ind->n; else 	ni=NULL;
	if (ind->p)  pi=ind->p; else 	pi=NULL;
	if (ni) ni->p=pi;
	else  t->e=ind->p;
	if (pi) pi->n=ni;
	else  t->a=ind->n;
	delete dl;
    t->sz--;
    return 1;
}

//next only, delete first element

template <class D, class M>
int _mdelete_(D &t, M*ind)
{
    if(t->sz==0)
        return 0;
	M *dl;
	if (t->sz==1)  {delete t->a; t->a=t->e=0; t->sz=0; return 0;}
	
	dl=t->a; t->a=t->a->n;
	delete dl;
	t->sz--;
    return 1;
}

//INSERT SORTED

// next // prev - standart
template < class D, class M>
M* _mssinsert(D&t, M*m, MString ms, int p){

	if(t->sz<1){
        t->a=new M;
        t->a->p=NULL;
        t->a->n=NULL;
		t->e=t->a;
		MString &tma=*(MString*)_mrap(t->a,p);
		tma=ms;
		t->sz=1;
        return (M*)t->a;
    }

//first sorted
MString &tma=*(MString*)_mrap(t->a,p);
if (tsor(ms, tma)>=0){
    t->a->p=new M;
    t->a->p->n=t->a;
    t->a->p->p=NULL;
	t->a=t->a->p;
	MString &tma=*(MString*)_mrap(t->a,p);
	tma=ms;
    t->sz++;
    return (M*)t->a;
}

//end sorted
MString &tme=*(MString*)_mrap(t->e,p);
if (tsor(ms, tme)==-1){
    t->e->n=new M;
    t->e->n->p=t->e;
    t->e->n->n=NULL;
	t->e=t->e->n;
	MString &tme=*(MString*)_mrap(t->a,p);
	tme=ms;
    t->sz++;
    return (M*)t->e;
}

M*ar=t->a;
while(ar){
MString tms=*(MString*)_mrap(ar,p);
if (tsor(ms, tms)==1){
	M*a=new M;
	ar->n->p=a;
	a->n=ar->n;
	ar->n=a;
	a->p=ar;
	MString tms=*(MString*)_mrap(a,p);
	tms=ms;
	t->sz++;
    return (M*)a;
}
ar=ar->n;
}

return 0;
}

// DELETE ALL

//for all types
template <class D, class M>
int _mdeleteall(D &t, M*ind)
{
    if(t->sz==0)
        return 0;
	
	M *dl=t->a, *n;


	while(dl){
	n=dl->n;
	delete dl;
	dl=n;
	}

	t->sz=0; t->a=t->e=0;
    return 1;
}


// CHANGE

//for all types
template <class D, class M>
int _change(D &t, M*ind, M*tind)
{
	M*pd=ind->p;
	if (t->a==ind) 
		t->a=tind;
	if (t->e==tind) 
		t->e=ind;
	ind->p=tind;
	ind->n=tind->n;
	tind->n=tind;
	tind->p=pd;

    return 1;
}


////////////////////////////////
// NEW MATRIX 
class Line; // your class Index


// new class
class DLine
{
public:
DLine *n,*p;		// class data
MString s;			// add your data
};


class Line
{
public:
DLine *a, *e;	// class cell
unsigned int sz;
	Line (){a=e=0; sz=0;}	//new
	
	DLine* insert(MString s){ //insert
	//DLine*ind=_minsert(this, (DLine*)1); // 
	DLine*ind=_minsert(this, new DLine); // 
	ind->s=s;
	return ind;
	}

	MString* minsert(){ //insert
	DLine*ind=_minsert(this, (DLine*)1); // 
	return &ind->s;
	}


	DLine* insert(){ //insert
	return _minsert(this, (DLine*)1); // 
	}

	void del(DLine * ind){ //delete
		_mdelete(this, ind);
		return ;
	
	 }

	void delall(){ //delete
		_mdeleteall(this, this->a);
		return ;
	
	 }

	DLine* Find(MString s){
	if(sz==0)
    return 0;
	DLine*ar=a;
	while(ar){
	if (ar->s==s) return ar;
	ar=ar->n;
	}
	return 0;
	}

	MString dtake(){
	if (a){
	MString ms=a->s;
	del(a);
	return ms;
	}
	return MString();
	}

	~Line(){ delall();}
};

/////////////////////////




////////////////////////////////
// NEW MATRIX 
class TLine; // your class Index


// new class
class DTLine
{
public:
DTLine *n,*p;		// class data
MString s;			// add your data
MString t;
};


class TLine
{
public:
DTLine *a, *e;	// class cell
unsigned int sz;
	TLine (){a=e=0; sz=0;}	//new
	
	DTLine* insert(MString s, MString t){ //insert
	DTLine*ind=_minsert(this, (DTLine*)1); // 
	ind->s=s; ind->t=t;
	return ind;
	}

	MString* minsert(){ //insert
	DTLine*ind=_minsert(this, (DTLine*)1); // 
	return &ind->s;
	}


	DTLine* insert(){ //insert
	return _minsert(this, (DTLine*)1); // 
	}

	void del(DTLine * ind){ //delete
		_mdelete(this, ind);
		return ;
	
	 }

	void delall(){ //delete
		_mdeleteall(this, this->a);
		return ;
	
	 }

	DTLine* Find(MString s){
	if(sz==0)
    return 0;
	DTLine*ar=a;
	while(ar){
	if (ar->s==s) return ar;
	ar=ar->n;
	}
	return 0;
	}

	MString dtake(){
	if (a){
	MString ms=a->s;
	del(a);
	return ms;
	}
	return MString();
	}

	~TLine(){ delall();}
};

/////////////////////////


////////////////////////////////
// NEW MATRIX 
class ILine; // your class Index


// new class
class DILine
{
public:
DILine *n,*p;		// class data
int i;			// add your data
};


class ILine
{
public:
DILine *a, *e;	// class cell
unsigned int sz;
	ILine (){a=e=0; sz=0;}	//new
	
	DILine* insert(int i){ //insert
	DILine*ind=_minsert(this, (DILine*)1); // 
	ind->i=i;
	return ind;
	}

	DILine* insert(){ //insert
	return _minsert(this, (DILine*)1); // 
	}

	void del(DILine * ind){ //delete
		_mdelete(this, ind);
		return ;
	
	 }

	void delall(){ //delete
		_mdeleteall(this, this->a);
		return ;
	
	 }

	DILine* Find(int i){
	if(sz==0)
    return 0;
	DILine*ar=a;
	while(ar){
	if (ar->i==i) return ar;
	ar=ar->n;
	}
	return 0;
	}

	int dtake(){
	if (a){
	int ms=a->i;
	del(a);
	return ms;
	}
	return int();
	}

	~ILine(){ delall();}
};

/////////////////////////

  ////////////////////////////////
// NEW MATRIX 
class ISLine; // your class Index


// new class
class DISLine
{
public:
DISLine *n,*p;		// class data
MString s;			// add your data
int i;
};


class ISLine
{
public:
DISLine *a, *e;	// class cell
unsigned int sz;
	ISLine (){a=e=0; sz=0;}	//new
	
	DISLine* insert(MString s, int i){ //insert
	DISLine*ind=_minsert(this, (DISLine*)1); // 
	ind->s=s; ind->i=i;
	return ind;
	}

	MString* minsert(){ //insert
	DISLine*ind=_minsert(this, (DISLine*)1); // 
	return &ind->s;
	}


	DISLine* insert(){ //insert
	return _minsert(this, (DISLine*)1); // 
	}

	void del(DISLine * ind){ //delete
		_mdelete(this, ind);
		return ;
	
	 }

	void delall(){ //delete
		_mdeleteall(this, this->a);
		return ;
	
	 }

	DISLine* Find(MString s){
	if(sz==0)
    return 0;
	DISLine*ar=a;
	while(ar){
	if (ar->s==s) return ar;
	ar=ar->n;
	}
	return 0;
	}

	MString dtake(){
	if (a){
	MString ms=a->s;
	del(a);
	return ms;
	}
	return MString();
	}

	~ISLine(){ delall();}
};

/////////////////////////

////////////////////////////////
// NEW MATRIX 
class XLine	// your class
{
public:
XLine *_a, *_e, *_p, *_n;	// class data
unsigned int _sz;			// class data
MString string;			// add your data

	XLine (){_a=0; _e=0; _p=0; _n=0; _sz=0;}	//new
	
	XLine* insert(MString text){ //insert
	//DLine*ind=_minsert(this, (DLine*)1); // 
	XLine*ind=(XLine*)_minsertx((MMatrixX*)&_a, (MMatrixX*)new XLine); // 
	ind->string=text;
	return ind;
	}

	XLine* insert(){ //insert
	return (XLine*)_minsertx((MMatrixX*)&_a, (MMatrixX*)new XLine); // 
	}

	void del(XLine * ind){ //delete
		_mdeletex((MMatrixX*)&_a, (MMatrixX*)ind);
		return ;
	 }

	void clear(){ //delete
		_mdeleteallx((MMatrixX*)&_a, (MMatrixX*)this->_a);
		return ;
	}
/*
	XLine* Find(MString string){
	if(_sz==0) return 0;
	XLine*ar=_a;
	while(ar){
	if (ar->string==string) return ar;
	ar=ar->_n;
	}
	return 0;
	}

	MString dtake(){
	if (_a){
	MString ms=_a->string;
	del(_a);
	return ms;
	}
	return MString();
	}
	*/
	~XLine(){ clear();}
	
};

/////////////////////////



////////////////////////////////
// NEW MATRIX 
class DXLine: protected MMatrixXE	// your class
{
public:
DXLine *_a, *_e, *_p, *_n;	// class data
unsigned int _sz;			// class data
MString s;					// add your data
MString t;

	DXLine (){_a=0; _e=0; _p=0; _n=0; _sz=0;}	//new
	
	DXLine* insert(MString s, MString t){ //insert
	//DLine*ind=_minsert(this, (DLine*)1); // 
	DXLine*ind=(DXLine*)_minsertx((MMatrixX*)this, (MMatrixX*)new DXLine); // 
	ind->s=s; ind->t=t;
	return ind;
	}

	DXLine* insert(){ //insert
	return (DXLine*)_minsertx((MMatrixX*)this, (MMatrixX*)new DXLine); // 
	}

	void del(void * ind){ //delete
		_mdeletex((MMatrixX*)this, (MMatrixX*)ind, this);
		return ;
	}

	void destroy(void *ind){delete (DXLine*)ind;}

	void clear(){ //delete
		_mdeleteallx((MMatrixX*)this, this);
		return ;
	}

	DXLine* Find(MString string){
	if(_sz==0) return 0;
	DXLine*ar=_a;
	while(ar){
	if (ar->s==string) return ar;
	ar=ar->_n;
	}
	return 0;
	}

	

	~DXLine(){ clear();}
	
};

/////////////////////////





/////////////////////////
//MMATRIX CURSOR
class CXLine{
ILine il;
public:
MMatrixX *root, *xline;

CXLine(MMatrixX *rot){root=0; xline=rot;}
CXLine(MMatrixX rot){root=0; xline=&rot;}

operator void*(){return xline;}

//MString& s(){return xline->string;}

public:
	int Prev(){
		if (xline->_p) {xline = xline->_p; return 1;}
		return 0;
	}

	int Next(){
		if (xline->_n) {xline = xline->_n; return 1;}
		return 0;
	}

	int Up(){
		if(xline->_a) { il.insert((int)root); root=xline; xline=xline->_a; return 1;}
		return 0;
	}

	int EUp(){
		if(xline->_e) { il.insert((int)root); root=xline; xline=xline->_e; return 1;}
		return 0;
	}

	int Down(){
		xline=root;
		if(il.e) { root=(MMatrixX *)il.e->i;  il.del(il.e); return 1;}
		else root=0;
		return 0;
	}

	~CXLine(){il.delall();}

};

 