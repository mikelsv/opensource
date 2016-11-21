// NEW TWO

// ! - not
// $ - return : $ret+notext or $ret$
// | - array

// / - sp to std //-> /, /|->|
// * - any sumbols	:r
// ? - one sumbol	:r

// # - #m - may(min #m=#m0), #x15# - 15, #a - any, #n - next
// #l+ - #u,  #l- - #d,  #l:aka use: #laka 
// #e - end find, #p - set pos

// WORK - find or compare. -> res: only find

#define ppexm(tmem, umem, nmem, buf) {while(tmem<umem+nmem) { tmem*=2; BYTE*b=_newn(tmem); memcpy(b, buf, umem); _del(buf); buf=b; } }

//#define push(stack, stacksize, tstack, data, size){ppexm(stacksize, tstack, size, stack); memcpy(stack+tstack, data, size); tstack+=size;}
#define pusho(stack, stacksize, tstack, data){ppexm(stacksize, tstack, 1, stack); stack[tstack]=data; tstack++;}
#define pushnull(stack, stacksize, tstack, data, size){ppexm(stacksize, tstack, size, stack); memset(stack+tstack, 0, size); tstack+=size;}
//if(t stack+size>stacksize) assert("Stack Error -");
//#define pop(stack, stacksize, tstack, data, size){ memcpy(data, stack+tstack-size, size); tstack-=size; if(tstack>65536 ||tstack<0 )globalerror(); }
#define popo(stack, stacksize, tstack, data){  tstack--; data=stack[tstack];}
#define popnull(stack, stacksize, tstack, data, size){tstack-=size;}
//#define ret(stack, stacksize, tstack, data, size){ memcpy(data, stack+tstack-size, size);}


#define def_codesz	S2K
#define def_stacksz S1K
#define def_retsz S4K
/*
class EFLOG{
HFILEM fl;
public:
	EFLOG(){ int i=0;
		while(1){
		fl=CreateFile("eflog"+itos(i)+".log", O_CREAT|O_RDWR|O_EXCL, S_IREAD|S_IWRITE);
		if(ishandle(fl)) break; i++;
		}}
	Write(MString line){ line+="\r\n"; WriteFile(fl, line, line);}

};*/

//#define EFLOG(m) eflog.Write("ST: "+itos(stacksz, 10, stacksz)+"  "+m)
#define EFLOG(m) 

class MSVEF{ //Expert Find
//EFBULine data;
unsigned int _codesz, _stacksz, _retsz, codesz, stacksz, retsz;
BYTE *code, *stack, *ret; // ret -> rt

public:
	bool fcmp; // full compare

	MSVEF(){
		codesz=0; stacksz=0; retsz=0; _codesz=def_codesz; _stacksz=def_stacksz; _retsz=def_retsz;
		code=_newn(_codesz); stack=_newn(_stacksz); ret=_newn(_retsz);
		fcmp=0;
	}
	~MSVEF(){ Clear(); }

	inline void push(BYTE *stack, unsigned int &stacksize, unsigned int &tstack, void*data, int size){ppexm(stacksize, tstack, size, stack); memcpy(stack+tstack, data, size); tstack+=size;}

	inline void pop(BYTE *stack, unsigned int &stacksize, unsigned int &tstack, void*data, int size){ memcpy(data, stack+tstack-size, size); tstack-=size; if(tstack>65536 ||tstack<0 )globalerror(); }

