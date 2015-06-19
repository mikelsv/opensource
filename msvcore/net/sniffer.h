//#include "stdlib.h"
//typedef struct in_addr IN_ADDR;

#ifndef WIN32
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_ether.h>
//#include <errno.h>
//#include <signal.h>
//#include <netinet/in.h>
#include <netinet/ip.h>
//#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#endif


#include "sniff_traff.h"

void quit(int ){
	exit(1);
}

void iphdr_decode (u8 *incoming, u32 len, struct iphdr*ip, u8 *out);
void iphdr_hexout(u8 *incoming, u32 len, u8 *out);

class sniffer{ public:
struct iphdr *ip;
unsigned int ipsz;
SOCKADDR_IN sa;
int sock;
#ifndef WIN32
struct ifreq interface;
#endif
u16	ep;

unsigned int packets;
unsigned int ltime;
int noprint, nosave, promisc;
unsigned int filter; // !htonl


u8 buf[65536], out[65536], device[1024];
int rc;

sniffer(){ ltime=time(); noprint=0; nosave=0; packets=0; promisc=1; filter=0; }
~sniffer(){ if(sock) closesocket(sock); sock=0; }

int Start(unsigned int ip=0){
#ifndef WIN32
	signal(SIGINT,quit); signal(SIGQUIT,quit); signal(SIGTERM,quit);
#endif
	//strcpy((char*)device, "eth0");

// allocating a socket    
// инициализация SOCK_RAW AF_INET SOCK_DGRAM       SOCK_PACKET
 
#ifdef WIN32
	sock=socket(AF_INET, SOCK_RAW, IPPROTO_IP);
#else	
	sock=socket(PF_PACKET,SOCK_PACKET,htons(ETH_P_ALL));
#endif	
	if(sock<0){ printf("Exiting: failed allocating a socket!\n"); return 1; }

// putting a device into non-blocking mode, if required
#ifdef WIN32
	char name[128]; // Имя хоста (компьютера).
	HOSTENT* phe;	// Информация о хосте.
//	ZeroMemory( &sa, sizeof(sa) );
	gethostname(name, sizeof(name));
	phe = gethostbyname( name );

	sa.sin_family = AF_INET;
	sa.sin_port=0;
	sa.sin_addr.s_addr = ((struct in_addr *)phe->h_addr_list[0])->s_addr;
	if(ip) sa.sin_addr.s_addr=htonl(ip);
//	sa.sin_addr.s_addr = inet_addr("89.108.75.95");
	bind(sock, (LPSOCKADDR)&sa, sizeof(sa));

	unsigned long flag=1;
	if(ioctlsocket(sock, SIO_RCVALL, &flag)<0){ printf("failed promiscious mode"); return 1; }

	int on = 1, error=0; 
	// error = setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&on, sizeof(on)); 
	if(error == SOCKET_ERROR){
	//printf("Error setsockopt(): %d", WSAGetLastError());
		printf("Error setsockopt().");
	return 0;
	}

#else
	struct ifaddrs *addr = 0, *paddr;
	if(getifaddrs(&addr) < 0){
		print("get interface address error \r\n");
		return 1;
	}

	int iploc = inet_addr("127.0.0.1");
	ip = htonl(ip);

	for(paddr = addr; paddr; paddr = paddr->ifa_next){
		if(!paddr->ifa_addr)
			continue;

		if(paddr->ifa_addr->sa_family == AF_INET){ // IP v4
			int tip = ((struct sockaddr_in *)paddr->ifa_addr)->sin_addr.s_addr;
			if(tip && ( ip && tip == ip || !ip && tip != iploc)){
				//printf("interface: %s\r\n", paddr->ifa_name);
				strcpy((char*)device, paddr->ifa_name);
				break;
			}
		}
	}

	freeifaddrs(addr);


    if(promisc){
		strcpy((char*)interface.ifr_name,(char*)device);
		if(ioctl(sock,SIOCGIFFLAGS,&interface)<0){ printf("failed reading data for %s!\n",device); return 1; }
    interface.ifr_flags |= IFF_PROMISC;
    // setting promiscious flag here
    if(ioctl(sock,SIOCSIFFLAGS,&interface)<0){ printf("failed setting data for %s!\n", device); return 1; }
	}

// binding a socket to device
    rc = setsockopt(sock,SOL_SOCKET,SO_BINDTODEVICE,device,strlen((char*)device)+1);
    if(rc == -1){ printf("Exiting: failed binding socket to %s\n",device); return 1; }
#endif
   
   // printf("Listening on %s\n",device);

	return 1;
}


