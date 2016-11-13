//MSL Int Library

// 100%		  // re create on 10 and any radix and add tos
MString itos(int val, int radix, int null){
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

if (null>0) for (int i=ret.GetLength(); i<null; i++) {ret="0"+ret;}
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


MString itob(unsigned int val, int null){	//100%
MString ret;					//  =0
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
if (val[i]<0)
 ret+=int(val[i])+256
 *int(pow(256, val.GetLength()-i-1));
else ret+=int(val[i])*int(pow(256, val.GetLength()-i-1));
}

return ret;
}


char stos(short s){	// short to char
if (s<256) return char(s);
if (s>1039 && s<1104){
char string[]="ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
return char(string[s-1040]);
}

return -1;
}

		  
MString xtos(void*x){
return itos(int(x));
}

void* stox(MString line){
return (void*)(stoi(line));
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


_int64 stoi64(MString val, int radix){
_int64 min=0, vl, ret=0; int st=0;
if (val.IsEmpty()) return 0;
if (val[0]=='-') min=1;

for (int i=val.GetLength()-1; i>min-1; i--){
vl= int(val[i]);
if (vl<48 || vl>57) break;

//ret+=(vl-48)*int(pow(10,st));
ret+=_int64(pow(radix,st))* (vl>48 && vl<57) ? vl-48 : vl-97;
st++;
}
if (min) ret*=-1;
return ret;
}


MString itos64(_int64 val, int radix, int null){
MString ret;
int mins=0;
if (val<0) {mins=1; val=val*-1;}

int stp=1;_int64 tmp;
if (val==0) {if (null>0) for (int i=0; i<=null;i++) {ret="0"+ret;} else return "0"; return ret; }	
while(val>0){
tmp=val%radix;
//val-=tmp;
if (tmp>=0 && tmp<=9) ret=MString(char(48+tmp))+ret;
else if (tmp>=10 && tmp<=33) ret=MString(char(55+tmp))+ret;
val/=radix;
}

if (null>0) for (int i=ret.GetLength(); i<null; i++) {ret="0"+ret;}
if (mins) ret="-"+ret;

return ret;
}

MString ftos(float val, int ml){
MString ret=itos(int(val));
val=val-int(val);
if(val<0) val*=-1;
if(!val) return ret;
ret+=".";
//val*=1000000;
while(val){
if(ml==0) break;
val*=10; ret+=itos(int(val)); val-=int(val);
ml--;
}
return ret;
}

int getpl(int val){
if (val<0) val*=-1;
return val;
}



float stof(MString val){
float ret=0;
MString tm[2];
explode(tm, val, ".", 1, 1); 
ret=float(stoi(tm[0]));

float rt=float(stoi(tm[1]));
rt/=float(pow(10, tm[1].size()));

ret+=rt;
return ret;
}
