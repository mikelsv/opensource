/*

MString HTI(const MString &line){
LString ret;
int sz=line.size();
for(int i=0;i<sz;i++){
	if(line[i]=='+') {ret+=" "; continue;}
if(line[i]=='%'){
	int ar[2];char car[2];
	car[0]=line[i+1];car[1]=line[i+2]; ar[0]=0; ar[1]=0;
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

MString HTIR(const VString &line){
MString ret;
unsigned char *ln=line, *to=line.endu(), *r; int sz=0;
for(ln; ln<to; ln++){ if(*ln<32 || *ln>127 || *ln=='%' || *ln=='+') sz+=2; }
ln=line; ret.Reserv(sz+line.size()); r=ret;
for(ln; ln<to; ln++){
	if(*ln<32 || *ln>127 || *ln=='%' || *ln=='+') {*r++='%'; *r++=cb16[*ln/16]; *r=cb16[*ln%16]; } else if(*ln==' ') *r='+'; else *r=*ln; r++;
}
return ret;
}

int Lexplode(TLine &ret, const VString &line){
DTLine *ind;
int l=0, pos=0, lpos=0, err, c;

while(1){
err=rts(line.uchar(), line, '&', pos);
if (err) { ind=ret.insert(); err=rts(line.data, pos, '=', c=lpos);
		if (err) { ind->s.Reserv(c-lpos); memcpy(ind->s.rchar(), line.pos(lpos), c-lpos); if(pos>c) c++;
					ind->t.Reserv(pos-c); memcpy(ind->t.rchar(), line.pos(c), pos-c); ind->t=HTI(ind->t);}
		else {ind->s.Reserv(pos-lpos); memcpy(ind->s.rchar(), line.pos(lpos), pos-lpos);}
}
else {if (lpos!=pos){ ind=ret.insert(); err=rts(line.data, pos, '=', c=lpos);
		if (err) { ind->s.Reserv(c-lpos); memcpy(ind->s.rchar(), line.pos(lpos), c-lpos); if(pos>c) c++;
					ind->t.Reserv(pos-c); memcpy(ind->t.rchar(), line.pos(c), pos-c); ind->t=HTI(ind->t);}
		else {ind->s.Reserv(pos-lpos); memcpy(ind->s.rchar(), line.pos(lpos), pos-lpos);}
}
	 return l;}
pos++; lpos=pos;
}

return 0;
}*/


/*class ILink{
public:
//MString data;
MString proto;
MString domain;
MString path;
MString file;
MString name;
MString ext;
MString ipath;
MString iquest;
unsigned short port; bool ltype;
TLine ilinkq;
int largs;*/

void ILink::Clean(){
	//proto.Empty(); domain.Empty(); path.Empty(); file.Empty(); name.Empty(); ext.Empty(); ipath.Empty(); iquest.Empty();
	memset((char*)&proto, 0, (char*)&iquest-(char*)&proto);
	
	//ilinkq.delall();
	port=0; ltype=0; args=0; portsz=0; largs=0; ilinkq=0; mdata.Clean(); return ;
}

//void Ilink(const MString &ilink){ return Ilink(VString(ilink)); }
	ILink::ILink(){ MSVMEMORYCONTROLC Clean(); }
	ILink::~ILink(){ MSVMEMORYCONTROLD }

