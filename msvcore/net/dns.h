#define MDNSUSE
//#include "dns.h"
// Define
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#define DNS_TYPE_ZERO	0x0000 // An empty record type (section 3.6 in [RFC1034] and section 3.2.2 in [RFC1035]).
#define DNS_TYPE_A		0x0001 // An A record type, used for storing an IP address (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_NS		0x0002 // An authoritative name-server record type (section 3.6 in [RFC1034] and section 3.2.2 in [RFC1035]).
#define DNS_TYPE_MD		0x0003 // A mail-destination record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_MF		0x0004 // A mail forwarder record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_CNAME	0x0005 // A record type that contains the canonical name of a DNS alias (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_SOA	0x0006 // A Start of Authority (SOA) record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_MB		0x0007 // A mailbox record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_MG		0x0008 // A mail group member record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_MR		0x0009 // A mail-rename record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_NULL	0x000A // A record type for completion queries (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_WKS	0x000B // A record type for a well-known service (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_PTR	0x000C // A record type containing FQDN pointer (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_HINFO	0x000D // A host information record type (section 3.2.2 in [RFC1035]).
#define	DNS_TYPE_MINFO	0x000E // A mailbox or mailing list information record type (section 3.2.2 in [RFC1035]).
#define DNS_TYPE_MX		0x000F // A mail-exchanger record type (section 3.2.2 in [RFC1035]).
#define	DNS_TYPE_TXT	0x0010 // A record type containing a text string (section 3.2.2 in [RFC1035]).
#define	DNS_TYPE_RP		0x0011 // A responsible-person record type [RFC1183].
#define	DNS_TYPE_AFSDB	0x0012 // A record type containing AFS database location [RFC1183].
#define DNS_TYPE_X25	0x0013 // An X25 PSDN address record type [RFC1183].
#define DNS_TYPE_ISDN	0x0014 // An ISDN address record type [RFC1183].
#define DNS_TYPE_RT		0x0015 // A route through record type [RFC1183].
#define DNS_TYPE_SIG	0x0018 // A cryptographic public key signature record type [RFC2931].
#define DNS_TYPE_KEY	0x0019 // A record type containing public key used in DNSSEC [RFC2535].
#define DNS_TYPE_AAAA	0x001C // An IPv6 address record type [RFC3596].
#define DNS_TYPE_LOC	0x001D // A location information record type [RFC1876].
#define DNS_TYPE_NXT	0x001E // A next-domain record type [RFC2065].
#define DNS_TYPE_SRV	0x0021 // A server selection record type [RFC2782].
#define DNS_TYPE_ATMA	0x0022 // An Asynchronous Transfer Mode (ATM) address record type [ATMA].
#define DNS_TYPE_NAPTR	0x0023 // An NAPTR record type [RFC2915].
#define DNS_TYPE_DNAME	0x0027 // A DNAME record type [RFC2672].
#define DNS_TYPE_DS		0x002B // A DS record type [RFC4034].
#define DNS_TYPE_RRSIG	0x002E // An RRSIG record type [RFC4034].
#define DNS_TYPE_NSEC	0x002F // An NSEC record type [RFC4034].
#define DNS_TYPE_DNSKEY	0x0030 // A DNSKEY record type [RFC4034].
#define DNS_TYPE_DHCID	0x0031 // A DHCID record type [RFC4701].
#define DNS_TYPE_ALL	0x00FF // A query-only type requesting all records [RFC1035].
#define DNS_TYPE_WINS	0xFF01 // A record type containing WINS forward lookup data [MS-WINSRA].
#define DNS_TYPE_WINSR	0xFF02 // A record type containing WINS reverse lookup data [MS-WINSRA].

