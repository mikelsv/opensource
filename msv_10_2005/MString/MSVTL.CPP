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
int rrts(const MString &line, MString fr, int &pos){ //read to sumbol to
for (pos; pos>-1; pos--){
	if (line[pos]==fr) return 1;
}
return 0;
}
/////
int rtst(const MString &line, MString fr, int &pos, int to, bool res=0){ //read to sumbol to
if(to<0) to=line.GetLength();
else if (to>line.GetLength()) to=line.GetLength();

if (res){
	for (pos; pos<to; pos--){ if (line[pos]==fr) return 1; }
}
else{
	for (pos; pos>to; pos++){ if (line[pos]==fr) return 1; }
}

return 0;
}


int rtms(const MString &line, MString fr, int &pos, bool res=0){ //read to sumbol to

if (!res){
	for (pos; pos<line.GetLength(); pos++){ 
		if (line[pos]==fr[0]){
			if (fr.GetLength()>1) { int e=1;
				for (int i=0; i<fr.GetLength(); i++) if (line[pos+i]!=fr[i]) e=0;
				if (e) return 1;
					
			}
				else return 1;

		}	}
}
else{
		for (pos; pos<line.GetLength(); pos++){ 
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


int rtmst(const MString &line, MString fr, int &pos, int to, bool res=0){ //read to sumbol to
if(to<0) to=line.GetLength();
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



int explode(MString *ret, MString line, MString fr, int c, bool a=0, bool r=0){

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
	ret[l].Reserv(lpos-pos); memcpy(ret[l].rchar(), line.pos(pos), lpos-pos); l++; }
else { if (lpos!=pos){ ret[l].Reserv(lpos-pos); memcpy(ret[l].rchar(), line.pos(pos), lpos-pos); l++;}
		return l;}
pos--; lpos=pos;
}}

int l=0, pos=line.GetLength()-1, lpos=line.GetLength()-1, err, to=0;
while(1){
err=rtmst(line, fr, pos, to);
if (err) { ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++; }
else if (lpos!=pos){ ret[l].Reserv(pos-lpos); memcpy(ret[l].rchar(), line.pos(lpos), pos-lpos); l++;}
	else {if(a) {ret[l].Reserv(line.GetLength()-lpos); memcpy(ret[l].rchar(), line.pos(lpos), line.GetLength()-lpos); l++;}  return l;}
pos-=fr.GetLength(); lpos=pos;
}

}

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




// 100%
MString itos(int val, int radix=10, int null=0){
MString ret;
int mins=0;
if (val<0) {mins=1; val=val*-1;}

int stp=1;int tmp;
if (val==0) {if (null>0) for (int i=0; i<=null;i++) {ret="0"+ret;} else return "0"; return ret; }	
while(val>0){
tmp=val%radix;
//val-=tmp;
if (tmp>=0 && tmp<=9) ret=MString(char(48+tmp))+ret;
else if (tmp>=10 && tmp<=33) ret=MString(char(55+tmp))+ret;
val/=radix;
}

if (null>0) for (int i=ret.GetLength(); i<=null; i++) {ret="0"+ret;}
if (mins) ret="-"+ret;

return ret;
}


// 100%
int stoi(MString val){
int min=0, vl, ret=0, st=0;
if (val.IsEmpty()) return 0;
if (val[0]=='-') min=1;

for (int i=val.GetLength()-1; i>min-1; i--){
vl= int(val[i]);
if (vl<48 || vl>57) break;

//ret+=(vl-48)*int(pow(10,st));
ret+=(vl-48)*int(pow(10,st));
st++;
}

if (min) ret*=-1;
return ret;
}


MString itob(unsigned int val, int null=0){	//100%
MString ret;
if (val==0) {if (null>0) for (int i=0; i<null;i++) {ret=MString(0)+ret;} else return MString(0); return ret;}
						   
int stp=1, tmp;

while(val>0){
//int pows=int(pow(256,stp));
tmp=val%256;
val/=256;
ret=MString(tmp)+ret;	//pows -1
//stp++;
}

if (null>0) for (int i=ret.GetLength(); i<null;i++) {ret=MString(0)+ret;} 


return ret;
}


int btoi(MString val){
unsigned int ret=0;
if (val.GetLength()==0 || val.IsEmpty() || val==MString(0)) return 0;

for (int i=val.GetLength()-1; i>-1; i--){
if (val[i]<0) ret+=int(val[i]+256)*int(pow(256, val.GetLength()-i-1));
else ret+=int(val[i])*int(pow(256, val.GetLength()-i-1));
}

return ret;
}



int isnum(MString val){
int min=0;
if (val.IsEmpty()) return 0;
if (val[0]=='-') min=1;

for (int i=val.GetLength()-1; i>min-1; i--){
if (int(val[i])<48 || int(val[i])>57) return 0;
}

return 1;
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
if(val[val.GetLength()-1]!='/') val+="/";
return 1;
}

int cslash(MString &val){
if(val[val.GetLength()-1]!='\\') val+="\\";
return 1;
}
				  

MString DSpase(MString line){
int i=0; if (line=="") return "";
while(line[i]==' ' && i<line.GetLength()) {i++;}

int ii=line.GetLength()-1;
while(line[i]==' ' && ii>-1) {ii--;}

return line.Mid(i,ii-i+1);
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