  /*  if(lo){
    while(!enough)	{
	rc = recvfrom(sock,buf,sizeof(buf),0,0,0);
        if(!memcmp(buf,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",12))	{
	    printf("\npacket #%u\n",packets++);
	    decode(buf,rc,out);
	    printf("%s\n",out);
	    hexout(buf,rc,out);
	    printf("%s\n",out);
	    }
        }
	}	else	{*/

int DoOne(){
	if(!sock) return 0;
	rc = recv(sock, (char*)buf, sizeof(buf), 0);
	return rc>0;
}

int Do(){ if(sock<=0) return 0;

	while(1){
	rc = recv(sock, (char*)buf, sizeof(buf), 0); //, 0, 0);
// test	
//	for(int i=0; i<S1K; i++){ buf[i]=rand(); }
//	rc=rand()%512;// Sleep(2);
// endtest
	ep = ntohs(*(u16*)(buf + 12));
#ifndef WIN32
	ip = (struct iphdr*)(buf + 14); ipsz=rc-14;
#else
	ip = (struct iphdr*)(buf); ipsz=rc; ep=0x0800;
#endif

	if(filter) if(ip->saddr!=filter && ip->daddr!=filter) continue;
	
	if(!noprint){
		printf("\npacket #%u\n", ++packets);
		iphdr_decode(buf, rc, ip, out); printf("%s\n",out);
	}

	//if(ip->protocol==IPPROTO_ICMP){
	//	printf("\npacket #%u\n", ++packets);
	//	iphdr_decode(buf, rc, ip, out); printf("%s\n",out);
	//}

	unsigned char*b=(unsigned char*)buf+sizeof(struct iphdr); int bs=ipsz-sizeof(struct iphdr);
	int ps=0, pd=0;
	if(ip->protocol==IPPROTO_TCP && ep==0x0800){
		b+=sizeof(tcphdr); bs-=sizeof(tcphdr); ps=((tcphdr*)(ip+1))->source; pd=((tcphdr*)(ip+1))->dest; atcp+=bs;
		t_tcp.addl(ip->saddr, ip->daddr, ps, pd, bs);
		dt_tcp.addl(ip->saddr, ip->daddr, ps, pd, bs);
	}
	else if(ip->protocol==IPPROTO_UDP && ep==0x0800){
		b+=sizeof(udphdr); bs-=sizeof(udphdr); ps=((udphdr*)(ip+1))->source; pd=((udphdr*)(ip+1))->dest; audp+=bs;
		t_udp.addl(ip->saddr, ip->daddr, ps, pd, bs);
		dt_udp.addl(ip->saddr, ip->daddr, ps, pd, bs);
	}
	else{  t_oth.addl(ip->saddr, ip->daddr, ps, pd, bs); dt_oth.addl(ip->saddr, ip->daddr, ps, pd, bs); aoth+=bs; }

	//altraff.add(ip->saddr, ip->daddr, ps, pd, rc);

	if(!nosave && ltime/3600!=time()/3600){ MTime mt; mt.mtime(time());
		if(!mt.hour){ sdt_tcp-=dt_tcp; sdt_udp-=dt_udp; sdt_oth-=dt_oth; }
		//t_tcp.OneBlock(); t_tcp.OneBlock(); t_tcp.OneBlock();
		st_tcp-=t_tcp; st_udp-=t_udp; st_oth-=t_oth;
		ltime=time();
		//dt_tcp.Clear(); dt_udp.Clear(); dt_oth.Clear();
		StartThread(TraffixSaveLogs);
	}

	if(!noprint){ iphdr_hexout(b,bs,out); printf("%s\n",out); }

	return 1;
	}
  
#ifndef WIN32
    if(promisc){
		interface.ifr_flags &= ~IFF_PROMISC;
		if(ioctl(sock,SIOCSIFFLAGS,&interface)<0){ printf("failed wiping IFF_PROMISC flag!\n"); return 1; }
	}
#endif
	if(sock) closesocket(sock); sock=0;

    return 0;
}    


};