//A  	1  	IP-адрес
//NS 	2 	Сервер имен.
//CNAME 5 	Каноническое имя.
//SOA 	6 	Начало списка серверов. Большое число полей, определяющих часть иерархии имен, которую использует сервер.
//MB 	7 	Имя домена почтового ящика.
//WKS 	11 	well-known service - стандартная услуга.
//PTR 	12 	Запись указателя.
//HINFO 13 	Информация об ЭВМ.
//MINFO 14 	Информация о почтовом ящике или списке почтовых адресов.
//MX 	15 	Запись о почтовом сервере.
//TXT 	16 	Не интерпретируемая строка ASCII символов.
//AXFR 	252	Запрос зонного обмена
//ANY 	255	Запрос всех записей.


struct _mdnstable{ int val; const char*key; };
_mdnstable mdnstable[]={
DNS_TYPE_ZERO,	"ZERO",
DNS_TYPE_A,		"A",
DNS_TYPE_NS,	"NS",
DNS_TYPE_MD,	"MD",
DNS_TYPE_MF,	"MF",
DNS_TYPE_CNAME,	"CNAME",
DNS_TYPE_SOA,	"SOA",
DNS_TYPE_MB,	"MB",
DNS_TYPE_MG,	"MG",
DNS_TYPE_MR,	"MR",
DNS_TYPE_NULL,	"NULL",
DNS_TYPE_WKS,	"WKS",
DNS_TYPE_PTR,	"PTR",
DNS_TYPE_HINFO,	"HIMFO",
DNS_TYPE_MINFO,	"MINFO",
DNS_TYPE_MX,	"MX",
DNS_TYPE_TXT,	"TXT",
DNS_TYPE_RP,	"RP",
DNS_TYPE_AFSDB,	"AFSDB",
DNS_TYPE_X25,	"X25",
DNS_TYPE_ISDN,	"ISDN",
DNS_TYPE_RT,	"RT",
DNS_TYPE_SIG,	"SIG",
DNS_TYPE_KEY,	"KEY",
DNS_TYPE_AAAA,	"AAAA",
DNS_TYPE_LOC,	"LOC",
DNS_TYPE_NXT,	"NXT",
DNS_TYPE_SRV,	"SRV",
DNS_TYPE_ATMA,	"ATMA",
DNS_TYPE_NAPTR,	"NAPTR",
DNS_TYPE_DNAME,	"DNAME",
DNS_TYPE_DS,	"DS",
DNS_TYPE_RRSIG,	"RRSIG",
DNS_TYPE_NSEC,	"NSEC",
DNS_TYPE_DNSKEY,"DNSKEY",
DNS_TYPE_DHCID,	"DHCID",
DNS_TYPE_ALL,	"ALL",
DNS_TYPE_WINS,	"WINS",
DNS_TYPE_WINSR,	"WINSR"
};


/*
#define DNS_TYPE_A		1	// IP
#define DNS_TYPE_NS		2
#define DNS_TYPE_MD		3
#define DNS_TYPE_MF		4
#define DNS_TYPE_CNAME	5
#define DNS_TYPE_SOA	6
#define DNS_TYPE_MB		7
#define DNS_TYPE_MG		8
#define DNS_TYPE_MR		9
#define DNS_TYPE_NULL	10
#define DNS_TYPE_WKS	11
#define DNS_TYPE_PTR	12
#define DNS_TYPE_HINFO	13
#define DNS_TYPE_MINFO	14
#define DNS_TYPE_MX		15
#define DNS_TYPE_TEXT	16
#define DNS_TYPE_AXFR	252
#define DNS_TYPE_MAILB	253
#define DNS_TYPE_MAILA	254
#define DNS_TYPE_ALL	255
*/
//Класс запроса (query class) обычно равен 1, что указывает на адреса Internet.
#define DNS_CLASS_IN	1
#define DNS_CLASS_CS	2
#define DNS_CLASS_CH	3
#define DNS_CLASS_HS	4
#define DNS_CLASS_ANY	255


