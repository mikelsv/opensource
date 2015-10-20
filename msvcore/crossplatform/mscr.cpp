#include "math.h"

unsigned int MSCRUNID[]={
1447629571u,
2128473579u,
1625038439u,
1989527194u,
2530880841u,
2171182033u,
2714863346u,
1514787740u,
2201251770u,
1180351800u,
1375847058u,
3844786896u,
2491100860u,
2727867879u,
2569459050u,
2884671564u,
3830920111u,
3106922954u,
1117879710u,
3189675458u,
2969937947u,
3357214622u,
3521638916u,
2755644789u,
904672351u,
3302504680u,
1441881647u,
456508434u,
4042204147u,
3649153421u,
4149565055u,
3908231411u,
1238190874u,
1159271754u,
1302972771u,
601505435u,
1105320804u,
4151885407u,
2330775420u,
3578528488u,
48788811u,
397581047u,
3726575538u,
2668156104u,
2589898729u,
3502965231u,
3724160939u,
1951385087u,
1597499213u,
1375510931u,
602183817u,
3324221613u,
1794452208u,
2114985671u,
4139167238u,
1017923959u,
236157931u,
1545138786u,
658089240u,
1383249279u,
332745553u,
1297990916u,
1579998329u,
3520610105u,
150313597u,
2968391999u,
4274202806u,
3767044869u,
1784376693u,
433711880u,
2928256024u,
777922222u,
3754583665u,
438183815u,
2098069124u,
2647614089u,
4198658890u,
3543333369u,
505770873u,
3826896428u,
1369905806u,
2947978575u,
4041040204u,
1598001903u,
3043181980u,
1798952609u,
518436559u,
1325087577u,
1347226661u,
1666176728u,
2989591923u,
3295233462u,
1744097085u,
3232381997u,
1147085230u,
1820976577u,
3209267547u,
1761173456u,
131952130u,
541471261u,
3044102411u,
3996874443u,
1698105632u,
1183357411u,
3682728787u,
2303853833u,
786976172u,
2613665544u,
850158271u,
349495503u,
2820906239u,
3594842459u,
613192760u,
3203131951u,
696504947u,
446850181u,
1721504451u,
2289232559u,
605447875u,
2020163627u,
2617715070u,
1103727057u,
2258480518u,
2165912188u,
2947201371u,
2081537659u,
644776921u,
4118620868u,
363974389u,
905199200u,
627638179u,
1835587532u,
2802126216u,
2774111137u,
1673723690u,
3363414170u,
2995658914u,
3550413282u,
2132187758u,
139248760u,
627967564u,
2330809276u,
1505615540u,
2411898443u,
493196287u,
3144157617u,
3172224288u,
1016896250u,
1438474661u,
1587095402u,
1316415615u,
3919460051u,
2933596143u,
461564771u,
3144118554u,
1174824771u,
4185100848u,
3016493085u,
1526401710u,
4232452836u,
3745156599u,
344271551u,
3694018522u,
1738329156u,
3125288521u,
85879665u,
419306095u,
1624906243u,
1534852352u,
1260759687u,
1905369105u,
4191206851u,
693477668u,
222860196u,
34885609u,
4005808032u,
228188054u,
1495347587u,
401237012u,
826093379u,
185785990u,
198064113u,
2269441064u,
906039372u,
242479681u,
2288219566u,
3932795328u,
3858901106u,
868374514u,
3219306142u,
4252606023u,
1898148034u,
2375279408u,
1986766623u,
1400341558u,
855008142u,
3946711739u,
3012703294u,
4008166523u,
3266764469u,
3289631197u,
2049444523u,
1221338815u,
3684999856u,
1303439324u,
4208496413u,
3686835152u,
3466103086u,
4277408812u,
3847218481u,
2685096129u,
3124296366u,
3017716307u,
1848298856u,
609554906u,
2789074439u,
3547461938u,
2872222803u,
3162696655u,
4094070817u,
223166770u,
2169844335u,
2258301289u,
190586107u,
557710918u,
2680298483u,
4140427593u,
783602247u,
1688937466u,
2300770270u,
2209710492u,
4022025208u,
2395860702u,
3934208412u,
1275102284u,
1890449333u,
1213819019u,
2774425661u,
1104298241u,
1103222136u,
3952804337u,
3298035591u,
706936119u,
3902307752u,
1349246081u,
4172310169u,
3613360974u,
187358947u,
2696688034u,
1650116403u,
4143576738u,
130987594u,
4271267549u,
709498911u,
1345929239u,
780378437u};


