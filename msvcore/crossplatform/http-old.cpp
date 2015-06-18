MString HTI(const MString &line){
LString ret;
int sz=line.size();
for(int i=0;i<sz;i++){
	if(line[i]=='+') {ret+=" "; continue;}
if(line[i]=='%'){
	int ar[2];char car[2];
	car[0]=line[i+1];car[1]=line[i+2]; ar[0]=0; ar[1]=0;
	for(int ii=0;ii<2;ii++){
		if (int(car[ii])>47 && int(car[ii])<58) ar[ii]=int(car[ii])-48;
		if (int(car[ii])>64 && int(car[ii])<71) ar[ii]=int(car[ii])-55;
	}
ret+=MString(char(ar[0]*16+ar[1]));i++;i++;
}
else { ret+=line[i];}
}
return ret;
}

MString HTInp(const MString &line){
LString ret;
int sz=line.size();
for(int i=0;i<sz;i++){
//	if(line[i]=='+') {ret+=" "; continue;}
if(line[i]=='%'){
	int ar[2];char car[2];
	car[0]=line[i+1];car[1]=line[i+2]; ar[0]=0; ar[1]=0;
	for(int ii=0;ii<2;ii++){
		if (int(car[ii])>47 && int(car[ii])<58) ar[ii]=int(car[ii])-48;
		if (int(car[ii])>64 && int(car[ii])<71) ar[ii]=int(car[ii])-55;
	}
ret+=MString(char(ar[0]*16+ar[1]));i++;i++;
}
else { ret+=line[i];}
}
return ret;
}

MString CleanVal(MString val){
val=Replace(val, "&#032;", " ");
val=Replace(val, "&", "&amp;");
val=Replace(val, "<", "&lt;");
val=Replace(val, ">", "&gt;");
val=Replace(val, "\\n", "<br>");
val=Replace(val, "$", "&#036;");
val=Replace(val, "\\r", "");
val=Replace(val, "!", "&#33;");
val=Replace(val, "\"", "&quot;");
val=Replace(val, "'", "&#39;");
//val=Replace(val, "%25", "%");
//val=Replace(val, "%40", "@");
//val=Replace(val, "%3A", ":");
//val=Replace(val, "%2F", "/");
return val;
}

MString CleanValN(MString val){
val=Replace(val, "&#032;", " ");
val=Replace(val, "&lt;", "<");
val=Replace(val, "&gt;", ">");
val=Replace(val, "\\n", "<br>");
val=Replace(val, "&#036;", "$");
val=Replace(val, "\\r", "");
val=Replace(val, "&#33;", "!");
val=Replace(val, "&quot;", "\"");
val=Replace(val, "&#39;", "'");
val=Replace(val, "&apos;", "'");
val=Replace(val, "&nbsp;", " ");
val=Replace(val, "&amp;", "&");
return val;
}

MString CleanValR(MString val){
val=Replace(val, "&", "%26");
return val;
}

MString HTIEncode(VString line){
	MString ret;
	ret.Reserv(line.sz*3);

	unsigned char*ln=line, *to=line.endu(), *r=ret;
	while(ln<to){
		*r++='%';
		*r++=cb16[(*ln)/16];
		*r++=cb16[(*ln)%16];
		ln++;
	}

	return ret;
}


// GetPage
#define HTTP_DEFSZ		S4K
#define HTTP_MAXSZ		S16K
#define HTTP_MAXSEND	S4K

class GetPage{
	MString ip, page; int port; // for proxy
	int retcode; bool retall; // returncode, return all?, text/html;
	IHeader head; ILink link;
	int arecv, asend;

	MString data, ret, post; VString http, code, codet; // VString method, path, http;
	MString agent; 

	int sock; int maxwait;

public:
	GetPage(){ port=0; arecv=0; asend=0; sock=0; maxwait=300; }

	bool RetAll(){ return retall; }
	bool RetAll(bool b){ return retall=b; }

	int64 ASend(){ return asend; }
	int64 ARecv(){ return arecv; }