void ILink::Link(const VString &ilink, int tp){// 0 sel, 1 - web, 2 -local
Clean(); if(!ilink) return ; 
VString _proto, _domain, _path, _file, _name, _ext, _ipath, _iquest, _target;

//int questpos=0;// ilink.sz;//, pos=0, lpos=0;
unsigned char *q1=0, *q2=0;
if(tp!=2 && (q1=ilink)!=0 && rts(ilink.endu(), '?', q1)) _iquest.data=q1;
else if(tp!=1 && (q2=ilink)!=0 && rts(ilink.endu(), ' ', q2)) _iquest.data=q2;
else _iquest.data=ilink.endu();
_iquest.sz=ilink.endu()-_iquest.data;


//_iquest.data=ilink;
//if(tp!=2 && ){  }
//else if(tp!=1 && rts(ilink.data, ' ', _iquest.data=ilink)){  }
//else questpos=ilink.sz;

// find target
_target.data=ilink;
rts(ilink.endu(), '#', _target.data);
if(_target.data<_iquest.data){ _iquest.data=_target.data; }
_iquest.sz=_target.data-_iquest.data;
_target.sz=ilink.endu()-_target.data;

unsigned char*line=ilink, *ln=line, *to=_iquest.data, *p;
if(to>line) _ipath.setu(line, to-line);

for(line; line<to; line++){
	if(*line==':'){ line++; if(line>=to){ _domain.setu(ln, line-ln); ltype=1; ln=line; continue; }
	switch(*(line)){
		case '\\': ltype=1;
			if(line>ln){ _domain.setu(ln, line-ln); ln=line; }
			//while(*line=='\\' || *line=='/') line++; ln=line;
		break;
		case '/': ltype=0;
			if(line>ln){
				while(*line=='\\' || *line=='/') line++;
				_proto.setu(ln, line-ln);
			} ln=line;

			p=0;
			while(line<to && *line!='\\' && *line!='/'){ if(*line==':') p=line; line++; }
			if(line>ln){
				if(p){ portsz++;
					_domain.setu(ln, p-ln); p++;
					port=0;
					for(p; p<to; p++){ if(*p>='0' && *p<='9'){ port=port*10+(*p)-'0'; portsz++; } else break;}
				} else { _domain.setu(ln, line-ln);}
				ln=line;
				//while(*line=='\\' || *line=='/') line++; 
			}
		break;
		default :
		break;
	}break;
}}

// path = line to to - file;
//if(line<to) path.set(line, to-line-1);
line=to; p=0; //while(*ln=='\\' || *ln=='/') {ln++;}
for(line; line>=ln; line--){
//spth:
	if(*line=='\\' || *line=='/' || line==ln){ if(*line=='\\' || *line=='/') line++;
		if(line>ln) { _path.setu(ln, line-ln); }// if(*line=='\\' || *line=='/') line++;
		if(to>line) _file.setu(line, to-line);
		ln=line; p=line;
     break;
	}
}// if(!p) {goto spth;}

/*
line=to; p=0;
for(line; line>ln; line--){
	if(*line=='.'){
sfnm:
		if(line>ln){ _name.set(ln, line-ln); }
		if(*line=='.'){ line++; if(to>line) _ext.set(line, to-line); }
		else _name.set(line, ln-line);
		ln=line; p=line;
     break;
	}
}if(!p){ goto sfnm; }
*/

// if
if(_target.data<ilink.endu() && *_target.data=='#'){ _target.data++; _target.sz--; }
if(_iquest.data<ilink.endu() && *_iquest.data=='?'){ _iquest.data++; _iquest.sz--; }

// quest
//ln=(char*)ilink.data+questpos+1; to=(char*)_target.data; char*lln;
if(_iquest){
	args=0;	AnalysIQuest(_iquest);
	//char*ln=_iquest, *lln=ln, *to=_iquest.end();
	/*ln=_iquest, lln=ln, to=_iquest.end();
	for(ln; ln<to; ln++){ if(*ln=='&' || *ln==' '){ if(lln!=ln) args++; lln=ln+1; } }
	if(lln!=ln) args++;
	if(!args) ilinkq=0; */
} else{ args=0; ilinkq=0; }

	//mdata.Reserv(_proto, _domain, _path, _file, _name, _ext, _ipath, _iquest
mdata.Add(_proto, VString(_domain.rchar(), _domain.size()+portsz), _path, _file, "?", _iquest, "#", _target, ilink.size()>sizeof(DUVS)*args ? ilink.str(0, sizeof(DUVS)*args) : mdata.Reserv(sizeof(DUVS)*args)); // , _name, _ext
	//ln=mdata;
	MoveData(mdata, 0, _proto, _domain, _path, _file, _iquest, _target);
//	((VString*)&proto)->set(ln, _proto); ln+=_proto;
//	((VString*)&domain)->set(ln, _domain); ln+=_domain;
//	((VString*)&path)->set(ln, _path); ln+=_path;
//	((VString*)&file)->set(ln, _file); ln+=_file;
//	*((VString*)&name)=((VString*)&file)->str(0, _name);
//	*((VString*)&ext)=((VString*)&file)->str(-_ext.size());
//	//((VString*)&name)->set(ln, _name); ln+=_name;
//	//((VString*)&ext)->set(ln, _ext); ln+=_ext;
//	((VString*)&ipath)->set(ln, _ipath); ln+=_ipath;
//	((VString*)&iquest)->set(ln, _iquest); ln+=_iquest;
//	ilinkq=(DUVS*)ln;

	AnalysIQuestSet(iquest);
	/*
	ln=iquest, lln=ln, to=iquest.end(); char *cm=ln; int i=0;
	for(ln; ln<=to; ln++){
		if(*ln=='&' || *ln==' ' || ln==to){  if(lln==ln){ lln=ln; continue; }
			if(*cm=='='){ ilinkq[i].key.set(lln, cm-lln); ilinkq[i].val.set(cm+1, ln-cm-1); }
			else { ilinkq[i].key.set(lln, ln-lln); ilinkq[i].val.Empty(); }
			lln=ln+1; cm=lln; i++;
		}
		else if(*ln=='=') cm=ln;
	}*/
	//if(lln!=ln) args++;
	//Lexplode(ilinkq, iquest);

//if(ln>=to) return ;
//if(iquest[0]=='?')
return ;
}