struct DNS_HEADER{
	unsigned id:16;
	unsigned rd:1;		// 1-битовое поле, которое означает "требуется рекурсия" (recursion desired). Бит может быть установлен в запросе и затем возвращен в отклике. Этот флаг требует от DNS сервера обработать этот запрос самому (т.е. сервер должен сам определить требуемый IP адрес, а не возвращать адрес другого DNS сервера), что называется рекурсивным запросом (recursive query). Если этот бит не установлен и запрашиваемый сервер DNS не имеет авторитетного ответа, запрашиваемый сервер возвратит список других серверов DNS, к которым необходимо обратиться, чтобы получить ответ. 
	unsigned tc:1;		// 1-битовое поле, которое означает "обрезано" (truncated). В случае UDP это означает, что полный размер отклика превысил 512 байт, однако были возвращены только первые 512 байт отклика.
	unsigned aa:1;		// 1-битовый флаг, который означает "авторитетный ответ" (authoritative answer). Сервер DNS имеет полномочия для этого домена в разделе вопросов. 
	unsigned opcode:4;	// Обычное значение 0 (стандартный запрос). Другие значения - это 1 (инверсный запрос) и 2 (запрос статуса сервера). 
	unsigned qr:1;		// querty 0, response 1
	unsigned rcode:4;	// rcode это 4-битовое поле кода возврата. Обычные значения: 0 (нет ошибок) и 3 (ошибка имени). Ошибка имени возвращается только от полномочного сервера DNS и означает, что имя домена, указанного в запросе, не существует.
	unsigned z:3;		// Это 3-битовое поле должно быть равно 0. 
	unsigned ra:1;		// 1-битовое поле, которое означает "рекурсия возможна" (recursion available). Этот бит устанавливается в 1 в отклике, если сервер поддерживает рекурсию. Мы увидим в наших примерах, что большинство серверов DNS поддерживают рекурсию, за исключением нескольких корневых серверов (коневые сервера не в состоянии обрабатывать рекурсивные запросы из-за своей загруженности). 
	unsigned qdcount:16;
	unsigned ancount:16;
	unsigned nscount:16;
	unsigned arcount:16;
// Следующие четыре 16-битных поля указывают на количество пунктов в четырех полях переменной длины, которые завершают запись. В запросе количество вопросов (number of questions) обычно равно 1, а остальные три счетчика равны 0. В отклике количество ответов (number of answers) по меньшей мере равно 1, а оставшиеся два счетчика могут быть как нулевыми, так и ненулевыми.
};

struct DNS_PKT{
	 DNS_HEADER head;
	 char data[2048];
};


struct DNS_A_DATA{
	in_addr addr;
};

struct DNS_PTR_DATA{
	char name[256];
};

struct DNS_CNAME_DATA{
	char name[256];
};

struct DNS_MX_DATA{
	unsigned short preference;
	char name[256-2];
};

struct DNS_NS_DATA{
	char name[256];
};

struct DNS_TXT_DATA{
	char text[256];
};

/*
typedef struct {
  DWORD IP6Dword[4];
} IP6_ADDRESS, *PIP6_ADDRESS;*/

typedef struct {
  IP6_ADDRESS Ip6Address;
} DNS_AAAA_DATA, *PDNS_AAAA_DATA;


union DNS_DATA{
	DNS_A_DATA a;
	DNS_AAAA_DATA aaaa;
	DNS_NS_DATA ns;
	DNS_MX_DATA mx;
	DNS_CNAME_DATA cname;
	DNS_PTR_DATA ptr;
	DNS_TXT_DATA txt;
};

struct DNS_SECTION{
	char name[256];
	unsigned short type;
	unsigned short cl;
	unsigned int ttl;
	unsigned short datalen;
	DNS_DATA data;
};
//int dns_query(char* server, int type, char *addr,DNS_SECTION *sections);


//struct MDNS

struct MDNST_DATA{
	unsigned int tm;
	unsigned short sz, type, ttl;
	unsigned short all:1; // *.
};

