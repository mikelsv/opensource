#define NEMU_PROTO_IP 0x0800
#define NEMU_PROTO_IP6 0x08DD
#define NEMU_PROTO_ARP 0x0806

#define MAX_PACKET_SIZE    0x10000
#define SIO_RCVALL         0x98000001
#include <stdlib.h>

#ifdef WIN32
#ifdef __GNUC__

typedef WINSOCK_API_LINKAGE INT (__stdcall *LPGETADDRINFO)(PCSTR pNodeName, PCSTR pServiceName, const addrinfo * pHints, addrinfo **ppResult);
typedef WINSOCK_API_LINKAGE INT (__stdcall *LPGETNAMEINFO)(const SOCKADDR * pSockaddr, socklen_t SockaddrLength, PCHAR pNodeBuffer, DWORD NodeBufferSize, PCHAR pServiceBuffer, DWORD ServiceBufferSize, INT Flags);
typedef WINSOCK_API_LINKAGE VOID (__stdcall *LPFREEADDRINFO)(addrinfo *pAddrInfo);

LPGETADDRINFO _getaddrinfo=0;
LPGETNAMEINFO _getnameinfo=0;
LPFREEADDRINFO _freeaddrinfo=0;

class WIN_MINGW_LOAD_GETADDRINFO{

public:
	WIN_MINGW_LOAD_GETADDRINFO(){
		HMODULE lib=LoadLibraryW(L"Ws2_32.dll");
		if(lib){
			_getaddrinfo = (LPGETADDRINFO)GetProcAddress(lib, "getaddrinfo");
			_getnameinfo = (LPGETNAMEINFO)GetProcAddress(lib, "getnameinfo");
			_freeaddrinfo = (LPFREEADDRINFO)GetProcAddress(lib, "freeaddrinfo");
		} // else: you have a biig surprise :)
	}
}WIN_MINGW_LOAD_GETADDRINFO;

#define getaddrinfo _getaddrinfo
#define getnameinfo _getnameinfo
#define freeaddrinfo _freeaddrinfo

#endif
#endif

#ifndef WIN32
	#include "net/if_arp.h"
#endif

//#pragma pack(4)

//—труктура заголовка IP-пакета
/*typedef struct IPHeader {
  UCHAR   iph_verlen;   // верси€ и длина заголовка
  UCHAR   iph_tos;      // тип сервиса
  USHORT  iph_length;   // длина всего пакета
  USHORT  iph_id;       // »дентификаци€
  USHORT  iph_offset;   // флаги и смещени€
  UCHAR   iph_ttl;      // врем€ жизни пакета
  UCHAR   iph_protocol; // протокол
  USHORT  iph_xsum;     // контрольна€ сумма
  ULONG   iph_src;      // IP-адрес отправител€
  ULONG   iph_dest;     // IP-адрес назначени€
} IPHeader;*/

#ifdef WIN32

struct iphdr{
	unsigned int ihl:4;			// длина шапки
    unsigned int version:4;		// верси€ и длина заголовка
	unsigned int tos:8;			// тип сервиса
	unsigned int tot_len:16;	// длина всего пакета
	unsigned int id:16;			// »дентификаци€
	unsigned int frag_off:16;	// флаги и смещени€
	unsigned int ttl:8;			// врем€ жизни пакета
	unsigned int protocol:8;	// протокол
	unsigned int check:16;		// контрольна€ сумма
	unsigned int saddr;			// IP-адрес отправител€
    unsigned int daddr;			// IP-адрес назначени€
    // The options start here.
  };
//#else
//#include "netinet/ip.h"
#endif


struct in6addr {
    union {
        UCHAR       Byte[16];
        USHORT      Word[8];
    } u;
};

//#define in_addr6 in6_addr


//#ifdef WIN32
#ifndef USEMSV_ANDROID
struct ipv6hdr {
        u8	version:4;
        u8	priority:4;
        u8	flow_lbl[3];
        u16	payload_len;
        u8	nexthdr;
        u8	hop_limit;

        struct  in6addr        saddr;
        struct  in6addr        daddr;
};
//#endif
#endif


//#define __u16  unsigned short 
//#define __u32  unsigned int 
#ifdef WIN32
#define __LITTLE_ENDIAN_BITFIELD
#endif

#ifndef __BIG_ENDIAN_BITFIELD
#define __LITTLE_ENDIAN_BITFIELD
#endif

struct tcphdr{
    u16   source;		// порт источника
    u16   dest;			// порт назначени€
    u32   seq;			// начальный пор€дковый номер
    u32   ack_seq;		// номер подтверждени€
#if defined(__LITTLE_ENDIAN_BITFIELD)
    u16   res1:4,
        doff:4,			// длина заголовка
        fin:1,
        syn:1,			// запрос соединени€ 
        rst:1,
        psh:1,
        ack:1,			// сообщение подтверждени€ запроса на соединение
        urg:1,
        ece:1,
        cwr:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
    u16   doff:4,
        res1:4,
        cwr:1,
        ece:1,
        urg:1,
        ack:1,
        psh:1,
        rst:1,
        syn:1,
        fin:1;
#else
#error  "Adjust your <asm/byteorder.h> defines"
#endif  
    u16   window;			// сумма байтов посылаемых прежде ACK
    u16   check;			// контрольна€ сумма
    u16   urg_ptr;
};

struct udphdr {
    u16   source;		// порт источника
    u16   dest;			// порт назначени€
	u16   size;			// размер пакета
    u16   crc;			// контрольна€ сумма
};