class MSCR{
int64 datasize;
public:
unsigned char cach[16];
protected:
int64 thissize;
//int64 cachlevel[3];
int thislevel;
public:

MSCR(){ Clear(); }

void Cach(int64 size){ Clear(); datasize=size;/* thislevel=0;
cachlevel[0]=size/100; if(cachlevel[0]>S1M) cachlevel[0]=S1M;
cachlevel[1]=size/10; if(cachlevel[1]>S128M) cachlevel[1]=S128M;
cachlevel[2]=size/2; if(cachlevel[2]>S2G) cachlevel[2]=S2G;
*/}


//int Add(const MString& data){ // return cach level
//return Add(data.uchar(), data);
//}

int64 Add(const VString& data){ // return cach level
return Add(data, data.sz);
}

//int AddFast(const MString& data){ // return cach level // ADD Max Cache
//return AddFast(data.uchar(), data);
//}

//int AddFast(const VString& data){ // return cach level
//return AddFast(data, data.sz);
//}


int64 Add(const unsigned char *data, const unsigned int sz){
	int sf=sz;// int k; //unsigned int p;
	unsigned int *p, *l;

//	if(!datasize){ if(thislevel>3) thislevel-=4;
//	if(sf){
		//for(int i=0; i<sf; i++){
		for(int i=0; i<sf; i++){
			p=(unsigned int*)(&cach[thislevel-thislevel%4]);
		*(unsigned char*)(&cach[thislevel])+=*(unsigned char*)(data++);//data+i
		*p+=MSCRUNID[*(unsigned char*)(&cach[thislevel])]; // -- %256

		l=p-1;// p;
		while(l>=(unsigned int*)&cach){ *l+=*p/2; l--; }
		l=p+1;
		while(l<(unsigned int*)&cach[16]){ *l+=*p/2; l++; }
			//p=*(unsigned int*)(&cach[thislevel]);
		//for(k=thislevel*4+(i%4); k>=0; k--){
		//	*(unsigned char*)(&cach[k])+=p; p=(p+1)*(7)/(k+1);
		//}
		//k=thislevel-thislevel%4;
		//while(k>=0) *(unsigned int*)(&cach[k])
			
		//	k=thislevel*4+(i%4);
		//	unsigned char*uc=&cach[k], *to=&cach[0];
			
		/*	if(uc-to>=8){
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
			}*/

			//while(uc>=to){ *uc+=p; p=(p+1)*(7)/(k+1); uc--; k--; }
			
		//	p=*(unsigned int*)(&cach[thislevel]);

		//	k=thislevel*4+(i%4);
		//	uc=&cach[k]; to=&cach[16];
			
	/*		if(to-uc>=8){
				// *uc+=*((char*)&p+k%4)+k; uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			}*/			
			
			//while(uc<to){ *uc+=p; p=(p+1)*(7)/(k+1); uc++; k++; }

		//if(thislevel!=0) { *(unsigned int*)(&cach[0])+=*(unsigned int*)(&cach[thislevel*4])/2; }
		//if(thislevel!=1) { *(unsigned int*)(&cach[4])+=*(unsigned int*)(&cach[thislevel*4])/2; }
		//if(thislevel!=2) { *(unsigned int*)(&cach[8])+=*(unsigned int*)(&cach[thislevel*4])/2; }
		//if(thislevel!=3) { *(unsigned int*)(&cach[12])+=*(unsigned int*)(&cach[thislevel*4])/2; }
		thissize++; thislevel++; if(thislevel>=16) thislevel=0;
	//	if(i%4==3){ if(thislevel==3) thislevel=0; else thislevel++;}
		}//} thislevel+=4; return 4;
/*	}
else { BYTE i4=0;
	if(sf){ for(int i=0; i<sf; i++){
		cach[i4]+=*data++;
		*(unsigned int*)(cach+thislevel*4)+=MSCRUNID[cach[i4]];

	// agressive
			p=*(unsigned int*)(&cach[thislevel*4]);

			k=i4;
			unsigned char*uc=&cach[k], *to=&cach[0]; k++;
			if(uc-to>=8){
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
			}
			while(uc>=to){ *uc+=p; p=(p+1)*(7)/(k); uc--; k--; }
// >>>
			p=*(unsigned int*)(&cach[thislevel*4]);

		k=i4;
		uc=&cach[k]; to=&cach[16]; k++;
		if(to-uc>=8){
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
		}
		while(uc<to){ *uc+=p; p=(p+1)*(7)/(k+1); uc++; k++; }

	// end gressive

		thissize++; i4++;
		if(thislevel<3 && thissize>=cachlevel[thislevel]) thislevel++;
		if(i4>15) i4=0;
	}}}*/

	//if(thissize>=datasize) return 4; return thislevel;
	return thissize;

}

/*
int AddFast(const unsigned char *data, const unsigned int sz){
	int sf=sz; int k; unsigned int p;

	if(!datasize){if(thislevel>3) thislevel-=4;
	if(sf){ for(int i=0; i<sf; i++){
		*(unsigned char*)(&cach[thislevel*4+i%4])+=*(unsigned char*)(data++);//data+i
		*(unsigned int*)(&cach[thislevel*4])+=MSCRUNID[*(unsigned char*)(&cach[thislevel*4+i%4])%256];
		if((i%16)==0){
		p=*(unsigned int*)(&cach[thislevel*4]);
		//for(k=thislevel*4+(i%4); k>=0; k--){
		//	*(unsigned char*)(&cach[k])+=p; p=(p+1)*(7)/(k+1);
		//}
			k=thislevel*4+(i%4);
			unsigned char*uc=&cach[k], *to=&cach[0];
			
			if(uc-to>=8){
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k+1); uc--; k--;
			}

			while(uc>=to){ *uc+=p; p=(p+1)*(7)/(k+1); uc--; k--; }
			
			p=*(unsigned int*)(&cach[thislevel*4]);

			k=thislevel*4+(i%4);
			uc=&cach[k]; to=&cach[16];
			
			if(to-uc>=8){
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k+1); uc++; k++;
			}			
			
			while(uc<to){ *uc+=p; p=(p+1)*(7)/(k+1); uc++; k++; }
		}

		thissize++;
		if(i%4==3){ if(thislevel==3) thislevel=0; else thislevel++;}
}} thislevel+=4; return 4;
	}
else { BYTE i4=0;
	if(sf){ for(int i=0; i<sf; i++){
		cach[i4]+=*data++;
		*(unsigned int*)(cach+thislevel*4)+=MSCRUNID[cach[i4]];

	// agressive
		if(!i4){
			p=*(unsigned int*)(&cach[thislevel*4]);

			k=i4;
			unsigned char*uc=&cach[k], *to=&cach[0]; k++;
			if(uc-to>=8){
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
				*uc+=p; p=(p+1)*(7)/(k); uc--; k--;
			}
			while(uc>=to){ *uc+=p; p=(p+1)*(7)/(k); uc--; k--; }
// >>>
			p=*(unsigned int*)(&cach[thislevel*4]);

		k=i4;
		uc=&cach[k]; to=&cach[16]; k++;
		if(to-uc>=8){
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
			*uc+=p; p=(p+1)*(7)/(k); uc++; k++;
		}
		while(uc<to){ *uc+=p; p=(p+1)*(7)/(k+1); uc++; k++; }
		}

	// end gressive

		thissize++; i4++;
		if(thislevel<3 && thissize>=cachlevel[thislevel]) thislevel++;
		if(i4>15) i4=0;
	}}}

	if(thissize>=datasize) return 4; return thislevel;
}*/

MString GetHashBin(){MString ret; ret.Reserv(16); /*if(thissize>=datasize) cach[16]=4; else if(thislevel) cach[16]=(unsigned char)thislevel; else cach[16]=0;*/ memcpy(ret, &cach, 16); return ret; }

MString GetHash(){
	MString ret; ret.RClean(32, 0);
	unsigned char *l=ret; 
	// if(thissize>=datasize) cach[16]=4; else if(thislevel) cach[16]=(unsigned char)thislevel; else cach[16]=0;
	for(int i=0; i<16; i++){
	*l++=((unsigned char)cach[i]/16 + (unsigned char)(cach[i]/16<10 ? 48 : 87));
	*l++=((unsigned char)cach[i]%16 + (unsigned char)(cach[i]%16<10 ? 48 : 87));
	}// *l++=cach[16]+48;
	return ret;
}

void Clear(){ memset(this, 0, sizeof(MSCR)); }

/*
int HashFile(VString file, int64 maxsize=S1G, bool aoi=0, BYTE level=4, int buffer=1024, bool eb=1){
HFILE fl=CreateFile(file, _O_RDONLY, _S_IREAD| S_IWRITE); int ret=0;
if(ishandle(fl)){ ret=HashFile(fl, maxsize, aoi, level, buffer, eb); CloseHandle(fl);}
return ret;
}

// Fast + Generate Block Hache (1block - 16b, asz=size/gbs*16)
// eb - effective buffer

int HashFile(HFILEM fl, int64 maxsize=S32G, bool aoi=0, BYTE level=4, int buffer=1024, bool eb=1, int ghs=0, MString*gh=0){
fl.Lock(); int64 size=SetFilePointer(fl, 0, FILE_END); SetFilePointer(fl, 0, FILE_BEGIN);
Cach(size); MString data;
if(eb) data.Reserv(size<buffer ? size : (size<S512K ? (int)size : (S512K/buffer)*buffer)); else data.Reserv(buffer);
int red; int64 reda=0; BYTE thislevel; int _gh=0, _ghp=0; MSCR mgh; if(ghs) {if(!gh) ghs=0; else if(divur(size, ghs)*16<gh->size()) ghs=0; }

if(data.size()==buffer)
while(1){
	red=ReadFile(fl, data, data); reda+=red; if(red<1) break;	
	thislevel=Add(VString((char*)data, red));
	if(ghs){
		if(_gh>=ghs) {memcpy(gh->pos(_ghp*16), mgh.cach, 16); _ghp++; _gh=0; mgh.Clear(); }
		mgh.Add(VString((char*)data, red)); _gh+=red;
	}
	if(!aoi){ if(reda>maxsize && thislevel>=level) break;} else if(maxsize && reda>maxsize || thislevel>=level) break;
}
else while(1){ int p=0, b;
	red=ReadFile(fl, data, data); reda+=red; if(red<1) break;
	while(p<red){
		b=red-p>=buffer ? buffer : red-p; thislevel=Add(data.uchar()+p, b);
	if(ghs){ /// >>
	if(_gh>=ghs) {memcpy(gh->pos(_ghp*16), mgh.cach, 16); _ghp++; _gh=0; mgh.Clear(); }
	mgh.Add(VString((char*)data, red)); _gh+=red;
	}	/// <<
		p+=b;}
	if(!aoi){ if(reda>maxsize && thislevel>=level) break;} else if(maxsize && (reda>maxsize || thislevel>=level)) break;
}

if(ghs && _gh) {memcpy(gh->pos(_ghp*16), mgh.cach, 16); _ghp++; _gh=0; mgh.Clear(); }

fl.UnLock(); data.Empty();
return 1;
}*/



unsigned int Rand(unsigned int k=S4GM){
unsigned int ret;
	for(int i=0; i<4; i++){
	*(((unsigned char*)&ret)+i)=(unsigned char)cach[i*4]-((unsigned char)cach[i*4+1])*(unsigned char)cach[i*4+2]/((unsigned char)cach[i*4+3]+1);
	}
if(!k) return 0; return ret%k;
}

};

