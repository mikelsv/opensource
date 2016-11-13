/*
 * Company:	MSV (MikelSV) code name Vyris
 * This: MMatrix - My Matrix class
 *		 My version Array class
 */
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	MMatrix version 1.2.0 16/Jan/2005

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

#define		MMATRIX_VER "1.2.0"
/*

class MMatrix{
public:
MMatrix *n, *p;
};



class DMatrix
{
public:
int sz;
MMatrix *a, *e;	


DMatrix(){a=e=0; sz=0;}

template < class M>
M* _insert(M*m){

	if(sz<1){
        a=new M;
		//sz=a->sz=0;
        a->p=NULL;
        a->n=NULL;
		//a->a=a->e=NULL;
		e=a;
		sz=1;
        return (M*)a;
    }
    e->n=new M;
    e->n->p=e;
    e->n->n=NULL;
	e=e->n;
	//e->e=e->a=NULL;
	//e->sz=0;
    sz++;
    return (M*)e;
}

template < class M>
int del(M*ind)
{
    if(sz==0)
        return 0;
	M *ni,*pi,*dl;
	if (ind->n) dl=ind->n->p;
	else if (ind->p) dl=ind->p->n;
	else {delete a; a=e=0; sz=0; return 1;}
    if (ind->n)  ni=ind->n; else 	ni=NULL;
	if (ind->p)  pi=ind->p; else 	pi=NULL;
	if (ni) ni->p=pi;
	else  a=ind->p;
	if (pi) pi->n=ni;
	else  a=ind->n;
	delete dl;
    sz--;
    return 1;
}


};				 */


// MMATRIX
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
	else  t->a=ind->p;
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





 /*

// NEW MATRIX 
class DSQL; // your class Index


// new class
class DiSQL
{
public:
DiSQL *n,*p;		// class data
int *rsql;			// add your data
};


class DSQL
{
public:
DiSQL *a, *e;	// class cell

	DSQL (){}	//new
	
	void insert(){ //insert
	DiSQL*ind=DMatrix::insert((DiSQL*)0); // 
	// ind-> you param = value;
	return ;
	}

	void del(DiSQL * ind){ //delete
		DMatrix::del(ind);
		return ;
	
	 }
};

	  */





 /*

class DMatrix{


	MMatrix * insert();
	del();


// TEMPLATE FUNCTIONS
template<class Ty>
class DtMatrix{
int f;
};


template<class Ty>
   bool tmps(DtMatrix<Ty>& op)
{
int x=0;
}


};

			 
*/




/*
DMatrix * MMatrix::insert()
// добавление элемента в стэк
{
    
	if(sz<1){
        arr=new pind;
		sz=arr->sz=0;
        arr->prev=NULL;
        arr->next=NULL;
		arr->arr=arr->earr=NULL;
		earr=arr;
		sz++;
        return arr;
    }
    earr->next=new pind;
    earr->next->prev=earr;
    earr->next->next=NULL;
	earr=earr->next;
	earr->earr=earr->arr=NULL;
	earr->sz=0;
    sz++;
    return earr;
}
		 

DMatrix::del()
{
    if(sz==0)
        return 0;
	pind *ni,*pi;
	pind*dl;
	if (this->next) dl=this->next->prev;
	else if (this->prev) dl=this->prev->next;
	else {delete arr; sz--; return 1;}
    if (this->next)  ni=this->next; else 	ni=NULL;
	if (this->prev)  pi=this->prev; else 	pi=NULL;
	if (ni) ni->prev=pi;
	if (pi) pi->next=ni;
	else  arr=this->next;
	delete dl;
    sz--;
    return 1;
}
		  

pind * pind::findk(MString key)
{
    if(sz==0)
        return 0;
	pind*ar=arr;
	while(1){
	if (ar->key==key) return ar;

	if (ar->next) ar=ar->next;
	else return 0;
	}
	
}

pind::set(MString val)
{
	this->val=val; return 1;
	
}		   */



