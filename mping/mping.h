//typedef struct {
//   unsigned char Ttl;                                 // Time To Live
//   unsigned char Tos;                                 // Type Of Service
//   unsigned char Flags;                               // IP header flags
//   unsigned char OptionsSize;                         // Size in bytes of options data
//   unsigned char *OptionsData;                        // Pointer to options data
//} IP_OPTION_INFORMATION, * PIP_OPTION_INFORMATION;
//
//#ifdef WIN32
//typedef HANDLE (WINAPI* ICMPCreateFile)(VOID);
//typedef BOOL (WINAPI* ICMPCloseHandle)(HANDLE);
//typedef DWORD (WINAPI* ICMPSendEcho)(HANDLE, DWORD, LPVOID, WORD,PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
//
//ICMPCreateFile icmpCreateFile;
//ICMPCloseHandle icmpCloseHandle;
//ICMPSendEcho icmpSendEcho;
//
//
//class LoadICMP{
//	HANDLE lib;
//public:
//
//	LoadICMP(){
//		HMODULE lib = LoadLibrary("ICMP.DLL");
//		icmpCreateFile = (ICMPCreateFile)GetProcAddress(lib, "IcmpCreateFile");
//		icmpCloseHandle = (ICMPCloseHandle)GetProcAddress(lib, "IcmpCreateFile");
//		icmpSendEcho = (ICMPSendEcho)GetProcAddress(lib, "IcmpCloseHandle");
//	}
//
//}LoadICMP;
//#endif

unsigned int MPINGSTOP = 0;

class MPing{
public:
	// data
	MString to, data;
	unsigned int tip;
	// config
	MString bind;
	unsigned int type, code, seq, usettl;
	//	unsigned int ttl;

	// return time, destination, source, size, type, code, ttl
	double rtm;
	unsigned int rdst, rscr, rsz, rttl;
	int rtype, rcode;

	// statistics
	int s_cnt, s_okcnt;
	double s_alltm, s_mintm, s_maxtm;

public:

