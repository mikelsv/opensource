#define PROJECTNAME "http_load"
#define PROJECTVER PROJECTNAME ## _versions
#define USEMSV_ITOS
#define USEMSV_ILIST
#define USEMSV_OLIST
#define USEMSV_MLIST
//#define USEMSV_HTTP
#define USEMSV_GENERALCPP

#include "../../opensource/msvcore/msvcore.cpp"

// -s=1 -t=2 -c=3 http://mikelsv.ru/first%20on%20moon.jpg

Versions PROJECTVER[]={
	// new version to up
	"0.0.0.2", "26.05.2015 19:45",
	"0.0.0.1", "--.--.--- --:--"
};


int64 gsec=0; int gend=0;
MString *urls, _urls; int urlsz=0;
// sec, threads, connects;
int sec=10, thr=5, con=3, full=0; // thr*con = in sec; in sec*sec = all.

GLock glock;

// global stat
int errcodes[S1K];
int acon=0; int64 asend=0, arecv=0;

int rthrs=0;

int http_test(VString con, VString sd, int &rcvs){
	ConIp ip(con);
	if(!ip.port)
		ip.port = 80;
	SOCKET sock = ip.Connect();
	
	if(!sock)
		return 0;

	int s, r;
	s = send(sock, sd, sd, 0);

	unsigned char buf[S1K], buft[S1K];
	rcvs = 0;

	while(1){
		r = recv(sock, rcvs ? (char*)buft : (char*)buf, S1K, 0);
		if(r>0)
			rcvs += r;
		else
			break;
	}

	closesocket(sock);

	if(rcvs)
		return stoi(VString(buf + 9, 3));

	return 0;
}

DWORD Thr(LPVOID){
	glock.Lock(); rthrs++; glock.UnLock();
	int64 tsec=gsec; int cons=0, mcon; if(full) mcon=sec*con; else mcon=con;
	//GetHttp http;

	VString url = urls[0];
	VString to = PartLineDouble(url, "http://", "/");
	if(!to)
		to = PartLineOT(url, "http://");

	HLString hsd;
	hsd + "GET / HTTP/1.0" "\r\n" \
		"Host: " + to + "\r\n" \
		"\r\n";

	MString sd = hsd;
	int rcv, i;

	while(!gend){
		if(gsec-tsec>1000){ tsec=gsec; if(!full) cons=0; }

		if(cons<con){
			//int i=rand()%urlsz; // int errc=0; IHeader head;
			//MString page=GetPage(MString(), 0, url, errc, head);
			//http.Request(url);
		
			i = http_test(to, sd, rcv);		
			//i = http.GetHead().GetCode();
		
			glock.Lock();
			acon++; asend += sd; arecv += rcv; //asend+=http.GetSendSz(); arecv+=http.GetRecvSz(); //asend+=url.size(); arecv+=page.size();

			if(i>=0 && i<S1K)
				errcodes[i]++;

			glock.UnLock();

			cons++;

		} else Sleep(10);
	}

	glock.Lock(); rthrs--; glock.UnLock();
	return 0;
}


int main(int args, char* arg[]){
	ILink link; mainp(args, arg, link);
	print(PROJECTNAME, " v.", PROJECTVER[0].ver," (", PROJECTVER[0].date, ").\r\n");

	memset(&errcodes, 0, sizeof(errcodes));

	int i=link.GetArg("-s").toi(); if(i) sec=i;
	i=link.GetArg("-t").toi(); if(i) thr=i;
	i=link.GetArg("-c").toi(); if(i) con=i;
	i=link.GetArg("-f").toi(); if(i) full=i;

	VString url;
	VString l; link.FirstArg();
	while(l=link.GetArg()){ if(l.is() && l[0]!='-'){ url=l; }}

	if(url.is() && url.incompare("http://")){
		_urls.Reserv(sizeof(MString), 0);
		urls=(MString*)_urls.rchar(); urls[0]=url; urlsz=1;	
	}/*else{
		MString lf=LoadFile(url); lf=Replace(lf, "\r\n", "\n");
		ExplodeLine el; explode(el, lf, lf, (unsigned char*)"\n", 1);
		urlsz=el.size();
		_urls.Reserv(sizeof(MString)*urlsz, 0); urls=(MString*)_urls.rchar();
		for(i=0; i<urlsz; i++){ urls[i]=el.el(i); }
	}*/

	print("MSV Http Load. [");
	print(itos(sec), " seconds(-s), ");
	print(itos(thr), " threads(-t), ");
	print(itos(con), " connects(-c), ");
	print(full ? "yes" : "no", " full(-f)"); print("]\r\n");
	print("Url(http://) or file: '", url, "' ", itos(urlsz), " url(s)\r\n");
	if(!url || !urlsz){ print("Error url not set\r\n"); exit(1); }


	gsec=time(); int64 tsec=sectime()+sec*1000;
	for(i=0; i<thr; i++){ StartThread(Thr); }

	while((gsec=sectime())<tsec){
		Sleep(10);
	}

	gend = 1;

    // results
	print("\r\nResults:\r\n");
	print("Connects: ", itos(acon), "/", itos(sec*thr*con), "(max), ");
	print("in sec: ", dtos(float(acon)/sec, 2), "\r\n");
	print(" Send: ", itos(asend), " Recv: ", itos(arecv), "\r\n");
	print("\r\n");
	print("Errors:\r\n");
	for(i=0; i<S1K; i++){
		if(errcodes[i]) print("Code: ", itos(i), " - ", itos(errcodes[i]), "\r\n");
	}

	int maxwait = 1000; //ms * 10 = 10 sec.

	while(rthrs & maxwait){
		Sleep(10);
		maxwait --;
	}

	return 1;
}