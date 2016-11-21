class listen_websockets : public storm_core_ssl{
	int wstat;

public:

	virtual void storm_init(){
		wstat = 0;
	}

	virtual void storm_recv(storm_work_el &wel, VString read){
		if(!wstat){
			VString head, data;
			head = PartLine(read, data, "\r\n\r\n");

			if(data.data){
				wel.readed(head.size()+4);
				head.sz += 2;

				return AnalysHead(wel, head);
			}
			else 
				return ;
		}else{
			int r, opcode;
			VString msg;

			r = WebSocketDecodeData(read, opcode, msg);
			if(r < 0)
				return ;
			if(r == 0){
				wel.close();
				return ;
			}

			AnalysData(wel, opcode, msg);
			wel.readed(r);
		}

		return ;
	}

	virtual void AnalysHead(storm_work_el &wel, VString head){
		//VString path = PartLineDouble(head, "GET ", " HTTP");
		//VString host = PartLineDouble(head, "\r\nHost: ", "\r\n");
		VString key = PartLineDouble(head, "\r\nSec-WebSocket-Key: ", "\r\n");
		VString origin = PartLineDouble(head, "\r\nOrigin: ", "\r\n");

		MString ret;
		ret.Add("HTTP/1.1 101 Switching Protocols\r\n" \
			"Upgrade: WebSocket\r\n" \
            "Connection: Upgrade\r\n" \
            "Sec-WebSocket-Origin: ", origin, "\r\n" \
			"Sec-WebSocket-Accept: ", WebSocketAcceptKey(key), "\r\n" \
            "\r\n");

		wel.send(ret);
		
		wstat = 1;
		return ;
	}

	virtual void AnalysData(storm_work_el &wel, int opcode, VString read){
		switch(opcode){
			case LWSOC_STRING:{
				MString ret = WebSocketEncodeData(LWSOC_STRING, HLString() + "recieved: " + read);
				wel.send(ret);
			}
			break;

			case LWSOC_PING:{
				MString ret = WebSocketEncodeData(LWSOC_PONG, MString());
				wel.send(ret);
			}
			break;

			default:
				wel.close();
			break;
		}

		return ;
	}

};