// Func
MString mscr(VString line){ MSCR mscr; mscr.Add(line); return mscr.GetHash(); }
MString mscrb(VString line){ MSCR mscr; mscr.Add(line); return mscr.GetHashBin(); }


MSCR mscrrand;// MString mscrrand_key="0";
unsigned int msrand(unsigned int limit=S4GM){
unsigned int u, z=0; u=(unsigned int)time(0);
mscrrand.Add(VString((char*)&u, 4));
#ifndef __GNUC__
//__asm{ mov dword ptr [u] , ebp  }
#endif
mscrrand.Add(VString((char*)&u, 4));
mscrrand.Add(VString(mscrrand.cach, 16));
//mscrrand.Add(mscrrand_key);
return mscrrand.Rand(limit);
}


unsigned int _mrand_r=clock(); unsigned int _mrand_rt=((unsigned int)time(0));
int mrand(int max){if(max<1) return 0; _mrand_r=(unsigned int)((_mrand_r+3)*clock()+time(0)+time(0));
return _mrand_rt=(unsigned(time(0)+clock())+_mrand_r*(_mrand_rt)+_mrand_r/3)%max;}

bool msrrand16(unsigned char *b){
memcpy(b, mscrrand.cach, 16); msrand(1);
for(int i=0; i<16; i++){ *b+=mscrrand.cach[i]; b++; }
return 1;
}

