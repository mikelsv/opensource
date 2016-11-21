#define STORMSERVER_HTTP_HEAD 0
#define STORMSERVER_HTTP_POST 1
#define STORMSERVER_HTTP_SEND 2
#define STORMSERVER_HTTP_SENDFILE 3
#define STORMSERVER_HTTP_PROXY 4
#define STORMSERVER_HTTP_ALIVE 5
#define STORMSERVER_HTTP_CLOSE 6

Versions STORMHTTPVER[]={
	// new version to up
	"0.0.0.2", "11.06.2015 07:02",
	"0.0.0.1", "03.06.2015 22:41"
};


#define STORMSERVER_HTTP_MAXPOST S1M

#ifndef USEMSV_CONSOLE
#error Use #define USEMSV_CONSOLE
#endif

#ifndef USEMSV_CONFLINE
#error Use #define USEMSV_CONFLINE
#endif

#include "listen-http-options.h"

class listen_http_file{
public:
	HFILE hfile;
	int64 size;

	listen_http_file(){
		hfile = 0;
		size = 0;
	}

	HFILE open(VString file, int64 sz){
		if(hfile)
			CloseHandle(hfile);

		hfile = CreateFile(file, O_RDONLY, S_IREAD | S_IWRITE);
		if(ishandle(hfile))
			size = sz;
		else{
			size = 0;
			hfile = 0;
		}

		return hfile;
	}

	operator HFILE(){
		return hfile;
	}

	void close(){
		if(hfile){
			CloseHandle(hfile);
			hfile = 0;
		}
	}

	~listen_http_file(){
		if(hfile)
			CloseHandle(hfile);
		hfile = 0;
		size = 0;
	}

};

class listen_http_sendstatus{
public:
	// error
	int error;
	int code;
	VString vcode;
	VString root, url;

	// request data
	VString method;
	//VString head, post;
	ILink ilink;

	// host
	ConfLineOption *host;

	// ip
	unsigned int cip, tip;
	unsigned short cport, tport;

	// options
	int opt_no_cl;

	listen_http_sendstatus(){
		host = 0;
		opt_no_cl = 0;
	}

