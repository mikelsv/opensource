// MSV Text Library for MString (MSVLibrary)
//1.12.04 (build 12)
//#include "stdafx.h"
//#include <afxsock.h>
//#include <math.h>

/*				
int explode(MString* ret, MString buf, MString fr, int c, MString param);
int explode(MString* ret, MString buf, MString fr);
MString Res(MString line);
//int rout(char ret, char *buf, int p, int t);
int substr(MString ret, MString buf, int p, int t);
int GetParam(int *ret, MString param);
//MString RetQ(ILinkQ *lq, MString key, int slq);


int explode(MString* ret, MString buf, MString fr, int c, MString param){
int gp[23];GetParam(gp, param);
//a - писать все

int l=0;										   
int sbuf=buf.GetLength();				//добавить обратную сторону
int sfr=fr.GetLength();					//ограничение изменений
if (!c) c=0; 

if (gp[17]) {buf=Res(buf);}	

									//игнороровать первые
for(int i=0;i<sbuf-sfr+1;i++){		//одно из двух, специальные символы.
int sfrd=0;
for(int ii=0;ii<sfr;ii++){ if(buf[i]==fr[ii]) sfrd++;}
if (sfrd==sfr) {if (gp[17]) { ret[l]=Res(ret[l]);} l++; i+=sfr-1;
				if (c>0 && l==c) { if (gp[0]) { i++; for(i;i<sbuf-sfr+1;i++) {ret[l]+=buf[i];} if (gp[17]) ret[l]=Res(ret[l]);} 
				else {return l+1;}
				}	
	}
	else ret[l]+=buf[i];
}



if(l==0) return -1;
else return l+1;
}


int explode(MString* ret, MString buf, MString fr){
int iret=explode(ret, buf, fr, 0, "");
return iret;}

MString Res(MString line){
MString ret;
for(int i=0;i<line.GetLength();i++){
ret=line[i]+ret;
}
return ret;
}

MString implode(MString *arr, int sarr, MString fr){
MString ret;

for(int i=0;i<sarr-1;i++){
ret+=arr[i];ret+=fr;
} ret+=arr[sarr-1];

return ret;
}			*/




int rts(const MString &line, MString fr, int &pos){ //read to submol
for (pos; pos<line.GetLength(); pos++){
	if (line[pos]==fr) return 1;
}
return 0;
}

/////
int rtns(const MString &line, MString fr, int &pos){ //read to not submol
for (pos; pos<line.GetLength(); pos++){
	if (line[pos]!=fr) return 1;
}
return 0;
}

/////
int rrts(const MString &line, MString fr, int &pos){ //read to sumbol to
for (pos; pos>-1; pos--){
	if (line[pos]==fr) return 1;
}
return 0;
}

/////
int rrtns(const MString &line, MString fr, int &pos){ //reserv read to no sumbol
for (pos; pos>-1; pos--){
	if (line[pos]!=fr) return 1;
}
return 0;
}

/////
int rtst(const MString &line, MString fr, int &pos, int to, bool res){ //read to sumbol to
if(to<0) to=line.GetLength();									//  =0
else if (to>line.GetLength()) to=line.GetLength();

if (res){
	for (pos; pos<to; pos--){ if (line[pos]==fr) return 1; }
}
else{
	for (pos; pos>to; pos++){ if (line[pos]==fr) return 1; }
}

return 0;
}


int rtms(const MString &line, MString fr, int &pos, bool res){ //read to more sumbol
														  //=0
if (!res){
	for (pos; pos<line.GetLength(); pos++){ 
		if (line[pos]==fr[0]){ int e=1;
			if (fr.GetLength()>1) { 
				for (int i=1; i<fr.GetLength(); i++) if (line[pos+i]!=fr[i]) e=0;
			}
				if (e) return 1;
		}	}
}
else{
		for (pos; pos<line.GetLength(); pos++){ 
		if (line[pos]==fr[0]){ int e=1;
			if (fr.GetLength()>1) { 
				for (int i=1; i<fr.GetLength(); i++) if (line[pos+i]!=fr[i]) e=0;
			}
				 if (e) return 1;
		}	}
}

return 0;
}


