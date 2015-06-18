#define PROJECTNAME "mping"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#define USEMSV_ITOS

#include "../../opensource/msvcore/msvcore.cpp"

#include "../../opensource/msvcore/net/sniffer.h"
#include "../../opensource/msvcore/net/sniffer_tcp.h"

#include "signal.h"
#include "mping.h"

Versions mpingversions[]={
"0.0.2.0", "09.06.2012 11:27",
"0.0.1.0", "03.12.2010 15:22",
"0.0.0.1", "03.12.2010 14:01"
};

void SigStop(int code){
	if(MPINGSTOP) exit(0);
	MPINGSTOP=1;
//	Sleep(5000);
//	exit(0);
	return ;
}


DWORD LPT(LPVOID){
	sniffer sniff;
	sniff.Start(); sniff.noprint=1;
	while(1){
		sniff.Do();
	}

	return 0;
}

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link); LoadLang("lang");
	print("mping v", mpingversions[0].ver," (", mpingversions[0].date, ") [--uselang ", msv_use_lang, "]\r\n\r\n");

	//StartThread(LPT);
	//LPT(0);

	// signals
#ifndef WIN32
	signal(SIGIO, SIG_IGN);
	//signal(SIGKILL, SigStop);
//	signal(SIGSTOP, SigStop);
#else
	signal(SIGBREAK, SigStop);
	signal(SIGABRT, SigStop);
#endif
	signal(SIGINT, SigStop);
	signal(SIGTERM, SigStop);

	// config
	VString host, bind, data;
	unsigned int cycles=0, wait=4000, ishelp=0, isver=0;
	// ext
	int type=8, code=0, seq=-1, ttl=0; // icmp.type & icmp.code & icmp.seq
	//
	int ver=0, tracert=0;

	// analys command line
	//VString l, t; l=PartLine(link.GetArg(), t);

	// analys command line
	VString l="GO", t; int lto=2, ltop=1; 

	while(l){
		if(!ltop && !lto) lto=1; ltop=lto;
		if(lto==1){ l=t; t=link.GetArg(); lto=0; }
		else if(lto==2){ l=link.GetArg(); t=link.GetArg(); lto=0; }		
		if(!l) break;

		if(l=="-h" || l=="--help"){ lto=1; ishelp=1; }
		else if(l=="--ver"){ lto=1; isver=1; }
		else if(lto=usemsv_iskey(l)){}		

		else if(l=="-t"){ lto=1; cycles=99999999; }
		else if((l=="--cycle" || l=="-c") && t){ lto=2; cycles=t.toi(); }
		else if((l=="--wait" || l=="-w") && t){lto=2; wait=t.toi(); }
		else if(l=="--type" && t){ lto=2; type=t.toi(); }
		else if(l=="--code" && t){ lto=2; code=t.toi(); }
		else if(l=="--seq" && t){ lto=2; seq=t.toi()-1; }
		else if(l=="--ttl" && t){ lto=2; ttl=t.toi(); }
		else if((l=="--bind" || l=="-b") && t){ lto=2; bind=t; }
		else if((l=="--data" || l=="-d") && t){ lto=2; data=t; }
		else if(l=="--tracert"){ lto=1; tracert=1; }
		else if(!host){ host=l; lto=1; }
		else { print(Lang("Invalid options: unknown key"), " '", l, "'\r\n"); return 1; }
	}

