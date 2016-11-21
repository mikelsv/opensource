// Use child class. Listen();

Versions lightserver_versions[]={
	"0.0.0.2", "01.07.2015 19:41",
	"0.0.0.1", "28.09.2013 16:29"	
};

class LightServerAccept{
public:
	unsigned int cip, tip; unsigned short cport, tport; SOCKET sock;
};

class LightServer{
	MString open_ip;
	//unsigned int open_ip; unsigned short open_port;
	SOCKET open_sock; int stopserver;

public:
	LightServer(){
		stopserver=0;
	}

	void SetPort(VString ip){
		open_ip=ip;
	}

	//void SetPort(unsigned short p, unsigned int ip=0){
	//	open_ip=ip; open_port=p;
	//}


	int StopServer(){
		stopserver=1;
	}

	int Listen(){
		fd_set rfds, efds; SOCKET maxs; int i, tps=0, pos=0, lpos=0, sel;
		sockaddr_in from; int fromlen=sizeof(from); timeval tm; SOCKET socks; ConIp cip;

		// Listen		
		//cip.ip=open_ip; cip.port=open_port;
		cip.IpL(open_ip);
		cip.ReUse(1); open_sock=cip.Listen();

		// Run server
		stopserver=0;

		while(!stopserver){
			FD_ZERO(&rfds); FD_ZERO(&efds); maxs=0;

			FD_SET(open_sock, &rfds); maxs=open_sock+1;

			tm.tv_sec=30; tm.tv_usec=0;
			sel=select(maxs, &rfds, 0, 0, &tm); if(sel<1){ Sleep(100); }

			if(FD_ISSET(open_sock, &rfds)){
				socks=accept(open_sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);
				if(socks<=0){
					closesocket(open_sock); stopserver=1;
				}

				LightServerAccept acc;
				acc.sock=socks; acc.cip=ntohl(from.sin_addr.s_addr); acc.cport=htons(from.sin_port);
				getsockname(socks, (struct sockaddr*)&from, (socklen_t*)&fromlen);
				acc.tip=ntohl(from.sin_addr.s_addr); acc.tport=htons(from.sin_port);

				Accept(acc);
			}
		}

		return 0;
	}

	virtual int Accept(LightServerAccept &acc){
		Itos it; it.Add("LightServer(", lightserver_versions[0].ver, ", ", lightserver_versions[0].date,")");
		send(acc.sock, it.ret, it.ret, 0);
		closesocket(acc.sock);
		return 0;
	}

};


#ifdef USEMSV_LIGHTSERVER_HTTP_CALL
int USEMSV_LIGHTSERVER_HTTP_CALL(LightServerAccept &acc, VString head, VString post);
#endif


class LightServerHttp : public LightServer, public MSVMCOT{

	// certs
	static VString cert_cert, cert_key;

public:
	void SetCerts(VString cert, VString key){
		cert_cert = cert;
		cert_key = key;
	}

	virtual int Accept(LightServerAccept &acc){
		// Call Accept() in new Thread.

		LightServerAccept *nacc = new LightServerAccept;
		*nacc = acc;
		MCOTS(nacc);

		return 1;
	}

	virtual DWORD MCOT(LPVOID lp){
		LightServerAccept *acc = (LightServerAccept*)lp;
		print("New connection!\r\n");
		AcceptData(*acc);
		print("Connection close!\r\n");
		closesocket(acc->sock);
		delete acc;
		return 0;
	}

	virtual int AcceptData(LightServerAccept &acc){
		unsigned char buf[S32K];
		unsigned int pos = 0;
		MySSL ssl;

		// Use ssl sertificate
		if(cert_cert && cert_key){
			ssl.Release();
			ssl.Accept(acc.sock, cert_cert, cert_key);
		}

		while(1){
			// read data from socket
			if(ifrecv(acc.sock)){
				int rcv = ssl.Recv((char*)(buf + pos), S32K - pos);
				if(rcv <= 0)
					break;

				pos += rcv;

				// analys data
				VString read(buf, pos);

				// get head
				VString head, post;
				head = PartLine(read, post, "\r\n\r\n");
				// get method
				VString method = PartLineO(read, " ");
				
				if(post.data){
					int cl = PartLineDoubleUp(head, "Content-Length: ", "\r\n").toi();
					if(cl && post.size() < cl)
						continue;
					post.sz = cl;
				}
				else
					continue;

				head.sz += 2;

#ifdef USEMSV_LIGHTSERVER_HTTP_CALL
				if(USEMSV_LIGHTSERVER_HTTP_CALL(acc, head, post)){
					SString shead;
					//VString shead = "HTTP/1.0 200 OK\r\n"
//						"Connection: close\r\n";
					
					int ka = PartLineDoubleUp(head, "Connection: ", "\r\n").compareu("Keep-Alive");

					VString sdata = "{}";
					shead.Format("HTTP/1.1 200 OK\r\n"
						"Connection: %s\r\n"
						"Content-Length: %d\r\n"
						"\r\n",
						ka ? "close" : "keep-alive",
						sdata.sz
						);

					ssl.Send(shead, shead);
					//ssl.Send("\r\n", 2);
					ssl.Send(sdata, sdata);

					// Keep-Alive
					if(ka){
						print("Keep-Alive.\r\n");
						pos = 0;
						continue;
					}

					ssl.Close();
					return 0;
				}
#endif

				// send answer
				VString shead = "HTTP/1.0 200 OK\r\n";
				VString sdata = "Hello world!";

				ssl.Send(shead, shead);
				ssl.Send("\r\n", 2);
				ssl.Send(sdata, sdata);

				ssl.Close();
				return 0;
			}
		}

		ssl.Close();
		return 0;
	}


};

VString LightServerHttp::cert_cert;
VString LightServerHttp::cert_key;