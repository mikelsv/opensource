class StormServerConnection{
public:
	MString ip;
	MString proto;
	SOCKET sock;
};

class StormServerProtos{
public:
	MString proto;
	storm_item *core;
};

class StormServer : public MSVCOT{
	
	// Configure file
	MString conf;
	MString core_proto;

	// Conections
	IList<StormServerConnection> cons;
	IList<StormServerProtos>protos;

	// Lock
	TLock acceptlock;

	// Core
	//RainCore core;
	StormCoreVirtual *core;

public:

	StormServer(StormCoreVirtual &c){
		core=&c;
		conf = STORMSERVER_CONFIG;
	}

	// Configure
	void UseConf(VString v){ conf=v; }

	void LoadConf(){
		ConfLine cf(conf);

		//core_proto=cf["rain.proto"];

		VString key, val, o, t, f; unsigned int p=0;
		while(p=cf.GetLine(key, p)){
			key=PartLine(key, val, " ");

			if(key.compareu("listen")){
				dspacevt(val);
				cons.n().ip=PartLine(val, val, " ");
				if(!cons.n().ip) continue;				
				
				dspacevt(val);
				cons.n().proto=PartLine(val, val, " ");
				cons.Added();
			}
		}
	}

	StormCoreVirtual* GetCore(){ return core; }

	// Run
	int Run(){
		if(!conf) return 0;
//		StartThread(KioThread);
		core->StormRunCore();// Run();
		Listen();
		return 1;
	}

	virtual DWORD COT(LPVOID lp){
		Run();
		return 0;
	}

	int RunInThread(){
		if(!conf) return 0;
		COTS();
		return 1;
	}

	void OpenPort(){
		ConfLine cf(conf);		
		ConIp cip;
		//cip.ReUse(cf["rain.reuse"].toi());
		cip.ReUse(1);

		for(unsigned int i=0; i<cons.size(); i++){
			cip.IpL(cons[i].ip);
			cons[i].sock=cip.Listen();
		}
	}

	void Listen(){
		fd_set rfds, efds; SOCKET maxs; unsigned int i, consel, conpos=0, conpos2;
		//unsigned int i; // int i, tps=0, pos=0, lpos=0, sel;
		sockaddr_in from; int fromlen=sizeof(from); timeval tm; // SOCKET socks; ConIp cip;

		// Open Port
		LoadConf();
		OpenPort();

		// Go
		while(1){
			FD_ZERO(&rfds); FD_ZERO(&efds); maxs=0; consel=0;

			for(i=conpos; i<cons.size(); i++){
				if(!cons[i].sock)
					continue;

				if(cons[i].sock>=maxs) maxs=cons[i].sock+1;
				FD_SET(cons[i].sock, &rfds);
				if(++consel>=32){ break; }
			}
			conpos2=i;

			// Select
			tm.tv_sec=1; tm.tv_usec=30000;
			int sel=select(maxs, &rfds, 0, 0, &tm);
			if(sel<0)
				return;

			for(i=conpos; i<conpos2; i++){
				if(FD_ISSET(cons[i].sock, &rfds)){
					SOCKET sock=accept(cons[i].sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

					//VString s = "HTTP/1.1 200 OK\r\n"; // test
					//send(sock, s, s, 0);
					////shutdown(sock, SD_SEND);
					//closesocket(sock);
					//sock = 0;

					if(sock <= 0)
						continue;

					Accept(cons[i], sock);
				}
			}

			if(conpos2==cons.size())
				conpos=0;
			else
				conpos=conpos2;
		}
		return ;
	}

	void Accept(StormServerConnection &con, SOCKET sock){
		storm_item *el=NewProtoEl(con);
		if(!el){
			closesocket(sock);
			return ;
		}

#ifdef RAIN_PRINT_ACCEPT
		print("A:", itos(sock), " Socks: ", itos(core.Size()), "\r\n");
#endif
		core->StormAddSock(el, sock);
		return ;
	}

	// Protocols
	void InsertProto(VString name, storm_item *el){
		protos.n().proto=name;
		protos.n().core=el;
		protos.Added();
		return ;
	}

	storm_item* NewProtoEl(StormServerConnection &con){
		storm_item *el=0;
		for(unsigned int i=0; i<protos.size(); i++){
			if(!con.proto || con.proto == "*"){ //core_proto || core_proto=="*"){
				if(!el)
					el = protos[i].core;
			}else{
				VString name = protos[i].proto;
				unsigned char *p = con.proto;
				int f = rtms(con.proto.endu(), name, name, p);
				if(f && (p == con.proto.data || *(p - 1) == ' ') && (p + name.sz == con.proto.endu() || *(p + name.sz) == ' '))
					el = protos[i].core;
			}
		}

		if(el)
			return el->storm_new();
		return 0;
	}

};