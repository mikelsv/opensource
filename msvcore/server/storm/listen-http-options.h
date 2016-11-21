//#define listen_http_option ConfLineOption

class _listen_http_options : public ConfLineOptions{
	
public:
	_listen_http_options(){
		LoadFile(STORMSERVER_CONFIG);
		Analys();
	}

	void Analys(){
		ConfLineOption *opt = GetFirst();

		while(opt){
			if(opt->key == "AccessLog" || opt->key == "FullLog"){
				ILink il;
				il.Link(opt->val);

				MkDir(il.GetProtoDomainPath());
			}
			else
				if(opt->key == "ModState")
					listen_http_modstate.OnOff(opt->val.compareu("On"));
			

			opt = opt->next;
		}

		return ;
	}

	VString GetOption(VString name, ConfLineOption *parent){
		ConfLineOption *opt = FindOption(name, 0, parent);
		if(opt)
			return opt->val;
		return VString();
	}

	VString GetOptionGlobal(VString name, ConfLineOption *parent){
		ConfLineOption *opt = FindOption(name, 0, parent);
		if(opt)
			return opt->val;

		if(parent){
			opt = FindOption(name, 0);
			if(opt)
				return opt->val;
		}

		return VString();
	}

	ConfLineOption* GetHost(VString host, unsigned int ip, unsigned short port){
		Itos it;

		ConfLineOption *opt = 0, *hopt;
		host = PartLineO(host, ":");

		while(opt = FindOption("VirtualHost", opt)){
			if(!opt->up)
				continue;

			int hostok = 0, portok = -1;
			hopt = 0;

			while(hopt = FindOption("ServerName", hopt, opt)){
				if(hopt->val == host){
					hostok = 1;
					break;
				}
			}

			if(!hostok){
				hopt = 0;

				while(hopt = FindOption("ServerAlias", hopt, opt)){
					if(hopt->val == host){
						hostok = 1;
						break;
					}
				}
			}

			if(!hostok)
				continue;

			hopt = opt->up;
			while(hopt = FindOption("ServerPort", hopt, opt)){
				VString vip, vport;
				vip = PartLine(hopt->val, vport, ":");
				if(!vport){
					vport = vip;
					vip.Clean();
				}

				if((vip == "*" || vip == it.ipitos(ip)) && (vport == "*" || vport == it.itos(port))){
					portok = 1;
					break;
				}
				else
					portok = 0;
			}

			if(!portok)
				continue;

			return opt;
		}

		return 0;
	}

	ConfLineOption* GetSSLHost(unsigned int ip, unsigned short port){
		Itos it;

		ConfLineOption *opt = 0/*, *hopt*/;
		VString o, t;

		while(opt = FindOption("SSLHost", opt)){
			if(!opt->up)
				continue;

			o = PartLine(opt->val, t, ":");
			if(!t){
				t = o;
				o.Clean();
			}

			if((!o || o == "*" || o == it.ipitos(ip)) && ( !t || t == "*" || t == it.itos(port)))
				return opt;
		}

		return 0;
	}

	ConfLineOption* GetExtension(VString ext){
		ConfLineOption *opt = 0, *hopt;

		while(opt = FindOption("Extension", opt)){
			if(!opt->up)
				continue;

			//int typeok = 0;
			hopt = 0;

			while(hopt = FindOption("UseType", hopt, opt)){
				VString key, val = hopt->val;
				while(val){
					key = GetKeyVal(val, val);

					if(key == ext){
						//typeok = 1;
						return opt;
					}
				}
			}			
		}

		return 0;
	}

} listen_http_options;




// Deleted
//class listen_http_option{
//public:
//	VString key, val;
//	listen_http_option *next, *up;
//};

