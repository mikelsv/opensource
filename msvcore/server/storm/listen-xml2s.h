//TLock listen_xml_lock;
MString _xmpp_verify_uniq_string = md5h(HLString()+time()+rand()+_procpath+"anystring");

#define XMPP_STATE_VERIFY_NO	0
#define XMPP_STATE_VERIFY_PRE	1
#define XMPP_STATE_VERIFY_UP	2
#define XMPP_STATE_VERIFY_OK	3

//#define XMPP_STATE_DVERIFY	-1

class listen_xml2s;

int listen_xml_srv_request(listen_xml2s *xmpp, XDataEl *el);
int listen_xml_cli_request(listen_xml2s *xmpp, XDataEl *el);

#ifndef USEMSV_LISTENXMPP_EXTFUNC
int listen_xml_srv_request(listen_xml2s *xmpp, XDataEl *el){
	return 0;
}

int listen_xml_cli_request(listen_xml2s *xmpp, XDataEl *el){
	return 0;
}
#endif



class listen_xml2s : public listen_xml{
	MString xmpp_verify_code;
	storm_socket sock;

	listen_xml2s *xmpp_veryfy_con;

public:
	int xmpp_verify;

	virtual void storm_start(){
		listen_xml::storm_start();

		xmpp_verify = 0;
		xmpp_veryfy_con = 0;
	}

	virtual void storm_setsock(StormCoreVirtual *core, storm_socket_data *data){
		sock.set(core, data);
	}

	virtual void AnalysXmlLineCli(storm_work_el &wel, XDataEl *el){
		VString act = el->key, res;
		Itos it;

		if(listen_xml_cli_request(this, el))
			return ;

		if(act == "stream:features"){
			if(el->a() && el->a()->key=="starttls"){
				res="<starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>";
				SendData(wel, res);
			} else{
					if(xmpp_verify_code){
						UGLOCK(storm_core_list);
						//MString code = GetVerifyCode(domain);

						//it.Format("<db:result from='%s' to='%s'>%s</db:result>", mydomain, domain, XmppGenerateCode(mydomain, domain));
						//SendData(wel, it);

						Itos it; it.Format("<db:verify from='%s' to='%s'>%s</db:verify>", from, to, xmpp_verify_code);
						//it.Format("<db:result from='%s' to='%s'>%s</db:result>", mydomain, domain, "111-b4835385f37fe2895af6c196b59097b16862406db80559900d96bf6fa7d23df3");
						SendData(wel, it);
					}else{
						if(!XmppIsVerifyDomain(to, from)){
							it.Format("<db:result from='%s' to='%s'>%s</db:result>", from, to, XmppGenerateVerifyCode(from, to));
							SendData(wel, it);
						}
					}
			}
		}
		else if(act == "proceed"){
			storm_ssl_connect(wel.data->sock);
			it.Format("<?xml version='1.0'?><stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:server' xmlns:db='jabber:server:dialback' from='%s' to='%s' version='1.0'>", to, from);
			SendData(wel, it);
		}
		else if(act == "db:result"){
			VString type = el->GetLVal("type");
			if(type){
				int ver = type == "valid";
				if(ver)
					XmppSetVerify(from, to, XMPP_STATE_VERIFY_UP);
				//XmppVerifiedConnect(mydomain, domain, this, ver);
			}
		}
		else if(act == "db:verify"){
			if(el->GetLVal("type") == "valid"){
				//XmppVerifiedConnect(domain, mydomain, con, el->GetLVal("type") == "valid");
				//SetDomainVerify(domain, con, el->GetLVal("type") == "valid");
				XmppSetVerify(to, from, XMPP_STATE_VERIFY_OK, xmpp_veryfy_con);

				// send my result
				if(!XmppIsVerifyDomain(from, to)){
					it.Format("<db:result from='%s' to='%s'>%s</db:result>", from, to, XmppGenerateVerifyCode(from, to));
					SendData(wel, it);
				}
			}

			it.Format("<db:result from='%s' to='%s' type='%s'/>", from, to, el->GetLVal("type"), el->val);
			print(HLString()+" >>>>> (" + !xmpp_state + ") " + it.ret + "\r\n\r\n");			
			XmppSend(to, from, it, xmpp_veryfy_con);
		}

		return ;
	}

