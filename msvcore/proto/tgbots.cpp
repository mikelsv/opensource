Versions TGBOT_VER[]={
	"0.0.0.2", "26.11.2015 22:37",
	"0.0.0.1", "24.11.2015 22:19"
};

class TgBots;

class TgBot{
public:
	TgBot *_p, *_n;

	// Bots class
	TgBots *bots;

	// data
	VString id, token, name, idr, keywords;
	MString data;

	// config
	ConfLine conf;

	// Load
	void Load(VString file, TgBots *pbots){
		conf.Load(file);
		bots = pbots;

		SetId(PartLineO(conf["token"], ":"), conf["token"], "", conf["keywords"]);

		return ;
	}

	// Get
	VString GetBotsCertCert();
	VString GetBotsServAddr();

	// Find
	int FindFound(VString line){
		return line == idr;
	}

	// Set
	void SetId(VString nid, VString ntoken, VString nname, VString nkeywords){
		SString tmp;

		id = nid;
		token = ntoken;
		name = nname;
		idr = tmp.Format("%s_%d", id, msrand());
		keywords = nkeywords;

		data.AddR(id, token, name, idr, keywords);

		return ;
	}

	// Test bot token
	int TestBot(){
		GetHttp gp;
		XDataCont ct;
		XDataPEl el;

		// http request
		gp.Request(HLString() + "https://api.telegram.org/bot" + token + "/getMe");

		// Parse json
		ct.Read(gp.GetData());
		el = ct;

		if(el["ok"] != "true"){
			print("Token error: ", el["description"], "\r\n");
			return 0;
		}

		//{"ok":true,"result":{"id":163124850,"first_name":"LivestreamTestBot","username":"LivestreamTestBot"}}
		el = el("result");
		print("Token info: id: ", el["id"], ", first_name: ", el["first_name"], ", username: ", el["username"], "\r\n");
		
		return InitBot();
	}

	// Send my server ip and public ssl key to telegram.
	int InitBot(){
		GetHttp gp;
		XDataCont ct;
		XDataPEl el;

		VString cert_key = GetBotsCertCert();// LoadFile("cert/cert_cert.pem");

		// Bot url = serv_addr + bot_id + random integer
		TString bot_url;
		bot_url.Add(GetBotsServAddr(), "tokenid", idr, "x");

		HLString ls;
		TString bnid = md5h(cert_key);
		bnid.Add("--------", bnid);
		gp.AddBoundary(ls, bnid, "url", bot_url);
		gp.AddBoundaryFile(ls, bnid, "certificate", "cert.pem", cert_key);
		gp.AddBoundaryEnd(ls, bnid);
		gp.SetBoundary(bnid, ls.String());

		//gp.SetBoundary("cert.key", cert_key);		
		gp.Request(HLString() + "https://api.telegram.org/bot" + token + "/setWebhook?url=" + bot_url 
			+ "&certificate=cert.key");

		// Parse json
		ct.Read(gp.GetData());
		el = ct;

		if(el["ok"] != "true"){
			print("Token error: ", el["description"], "\r\n");
			return -1;
		}

		return 1;
	}
	
};

class TgBots{
	// global config
	ConfLine config;

	// ssl certificates
	MString cert_cert;
	MString cert_key;

	// bots
	AList<AListAllocOList<TgBot>, TgBot> bots;

	// random text
	TULine botword;
	MString botword_d;

public:
	// ------- Constructor
	TgBots(){

	}

	// ------- Get
	VString GetCertCert(){
		return cert_cert;
	}

	VString GetCertKey(){
		return cert_key;
	}

	VString GetServAddr(){
		return config["serv_addr"];
	}

	TgBot* GetBotByIdr(VString line){
		return bots.Find(line);
	}