struct MDNST_A{ unsigned int ip; };
struct MDNST_AAAA{ IP6_ADDRESS ip; };
typedef struct { unsigned short n; unsigned char d[0]; } MDNST_CNAME, MDNST_TXT, MDNST_NS; //sizeof + n;
typedef struct { unsigned short p, n; unsigned char d[0]; } MDNST_MX; // sizeof + n

struct MDNST_SOA{
	int serial, refresh, retry, expire, minimum;
	unsigned short n, v; unsigned char d[0];
};

struct DNS_HDATA{ // head data
	unsigned short type;
	unsigned short cl;
	unsigned int ttl;
	unsigned short sz;
	unsigned char data[0];
};

#define DNSHDATASZ	10

int dns_copy_name_full(char * p, char * name, char * dst)
{
		char *s=name;
		int i=0,k=0;
		while(*name)
		{
			if((*name & 0xC0)==0xC0)
			{
				dns_copy_name_full(p,p+(htons(*((unsigned short*)name)) & ~0xC000),dst);
				return k+2;
			}
			else
			{
				i=*name++;
				k+=i+1;
				while(i--) *dst++=*name++;
				*dst++ = '.';
			}
		}
		*(dst-1) = 0;
		return k;
}


int dns_query(char* server, int type, char *addr, DNS_SECTION *sections, int msxwait=30){
	DNS_PKT pkt;
	memset((void *)&pkt,0,sizeof(pkt));
	pkt.head.id = 0xdead; //ID
	pkt.head.rd = 1;	 //Recurse query
	pkt.head.qdcount = htons(1);

	char dns_format[128];
	memset((void *)dns_format,0,128);

	int n=0,i=1;
	while(*addr)
	{
		if(*addr == '.') n = i;
		else
		{
			dns_format[n]++;
			dns_format[i]=*addr;
		}
		addr++;
		i++;
	}
	//dns_format[i]=0;

	int len = strlen(dns_format)+1;
	strcpy(pkt.data,dns_format);
	*((unsigned short*)(pkt.data+len))=htons(type);
	*((unsigned short*)(pkt.data+len+2))=htons(1); //Internet class


	int s=ConnectPortEx(GetIP(server), 53, SOCK_DGRAM, IPPROTO_UDP); //= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	/*sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(server);
	sin.sin_port = htons(53);
	if(connect(s,(sockaddr *)&sin,sizeof(sin))==SOCKET_ERROR) return -1;*/
	if(!s) return -1;
	if(send(s,(char*)&pkt,sizeof(DNS_HEADER)+len+4,0)==SOCKET_ERROR) return -1;
	// msxwait
	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(s, &rfds);
	tm.tv_sec=msxwait; tm.tv_usec=0;
	if(select(s+1, &rfds, 0, 0, &tm)<=0) return 0;

	int bytes_recv = recv(s,(char*)&pkt,sizeof(DNS_PKT),0);
	if(bytes_recv==SOCKET_ERROR) return -1;
	closesocket(s);

	if (pkt.head.rcode) return 0;
	if(!sections) return htons(pkt.head.ancount)+htons(pkt.head.arcount)+htons(pkt.head.nscount);
	int nsections = 0;
	char *p = (char*)&pkt,*section = p+sizeof(DNS_HEADER)+len+4;
	while( ((int)(section-(char*)&pkt))<bytes_recv)
	{
		//Copy name to structure
		section+=dns_copy_name_full(p,section,sections[nsections].name);
		//Copy params to structure
		sections[nsections].type = htons(*((unsigned short*)section));
		sections[nsections].cl = htons(*((unsigned short*)(section+2)));
		sections[nsections].ttl = htonl(*((unsigned int*)(section+4)));
		sections[nsections].datalen = htons(*((unsigned short*)(section+8)));
		section+=10;

		//Copy data to structure
		switch(sections[nsections].type)
		{
		case DNS_TYPE_A: sections[nsections].data.a.addr.s_addr = *((unsigned int*)section); //A data
				break;
		case DNS_TYPE_NS: 
		case DNS_TYPE_MB:
		case DNS_TYPE_MD:
		case DNS_TYPE_MF:
		case DNS_TYPE_MG:
		case DNS_TYPE_MR:
		case DNS_TYPE_PTR:
				dns_copy_name_full(p,section,sections[nsections].data.ns.name); //MB, MD, MF, MG, MR, NS,PTR,CNAME data
				break;
		case DNS_TYPE_MX:
				sections[nsections].data.mx.preference = htons(*((unsigned short*)section));
				dns_copy_name_full(p,section+2,sections[nsections].data.mx.name);//MX data
				break;
		case DNS_TYPE_TXT:
				strncpy(sections[nsections].data.txt.text,section+1,sections[nsections].datalen-1);
				sections[nsections].data.txt.text[sections[nsections].datalen-1]=0;
				break;
		}

		section+=sections[nsections].datalen;
		nsections++;
	}

	return nsections;
}