	virtual void AnalysXmlLineServ(storm_work_el &wel, XDataEl *el){
		VString act = el->key, res;
		Itos it;

		if(listen_xml_srv_request(this, el))
			return ;

		if(!to && act != "stream:stream")
			return ;

		if(act == "stream:stream"){
			res="<?xml version='1.0'?><stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:server' xmlns:db='jabber:server:dialback' version='1.0'>";
			SendData(wel, res);

			if(!to){
				from=el->GetLVal("from");
				to=el->GetLVal("to");
			}

			if(!from || !to)
				wel.close();

			if(!ssl){
				res="<stream:features><starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'/><c xmlns='http://jabber.org/protocol/caps' hash='sha-1' node='http://www.process-one.net/en/ejabberd/' ver='S4v2n+rKmTsgLFog7BtVvK2o660='/></stream:features>";
			} else{
				res="<stream:features><c xmlns='http://jabber.org/protocol/caps' hash='sha-1' node='http://www.process-one.net/en/ejabberd/' ver='S4v2n+rKmTsgLFog7BtVvK2o660='/></stream:features>";
			}
			SendData(wel, res);
		}
		else if(act == "starttls"){
			res="<proceed xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>";
			SendData(wel, res);

			// init ssl connection
			storm_ssl_accept_file(wel.data->sock, "zverus.ru.cert", "zverus.ru.key");
		}
		else if(act == "db:result"){
			if(!el->GetLVal("type")){
				UGLOCK(storm_core_list);
				listen_xml2s *con;

				// send my result
				if(!XmppIsVerifyDomain(to, from)){
					it.Format("<db:result from='%s' to='%s'>%s</db:result>", to, from, XmppGenerateVerifyCode(to, from));
					//SendData(wel, it);
				}

				it.Format("<db:verify from='%s' to='%s'>%s</db:verify>", to, from, el->val);

				if(con = XmppSend(to, from, it)){
					print(HLString()+" >>>>> (" + !xmpp_state + ") " + it.ret + "\r\n\r\n");
					con->xmpp_veryfy_con = this;
				}
				else
					XmppConnect(to, from, this, el->val);
			}
			else if(el->GetLVal("type") == "valid" && xmpp_verify == XMPP_STATE_VERIFY_PRE)
				xmpp_verify = XMPP_STATE_VERIFY_OK;
		}
		else if(act == "db:verify"){
			if(!el->GetLVal("type")){
				UGLOCK(storm_core_list);

				int ver = el->val == XmppGenerateVerifyCode(to, from);
				if(ver)
					XmppSetVerify(to, from, XMPP_STATE_VERIFY_PRE);

				it.Format("<db:verify from='%s' to='%s' id='' type='%s' />", to, from, ver ? "valid" : "invalid");
				SendData(wel, it);
			}
		}

		return ;
	}

	void SendData(storm_work_el &wel, VString line){
		print(HLString()+" >>>>> (" + xmpp_state + ") " + line + "\r\n\r\n");
		wel.send(line);
		return ;
	} 

	static int XmppConnect(VString mydomain, VString domain, listen_xml2s *con = 0, VString verify_code = VString());

	static MString GetVerifyCode(VString domain);

	static MString XmppGenerateVerifyCode(VString from, VString to){
		MString ret;
		ret.Add(md5h(ret.Add(_xmpp_verify_uniq_string, from, _xmpp_verify_uniq_string, to, _xmpp_verify_uniq_string)), md5h(ret.Add(to, from, "zverus", _xmpp_verify_uniq_string)));
		return ret;
	}

	static int XmppIsConnection(VString from, VString to){
		UGLOCK(storm_core_list);

		listen_xml2s *p = 0;
		while(p = storm_core_list.Next(p)){
			if(p->from == from && p->to == to)
				return 1;
		}

		return 0;
	}