	VString GetData(){ return data; }
	void MaxWait(int m){ maxwait=m; }

// Loaded
//VString GetMethod(){ return method; }
//VString GetPath(){ return path; }
//VString GetHttp(){ return http; }

bool SetAgent(VString line){ agent=line; }
void SetPost(VString line){ post=line; }

int Load(VString page, int all=1){ retcode=0; arecv=0; asend=0;
ILink link; link.Link(page); if(all) retall=1; MString tmp;// Itos it;

// clear
data.Clean(); ret.Clean(); http.Clean(); code.Clean(); codet.Clean(); //method.Empty(); path.Empty(); http.Empty(); data.Empty();

//if(!ip) ip=domain
sock=ConnectPort(MString(ip ? ip : link.domain), link.port ? link.port : 80); if(sock<=1) return 0;
if(!link.domain) link.SetDomain(ip);

head.Header(tmp.Add("Host: ", link.domain, link.port ? ":" : VString(), link.port ? itos(link.port) : VString() ));
if(agent) head.Header("User-Agent: "+agent);
else head.Header("User-Agent: Centerix /1.0 (compatible; MSIE 6.0; Windows NT 5.1)");
//head.Header("User-Agent: Findex/1.0 (compatible; MSIE 6.0; Windows NT 5.1)");
head.Header("Accept: */*");
//head.Header("Keep-Alive: 300");
//head.Header("Connection: Keep-Alive");
//head.Header("Keep-Alive: 300");
head.Header("Connection: Close");
head.Header("Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7");
head.Header("Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3");
//head.Header("Referer: http://www.muzoff.ru/pages/51/5135.shtml");

if(post){
	head.Header("Content-Length: "+itos(post.size()));
	head.Header("Content-Type: application/x-www-form-urlencoded");
}
asend+=head.SendHeader( !post ? "GET" : "POST", link.GetPathFileQuest(), "HTTP/1.0", sock, 1); // send_(sock, MString("\r\n"));

if(post){
	char*ln=post, *to=post.end();
	while(ln<to){
		int s=to-ln; if(s>HTTP_MAXSEND) s=HTTP_MAXSEND;
		s=send(sock, ln, s, 0);
		if(s<=0) return 0;
		asend+=s; ln+=s;
	}
 send(sock, "\r\n", 2, 0);
}

return LoadRecv();
}


int LoadRecv(){ retcode=0;
	MString post; char *rcv, *prcv; int ex=0;

while(1){
	if(!post){post.Reserv(HTTP_DEFSZ); rcv=post; prcv=rcv;}
	if(post.end()==prcv && post.size()==HTTP_DEFSZ){
		post.Reserv(HTTP_MAXSZ); post.Reserv(HTTP_MAXSZ); rcv=post; prcv=rcv+HTTP_DEFSZ;
	}
	VString ln; int rd;

	if((rd=recv(sock, prcv, post.size()-(prcv-rcv), 0))<1 || rd>post.size()) return 0;
	prcv+=rd; if(arecv>S32K) return 0;
// analys
	char*line=rcv, *lln=rcv;// pos=to-rcv; //line;

	for(line; line<prcv; line++){
		if(*line==13){
			ln.set(lln, line-lln); if(!ln) { ex++; } else ex=0;
	if(!ret){
		ret=ln; unsigned char*l=ret, *lp=ret;
		rts(ret.endu(), ' ', l); http.setu(lp, l-lp); lp=++l;
		rts(ret.endu(), ' ', l); code.setu(lp, l-lp); lp=++l;
		l=ret.endu(); codet.setu(lp, l-lp); retcode=code.toi();
	//if ((ln.incompare("HEAD ") || ln.incompare("GET ") || ln.incompare("POST ") || ln.incompare("CONNECT ") || ln.incompare("FILE ")) && !method){
	//	get=ln; unsigned char*l=get, *lp=get;
	//	rts(get.endu(), ' ', l); method.setu(lp, l-lp); lp=++l;
	//	rts(get.endu(), ' ', l); path.setu(lp, l-lp); lp=++l;
	//	l=get.endu(); http.setu(lp, l-lp);
	} else{
		head.SHeader(ln);
	}
	if(line+1<prcv && *(line+1)==10) line++; if(ex>=1) {line++; break;}
	lln=line+1;// if(*lln==10) lln++;
		}
	}

	arecv+=rd-(prcv-line);
	if(prcv-line){ memcpy(rcv, line, prcv-line); prcv=rcv+(prcv-line); } else { prcv=post;}
	if(ex) break;
}

// Load data
VString ct=head.SGet("Content-Type"); int isread=retall;
if(!isread && ct.is()){
	unsigned char*ln=ct, *lln=ln, *to=ct.endu();
	while(lln>=to){
		rts(to, ';', ln);
		if(ln-lln==9 && cmp((char*)lln, "text/html", 9)){ isread=1; break; }
		lln=++ln;
	}
}

// Load
HLString ls;
char buf[S4K]; int rd=0, slt=0;
if(prcv>rcv){ ls.add(rcv, prcv-rcv); }
// Content-Length:
int64 cl=stoi64(head.SGet("Content-Length"));

if(isread){
		
	while(1){
		if(maxwait){			
			timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
			tm.tv_sec=maxwait; tm.tv_usec=0;
			if(select(sock+1, &rfds, 0, 0, &tm)<1){
				retcode=0; break;
			}
		}
		rd=recv(sock, buf, S4K,0);
		if(rd<1) break; arecv+=rd;
		//if((unsigned int)rd<mrecv.size()) {Sleep(slt); slt+=10;} else {if(slt>0) slt--;}
	ls.add(buf, rd);
	}
}

ls.String(data); ls.Clear();
//retcode=stoi();
closesocket(sock); sock=0;

return retcode;
}


};


#ifdef NO_USE_IT_PLEASE