// Domain DNS functions

bool isdomain(VString line){
	if(line=="@" || line=="*") return 1;
	char*ln=line, *to=ln+line.size();
	if(to-ln>=2){ if(*ln=='*' && *(ln+1)=='.') ln+=2; }
	while(ln<to){
		if(!(*ln>='0' && *ln<='9') && !(*ln=='.') && !(*ln=='-') && !(*ln=='_') && !(*ln>='a' && *ln<='z')) return 0;
		ln++;
	}
return 1;
}

bool isdomainlow(VString line){
	if(line=="@" || line=="*") return 1;
	char*ln=line, *to=ln+line.size();
	if(to-ln>=2){ if(*ln=='*' && *(ln+1)=='.') ln+=2; }
	while(ln<to){
		if(*ln>='A' && *ln<='Z'){ *ln-='A'-'a'; }
		if(!(*ln>='0' && *ln<='9') && !(*ln=='.') && !(*ln=='-') && !(*ln=='_') && !(*ln>='a' && *ln<='z')) return 0;
		ln++;
	}
return 1;
}

void mdns_addp(VString line){ // 0, data.
	if(!line) return ;
	unsigned char *ln=line, *lln=ln, *to=line.endu(); ln++;
	for(ln; ln<to; ln++){
		if(*ln=='.' || *ln==0){
			*lln=ln-lln-1; lln=ln;
		}
	}
}

int dnsstoi(VString line){
	for(int i=0; i<sizeof(mdnstable)/sizeof(_mdnstable); i++){
		if(compareu(line, line, (char*)mdnstable[i].key, strlen(mdnstable[i].key))) return mdnstable[i].val;
	}
	return 0;
}
VString dnsitos(int t){
	for(int i=0; i<sizeof(mdnstable)/sizeof(_mdnstable); i++){
		if(t==mdnstable[i].val) return mdnstable[i].key;
	}
	return VString();
}


//#pragma (push, 1)
class DnsData{ public:
DnsData *_n;
unsigned int ttl;
unsigned short type;
unsigned short pri;
int n, v, _soa;
unsigned char d[1];
// if soa + 20.

VString key(){ return VString(d, n); }
VString val(){ return VString(&d[n], v); }
VString soa(){ return VString(&d[n+v], _soa); }

unsigned int s_ser(){ return *(unsigned int*)&d[n+v+_soa]; }
unsigned int s_ref(){ return *(unsigned int*)&d[n+v+_soa+4]; }
unsigned int s_ret(){ return *(unsigned int*)&d[n+v+_soa+8]; }
unsigned int s_exp(){ return *(unsigned int*)&d[n+v+_soa+12]; }
unsigned int s_min(){ return *(unsigned int*)&d[n+v+_soa+16]; }

unsigned int ip(){ if(type==DNS_TYPE_A && v==4){ return *(unsigned int*)&d[n]; } return 0; }
IP6_ADDRESS ip6(){ if(type==DNS_TYPE_AAAA && v==16){ return *(IP6_ADDRESS*)&d[n]; } return IP6_ADDRESS(); }

};
//#pragma (pop)