int64 mscrkbsize(int64 size){
int64 bs=size/S1K, ms=S32K;
while(bs<ms){ if(bs<ms) return bs=ms; ms*=2; }
return bs;
}


#ifdef WIN32
#define PRTHREAD_COND_SIGNAL 0
//#define PRTHREAD_COND_BROADCAST 1

typedef CRITICAL_SECTION pthread_mutex_t;
typedef CONDITION_VARIABLE pthread_cond_t;

void pthread_mutex_lock(pthread_mutex_t *shared_mutex){
	EnterCriticalSection(shared_mutex);
}

void pthread_mutex_unlock(pthread_mutex_t *shared_mutex){
	LeaveCriticalSection(shared_mutex);
}

void WINAPI pthread_cond_init_winxp(pthread_cond_t *cond){
	cond->Ptr = CreateEvent(NULL, FALSE, FALSE, NULL);
}

void WINAPI pthread_cond_wait_winxp(pthread_cond_t *cond, pthread_mutex_t *mutex, DWORD tm){
	pthread_mutex_unlock(mutex);
	WaitForMultipleObjects(1, (HANDLE*)&cond->Ptr, FALSE, tm);
	pthread_mutex_lock(mutex);
}

void WINAPI pthread_cond_signal_winxp(pthread_cond_t *cond){
  // Try to release one waiting thread.
  //PulseEvent(cv->events_[PRTHREAD_COND_SIGNAL]);
	SetEvent(cond->Ptr);
}


