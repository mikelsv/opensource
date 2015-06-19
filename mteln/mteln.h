// format
// loop:15; code; do
// send:"data"

#define MTELNS_LOOP		1
#define MTELNS_DO		2
#define MTELNS_SEND		3
#define MTELNS_WAIT		4
#define MTELNS_SETIT	5

//int mtelns_sz[]={0, }

//struct MTelnSLoop{
//	unsigned int steps, step, do pos; // step, all steps, 
//};
//
//struct MTelnSLoopDo{
//	unsigned int ploop; // loop position
//};
//
//struct MTelnSend{
//	unsigned int sz;
//};
//
//struct MTelnWait{
//	unsigned int wait;
//};

// mods:
// loop, pos
// do, loop pos, do pos

class MTelnScript{
	MString script;
	MString code; unsigned char *codep;
	MTimer mt;
	SOCKET sock;
	unsigned int it, waita; int64 wtm; // iterations, wait activate, wait time

	public:
		MTelnScript(){ sock=0; codep=0; waita=0; }
		int Run(VString line){
			script=line; it=1; codep=0;

			unsigned char *ln=script, *lln=ln, *to=script.endu();
			while(ln<to){
				if(*ln=='\\' && ln+1<to){
					if(*(ln+1)=='r' || *(ln+1)=='n' || *(ln+1)=='\\'){
						if(*(ln+1)=='r') *ln='\r';
						if(*(ln+1)=='n') *ln='\n';
						if(*(ln+1)=='\\') *ln='\\';
						memcpy(ln+1, ln+2, to-ln-2); to--;
					}
				}
				ln++;
			} script.sz=to-script.data;

			HLString ls; int type; VString scr=script;
			//unsigned int loopid=-1;
			unsigned int mods[S4K], mods1[S4K], mods2[S4K], modsi=0; // id + mod
			unsigned int i;

			//while(scr){
				//unsigned char *ln=scr, *lln=ln, *to=scr.endu();
			ln=scr; lln=ln; to=scr.endu();

				while(ln<to){
					if((*ln==' ' || *ln=='\r' || *ln=='\n' || *ln=='\t') && lln+1==ln){ lln++;}

					if(*ln==';' || ln+1==to){ if(ln+1==to) ln++;
						VString t=VString(lln, ln-lln);
						VString o=PartLine(t, t, ":");

						if(modsi>=S4K){ print("mteln script fail: OMG mods buffer is full.\r\n"); }

						if(o=="loop"){
							mods[modsi]=MTELNS_LOOP; mods1[modsi]=ls.size(); modsi++;

							type=MTELNS_LOOP; ls.add((char*)&type, 1);
							if(t) type=t.toi(); else type=S1G;
							ls.add((char*)&type, 4);
							type=0; ls.add((char*)&type, 4); // count
							type=0; ls.add((char*)&type, 4); // do pointer
						}
						else if(o=="do"){
							i=-1; if(modsi) for(unsigned int j=modsi-1; j<=modsi-1; j--){
								if(mods[j]==MTELNS_LOOP){ i=mods1[j]; mods[j]=MTELNS_DO; mods2[j]=ls.size(); break; }
							}
							if(i==-1){ print("mteln script error: do prevous loop\r\n"); code.Clean(); return 0; }

							type=MTELNS_DO; ls.add((char*)&type, 1);
							ls.add((char*)&i, 4);
						}
						else if(o=="send"){
							if(t.sz>1 && t[0]=='"' && t.endo()=='"'){ t.data++; t.sz-=2; }
							type=MTELNS_SEND; ls.add((char*)&type, 1);
							type=t.sz; ls.add((char*)&type, 4);
							ls.add(t, t);
						}
						else if(o=="wait"){
							type=MTELNS_WAIT; ls.add((char*)&type, 1);
							if(t) type=t.toi(); else type=1000;
							ls.add((char*)&type, 4);
						}
						else if(o=="set-it"){
							type=MTELNS_SETIT; ls.add((char*)&type, 1);
							if(t) type=t.toi(); else type=1000;
							ls.add((char*)&type, 4);
						}
						else{ print("mteln script error: command not found: ", o, "\r\n"); code.Clean(); return 0; }
						lln=ln+1;
					}
					ln++;
				}
			//}
				ls.String(code);
				
				if(modsi) for(unsigned int j=0; j<modsi; j++){
					if(mods[j]==MTELNS_DO){
						*(unsigned int*)(code.data+mods1[j]+9)=mods2[j];
					}
				}

			return 1;
		}

		void Sock(SOCKET s){ sock=s; }

		void Do(){
			if(!codep || codep<code.data || codep>code.endu()) codep=code.data;
			unsigned char *codet=code.endu();
			unsigned int tit=it;
			unsigned int i, j;

			if(waita){
				int64 t=mt.time();
				if(wtm+waita<t){ waita=0; } else return ;
			}

			while(codep<codet && tit){
				switch(*codep){
				case MTELNS_SEND:
					j=*(unsigned int*)(codep+1);
					if(sock) send(sock, (char*)codep+5, j, 0);
					codep+=5+j;
					break;
				case MTELNS_LOOP:{
					unsigned int &steps=*(unsigned int*)(codep+1);
					unsigned int &step=*(unsigned int*)(codep+1+4);
					unsigned int &dopos=*(unsigned int*)(codep+1+8);
					if(step>=steps){ codep=code.data+dopos+5; } else{ codep+=13; step++; }
					break;
				}
				case MTELNS_DO:
					j=*(unsigned int*)(codep+1);
					codep=code.data+j;
					//codep+=5;
					break;
				case MTELNS_WAIT:
					j=*(unsigned int*)(codep+1);
					codep+=5;
					wtm=mt.time(); waita=j; tit=1;
					break;
				case MTELNS_SETIT:
					j=*(unsigned int*)(codep+1);
					it=j; codep+=5;
					break;
				default:
					print("mteln script fail: type not found!\r\n");
					codep=codet; return ;
					break;
				}

				tit--;
			}

			return ;
		}
};