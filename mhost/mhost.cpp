#define PROJECTNAME "mhost"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_GENERALCPP

#define USEMSV_ITOS

//#include <ws2tcpip.h>

#include "../../opensource/msvcore/msvcore.cpp"

#include "../../opensource/msvcore/net/dns.h"
//bool DomainDnsSendData(VString domain, int t, DNS_HEADER *dh, PROCSTRUCTCN&el, int tp, int ic){ return 0; }
//bool DomainDnsSendData(VString domain, int t, DNS_HEADER *dh, SOCKET sock, unsigned int cip, unsigned int cport, int tp, int cl){ return 1; }

#include "mhost.h"

Versions mhostversions[]={
"0.0.1.1", "10.11.2012 19:56",
"0.0.1.0", "30.10.2010 00:00"
};

int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link); LoadLang("lang");
	print("mhost v.", mhostversions[0].ver," (", mhostversions[0].date, "). [--uselang ", msv_use_lang, "]\r\n\r\n");
		
	// analys command line
	VString l="GO", t; int lto=2, ltop=1; 
	// add data
	VString domain, serv; Itos it;
	int type=DNS_TYPE_A, wait=10, ishelp=0, isver=0;

	while(l){
		if(!ltop && !lto) lto=1; ltop=lto;
		if(lto==1){ l=t; t=link.GetArg(); lto=0; }
		else if(lto==2){ l=link.GetArg(); t=link.GetArg(); lto=0; }		
		if(!l) break;

		if(l=="--help"){ lto=1; ishelp=1; }
		else if(l=="--ver"){ lto=1; isver=1; }
		else if(lto=usemsv_iskey(l)){}		
		else if(l=="t"){ lto=2;
			type=dnsstoi(t); if(t=="ANY") type=255;
			if(!type){ print(Lang("Type error: type not found"), " '", t, "'\r\n"); return 1; }
		}
		else if(l=="-a"){ lto=1; type=255; }
		else if(l=="-W"){ lto=2; wait=t.toi(); }
		else if(!domain){ lto=1; domain=l; }
		else if(!serv){ lto=1; serv=l; }
		else { print(Lang("Invalid options: unknown key"), " '", l, "'\r\n"); return 1; }
	}

	if(isver){
		VersionsPrint(mhostversions, sizeof(mhostversions)/sizeof(Versions));
		return 0;
	}

	if(!domain || ishelp){
		print(Lang("Usage: mhost [-atW] [-t type] [-W time] hostname [server]"), "\r\n");
		print(Lang(" ## msv options: use --usemsv --help"), "\r\n");
		print(Lang(" ## info"), "\r\n");
		print("--help", "\t\t", Lang("print this help"), "\r\n");
		print("--ver", "\t\t", Lang("print versions list"), "\r\n");
		print(Lang(" ## options"), "\r\n");
		print("-t", "\t\t", Lang("answer type"), "\r\n");
		print("-a", "\t\t", Lang("quest ANY data"), "\r\n");
		print("-W", "\t\t", Lang("wait time (ms)"), "\r\n");
		return 0;
	}

	// Server
	unsigned int ip=0; unsigned short port=53;

	if(serv){
		VString a1, a2; a1=PartLine(serv, a2, ":");
		if(!a1){ print(Lang("Fatal error: invalid DNS server"), ": '", serv, "'\r\n"); return 1; }
		ip=GetIP(a1); if(a2) port=a2.toi();
		//print("use server: ", serv, "\r\n");
	}else{
		unsigned int arr[8], asz=8;
		int s=GetDNSServs(arr, asz);
		if(s){ ip=arr[0]; }
	}

	if(!ip){ print(Lang("Fatal error: dns servers not found"), "\r\n"); return 0; }

	print(Lang("DNS server"), ": ", it.ipitos(ip), ":"); print(it.itos(port), "\r\n\r\n");

	DnsQuest dq;
	dq.Quest(domain, type, ip, port, wait);

	print(";; ->>HEADER<<- opcode: ", !dq.hdr.opcode ? "QUERY" : "???" );
	print(" status: ", dq.hdr.rcode ? "ERROR" : "NOERROR");
	print(", id: ", it.itos(dq.hdr.id), "\r\n");
	print(";; flags:", dq.hdr.qr ? " qr" : "", dq.hdr.rd ? " rd" : "", dq.hdr.ra ? " ra" : "",
	dq.hdr.tc ? " tc" : "", dq.hdr.aa ? " aa" : "");
	print("; QUERY: ", itos(dq.hdr.qdcount),", ANSWER: ", itos(dq.hdr.ancount),
		  ", AUTHORITY: ", itos(dq.hdr.nscount),", ADDITIONAL: ", itos(dq.hdr.arcount),"\r\n");
	//;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 51336
	//;; flags: qr rd ra; QUERY: 1, ANSWER: 12, AUTHORITY: 0, ADDITIONAL: 8


	print("\r\n;; QUESTION SECTION:\r\n;");
	print(dq.name, ".\t\t\tIN\t", dnsitos(dq.type), "\r\n\r\n");
	//;yandex.ru.                     IN      ANY

	print(";; ANSWER SECTION:\r\n");
	DnsData *d=dq.GetData(); int as=dq.hdr.ancount+dq.hdr.nscount;
while(d){
	if(as==0) print("\r\n;; ADDITIONAL SECTION:\r\n"); as--;

	print(d->key(), ".\t\t", it.itos(d->ttl), "\tIN\t");
	print(dnsitos(d->type), "\t");
	switch(d->type){
		case DNS_TYPE_A: print(it.ipitos(d->ip())); break;
		case DNS_TYPE_AAAA: print(ip6itos(d->ip6())); break;
		case DNS_TYPE_MX: print(it.itos(d->pri), " ");
		case DNS_TYPE_NS:
		case DNS_TYPE_MD:
		case DNS_TYPE_MF:
		case DNS_TYPE_CNAME:
		case DNS_TYPE_MB:
		case DNS_TYPE_MG:
		case DNS_TYPE_MR:
		case DNS_TYPE_PTR:
			print(d->val()); break;
		case DNS_TYPE_TXT:
			print("'", d->val(), "'"); break;
		case DNS_TYPE_SOA:
			print(d->val(), " ", d->soa(), " ");
			print(it.itos(d->s_ser()), " ");
			print(it.itos(d->s_ref()), " ");
			print(it.itos(d->s_ret()), " ");
			print(it.itos(d->s_exp()), " ");
			print(it.itos(d->s_min()));
			break;
		default : print(Lang("Error: Unknown DNS type"));
	}

	print("\r\n");
	d=d->_n;
}

	if(dq.error) print("\r\n", Lang("Program don't normal closed. Error found! DNS data corrypt?"));
	print("\r\n", Lang("Received"), " ", it.itos(dq.rcvsz));
	print(Lang(" bytes from "), it.ipitos(ip));
	print(":", it.itos(port));
	print(Lang(" in "), it.itos(dq.rtm)); print(Lang(" ms"), "\r\n");

return 0;
}