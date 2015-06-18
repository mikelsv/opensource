// Activate Socket

class _msv_active_socket{
public:
	_msv_active_socket(){
#ifdef WIN32
		WSADATA WsaData; WSAStartup(MAKEWORD(2, 2), &WsaData);
#else
		signal(SIGPIPE, SIG_IGN);
#endif 
	}

}_msv_active_socket;

// traffix all send, all recv
int64 _msvps_send=0, _msvps_recv=0;

#include "net.cpp"

unsigned int GetIP(VString ipa);
unsigned int GetIPh(VString ipa);

void KeepAlive(SOCKET sock){
	int i=1;
	if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&i, 4)) print("SOCKOPT ERROR\r\n");
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i))) print("SOCKOPT ERROR\r\n");
	return ;
}

bool ifrecv(SOCKET sock, unsigned int wt){
	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
	tm.tv_sec=wt/1000; tm.tv_usec=1000;
	return select(sock+1, &rfds, 0, 0, &tm)!=0;
}

bool ifsend(SOCKET sock, unsigned int wt){
	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
	tm.tv_sec=wt/1000; tm.tv_usec=0;
	return select(sock+1, 0, &rfds, 0, &tm)!=0;
}

// Connect Ip
class ConIp{
public:
	// data
	unsigned int ip, bip;
	unsigned short port, bport;
	char reuse, keepalive, nolisten, set_nonblock;
	int type, proto;

public:
	//con
	ConIp(){ Init(); }
	ConIp(VString _ip, VString _bip=VString()){
		Init(); Ip(_ip, _bip); return ;
		//VString p;
		//if(_ip){ _ip=PartLine(_ip, p, ":"); if(_ip && !p){ p=_ip; _ip.Clean(); } ip=GetIP(_ip); port=p.toi(); }
		//if(_bip){ _bip=PartLine(_bip, p, ":"); bip=GetIP(_bip); bport=p.toi(); }
		return ;
	}

	void Ip(VString _ip, VString _bip=VString()){
		VString p;
		if(_ip){ _ip=PartLine(_ip, p, ":"); ip=GetIP(_ip); port=p.toi(); }
		if(_bip){ _bip=PartLine(_bip, p, ":"); bip=GetIP(_bip); bport=p.toi(); }
		return ;
	}

	void IpL(VString _ip, VString _bip=VString()){
		VString p;
		if(_ip){ _ip=PartLine(_ip, p, ":"); if(_ip && !p){ p=_ip; _ip.Clean(); } ip=GetIP(_ip); port=p.toi(); }
		if(_bip){ _bip=PartLine(_bip, p, ":"); bip=GetIP(_bip); bport=p.toi(); }
		return ;
	}

	//ConIP(const char* ip, const char *bip){
	//	Init(); VString p;
	//	if(_ip){ _ip=PartLine(_ip, p, ":"); ip=GetIP(_ip); port=p.toi(); }
	//	if(_bip){ _bip=PartLine(_bip, p, ":"); bip=GetIP(_bip); bport=p.toi(); }
	//	return ;
	//}

	ConIp(unsigned int _ip, unsigned short _port, unsigned int _bip=0, unsigned short _bport=0){
		Init(); ip=_ip; port=_port; bip=_bip; bport=_bport;
		return ;
	}
	
	// init
	void Init(){
		ip=0; bip=0; port=0; bport=0; type=SOCK_STREAM; proto=IPPROTO_TCP;
		reuse=0; keepalive=0; nolisten=0;
		set_nonblock = 0;
	}

	// opt
	int ReUse(){ return reuse; }
	int ReUse(int v){ return reuse=v; }
	int KeepAlive(){ return keepalive; }
	int KeepAlive(int v){ return keepalive=v; }
	int NoListen(){ return nolisten; }
	int NoListen(int t){ return nolisten=t; }

	void SetNonBlock(int v = 1){
		set_nonblock = v;
	}