/*  For Windows XP
typedef struct{
  HANDLE events_[2];
  // Signal and broadcast event HANDLEs.
} pthread_cond_t;

void pthread_cond_init(pthread_cond_t *cv, const void*){
  // Create an auto-reset event.
  cv->events_[PRTHREAD_COND_SIGNAL] = CreateEvent (NULL,  // no security
                                     FALSE, // auto-reset event
                                     FALSE, // non-signaled initially
                                     NULL); // unnamed

  // Create a manual-reset event.
  cv->events_[PRTHREAD_COND_BROADCAST] = CreateEvent (NULL,  // no security
                                        TRUE,  // manual-reset
                                        FALSE, // non-signaled initially
                                        NULL); // unnamed
}

void pthread_cond_wait (pthread_cond_t *cv, pthread_mutex_t *external_mutex){
  // Release the <external_mutex> here and wait for either event
  // to become signaled, due to <pthread_cond_signal> being
  // called or <pthread_cond_broadcast> being called.
  LeaveCriticalSection(external_mutex);
  WaitForMultipleObjects (2, // Wait on both <events_>
                          cv->events_,
                          FALSE, // Wait for either event to be signaled
                          INFINITE); // Wait "forever"

  // Reacquire the mutex before returning.
  EnterCriticalSection(external_mutex);
}

void pthread_cond_signal(pthread_cond_t *cv){
  // Try to release one waiting thread.
  //PulseEvent(cv->events_[PRTHREAD_COND_SIGNAL]);
	SetEvent(cv->events_[PRTHREAD_COND_SIGNAL]);
}*/

typedef void (WINAPI *t_InitializeConditionVariable)(pthread_cond_t *external_cond);
typedef void (WINAPI *t_SleepConditionVariable)(pthread_cond_t *external_cond, pthread_mutex_t *external_mutex, DWORD tm);
typedef void (WINAPI *t_WakeConditionVariable)(pthread_cond_t *external_cond);

t_InitializeConditionVariable f_InitializeConditionVariable = 0;
t_SleepConditionVariable f_SleepConditionVariable = 0;
t_WakeConditionVariable f_WakeConditionVariable = 0;