	static int XmppIsVerifyDomain(VString from, VString to){
		UGLOCK(storm_core_list);

		listen_xml2s *p = 0;
		while(p = storm_core_list.Next(p)){
			if(p->from == from && p->to == to && p->xmpp_verify)
				return 1;
		}

		return 0;
	}

	static int XmppSetVerify(VString from, VString to, int st, listen_xml2s *con = 0){
		UGLOCK(storm_core_list);

		listen_xml2s *p = 0;
		while(p = storm_core_list.Next(p)){
			if(p->from == from && p->to == to && (!con || p->xmpp_veryfy_con == con)){
				if(st == XMPP_STATE_VERIFY_UP){
					if(p->xmpp_verify == XMPP_STATE_VERIFY_PRE)
						p->xmpp_verify = XMPP_STATE_VERIFY_OK;
				}else
					p->xmpp_verify = st;
				return 1;
			}
		}

		return 0;
	}

	static listen_xml2s* XmppSend(VString from, VString to, VString line, listen_xml2s *con = 0){
		UGLOCK(storm_core_list);

		listen_xml2s *p = 0;
		while(p = storm_core_list.Next(p)){
			if(p->from == from && p->to == to && (!con || p == con)){
				p->sock.SendData(line);
				return p;
			}
		}

		return 0;
	}

	static int XmppSendMessage(VString from, VString to, VString msg){
		Itos it;
		// <message from='mikelsv@jabber.ru/Miranda' to='ololo@zverus.ru' xml:lang='ru' type='chat' id='mir_1681'><body>7</body></message>

		VString fd = PartLineO(PartLineOT(from, "@"), "/");
		VString td = PartLineO(PartLineOT(to, "@"), "/");

		if(XmppIsVerifyDomain(fd, td)){
			UGLOCK(storm_core_list);

			listen_xml2s *p = 0;
			while(p = storm_core_list.Next(p)){
				if(p->from == fd && p->to == td && p->xmpp_verify == XMPP_STATE_VERIFY_OK){
					it.Format("<message from='%s' to='%s' type='chat'><body>%s</body></message>", from, to, msg);
					p->sock.SendData(it);
					print(" *** > ", it, "\r\n");
					return 1;
				}
			}
		} else
			if(!XmppIsConnection(fd, td))
				XmppConnect(fd, td);

		return 0;
	}

	STORM_DEFINE_NEW_DELETE(listen_xml2s);
};

STORM_DEFINE_NEW_DELETE_OK(listen_xml2s);


// type: 0 - clave, 1 - master
int listen_xml2s::XmppConnect(VString from, VString to, listen_xml2s *con, VString verify_code){
	ConIp ip(GetIP(to), 5269);
	SOCKET sock=ip.Connect();
	if(sock){
		UGLOCK(storm_core_list);
		listen_xml2s *xmpp=(listen_xml2s*)listen_xml2s::static_storm_new();
		
		xmpp->xmpp_state = 1;
		xmpp->from = from;
		xmpp->to = to;
		xmpp->xmpp_verify_code = verify_code;

		if(con)
			con->xmpp_veryfy_con = xmpp;

		//xmpp->xmpp_state = 1 + ((type&2) ? 0 : 2 );
		//xmpp->xmpp_type = type;
		//xmpp->xmpp_state = 1;
//		xmpp->con = con;
//		if(con)
//			con->con = xmpp;
		
		//if(type == 1)
		//	XmppVerified.AddConnect(mydomain, domain, xmpp);
		//else
		//	XmppVerified.AddConnect(domain, mydomain, con);

		Itos it;
		it.Format("<?xml version='1.0'?><stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:server' xmlns:db='jabber:server:dialback' from='%s' to='%s' version='1.0'>", from, to);
		send(sock, it, it, 0);
		print(HLString()+" >>>>> (" + 1 + ") " + it.ret + "\r\n\r\n");

		return MyStormCore.StormAddSock(xmpp, sock, 0);
	}

	return 0;
}