void ILink::AnalysIQuest(const VString &_iquest){
	char *ln=_iquest, *lln=ln, *to=_iquest.end(); if(!ln){ args=0; return ; }
	char *cm=ln; int i=0;
	for(ln; ln<=to; ln++){
		if(*ln=='&' || *ln==' ' || ln==to){  if(lln==ln){ continue; }
			//if(*cm=='='){ ilinkq[i].key.set(lln, cm-lln); ilinkq[i].val.set(cm+1, ln-cm-1); }
			//else { ilinkq[i].key.set(lln, ln-lln); ilinkq[i].val.Empty(); }
			lln=ln+1; i++; //cm=lln;
		}
		//else if(*ln=='=') cm=ln;
	}

	args=i;

	//char *ln=_iquest, *lln=ln, *to=_iquest.end();
	//for(ln; ln<to; ln++){
	//	if(*ln=='&' || *ln==' '){ if(lln!=ln) args++; lln=ln+1; } }
	//if(lln!=ln) args++;
	//if(!args) ilinkq=0;
return ;
}

void ILink::AnalysIQuestSet(const VString &_iquest){
	char *ln=_iquest, *lln=ln, *to=_iquest.end(); if(!ln){ args=0; return ; }
	char *cm=ln; int i=0, f=1;
	for(ln; ln<=to; ln++){
		if(*ln=='&' || *ln==' ' || ln==to){ f=1;  if(lln==ln){ continue; }
			if(*cm=='='){ ilinkq[i].key.set(lln, cm-lln); ilinkq[i].val.set(cm+1, ln-cm-1); }
			else { ilinkq[i].key.set(lln, ln-lln); ilinkq[i].val.Clean(); }
			lln=ln+1; cm=lln; i++;
		}
		else if(*ln=='=') if(f){ cm=ln; f=0; }
	}
	for(i; i<args; i++){ ilinkq[i].key.set(VString()); ilinkq[i].val.set(VString()); }
return ;
}

void ILink::MoveData(char*ln, int mo, int pr, int dom, int pa, int fi, int iq, int tg){
	((VString*)&proto)->set(ln, pr); ln+=pr;
	((VString*)&domain)->set(ln, dom); ln+=dom; ln+=portsz;
	((VString*)&path)->set(ln, pa); ln+=pa;
	((VString*)&file)->set(ln, fi); ln+=fi;
//	*((VString*)&name)=((VString*)&file)->str(0, nm);
//	*((VString*)&ext)=((VString*)&file)->str(-ex);
	*ln++='?';
	//((VString*)&name)->set(ln, _name); ln+=_name;
	//((VString*)&ext)->set(ln, _ext); ln+=_ext;
	//((VString*)&ipath)->set(ln, ip); ln+=ip;
	((VString*)&iquest)->set(ln, iq); ln+=iq;
	*ln++='#';
	((VString*)&target)->set(ln, tg); ln+=tg;
	//int m=(char*)ilinkq-ln;
	unsigned char*lq=(unsigned char*)ilinkq;
	ilinkq=(DUVS*)ln;
	if(mo) for(int i=0; i<args; i++){
		ilinkq[i].key.data=(unsigned char*)ilinkq[i].key.data-lq+(unsigned char*)ilinkq;
		ilinkq[i].val.data=(unsigned char*)ilinkq[i].val.data-lq+(unsigned char*)ilinkq;
	}
	//if(ln+args*sizeof(DUVS)!=mdata.end())
	//	int tyry=657;
}

