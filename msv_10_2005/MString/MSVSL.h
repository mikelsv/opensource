// MSV Server Library for MString (MSVLibrary)
//1.12.04 (build 1)
typedef struct IHeader {
	MString method;		// GET POST
	MString path;		// /index.msl?act=allok
	MString post;		// post
	MString	size;		// size post or file
	MString http;		// HTTP/1.1 200 OK
	MString referrer;	// http://localhost/www/index.msl?all=nook
//	MString *accept;
	MString accept;		// Accept: */*
	MString acceptl;	// Accept-Language: ru
//	MString *acceptc;
//	MString *accepte;
	MString acceptc;	// 
	MString accepte;	// Accept-Encoding: gzip, deflate
	MString cookie;		// Cookie: CPSRPR1=EcgWvoI1Z5cs; CPSR=%3Cnick%3AcuulOETJnQ%3D%3D%3E%3Ctextcol%3AVQ%3D%3D%3E%3Cun%3AWA%3D%3D%3E%3Cskin%3AmfSpQ0HhsA%3D%3D%3E; hotlog=1
	MString connection;	// Connection: Keep-Alive
	MString useragent;	// User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)
	MString host;		// Host: 127.0.0.1

	//Server
//	MString http;
	MString code;		// 404 Not Found
	MString date;		// Date: Sun, 10 Oct 2004 12:50:42 GMT
	MString server;		// Server: Apache/2.0.44 (Win32) PHP/4.3.1
//	MString connection;
	MString acceptr;	// Accept-Ranges: bytes
	MString xpoweredby;	// X-Powered-By: PHP/4.3.1
	MString contentt;	// Content-Type: text/html; charset=iso-8859-1
	MString contentl;	// Content-Language: ru
	MString contentle;	// Content-Length: 287
	MString pragma;		// Pragma: no-cache
	MString cachec;		// Cache-Control: no-cache, must-revalidate, max-age=0
	MString expires;	// Expires: Thu, 01 Jul 1970 00:00:00 GMT
	MString lastmodified;	// Last-Modified: 
	MString etag;		// ETag: "75c54fc0-135-9b594d80"
	MString keepalive;	// Keep-Alive: timeout=15, max=99
} IHeader;

struct Connect{
MString ip;
int port;
int socks;
};

IHeader LoadHeader(IHeader header, IHeader head){
header.http=head.http;
header.code="404 Not Found";
header.server="Server: MSVServer/1.0.0 !Impossible MSL/1.0.0";
return header;	   
}


IHeader Header(IHeader &head, MString val ){
MString v[2];
explode(v, val, " ", 1, 1);
	
	if (v[0]=="HCode:")  head.http=v[1];
	if (v[0]=="Code:")  head.code=v[1];
	if (v[0]=="Accept:")  head.accept=val;
	if (v[0]=="Accept-Language:")  head.acceptl=val;
	if (v[0]=="Accept-Encoding:")  head.accepte=val;
	if (v[0]=="Accept-Ranges:")  head.acceptr=val;
	if (v[0]=="Cookie:")  head.cookie=val;
	if (v[0]=="Connection:")  head.connection=val;
	if (v[0]=="Host:")  head.host=val;

	if (v[0]=="Date:")  head.date=val;
	if (v[0]=="Server:")  head.server=val;
	if (v[0]=="Last-Modified:")  head.lastmodified=val;
	if (v[0]=="Content-Language:")  head.contentl=val;
	if (v[0]=="Content-Length:")  head.contentle=val;
	if (v[0]=="Expires:")  head.expires=val;
	if (v[0]=="X-Powered-By:")  head.xpoweredby=val;
	if (v[0]=="Content-Type:")  head.contentt=val;
	if (v[0]=="Pragma:")  head.pragma=val;
	if (v[0]=="Cache-Control:")  head.cachec=val;
	if (v[0]=="ETag:")  head.etag=val;
	if (v[0]=="Keep-Alive:")  head.keepalive=val;

return head;
}


MString SendHeader(IHeader head, int socket){
MString rn=MString(13)+MString(10);
MString hsend;

hsend +=	head.http+" "+head.code+rn;
hsend += 	head.connection!="" ? head.connection+rn : "";
hsend += 	head.date!="" ? head.date+rn : "";
hsend += 	head.server!="" ? head.server+rn : "";
hsend += 	head.lastmodified!="" ? head.lastmodified+rn : "";
hsend += 	head.contentl!="" ? head.contentl+rn : "";
hsend += 	head.contentle!="" ? head.contentle+rn : "";
hsend += 	head.expires!="" ? head.expires+rn : "";
hsend += 	head.xpoweredby!="" ? head.xpoweredby+rn : "";
hsend += 	head.contentt!="" ? head.contentt+rn : "";
hsend += 	head.pragma!="" ? head.pragma+rn : "";
hsend += 	head.cachec!="" ? head.cachec+rn : "";
hsend += 	head.etag!="" ? head.etag+rn : "";
hsend += 	head.keepalive!="" ? head.keepalive+rn : "";
hsend +=	rn;

//send(socket, hsend.rchar(), hsend.GetLength(), MSG_DONTROUTE);

return hsend;
}