class __UniqClassConditionVariable{

public:
	__UniqClassConditionVariable(){
		HMODULE module = GetModuleHandle(MODUNICODE("kernel32"));

		if(module){
			f_InitializeConditionVariable = (t_InitializeConditionVariable)GetProcAddress(module, "InitializeConditionVariable");
			f_SleepConditionVariable = (t_SleepConditionVariable)GetProcAddress(module, "SleepConditionVariableCS");
			f_WakeConditionVariable = (t_WakeConditionVariable)GetProcAddress(module, "WakeConditionVariable");
		}

		if(!f_InitializeConditionVariable || !f_SleepConditionVariable || !f_WakeConditionVariable){
			f_InitializeConditionVariable = pthread_cond_init_winxp;
			f_SleepConditionVariable = pthread_cond_wait_winxp;
			f_WakeConditionVariable = pthread_cond_signal_winxp;
		}

		return ;
	}

} __UniqClassConditionVariable;

void pthread_cond_init(pthread_cond_t *external_cond, const void*){
	f_InitializeConditionVariable(external_cond);
}

void pthread_cond_wait(pthread_cond_t *external_cond, pthread_mutex_t *external_mutex){
	f_SleepConditionVariable(external_cond, external_mutex, INFINITE);
}

void pthread_cond_signal(pthread_cond_t *external_cond){
	f_WakeConditionVariable(external_cond);
}

#endif


class GLock{
	pthread_mutex_t shared_mutex;

public:
	bool lock; bool flock;

GLock(){
	memset(this, 0, sizeof(GLock));
#ifdef WIN32
#ifndef WINCE
	if(shared_mutex.LockCount>=0 && !shared_mutex.OwningThread) { InitializeCriticalSection(&shared_mutex); flock=1; }
#else
	if(shared_mutex.LockCount==0 && !shared_mutex.hCrit){ InitializeCriticalSection(&shared_mutex); flock=1; }
#endif
#else
	pthread_mutex_init(&shared_mutex, NULL);
#endif
return ;
}

~GLock(){
	if(lock) UnLock();
#ifdef WIN32
	DeleteCriticalSection(&shared_mutex);
#else
	pthread_mutex_destroy(&shared_mutex);
#endif
}

bool Lock(){
#ifdef WIN32
#ifndef WINCE
	if(!flock && shared_mutex.LockCount>=0 && !shared_mutex.OwningThread && !shared_mutex.LockSemaphore){
		InitializeCriticalSection(&shared_mutex); flock=1; }
#else
	if(!flock && shared_mutex.LockCount==0 && !shared_mutex.hCrit && !shared_mutex.LockSemaphore){ InitializeCriticalSection(&shared_mutex); flock=1; }
#endif
#endif

#ifdef WIN32
	int llc=shared_mutex.LockCount;
	HANDLE t=shared_mutex.OwningThread;
	EnterCriticalSection(&shared_mutex);
	if(lock || !shared_mutex.OwningThread){ // || t
		globalerror("GLOCK Lock!\r\n");
	}
#else
	pthread_mutex_lock(&shared_mutex);
	if(lock){
		globalerror("GLOCK Lock!\r\n");
	}
#endif

	lock=1; return 1;
}

bool UnLock(){
#ifdef WIN32
	if(!lock || !shared_mutex.OwningThread){
		globalerror("GLOCK UnLock!\r\n");
	}
	lock=0;
	LeaveCriticalSection(&shared_mutex);
#else
	if(!lock){
		globalerror("GLOCK UnLock!\r\n");
	}
	lock=0;
	pthread_mutex_unlock(&shared_mutex);
#endif
return 1;
}

};

GLock tlocklock;

class TLock{
protected:
	pthread_mutex_t shared_mutex;
	DWORD threadid;
public:
	int lock; bool flock;

	TLock(){
		Init();	
	}

