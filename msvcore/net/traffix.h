#ifndef USEMSV_CONFLINE
	#error #define USEMSV_CONFLINE // need ConfLine
#endif

class Traffix;
#define TRAFFIX_RECV 1
#define TRAFFIX_SEND 2

#ifndef WIN32
#include <ifaddrs.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#include <pcap/pcap.h>

class _TraffixSnifferIfs{
	struct ifaddrs *addr;

public:
	_TraffixSnifferIfs(){
		addr = 0;
		getifaddrs(&addr);
	}

	~_TraffixSnifferIfs(){
		freeifaddrs(addr);
	}

	// Get SEND or RECV
	int GetSR(VString ifname, unsigned int sip, unsigned int dip){
		struct ifaddrs *p;
		int ret = 0;

		for(p = addr; p; p = p->ifa_next){
			if(!p->ifa_addr)
				continue;

			if(sip == ((struct sockaddr_in *)p->ifa_addr)->sin_addr.s_addr && ifname == p->ifa_name)
				ret |= TRAFFIX_SEND;

			if(dip == ((struct sockaddr_in *)p->ifa_addr)->sin_addr.s_addr && ifname == p->ifa_name)
				ret |= TRAFFIX_RECV;
		}

		return ret;
	}

} TraffixSnifferIfs;

#endif

#define ETHER_ADDR_LEN	6
	/* Ethernet header */
	struct sniff_ethernet {
		u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
		u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
		u_short ether_type; /* IP? ARP? RARP? etc */
	};

class TraffixSniffer{

	// Socket
	int sock;
	// Pcap
#ifndef WIN32
	pcap_t *pcap;
	char pcap_errbuf[PCAP_ERRBUF_SIZE];
#endif

	// Interface
	MString ifname;

	// Buffer
	MString buf;
	int lrecv;

	// Data
	u16	ep;
	struct iphdr *ip;
	unsigned int ipsz;

	unsigned int win_local_ip;

	// Error
	MString error;

public:
	TraffixSniffer(){
		sock = 0;
#ifndef WIN32
	pcap = 0;
#endif
	}

	~TraffixSniffer(){
		if(sock)
			closesocket(sock);
	}

	int Init(VString name){
		if(sock){
			closesocket(sock);
		}

		ifname = name;

		if(!ifname)
			return Error("Interface name is empty.");

		if(ifname.sz > 15)
			return Error("Interface name is very big.");

		// Create Socket
#ifdef WIN32
		sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
#else	
		sock = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ALL));
#endif	
		if(sock < 0){
			sock = 0;
			return Error("Failed allocating a socket.");
		}

#ifdef WIN32
		char hname[128];
		HOSTENT* phe;
		SOCKADDR_IN sa;

		gethostname(hname, sizeof(hname));
		phe = gethostbyname( hname );

		sa.sin_family = AF_INET;
		sa.sin_port=0;
		sa.sin_addr.s_addr = ((struct in_addr *)phe->h_addr_list[0])->s_addr;
		//if(ip) sa.sin_addr.s_addr=htonl(ip);
		//	sa.sin_addr.s_addr = inet_addr("89.108.75.95");
		if(bind(sock, (LPSOCKADDR)&sa, sizeof(sa)) < 0)
			return Error("Failed bind sockete.");

		unsigned long flag = 1;
		if(ioctlsocket(sock, SIO_RCVALL, &flag) < 0)
			return Error("Failed promiscious mode.");

		win_local_ip = sa.sin_addr.s_addr;
#else
		if(pcap)
			pcap_close(pcap);

		pcap = pcap_open_live(ifname, S64K, 1, -1, pcap_errbuf); 
		if(!pcap){
			return Error(pcap_errbuf);
		}
#endif

		ip = 0;
		buf.Reserv(S64K);

		return 1;
	}

	int Do(){
#ifdef WIN32
		if(sock <= 0)
			return Error("Socket not opened. See prevous error.");

		lrecv = recv(sock, buf, buf, 0);
		
		if(lrecv <= 0)
			return Error("Socket recieve failed.");

		ip = (struct iphdr*)(buf.data);
		ep = 0x0800;
		ipsz = lrecv;
#else
		struct pcap_pkthdr *header;
		const u_char *pkt_data;	
		int rc;
		
		while((rc = pcap_next_ex(pcap, &header, &pkt_data)) >= 0){
			if(rc == 1){

				print("len: ", itos(header->len), " ");

				// process
				struct sniff_ethernet* eptr = (sniff_ethernet *)pkt_data;
					//if(ntohs (eptr->type) == 0x0800){
						printf("Ethernet type hex:%x dec:%d is an IP packet\n",
								ntohs(eptr->ether_type),
								ntohs(eptr->ether_type));

					//ETHER_HDR *ethhdr = (ETHER_HDR*)pkt_data;
					//ep = ntohs(ethhdr->type);

					//print(itos(lrecv), " ");
					//print(itos(ep), " ");
				//}

				print("\r\n");
			}

		}


		//ip = (struct iphdr*)(buf.data + 14);
		//ep = ntohs(*(u16*)(buf.data + 12));
		//ipsz = lrecv - 14;
#endif

		return lrecv;		
	}

	// get
	VString GetData(){
		return buf.str(0, lrecv);
	}

	int GetSize(){
		return lrecv;
	}

	// send or recv
	int GetSR(){
		if(!lrecv || !buf || !ip || ep != 0x0800)
			return 0;

#ifdef WIN32
		if(ip->protocol == IPPROTO_TCP || ip->protocol == IPPROTO_UDP ){
			return (win_local_ip == ip->saddr ? TRAFFIX_RECV : 0) + (win_local_ip == ip->daddr ? TRAFFIX_SEND : 0);
		}

#else
		return TraffixSnifferIfs.GetSR(ifname, ip->saddr, ip->daddr);
#endif

		return 0;
	}

	iphdr* GetIp(){
		if(!sock)
			return 0;

		return ip;
	}

	VString GetError(){
		return error;
	}

	int Error(VString line){
		error = line;
		return 0;
	}
};