	void Proto(int _type, int _proto){ type=_type; proto=_proto; }

	// act
	int Connect() const{
		int err; if(!ip) return 0;
		SOCKADDR_IN csocket;
		SOCKET sock = socket(AF_INET, type, proto);

		// bind
		if(bip){
			csocket.sin_family = AF_INET;
			csocket.sin_addr.s_addr = htonl(bip);
			csocket.sin_port = htons(bport);
			err = bind(sock, (LPSOCKADDR)&csocket, sizeof(csocket) );
			if(err<0){ closesocket(sock); return 0; }
		}

		// connect
		csocket.sin_addr.s_addr = htonl(ip);
		csocket.sin_family = AF_INET;
 		csocket.sin_port = htons(port);

		if(proto == IPPROTO_UDP)
			return sock;

		if(set_nonblock){
#ifdef WIN32
			unsigned long v = 1;
		if(ioctlsocket(sock, FIONBIO, &v) < 0)
#else
		if(fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
#endif
			print("SOCKOPT O_NONBLOCK failed.\r\n");
		//fcntl(sock, F_SETFL, O_NONBLOCK);
		}		

		err = connect(sock, (struct sockaddr*)&csocket, sizeof(csocket));
		if(err < 0 && !set_nonblock){
			closesocket(sock);
			return 0;
		}
		//KeepAlive(sock);

		// options
		int i=1;
		if(keepalive)
			if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&i, 4)) print("SOCKOPT ERROR\r\n");
		if(reuse)
			if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i))) print("SOCKOPT ERROR\r\n");
		return (int)sock;
	}

	SOCKET Listen(){
		int err;// if(!ip) return 0;
		SOCKADDR_IN csocket;
#ifdef WIN32
		SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
#else
		SOCKET sock = socket(AF_INET, type, proto);
#endif

		// options
		int i=1;
		if(reuse){
			if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i, sizeof(i))) 
				print("SOCKOPT ERROR: SO_REUSEADDR\r\n");
		}

        struct timeval tv;
		tv.tv_sec = 30; tv.tv_usec=0;
		if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)))
			print("SOCKOPT ERROR: SO_RCVTIMEO\r\n");

		// bind
		//if(bip){
			csocket.sin_family = AF_INET;
			csocket.sin_addr.s_addr = htonl(ip);
			csocket.sin_port = htons(port);
			err = bind(sock, (LPSOCKADDR)&csocket, sizeof(csocket) );
			if(err<0){ closesocket(sock); return 0; }
		//}

		// options
		if(keepalive)
			if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&i, 4)) print("SOCKOPT ERROR\r\n");

		if(nolisten || proto==IPPROTO_UDP) return sock;
		err = listen(sock, 0x7fffffff); if(err<0){ closesocket(sock); return 0; }
		return sock;
	}
};

// Connect functions
int ConnectPortI(unsigned int lip, int port){ if(!lip) return 0;
	SOCKADDR_IN csocket; int err;
	csocket.sin_addr.s_addr = htonl(lip);
	csocket.sin_family = AF_INET;
	csocket.sin_port = htons(port);

	SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	err=connect(sock, (struct sockaddr*)&csocket, sizeof(csocket));
	if (err<0){ closesocket(sock); return 0; }
	KeepAlive(sock);
	return (int)sock;
}

int ConnectPort(VString lip, int port=0){ if(!lip) return 0;
	VString _p; lip=PartLine(lip, _p, ":"); if(_p) port=_p.toi();
	return ConnectPortI(GetIP(lip), port);
}