/*while(l){
	if(l=="--help" || l=="-h" || l=="/?"){ ishelp=1; }
	else if(l=="-t"){ cycles=99999999; }
	else if((l=="--cycle" || l=="-c") && t.is()){ cycles=t.toi(); }
	else if((l=="--wait" || l=="-w") && t.is()){ wait=t.toi(); }
	else if(l=="-w" && t.is()){ cycles=99999999; }
	else if(l=="--type" && t.is()){ type=t.toi(); }
	else if(l=="--code" && t.is()){ code=t.toi(); }
	else if(l=="--seq" && t.is()){ seq=t.toi()-1; }
	else if(l=="--ttl" && t.is()){ ttl=t.toi(); }
	else if((l=="--bind" || l=="-b") && t.is()){ bind=t; }
	else if((l=="--data" || l=="-d") && t.is()){ data=t; }
	else if(l=="--ver"){ ver=1; }
	else if(l=="--tracert"){ tracert=1; }
	else if(!host) host=l;
	else print("uncnown parameter: ", l, "\r\n");
	l=PartLine(link.GetArg(), t);
}*/

	if((!host || ishelp) && !ver){ // help
		print(Lang("Usage: mping [flags: -key val ] hostname"), "\r\n");
		print(Lang(" ## msv options: use --usemsv --help"), "\r\n");
		print(Lang(" ## info"), "\r\n");
		print("--help", "\t\t", Lang("print this help"), "\r\n");
		print("--ver", "\t\t", Lang("print versions list"), "\r\n");
		print(Lang(" ## mode"), "\r\n");
		print("--tracert\t", Lang("traceroute mode"), "\r\n");		
		print(Lang(" ## options"), "\r\n");
		print("-c, --cycle\t", Lang("set cycles"), "\r\n");
		print("-t\t\t", Lang("set 99999999 cycles"), "\r\n");
		print("-w, --wait\t", Lang("set wait time"), "\r\n");
		print("-b, --bind\t", Lang("bind to local ip"), "\r\n");
		print(Lang(" ## extended"), "\r\n");
		print("--type\t\t", Lang("set icmp.type"), "\r\n");
		print("--code\t\t", Lang("set icmp.code"), "\r\n");
		print("--seq\t\t", Lang("set first icmp.seq"), "\r\n");
		print("--ttl\t\t", Lang("set ttl"), "\r\n");
		return 0;
	}

	if(isver){
		VersionsPrint(mpingversions, sizeof(mpingversions)/sizeof(Versions));
		return 0;
	}

	MPing ping; Itos it, it2;
	VString ttype, tcode;

	// ping parameters
	ping.Ping(host); ping.bind=bind;
	if(data) ping.Data(data);
	ping.type=type; ping.code=code; ping.seq=seq; ping.usettl=ttl;

	// print
	if(!tracert)
		print(Lang("ping to"), " ", ping.to, "(", it.ipitos(ping.tip), ")\r\n");
	else
		print(Lang("tracert to"), " ", ping.to, "(", it.ipitos(ping.tip), ")\r\n");
		
	// Do it
	if(!tracert){
		if(!cycles) cycles=4;

		for(int i=0; i<cycles; i++){
			int r=ping.Do(wait);

			if(r>0){ // good
				ttype=icmptostr(ping.rtype, ping.rcode, tcode);

				if(tcode) it2.Add(" (", tcode, ")"); else it2.Reserv(0);
				it.Format(Lang("Answer from %ip!: %d bytes, %3fms, TTL %d. %s %s"),
					ping.rscr, ping.rsz, ping.rtm, ping.rttl, ttype, it2.ret);
				print(it); print("\r\n");
			}
			else if(!r){ // error.
				it.Format(Lang("Timeout."));
				print(it); print("\r\n");
			}
			else if(r<0){ // need root
				print(Lang("Sorry, need root acces for use RAW socket."));
				break;
			}

			Sleep(1000);
			if(MPINGSTOP) break;
		}

		// statistics
		print("\r\n");
		it.Format(Lang("Result ping to %ip!: %d packets sended, %d recieved, min/avg/max %3f/%3f/%3f ms"),
			ping.tip, ping.s_cnt, ping.s_okcnt, ping.s_mintm, ping.s_alltm/ping.s_okcnt, ping.s_maxtm);
		print(it); print("\r\n");
	}
	else{
		if(!cycles) cycles=30;

		for(int i=0; i<cycles; i++){
			print("  ", itos(i+1), "  ");
			int fip=0, sz=4+(i<9 ? 1 : (i<100 ? 2 : 3));

			for(int j=0; j<3; j++){
                ping.usettl=i+1;
				int r=ping.Do(wait);
				if(r<0){ // need root
					print(Lang("Sorry, need root acces for use RAW socket.")); cycles=0; break;
				}

				if(j==0) while(sz<6){ print(" "); sz++; } 
				else if(j==1) while(sz<16){ print(" "); sz++; }
				else if(j==2) while(sz<26){ print(" "); sz++; }

				if(r>0){
					it.Format("%2fms", ping.rtm); sz+=it;
					print(it); fip=1;
				}
				else{ print("*"); sz+=1; }

				Sleep(500);
			}

			while(sz<36){ print(" "); sz++; }
			
			if(fip) it.Format("%ip!\r\n", ping.rscr);
			else it.Format("\r\n");
			print(it);

			if(ping.rscr==ping.tip){
				print(Lang("Ok."), "\r\n"); break;
			}

			Sleep(1000);
			if(MPINGSTOP) break;
		}

		// statistics
		//it.Format("\r\nResult ping to %ip!: %d packets sended, %d recieved, min/avg/max %3f/%3f/%3f ms\r\n",
		//	ping.tip, ping.s_cnt, ping.s_okcnt, ping.s_mintm, ping.s_alltm/ping.s_okcnt, ping.s_maxtm);
		//print(it);
	}



	return 1;
}