int rtmst(const MString &line, MString fr, int &pos, int to, bool res){ //read to more sumbol to
if(to<0) to=line.GetLength();									   //=0
else if (to>line.GetLength())to=line.GetLength()-fr.GetLength();

if (!res){
	for (pos; pos<to; pos++){ 
		if (line[pos]==fr[0]){
			if (fr.GetLength()>1) { int e=1;
				for (int i=0; i<fr.GetLength(); i++) if (line[pos+i]!=fr[i]) e=0;
				if (e) return 1;
					
			}
				else return 1;

		}	}
}
else{
		for (pos; pos<to; pos++){ 
		if (line[pos]==fr[0]){
			if (fr.GetLength()>1) { int e=1;
				for (int i=0; i<fr.GetLength(); i++) if (line[pos+i]!=fr[i]) e=0;
				if (e) return 1;
					
			}
				else return 1;

		}	}
}

return 0;
}


int rta(const MString &line, int o, int t, int &pos){ // read to array
for (pos; pos<line.GetLength(); pos++){
	if (line[pos]>o && line[pos]<t) return 1;
}
return 0;
}


int rtna(const MString &line, int o, int t, int &pos){ // read to no array
for (pos; pos<line.GetLength(); pos++){
	if (line[pos]>=o && line[pos]<=t) return 1;
}
return 0;
}


int rrtna(const MString &line, int o, int t, int &pos){	// reserv read to no array
for (pos; pos>-1; pos--){
	if ((line[pos]>o && line[pos]<t)!=1) return 1;
}
return 0;
}


int explode(MString *ret, MString line, MString fr, int c, bool a, bool r){
												//	    =0		=0		=0
if (!r){
	
if (fr.GetLength()==1){
int l=0, pos=0, lpos=0, err;

while(1){
err=rts(line, fr, pos);
if (err) { if(c) if(l==c) {	if (a) {ret[l].Reserv(line.GetLength()-lpos); memcpy(ret[l].rchar(), line.pos(lpos), line.GetLength()-lpos); return l+1;}
								else return l;
							}
		ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++; }
else {if (lpos!=pos){ ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++;}
	 return l;}
pos++; lpos=pos;
}}

int l=0, pos=0, lpos=0, err, to=line.GetLength();
while(1){
err=rtmst(line, fr, pos, to);
if (err) {  if(c) if(l==c) {	if (a) {ret[l].Reserv(line.GetLength()-lpos); memcpy(ret[l].rchar(), line.pos(lpos), line.GetLength()-lpos); return l+1;}
								else {ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); return l+1;}
							}
	ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++; }
else {if (lpos!=pos){ ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++;}
	return l;}
pos+=fr.GetLength(); lpos=pos;
}

}
else{

if (fr.GetLength()==1){
int l=0, pos=line.GetLength(), lpos=line.GetLength()-1, err;

while(1){
err=rrts(line, fr, pos);
if (err) { if(c) if(l==c) {	if (a) {ret[l].Reserv(lpos); memcpy(ret[l].rchar(), line.pos(0), lpos); return l+1;}
								else return l;
							}	
	ret[l].Reserv(lpos-pos); memcpy(ret[l], line.pos(pos+1), lpos-pos); l++; }
else { if (lpos!=pos){ ret[l].Reserv(lpos-pos); memcpy(ret[l].rchar(), line, lpos-pos); l++;}
		return l;}
pos--; lpos=pos;
}}

int l=0, pos=line.GetLength()-1, lpos=line.GetLength()-1, err, to=0;
while(1){
err=rtmst(line, fr, pos, to);
if (err) { ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos+1), pos-lpos); l++; }
else if (lpos!=pos){ ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++;}
	else {if(a) {ret[l].Reserv(line.GetLength()-lpos); memcpy(ret[l].rchar(), line.pos(lpos), line.GetLength()-lpos); l++;}  return l;}
pos-=fr.GetLength(); lpos=pos;
}

}

return 0;
}


