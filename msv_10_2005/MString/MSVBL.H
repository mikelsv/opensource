// MSV Byte Functions
//4.2.6 (build 3)

int tobit(int *ret, int val){
int tmp; int st;

for (int i=0;i<8;i++){
st=int(pow(2,i+1));
tmp=(val%st);
if (tmp) {ret[i]=1;
val-=int(pow(2,i));}
else ret[i]=0;
}

return 1;
}

int fobit(int *bit){
int ret=0;

for (int i=0;i<8;i++){
if (bit[i]==1) ret+=int(pow(2,i));
}
return ret;
}


int tobit(bool *ret, int val){
int tmp; int st;

for (int i=0;i<8;i++){
st=int(pow(2,i+1));
tmp=(val%st);
if (tmp) {ret[i]=1;
val-=int(pow(2,i));}
else ret[i]=0;
}

return 1;
}

int fobit(bool *bit){
int ret=0;

for (int i=0;i<8;i++){
if (bit[i]==1) ret+=int(pow(2,i));
}
return ret;
}