	void Init(){
		memset(this, 0, sizeof(*this));
#ifdef WIN32
	#ifndef WINCE
		if(shared_mutex.LockCount >= 0 && !shared_mutex.OwningThread && !shared_mutex.LockSemaphore){ InitializeCriticalSection(&shared_mutex); flock = 1; }
	#else
		if(shared_mutex.LockCount == 0 && !shared_mutex.hCrit && !shared_mutex.LockSemaphore){ InitializeCriticalSection(&shared_mutex); flock = 1; }
	#endif
#else
		pthread_mutex_init(&shared_mutex, NULL); flock = 1;
#endif
	}

~TLock(){
	if(lock) UnLock();
#ifdef WIN32
	DeleteCriticalSection(&shared_mutex);
#else
	pthread_mutex_destroy(&shared_mutex);
#endif
}

bool IsLock(){
	DWORD thid=GetCurrentThreadId();
	if(lock && thid!=threadid){
		return 1;
	}

	return 0;
}

bool Lock(){
#ifdef WIN32
	if(!flock){
		tlocklock.Lock();
		Init();
		tlocklock.UnLock();
	}
#endif

	DWORD thid=GetCurrentThreadId();
	if(lock && thid==threadid){
		lock++; return 1;
	}

#ifdef WIN32
	int llc=shared_mutex.LockCount;
	HANDLE t=shared_mutex.OwningThread;

	try{
	EnterCriticalSection(&shared_mutex);
	}catch(...){
		return 0;
	}
	if(lock || !shared_mutex.OwningThread){ // || t
		globalerror("GLOCK Lock!\r\n");
	}
#else
	pthread_mutex_lock(&shared_mutex);
	if(lock){
		globalerror("GLOCK Lock!\r\n");
	}
#endif

	lock=1; threadid=thid; return 1;
}

bool UnLock(){
	DWORD thid=GetCurrentThreadId();

	if(lock>1){
		if(thid!=threadid){
			globalerror("TLOCK UnLock thread id!\r\n"); return 0;
		}
		lock--; return 1;
	}

#ifdef WIN32
	if(!lock || !shared_mutex.OwningThread){
		globalerror("GLOCK UnLock!\r\n");
	}

	lock=0; threadid=0;
	LeaveCriticalSection(&shared_mutex);
#else
	if(!lock){
		globalerror("GLOCK UnLock!\r\n");
	}
	lock=0; threadid=0;
	pthread_mutex_unlock(&shared_mutex);
#endif
return 1;
}

};


// conditional lock
class CLock{
	pthread_mutex_t shared_mutex;
	pthread_cond_t shared_cond;
	int lock; bool flock;

public:
	CLock(){
		Init();
	}

private:
	void Init(){
		memset(this, 0, sizeof(*this));
#ifdef WIN32
	#ifndef WINCE
		if(shared_mutex.LockCount >= 0 && !shared_mutex.OwningThread && !shared_mutex.LockSemaphore){ InitializeCriticalSection(&shared_mutex); }
	#else
		if(shared_mutex.LockCount == 0 && !shared_mutex.hCrit && !shared_mutex.LockSemaphore){ InitializeCriticalSection(&shared_mutex); }
	#endif
#else
		pthread_mutex_init(&shared_mutex, NULL);
#endif
		pthread_cond_init(&shared_cond, 0);
		flock = 1;
	}

public:

	bool Lock(){
#ifdef WIN32
	if(!flock){
		tlocklock.Lock();
		Init();
		tlocklock.UnLock();
	}
#endif

	pthread_mutex_lock(&shared_mutex);
	lock = 1;
	return 1;
}

bool UnLock(){
	lock = 0;
	pthread_mutex_unlock(&shared_mutex);
	return 1;
}

	void Wait(int lockit = 1){
		if(lockit)
			pthread_mutex_lock(&shared_mutex);

		pthread_cond_wait(&shared_cond, &shared_mutex);

		if(lockit)
			pthread_mutex_unlock(&shared_mutex);
	}

	void Signal(){
		//if(lockit)
		//	pthread_mutex_lock(&shared_mutex);

		pthread_cond_signal(&shared_cond);

		//if(lockit)
		//	pthread_mutex_unlock(&shared_mutex);
	}
	
	~CLock(){
#ifdef WIN32
		//CloseHandle(shared_cond.events[PRTHREAD_COND_SIGNAL]);
		//CloseHandle(shared_cond.events[PRTHREAD_COND_BROADCAST]);		
#else
		pthread_cond_destroy(&shared_cond);
#endif
	}
};