class DnsQuest{
	unsigned char data[S8K]; int ds;
	DnsData *ddata;
	int rsz;
	HLString *ls;
public:
    DNS_HEADER hdr;
	VString name;
	int type, ic;
	int rcvsz; int rtm; int error;
	unsigned int cip; unsigned short cport;

	DnsQuest(){ rsz=0; ddata=0; ds=0; ls=0; }
	DnsData* GetData(){ return ddata; }


	int QuestGen(DNS_PKT &pkt, VString domain, int type){
		unsigned char *buf=(unsigned char*)pkt.data; memset(&pkt, 0, sizeof(pkt));
		pkt.head.id = rand()%0xdead;	//ID
		pkt.head.rd = 1;		//Recurse query
		pkt.head.qdcount = htons(1);
		memcpy(buf+1, domain, domain);
		mdns_addp(VString(buf, domain.sz+2));
		buf+=domain.sz+2;
		*((unsigned short*)(buf))=htons(type); buf+=2;
		*((unsigned short*)(buf))=htons(1); buf+=2;

		rtm=0; rcvsz=0; error=1;
		cip=0; cport=0;
		return (char*)buf-(char*)&pkt;
	}


	int Quest(VString domain, int type, unsigned int ip, unsigned short port, int maxwait=10){
		if(domain.sz>256) return 0;

		DNS_PKT pkt; rtm=0; rcvsz=0; error=1;
		int pktsz=QuestGen(pkt, domain, type);
		
		int sock=ConnectPortEx(ip, port, SOCK_DGRAM, IPPROTO_UDP); //= socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(!sock){ return -1; }
		timeb tmb=alltime();
		if(send(sock, (char*)&pkt, pktsz, 0)==SOCKET_ERROR) return -1;

		timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
		tm.tv_sec=maxwait; tm.tv_usec=0;
		if(select(sock+1, &rfds, 0, 0, &tm)<=0) return 0;

		rcvsz = recv(sock, (char*)&pkt, sizeof(DNS_PKT), 0);
		rtm=timemit(tmb);
		if(rcvsz==SOCKET_ERROR){ rcvsz=0; return -1; }
		closesocket(sock);
		//if(pkt.head.rcode) return 0;

		pkt.head.qdcount=htons(pkt.head.qdcount);
		pkt.head.ancount=htons(pkt.head.ancount);
		pkt.head.nscount=htons(pkt.head.nscount);
		pkt.head.arcount=htons(pkt.head.arcount);

		return Read((unsigned char*)&pkt, (unsigned char*)&pkt+rcvsz);
	}


	int cpyname(unsigned char*&rln, unsigned char *to, unsigned char *d, unsigned char *p){
		unsigned char *bd=d, *lt, *ln=rln; rln=0;// print("l::.");
		while(ln<to){
//			print("l:", itos(*ln), ".");
			if(ln<p || ln>=to){ rln=0; return 0; }
			if(!*ln){ ln++; if(!rln) rln=ln; if(d>bd) d--; *d=0; return d-bd; }
			if(*ln==0xC0){
				if(!rln) rln=ln+2;
				ln=p+(htons(*((unsigned short*)ln)) & ~0xC000);
				continue;
			}
			lt=ln+*ln+1; ln++;
			while(ln<lt) *d++=*ln++; *d++='.';
		}
		return 0;
	}