void iphdr_decode(u8 *incoming, u32 len, struct iphdr*ip, u8 *out){
	u8	*ptr = out; char *prot=0;
    int	i = 0;
	struct in_addr addr;
	u16	ep = ntohs(*(u16*)(incoming + 12));

if(incoming+12<=(u8*)ip){	
	//ip = (struct iphdr*)(incoming + 14);

    sprintf((char*)ptr,"Source MAC Address: "), ptr+=20;
    for(i=6;i<12;i++) sprintf((char*)ptr,"%02X-",incoming[i]), ptr+=3;
    ptr[-1] = '\n';
    sprintf((char*)ptr,"Destination MAC Address: "), ptr+=25;
	for(i=0;i<6;i++) sprintf((char*)ptr,"%02X-",incoming[i]), ptr+=3;
    ptr[-1] = '\n';
    sprintf((char*)ptr,"Encap: %04X ", ep), ptr+=12;

	switch(ep){
		case NEMU_PROTO_IP: prot="IP"; break;
		case NEMU_PROTO_IP6: prot="IP6"; break;
		case NEMU_PROTO_ARP: prot="ARP"; break;
		default: prot="???"; break;
	}
	sprintf((char*)ptr, prot); ptr+=strlen(prot);
	sprintf((char*)ptr," \n", ep), ptr+=2;
} else{
	if(len<sizeof(struct iphdr)){ ptr[0] = '\0'; return ; }
	ep=0;
	//ip = (struct iphdr*)(incoming);
}

// IP4
if(!ep || ep==0x0800){
    sprintf((char*)ptr,"IP Version:      %02X\n", ip->version); ptr+=20;
    sprintf((char*)ptr,"Header Length:   %02X\n", ip->ihl); ptr+=20;
    sprintf((char*)ptr,"Type of Service: %02X\n", ip->tos); ptr+=20;
    sprintf((char*)ptr,"Total Length:    %06d\n", ntohs(ip->tot_len)); ptr+=24;
   
	prot=0;
	switch(ip->protocol){
		case IPPROTO_IP: prot="IP"; break;
		case IPPROTO_ICMP: prot="ICMP"; break;
		case IPPROTO_TCP: prot="TCP"; break;
		case IPPROTO_UDP: prot="UDP"; break;
		default: prot="???"; break;
	}
	if(prot){ sprintf((char*)ptr,"Protocol:        %s\n", prot); ptr+=18+strlen(prot); }

	sprintf((char*)ptr,"Identification:  %04X\n", ntohs(ip->id)); ptr+=22;
    sprintf((char*)ptr,"Fragmentation:   %04X\n", ntohs(ip->frag_off)); ptr+=22;
    sprintf((char*)ptr,"Time To Live:    %02X\n", ip->ttl); ptr+=20;

	u16 pcrc=ip->check, tcrc; ip->check=0; tcrc=crc16((unsigned short*)ip, sizeof(struct iphdr));
	if(pcrc==tcrc){ prot="OK"; } else prot="FAIL"; ip->check=pcrc;
	sprintf((char*)ptr,"CRC-16 CheckSum: %04X, %s\n", ntohs(pcrc), prot); ptr+=24+strlen(prot);
 
	addr.s_addr = ip->saddr;
	if(ip->protocol!=IPPROTO_TCP && ip->protocol!=IPPROTO_UDP){
		sprintf((char*)ptr,"Source IP Address:      %s\n",inet_ntoa(addr)); ptr+=25+strlen(inet_ntoa(addr));
		addr.s_addr = ip->daddr;
		sprintf((char*)ptr,"Destination IP Address: %s\n",inet_ntoa(addr)); ptr+=25+strlen(inet_ntoa(addr));
	}
	else{
		sprintf((char*)ptr,"Source IP Address:      %s:%d\n",inet_ntoa(addr), htons(((udphdr*)(ip+1))->source));
		ptr+=26+strlen(inet_ntoa(addr))+dsize(htons(((udphdr*)(ip+1))->source));
		addr.s_addr = ip->daddr;
		sprintf((char*)ptr,"Destination IP Address: %s:%d\n",inet_ntoa(addr), htons(((udphdr*)(ip+1))->dest));
		ptr+=26+strlen(inet_ntoa(addr))+dsize(htons(((udphdr*)(ip+1))->dest));
	}

	if(ip->protocol==IPPROTO_ICMP){
		sprintf((char*)ptr,"ICMP Data:\n"); ptr+=11;
		icmphdr &ihr=*(icmphdr*)((char*)ip+sizeof(iphdr));
		sprintf((char*)ptr,"Type:      %02X\n", ihr.type); ptr+=14;
		sprintf((char*)ptr,"Code:      %02X\n", ihr.code); ptr+=14;
		sprintf((char*)ptr,"Id:        %04X\n", ihr.id); ptr+=16;
		sprintf((char*)ptr,"Seq:       %04X\n", ihr.seq); ptr+=16;
		//sprintf((char*)ptr,"Data:      %s\n", ihr.code); ptr+=20;

		u16 pcrc=ihr.crc, tcrc; ihr.crc=0; tcrc=crc16((unsigned short*)ip, ntohs(ip->tot_len)-sizeof(struct iphdr));
		if(pcrc==tcrc){ prot="OK"; } else prot="FAIL"; ihr.crc=pcrc;
		sprintf((char*)ptr,"CRC-16 CheckSum: %04X, %s\n", ntohs(pcrc), prot); ptr+=24+strlen(prot);

		if(ntohs(ip->tot_len)-sizeof(iphdr)-sizeof(icmphdr)>=sizeof(iphdr)+sizeof(icmphdr) && ihr.type==11){

			iphdr &ipr2=*(iphdr*)((char*)ip+sizeof(iphdr)+sizeof(icmphdr));
			icmphdr &ihr2=*(icmphdr*)((char*)ip+sizeof(iphdr)+sizeof(icmphdr)+sizeof(iphdr));

			addr.s_addr = ipr2.saddr;
			sprintf((char*)ptr,"Source IP Address:      %s\n",inet_ntoa(addr)); ptr+=25+strlen(inet_ntoa(addr));
			addr.s_addr = ipr2.daddr;
			sprintf((char*)ptr,"Destination IP Address: %s\n",inet_ntoa(addr)); ptr+=25+strlen(inet_ntoa(addr));


		
		}



	}


}// end ip4
else if(ep==0x08DD){
	ipv6hdr *ip6=(ipv6hdr*)ip;
	sprintf((char*)ptr,"IP Version:      %02d\n", ip6->version); ptr+=20;
	sprintf((char*)ptr,"Priority:        %02d\n", ip6->priority); ptr+=20;
	sprintf((char*)ptr,"Flow Label:      %02X %02X %02X\n", ip6->flow_lbl[0], ip6->flow_lbl[1], ip6->flow_lbl[2]); ptr+=26;
	sprintf((char*)ptr,"Total Length:    %04X\n", ip6->payload_len); ptr+=22;
	sprintf((char*)ptr,"Next Header:     %02X\n", ip6->nexthdr); ptr+=20;
	sprintf((char*)ptr,"Hop Limit:       %03d\n", ip6->nexthdr); ptr+=21;
	sprintf((char*)ptr,"Source IP Address: "), ptr+=19;
	for(i=0;i<8;i++) sprintf((char*)ptr,"%04x:",ip6->saddr.u.Word[i]), ptr+=5; ptr[-1]='\n';
	sprintf((char*)ptr,"Destination IP Address: "), ptr+=24;
	for(i=0;i<8;i++) sprintf((char*)ptr,"%04x:",ip6->daddr.u.Word[i]), ptr+=5; ptr[-1]='\n';
}

else if(ep==0x0806){
	arphdr *arp=(arphdr*)ip;
	sprintf((char*)ptr,"Format of hardware address:      0x%04x\n", ntohs(arp->ar_hrd)); ptr+=40;
	sprintf((char*)ptr,"Format of protocol address:      0x%04x\n", ntohs(arp->ar_pro)); ptr+=40;
	sprintf((char*)ptr,"Length of hardware address:      %02d\n", arp->ar_hln); ptr+=36;
	sprintf((char*)ptr,"Length of protocol address:      %02d\n", arp->ar_pln); ptr+=36;
	
	char *opk="???";
	switch(ntohs(arp->ar_op)){
		case ARPOP_REQUEST: opk="ARP request"; break;
		case ARPOP_REPLY: opk="ARP reply"; break;
		case ARPOP_RREQUEST: opk="RARP request"; break;
		case ARPOP_RREPLY: opk="RARP reply"; break;
		case ARPOP_InREQUEST: opk="InARP request"; break;
		case ARPOP_InREPLY: opk="InARP reply"; break;
		case ARPOP_NAK: opk="(ATM)ARP NAK"; break;
	}
	sprintf((char*)ptr,"ARP opcode:      %s\n", opk); ptr+=24+strlen(opk);
}
    
    ptr[0] = '\0';
    return;

}

