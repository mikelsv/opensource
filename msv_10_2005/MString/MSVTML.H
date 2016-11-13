// MSV Time Functions (MSVLibrary) MSV TML
//1.12.04 (build 13)

class MTime{
public:
long tm;

void Time(){
   time( &tm );
return ;
}


operator long*(){return &tm;}
operator int(){return (int)tm;}

	//   time_t ltime;

   
   //printf( "The time is %s\n", ctime( &ltime ) );


};


MString ttime(int val, int l=3){
MString ret;
if (l>1) {ret=itos(val%60, 10, 2); ret="."+ret;}
val=val/60;
if (l>2) {ret=itos(val%60, 10, 2)+ret; ret=":"+ret;}
else {return itos(val, 10, 2)+":"+ret;  }
val=val/60;
if (l>3) {ret=itos(val%24, 10, 2)+ret; ret=" "+ret;}
else {return itos(val, 10, 2)+ret;  }
val=val/24;

return ret;
}