MString dlrtna9101332(const MString &ln){ // del all 9, 10, 14, 32
LString ls; unsigned char *to=ln.uchar()+ln.size(), *line=ln.uchar(); bool lip=0;
for (line; line<to; line++){
	if(*line==9 || *line==10 || *line==13 || *line==32) {if(!lip){lip=1; ls+=32;} }
	else {ls+=*line; lip=0;}
}
return ls;
}

void NoCache(IHeader&ih){
MTime64 mt;
ih.Header("Date: "+mt.date("D, d M Y H:i:s \\G\\M\\T", mt.time(), 1));
ih.Header("Pragma: no-cache");
ih.Header("Cache-Control: no-cache");
ih.Header("Last-Modified: Sat, Jan 1970 00:00:00 GMT");
return ;
}

void NoCacheS(IHeader&ih){
MTime64 mt;
ih.SHeader("Date: "+mt.date("D, d M Y H:i:s \\G\\M\\T", mt.time(), 1));
ih.SHeader("Pragma: no-cache");
ih.SHeader("Cache-Control: no-cache");
ih.SHeader("Last-Modified: Sat, Jan 1970 00:00:00 GMT");
return ;
}

MString Rephtml(VString ln){
HLString ls;
unsigned char *pos=ln.data, *lpos=pos, *to=ln.data+ln.sz;
while(1){
	int err=rtms(ln.data, ln.sz, (unsigned char*)"&lt;", 4, pos);
	ls+VString(lpos, pos-lpos);

	if(err){ pos+=4; VString rep;// 
	if(pos+1<to && cmp((char*)pos, (char*)"a", 1)) rep="a";
	if(pos+1<to && cmp((char*)pos, (char*)"/a", 2)) rep="/a";
	if(pos+1<to && cmp((char*)pos, (char*)"img", 3)) rep="img";

	if(rep){ pos+=rep.sz; lpos=pos;
	if(rtms(ln.data, ln.sz, (unsigned char*)"&gt;", 4, pos)){ ls+"<"+rep+VString(lpos, pos-lpos)+">"; pos+=4; lpos=pos; }
	else ls+"&lt;"+rep+VString(lpos, pos-lpos)+">"; lpos=pos;
	} else {ls+="&lt;"; lpos=pos;}

	} else { break;}
}

return ls;
}