	MPing(){
		Clear();
		data="ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	}

	void Clear(){
		type=8; code=0; seq=-1; usettl=0; s_alltm=0; s_mintm=0; s_maxtm=0;
		s_cnt=0; s_okcnt=0;
		bind.Clean();
	}

	void Ping(VString a){
		to=a; tip=GetIP(a);
	}

	void Data(VString d){
		data=d;
	}

	// use Ping ping; ping.ttl=int; type and code.

	int Do(int timeout=3000){ // go
		ItosX<S4K> it, ir; MTimer mr; double mra, /*mrt,*/ mre;
		if(!it.Reserv(sizeof(icmphdr)+data.size())) return 0; ir.Reserv(S4K-1);

		// header & data
		icmphdr &ih=*(icmphdr*)it.ret.data; if(seq==-1) seq=msrand();
		sockaddr_in from, to; int fromlen=sizeof(from);
		memcpy(ih.data, data, data);
		// icmp & crc
		ih.type=type; ih.code=code; ih.id=ntohs(1); ih.seq=++seq; ih.crc=0;
		ih.crc=crc16((unsigned short*)&ih, sizeof(icmphdr)+data.size());

		// sock & local ip
		int sock=ConnectPortEx(tip, 0, SOCK_RAW, IPPROTO_ICMP, GetIP(bind));
		//int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
		if(!sock){ return -1; }

		if(usettl){ setsockopt(sock, IPPROTO_IP, IP_TTL, (char*)&usettl, 1); }

		// sniffer
		sniffer sniff; sniff.noprint=1; // struct sockaddr_in from; int fromlen=sizeof(from);
		getsockname(sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);
		sniff.Start();

		// send
		int se=send(sock, it.ret, it, 0);
		mra=mr.dtime(); mre=mra;

		//memset(&to, 0, sizeof(to));
		//to.sin_family = AF_INET;
		//to.sin_addr.s_addr = htonl(tip);
		//to.sin_port = 0;
		
		//int se=sendto(sock, it.ret, it, 0, (sockaddr*)&to, fromlen);
		if(se<=0){ closesocket(sock); return 0; }

		// wait
		//timeval tm; fd_set rfds;
		s_cnt++;

		while(mre-mra<timeout){ mre=mr.dtime();
		//	FD_ZERO(&rfds); FD_SET(sock, &rfds);
		//	tm.tv_sec=timeout/1000; tm.tv_usec=(timeout%1000)*1000;

		//	if(select(sock+1, &rfds, 0, 0, &tm)<=0){
		//		//mre=mr.dtime();
		//		//if(mre-mra<timeout){
		//		//	Sleep(5); // if(wait<1000) select fast exit.
		//		//	continue;
		//		//}
		//		//else{
		//		closesocket(sock); return 0;
		//	//	}
		//	}

			sniff.Do();
			u16	ep = ntohs(*(u16*)(sniff.buf + 12));
#ifdef WIN32
			ep=0x0800;
#endif

			if(sniff.ip->protocol==IPPROTO_ICMP && ep==0x0800){
				int z=34;
			} else continue;

			// recv
			se=sniff.rc; ir.set(VString((char*)sniff.ip, sniff.ipsz));
			//se = recv(sock, ir.ret, ir, 0);
			/*closesocket(sock);*/ mre=mr.dtime();
			if(se<=0 || se<sizeof(iphdr)+sizeof(icmphdr)) break;
			
			iphdr &ipr=*(iphdr*)(ir.ret.data);
			icmphdr &ihr=*(icmphdr*)(ir.ret.data+sizeof(iphdr));
			VString rd((char*)&ihr+sizeof(icmphdr), se-sizeof(icmphdr));

			if(ipr.saddr==from.sin_addr.s_addr && ipr.daddr==tip && type==ihr.type && code==ihr.code) continue;

			if(ihr.id==ih.id && ihr.seq==ih.seq){ // my packet
				// time, size, ttl, icmp.type, icmp.code, source & destination ip.
				rtm=mre-mra; rsz=rd.sz; rttl=ipr.ttl;
				rtype=ihr.type; rcode=ihr.code;
				rdst=htonl(ipr.daddr); rscr=htonl(ipr.saddr);

				if(!s_mintm || s_mintm>rtm) s_mintm=rtm;
				if(s_maxtm<rtm) s_maxtm=rtm;
				s_alltm+=rtm; s_okcnt++;
				closesocket(sock);
				return 1;
			}

			else{ // my packet
				if(ir.ret.size()-sizeof(iphdr)-sizeof(icmphdr)<sizeof(iphdr)+sizeof(icmphdr)) 
					continue;

				iphdr &ipr2=*(iphdr*)(ir.ret.data+sizeof(iphdr)+sizeof(icmphdr));
				icmphdr &ihr2=*(icmphdr*)(ir.ret.data+sizeof(iphdr)+sizeof(icmphdr)+sizeof(iphdr));

				if(/*ipr2.version==4 && */ipr2.protocol==IPPROTO_ICMP && ipr2.ttl==1 && ipr2.saddr==from.sin_addr.s_addr && ipr2.daddr==tip && ihr2.type==type && ihr2.code==code && ihr2.id==ih.id && ihr2.seq==ih.seq){
				// time, size, ttl, icmp.type, icmp.code, source & destination ip.
				rtm=mre-mra; rsz=rd.sz; rttl=ipr.ttl;
				rtype=ihr.type; rcode=ihr.code;
				rdst=htonl(ipr.daddr); rscr=htonl(ipr.saddr);

				if(!s_mintm || s_mintm>rtm) s_mintm=rtm;
				if(s_maxtm<rtm) s_maxtm=rtm;
				s_alltm+=rtm; s_okcnt++;
				closesocket(sock);
				return 1;
				}
			}
		}

		closesocket(sock);
		return 0;
	}

};



VString icmptostr(unsigned int t, unsigned int c, VString &vc){
	vc.Clean();

	switch(t){
		case 0: return "Echo reply";
		case 1: return "Reserved";
		case 2: return "Reserved";
		
		case 3:
			switch(c){
				case 0: vc="Destination network unreachable"; break;
				case 1: vc="Destination host unreachable"; break;
				case 2: vc="Destination protocol unreachable"; break;
				case 3: vc="Destination port unreachable"; break;
				case 4: vc="Fragmentation required, and DF flag set"; break;
				case 5: vc="Source route failed"; break;
				case 6: vc="Destination network unknown"; break;
				case 7: vc="Destination host unknown"; break;
				case 8: vc="Source host isolated"; break;
				case 9: vc="Network administratively prohibited"; break;
				case 10: vc="Host administratively prohibited"; break;
				case 11: vc="Network unreachable for TOS"; break;
				case 12: vc="Host unreachable for TOS"; break;
				case 13: vc="Communication administratively prohibited"; break;
				//case 14: vc="Нарушение порядка предпочтения узлов"; break;
				//case 15: vc="Активно отсечение порядка предпочтения"; break;
			}
			return "Destination Unreachable";

		case 4: return "Source quench (congestion control)";
		case 5:
			switch(c){
				case 0: vc="Redirect Datagram for the Network"; break;
				case 1: vc="Redirect Datagram for the Host"; break;
				case 2: vc="Redirect Datagram for the TOS & network"; break;
				case 3: vc="Redirect Datagram for the TOS & host"; break;
			}			
			return "Redirect Message";
		
		case 6: return "Alternate Host Address";
		case 7: return "Reserved";
		case 8: return "Echo request";
		case 9:
			//switch(c){
			//	case 0: vc="Нормальное объявление маршрутизатора"; break;
			//	case 16: vc="Не маршрутизировать обычный трафик"; break;
			//}
			return "Router Advertisement";

		case 10: return "Router discovery/selection/solicitation";
		case 11:
			switch(c){
				case 0: vc="TTL expired in transit"; break;
				case 1: vc="Fragment reassembly time exceeded"; break;
			}			
			return "Time Exceeded";
		
		case 12:
			switch(c){
				case 0: vc="Pointer indicates the error"; break;
				case 1: vc="Missing a required option"; break;
				case 2: vc="Bad length"; break;
			}
			return "Parameter Problem: Bad IP header";

		case 13: return "Timestamp";
		case 14: return "Timestamp reply";
		case 15: return "Information Request";
		case 16: return "Information Reply";
		case 17: return "Address Mask Request";
		case 18: return "Address Mask Reply";
		case 19: return "Reserved for security";
		case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29:
			return "Reserved for robustness experiment";
		
		case 30: return "Traceroute";
		case 31: return "Datagram Conversion Error";
		case 32: return "Mobile Host Redirect";
		case 33: return "IPv6 Where-Are-You";
		case 34: return "IPv6 I-Am-Here";
		case 35: return "Mobile Registration Request";
		case 36: return "Mobile Registration Reply";
		case 37: return "Domain Name Request";
		case 38: return "Domain Name Reply";
		case 39: return "SKIP Algorithm Discovery Protocol";
		case 40: 
/*			switch(c){
				case 0: vc="Зарезервировано"; break;
				case 1: vc="Неизвестный индекс параметров безопасности"; break;
				case 2: vc="Параметры безопасности верны, но произошла ошибка аутентификации"; break;
				case 3: vc="Параметры безопасности верны, но произошел сбой при расшифровке"; break;
				case 4: vc="Требуется проверка подлинности"; break;
				case 5: vc="Требуется авторизация"; break;
			}*/			
			return "Photuris, Security failures";
		case 42:
			return "ICMP for experimental mobility protocols such as Seamoby";

		default: return "Reserved";
			break;
	}
};


VString icmptostr_ru(unsigned int t, unsigned int c, VString &vc){
	vc.Clean();

	switch(t){
		case 0: return "Эхо-ответ";
		case 1: return "Зарезервировано";
		case 2: return "Зарезервировано";
		
		case 3:
			switch(c){
				case 0: vc="Сеть недостижима"; break;
				case 1: vc="Узел недостижим"; break;
				case 2: vc="Протокол недостижим"; break;
				case 3: vc="Порт недостижим"; break;
				case 4: vc="Необходима фрагментация, но установлен флаг ее запрета (DF)"; break;
				case 5: vc="Неверный маршрут от источника"; break;
				case 6: vc="Сеть назначения неизвестна"; break;
				case 7: vc="Узел назначения неизвестен"; break;
				case 8: vc="Узел источник изолирован"; break;
				case 9: vc="Сеть административно запрещена"; break;
				case 10: vc="Узел административно запрещен"; break;
				case 11: vc="Сеть недоступна для ToS"; break;
				case 12: vc="Узел недоступен для ToS"; break;
				case 13: vc="Коммуникации административно запрещены"; break;
				case 14: vc="Нарушение порядка предпочтения узлов"; break;
				case 15: vc="Активно отсечение порядка предпочтения"; break;
			}
			return "Адресат недоступен";

		case 4: return "Сдерживание источника (отключение источника при переполнении очереди)";
		case 5:
			switch(c){
				case 0: vc="Перенаправление пакетов в сеть"; break;
				case 1: vc="Перенаправление пакетов к узлу"; break;
				case 2: vc="Перенаправление для каждого типа обслуживания (TOS)"; break;
				case 3: vc="Перенаправление пакета к узлу для каждого типа обслуживания"; break;
			}			
			return "Перенаправление";
		
		case 6: return "Альтернативный адрес узла";
		case 7: return "Зарезервировано";
		case 8: return "Эхо-запрос";
		case 9:
			switch(c){
				case 0: vc="Нормальное объявление маршрутизатора"; break;
				case 16: vc="Не маршрутизировать обычный трафик"; break;
			}
			return "Объявление маршрутизатора";

		case 10: return "Запрос маршрутизатора";
		case 11:
			switch(c){
				case 0: vc="Время жизни пакета (TTL) истекло при транспортировке"; break;
				case 1: vc="Время жизни пакета (время сборки фрагментов) истекло при дефрагментации"; break;
			}			
			return "Превышение временного интервала (для дейтаграммы время жизни истекло)";
		
		case 12:
			switch(c){
				case 0: vc="Указатель говорит об ошибке"; break;
				case 1: vc="Отсутствует требуемая опция"; break;
				case 2: vc="Некорректная длина"; break;
			}
			return "Неверный параметр (проблема с параметрами дейтаграммы: ошибка в IP-заголовке или отсутствует необходимая опция)";

		case 13: return "Запрос метки времени";
		case 14: return "Ответ с меткой времени";
		case 15: return "Информационный запрос";
		case 16: return "Информационный ответ";
		case 17: return "Запрос адресной маски";
		case 18: return "Отклик на запрос адресной маски";
		case 19: return "Зарезервировано (для обеспечения безопасности)";
		case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29:
			return "Зарезервировано (для экспериментов на устойчивость к ошибкам)";
		
		case 30: return "Трассировка маршрута";
		case 31: return "Ошибка преобразования датаграммы";
		case 32: return "Перенаправление для мобильного узла";
		case 33: return "IPv6 Where-Are-You (где вы находитесь)";
		case 34: return "IPv6 I-Am-Here (я здесь)";
		case 35: return "Запрос перенаправления для мобильного узла";
		case 36: return "Отклик на запрос перенаправления для мобильного узла";
		case 37: return "Запрос доменного имени";
		case 38: return "Ответ на запрос доменного имени";
		case 39: return "SKIP";
		case 40: 
			switch(c){
				case 0: vc="Зарезервировано"; break;
				case 1: vc="Неизвестный индекс параметров безопасности"; break;
				case 2: vc="Параметры безопасности верны, но произошла ошибка аутентификации"; break;
				case 3: vc="Параметры безопасности верны, но произошел сбой при расшифровке"; break;
				case 4: vc="Требуется проверка подлинности"; break;
				case 5: vc="Требуется авторизация"; break;
			}			
			return "Photuris";

		default: return "Зарезервировано";
			break;
	}
};