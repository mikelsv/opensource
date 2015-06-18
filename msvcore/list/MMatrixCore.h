// MMATRIX



// <- & ->	DOUBLE

// INSERT

// insert - prev, next
void* _minsert(MMatrixR* root, MMatrixD* data){

	if(root->_sz<1){
        root->_a=data;
		root->_e=data;
        data->_p=NULL;
        data->_n=NULL;
		root->_sz=1;
        return root->_a;
    }

    root->_e->_n=data;
    data->_p=root->_e;
    data->_n=NULL;
	root->_e=data;
    root->_sz++;
    return root->_e;


return 0;
}



//DELETE

//delete - next, prev	automatic delete
int _mdelete(MMatrixR* root, MMatrixD* data)
{
    if(root->_sz==0)
        return 0;
	MMatrixD *ni,*pi,*dl;
	if (data->_n) dl=data->_n->_p;
	else if (data->_p) dl=data->_p->_n;
	else {delete root->_a; root->_a=root->_e=0; root->_sz=0; return 1;}
    if (data->_n) ni=data->_n; else ni=NULL;
	if (data->_p) pi=data->_p; else pi=NULL;
	if (ni) ni->_p=pi;
	else  root->_e=data->_p;
	if (pi) pi->_n=ni;
	else  root->_a=data->_n;
	delete dl;
    root->_sz--;
    return 1;
}


//delete - next, prev	user delete
void* _mdelete_u(MMatrixR* root, MMatrixD* data)
{
    if(root->_sz==0)
        return 0;
	MMatrixD *ni,*pi,*dl;
	if (data->_n) dl=data->_n->_p;
	else if (data->_p) dl=data->_p->_n;
	else {dl=root->_a; root->_a=root->_e=0; root->_sz=0; return dl;}
    if (data->_n) ni=data->_n; else	ni=NULL;
	if (data->_p) pi=data->_p; else	pi=NULL;
	if (ni) ni->_p=pi;
	else  root->_e=data->_p;
	if (pi) pi->_n=ni;
	else  root->_a=data->_n;
	root->_sz--;
    return dl;
}




// delete all for all types		automatic delete all
int _mdeleteall(MMatrixR* root, MMatrixD* data)
{
//  if(root->_sz==0)
//        return 0;
	
	MMatrixD *dl=root->_a, *n;


	while(dl){
	n=dl->_n;
	delete dl;
	dl=n;
	}

	root->_sz=0; root->_a=0; root->_e=0;
    return 1;
}


/*
// delete all for all types		size delete all
int _mdeleteall(MMatrixR* root, MMatrixD* data, class *clr)
{
//    if(root->_sz==0)
//      return 0;
	
	MMatrixD *dl=root->_a, *n;


	while(dl){
	n=dl->_n;
	delete [sizeof(clr)] dl;
	dl=n;
	}

	root->_sz=0; root->_a=0; root->_e=0;
    return 1;
}
*/




// MMATRIXX

// <- & -> & /\ & \/

// INSERT

// insert - prev, next
void* _minsertx(MMatrixX* root, MMatrixX* data){

	if(root->_sz<1){
        root->_a=root->_e=data;
        data->_p=NULL;
        data->_n=NULL;
		root->_sz=1;
        return root->_a;
    }

    root->_e->_n=data;
    data->_p=root->_e;
    data->_n=NULL;
	root->_e=data;
    root->_sz++;
    return root->_e;


return 0;
}


//DELETE

//delete - next, prev	automatic delete
int _mdeletex(MMatrixX* root, MMatrixX* data)
{
    if(root->_sz==0)
        return 0;
	MMatrixX *ni,*pi,*dl;
	if (data->_n) dl=data->_n->_p;
	else if (data->_p) dl=data->_p->_n;
	else {delete root->_a; root->_a=root->_e=0; root->_sz=0; return 1;}
    if (data->_n) ni=data->_n; else ni=NULL;
	if (data->_p) pi=data->_p; else pi=NULL;
	if (ni) ni->_p=pi;
	else  root->_e=data->_p;
	if (pi) pi->_n=ni;
	else  root->_a=data->_n;
	delete dl;
    root->_sz--;
    return 1;
}



//delete - next, prev	class delete
int _mdeletex(MMatrixX* root, MMatrixX* data, MMatrixXD*cls)
{
    if(root->_sz==0)
        return 0;
	MMatrixX *ni,*pi,*dl;
	if (data->_n) dl=data->_n->_p;
	else if (data->_p) dl=data->_p->_n;
	else {delete root->_a; root->_a=root->_e=0; root->_sz=0; return 1;}
    if (data->_n) ni=data->_n; else ni=NULL;
	if (data->_p) pi=data->_p; else pi=NULL;
	if (ni) ni->_p=pi;
	else  root->_e=data->_p;
	if (pi) pi->_n=ni;
	else  root->_a=data->_n;
	//cls->//(dl));
	delete dl;
    root->_sz--;
    return 1;
}


// delete all for all types		size delete all	 (no) automatic delete
int _mdeleteallx(MMatrixX* root, MMatrixX* data)
{
    if(!root) return 0;
	if(!root->_a) return 0;
	
	MMatrixX *dl=root->_a, *n;

	while(dl){
	n=dl->_n;
	delete dl;	//  [sizeof(clr)]
	dl=n;
	}

	root->_sz=0; root->_a=root->_e=0;
    return 1;
}
	   
// delete all for all types		size delete all	 class delete
int _mdeleteallx(MMatrixX* root, MMatrixXD*cls)
{
    if(!root) return 0;
	if(!root->_a) return 0;
	
	MMatrixX *dl=root->_a, *n;

	while(dl){
	n=dl->_n;
	delete(dl);	//  [sizeof(clr)]
	dl=n;
	}

	root->_sz=0; root->_a=root->_e=0;
    return 1;
}


// CHANGE

//for all types
int _change(MMatrixR* t, MMatrixD*ind, MMatrixD*tind)
{
	if(!ind || !tind)
	{int x=56;}
	MMatrixD*pd=ind->_p;
	if (t->_a==ind) t->_a=tind;
	else ind->_p->_n=tind;
	if (t->_e==tind) t->_e=ind;
	else tind->_n->_p=ind;
	ind->_p=tind;
	ind->_n=tind->_n;
	tind->_n=ind;
	tind->_p=pd;

    return 1;
}