void iphdr_hexout(u8 *incoming, u32 len, u8 *out){
	u32	i = 0; if(len>S64K || !len){ *out=0; return ; }
	u8	*ptr = out; Itos it; print("L:", it.itos(len), "\r\n");
    
    for(i=0;i<len;i++){
		if(!(i%16)) sprintf((char*)ptr,"%04X    ",i), ptr+=8;
		sprintf((char*)ptr,"%02X ",incoming[i]), ptr+=3;
		if((i%16)==15){
			sprintf((char*)ptr, " "), ptr++;
			for(int k=0; k<16; k++){unsigned char c=incoming[i-15+k]; if(c<32) c=32; sprintf((char*)ptr, "%c", c), ptr++; }
				//if(incoming[i-15+k]>=32) sprintf((char*)ptr, "%c", incoming[i-15+k]), ptr++; 
				//else sprintf((char*)ptr, " ", incoming[i-15+k]), ptr++; 
		}
		if(!((i-15)%16)) sprintf((char*)ptr,"\n"),ptr++;
	}

	if((i%16)){ int k; 
		for(k=0; k<16-i%16; k++) sprintf((char*)ptr, "   "), ptr+=3;
		for(k=0; k<i%16; k++){
			unsigned char c=incoming[i-i%16+k]; if(c<32) c=32; sprintf((char*)ptr, "%c", c), ptr++;
		}
	}
    
    ptr[1] = '\n', ptr[2] = '\0';
    return;
}