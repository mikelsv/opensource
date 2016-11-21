void USEMSV_STORMSERVER_SMTP_CALL(VString head, VString data);


ConfLineOptions listen_smtp_conf(USEMSV_STORMSERVER_SMTP_CONF);

#define STORMSERVER_SMTP_HEAD	0
#define STORMSERVER_SMTP_DATA	1
//#define STORMSERVER_SMTP_END	2

class listen_smtp : public storm_item{
	MString head;
	//MString data;
	HLString0 tls;

	int wstate;
	int pos;

	virtual void storm_init(){
		wstate = 0;
		pos = 0;
	}

	virtual void storm_setsock(StormCoreVirtual *core, storm_socket_data *data){
		ConfLineOption *sn = listen_smtp_conf.FindOption("ServerName", 0);
		ConfLineOption *sh = listen_smtp_conf.FindOption("ServerHello", 0);

		storm_socket sock;
		sock.set(core, data);

		Itos it;
		it.Format("220 %s %s\r\n", sn ? sn->val : "stormsmtp", sh ? sh->val : "SMTP is glad to see you!");
		
		sock.SendData(it);
		
		return ;	
	}

	virtual void storm_recv(storm_work_el &wel, VString read){
		while(1){
			switch(wstate){
				case STORMSERVER_SMTP_HEAD:
					if(AnalysHead(wel, read))
						return ;
					break;
				case STORMSERVER_SMTP_DATA:
					if(AnalysData(wel, read))
						return ;
				break;
			}
		}

		return ;
	}

	int AnalysHead(storm_work_el &wel, VString &read){
		VString line = read.str(pos), k, v;

		if(read.sz >= S32K){
			wel.send("500\r\n");
			wel.close();
			return 1;
		}

		while(line){
			k = PartLine(line, line, "\r\n");
			k = PartLine(k, v, ":");

			if(!line.data){
				pos = line.data - read.data;
				return 1;
			}			

			if(k == "HELO"){
				wel.send("250\r\n");
			} else if(k == "MAIL FROM"){
				wel.send("250\r\n");
			} else if(k == "RCPT TO"){
				wel.send("250\r\n");
			} else if(k == "DATA"){
				wel.send("354\r\n");
				head.set(read.str(0, pos));
				wel.readed(read, line.endu() - read.data);

				wstate = STORMSERVER_SMTP_DATA;				
				pos = 0;
				return 0;
			} else if(k == "QUIT"){
				ConfLineOption *sn = listen_smtp_conf.FindOption("ServerName", 0);
				Itos it;

				it.Format("221 %s\r\n", sn ? sn->val : "stormsmtp");

				wel.send(it);
				wel.close();
			} else{
				wel.send("502\r\n");
			}
		}

		pos = line.data - read.data;

		return 1;
	}

	int AnalysData(storm_work_el &wel, VString &read){
		VString line = read, k;

		while(line){
			pos = line.data - read.data;
			k = PartLine(line, line, "\r\n");

			if(!line.data){
				tls.Add(read.str(pos));
				wel.readed(pos);
				return 1;
			}

			if(k == "."){
				//pos = line.data - read.data - 2;
				tls + read.str(0, k.data - read.data - 2);
				wel.readed(read, line.endu() - read.data);

				USEMSV_STORMSERVER_SMTP_CALL(head, tls.String());

				//data = tls.String();
				tls.Clear();
				pos = 0;

				wstate = STORMSERVER_SMTP_HEAD;

				wel.send("250\r\n");

				return 0;
			}
		}

		tls + read.str(0, pos);
		wel.readed(pos);

		return 1;
	}
	
	STORM_DEFINE_NEW_DELETE(listen_smtp);
};

STORM_DEFINE_NEW_DELETE_OK(listen_smtp);