class GetPage{
MString ip, page; int port; // for proxy
int retcode; bool retall; // returncode, return all?, text/html;
IHeader head; ILink link;
int arecv, asend;

MString data, ret, post; VString http, code, codet; // VString method, path, http;
MString agent; 

int sock; int maxwait;

public:
GetPage(){ port=0; arecv=0; asend=0; sock=0; maxwait=300; }

bool RetAll(){ return retall; }
bool RetAll(bool b){ return retall=b; }

int64 ASend(){ return asend; }
int64 ARecv(){ return arecv; }

VString GetData(){ return data; }
void MaxWait(int m){ maxwait=m; }

// Loaded
//VString GetMethod(){ return method; }
//VString GetPath(){ return path; }
//VString GetHttp(){ return http; }

bool SetAgent(VString line){ agent=line; }
bool SetPost(VString line){ post=line; }

int Load(VString page, int all=1){ retcode=0; arecv=0; asend=0;
ILink link; link.Link(page); if(all) retall=1; MString tmp; Itos it;

// clear
data.Clean(); ret.Clean(); http.Clean(); code.Clean(); codet.Clean(); //method.Empty(); path.Empty(); http.Empty(); data.Empty();

//if(!ip) ip=domain
sock=ConnectPort(MString(ip ? ip : link.domain), link.port ? link.port : 80); if(sock<=1) return 0;
if(!link.domain) link.SetDomain(ip);

head.Header(tmp.Add("Host: ", link.domain, link.port ? ":" : VString(), link.port ? it.itos(link.port) : VString() ));
if(agent) head.Header("User-Agent: "+agent);
else head.Header("User-Agent: Centerix /1.0 (compatible; MSIE 6.0; Windows NT 5.1)");
//head.Header("User-Agent: Findex/1.0 (compatible; MSIE 6.0; Windows NT 5.1)");
head.Header("Accept: */*");
//head.Header("Keep-Alive: 300");
//head.Header("Connection: Keep-Alive");
//head.Header("Keep-Alive: 300");
head.Header("Connection: Close");
head.Header("Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7");
head.Header("Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3");
//head.Header("Referer: http://www.muzoff.ru/pages/51/5135.shtml");

if(post){
	head.Header("Content-Length: "+itos(post.size()));
	head.Header("Content-Type: application/x-www-form-urlencoded");
}
asend+=head.SendHeader( !post ? "GET" : "POST", link.GetPathFileQuest(), "HTTP/1.0", sock, 1); // send_(sock, MString("\r\n"));

if(post){
	char*ln=post, *to=post.end();
	while(ln<to){
		int s=to-ln; if(s>HTTP_MAXSEND) s=HTTP_MAXSEND;
		s=send(sock, ln, s, 0);
		if(s<=0) return 0;
		asend+=s; ln+=s;
	}
 send(sock, _er, _er, 0);
}

return LoadRecv();
}


int LoadRecv(){ retcode=0;
	MString post; char *rcv, *prcv; int ex=0;

while(1){
	if(!post){post.Reserv(HTTP_DEFSZ); rcv=post; prcv=rcv;}
	if(post.end()==prcv && post.size()==HTTP_DEFSZ){
		post.Reserv(HTTP_MAXSZ); post.Reserv(HTTP_MAXSZ); rcv=post; prcv=rcv+HTTP_DEFSZ;
	}
	VString ln; int rd;

	if((rd=recv(sock, prcv, post.size()-(prcv-rcv), 0))<1 || rd>post.size()) return 0;
	prcv+=rd; if(arecv>S32K) return 0;
// analys
	char*line=rcv, *lln=rcv;// pos=to-rcv; //line;

	for(line; line<prcv; line++){
		if(*line==13){
			ln.set(lln, line-lln); if(!ln) { ex++; } else ex=0;
	if(!ret){
		ret=ln; unsigned char*l=ret, *lp=ret;
		rts(ret.endu(), ' ', l); http.setu(lp, l-lp); lp=++l;
		rts(ret.endu(), ' ', l); code.setu(lp, l-lp); lp=++l;
		l=ret.endu(); codet.setu(lp, l-lp); retcode=code.toi();
	//if ((ln.incompare("HEAD ") || ln.incompare("GET ") || ln.incompare("POST ") || ln.incompare("CONNECT ") || ln.incompare("FILE ")) && !method){
	//	get=ln; unsigned char*l=get, *lp=get;
	//	rts(get.endu(), ' ', l); method.setu(lp, l-lp); lp=++l;
	//	rts(get.endu(), ' ', l); path.setu(lp, l-lp); lp=++l;
	//	l=get.endu(); http.setu(lp, l-lp);
	} else{
		head.SHeader(ln);
	}
	if(line+1<prcv && *(line+1)==10) line++; if(ex>=1) {line++; break;}
	lln=line+1;// if(*lln==10) lln++;
		}
	}

	arecv+=rd-(prcv-line);
	if(prcv-line){ memcpy(rcv, line, prcv-line); prcv=rcv+(prcv-line); } else { prcv=post;}
	if(ex) break;
}

// Load data
VString ct=head.SGet("Content-Type"); int isread=retall;
if(!isread && ct.is()){
	unsigned char*ln=ct, *lln=ln, *to=ct.endu();
	while(lln>=to){
		rts(to, ';', ln);
		if(ln-lln==9 && cmp((char*)lln, "text/html", 9)){ isread=1; break; }
		lln=++ln;
	}
}

// Load
HLString ls;
char buf[S4K]; int rd=0, slt=0;
if(prcv>rcv){ ls.add(rcv, prcv-rcv); }
// Content-Length:
int64 cl=stoi64(head.SGet("Content-Length"));

if(isread){
		
	while(1){
		if(maxwait){			
			timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
			tm.tv_sec=maxwait; tm.tv_usec=0;
			if(select(sock+1, &rfds, 0, 0, &tm)<1){
				retcode=0; break;
			}
		}
		rd=recv(sock, buf, S4K,0);
		if(rd<1) break; arecv+=rd;
		//if((unsigned int)rd<mrecv.size()) {Sleep(slt); slt+=10;} else {if(slt>0) slt--;}
	ls.add(buf, rd);
	}
}

ls.String(data); ls.Clear();
//retcode=stoi();
closesocket(sock); sock=0;

return retcode;
}


};