	// ip
	void gettip(SOCKET sock){
		sockaddr_in from; int fromlen = sizeof(from);

		getsockname(sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

		tip = ntohl(from.sin_addr.s_addr);
		tport = htons(from.sin_port);
	}

	void getcip(SOCKET sock){
		sockaddr_in from; int fromlen = sizeof(from);

		getpeername(sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

		cip = ntohl(from.sin_addr.s_addr);
		cport = htons(from.sin_port);
	}

};

class listen_http : public storm_core_ssl{
protected:
	// status
	int wstat;

	// head & data
	MString head, post;
	HLString0 tls;

	// file
	listen_http_file send_file;

	// socket for proxy
	storm_socket socket;
	static listen_http *static_con;

	virtual void storm_init(){
		wstat = 0;
	}

	virtual void storm_setsock(StormCoreVirtual *core, storm_socket_data *data){
		if(static_con){
			wstat = STORMSERVER_HTTP_PROXY;
			static_con->socket.set(core, data);
		} else {
			// SSL
			ConfLineOption *opt = 0;
			opt = listen_http_options.FindOption("SSLEngine", opt, 0);

			if(opt && opt->val.compareu("on")){
				listen_http_sendstatus status;
				status.gettip(data->sock);

				ConfLineOption *host = listen_http_options.GetSSLHost(status.tip, status.tport);

				VString cert = listen_http_options.GetOptionGlobal("SSLCertificateFile", host);
				VString key = listen_http_options.GetOptionGlobal("SSLCertificateKeyFile", host);

				if(cert && key){
					storm_ssl_accept_file(data->sock, cert, key);
				}
			}
		}

	}

	virtual void storm_start(){
		listen_http_modstate.OnConnect();
		wstat = 0;
		return ;		
	}

	virtual void storm_stop(){
		listen_http_modstate.OnClose();
		socket.CloseSocket();
	}

	//virtual void rain_stop(){}

	virtual void storm_recv(storm_work_el &wel, VString read){
		while(1){
			//print("RECV STATE: ", itos(wstat), "\r\n");

			switch(wstat){
			case STORMSERVER_HTTP_HEAD:
				if(!AnalysHead(wel, read))
					return ;
			break;

			case STORMSERVER_HTTP_POST:
				if(!AnalysPost(wel, read))
					return ;
			break;

			case STORMSERVER_HTTP_SEND:
				if(!AnalysSend(wel))
					return ;
			break;

			case STORMSERVER_HTTP_PROXY:
				if(!socket.SendData(read))
					wel.close();
				wel.readed(read);
				return ;
			break;

			case STORMSERVER_HTTP_SENDFILE:
				return ;
			break;

			case STORMSERVER_HTTP_ALIVE:
				post.Clean();
				if(PartLineDouble(head, "Connection: ", "\r\n").compareu("keep-alive"))
					wstat = STORMSERVER_HTTP_HEAD;
				else
					wstat = STORMSERVER_HTTP_CLOSE;
				msvcore_memcon_print();
			break;

			case STORMSERVER_HTTP_CLOSE:
			default:
				wel.close();
				return ;
			break;
			}
		}
	}

	virtual int storm_send(storm_work_el &wel, VString send){
		if(wstat == STORMSERVER_HTTP_SENDFILE){
			//print("SEND STATE: ", itos(wstat), "\r\n");
			if(send_file.size){
				int rd = ReadFile(send_file, send, minel(send.sz, send_file.size));
				send_file.size -= rd;

				if(rd <= 0 || !send_file.size){
					wstat = STORMSERVER_HTTP_ALIVE;
					send_file.close();
					wel.eread();
				}

				return rd;
			}
			else{
				if(PartLineDouble(head, "Connection: ", "\r\n").compareu("keep-alive")){
					post.Clean();
					wstat = STORMSERVER_HTTP_HEAD;
				}else{
					wstat = STORMSERVER_HTTP_CLOSE;
					wel.close();
				}
				//wstat = STORMSERVER_HTTP_ALIVE;
				wel.eread();
			}
		}

		return 0;
	}

	virtual int AnalysHead(storm_work_el &wel, VString &read){
		VString h, data;
		h = PartLine(read, data, "\r\n\r\n");

		if(!data.data){
			if(read.sz > S16K)
				wel.close();
			return 0;
		}

		if(data.data){
			head.setu(h, h.sz + 4);
			wel.readed(read, head);

			h = PartLineO(head, " ");
			if(h == "POST")
				wstat = STORMSERVER_HTTP_POST;
			else
				wstat = STORMSERVER_HTTP_SEND;

			tls.Clear();
		}

		return 1;
	}

	virtual int AnalysPost(storm_work_el &wel, VString &read){
		VString cl = PartLineDoubleUp(head, "Content-Length: ", "\r\n");
		unsigned int cls = cl.toui(), m = (cls - tls.size()) < read.sz ? (cls - tls.size()) : read.sz;

		if(cls > STORMSERVER_HTTP_MAXPOST){
			// get config
			listen_http_sendstatus status;
			wel.gettip(status.tip, status.tport);

			status.host = listen_http_options.GetHost(PartLineDouble(head, "Host: ", "\r\n"), status.tip, status.tport);
			
			status.code = 413;
			status.vcode = "Request Entity Too Large";
			SendHeader(wel, status, "", 0);
			wstat = STORMSERVER_HTTP_ALIVE;
			return 1;
		}
		
		if(m + tls.size() == cls){
			wstat = STORMSERVER_HTTP_SEND;
			post.Add(VString(tls.oneline(), tls.size()), read.str(0, m));
			wel.readed(read, m);
			tls.Clear();
			return 1;
		} else{
			tls + read.str(0, m);
			wel.readed(read, m);
			return 0;
		}		

		return 0;
	}

	virtual int AnalysSendInt(storm_work_el &wel, listen_http_sendstatus &status){
		// Use it:
		//SString s;
		//s.Format("Content-type: %s; charset=%s\r\n", "text/html", "UTF-8");
		//
		//VString data = "Hello World!";
		//
		//SendHeader(wel, status, s, data);
		//wel.send(data);
		//return 1;

		return 0;
	}

	virtual int AnalysSend(storm_work_el &wel){
		//VString rootpath="http/";
		//ILink ilink; ilink.Link(path, 1);

		listen_http_modstate.OnHttpRequest();
		
		// file
		VString url = PartLineDouble(head, " ", " ");
		Itos it, it2, iturl, itroot;
		sstat64 stt;

		// status
		listen_http_sendstatus status;
		wel.getcip(status.cip, status.cport);
		wel.gettip(status.tip, status.tport);

		// error code
		status.error = 0;
		status.code = 200;
		status.vcode = "OK";

		ILink &ilink = status.ilink;

		// host
		//ConfLineOption *
			status.host = listen_http_options.GetHost(PartLineDouble(head, "Host: ", "\r\n"), status.tip, status.tport);

		// test method
		VString &method = status.method = PartLineO(head, " ");

		if(method != "GET" && method != "POST" && method != "HEAD" && method != "CONNECT"){
			status.code = 404;
			status.vcode = "Not Found";

			SendHeader(wel, status, "", 13);
			wel.send("404 Not Found");
			//AnalysSendWriteLog(wel, host, 404, it.ret.sz + 13);
			return 1;
		}

		// ifproxy
		if(VString xip = PartLineDouble(head, "X-Forwarded-For: ", "\r\n")){
			ConfLineOption *opt = 0;

			while(opt = listen_http_options.FindOption("EnableProxy", opt, status.host)){
				if(ipstoi(opt->val) == status.cip){
					status.cip = ipstoi(xip);
					break;
				}
			}

			if(!opt){
				status.error = 1;
				status.code = 400;
				status.vcode = "Bad Request";
				SendHeader(wel, status, "", status.vcode);
				wel.send(status.vcode);
				return 1;
			}
		}

		// dirindex & rootpath
		VString rootpath = listen_http_options.GetOptionGlobal("DocumentRoot", status.host);
		VString dirindex = listen_http_options.GetOptionGlobal("DirectoryIndex", status.host);

		// mod state
		if(listen_http_modstate.OnOff()){
			VString modstate = listen_http_options.GetOptionGlobal("ModStateRule", status.host);
			if(modstate == PartLineO(url.str(1), "?")){
				Itos ithead;
				it = listen_http_modstate.GetState(url, ithead);
				SendHeader(wel, status, ithead, it);
				wel.send(it);
				return 1;
			}
		}

		if(!rootpath)
			rootpath = _procpath;

#ifdef USEMSV_PCRE
		//RewriteEngine
		if(listen_http_options.GetOptionGlobal("RewriteEngine", status.host).compareu("on")){
			ilink.Link(url);

			// rewrite
			ConfLineOption *rw_host = status.host;
			ConfLineOption *opt = 0;

			// match
			pcre2_match_data *match_data = pcre2_match_data_create(100, NULL);
			size_t *ovector = pcre2_get_ovector_pointer(match_data);
			int cond = 1;

			while(1){
				opt = listen_http_options.FindOption2("RewriteRule", "RewriteCond", opt, rw_host);

				if(!opt && rw_host){
					opt = listen_http_options.FindOption2("RewriteRule", "RewriteCond", 0, rw_host);
					rw_host = 0;
				}

				if(!opt)
					break;

				if(opt->key == "RewriteCond"){
					if(!cond)
						continue;

					if(opt->val == "%{REQUEST_FILENAME} !-s"){
						// badcode
						ILink ilink;
						Itos it;						
						it = url;

		if(rootpath[0] != '/' && rootpath[1] != ':' && rootpath[0] != '\\'){
			ilink.Link(it2.Add(_procpath, "/", rootpath, "/", NormalPath(it)), 1);
		} else{
			ilink.Link(it2.Add(rootpath, "/", NormalPath(it)), 1);
		}

		stt = GetFileInfo(ilink.GetPathFile());


		cond = !(stt.st_size && !(stt.st_mode & S_IFDIR));

						// very bad code

					}
					continue;
				} else
					if(!cond){
						cond = 1;
						continue;
					}

				// init
				VString rule, result, options;
				int perrcode; size_t erroffset;
				int opt_l = 0, opt_qsa = 0;
				int arr[255];

				VString rurl = ilink.GetPathFile().str(1);
				rule = PartLine(opt->val, result, " ");
				result = PartLine(result, options, " ");
				
				// compile
				pcre2_code *p = pcre2_compile(rule, rule, 0, &perrcode, &erroffset, 0);				
				// find
//				int f = pcre2_match(p, rurl, rurl, 0, 0, arr, 255);
				int f = pcre2_match(p, rurl, rurl, 0, 0, match_data, 0);

				// found
				if(f > 0){
					// options
					if(options[0] == '[' && options.endo() == ']'){
						options = options.str(1, -2);
					
						while(options){
							VString k = PartLine(options, options, ",");
							if(k == "L" || k == "last")
								opt_l = 1;
							else if(k == "SQA" || k == "qsappend")
								opt_qsa = 1;
						}
					}

					it.Reserv(prmf_pcre_result(0, 0, result, result, rurl, rule, ovector, f, opt_qsa, ilink.iquest));
					prmf_pcre_result(it, it, result, result, rurl, rule, ovector, f, opt_qsa, ilink.iquest);
					ilink.Link(url = it);
				}

				pcre2_code_free(p);

				if(opt_l)
					break;				
			}

			pcre2_match_data_free(match_data);
		}
#endif

		// proxy
		if(listen_http_options.GetOptionGlobal("ProxyRequests", status.host).compareu("on")){
			ConfLineOption *t_host = status.host;
			ConfLineOption *opt = 0;
			VString key, val;

			while(1){
				opt = listen_http_options.FindOption("ProxyPass", opt, t_host);

				if(!opt && t_host){
					opt = listen_http_options.FindOption("ProxyPass", 0, t_host);
					t_host = 0;
				}

				if(!opt)
					break;

				// proxy it
				key = listen_http_options.GetKeyVal(opt->val, val);
				if(url.incompare(key)){
					it.Add(val, url.str(key));
					ilink.Link(it);

					status.code=299;
					AnalysSendWriteLog(wel, status, 0);

					// connect
					ConIp ip(ilink.domain);
					ip.port = ilink.port ? ilink.port : 80;
					ip.SetNonBlock();

					SOCKET sock = ip.Connect();
					
					// new http
					UGLOCK(storm_items_list);

					listen_http *con = (listen_http*)static_storm_new();
					con->storm_init();
					con->socket = wel;
					static_con = this;

					MyStormCore.StormAddSock(con, sock, 0);
					static_con = 0;

					HLString ls;
					ls + PartLineO(head, " ") + " " + ilink.GetPathFileQuest() + " HTTP/1.1" "\r\n"
						+ "Host: " + ilink.domain;
					if(ilink.port)
						ls + ":" + ilink.port;
						
					ls + "\r\n"
						+ "X-Forwarded-For: " + it.Format("%ip!", status.cip) + "\r\n"
						+ "Connection: close" "\r\n";

					key = head;
					while(key){
						val = PartLine(key, key, "\r\n");
						if(val && !(val.incompareu("Host:") || val.incompareu("X-Forwarded-For:") || val.incompareu("Connection:")  ))
							ls + val + "\r\n";
					}

					ls + "\r\n";

					socket.SendData(VString(ls.OneLine(), ls.Size()));
					if(post)
						socket.SendData(post);
					
					wstat = STORMSERVER_HTTP_PROXY;
					return 1;
				}
			}
		}


		// print
		//print("Request head: ", head, "\r\n\r\n");

		it = url;

		if(rootpath[0] != '/' && rootpath[1] != ':' && rootpath[0] != '\\'){
			itroot.Add(_procpath, "/", rootpath);
			itroot.Add(NormalPath(itroot), VString());
			ilink.Link(it2.Add(/*_procpath, "/", rootpath,*/ itroot, "/", NormalPath(it)), 1);
		} else{
			itroot = rootpath;
			ilink.Link(it2.Add(rootpath, "/", NormalPath(it)), 1);
		}

		status.root = itroot;

		stt = GetFileInfo(ilink.GetPathFile());

//		if(!stt.st_mode){
//#ifdef WIN32
//			if(access((VString)it, 0)==0){
//				if(!ilink.file)
//					errorcode = 301;
//			} else
//#endif
//				errorcode = 404;
//		}

		if(stt.st_mode & _S_IFDIR && ilink.GetPathFile().endo() != '/')
			status.code=301;
		else
		if((!stt.st_mode || (stt.st_mode&S_IFDIR)) && !ilink.file){
			while(dirindex){
				VString ind = PartLine(dirindex, dirindex, " ");
				if(!ind)
					continue;

				ilink.SetFile(ind);
				stt = GetFileInfo(ilink.GetPathFile());

				if(stt.st_mode && !(stt.st_mode&S_IFDIR))
					break;
			}
		}

		if(!stt.st_mode){
			status.code = 404;
			status.vcode = "Not Found";

			SendHeader(wel, status, "", 13);
			wel.send("404 Not Found");
			//AnalysSendWriteLog(wel, host, 404, it.ret.sz + 13);
			return 1;
		}

		if(status.code == 301){
			status.vcode = "Moved Permanently";
			ilink.Link(url);

			it.Format("Location: %s%s/\r\n"
			, ilink.path, ilink.file
			);

			SendHeader(wel, status, it, 0);
			return 1;
		}

		status.url = url;
		if(AnalysSendInt(wel, status))
			return 1;

		// extension
		ConfLineOption *ext = listen_http_options.GetExtension(ilink.ext());
		if(ext){
			status.url = url;
			return AnalysSendExtension(wel, ext, ilink, status);
		}

		//MString data = LoadFile(ilink.GetFullPath());
		MTime mt;

		//VString errorstring = "200 OK";

		if(stt.st_size){
			if(!send_file.open(ilink.GetPathFile(), stt.st_size)){
				status.code = 404;
				status.vcode = "Not Found";

				SendHeader(wel, status, "", 13);
				wel.send("404 Not Found");
				return 0;
			}
		}

		// Range
		Itos itrange;
		if(VString range = PartLineDouble(head, "Range: ", "\r\n")){
			VString to;
			range =  PartLine(PartLineOT(PartLineO(range, ","), "bytes="), to, "-");
			int64 r = stoi64(range);
			int64 t = stoi64(to);

			if(t <= 0 || t > stt.st_size)
				t = stt.st_size;
			if(r < 0 || r >= stt.st_size)
				r = stt.st_size;

			SetFilePointer(send_file, r);

			send_file.size = t - r;
			itrange.Format("Content-Range: bytes %d-%d/%d\r\n", r, t - 1, (int64)stt.st_size);

			status.code = 206;
			status.vcode = "Partial Content";
		}

		VString type = listen_http_options.GetOptionGlobal("DefaultCharset", ext);
		Itos itct;
		itct.Format("Content-type: %s; charset=%s\r\n"
			, ilink.ext() == "html" ? "text/html" : ""
			, type ? type : "UTF-8"
		);

		it.Format("Last-Modified: %s\r\n"
			"%s"
			"%s"
			, mt.date("D, d M Y H:i:s \\G\\M\\T", (unsigned int)stt.st_mtime, 1)
			, itrange.ret
			, itct.ret
		);

		SendHeader(wel, status, it, send_file.size);
		//wel.send(data);

		return 1;
	}

	void SendHeader(storm_work_el &wel, listen_http_sendstatus &status, VString sendhead, int64 datasize){ // ConfLineOption *host, 
		Itos it, it2;
		MTime mt;

		// alive
		VString alive = PartLineDouble(head, "Connection: ", "\r\n");
		if(alive.compareu("keep-alive"))
			alive = "Connection: Keep-Alive\r\n";
		else
			alive.Clean();

		if(!status.opt_no_cl)
			it2.Format("Content-Length: %d\r\n", datasize);

		it.Format("HTTP/1.1 %d %s\r\n"
			"Server: StormServer/%s(%s)\r\n"
			"Date: %s\r\n"
			"%s"
			"%s"
			"%s"
			"\r\n"
			, status.code, status.vcode
			, STORMHTTPVER[0].ver, stormserver_poll
			, mt.date("D, d M Y H:i:s \\G\\M\\T", time(), 1)
			, sendhead
			, alive
			, it2.ret
			);

		wel.send(it);
		//wel.send(data);

		AnalysSendWriteLog(wel, status, it.ret.sz + datasize); // host, 
		wstat = STORMSERVER_HTTP_SENDFILE;
		return ;
	}

	void AnalysSendWriteLog(storm_work_el &wel, listen_http_sendstatus &status, int64 ssize){ // ConfLineOption *host, 
		Itos it;
		MTime mt;

		VString logpath = listen_http_options.GetOptionGlobal("AccessLog", status.host);
		if(logpath){
			it.Format("%ip! [%s] \"%s\" \"%s\" %d %d\r\n", status.cip, mt.date("d/M/Y:H:i:s"), !status.host ? PartLineDouble(head, "Host: ", "\r\n") : VString(), PartLineO(head, "\r\n"), status.code, ssize);
			SaveFileAppend(logpath, it);
		}

		logpath = listen_http_options.GetOptionGlobal("FullLog", status.host);
		if(logpath){
			it.Format("%ip! [%s] %d %d\r\n%s", status.cip, mt.date("d/M/Y:H:i:s"), status.code, ssize, head);
			SaveFileAppend(logpath, it);
		}
		
		return ;
	}

	VString SafeCGI(VString line){
		return PartLineO(line, VString("\0", 1));
	}

	virtual int AnalysSendExtension(storm_work_el &wel, ConfLineOption *ext, ILink &ilink, listen_http_sendstatus &status){ // ConfLineOption *host, 
		Itos it;
		VString  _rc("\0", 1);

		VString exec = listen_http_options.GetOption("Execute", ext);
		VString method = PartLineO(head, " ");

		if(ext->val == "cgi" && exec){

			// environments
			HLString env;

			// http
			env + "HTTP_HOST=" + SafeCGI(PartLineDouble(head, "Host: ", "\r\n")) + _rc;
			env + "HTTP_USER_AGENT=" + SafeCGI(PartLineDouble(head, "User-Agent: ", "\r\n")) + _rc;
			env + "HTTP_ACCEPT=" + SafeCGI(PartLineDouble(head, "Accept: ", "\r\n")) + _rc;
			env + "HTTP_ACCEPT_LANGUAGE=" + SafeCGI(PartLineDouble(head, "Accept-Language: ", "\r\n")) + _rc;
			env + "HTTP_ACCEPT_ENCODING=" + SafeCGI(PartLineDouble(head, "Accept-Encoding: ", "\r\n")) + _rc;
			env + "HTTP_ACCEPT_CHARSET=" + SafeCGI(PartLineDouble(head, "Accept-Charset: ", "\r\n")) + _rc;
			//env + "HTTP_KEEP_ALIVE=" + SafeCGI(PartLineDouble(head, "Keep-Alive: ", "\r\n")) + _rc;
			env + "HTTP_CONNECTION=" + SafeCGI(PartLineDouble(head, "Connection: ", "\r\n")) + _rc;
			env + "HTTP_REFERER=" + SafeCGI(PartLineDouble(head, "Referrer: ", "\r\n")) + _rc;
			env + "HTTP_COOKIE=" + SafeCGI(PartLineDouble(head, "Cookie: ", "\r\n")) + _rc;

			// server
			//wel.gettip(ip, port);
			env + "SERVER_SOFTWARE=" + "StormServer" + _rc;
			env + "SERVER_NAME=" + "StormServer" + _rc;
			env + "SERVER_ADDR=" + it.ipitos(status.tip) + _rc;
			env + "SERVER_PORT=" + status.tport + _rc;

			// remote
			//wel.getcip(ip, port);
			env + "REMOTE_ADDR=" + it.ipitos(status.cip) + _rc;
			env + "REMOTE_PORT=" + status.cport + _rc;

			// document
			env + "DOCUMENT_ROOT=" + SafeCGI(status.root) + _rc;
			env + "SCRIPT_FILENAME=" + SafeCGI(ilink.GetOnlyPath()) + _rc;
			env + "REQUEST_METHOD=" + SafeCGI(method) + _rc;
			env + "QUERY_STRING=" + SafeCGI(ilink.iquest) + _rc;
			env + "REQUEST_URI=" + SafeCGI(status.url) + _rc;
			//if(ilink.iquest)
			//	env + "?" + SafeCGI(ilink.iquest);
			//env + _rc;
			env + "SCRIPT_NAME=" + PartLineO(SafeCGI(status.url), "?") + _rc; // + SafeCGI(ilink.file) + _rc;
			env + "PHP_SELF=" + PartLineO(SafeCGI(status.url), "?") + _rc; //;SafeCGI(ilink.file) + _rc;


			// Gateway
			env + "GATEWAY_INTERFACE=CGI/1.1" + _rc;
			env + "SERVER_PROTOCOL=HTTP/1.0" + _rc;
			env + "REDIRECT_STATUS=200" + _rc;

			env + "SystemRoot=" + windows_directory + _rc;

			if(method == "POST"){
				env + "CONTENT_TYPE=application/x-www-form-urlencoded" + _rc;
				env + "CONTENT_LENGTH=" + post.size() + _rc;
			}

			env + _rc;

			// go
			PipeLine ppl;
			HLString hls, ehls;
			int rc;

			// exec
			ppl.SetEnv(VString(env.oneline(), env.size()));
			ppl.SetInput(post);
			int run = ppl.Run(exec, rc, hls, ehls, 0);

			// results
			VString ret, rhead, rbody;
			ret=VString(hls.oneline(), hls.size());
			rhead=PartLine(ret, rbody, "\r\n\r\n");
			if(!rbody && !rhead.incompare("Status: ")){
				rbody = rhead;
				rhead.Clean();
			}

			if(ehls)
				print("PHP Error: ", VString(ehls.oneline(), ehls.size()), "\r\n");

			if(rhead.incompare("Status: ")){
				status.code = rhead.str(8).toi();
				status.vcode = PartLineDouble(rhead.str(8), " ", "\r\n");
			}

			it.Format("%s%s"
				, rhead, rhead ? "\r\n" : ""
			);

			if(!rbody && !run){
				status.code = 500;
				status.vcode = "Server Error";
			}// else
			//	status.opt_no_cl = 1;

			SendHeader(wel, status, it, run ? rbody.sz : 16);
			if(run)
				wel.send(rbody);
			else
				wel.send("500 Server Error");

			//it.Format("HTTP/1.1 %s\r\n"
			//	"Server: StormServer(%s)\r\n"
			//	"Content-Length: %i\r\n"
			//	"%s%s"
			//	"\r\n"
			//	, "HTTP/1.0 200 OK"
			//	, stormserver_poll
			//	, rbody.sz
			//	, rhead, rhead ? "\r\n" : ""
			//	);

			//wel.send(it);
			//wel.send(rbody);
			//wstat = STORMSERVER_HTTP_CLOSE;

			//AnalysSendWriteLog(wel, host, 200, it.ret.sz + rbody.sz);
			return 1;
		}
#ifdef USEMSV_MSL_FL
		else if(ext->val == "msl"){
			msl_fl msl;

			// input
			msl.SetHttpCookie(PartLineDouble(head, "Cookie: ", "\r\n"));
			msl.SetHttpGet(ilink.iquest);
			if(method == "POST")
				msl.SetHttpPost(post);
			
			// input ip
			//wel.getcip(status.cip, port);
			msl.SetValue("_SERVER", "")->Set("REMOTE_ADDR", ipitos(status.cip));
			msl.SetValue("_SERVER", "")->Set("REMOTE_ADDR_INT", itos(status.cip));

			// do
			msl.DoFile(ilink.GetPathFile());
			MString out = msl.GetOutput();

			VString type = listen_http_options.GetOptionGlobal("DefaultCharset", ext);

			it.Format("Content-type: text/html; charset=%s\r\n"
				, type ? type : "UTF-8"
			);

			SendHeader(wel, status, it, out);
			wel.send(out);

			return 1;
		}
#endif

		status.code = 500;
		status.vcode = "Server Error";
		SendHeader(wel, status, "", 16);
		wel.send("500 Server Error");

		return 1;
	}

	int CloseConnection(storm_work_el &wel, listen_http_sendstatus &status, VString data){
		SString s;
		s.Format("Content-type: %s; charset=%s\r\n", "text/html", "UTF-8");
		
		SendHeader(wel, status, s, data);
		wel.send(data);

		return 1;
	}

	int CloseConnection(storm_work_el &wel, VString data){
		HLString ls;

		ls + "HTTP/1.0 200 OK\r\n"
		"Server: Rain\r\n"
		"Content-Length: " + data.sz + "\r\n"
		"\r\n"
		+ data;

		wel.send(VString(ls.OneLine(), ls.Size()));

		wstat = STORMSERVER_HTTP_SENDFILE;

		//wel.close();
		return 1;
	}

	STORM_DEFINE_NEW_DELETE(listen_http);
};

STORM_DEFINE_NEW_DELETE_OK(listen_http);

listen_http *listen_http::static_con = 0;