// Get
VString ILink::name(){ char*ln=file.end(), *to=file; if(!file) return file; while(ln>=to){ if(*ln=='.') return VString(to, ln-to); ln--; } return file; }
VString ILink::ext(){ char*ln=file.end(), *to=file; if(!file) return file; while(ln>=to){ if(*ln=='.') return VString(ln+1, file.end()-ln-1); ln--; } return VString(); }

// Find
DUVS* ILink::Find(const VString &line){
	for(int i=0; i<args; i++){
	if(ilinkq[i].key==line) return &ilinkq[i];
	} return 0;
}


// Positions
char* ILink::ProtoPos(){ if(proto) return proto; return mdata; }
char* ILink::DomainPos(){ if(domain) return domain; if(proto) return proto.end(); return mdata; }
char* ILink::PathPos(){ if(path) return path; return DomainPos()+domain.size()+portsz; }
char* ILink::FilePos(){ if(file) return file; return PathPos()+path.size(); }
char* ILink::QuestPos(){ if(iquest) return iquest; return FilePos()+file.size()+1; }

// Set
void ILink::SetProto(const VString nproto){
int mv=nproto.size()-proto.size(); int vp=ProtoPos()-mdata.rchar();
if(mv>0){ mdata.Reserv(mdata.size()+mv+(mdata.size()==0)); }
//else if(mv){ mdata.sz+=mv; mv=0; }
char*mvp=mdata.rchar()+vp;
memcpy(mvp+mv+proto.sz, mvp+proto.sz, mdata.size()-vp-mv-proto.sz);
//memcpy(mvp, mvp-mv, mdata.size()-vp+mv);

if(vp-mv+mdata.size()-vp+mv!=mdata.size())
		int tyry=657;

memcpy(mvp, nproto, nproto);
((VString*)&proto)->set(mvp, nproto);
if(mv<0){ mdata.sz+=mv; }
MoveData(mdata, mv, proto, domain, path, file, iquest, target);
return ;
}

void ILink::SetDomain(const VString &ndomain){ if(!ndomain) return ;
char*ln=ndomain, *to=ln+ndomain.sz, *line=to;
VString _domain; port=0; int lportsz=portsz;   //domain.Empty(); port=0; 

line=to; portsz=0;
for(line; line>=ln; line--){
	if(*line==':' || line==ln){ if(*line!=':') line=to;
//sfnm:
	if(line>ln) {_domain.set(ln, line-ln); } if(*line==':'){ portsz++; line++; }
		//if(to>line) ext.set(line, to-line);
		for(line; line<to; line++){ if(*line>='0' && *line<='9') port=port*10+(*line)-'0'; portsz++; } if(portsz>6) portsz=0;
		ln=line;
     break;
	}
}// line=to; goto sfnm;

int mv=_domain.size()-domain.size()+portsz-lportsz; int vp=DomainPos()-mdata.rchar();
if(mv>0){ mdata.Reserv(mdata.size()+mv+(mdata.size()==0)); }
//else if(mv){ mdata.sz+=mv; mv=0; }
char *mvp=mdata.rchar()+vp;
memcpy(mvp+mv+domain.sz, mvp+domain.sz, mdata.size()-vp-mv-domain.sz);
//memcpy(mvp+mv, mvp, mdata.end()-mvp-mv);
memcpy(mvp, _domain, _domain.size()+portsz);
if(mv<0){ mdata.sz+=mv; }
MoveData(mdata, mv, proto, _domain, path, file, iquest, target);
return ;
}