//////////////////////////////
MString GetPageOld(MString ip, int port, MString page, int &errorcode, IHeader &ghead, int allr=0, VString post=VString()){
//get page

	ILink link; link.Link(page); errorcode=0; MString tmp; Itos it;

	MString pname=page; //pname='/'; pname+=link.path; cslash(pname); if(link.file)
	int pos=0; //rtms(page, link.domain, pos); pos+=link.domain.size();
	//if(link.port) rts(page, '/', pos); page.Mid(pos); if(!pname) pname="/";

//page=page.str(link.domain.size()+7); if(!page) page="/";
if(page.size()==link.domain.size()+7) page+="/";
if(!link.domain) link.SetDomain(ip);

	if(!ip) ip=link.domain; if(!port) port=link.port ? link.port : 80;
	if(!isip(ip)){
	struct hostent *hp = gethostbyname(ip);
	if (!hp)
		return "";
	DWORD dwIP = ((in_addr*)hp->h_addr_list[0])->s_addr;
    ip=itos(LOBYTE(LOWORD(dwIP)))+"."+itos(HIBYTE(LOWORD(dwIP)))+"."+itos(LOBYTE(HIWORD(dwIP)))+"."+itos(HIBYTE(HIWORD(dwIP)));
	}

int sock=ConnectPort(ip, port);	if (sock<1) return MString();
IHeader &head=ghead;



//send_(sock, MString("GET "+page+" HTTP/1.1\r\n")); 
//head.Header(HLString()+"Host: "+link.domain+ ( link.port ? ":"+itos(link.port) : "" ));
head.Header(tmp.Add("Host: ", link.domain, link.port ? ":" : VString(), link.port ? it.itos(link.port) : VString() ));
head.Header("User-Agent: Centerix /1.0 (compatible; MSIE 6.0; Windows NT 5.1)");
//head.Header("User-Agent: Findex/1.0 (compatible; MSIE 6.0; Windows NT 5.1)");
head.Header("Accept: */*");
//head.Header("Keep-Alive: 300");
//head.Header("Connection: Keep-Alive");
//head.Header("Keep-Alive: 300");
head.Header("Connection: Close");
head.Header("Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7");
head.Header("Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3");
//head.Header("Referer: http://www.muzoff.ru/pages/51/5135.shtml");

if(post) {
	head.Header("Content-Length: "+itos(post.size()));
	head.Header("Content-Type: application/x-www-form-urlencoded");
}
head.SendHeader( !post ? "GET" : "POST", pname, "HTTP/1.0", sock, 1);// send_(sock, MString("\r\n"));

if(post){
send(sock, post, post, 0); send(sock, _er, _er, 0);
}

/*

GET http://download.muzoff.ru/m3_files/51/5135/serega_-_sportivnie_chastushki.mp3 HTTP/1.1
Host: download.muzoff.ru
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.8.0.5) Gecko/20060719 Firefox/1.5.0.5
Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,* /*;q=0.5
Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3
Accept-Encoding: gzip,deflate
Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7
Keep-Alive: 300
Proxy-Connection: keep-alive
Referer: http://www.muzoff.ru/pages/51/5135.shtml



*/

	 
	MString sn="Host: www.bossal.narod.ru\r\n\
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.8.0.4) Gecko/20060508 Firefox/1.5.0.4\r\n\
Accept: text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\n\
Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3\r\n\
Accept-Encoding: gzip,deflate\r\n\
Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\r\n\
Keep-Alive: 300\r\n\
Proxy-Connection: keep-alive\r\n\
Cookie: nuid=749925551150745891\r\n\
If-Modified-Since: Fri, 14 May 1984 02:48:52 GMT\r\n\
If-None-Match: \"11e2d8-1bd1-40a43394-windows-1251\"\r\n\
Cache-Control: max-age=0\r\n\
\r\n\
";
//send(sock, sn);


/*
GET /fon%5C003.gif HTTP/1.0
Host: www.bossal.narod.ru
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.8.0.4) Gecko/20060508 Firefox/1.5.0.4
Accept: image/png,* /*;q=0.5
Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3
Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7
Keep-Alive: 300
Referer: http://www.bossal.narod.ru/
Cookie: nuid=749925551150745891
Connection: Keep-Alive
*/




MString http, code, codes, data;

char rrecv[1];  MString mrecv; mrecv.Reserv(65536); 
int en=0;

MString line;


while(1)					 
{
		
if(recv(sock,rrecv,sizeof(rrecv),0)<1) break;
if (int(rrecv[0])==10 && en) {en=0; continue;}
//		printf("%i - %i\n",s, rrecv[0]);
//		Sleep(50);				   

//		printf("%s\n ",tmp);
		
if (int(rrecv[0])==13){
if(!line) break;
//MString ifs[2];
en=1;

//explode(ifs, line, " ", 1, 1);

//printf("%s", ifs[0]);
//LPTSTR lin = new CHAR[eline[0].GetLength()];	
//char *clin= lin;
//char lin[[eline[0].GetLength()];	
//int sz=eline[0].GetLength();
//char lin[sz]=eline[1];	


	//lines=line[0];
//line[0]=line[0].Left(line[0].GetLength()-1);


if (line.incompare("HTTP"))
{
MString tmp[3];
pos=line.size()-1;
rrts(line, ' ', pos);
//if (pos>0) tmp[2]=line.Mid(pos+1, line.size()-pos+1); 
//line=line.Left(pos);
explode(tmp, line, " ", 2, 1);
//head.GetI(tmp, line);
http=tmp[0]; code=tmp[1]; codes=tmp[2];
}

else {
ghead.SHeader(line);
}

/*

if (ifs[0]=="Accept:") { head.accept=ifs[1]; }
if (ifs[0]=="Accept-Language:") { head.acceptl=ifs[1]; }
if (ifs[0]=="Accept-Encoding:") { head.accepte=ifs[1]; }
if (ifs[0]=="Connection:") { head.connection=ifs[1]; }
if (ifs[0]=="User-Agent:") { head.useragent=ifs[1]; }
if (ifs[0]=="Cookie:") { head.cookie=ifs[1]; }
if (ifs[0]=="Referer:") { head.referrer=ifs[1]; }
if (ifs[0]=="Content-Length:") { head.contentl=ifs[1]; }

//Proxy
if (ifs[0]=="Host:") { head.host=ifs[1]; }
		 */

//switch(cr){
//case '123': printf("fghj");break;
//case 'GET':
//case 'POST':{
//CString tmp[3];
//explode(tmp, line, " ", 0);
//head.method=tmp[0]; head.path=line[1]; head.http=line[2];} break;	
//case 'Acce':	{
//	CString tmp[2]; explode(tmp, line, " ", 2);
//	for(int i=0;i<nline;i++){head.accept[i]=line[i].Left(line[i].GetLength()-1);break;
//	head.accept[1]=tmp[1]; break;}
//case "Accept-Language:":for(i=0;i<nline;i++){head->acceptl[i]=line[i].Left(line[i].GetLength()-1);
//case "Accept-Encoding:":for(i=0;i<nline;i++){head->accepte[i]=line[i].Left(line[i].GetLength()-1);
//case "User-Agent:":for(i=0;i<nline;i++){head->useragent+=line[i]+" ";
//case "Host:":for(i=0;i<nline;i++){head->useragent+=line[i]+" ";


////GET / HTTP/1.1
//Accept: */*
//Accept-Language: ru
//Accept-Encoding: gzip, deflate
//User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)
//Host: 127.0.0.1
//Connection: Keep-Alive
//Cookie: CPSRPR1=EcgWvoI1Z5cs; CPSR=%3Cnick%3AcuulOETJnQ%3D%3D%3E%3Ctextcol%3AVQ%3D%3D%3E%3Cun%3AWA%3D%3D%3E%3Cskin%3AmfSpQ0HhsA%3D%3D%3E; hotlog=1


line.Clean();			
}
else line+=rrecv[0];

}
recv(sock, rrecv, sizeof(rrecv),0);

int readdata=1; 

MString ct = ghead.Get("Content-Type");
if(ct){
	MString tmp[5];
int err=explode(tmp, ct, ";", 4, 1);
if (tmp[0]!="text/html") {readdata=allr; code=itos(700);}
}
	   
if(readdata){
int rd=0, slt=0;	
while(1){ 
	rd=recv(sock,mrecv.rchar(),mrecv.size(),0);
	if(rd<1) break;
	if((unsigned int)rd<mrecv.size()) {Sleep(slt); slt+=10;} else {if(slt>0) slt--;}
data+=mrecv.Left(rd);
}}

errorcode=stoi(code);
closesocket(sock);

return data;
}