template <class D>
int explodem(D &ret, MString line, MString fr, int c=0, bool a=0, bool r=0){
	
if (fr.GetLength()==1){
int l=0, pos=0, lpos=0, err; MString *ms;

while(1){
err=rts(line, fr, pos);
if (err) { if(c) if(l==c) {	if (a) {ms=ret.minsert(); ms->Reserv(line.GetLength()-lpos); memcpy(ms->rchar(), line.pos(lpos), line.GetLength()-lpos); return l+1;}
								else return l;
							}
		ms=ret.minsert(); ms->Reserv(pos-lpos); memcpy(ms->rchar(), line.pos(lpos), pos-lpos); l++; }
else {if (lpos!=pos){ms=ret.minsert(); ms->Reserv(pos-lpos); memcpy(ms->rchar(), line.pos(lpos), pos-lpos); l++;}
	 return l;}
pos++; lpos=pos;
}}
/*
int l=0, pos=0, lpos=0, err, to=line.GetLength();
while(1){
err=rtmst(line, fr, pos, to);
if (err) {  if(c) if(l==c) {	if (a) {ret[l].Reserv(line.GetLength()-lpos); memcpy(ret[l].rchar(), line.pos(lpos), line.GetLength()-lpos); return l+1;}
								else {ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); return l+1;}
							}
	ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++; }
else {if (lpos!=pos){ ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++;}
	return l;}
pos+=fr.GetLength(); lpos=pos;
}				   */

return 0;
}


template <class D>
int cexplodem(D &ret, MString line, MString fr, int c=0, bool a=0, bool r=0){
	
if (fr.GetLength()==1){
int l=0, pos=0, lpos=0, err; MString *ms;

while(1){
if (line[pos]=='"') err=rts(line, '"', ++pos);
else if (line[pos]=='\'') err=rts(line, '\'', ++pos);
err=rts(line, fr, pos);
if (err) { if(c) if(l==c) {	if (a) {ms=ret.minsert(); ms->Reserv(line.GetLength()-lpos); memcpy(ms->rchar(), line.pos(lpos), line.GetLength()-lpos); return l+1;}
								else return l;
							}
		ms=ret.minsert(); ms->Reserv(pos-lpos); memcpy(ms->rchar(), line.pos(lpos), pos-lpos); l++; }
else {if (lpos!=pos){ms=ret.minsert(); ms->Reserv(pos-lpos); memcpy(ms->rchar(), line.pos(lpos), pos-lpos); l++;}
	 return l;}
pos++; lpos=pos;
}}
/*
int l=0, pos=0, lpos=0, err, to=line.GetLength();
while(1){
err=rtmst(line, fr, pos, to);
if (err) {  if(c) if(l==c) {	if (a) {ret[l].Reserv(line.GetLength()-lpos); memcpy(ret[l].rchar(), line.pos(lpos), line.GetLength()-lpos); return l+1;}
								else {ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); return l+1;}
							}
	ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++; }
else {if (lpos!=pos){ ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++;}
	return l;}
pos+=fr.GetLength(); lpos=pos;
}				   */

return 0;
}




int zrout(MString ret, MString buf, int p, int t){
ret.Empty();
	for(int i=p;i<p+t;i++){
	ret+=buf[i];
	}
//ret="gfhfghfg";
return 1;
}



int GetParam(int *ret, MString param){
	for (int ii=0;ii<23;ii++) ret[ii]=0;
int sparam=param.GetLength();
for(int i=0;i<sparam;i++){
int par=int(param[i]);
if (par>96 && par <123) ret[par-97]=1;
}
return i;
}