int ConnectPortEx(unsigned int ip, int port, int type=SOCK_STREAM, int proto=IPPROTO_TCP, unsigned int bip=0, unsigned short bipp=0){
int err; if(!ip) return 0;
	SOCKADDR_IN csocket;
	SOCKET sock = socket(AF_INET, type, proto);

	// bind
	if(bip){
		csocket.sin_family = AF_INET;
		csocket.sin_addr.s_addr = htonl(bip);
		csocket.sin_port = htons(bipp);
		err = bind(sock, (LPSOCKADDR)&csocket, sizeof(csocket) );
		if(err<0){ closesocket(sock); return 0; }
	}

	// connect
	csocket.sin_addr.s_addr = htonl(ip);
	csocket.sin_family = AF_INET;
 	csocket.sin_port = htons(port);

	err=connect(sock, (struct sockaddr*)&csocket, sizeof(csocket));
	if (err<0){ closesocket(sock); return 0; }
	if(type==IPPROTO_TCP) KeepAlive(sock);
	return (int)sock;
}


bool isip(const char*line, unsigned int size){
	if(!line) return 0;
	int sz= size ? size : strlen(line);
	int i=0, s=0; char*l=(char*)line, *t=l+sz;

	for(l; l<t; l++){
		if(*l=='.'){ i++; s=0;}
		else if(*l<'0' || *l>'9') return 0;
		else{ s=s*10+*l-'0'; }
		
		if(i>3 || s>255) return 0;
	}
	if(i!=3 || s>255) return 0;
	return 1;
}

bool isip(const VString line){ return isip((char*)line.data, line.sz); }

unsigned int GetIPh(VString ipa){
	if(!ipa || ipa.sz >= S2K-1)
		return 0;

	unsigned char safe[S2K];
	memcpy(safe, ipa, ipa);
	safe[ipa.sz] = 0;

	char *ip = (char*)&safe;
	//ItosX<S2K> it; it=ipa; char*ip=it.ret;
	if(isip(ip))
		return inet_addr(ip);
    
	struct addrinfo *ai = 0, *pai;
	unsigned int ipr=0;

	if(getaddrinfo(ip, 0, 0, &ai))
		return 0;

	pai = ai;
	while(pai){
		if(pai->ai_family == PF_INET)
			break;
		pai = pai->ai_next;
	}

	if(pai)
		ipr = ((struct sockaddr_in*)pai->ai_addr)->sin_addr.s_addr;
	else
		ipr = 0;

	freeaddrinfo(ai);

	return ipr;
}

unsigned int GetIP(VString ipa){ return htonl(GetIPh(ipa)); }


// MSV IP Library (27.02.2005)
/*
unsigned int ipstoi(const MString &line){
MString tmp[4];
int err=explode(tmp, line, ".", 4);
return stoi(tmp[0])*256*256*256+stoi(tmp[1])*256*256+stoi(tmp[2])*256+stoi(tmp[3]);
}*/

unsigned int ipstoi(char*data, unsigned int sz){ if(!data) return 0;
	char*to=data+sz, *p=data; unsigned int x=256*256*256, ret=0;
	while(p<to){
		if(*p=='.') {ret+=stoi(data, p-data)*x; x/=256; p++; data=p; continue; }
		if(*p<'0' || *p>'9') return 0; p++;
	}
	return ret+stoi(data, p-data);
}

unsigned int ipstoi(const VString& line){ return ipstoi((char*)line.data, line.sz); }

IP6_ADDRESS ip6stoi(char*data, unsigned int sz){ // 1:2:3:4:5:6:7:8
unsigned short ip[8]; memset(&ip, 0, sizeof(ip));  if(!data) return *(IP6_ADDRESS*)&ip;
char*l=data, *ll=l, *to=data+sz; int p=0, s=9;
if(*l==':') ll=++l;

while(l<to+1){
	if(*l==':' || l==to){
		if(*l==':' && ll==l){
			if(s!=9){
				memset(&ip, 0, sizeof(ip)); return *(IP6_ADDRESS*)&ip;
			}
			s=p; l++; continue;
		}
		unsigned short a=stoi(ll, l-ll, 16); ip[p]=a; p++;
		ll=l+1; if(l==to) break;
	}
l++;
}

if(p==8) return *(IP6_ADDRESS*)&ip;
if(s==9){ memset(&ip, 0, sizeof(ip)); return *(IP6_ADDRESS*)&ip; }
memcpy(&ip[8-p], &ip[s], (8-(8-p))*2);
memset(&ip[s], 0, (8-p)*2);
return *(IP6_ADDRESS*)&ip;
}

