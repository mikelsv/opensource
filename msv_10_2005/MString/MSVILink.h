// MSV Link Library
//22.01.05 (build 13)

//typedef struct ILinkQ{
//MString k;
//MString v;
//} ILinkQ;




class ILinkQ;


// NEW MATRIX 
 // your class Index

   
/*
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
	DLine*ind=_minsert_(this, (DLine*)1); // 
	ind->s=s;
	return ind;
	}

	DLine* insert(){ //insert
	return _minsert_(this, (DLine*)1); // 
	}

	void del(DLine * ind){ //delete
		_mdelete_(this, ind);
		return ;
	
	 }

	void delall(){ //delete
		_mdeleteall(this, this->a);
		return ;
	
	 }

	~Line(){ delall();}
};*/



// new class : public MMatrix
class DILinkQ
{
public:
DILinkQ *n;		// class data
MString k;			// add your data
MString v;			// add your data
};


class ILinkQ
{
public:
DILinkQ *a, *e;	// class cell
unsigned int sz;

	ILinkQ (){a=e=0; sz=0;}	//new
	
	DILinkQ* insert(MString k, MString v){ //insert
	DILinkQ*ind=_minsert_(this, (DILinkQ*)1); // 
	ind->k=k; ind->v=v;
	return ind;
	}

	DILinkQ* insert(){ //insert
	return _minsert_(this, (DILinkQ*)1); // 
	}

	void del(DILinkQ * ind){ //delete
		_mdelete_(this, ind);
		return ;
	
	 }

	void delall(){ //delete
		_mdeleteall(this, this->a);
		return ;
	
	 }
};

typedef struct ILink{
MString ltype;
MString disk;
MString domain;
MString port;
MString path;
MString file;
MString name;
MString ext;
MString ipath;
MString iquest;
ILinkQ ilinkq;
}ILink;


void Ilink(ILink &ret, MString link);
int IlinkQ(ILinkQ *ret, MString line);
MString RetQ(const ILinkQ &lq, const MString &key);
int Lexplode(ILinkQ &ret, const MString &line);
MString HTI(const MString &line);



//void Ilink(ILink &ret, MString link);
//int IlinkQ(ILinkQ *ret, MString line);
//MString RetQ(ILinkQ *lq, MString key, int slq);
//int Lexplode(ILinkQ &ret, const MString &line);
//MString HTI(const MString &line);


void Ilink(ILink &ret, MString link){
if(!link) return ; 

int err;

MString qu[2];
err=explode(qu, link, "?", 1, 1);
if (err==2){ ret.iquest=qu[1]; //IlinkQ(ret.ilinkq, qu[1]); }
Lexplode(ret.ilinkq, qu[1]); }
MString rt=ret.ipath=qu[0];

int s=0;
while(s<rt.GetLength()){
if (rt[s]==':'){ break;}
else { ret.disk+=rt[s];}
if(rt[s]=='\\') {ret.disk.Empty(); ret.ltype='l'; ret.disk.Empty(); break;}
if(rt[s]=='/') {ret.disk.Empty(); ret.ltype='n'; ret.disk.Empty(); break;}
s++;							   
}


if (ret.disk){	  // ret.disk!=""
s+=3;
if(ret.disk.GetLength()>1) ret.ltype='n';
else {ret.ltype='l';s--;}
}
else s=0;


int ss=rt.GetLength()-1;MString file;
while((rt[ss]!='\\' && rt[ss]!='/') && rt[ss]){
file=MString(rt[ss])+file;
ss--;
}

if (file){
MString tmp[2];
err=explode(tmp, file, ".", 1, 1, 1);
if (err==2) {ret.ext=tmp[0]; ret.name=tmp[1]; ret.file=tmp[1]+"."+tmp[0];}
else {ret.name=ret.file=file;}
}

MString path;
if(s>ss) path=rt.Mid(s,ss-s);
if (ret.ltype=="n") {
MString tmp[2];
err=explode(tmp, path, "/", 1, 1);
ret.path=tmp[1];
if (err==2) {
MString temp[2];
explode(temp, tmp[0], ":", 1, 1);
ret.domain=temp[0];ret.port=temp[1];
}
else ret.domain=tmp[0];

}
else ret.path=path;

return ;
}

/*
int IlinkQ(ILinkQ *ret, MString line){
MString kv[512];
int skv=explode(kv,line, "&", 0);if (line.ok() && skv<0) skv=1; 
for (int i=0;i<skv;i++){
MString rkv[2];
explode(rkv, kv[i], "=", 1 , "a");
ret[i].k=rkv[0];ret[i].v=rkv[1];
}
return skv;
}*/
	  
	  
MString RetQ(const ILinkQ &lq, const MString &key){
DILinkQ * data=lq.a;

while(data){
if (key==data->k) return data->v;
data=data->n;
}
	
return "";
}

//////////
int Lexplode(ILinkQ &ret, const MString &line){
DILinkQ *ind;
int l=0, pos=0, lpos=0, err, c;

while(1){
err=rts(line, "&", pos);
if (err) { ind=ret.insert(); err=rtmst(line, "=", c=lpos, pos+1);
		if (err) { ind->k.Reserv(c-lpos); memcpy(ind->k.rchar(), line.pos(lpos), c-lpos); c++;
					ind->v.Reserv(pos-c); memcpy(ind->v.rchar(), line.pos(c), pos-c); ind->v=HTI(ind->v);}
		else {ind->k.Reserv(pos-lpos); memcpy(ind->k.rchar(), line.pos(lpos), pos-lpos);}
}
else {if (lpos!=pos){ ind=ret.insert(); err=rtmst(line, "=", c=lpos, pos+1);
		if (err) { ind->k.Reserv(c-lpos); memcpy(ind->k.rchar(), line.pos(lpos), c-lpos); c++;
					ind->v.Reserv(pos-c); memcpy(ind->v.rchar(), line.pos(c), pos-c); ind->v=HTI(ind->v);}
		else {ind->k.Reserv(pos-lpos); memcpy(ind->k.rchar(), line.pos(lpos), pos-lpos);}	
}
	 return l;}
pos++; lpos=pos;
}

return 0;
}


MString HTI(const MString &line){
MString ret;
int sz=line.GetLength();
for(int i=0;i<sz;i++){
	if(line[i]=='+') {ret+=" "; continue;}
if(line[i]=='%'){
	int ar[2];char car[2];
	car[0]=line[i+1];car[1]=line[i+2];
	for(int ii=0;ii<2;ii++){
		if (int(car[ii])>47 && int(car[ii])<58) ar[ii]=int(car[ii])-48;
		if (int(car[ii])>64 && int(car[ii])<71) ar[ii]=int(car[ii])-55;
	}


ret+=MString(char(ar[0]*16+ar[1]));i++;i++;
}
else { ret+=line[i];}


}

return ret;
}