	int Read(unsigned char*line, unsigned char *to){
		unsigned char *ln=line; memset(data, 0, sizeof(data)); name.Clean();
		hdr=*(DNS_HEADER*)ln; ln+=sizeof(DNS_HEADER); rsz=0;
		unsigned char*l=ln, *ll=l;//, *lt, *nm;

		 // nm=name.data;
		name.sz=cpyname(l, to, &data[ds], line); if(!l) return 0;
		name.data=&data[ds]; ds+=name;
		/*
		while(l<to){
			if(!*l){ l++; nm--; *nm=0; break; } 
			lt=l+*l+1; l++;
			while(l<lt) *nm++=*l++; *nm++='.';
		}
		name.sz=nm-name.data; */
		
		if(l+4>to) return 0;
		type=htons(*(unsigned short*)l); l+=2;
		ic=htons(*(unsigned short*)l); l+=2;

		// Analys
		//l=ln;
		while(l<to && ds<sizeof(data)-256){
			DnsData*d=(DnsData*)&data[ds], *dd=ddata; ds+=sizeof(DnsData);
			if(!ddata) ddata=d; while(dd){ if(!dd->_n){ dd->_n=d; break; } dd=dd->_n; }
            
			// domain
			d->n=cpyname(l, to, d->d, line); if(!l){ dd->_n=0; break; }
			ds+=d->n;
//print("n:", VString(&data[ds], d->n), "; ");

			//while(l<to){
			//	if(!*l){ l++; nm--; break; } 
			//	if(*l==0xC0 && *(l+1)==0xC && l+1<to){ memcpy(nm, name, name); nm+=name; l+=2; break; }
			//	memcpy(nm, l+1, *l); nm+=*l; *nm++='.'; l+=*l+1;
			//} ; 
            
			//header
			DNS_HDATA *hd=(DNS_HDATA*)l; l+=DNSHDATASZ; //sizeof(MDNST_HEADER);
			hd->cl=htons(hd->cl); hd->type=htons(hd->type);
			hd->ttl=htonl(hd->ttl); hd->sz=htons(hd->sz);
			
			d->type=hd->type; d->ttl=hd->ttl;
			ll=l+hd->sz;// nm=&data[ds]; // jump

			switch(hd->type){
				case DNS_TYPE_A:
					if(hd->sz!=4) break;
					d->v=4; *(unsigned int*)&d->d[d->n]=htonl(*(unsigned int*)l); ds+=4;
//print("ip:", ipitos(*(unsigned int*)&data[ds]), "; ");
 
					break;
				case DNS_TYPE_AAAA:
					if(hd->sz!=16) break;
					d->v=16; ds+=16; // memcpy(&d->d[d->n], l, 16); ds+=16;
					for(int i = 0; i < 8; i++){
						d->d[d->n + i * 2] = *(l + i * 2 + 1);
						d->d[d->n + i * 2 + 1] = *(l + i * 2);
					}
					break;
				case DNS_TYPE_MX:
					d->pri=htons(*((unsigned short*)l)); l+=2;
				case DNS_TYPE_NS: 
				case DNS_TYPE_MD:
				case DNS_TYPE_MF:
				case DNS_TYPE_CNAME:
				case DNS_TYPE_MB:
				case DNS_TYPE_MG:
				case DNS_TYPE_MR:
				case DNS_TYPE_PTR:
					d->v=cpyname(l, to, &d->d[d->n], line); ds+=d->v; if(!l){ dd->_n=0; break; }
					break;
				case DNS_TYPE_TXT:
					memcpy(&d->d[d->n], l+1, *l); d->v=*l; ds+=*l;
					break;
				case DNS_TYPE_SOA:
					int p = 0;
					p = d->n;
					d->v=cpyname(l, to, &d->d[p], line); ds+=d->v; if(!l){ dd->_n=0; break; } p += d->v;
					d->_soa=cpyname(l, to, &d->d[p], line); ds+=d->_soa; if(!l){ dd->_n=0; break; } p += d->_soa;			
					*(unsigned int*)&d->d[p]=htonl(*(unsigned int*)l); l+=4; ds+=4; p+=4;
					*(unsigned int*)&d->d[p]=htonl(*(unsigned int*)l); l+=4; ds+=4; p+=4;
					*(unsigned int*)&d->d[p]=htonl(*(unsigned int*)l); l+=4; ds+=4; p+=4;
					*(unsigned int*)&d->d[p]=htonl(*(unsigned int*)l); l+=4; ds+=4; p+=4;
					*(unsigned int*)&d->d[p]=htonl(*(unsigned int*)l); l+=4; ds+=4; p+=4;
					break;
			}
			l=ll; if(l>to) break; rsz++;
		}

		if(l==to) error=0;
		return rsz;
	}