//class TLock{
//	DWORD threadid; //char mutexuid[4+32+1];
////MString mutexuid;
//#ifdef WIN32
//	CRITICAL_SECTION shared_mutex;
//	//HANDLE shared_mutex, use_mutex;
//#else
//pthread_mutex_t shared_mutex;
//inline DWORD GetCurrentThreadId(){ return pthread_self (); }
//#endif
//
//public:
//	int lock;
//
//#ifdef WIN32
//TLock(){
//	lock=0; threadid=0;
//	msrand(1); memcpy(mutexuid, "mscr", 4); memcpy(mutexuid+4, mscrrand.GetHash(), 32); mutexuid[36]=0;
//	//mutexuid="dtxt"+mscrrand.GetCach(); 
//	shared_mutex = CreateMutex(0, true, MODUNICODE(mutexuid)); ReleaseMutex(shared_mutex);
//	return ;
//}
//
//~TLock(){ if(lock) UnLock(); ReleaseMutex(shared_mutex); CloseHandle(shared_mutex); return ; }
//
//bool Lock(){
//	DWORD thid=GetCurrentThreadId();
//	if(lock && thid==threadid){ lock++; return 1;}
//
//#ifndef WINCE
//HANDLE mutex=OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexuid);
//#else
//HANDLE mutex=CreateMutex(0, FALSE, MODUNICODE(mutexuid));
//#endif
//
//WaitForSingleObject(mutex, INFINITE);
//use_mutex=mutex;
//
//if(lock)
//	int err=1;
//lock=1; threadid=thid; return 1;
//}
//
//bool UnLock(){
//	DWORD thid=GetCurrentThreadId();
//	if(!lock){
//		int nolock=1; globalerror("TLOCK UnLock!\r\n"); return 0;
//	}
//	if(lock>1){
//		if(thid!=threadid){ globalerror("TLOCK UnLock thread id!\r\n"); return 0; }
//		lock--; return 1;
//	}
//lock=0; threadid=0;
//HANDLE mutex=use_mutex;// use_mutex=0;
//ReleaseMutex(mutex);
//CloseHandle(mutex);
//return 1;
//}
//#else
//TLock(){
//	lock=0; threadid=0;
//	pthread_mutex_init(&shared_mutex, NULL);
//	return ;
//}
//
//~TLock(){
//	pthread_mutex_destroy(&shared_mutex);
//return ; }
//
//bool Lock(){
//	DWORD thid=GetCurrentThreadId();
//	if(lock && thid==threadid){ lock++; return 1; }
//	pthread_mutex_lock(&shared_mutex);
//if(lock)
//	globalerror("TLOCK Lock!\r\n");
//	
//	//int err=1;
//lock=1; threadid=thid;
//	return 1;
//}
//
//bool UnLock(){
//	if(lock>1) {lock--; return 1;}
//	if(!lock)
//		int nolock=1;
//	lock=0; threadid=0;
//	pthread_mutex_unlock(&shared_mutex);
//	return 1;
//}
//#endif
//};


// Lock
#define UGLOCK(l) UGLock __l(l);
#define UGLOCK2(l) UGLock __l2(l);
#define UGLOCKD(l, r) UGLock __ld(l); UGLock __rd(r);

class UGLock{
	union {
		GLock *gl;
		TLock *tl;
	};
	int tp;

public:
	UGLock(GLock &g) : gl(&g){ g.Lock(); tp=1; }
	UGLock(TLock &t) : tl(&t){ t.Lock(); tp=2; }
	UGLock(GLock *g) : gl(g){ /*if(g){*/ g->Lock(); tp=1; /*} else tp=0;*/ }
	UGLock(TLock *t) : tl(t){ /*if(t){*/ t->Lock(); tp=2; /*} else tp=0;*/ }
	UGLock(){ gl=0; tp=0; }

//	void operator=(GLock &g){ gl=(GLock*)&g; gl->Lock(); tp=1; return ; }
//	void operator=(TLock &t){ tl=(TLock*)&t; tl->Lock(); tp=2; return ; }
	void Lock(GLock *g){ gl=(GLock*)g; gl->Lock(); tp=1; return ; }
	void Lock(TLock *t){ tl=(TLock*)t; tl->Lock(); tp=2; return ; }
	void operator=(GLock *g){ gl=(GLock*)g; gl->Lock(); tp=1; return ; }
	void operator=(TLock *t){ tl=(TLock*)t; tl->Lock(); tp=2; return ; }

	void set(GLock *g){ gl=(GLock*)g; gl->Lock(); tp=1; return ; }

	void UnLock(){ if(tp==1) gl->UnLock(); else if(tp==2) tl->UnLock(); tp=0; }
	~UGLock(){ if(tp==1) gl->UnLock(); else if(tp==2) tl->UnLock(); }
};

TLock memconlock;
bool MemConLock() { return memconlock.Lock(); }
bool MemConUnLock() { return memconlock.UnLock(); }

GLock filesystemlock;
bool FSLock(){ return filesystemlock.Lock(); }
bool FSUnLock(){ return filesystemlock.UnLock(); }
