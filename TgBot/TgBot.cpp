#define PROJECTNAME "tgbot"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#define USEMSV_ITOS
#define USEMSV_ILIST
#define USEMSV_OLIST
#define USEMSV_MLIST
#define USEMSV_XDATACONT

#define USEMSV_CONFLINE

#define USEMSV_HTTP
#define USEMSV_OPENSSL
#define USEMSV_LIGHTSERVER

#define USEMSV_LIGHTSERVER_HTTP_CALL	TgBotCall

#include "../../opensource/msvcore/msvcore.cpp"

Versions PROJECTVER[]={
	// new version to up
	"0.0.0.1", "24.11.2015 22:19"
};

void TgBotLoadRandomText();

VString bot_token = "163124850:AAHJrtQYMtNAdIsTQAZkOFyM7JoN8Nv0JLk";
VString bot_url = "https://95.143.221.149:88/";
VString bok_keys = "bot,robo";

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link);

	print(PROJECTNAME, " v.", PROJECTVER[0].ver," (", PROJECTVER[0].date, ").\r\n");

	// Create sertificate: to cert/ folder
	// openssl req -x509 -newkey rsa:2048 -new -days 10000 -keyout cert_key.pem -out cert_cert.pem -nodes -subj "/C=RU/L=Moscow/O=Telegram Bot/OU=Telegram Bot/CN=95.143.221.149"
	// replace ip address to your
	// And run this programs!

	// Test bot token
	GetHttp gp, gp2;
	XDataCont ct;
	XDataPEl el;

	// http request
	gp.Request(HLString() + "https://api.telegram.org/bot" + bot_token + "/getMe");
	// Parse json
	ct.Read(gp.GetData());
	el = ct;

	if(el["ok"] != "true"){
		print("Token error: ", el["description"], "\r\n");
		return -1;
	}

	//{"ok":true,"result":{"id":163124850,"first_name":"LivestreamTestBot","username":"LivestreamTestBot"}}
	el = el("result");
	print("Token info: id: ", el["id"], ", first_name: ", el["first_name"], ", username: ", el["username"], "\r\n");


	// Send my server ip and public key to telegram.
	// http request
	MString cert_key = LoadFile("cert/cert_cert.pem");

	HLString ls;
	TString bnid = md5h(cert_key);
	bnid.Add("--------", bnid);
	gp.AddBoundary(ls, bnid, "url", bot_url);
	gp.AddBoundaryFile(ls, bnid, "certificate", "cert.pem", cert_key);
	gp.AddBoundaryEnd(ls, bnid);
	gp.SetBoundary(bnid, ls.String());

	//gp2.SetBoundary("cert.key", cert_key);
	gp.Request(HLString() + "https://api.telegram.org/bot" + bot_token + "/setWebhook?url=" + bot_url
	+ "&certificate=cert.key");

	// Parse json
	ct.Read(gp.GetData());
	el = ct;

	if(el["ok"] != "true"){
		print("Token error: ", el["description"], "\r\n");
		return -1;
	}

	// Load random text
	TgBotLoadRandomText();

	// Run server
	print("Run server.\r\n");
	LightServerHttp http;
	http.SetPort("88");
	http.Listen();

	return 0;
}

// random text
TULine botword;
MString botword_d;

// Load random text
void TgBotLoadRandomText(){
	botword_d = LoadFile("botword_en.txt");
	VString line = botword_d;

	while(line){
		VString k = PartLine(line, line, "|");
		//VString v = PartLine(line, line, "|");
		botword.insert(k, VString());		
	}

	return ;
}

// Get random text
VString TgBotGetRandomText(){
	return botword.el(rand() % botword.size()).key;
}

// Send message to user
int TgBotSendMsg(VString id, VString text){
	GetHttp gp;

	return gp.Request(HLString() + "https://api.telegram.org/bot" + bot_token + "/sendMessage?chat_id=" + id
		+ "&text=" + text);	
}

// Analys user command
int TgBotCallCmd(VString id, VString cmd){
	if(cmd == "start"){
		TgBotSendMsg(id, "Hello!");
	} else
		TgBotSendMsg(id, "Unknown command.");

	return 1;
}

// if text contained bok_keys
int TgBotIfKeyword(VString text){
	VString keys = bok_keys;

	while(keys){
		VString key = PartLine(keys, keys, ","), o, t;
		o = PartLine(text, t, key);

		// text => o + key + t.
		if((!o || o.endo() == ' ') && t.data && (!t || t[0] == ' '))
			return 1;
	}

	return 0;
}

// Analys message
int TgBotCallMsg(VString id, VString text){
	if(TgBotIfKeyword(text))
		TgBotSendMsg(id, TgBotGetRandomText());
	return 1;
}

int TgBotCall(LightServerAccept &acc, VString head, VString post){
	XDataCont ct(post);
	XDataPEl message = ct("message");
	XDataPEl chat = message("chat");

	VString text = message["text"];
	VString chat_id = chat["id"];

	if(message && text){
		text = Replace(text, "\\/", "/");
		print("Message from: ", chat_id, ", text: ", text, "\r\n");

		if(text[0] == '/')
			TgBotCallCmd(chat_id, text.str(1));
		else
			return TgBotCallMsg(chat_id, text);

		return 1;
	}

	return 0;
}