	inline void retf(BYTE *stack, unsigned int &stacksize, unsigned int &tstack, void*data, int size){ memcpy(data, stack+tstack-size, size); }

#define EFFIRST	1
#define EFNEXT	2
#define EFNOT	4
#define EFANY	8
#define EFQUE	16
#define EFLUP	32
#define EFLDW	64

//				128

#define EFC_FIND		1 // find ^ sz data
#define EFC_FIRST		2 // set findfirst;
#define EFC_NEXT		3 // set next
#define EFC_NOT			4 // set not
//#define EFC_ANY			5 // find any
#define EFC_QUE			6 // find quest
#define EFC_SETASPOS	7 // set as input &pos
#define EFC_SPOS		8 // push pos
#define EFC_GPOS		9 // pop pos
#define EFC_KPOS		10// kill pos
#define EFC_SRES		11// save results ^ pos type // set(itmp, fc);
#define EFC_ARES		12// analys results ^ size
#define EFC_RET			13// return data ^ sz name
#define EFC_STMPPOS		14// set pos in tmp
//#define EFC_MTMPPOS		15// set pos in tmp
#define EFC_GTMPPOS		15// get pos in tmp

// OPTIONSCODE			# spcode #
#define EFO_M			32// minimum or maybe #m=0
#define EFO_X			33// maximum #x=999999
#define EFO_U			34// #u=1, use maximum and minimum
#define EFO_RET			35// #rret set data in as ret
#define EFO_RETPOS		36// #ppos set pos in as ret
#define EFO_RETLPOS		37// #lpos set lpos in as ret
#define EFO_SETRET		38// #sret set data in as ret

bool Read(const MString& line){
	int lpos=0; unsigned int i; BYTE f=0; int level=0; unsigned int *fsz=0; int pp=0;
	BYTE ffind=3; BYTE vfind=0; unsigned int sresp=0, tmp; MString stmp; // 1 - find, 2 - setpos
	bool spm=0;
	bool mu; // may use to all
//	MSVEFB*e=&data.getelement(data.add(MSVEFB()));
//	e->level=level;// f|=EFANY; e->data='*'; e->level=level; e->f=f; f=0;
//	f|=EFNEXT;// data.getelement(data.add(MSVEFB())); e->level=level;
///	f|=EFFIRST;
	push(code, _codesz, codesz, itob(EFC_FIRST), 1);


	for(i=0; i<line.size(); i++){

/*char *to=line+sz; line+=pos;
for (line; line<to; line++){
	if (low(*line)==low(fr)) {pos=sz-(to-line); return 1;}
} pos=sz-(to-line); return 0;
*/
	switch(line[i]){
		case '*':  fsz=0; pusho(code, _codesz, codesz, EFC_FIRST);
		break;

		case '?': fsz=0;
			pusho(code, _codesz, codesz, EFC_SPOS); pp++;
			pusho(code, _codesz, codesz, EFC_QUE);
		break;

		case '|': fsz=0; 
			if(pp){
				pusho(code, _codesz, codesz, EFC_STMPPOS);			
				for(pp; pp>0; pp--){ pusho(code, _codesz, codesz, EFC_GPOS); }
				pusho(code, _codesz, codesz, EFC_SRES); vfind++;
				//pusho(code, _codesz, codesz, EFC_GTMPPOS);
			}
			if(ffind&1) {pusho(code, _codesz, codesz, EFC_FIRST);}
			//pusho(code, _codesz, codesz, EFC_FIRST, 1);
		break;

		case '^': //if(ffind&2){ push(code, _codesz, codesz, itob(EFC_SETASPOS), 1); ffind=0;}
				  if(pp){
					  	pusho(code, _codesz, codesz, EFC_STMPPOS);			
						for(pp; pp>0; pp--){ pusho(code, _codesz, codesz, EFC_GPOS); }
						pusho(code, _codesz, codesz, EFC_SRES); vfind++;
						//pusho(code, _codesz, codesz, EFC_GTMPPOS);
				  }
				  if(vfind) {
					if(vfind==1){
						popo(code, _codesz, codesz, tmp);
						/*if(tmp){
							pop(code, _codesz, codesz, &tmp, 4);
							pop(code, _codesz, codesz, &tmp, 4);
						}*/} else {
					pusho(code, _codesz, codesz, EFC_ARES);
					pusho(code, _codesz, codesz, vfind);
						} vfind=0;
					//pusho(code, _codesz, codesz, EFC_GTMPPOS);
				  }
				pusho(code, _codesz, codesz, EFC_GTMPPOS);
				if(ffind&2) {push(code, _codesz, codesz, itob(EFC_SETASPOS), 1); ffind&=~2;}
				fsz=0;
		break;

		case '!': fsz=0; f|=EFNOT; break;
		
		case '#': fsz=0; if(!spm) spm=1;i++;
			for(i; i<line.size(); i++){

				switch(line[i]){
					case '#': spm=0; break;
					case 'u': pusho(code, _codesz, codesz, EFO_U); break;
					case 'm':
						tmp=0; i++; mu=0; //int m=0;
						for(i; i<line.size(); i++)
							if(isnumd(line[i])) {tmp*=10; tmp+=line[i]-48;}
							else if(line[i]=='-') mu=1; //m=1;
							else break; i--;
							if(mu) tmp*=-1;
						pusho(code, _codesz, codesz, EFO_M);
						push(code, _codesz, codesz, &tmp, 4);
					break;
					case 'x':
						tmp=0; i++;
						for(i; i<line.size(); i++){
							if(isnumd(line[i])) {tmp*=10; tmp+=line[i]-48;}
							break;} i--; if(!tmp) tmp=S2GM;
						pusho(code, _codesz, codesz, EFO_X);
						push(code, _codesz, codesz, &tmp, 4);
					break;
					case 'r': i++; tmp=0;
						pusho(code, _codesz, codesz, EFO_RET);
						fsz=(unsigned int*)(code+codesz); pushnull(code, _codesz, codesz, 0, 4);
                        for(i; i<line.size(); i++){ if(line[i]>=65 && line[i]<=90 || line[i]>=97 && line[i]<=122) {
							*fsz+=1; pusho(code, _codesz, codesz, line[i]);} else break;} i--;
						fsz=0;
					break;
					case 'p': i++; tmp=0;
						pusho(code, _codesz, codesz, EFO_RETPOS);
						fsz=(unsigned int*)(code+codesz); pushnull(code, _codesz, codesz, 0, 4);
                        for(i; i<line.size(); i++){ if(line[i]>=65 && line[i]<=90 || line[i]>=97 && line[i]<=122) {
							*fsz+=1; pusho(code, _codesz, codesz, line[i]);} else break;} i--;
						fsz=0;
					break;
					case 'l': i++; tmp=0;
						pusho(code, _codesz, codesz, EFO_RETLPOS);
						fsz=(unsigned int*)(code+codesz); pushnull(code, _codesz, codesz, 0, 4);
                        for(i; i<line.size(); i++){ if(line[i]>=65 && line[i]<=90 || line[i]>=97 && line[i]<=122) {
							*fsz+=1; pusho(code, _codesz, codesz, line[i]);} else break;} i--;
						fsz=0;
					break;
					case 's': i++; tmp=0;
						pusho(code, _codesz, codesz, EFO_SETRET);
						fsz=(unsigned int*)(code+codesz); pushnull(code, _codesz, codesz, 0, 4);
                        for(i; i<line.size(); i++){ if(line[i]>=65 && line[i]<=90 || line[i]>=97 && line[i]<=122 || (unsigned char)line[i]>=192){
							*fsz+=1; pusho(code, _codesz, codesz, line[i]);} else break;} i--;
						fsz=0;
					break;
				}

                if(!spm) break;
			}
		break;


		case '\\': i++;
		default:
			if(!fsz){ pusho(code, _codesz, codesz, EFC_SPOS); pp++;
			push(code, _codesz, codesz, itob(EFC_FIND), 1);
			//push(code, _codesz, codesz, (char*)EFC_SPOS, 1);
			fsz=(unsigned int*)(code+codesz); pushnull(code, _codesz, codesz, 0, 4);
			}
			push(code, _codesz, codesz, line.pos(i), 1); *fsz+=1;
		break;
	}	
}

if(pp){
	pusho(code, _codesz, codesz, EFC_STMPPOS);			
	for(pp; pp>0; pp--){ pusho(code, _codesz, codesz, EFC_GPOS); }
	pusho(code, _codesz, codesz, EFC_SRES); vfind++;
	//pusho(code, _codesz, codesz, EFC_GTMPPOS);
}
if(vfind) {
if(vfind==1){
	popo(code, _codesz, codesz, tmp);
	/*if(tmp){
		pop(code, _codesz, codesz, &tmp, 4);
		pop(code, _codesz, codesz, &tmp, 4);
	}*/} else {
pusho(code, _codesz, codesz, EFC_ARES);
pusho(code, _codesz, codesz, vfind);
	} vfind=0;
//pusho(code, _codesz, codesz, EFC_GTMPPOS);
}
pusho(code, _codesz, codesz, EFC_GTMPPOS);
if(ffind&2) {push(code, _codesz, codesz, itob(EFC_SETASPOS), 1); ffind&=~2;}
fsz=0;

/*
//if(fsz){ pusho(code, _codesz, codesz, EFC_GPOS, 1); fsz=0; }
if(pp){
	pusho(code, _codesz, codesz, EFC_STMPPOS); pp--;			
	for(pp; pp>0; pp--){ pusho(code, _codesz, codesz, EFC_GPOS); }
	//pusho(code, _codesz, codesz, EFC_SRES); vfind++;
	//pusho(code, _codesz, codesz, EFC_GTMPPOS);
}
if(vfind){
	if(vfind==1){
		popo(code, _codesz, codesz, tmp);
		/*if(tmp){
			pop(code, _codesz, codesz, &tmp, 4);
			pop(code, _codesz, codesz, &tmp, 4);
	}* /} else {
		pusho(code, _codesz, codesz, EFC_ARES);
		pusho(code, _codesz, codesz, vfind);
	} vfind=0;
}
if(ffind&2){
	pusho(code, _codesz, codesz, EFC_GTMPPOS);
	push(code, _codesz, codesz, itob(EFC_SETASPOS), 1); ffind&=~2;
}
//	if(lpos!=i) { e->f=f; f=0; e->data=line.str(lpos, i-lpos);}
*/
	return 1;
}	   

// ADD: one block logic

int rts(const MString &line, unsigned int &pos){
//int i=0; int level=0; int vars=0, bpos;
bool first=0; int tmp, itmp, ltmp, btmp; MString t; int p=pos, lp=0, i; bool fc=1, fnot=0; int64 i64; // fnot - flag not 

int efom=1, efox=1, efret=0; MString efretn;
//DelRet();
int movecur=0;

itmp=p; ltmp=p; // error this

unsigned int codel=0;

while(codel<codesz){
//	MSVEFB ef=data[i];
	switch(code[codel]){

		case EFC_FIND: codel++;
			tmp=*(unsigned int*)&code[codel]; codel+=4;
			t.Reserv(tmp); memcpy(t, &code[codel], tmp); codel+=tmp; EFLOG("FIND:"+t);
			if(p==-1) p=line.size();
			if(first) { fc=rtms(line, t, p, fnot)>0; first=0; }
			else {
				if(!fnot) if(fc) for(int i=0; i<tmp; i++){ if(t[i]!=line[i+p]) {fc=0; break;}}
				else{ if(fc){itmp=1; for(int i=0; i<tmp; i++){ if(t[i]!=line[i+p]) {itmp=0; break;} if(itmp) fc=0; }}}
			}lp=p; p+=tmp; if((unsigned int)p>line.size()) fc=0; if(!fc) p=-1; EFLOG("    "+itos(fc));


			if(efret && fc){ EFLOG("EFRET "+itos(p>=0 ? 1 : 0)); efret=0;
			if(p>=0){
				retf(stack, _stacksz, stacksz, &i64, 8);
				tmp=*((int*)&i64+1); i=efretn.size();
				push(ret, _retsz, retsz, &i, 4);
				push(ret, _retsz, retsz, efretn, efretn.size()); i=lp-tmp;
				push(ret, _retsz, retsz, &i, 4);
				push(ret, _retsz, retsz, line.pos(tmp), i);
			}}// else if(efret) efret=0;
			
			
		break;

		case EFC_FIRST: codel++; first=1; EFLOG("FIRST"); break;

		case EFC_NOT: codel++; fnot=1; EFLOG("NOT"); break;
		case EFC_QUE: codel++; lp=p; p++;  if((unsigned int)p>line.size()) fc=0; if(!fc) p=-1; EFLOG("QUE"); break;

		case EFC_SETASPOS: codel++; pos=ltmp; EFLOG("SET AS POS"); break;
		case EFC_SPOS: codel++; EFLOG("SPOS "+itos(fc));
			if(!fc) p=-1;
			else if(p!=-1) push(stack, _stacksz, stacksz, &lp, 4);
			push(stack, _stacksz, stacksz, &p, 4);
//			else push(stack, _stacksz, stacksz, &p, 4);
			break;
		case EFC_GPOS: codel++; pop(stack, _stacksz, stacksz, &p, 4);
			if(p!=-1) {fc=1; pop(stack, _stacksz, stacksz, &lp, 4);} else fc=0; EFLOG("GPOS "+itos(fc)); break;
		case EFC_KPOS: codel++; pushnull(stack, _stacksz, stacksz, &p, 4); EFLOG("KPOS"); break;
		case EFC_SRES: codel++; EFLOG("SRES "+ itos(itmp==-1 ? 0 : 1 ));
			if(itmp>-1){push(stack, _stacksz, stacksz, &lp, 4); push(stack, _stacksz, stacksz, &itmp, 4); pusho(stack, _stacksz, stacksz, 1);}
			else {pusho(stack, _stacksz, stacksz, 0);} break;
		case EFC_ARES: codel++;
			tmp=code[codel]; codel++;
			itmp=-1; EFLOG("ARES "+itos(tmp));
			//popo(code, _codesz, stacksz, tmp);
			for(i=0; i<tmp; i++){
			popo(stack, _stacksz, stacksz, btmp); EFLOG(" v "+ itos(btmp==-1 ? 0 : 1 ));
			if(btmp){ pop(stack, _stacksz, stacksz, &btmp, 4);
			pop(stack, _stacksz, stacksz, &lp, 4);
			// line
			if(itmp<0 || itmp>btmp) itmp=btmp;
			}}
			if(itmp<0) fc=0; else fc=1;
 			break;
		case EFC_STMPPOS: codel++; itmp=p; ltmp=lp; EFLOG("SET TMP POS"); break;
		case EFC_GTMPPOS: codel++; p=itmp; lp=ltmp; if(p==-1) fc=0; EFLOG("GET TMP POS"); break;

		case EFO_RET: codel++; 
			tmp=*(unsigned int*)&code[codel]; codel+=4;
			efretn.Reserv(tmp); memcpy(efretn, &code[codel], tmp); codel+=tmp; efret=1;
		break;
			case EFO_RETPOS: codel++;
				tmp=*(unsigned int*)&code[codel]; codel+=4;
				push(ret, _retsz, retsz, &tmp, 4);
				push(ret, _retsz, retsz, &code[codel], tmp); tmp=4;
				push(ret, _retsz, retsz, &tmp, 4);
				push(ret, _retsz, retsz, &p, 4);
		break;
			case EFO_RETLPOS: codel++;
				tmp=*(unsigned int*)&code[codel]; codel+=4;
				push(ret, _retsz, retsz, &tmp, 4);
				push(ret, _retsz, retsz, &code[codel], tmp); tmp=4;
				push(ret, _retsz, retsz, &tmp, 4);
				push(ret, _retsz, retsz, &lp, 4);
		break;

		case EFO_SETRET: codel++;
			if(efret){
				i=efretn.size();
				push(ret, _retsz, retsz, &i, 4);
				push(ret, _retsz, retsz, efretn, efretn.size());
				tmp=*(unsigned int*)&code[codel]; codel+=4;
				push(ret, _retsz, retsz, &tmp, 4);
				push(ret, _retsz, retsz, &code[codel], tmp);
				codel+=tmp;
			efret=0;}
		break;

		case EFO_M:{ codel++;
			int t=*(int*)&code[codel]; codel+=4;
			movecur+=t;
			//if(t<0) if((t*-1)>tmp) lp=0; else tmp+=t;
			//if(t>0) if(t+tmp>line.size()) tmp=line.size(); else tmp+=t;
		} break;
		default:
			tmp=0;
	}
}

	if(efret && fc){ EFLOG("EFRET "+itos(p>=0 ? 1 : 0));
//	if(p>=0){
		lp=line.size();// p=lp;
		//ret(stack, _stacksz, stacksz, &i64, 8);
		//tmp=*((int*)&i64+1);
		tmp=p; i=efretn.size();
		// movecur
		if(movecur<0) if((movecur*-1)>tmp) tmp=0; else tmp+=movecur;
		if(movecur>0) if((unsigned int)(movecur+tmp)>line.size()) tmp=line.size(); else tmp+=movecur;
		movecur=0;
		//end movecur
		push(ret, _retsz, retsz, &i, 4);
		push(ret, _retsz, retsz, efretn, efretn.size()); i=lp-tmp;
		push(ret, _retsz, retsz, &i, 4);
		push(ret, _retsz, retsz, line.pos(tmp), i);
	} efret=0; //} efret=0;

if(stacksz!=0) 
	int err=1;

if(first && fc) p=line.size();
if(!fc) pos=line.size(); else pos=p;
if(fcmp && pos<line.size()) return 0;
return fc;
}

MString Ret(const MString &line, unsigned int &pos, bool null=1){ // return val
MString retd; unsigned int i; if(null==0) pos=0;
while(pos<retsz){
i=*(unsigned int*)(ret+pos); pos+=4;
if(line.compare((char*)ret+pos, i)){ pos+=i; if(pos>=_retsz) return MString();
	i=*(unsigned int*)(ret+pos); pos+=4; retd.Reserv(i); memcpy(retd, ret+pos, i); pos+=i; return retd;
} else { pos+=i;  if(pos>=_retsz) return MString();
	i=*(unsigned int*)(ret+pos); pos+=4+i;
}}
return MString();
}

MString RetF(MString &line, unsigned int &pos){ // return key - val
MString retd; unsigned int i;
while(pos<retsz){
i=*(unsigned int*)(ret+pos); pos+=4;
if(1){
	line.set(ret+pos, i);
pos+=i; if(pos>=_retsz) return MString();
i=*(unsigned int*)(ret+pos); pos+=4; retd.set(ret+pos, i); pos+=i; return retd;
}}
return MString();
}

unsigned int RetP(const MString &line, unsigned int &pos, bool null=1){ // return pos
unsigned int retd; unsigned int i; if(null==0) pos=0;
while(pos<retsz){
i=*(unsigned int*)(ret+pos); pos+=4;
if(line.compare((char*)ret+pos, i)){ pos+=i; if(pos>=_retsz) return 0;
	i=*(unsigned int*)(ret+pos); pos+=4; if(i!=4) return 0; memcpy(&retd, ret+pos, i); pos+=i; return retd;
} else { pos+=i; if(pos>=_retsz) return MString();
	i=*(unsigned int*)(ret+pos); pos+=4+i;
}}
return 0;
}

unsigned int RetPF(MString &line, unsigned int &pos){ // return full pos
unsigned int retd; unsigned int i;
while(pos<retsz){
i=*(unsigned int*)(ret+pos); pos+=4;
if(1){ line.set(ret+pos, i); pos+=i; if(pos>=_retsz) return 0;
	i=*(unsigned int*)(ret+pos); pos+=4;
	if(i!=4) {pos+=i; continue;}
	 memcpy(&retd, ret+pos, i); pos+=i; return retd;
}}
return 0;
}

MString RetM(const MString &line, unsigned int &pos, bool null=1){ // return multi line
LString retd; unsigned int i; if(null==0) pos=0;
while(pos<retsz){
i=*(unsigned int*)(ret+pos); pos+=4;
if(line.compare((char*)ret+pos, i)){ pos+=i; if(pos>=_retsz) return MString();
	i=*(unsigned int*)(ret+pos); pos+=4; retd+=MString(ret+pos, i); pos+=i;
} else { pos+=i;  if(pos>=_retsz) return MString(retd.oneline(), retd.size());
	i=*(unsigned int*)(ret+pos); pos+=4+i;
}}
return MString(retd.oneline(), retd.size());
}

void DelRet(){
if (_stacksz!=def_stacksz) {_deln(stack); stack=_newn(_stacksz); } memset(stack, 0, _stacksz);
if (_retsz!=def_retsz) {_deln(ret); ret=_newn(_retsz); } memset(ret, 0, _retsz);
retsz=0; stacksz=0;
return ;}

void DelAll(){
if (_stacksz!=def_stacksz) {_deln(stack); stack=_newn(_stacksz); } memset(stack, 0, _stacksz);
if (_retsz!=def_retsz) {_deln(ret); ret=_newn(_retsz); } memset(ret, 0, _retsz);
if (_codesz!=def_codesz) {_deln(code); code=_newn(_codesz); } memset(code, 0, _codesz);
retsz=0; stacksz=0; codesz=0;
return ;}

void ClearAct(){
	codesz=0; stacksz=0; retsz=0; fcmp=0;
}

void Clear(){
	_deln(stack); _stacksz=0;
	_deln(ret); _retsz=0;
	_deln(code); _codesz=0;
}


};

// % - special
// ? - hidden
// * - hidden sumbols
// [] - code or multi. [k] if(k!=special)
// $ - getparam
// | - multi in []
// / - end tag

// , more : 1,2
// - arr : 1-3

// SPECIALS
// a - all
// fres or fr - FREE SUMBOLS
// quotes(/quotes) or qu(/qu) - quotes
// while - wh
// may - may be
// 

// }{ak
// ?- - one or no
// ?* - more or one
// ?? - ?, // ** - *, // $$ - $, // [[ -

// NEW
// ! - not

// $ - return : $ret or $ret$

// | - array
// / - sp to std //-> /, /|->|

// * - any, любой, *
// ? - one, один 1
// ?* - one is more, хотьбы один,  >1
// *? - more is one, один или ничего, 0-1	 // may