MString Replace(const MString &line, MString fr, MString to){
//int gp[23];//GetParam(gp, param);
MString ret;

int l=0;										   
int sline=line.GetLength();				//добавить обратную сторону
int sfr=fr.GetLength();					//ограничение изменений
//if (!c) c=0; 

//if (gp[17]) {buf=Res(buf);}	

									//игнорировать первые
for(int i=0;i<sline-sfr+1;i++){		//одно из двух, специальные символы.
int sfrd=0;
for(int ii=0;ii<sfr;ii++){ if(line[i+ii]==fr[ii]) sfrd++;}
if (sfrd==sfr) {
ret+=to; l++;
i=i+sfr-1;

//				if (c>0 && l==c) { if (gp[0]) { i++; for(i;i<sbuf-sfr+1;i++) {ret[l]+=buf[i];} if (gp[17]) ret[l]=Res(ret[l]);} 
//				else {return l+1;}
//				}	
}
else ret+=line[i];
}


//line=ret;
//if(l==0) return -1;
//else return l+1;
return ret;




}

		 
MString replace(const MString &line, MString fr, MString to){

MString ret;
int sfr=fr.GetLength();					//ограничение изменений

int err, pos=0, lpos=0, rpos=0, ps;
if (sfr==1) ret.Reserv(line.GetLength());
else if (sfr==2) ret.Reserv(line.GetLength()+100*fr.GetLength());
else ret.Reserv(line.GetLength()*2+100*fr.GetLength());


while(1){
err=rtms(line, fr, pos);
if (err) {ps=pos-lpos; memcpy(ret.pos(rpos), line.pos(lpos), ps); rpos+=ps;
			memcpy(ret.pos(rpos), fr.pos(0), sfr); rpos+=sfr; }
else {if (pos!=lpos) {ps=pos-lpos; memcpy(ret.pos(rpos), line.pos(lpos), ps); rpos+=ps;}
		ret.resize(rpos); return ret; }
pos+=sfr;
}

return ret;
}




MString sons(MString line, int s, bool d, int dc){
MString t[2], ret;

t[0]=line.Left(line.GetLength()-s);
t[1]=line.Right(s);


if (t[1].GetLength()>dc) t[1].Left(dc);

int dl=t[0].GetLength()/3;
int n=t[0].GetLength()%3;
if (n<1) dl--;
for (int i=dl-1;i>-1;i--){
ret=" "+t[0].Mid(d*3, 3)+ret;
}

ret=t[0].Mid(0,n)+ret;
  
if (d) ret+=","+t[1].Left(s-dc);

return ret;
}

		 
//26.11:12:05 C

int tsor(MString &o, MString &t){
int so, st;
so=o.GetLength();
st=t.GetLength();

int ts; if (so>st) ts=st; else ts=so;

for (int i=0; i<ts; i++){
if (o[i]==t[i]) continue;
if (o[i]<t[i]) return 1;
if (o[i]>t[i]) return -1;
}

if (so!=st){
if (so<st) return 1; else return -1;
}
return 0;

}

		 
int wcslash(MString &val){
if(val[val.GetLength()-1]!='/')
 val+="/";
return 1;
}

int cslash(MString &val){
if(val[val.GetLength()-1]!='\\') val+="\\";
return 1;
}
				  

MString dspace(MString line, int s){ // удаление пробелов s - начало конец;
if(!line) return "";
int i=0, ii=line.size()-1;
if (s & 1) rtns(line, " ", i);
if (s & 2) rrtns(line, " ", ii);
if(ii<i) ii=i;
return line.Mid(i,ii-i+1);
}

int dspacea(MString &line, int s){ // удаление пробелов s - начало конец;  1,2,3
int i=0, ii=line.size()-1;
if (s & 1) { rtns(line, " ", i); }
if (s & 2) { rrtns(line, " ", ii);}
if(ii<i) ii=i;
line=line.Mid(i,ii-i+1);
return  1; 
}


MString dinc(MString &line){
if (line.GetLength()>1){
if (line[0]=='"' && line[line.GetLength()-1]=='"') return line.Mid(1,line.GetLength()-2);
if (line[0]=='\'' && line[line.GetLength()-1]=='\'') return line.Mid(1,line.GetLength()-2);
}
return line;
}

				 
		
void tos(MString &line, int s){
	if (line.GetLength()<s)
		for (int i=0; i<line.GetLength(); i++){
		line=line;
		}
return ;
}

					
int umn(int val){
if (val<0) return val+256;
else return val;
					  
}

bool ishandle(void* h){
if (h && h!=(void*)0xffffffff) return 1;
return 0;
}


bool IsHandle(void* h){
if (h && h!=(void*)0xffffffff) return 1;
return 0;
}


MString ants(MString string, int n){
for (int i=string.GetLength(); i<n;i++) string="0"+string;
return string;
}					  