class TraffixInt{
	// Sniffer
	TraffixSniffer sniffer;

	// Interface name
	MString ifname;

	// Buffer
	MString buf;
	// write time
	int wtime;

	// Traffic
	int64 a_recv, a_send;

public:

	TraffixInt(){
		a_recv = 0;
		a_send = 0;
	}

	VString GetName(){
		return ifname;
	}

	int64 GetARecv(){
		return a_recv;
	}

	int64 GetASend(){
		return a_send;
	}


	VString GetError(){
		return sniffer.GetError();
	}

	int Init(VString name, ConfLineOption *opt){
		ifname = name;
		wtime = time();

		return sniffer.Init(ifname);
	}

	int Run(){
		while(sniffer.Do()){
			//VString

			int sz = sniffer.GetSize();
			int sr = sniffer.GetSR();

			if(sr & TRAFFIX_RECV){
				a_recv += sz;
			}
			
			if(sr & TRAFFIX_SEND){
				a_send += sz;
			}
		}

		return 0;
	}

};

class Traffix : public TLock{
	// Interfaces
	IList<TraffixInt> ints;

	// Config
	ConfLineOptions conf;

	// Paths
	MString path_conf, path_data;

	// Errors
	MString error;

public:

	Traffix(){
		if(_TSYS == TSYSC_WIN)
			Init("./", "./data/");
		else
			Init("/etc/traffix/", "/var/log/traffix/");
	}

	Traffix(VString conf, VString data){
		Init(conf, data);
	}

	int Init(VString conf, VString data){
		path_conf = conf;
		path_data = data;

		// make dir for conf and data
		//MkDir(path_conf);
		//MkDir(path_data);

		return Init();
	}

	int Run(){
		if(error)
			return 0;

		if(ints.size())
			ints[0].Run();

		return 1;
	}

	VString GetError(){
		return error;
	}

private:

	int Init(){
		// Load configuration file
		conf.LoadFile(HLString() + path_conf + "/traffix.conf");

		int icount = 0;

		ConfLineOption *opt = 0;
		while(opt = conf.Next(opt)){
			VString key = opt->key, val = opt->val;

			if(key == "Interface"){
				while(val){
					VString o = PartLineTwo(val, val);
					if(o){
						if(!AddInt(o, opt))
							return 0;
						icount ++;
					}
				}
			} else
				if(key == "Users"){

			}else
				return Error(HLString() + "Unknown key: " + key);			
		}

		if(!icount)
			return Error("Interfaces not found. Use in config: Interface name");

		return 1;
	}

public:
	int GetIntSize(){
		return ints.size();
	}

	VString GetIntName(unsigned int i){
		if(i >= ints.size())
			return VString();

		return ints[i].GetName();
	}

	int64 GetIntARecv(int ifi){
		return ints[ifi].GetARecv();
	}

	int64 GetIntASend(int ifi){
		return ints[ifi].GetASend();
	}

	int GetIntId(VString name){
		for(unsigned int i = 0; i < ints.size(); i++){
			if(ints[i].GetName() == name)
				return i;
		}

		return -1;
	}


private:
	TraffixInt * GetInt(VString name, int set = 0){
		for(unsigned int i = 0; i < ints.size(); i++){
			if(ints[i].GetName() == name)
				return &ints[i];
		}

		if(set){
			ints.A();
			//ints.n().name = name;
			ints.Added();
			return &ints.End();
		}

		return 0;
	}

	int AddInt(VString name, ConfLineOption *opt){
		TraffixInt *ti = GetInt(name, 1);
		if(!ti->Init(name, opt))
			return Error(HLString() + "Interface '" + name + "'. " + ti->GetError());
		return 1;
	}

	int Error(VString line){
		error = line;
		return 0;
	}

};