/*

class _listen_http_options2 : public TLock{
	HLString confdata;
	listen_http_option *options; 

public:
	_listen_http_options2(){
		Init();
	}

	VString GetOption(VString name, listen_http_option *parent){
		listen_http_option *opt = FindOption(name, parent ? parent->up : 0);
		if(opt)
			return opt->val;
		return VString();
	}

	VString GetOptionGlobal(VString name, listen_http_option *parent){
		listen_http_option *opt = FindOption(name, parent ? parent->up : 0);
		if(opt)
			return opt->val;

		if(parent){
			opt = FindOption(name, 0);
			if(opt)
				return opt->val;
		}

		return VString();
	}

	listen_http_option* GetHost(VString host, unsigned int ip, unsigned short port){
		 Itos it;

		 listen_http_option *opt = 0, *hopt;
		 host = PartLineO(host, ":");

		 while(opt = FindOption("VirtualHost", opt)){
			 if(!opt->up)
				 continue;

			 int hostok = 0, portok = -1;
			 hopt = 0;

			  while(hopt = FindOption("ServerName", hopt, opt)){
				  if(hopt->val == host){
					  hostok = 1;
					  break;
				  }
			  }

			  if(!hostok){
				  hopt = 0;

				  while(hopt = FindOption("ServerAlias", hopt, opt)){
					  if(hopt->val == host){
						  hostok = 1;
						  break;
					  }
				  }
			  }

			  if(!hostok)
				  continue;

			hopt = opt->up;
			while(hopt = FindOption("ServerPort", hopt, opt)){
				VString vip, vport;
				vip = PartLine(hopt->val, vport, ":");
				if(!vport){
					vport = vip;
					vip.Clean();
				}

				if(vip == "*" || vip == it.ipitos(ip) && vport == "*" && vport == it.itos(port)){
					portok = 1;
					break;
				}
				else
					portok = 0;
			}

			if(!portok)
				continue;

			return opt;
		 }

		return 0;
	}

	listen_http_option* GetExtension(VString ext){
		listen_http_option *opt = 0, *hopt;

		while(opt = FindOption("Extension", opt)){
			if(!opt->up)
				continue;

			//int typeok = 0;
			hopt = 0;

			while(hopt = FindOption("UseType", hopt, opt)){
				VString key, val = hopt->val;
				while(val){
					key = GetKeyVal(val, val);

					if(key == ext){
						//typeok = 1;
						return opt;
					}
				}
			}			
		}

		return 0;
	}

	listen_http_option* FindOption(VString name, listen_http_option *opt, listen_http_option *parent = 0){
		if(opt)			
			opt = opt->next;
		else
			if(parent)
				opt = parent->up;
			else
				opt = options;

		for(opt; opt; opt = opt->next){
			if(opt->key == name)
				return opt;
		}

		return 0;
	}

	listen_http_option* FindOption2(VString name, VString name2, listen_http_option *opt, listen_http_option *parent = 0){
		if(opt)			
			opt = opt->next;
		else
			if(parent)
				opt = parent->up;
			else
				opt = options;

		for(opt; opt; opt = opt->next){
			if(opt->key == name || opt->key == name2)
				return opt;
		}

		return 0;
	}

	VString DelSpace(VString line){
		// first
		while(line[0] == ' ' || line[0] == '\t'){
			line.data++; line.sz--;
		}

		// end
		while(line.endo() == ' ' || line.endo() == '\t' || line.endo() == '\r'){
			line.sz--;
		}

		return line;
	}

	VString GetKeyVal(VString line, VString &val){
		line = DelSpace(line);
		
		for(int i = 0; i < line.sz; i++){
			if(line[i] == ' ' || line[i] == '\t'){
				val = DelSpace(line.str(i));
				return line.str(0, i);;
			}
		}

		val.Clean();
		return line;
	}


	listen_http_option* NewOption(){
		return (listen_http_option*)confdata.Add(0, sizeof(listen_http_option), 1);
	}

	VString NewOptionVal(VString line){
		return confdata.addnfr(line);
	}

protected:

	void Init(){
		options = 0;

		MString conf = LoadFile(STORMSERVER_CONFIG);
		VString line = conf;
		int lnum = 0;
		InitRead(line, options, lnum);
		InitAnalys();
	}

	void InitRead(VString &data, listen_http_option *parent, int &lnum, int level = 0){
		listen_http_option *lopt = 0;

		while(data){
			VString line, key, val;
			
			 line = PartLine(data, data, "\n");
			 key = GetKeyVal(line, val);
			 lnum++;

			 int up = 0, down = 0;

			 // up
			 if(val && val.endo() == '{'){
				 val.sz--; up = 1;
			 }else if(!val && key.endo() == '{'){
				 key.sz--; up = 1;
			 }

			 // down
			 if(key[0] == '}'){
				 if(level <= 0){
					 Error("found } prevous {", lnum);
				 }
				 return ;
			 }

			 if(!key){
				 if(up)
					 InitRead(data, lopt, lnum, level + 1);
				 continue;
			 }

			 if(lopt)
				 lopt = lopt->next = NewOption();
			 else
				 if(parent)
					 parent->up = lopt = NewOption();
				 else
					 options =  lopt = NewOption();

			 lopt->up = 0;
			 lopt->key = NewOptionVal(key);
			 lopt->val = NewOptionVal(val);

			  if(up)
				InitRead(data, lopt, lnum, level + 1);

			  // continue;
		}

		if(!data && level){
			Error("not found }", lnum);
		}
	}

	void InitAnalys(){
		listen_http_option *opt = options;

		while(opt){
			if(opt->key == "AccessLog" || opt->key == "FullLog"){
				ILink il;
				il.Link(opt->val);

				MkDir(il.GetProtoDomainPath());
			}

			opt = opt->next;
		}

		return ;
	}

	void Error(VString line, int lnum){
		Itos it;
		it.Add("Error in config file: '", line, "' on line ", itos(lnum+1), ".\r\n");
		print(it);
	}


} listen_http_options2;
*/