void ILink::SetPath(const VString &npath){
int mv=npath.size()-path.size(); int vp=PathPos()-mdata.rchar();
if(mv>0){ mdata.Reserv(mdata.size()+mv+(mdata.size()==0)); }
//else if(mv){ mdata.sz+=mv; mv=0; }
char*mvp=mdata.rchar()+vp;
memcpy(mvp+mv+path.sz, mvp+path.sz, mdata.size()-vp-mv-path.sz);
//memcpy(mvp, mvp-mv, mdata.size()-vp+mv);

if(vp-mv+mdata.size()-vp+mv!=mdata.size())
		int tyry=657;

memcpy(mvp, npath, npath);
((VString*)&path)->set(mvp, npath);
if(mv<0){ mdata.sz+=mv; }
MoveData(mdata, mv, proto, domain, path, file, iquest, target);
return ;
}

void ILink::SetFile(const VString &nfile){
int mv=nfile.size()-file.size(); int vp=FilePos()-mdata.rchar();
if(mv>0){ mdata.Reserv(mdata.size()+mv+(mdata.size()==0)); }
//else if(mv){ mdata.sz+=mv; mv=0; }
char*mvp=mdata.rchar()+vp;
memcpy(mvp+mv+file.sz, mvp+file.sz, mdata.size()-vp-mv-file.sz);
//memcpy(mvp, mvp-mv, mdata.size()-vp+mv);

//memcpy(mvp+mv, mvp, mdata.end()-mvp-mv);
memcpy(mvp, nfile, nfile);
((VString*)&file)->set(mvp, nfile);
if(mv<0){ mdata.sz+=mv; }
MoveData(mdata, mv, proto, domain, path, file, iquest, target);

//char*ln=mvp, *to=ln+nfile.sz, *line=to;
/*	line=to;
for(line; line>=ln; line--){
	if(*line=='.' || line==ln){
		if(line>ln){ ((VString*)&name)->set(ln, line-ln); } if(*line=='.') line++;
		if(to>line) ((VString*)&ext)->set(line, to-line);
		ln=line;
     break;
	}
}*/
return ;
}

void ILink::SetQuest(const VString &nquest){
int mv=nquest.size()-iquest.size(); int vp=QuestPos()-mdata.rchar();
int ar=args; AnalysIQuest(nquest);
if(mv>0){ mdata.Reserv(mdata.size()+mv+(mdata.size()==0)+(args-ar)*sizeof(DUVS)); }
char*mvp=mdata.rchar()+vp;
memcpy(mvp+mv+iquest.sz, mvp+iquest.sz, mdata.size()-vp-mv-iquest.sz-(ar<args ? (args-ar)*sizeof(DUVS) : 0));
//memcpy(mvp, mvp-mv, mdata.size()-vp+mv-(ar<args ? (args-ar)*sizeof(DUVS) : 0));
//memcpy(mdata.rchar()+vp, mdata.rchar()+vp-mv, mdata.size()-vp-mv-(ar<args ? (args-ar)*sizeof(DUVS) : 0));

if(vp-mv+mdata.size()-vp+mv!=mdata.size())
		int tyry=657;

if(mv<0){ mdata.sz+=mv; }
//memcpy(mvp+mv, mvp, mdata.end()-mvp-mv-(ar<args ? (args-ar)*sizeof(DUVS) : 0));
memcpy(mvp, nquest, nquest);
((VString*)&iquest)->set(mvp, nquest);
MoveData(mdata, 0, proto, domain, path, file, iquest, target);
AnalysIQuestSet(iquest);
return ;
}

/*VString ILink::GetFullPath(){
HLString hl;
globalerror("UPDATE IT! ILink::GetFullPath()");
//if(ltype) {if(domain) hl+domain+":\\";}
//else { if(proto) hl+=proto+"://"; if(domain){ hl+domain; if(port) hl+=":"+itos(port); hl+="/";} }
//if(path) hl+=path+ (ltype ? "\\" : "/"); hl+file; if(iquest) hl+"?"+iquest;
return hl;
}*/


VString ILink::GetOnlyPath(){ return VString(proto.rchar(), proto.size()+domain.size()+path.size()+file.size()); }
VString ILink::GetPathFile(){ return VString(path.rchar(), path.size()+file.size()); }
VString ILink::GetPathFileQuest(){ return VString(path.rchar(), path.size()+file.size()+(iquest ? iquest.size()+1 : 0)); }

VString ILink::GetProtoDomainPath(){
	return VString(proto.rchar(), proto.size()+domain.size()+path.size());
}