bool ReadPage(MString &line, MString &title, MString &rettext, Line &links){

int pos=0, epos=0;
LString text;
int err=rtms(line, "<title>", pos); epos=pos;
if (err) err=rtms(line, "</title>", epos);

if (err){title=line.Mid(pos+7, epos-pos-7); }

// Delete Tag
int t=0; int k=0; int wr=1, p=0; MString tag;
for(unsigned int i=0; i<line.size(); i++){

	if (t==0){
		if(line[i]=='<') {t=1; tag.Clean(); }

		else if(line[i]==10) {}
		else if(line[i]==13) {text+=' ';}

	else if(!wr) text+=line[i];
	}


	else if(t==1){
	switch(line[i]){
	case '>': if (!k) t=9; goto etag; break;
	case  '"': if (k==0) k=2; else if (k==2) k=0; else tag+=line[i]; break;
	case  '\'': if (k==0) k=1; else if (k==1) k=0; else tag+=line[i]; break;
    default : tag+=line[i]; 
	}

	if(0){
	etag:
	int pos=0, tpos=0;
	int err=rts(tag, ' ', pos);
	//if (!err)
	MString tgn=tag.Left(pos);

	if (tgn.incompare("br")) text+=MString(13);

	if (tgn=="a"){
	err=rtms(tag, "href=", pos); pos+=5;
	if(err){tpos=pos+1;
	if (tag[pos]=='"') {pos++; err=rts(tag, '"', tpos);}
	else if (tag[pos]=='\'') {pos++; err=rts(tag, '\'', tpos);   }
	else err=rts(tag, ' ', tpos);
	links.insert(tag.Mid(pos, tpos-pos));  
	}}
	
	
	if(tgn=="style") wr=1; if(tgn=="/style") wr=0;
	if(tgn=="title") wr=1; if(tgn=="/title") wr=0;


	if(tgn=="script") wr=1; if(tgn=="/script") wr=0;
	if(tgn=="noscript") wr=1; if(tgn=="/noscript") wr=0;

	if(tgn.incompare("!--")) {p=i;
		int err=rtms(line, "-->", p);
		if(err) {i=p+3;} 
			//wr=1; if(tgn.incompare("//--")) {wr=0; i+=3;}
	}
	
	t=0;
	}



	}



}

rettext=text.operator MString();
rettext=Replace(rettext, "          ", " ");
rettext=Replace(rettext, "          ", " ");
rettext=Replace(rettext, "          ", " ");
rettext=Replace(rettext, "     ", " ");
rettext=Replace(rettext, "    ", " ");
rettext=Replace(rettext, "   ", " ");
rettext=Replace(rettext, "  ", " ");
rettext=Replace(rettext, "  ", " ");
return 1;
}




