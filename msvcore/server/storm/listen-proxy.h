#define STORMSERVER_PROXY_HEAD 0
#define STORMSERVER_PROXY_PROXY 1

//#define STORMSERVER_PROXY_ 0
#define STORMSERVER_PROXY_SOCK5 1
#define STORMSERVER_PROXY_SOCK5_IGN 3 // ignore
#define STORMSERVER_PROXY_SOCK5_CON 2 // proxy connect
//#define STORMSERVER_PROXY_CONNECT 2

struct socks5_str{
	unsigned char ver, cmd, rsv, atyp;
	unsigned int ip; unsigned short port;
};

int64 listen_proxy_cons = 0;
int64 listen_proxy_recv = 0;
int64 listen_proxy_send = 0;

class listen_proxy : public storm_core_ssl{
protected:

	// status
	int wstat, astat;

	// socket
	storm_socket sock;

	// proxy
	static listen_proxy *static_con;

	virtual void storm_start(){
		listen_proxy_cons ++;
		wstat = STORMSERVER_PROXY_HEAD;
		astat = STORMSERVER_PROXY_HEAD;
	}

	virtual void storm_setsock(StormCoreVirtual *core, storm_socket_data *data){
		if(static_con)
			static_con->sock.set(core, data);
	}

	virtual void storm_recv(storm_work_el &wel, VString read){
		while(1){
			switch(wstat){
				case STORMSERVER_PROXY_HEAD:
					if(!AnalysHead(wel, read))
						return ;
				break;

				case STORMSERVER_PROXY_PROXY:
					if(!AnalysProxy(wel, read))
						return ;
				break;

				default:
					wel.close();
				break;
			}
		}

		return ;
	}

	int AnalysHead(storm_work_el &wel, VString &read){
		if(!read)
			return 0;

		switch(astat){
			case STORMSERVER_PROXY_HEAD:
				if(read[0] == 0x05 || read[0] == 0x04){
					if(read[0] != 0x05 || read[1] != 1){
						wel.send("\05\02\00", 3);
					}
					else{
						wel.send("\05\00", 2);
						astat = STORMSERVER_PROXY_SOCK5;
					}
					wel.readed(read, 3);
				}
				else{
					SendError(wel,read);
					return 0;
				}
			break;
			case STORMSERVER_PROXY_SOCK5:
				if(read.sz < 10)
					return 0;

				socks5_str &str=*(socks5_str*)read.data;
				if(str.atyp != 1 && str.atyp != 3){
					wel.send("\05\08\00", 3);
					astat = STORMSERVER_PROXY_HEAD;
					return 1;
				}

				socks5_str snd = str;
				snd.cmd = 0;

				if(str.atyp == 1){
					str.ip = htonl(str.ip);
					str.port = htons(str.port);
					wel.readed(read, 10);
				} else{
					unsigned int sz = str.ip & 255;
					if(read.sz < 5 + sz)
						return 0;

					str.ip = GetIP(read.str(5, sz));
					str.port = htons(str.port);
					wel.readed(read, 5 + sz + 2);
				}

				print(HLString() + "Px Connect: " + ipitos(str.ip) + ":" + str.port + "\r\n");

				ConIp ip(str.ip, str.port);
				ip.SetNonBlock();
				SOCKET sock = ip.Connect();
				if(sock<=0){
					wel.send("\05\05\00", 3);
					astat = STORMSERVER_PROXY_HEAD;
					return 1;
				}

				UGLOCK(storm_items_list);

				listen_proxy *con = (listen_proxy*)static_storm_new();
				con->storm_init();
				con->sock = wel;

				static_con = this;

				MyStormCore.StormAddSock(con, sock, 0);

				static_con = 0;

				wel.send((char*)&snd, 10);
				//wel.readed(read, 10);

				wstat = STORMSERVER_PROXY_PROXY;
				con->wstat = STORMSERVER_PROXY_PROXY;

				astat = STORMSERVER_PROXY_SOCK5;
				con->astat = STORMSERVER_PROXY_SOCK5_CON;

				print("GO!\r\n");
			break;
		}

		return 1;
	}

	void SendError(storm_work_el &wel, VString &read){
		Itos it, it2;
		unsigned int tm = time();
		uptime(it2, tm - _proc_upped);

		it.Format("HTTP/1.0 200 OK\r\n"
		"Content-type: text/html\r\n"
		"\r\n"
		"Hi friend! I'am Free Public proxy based on StormServer(by MikelSV)<p>"
		"My status: OK<br>"
		"Uptime: %s<br>"
		"Connections %d<br>"
		"Recv data: %d<br>"
		"Send data: %d<br>"
		"<p><p>"
		"01010011 01110100 01101111 01110010 01101101  01010011 01100101 01110010 01110110 01100101 01110010"
		, it2.ret
		, listen_proxy_cons
		, listen_proxy_recv
		, listen_proxy_send
		);

		listen_proxy_recv += read;
		listen_proxy_send += it;

		wel.send(it);
		wel.close();
		return ;
	}

	virtual int AnalysProxy(storm_work_el &wel, VString &read){
		if(!read)
			return 0;

		//print("DATA!\r\n");
		listen_proxy_recv += read;
		listen_proxy_send += read;

		if(!sock.SendData(read))
			wel.close();
		wel.readed(read, read);
		
		return 0;
	}

	virtual void storm_stop(){
		print("STOP!\r\n");
		sock.CloseSocket();
	}

	STORM_DEFINE_NEW_DELETE(listen_proxy);
};

STORM_DEFINE_NEW_DELETE_OK(listen_proxy);

listen_proxy *listen_proxy::static_con = 0;





// Test
DWORD TestSocks5(LPVOID){

	for(int i=0; i<S1K; i++){
		ConIp ip("127.0.0.1:8079");
		SOCKET sock=ip.Connect();

		struct socks5_str{
			unsigned char ver, cmd, rsv, atyp;
			unsigned int ip; unsigned short port;
		};

		MString r;
		r.Reserv(S16K);
		VString s="GET / HTTP/1.0\r\nHost: yandex.ru\r\n\r\n";

		socks5_str str;
		str.ver=5; str.cmd=1; str.atyp=1;
		str.ip=GetIPh("yandex.ru");
		str.port=htons(80);

		send(sock, (char*)&str, 3, 0);
		int rs=recv(sock, r, r, 0);

		Sleep(100);

		if(rs!=2)
			int wty=47;

		send(sock, (char*)&str, 10, 0);
		rs=recv(sock, r, r, 0);

		if(rs!=10)
			int wty=47;

		send(sock, s, s, 0);
		rs=recv(sock, r, r, 0);

		if(rs != 478)
			int wty=47;

		//Sleep(100);
		closesocket(sock);
	}

	print("END OF THREAD.\r\n");

	return 0;
}

DWORD TestSocks5_Old(LPVOID){
	for(int i=0; i<S1K; i++){
		ConIp ip("127.0.0.1:8079");
		SOCKET sock=ip.Connect();

		struct socks5_str{
			unsigned char ver, cmd, rsv, atyp;
			unsigned int ip; unsigned short port;
		};

		MString r;
		r.Reserv(S16K);
		VString s="GET / HTTP/1.0\r\nHost: yandex.ru\r\n\r\n";

		socks5_str str;
		str.ver=5; str.cmd=1; str.atyp=1;
		str.ip=GetIPh("yandex.ru");
		str.port=htons(80);

		Sleep(10);

		send(sock, (char*)&str, 3, 0);
		int rs=recv(sock, r, r, 0);

		closesocket(sock);
	}

	print("END OF THREAD.\r\n");

	return 0;
}