	// ------- Load
	int LoadConfig(VString file){
		if(!config.Load(file))
			return 0;

		// Automake ssl certificates
		if(config["cert_auto"].toi()
			&& config["cert_key"] && config["cert_cert"] && config["cert_addr"]
			&& ( !IsFile(config["cert_cert"]) || !IsFile(config["cert_key"]))){
			// Create certificate: to cert/ folder
			// openssl req -x509 -newkey rsa:2048 -new -days 10000 -keyout cert_key.pem -out cert_cert.pem -nodes -subj "/C=RU/L=Moscow/O=Telegram Bot/OU=Telegram Bot/CN=95.143.221.149"
	
			TString tmp, key, cert;
			OpenSSLCreateRSAPrivateKey(key, S2K);
			int s = OpenSSLCreateCACertificate(key, cert, "RU", "Telegram Bot", config["cert_addr"]);
			
			SaveFile(config["cert_key"], key);
			SaveFile(config["cert_cert"], cert);

			cert_cert = cert;
			cert_key = key;
		} else{
			cert_cert = LoadFile(config["cert_cert"]);
			cert_key = LoadFile(config["cert_key"]);
		}

		// Certificates
		if(!IsFile(config["cert_cert"]) || !IsFile(config["cert_key"])){
			print("Error: config files: cert_cert or cert_key not existed.\r\n");
			return 0;
		}

		// Server addr
		if(!config["serv_addr"]){
			print("Error: config serv_addr is empty.\r\n");
			return 0;
		}

		return 1;
	}

	// Load
	int LoadBots(VString path = "bots/"){
		Readdir rd;
		rd.ReadDir(HLString() + path + "*");

		for(int i = 0; i < rd.size(); i ++){
			if(!rd[i].isdir()){
				ConfLine cf(HLString() + path + rd[i].key);

				if(!cf["token"])
					continue;

				TgBot *p = bots.New();
				p->Load(HLString() + path + rd[i].key, this);

				p->TestBot();
			}
		}

		return 1;
	}

	// ------- Calls
	int Call(VString head, VString post){
		// Get url
		VString url = PartLineDouble(head, " ", " ");
		
		// Get bot id
		VString idr = PartLineDouble(url, "tokenid", "x");
		TgBot *bot = GetBotByIdr(idr);
		if(!bot)
			return 0;
		
		XDataCont ct(post);
		XDataPEl message = ct("message");
		XDataPEl chat = message("chat");

		VString text = message["text"];
		VString chat_id = chat["id"];

		if(message && text){
			text = Replace(text, "\\/", "/");
			print("Message from: ", chat_id, ", text: ", text, "\r\n");

			if(text[0] == '/')
				CallCmd(bot, chat_id, text.str(1));
			else
				return CallMsg(bot, chat_id, text);

			return 1;
		}

		return 0;
	}

	// Analys user command
	int CallCmd(TgBot *bot, VString id, VString cmd){
		if(cmd == "start"){
			SendMsg(bot, id, "Hello!");
		} else
			SendMsg(bot, id, "Unknown command.");

		return 1;
	}

	// Analys message
	int CallMsg(TgBot *bot, VString id, VString text){
		if(IfKeyword(bot, text))
			SendMsg(bot, id, GetRandomText());
		return 1;
	}

	// if text contained bok_keys
	int IfKeyword(TgBot *bot, VString text){
		VString keys = bot->keywords;

		while(keys){
			VString key = PartLine(keys, keys, ","), o, t;
			o = PartLine(text, t, key);

			// text => o + key + t.
			if((!o || o.endo() == ' ') && t.data && (!t || t[0] == ' '))
				return 1;
		}

		return 0;
	}

	// Send message to user
	int SendMsg(TgBot *bot, VString id, VString text){
		GetHttp gp;

		return gp.Request(HLString() + "https://api.telegram.org/bot" + bot->token + "/sendMessage?chat_id=" + id
			+ "&text=" + text);	
	}

	// ------- Random Text
	// Load random text
	int LoadRandomText(VString file){
		if(!IsFile(file))
			return 0;

		botword_d = LoadFile(file);
		VString line = botword_d;

		while(line){
			VString k = PartLine(line, line, "|");
			//VString v = PartLine(line, line, "|");
			botword.insert(k, VString());		
		}

		return 1;
	}

	// Get random text
	VString GetRandomText(){
		return botword.el(msrand() % botword.size()).key;
	}

};



VString TgBot::GetBotsCertCert(){
	return bots->GetCertCert();
}

VString TgBot::GetBotsServAddr(){
	return bots->GetServAddr();
}
		