MString DeleteTagPage(MString line){
LString text; line==dlrtna9101332(line);
int pos=0, epos=0;

int lp=0, lr=0;

// Delete Tag
int t=0; int k=0; int wr=0, p=0; MString tag;
for(unsigned int i=0; i<line.size(); i++){


	if (t==0){
		if(line[i]=='<') {t=1; tag.Clean(); }

		else if(line[i]==10) { }
		else if(line[i]==13) { }
	else if(line[i]==' ' && lp==text.size()) {}
	else if(!wr) text+=line[i];
	}


	else if(t==1){
	switch(line[i]){
	case '>': if (!k) t=9; goto etag; break;
	case  '"': if (k==0) k=2; else if (k==2) k=0; else tag+=line[i]; break;
	case  '\'': if (k==0) k=1; else if (k==1) k=0; else tag+=line[i]; break;
    default : tag+=line[i]; 
	}

	if(0){
	etag:
	int pos=0, tpos=0;
	int err=rts(tag, ' ', pos);
	//if (!err)
	MString tgn=tag.Left(pos);

	if (tgn.incompare("br")) { if(lp!=text.size()){ /*text+="<br>";*/ lp=text.size();}}
	if (tgn.incompare("p")) {if(lp!=text.size()){ /*text+="<br>";*/ lp=text.size();}}

	if (tgn=="a"){
	err=rtms(tag, "href=", pos); pos+=5;
	if(err){tpos=pos+1;
	if (tag[pos]=='"') {pos++; err=rts(tag, '"', tpos);}
	else if (tag[pos]=='\'') {pos++; err=rts(tag, '\'', tpos);   }
	else err=rts(tag, ' ', tpos);
//	links.insert(tag.Mid(pos, tpos-pos));  
	}}
	
	
	if(tgn=="style") wr=1; if(tgn=="/style") wr=0;
	if(tgn=="title") wr=1; if(tgn=="/title") wr=0;


	if(tgn=="script") wr=1; if(tgn=="/script") wr=0;
	if(tgn=="noscript") wr=1; if(tgn=="/noscript") wr=0;

	if(tgn.incompare("!--")) {p=i;
		int err=rtms(line, "-->", p);
		if(err) {i=p+3;} 
			//wr=1; if(tgn.incompare("//--")) {wr=0; i+=3;}
	}
	
	t=0;
	}



	}



}

text=dlrtna9101332(text);
/*
text=Replace(text, "          ", " ");
text=Replace(text, "          ", " ");
text=Replace(text, "          ", " ");
text=Replace(text, "     ", " ");
text=Replace(text, "    ", " ");
text=Replace(text, "   ", " ");
text=Replace(text, "  ", " ");
text=Replace(text, "  ", " ");
*/
int _x=0;
return text;
}


