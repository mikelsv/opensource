// Listen WebSockets Operacion Code
#define LWSOC_CONTINUE	0x00
#define LWSOC_STRING	0x01
#define LWSOC_BINARY	0x02
#define LWSOC_CLOSE		0x08
#define LWSOC_PING		0x09
#define LWSOC_PONG		0x0A

#define WSCLI_MSG_MAX_SIZE S16K


// Input: Sec-WebSocket-Key. Return: Sec-WebSocket-Accept.
MString WebSocketAcceptKey(VString key){
	MString ret;
	MHash mh(MHASHT_SHA1);

	mh.Add(key);
	mh.Add("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	mh.Finish();

	return Base64::stob(mh.Get());
}

// Return: -1 - need more data, 0 - error, any - used size.
int WebSocketDecodeData(VString read, int &r_opcode, VString &r_value){
		// need more data
		if(read.sz <= 4)
			return -1;

		unsigned int fin = *read.data, opcode = fin&15, lpos;
		uint64 len = *(read.data+1);
		unsigned int mask = len & 128;

		//if(opcode == LWSOC_CLOSE){
		//	wel.closed();
		//	return ;
		//}

		switch(len&127){
			case 126:
				len = *(unsigned short*)(read.data+2);
				lpos = 4;
			break;
			case 127:
				if(read.sz<10)
					return -1;
				len = *(uint64*)(read.data+2);
				lpos = 10;
			break;
			default:
				lpos = 2;
				len &= 127;
			break;
		}

		// need more data
		if(read.sz < lpos || read.sz < lpos + len + (mask ? 4 : 0))
			return -1;

		if(mask){
			mask = *(unsigned int*)(read.data+lpos);
			lpos += 4;
		}

		if(len > S1M){
			//wel.closed();
			return 0;
		}

		if(mask){
			unsigned char *l = read.data + lpos;
			unsigned char *t = read.data + lpos + len;
			unsigned char *ft = t - (lpos + len) % 4;
			unsigned char *m = (unsigned char*)&mask;

			for(l; l < ft; l += 4){
				*(l+0) ^= m[0];
				*(l+1) ^= m[1];
				*(l+2) ^= m[2];
				*(l+3) ^= m[3];
			}

			for(l; l < t; l++)
				*l ^= *m++;
		}


		//AnalysData(wel, opcode, read.str(lpos, len));
		//wel.readed(lpos + len);
		r_opcode = opcode;
		r_value = read.str(lpos, len);
		return lpos + len;
}

MString WebSocketEncodeData(int type, VString data, int fin = 1){
	unsigned char buf[10];
	int mask = rand(), lpos;

	if(!data)
		mask = 0;

	buf[0] = (fin << 7) + (type&15);

	if(data.sz <= 125){
		buf[1] = data.sz + (mask ? 128 : 0);
		lpos = 2;
	}else if(data.sz <= 65535){
		buf[1] = 126 + (mask ? 128 : 0);
		*(unsigned short*)(buf + 2) = data.sz;
		lpos = 4;
	} else{
		buf[1] = 127 + (mask ? 128 : 0);
		*(uint64*)(buf + 2) = data.sz;
		lpos = 10;
	}

	MString ret;
	ret.Reserv(lpos + (mask ? 4 : 0) + data.sz);
	if(!ret)
		return MString();

	unsigned char *l = ret.data + lpos + (mask ? 4 : 0);

	memcpy(ret.data, buf, lpos);
	memcpy(l, data, data);

	if(mask){
		unsigned char *t = l + data.sz;
		unsigned char *ft = t - (data.sz % 4);
		unsigned char *m = (unsigned char*)&mask;

		memcpy(ret.data + lpos, &mask, 4);

		for(l; l < ft; l += 4){
			*(l+0) ^= m[0];
			*(l+1) ^= m[1];
			*(l+2) ^= m[2];
			*(l+3) ^= m[3];
		}

		for(l; l < t; l++)
			*l ^= *m++;
	}

	return ret;
}


struct WebSocketCliTraff{
	int count, acount, size, asize;

	WebSocketCliTraff(){
		CleanA();
	}

	void Traff(int sz){
		size += sz;
		asize +=sz;
	}

	void Count(){
		count++;
		acount++;
	}

	void Clean(){
		count = 0;
		size = 0;
	}

	void CleanA(){
		count = 0;
		acount = 0;
		size = 0;
		asize = 0;
	}

};

class WebSocketsCli : public TLock{
private:
	SOCKET sock;
	int usessl, usethreads;
	unsigned int maxsize;

	// buffers
	MString buf, tbuf;
	VString vbuf;

	// message
	int ws_opcode;
	VString ws_data;

	// traff
	WebSocketCliTraff sndt, rcvt;

#ifdef USEMSV_OPENSSL
	MySSL ssl;
#endif

public:
	static const int state_test = 1;

	WebSocketsCli(){
		sock = 0;
		usessl = 0;
		usethreads = 0;
		maxsize = WSCLI_MSG_MAX_SIZE;
	}

	// options
	void UseSsl(int val = 1){
		usessl = val;
	}

	void UseThreads(int val = 1){
		usessl = val;
	}

	void UseMaxSize(int val = WSCLI_MSG_MAX_SIZE){
		maxsize = val;
	}

	int IsConnect(){
		return sock;
	}

	// operations
	int Connect(VString path){
		if(usethreads && !TLock::lock)
			return 0;

		ConIp ip(path);
		sock=ip.Connect();

		if(!sock)
			return 0;

		sndt.Clean();
		rcvt.Clean();

		buf.Clean();
		vbuf.Clean();
		ws_data.Clean();

		return SendHello();
	}

	int SendHello(){
		Itos it;

		tbuf = md5h(it.Format("%i%s%i", time(), "MikelSV", rand()));

		it.Add("GET / HTTP/1.1" "\r\n"
			"Upgrade: WebSocket" "\r\n"
			"Connection: Upgrade" "\r\n"
			"Sec-WebSocket-Key: ", tbuf, "\r\n" 
			"\r\n");

		if(!Send(it, 0))
			return 0;

		return Recv();
	}

	// Return: 0 - fail, 1 - ok.
	int Send(VString line, int enc = 1){
		if(usethreads && !TLock::lock)
			return 0;

		if(!sock)
			return 0;

		MString sline;
		if(enc)
			line = sline = WebSocketEncodeData(LWSOC_BINARY, line);

		int s = send(sock, line, line, 0);
		if(s <= 0)
			return 0;

		sndt.Traff(line);
		sndt.Count();

		return 1;
	}

	// Return: 0 - fail, 1 - ok.
	int Recv(int state = 0){
		if(usethreads && !TLock::lock)
			return 0;

		if(!sock)
			return 0;

		int r;
		ws_data.sz = 0;

		if(vbuf.data != buf.data){
			if(vbuf.sz)
				memcpy(buf, vbuf, vbuf);
			vbuf.data = buf.data;
		}

		if(vbuf){
			if((r = Analys()) != -1)
				return r;
		}

		while(1){
			if(buf.sz == vbuf.sz){
				if(buf.sz >= maxsize){
					Close();
					return 0;
				}

				buf.Reserv(buf.sz+S4K);
				vbuf.data = buf.data;
			}

			int maxtm = 120 * 60 *1000 / 5000, w = 5000;

			if(state == state_test){
				maxtm = 1;
				w = 10;
			}

			while(!ifrecv(sock, w)){
				if(!maxtm--){
					if(state == state_test)
						return -1;
					Close();
					return 0;
				}
			}

			r = recv(sock, vbuf.end(), buf.sz - vbuf.sz, 0);
			if(r<=0){
				Close();
				return 0;
			}

			rcvt.Traff(r);
			vbuf.sz += r;			

			if((r = Analys()) != -1)
				return r;
		}

		return 1;
	}


	// Return: -1 - need more data, 0 - fail, 1 - ok.
	int Analys(){

		// HTTP Request
		if(!rcvt.count){
			VString head, data;
			head = PartLine(vbuf, data, "\r\n\r\n");

			if(!data.data)
				return -1;

			head.sz += 2;

			// Code
			VString res = head.str(9, 3);
			if(res != "101"){
				Close();
				return 0;
			}

			// Sec-WebSocket-Accept
			VString key = PartLineDouble(head, "Sec-WebSocket-Accept: ", "\r\n");
			if(key != WebSocketAcceptKey(tbuf)){
				Close();
				return 0;
			}

			// Content-Length
			unsigned int length = PartLineDouble(head, "Content-Length: ", "\r\n").toi();
			if(length && data.sz < length)
				return -1;

			// Ok
			vbuf.sz = data.sz-length;
			memcpy(buf, data.str(length), vbuf.sz);

			rcvt.Count();
			return 1;
		}

		// else WebSocket
		int opcode; 
		VString val;
		ws_data.sz = 0;

		int sz = WebSocketDecodeData(vbuf, opcode, ws_data);

		if(sz < 0)
			return -1;
		else if(sz == 0){
			Close();
			return 0;
		}

		vbuf.data += sz;
		vbuf.sz -= sz;
		rcvt.Count();

		return 1;
	}

	int GetOpcode(){
		if(usethreads && !TLock::lock)
			return 0;

		return ws_opcode;
	}

	VString GetData(){
		if(usethreads && !TLock::lock)
			return VString();

		return ws_data;
	}


	int Close(){
		if(usethreads && !TLock::lock)
			return 0;
		
		Release();

		return 1;
	}

	int Release(){
		UGLOCK(this);

		if(sock){
			closesocket(sock);
			sock = 0;
		}

		buf.Clean();
		vbuf.Clean();
		ws_data.Clean();

		return 1;
	}

	~WebSocketsCli(){
		Release();
	}

};