MString ip6itosf(IP6_ADDRESS ip){ // 1:2:3:4:5:6:7:8
	MString ret; ret.Reserv(7+8*4);
	unsigned short *i=(unsigned short *)&ip, *t=i+8;
	unsigned char *l=ret;

	for(i; i<t; i++){
		if(l!=ret.data){ *l=':'; l++; }
		unsigned char b;
		b=*i>>12; if(b<10) *l++='0'+b; else *l++=87+b;
		b=*i>>8&15; if(b<10) *l++='0'+b; else *l++=87+b;		
		b=*i>>4&15; if(b<10) *l++='0'+b; else *l++=87+b;
		b=*i&15; if(b<10) *l++='0'+b; else *l++=87+b;
	}
	return ret;
}

MString ip6itos(IP6_ADDRESS ip){ // 1:2:3:4:5:6:7:8
	unsigned char data[7+8*4];
	//MString ret; ret.Reserv(7+8*4);
	unsigned short *i=(unsigned short *)&ip, *t=i+8;
	unsigned char *l=data;

	int n=0, mn=0; unsigned short *mp=0;
	for(++i; i<t-1; i++){
		if(!*i) n++; else n=0;
		if(n>mn){ mn=n; mp=i-n+1; }
	}    

	i=(unsigned short *)&ip;
	for(i; i<t; i++){
		if(l!=data){ *l=':'; l++; }
		if(mp==i){ i+=mn-1; continue; }

		unsigned char b, s=0;
		b=*i>>12;	if(b || s){ if(b<10) *l++='0'+b; else *l++=87+b; s=1; }
		b=*i>>8&15; if(b || s){ if(b<10) *l++='0'+b; else *l++=87+b; s=1; }
		b=*i>>4&15;	if(b || s){	if(b<10) *l++='0'+b; else *l++=87+b; s=1; }
		b=*i&15;	if(b<10) *l++='0'+b; else *l++=87+b;
	}
	return MString((unsigned char*)data, (char*)l-(char*)data);
}


MString ipitos(unsigned int val){
	char buf[15], *p=buf; BYTE b;
	//MString ret;
	//BYTE b=*(BYTE*)&val; int sz=b<10 ? 1 : b<100 ? 2 : 3;
	//b=*((BYTE*)&val+1); sz+=b<10 ? 1 : b<100 ? 2 : 3;
	//b=*((BYTE*)&val+2); sz+=b<10 ? 1 : b<100 ? 2 : 3;
	//b=*((BYTE*)&val+3); sz+=b<10 ? 1 : b<100 ? 2 : 3;
	//sz+=3;
	//ret.Reserv(sz); char *p=ret;

	b=*((BYTE*)&val+3); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
	b=*((BYTE*)&val+2); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
	b=*((BYTE*)&val+1); if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; } *p++='.';// sz++;
	b=*(BYTE*)&val; if(b<10) *p++='0'+b; else if(b<100){ *p++='0'+b/10; *p++='0'+b%10; } else{ *p++='0'+b/100; *p++='0'+b/10%10; *p++='0'+b%10; }// *p++='.';// sz++;
	return MString(buf, p-buf);
}


MString HttpToVal(VString line){
	MString ret;
	ret.Reserv(prmf_httptoval(0, 0, line, line));
	prmf_httptoval(ret, ret, line, line);
	return ret;
}

MString HttpToValNoPlus(VString line){
	MString ret;
	ret.Reserv(prmf_httptoval(0, 0, line, line, 1));
	prmf_httptoval(ret, ret, line, line, 1);
	return ret;
}