	void SelLS(HLString *l){ ls=l; }

	void print(const VString &l1, const VString &l2=VString(), const VString &l3=VString(), const VString &l4=VString(), const VString &l5=VString(), const VString &l6=VString(),
		const VString &l7=VString(), const VString &l8=VString(), const VString &l9=VString(), const VString &l10=VString()){
        if(!ls) return ;
		if(l1) *ls+l1; if(l2) *ls+l2; if(l3) *ls+l3; if(l4) *ls+l4; if(l5) *ls+l5;
		if(l6) *ls+l6; if(l7) *ls+l7; if(l8) *ls+l8; if(l9) *ls+l9; if(l10) *ls+l10;
	}

	void Print(HLString &ls){
		SelLS(&ls); Itos it;

		print(";; ->>HEADER<<- opcode: ", !hdr.opcode ? "QUERY" : "???" );
		print(" status: ", hdr.rcode ? "ERROR" : "NOERROR");
		print(", id: ", it.itos(hdr.id), "\r\n");
		print(";; flags:", hdr.qr ? " qr" : "", hdr.rd ? " rd" : "", hdr.ra ? " ra" : "",
		hdr.tc ? " tc" : "", hdr.aa ? " aa" : "");
		print("; QUERY: ", itos(hdr.qdcount),", ANSWER: ", itos(hdr.ancount),
			", AUTHORITY: ", itos(hdr.nscount),", ADDITIONAL: ", itos(hdr.arcount),"\r\n");
		//;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 51336
		//;; flags: qr rd ra; QUERY: 1, ANSWER: 12, AUTHORITY: 0, ADDITIONAL: 8


		print("\r\n;; QUESTION SECTION:\r\n;");
		print(name, ".\t\t\tIN\t", dnsitos(type), "\r\n\r\n");
		//;yandex.ru.                     IN      ANY

		print(";; ANSWER SECTION:\r\n");
		DnsData *d=GetData(); int as=hdr.ancount+hdr.nscount;
		while(d){
			if(as==0) print("\r\n;; ADDITIONAL SECTION:\r\n"); as--;

			print(d->key(), ".\t\t", it.itos(d->ttl), "\tIN\t");
			print(dnsitos(d->type), "\t");
			switch(d->type){
				case DNS_TYPE_A: print(it.ipitos(d->ip())); break;
				case DNS_TYPE_AAAA: print(ip6itos(d->ip6())); break;
				case DNS_TYPE_MX: print(it.itos(d->pri), " ");
				case DNS_TYPE_NS:
				case DNS_TYPE_MD:
				case DNS_TYPE_MF:
				case DNS_TYPE_CNAME:
				case DNS_TYPE_MB:
				case DNS_TYPE_MG:
				case DNS_TYPE_MR:
				case DNS_TYPE_PTR:
					print(d->val()); break;
				case DNS_TYPE_TXT:
					print("'", d->val(), "'"); break;
				case DNS_TYPE_SOA:
					print(d->val(), " ", d->soa(), " ");
					print(it.itos(d->s_ser()), " ");
					print(it.itos(d->s_ref()), " ");
					print(it.itos(d->s_ret()), " ");
					print(it.itos(d->s_exp()), " ");
					print(it.itos(d->s_min()));
					break;
				default : print("<ERROR I Can't read this format.>");
			}


			print("\r\n");
			d=d->_n;
		}

		if(error) print("\r\nProgram don't normal closed. Error found! DNS data corrypt? ");
		print("\r\n", "Received ", it.itos(rcvsz));
		print(" bytes from ", it.ipitos(cip));
		print(":", it.itos(cport));
		print(" in ", it.itos(rtm), " ms\r\n");

		SelLS(0);
		return ;
	}




};