//////////////////////////////
MString SendFile(VString ip, int port, VString page, VString file, MString data, unsigned int &errorcode, IHeader &ghead){
//get page

	ILink link; link.Link(page);

	MString pname=page; //pname='/'; pname+=link.path; cslash(pname); if(link.file)
	//int pos=0; rtms(page, link.domain, pos); //pname=page.Mid(pos+link.domain.size());
	int pos=0; rtms(page, link.domain, pos); pos+=link.domain.size();
	if(link.port) rts(page, '/', pos); pname=page.Mid(pos); if(!pname) pname="/";
	if(!ip) ip=link.domain;
	if(!isip(ip)){
	struct hostent *hp = gethostbyname(ip);
	if (!hp)
		return "";
	DWORD dwIP = ((in_addr*)hp->h_addr_list[0])->s_addr;
    ip=itos(LOBYTE(LOWORD(dwIP)))+"."+itos(HIBYTE(LOWORD(dwIP)))+"."+itos(LOBYTE(HIWORD(dwIP)))+"."+itos(HIBYTE(HIWORD(dwIP)));
	}

int sock=ConnectPort(ip, port);	if (!sock) return "";
IHeader head;

MString _err=MString("\r\n"); Itos it; 

// POST
it.Add("POST ", page, " HTTP/1.0", _err);
send(sock, it.ret, it, 0); 
send_(sock, it.Add("Host: ", link.domain, _err));
send_(sock, MString("User-Agent: Centerix /1.0 (compatible; MSIE 6.0; Windows NT 5.1)"+_err));
send_(sock, MString("Accept: * /*"+_err));
send_(sock, MString("Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7"+_err));
send_(sock, MString("Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3"+_err));
send_(sock, MString("Keep-Alive: 300"+_err));
//head.Snd(sock);// send_(sock, MString("\r\n"));
send_(sock, MString("Referer: http://www.centerix.ru/uplfl.asp"+_err));
send_(sock, MString("Content-Type: multipart/form-data; boundary=---------------------------27021169979319"+_err));

MString sd=MString("-----------------------------27021169979319"+_err)+
MString("Content-Disposition: form-data; name=\"userfile\"; filename=\""+MString(file)+"\""+_err)+
MString("Content-Type: image/jpeg"+_err)+
_err+
data+
_err+
MString("-----------------------------27021169979319--"+_err);

send_(sock, MString("Content-Length: "+itos(sd.size())+_err));
send_(sock, MString("Connection: Keep-Alive"+_err));
send_(sock, _err);
send_(sock, sd);

data.Clean();

MString http, code, codes;

char rrecv[1];  MString mrecv; mrecv.Reserv(65536); 
int en=0;

MString line;


while(1)					 
{
		
if(recv(sock,rrecv,sizeof(rrecv),0)<1) break;
if (int(rrecv[0])==10 && en) {en=0; continue;}
//		printf("%i - %i\n",s, rrecv[0]);
//		Sleep(50);				   

//		printf("%s\n ",tmp);
		
if (int(rrecv[0])==13){
if(!line) break;
//MString ifs[2];
en=1;

//explode(ifs, line, " ", 1, 1);

//printf("%s", ifs[0]);
//LPTSTR lin = new CHAR[eline[0].GetLength()];	
//char *clin= lin;
//char lin[[eline[0].GetLength()];	
//int sz=eline[0].GetLength();
//char lin[sz]=eline[1];	


	//lines=line[0];
//line[0]=line[0].Left(line[0].GetLength()-1);


if (line.incompare("HTTP"))
{
MString tmp[3];
pos=line.size()-1;
rrts(line, ' ', pos);
//if (pos>0) tmp[2]=line.Mid(pos+1, line.size()-pos+1); 
//line=line.Left(pos);
explode(tmp, line, " ", 2, 1);
//head.GetI(tmp, line);
http=tmp[0]; code=tmp[1]; codes=tmp[2];
}

else {
ghead.SHeader(line);
}

/*

if (ifs[0]=="Accept:") { head.accept=ifs[1]; }
if (ifs[0]=="Accept-Language:") { head.acceptl=ifs[1]; }
if (ifs[0]=="Accept-Encoding:") { head.accepte=ifs[1]; }
if (ifs[0]=="Connection:") { head.connection=ifs[1]; }
if (ifs[0]=="User-Agent:") { head.useragent=ifs[1]; }
if (ifs[0]=="Cookie:") { head.cookie=ifs[1]; }
if (ifs[0]=="Referer:") { head.referrer=ifs[1]; }
if (ifs[0]=="Content-Length:") { head.contentl=ifs[1]; }

//Proxy
if (ifs[0]=="Host:") { head.host=ifs[1]; }
		 */

//switch(cr){
//case '123': printf("fghj");break;
//case 'GET':
//case 'POST':{
//CString tmp[3];
//explode(tmp, line, " ", 0);
//head.method=tmp[0]; head.path=line[1]; head.http=line[2];} break;	
//case 'Acce':	{
//	CString tmp[2]; explode(tmp, line, " ", 2);
//	for(int i=0;i<nline;i++){head.accept[i]=line[i].Left(line[i].GetLength()-1);break;
//	head.accept[1]=tmp[1]; break;}
//case "Accept-Language:":for(i=0;i<nline;i++){head->acceptl[i]=line[i].Left(line[i].GetLength()-1);
//case "Accept-Encoding:":for(i=0;i<nline;i++){head->accepte[i]=line[i].Left(line[i].GetLength()-1);
//case "User-Agent:":for(i=0;i<nline;i++){head->useragent+=line[i]+" ";
//case "Host:":for(i=0;i<nline;i++){head->useragent+=line[i]+" ";


////GET / HTTP/1.1
//Accept: */*
//Accept-Language: ru
//Accept-Encoding: gzip, deflate
//User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)
//Host: 127.0.0.1
//Connection: Keep-Alive
//Cookie: CPSRPR1=EcgWvoI1Z5cs; CPSR=%3Cnick%3AcuulOETJnQ%3D%3D%3E%3Ctextcol%3AVQ%3D%3D%3E%3Cun%3AWA%3D%3D%3E%3Cskin%3AmfSpQ0HhsA%3D%3D%3E; hotlog=1


line.Clean();			
}
else line+=rrecv[0];

}
recv(sock, rrecv, sizeof(rrecv),0);

int readdata=1; 

MString ct = ghead.Get("Content-Type");
if(ct){
	MString tmp[5];
int err=explode(tmp, ct, ";", 4, 1);
if (tmp[0]!="text/html") {readdata=0; code=itos(700);}
}
	   
if(readdata){
int rd=0, slt=0;	
while(1){ 
	rd=recv(sock,mrecv.rchar(),mrecv.size(),0);
	if(rd<1) break;
	if((unsigned int)rd<mrecv.size()) {Sleep(slt); slt+=10;} else {if(slt>0) slt--;}
data+=mrecv.Left(rd);
}}

errorcode=stoi(code);
closesocket(sock);


	return 1;
}

#endif