VString ILink::GetFullPath(){
	return VString(proto.rchar(), proto.size()+domain.size()+path.size()+file.size()+iquest.size()+portsz+(iquest.size()>0));
}



//HLString hl;
//globalerror("UPDATE IT! ILink::GetPath()");
//if(ltype) {if(domain) hl+domain+":\\";}
//else { if(proto) hl+=proto+"://"; if(domain){ hl+domain; if(port) hl+=":"+itos(port); hl+="/";} }
//if(path) hl+=path+ (ltype ? "\\" : "/"); //hl+file; if(iquest) hl+"?"+iquest;
//return hl;


void ILink::FirstArg(){ largs=0; }

VString ILink::GetArg(){
VString ret; if(largs>=iquest.size()) return ret;
unsigned char *ln=iquest.uchar()+largs, *to=iquest.endu();
rtns(to, ' ', ln); ret.data=ln; char t=0;
for(ln; ln<to; ln++){
	if(!t){ if(*ln=='\'') t=1; else if(*ln=='"') t=2; else if(*ln==' ') break; }
	else if(t==1 && *ln=='\'' || t==2 && *ln=='"'){ t=0; }	
}
//rts(to, ' ', ln);
ret.sz=ln-ret.data; largs=ln-iquest.uchar(); return ret;
}

VString ILink::GetLastArg(){
	VString ret; if(largs>=iquest.size()) return ret;
	unsigned char *ln=iquest.uchar()+largs, *to=iquest.endu();
	rtns(to, ' ', ln); ret.data=ln;
	ret.sz=to-ln; largs=iquest.size();
	return ret;
}

VString ILink::GetArg(VString key){
	for(int i=0; i<args; i++){
		if(ilinkq[i].key==key) return ilinkq[i].val;
	}
	return VString();
}

//VString ILink::GetArg(VString ret){
//	unsigned char *ln=iquest.uchar(), *to=iquest.endu();
//	while(rtms(to, ret, ret, ln)){
//		if((ln==iquest.uchar() || *(ln-1)==' ') && (ln+ret.size()<to || *(ln+ret.size())=='=')){
//			ln+=ret.size()+1;
//			unsigned char*p=ln; 
//
//			rts(to, ' ', ln);
//			if(ln>p){ VString ret(p, ln-p); return ret;  }
//		} ln++;
//	}
//return VString();
//}

int ILink::GetArgSz(){
	int s=0; int la=largs;
	largs=0;
	while(GetArg()){ s++; }
	largs=la;
return s;
}


//VString PartArg(VString line, VString &two, VString el){
//	unsigned char *pos=line;
//	if(!rtms(line, line, el, el, pos)){ two.Clean(); return line; }
//	two.setu(pos+el.sz, line.endu()-pos-el.sz);
//	return VString(line.uchar(), pos-line.uchar());
//}

/*
VString PartArgF(VString line, VString &two, VString el, VString el2){
	unsigned char *pos=line, *pos2=line;
	rtms(line, line, el, el, pos); rtms(line, line, el2, el2, pos2);
	if(pos>=line.endu() && pos2>=line.endu()){ two.Clean(); return line; }
	if(pos<pos2){
		two.setu(pos+el.sz, line.endu()-pos-el.sz);
		return VString(line.uchar(), pos-line.uchar());
	}
	else{
		two.setu(pos2+el.sz, line.endu()-pos2-el.sz);
		return VString(line.uchar(), pos2-line.uchar());
	}
}

int PartArgM(VString line, VString *two, int sz, VString el, int a){
	unsigned char *pos=line, *lpos=pos; int ct=0;
	while(1){
		if(rtms(line, line, el, el, pos)){ 
			if(ct==sz && a){ two[ct].setu(lpos, line.endu()-lpos); ct++; return ct; }

			two[ct].setu(lpos, pos-lpos); ct++; pos+=el.sz; lpos=pos;
			if(ct==sz) return ct;
		}
		else { if(pos-lpos){ two[ct].setu(lpos, line.endu()-lpos); ct++; return ct; } }
	}
	return ct;
}
*/

//void Ilink(ILink &ilink, const VString &path, int tp){ return ilink.Link(path, tp); }