struct icmphdr{
	unsigned char type, code; unsigned short crc;
	unsigned short id, seq; unsigned char data[0];
};

struct icmphdr_{
	unsigned char type, code; unsigned short crc;
	unsigned char data[0];
};

struct machdr{
	unsigned char src[6], dest[6]; // mac source, mac dest;
	unsigned short ep; // ethernet protocol
};


#ifdef WIN32
/* ARP protocol opcodes. */
#define ARPOP_REQUEST   1               /* ARP request.  */
#define ARPOP_REPLY     2               /* ARP reply.  */
#define ARPOP_RREQUEST  3               /* RARP request.  */
#define ARPOP_RREPLY    4               /* RARP reply.  */
#define ARPOP_InREQUEST 8               /* InARP request.  */
#define ARPOP_InREPLY   9               /* InARP reply.  */
#define ARPOP_NAK       10              /* (ATM)ARP NAK.  */


struct arphdr{
    unsigned short int ar_hrd;          /* Format of hardware address.  */
    unsigned short int ar_pro;          /* Format of protocol address.  */
    unsigned char ar_hln;               /* Length of hardware address.  */
    unsigned char ar_pln;               /* Length of protocol address.  */
    unsigned short int ar_op;           /* ARP opcode (command).  */
#if 0
    /* Ethernet looks like this : This bit is variable sized
       however...  */
    unsigned char __ar_sha[ETH_ALEN];   /* Sender hardware address.  */
    unsigned char __ar_sip[4];          /* Sender IP address.  */
    unsigned char __ar_tha[ETH_ALEN];   /* Target hardware address.  */
    unsigned char __ar_tip[4];          /* Target IP address.  */
#endif
  };

#endif

#pragma pack(push, 1)
struct arphdr_request{
	unsigned char ar_sha[6];   /* Sender hardware address.  */
    unsigned int ar_sip;   /* Sender IP address.  */
    unsigned char ar_tha[6];   /* Target hardware address.  */
    unsigned int ar_tip;   /* Target IP address.  */
};
#pragma pack(pop)

// if
bool ifrecv(SOCKET sock){
	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
	tm.tv_sec=0; tm.tv_usec=1000;
	return select(sock+1, &rfds, 0, 0, &tm)!=0;
}

bool ifsend(SOCKET sock){
	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
	tm.tv_sec=0; tm.tv_usec=0;
	return select(sock+1, 0, &rfds, 0, &tm)!=0;
}

// Network convert
MString macitos(MACADDR_S mac){
	MString ret; ret.Reserv(17, ':');
	char*l=ret; unsigned char c; 

	for(int i=0; i<6; i++){
		c=mac.mac[i]/16; *l=((c<10) ? c+48 : c+55); l++;
		c=mac.mac[i]%16; *l=((c<10) ? c+48 : c+55); l++;
		l++;
	}

	return ret;
}

MACADDR_S macstoi(VString line){
	MACADDR mac; if(line.size()!=17){ memset(mac, 0, 6); return *(MACADDR_S*)&mac; }
	unsigned char*l=line;
	unsigned char c;

	for(int i=0; i<6; i++){
		c=*l; l++; c=c<='9' ? c-=48 : c-=55; mac[i]=c<<4;
		c=*l; l++; c=c<='9' ? c-=48 : c-=55; mac[i]|=c;
		l++;
	}

	return *(MACADDR_S*)&mac;
}

u16 crc16(u16 *ptr, int nbytes){
    register u32 sum;
    u16 oddbyte;
    register u16 answer;
    sum = 0;
    while( nbytes > 1 ){
		sum += *ptr ++;
		nbytes -= 2;
    }

	if ( nbytes == 1 ){
		oddbyte = 0;
		* (( unsigned char *) &oddbyte ) = * (unsigned char *) ptr;
		sum += oddbyte;
	}

	sum = ( sum >> 16 ) + ( sum & 0xFFFF);
    sum += (sum >> 16 );
    answer=~sum;
    return (answer);
}

unsigned int crc32(unsigned char *buf, unsigned long len){
	unsigned long crc_table[256];
	unsigned long crc;
	for (int i = 0; i < 256; i++){
		crc = i;
		for (int j = 0; j < 8; j++)
		crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
		crc_table[i] = crc;
	};
	crc = 0xFFFFFFFFUL;
	while (len--)
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	return crc ^ 0xFFFFFFFFUL;
}

// Traffic Strea
template<int SZ>
class TraffixS{
	int data[SZ];
	int tm; int64 asz;

public:
	TraffixS(){ memset(data, 0, sizeof(data)); tm=0; asz=0; }
	void Add(int t, int v){ if(v<0) return ;
		if(tm+SZ<t || tm>t){ memset(data, 0, sizeof(data)); tm=t; }
		if(tm+SZ==t){ memcpy(data, data+1, sizeof(data)-4); data[SZ-1]=0; tm++; }
		data[t-tm]+=v; asz+=v;
	}

	int C(){
		int a=0;
		for(int i=0; i<SZ; i++){
			a+=data[i];
		}
		return a/SZ;	
	}

	int C(int t){ t--;
		if(tm+SZ<t || tm>t){ memset(data, 0, sizeof(data)); tm=t; }
		if(tm+SZ==t){ memcpy(data, data+1, sizeof(data)-4); data[SZ-1]=0; tm++; }
		return C();	
	}

	int64 A(){ return asz; }

};

//#pragma pack(pop)