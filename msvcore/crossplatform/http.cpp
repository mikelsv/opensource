#include "http-old.cpp"

struct httpval{
	httpval *n; unsigned int k, v; char data[0];
	httpval* Next(httpval *&n){ if(n) n=n->n; return n;}
	VString key(){ if(!this) return VString(); return VString((char*)&data, k); }
	VString val(){ if(!this) return VString(); return VString(&data[k], v); }
	httpval* FindV(const VString &ln){
		httpval *p=(httpval*)this;
		while(p){ if(compare(p->data, p->k, ln, ln)) return p; p=p->n; }
		return 0;
	}
	httpval* FindVU(const VString ln){
		httpval *p=(httpval*)this;
		while(p){ if(compareu(p->data, p->k, ln, ln)) return p; p=p->n; }
		return 0;
	}

	VString Find(const VString &ln){
		httpval *p=(httpval*)this;
		while(p){ if(compare(p->data, p->k, ln, ln)) return p->val(); p=p->n; }
		return VString();
	}

	httpval* Insert(HLString &data, httpval *&v, const VString &key, const VString &val){
		httpval *d=(httpval*)&data.el(data.addnf(0, sizeof(void*)+sizeof(int)*2+key.size()+val.size()));
		//char *p=d+sizeof(void*);
		d->k=key.size(); d->v=val.size();
		memcpy((char*)&d->data, key, key);
		memcpy(&d->data[key.size()], val, val);
		if(!v){ v=d; return d; }
		httpval *el=(httpval*)v; 
		while(el){ if(!el->n){ el->n=(httpval*)d; break; } el=el->n; }
		return d;
	}

};

struct httpfile{
	httpfile *n;
	VString name, fname, data;
	
	httpfile* Find(const VString &nm){
		httpfile *p=(httpfile*)this;
		while(p){ if(compare(nm, nm, p->name, p->name)) return p; p=p->n; }
		return 0;
	}
};

class MCookieD{
	MString data;
public:
	VString domain, path, name, value;
	int *expires;

	void Set(VString _domain, VString _path, VString _name, VString _value, int _expires){
		domain=_domain; path=_path; name=_name; value=_value;
		VString exp((char*)&_expires, sizeof(int));
		data.AddR(domain, path, name, value, exp);
		expires=(int*)exp.data;
		return ;
	}


};

class MCookie{
	OList<MCookieD> cookies;

public:
	MCookie(){}
	MCookie(MCookie &cookie){ Copy(cookie); }
	~MCookie(){ Clear(); }

	void Clear(){ cookies.Clear(); }

	MCookie& operator=(MCookie &cookie){
		Copy(cookie);
		return *this;
	}

protected:
	MCookieD* _Get(VString domain, VString path, VString name){
		MCookieD *p=0;
		while(p=cookies.Next(p)){
			if(p->domain==domain && p->path==path && p->name==name)
				return p;
		}
		return p;
	}

public:
	// Get / Set
	void Set(VString domain, VString path, VString name, VString value, int expires=S2GM){
		UGLOCK(cookies);
		MCookieD*p=_Get(domain, path, name);
		if(!p)
			p=cookies.New();
		p->Set(domain, path, name, value, expires);
		return ;
	}

	VString Get(VString domain, VString path, VString name){
		UGLOCK(cookies);
		MCookieD *p=0;
		while(p=cookies.Next(p)){
			if(p->domain==domain && p->path==path && p->name==name)
				return p->value;
		}
		return VString();
	}

	MString Get(VString domain, VString path){
		HLString ls;
		MCookieD *p=0, *d;
		int tm=time();

		while(p=cookies.Next(p)){
			if(*p->expires<tm){
				// d=p; p=cookies.Prev(p); cookies.Del(d);
				continue;
			}

			if((domain.str(-p->domain.sz)==p->domain || p->domain[0]=='.' && p->domain.str(1)==domain) && path.incompare(p->path))
				ls+p->name+"="+p->value+"; ";
		}
		return VString(ls.oneline(), ls.size());
	}

	int GetTime(VString line){
		MTime mt;
		return mt.gmttotime(line);
	}

	MString GetTime(int tm){
		MTime mt;
		return mt.date("r", tm, 1);
	}

	void Set(VString line, VString _domain, VString _path){
		VString name, value, domain=_domain, path=_path, o, t;
		int expires=S2GM, first=1;

		// username=aaa13; expires=Friday, 31-Dec-99 23:59:59 GMT; path=/; domain=www.citforum.ru;\n\n";
		while(line){
			dspacev(line, 7);
			o=PartLine(line, line, ";");
			o=PartLine(o, t, "=");

			if(first){
				name=o; value=t;
				first=0;
			}else if(o=="path")
				path=t;
			else if(o=="domain")
				domain=t;
			else if(o=="expires")
				expires=GetTime(t);
			else if(o=="secure")
				{} // none
		}

		if(domain && path && name)
			cookies.New()->Set(domain, path, name, value, expires);

		return ;
	}

	void Copy(MCookie &cookie, int clear=1){
		UGLOCKD(cookies, cookie.cookies);

		if(clear)
			Clear();

		MCookieD *p=0, *e;
		while(p=cookie.cookies.Next(p)){
			e=_Get(p->domain, p->path, p->name);
			if(!e)
				e=cookies.New();
			e->Set(p->domain, p->path, p->name, p->value, *p->expires);			
		}

		return ;
	}

	MString Export(){
		UGLOCK(cookies);
		HLString ls;
		
		MCookieD *p=0;
		while(p=cookies.Next(p)){
			ls+p->name+"="+p->value+"; expires="+GetTime(*p->expires)+"; path="+p->path+"; "+"domain="+p->domain+"\r\n";
		}

		return VString(ls.oneline(), ls.size());
	}

	MString ExportDO(){
		UGLOCK(cookies);
		HLString ls;
		
		MCookieD *p=0;
		while(p=cookies.Next(p)){
			ls+p->name+"="+p->value+"; ";
		}

		return VString(ls.oneline(), ls.size());
	}

	void Import(VString line){
		UGLOCK(cookies);

		while(line){
			VString o=PartLine(line, line, "\r\n");
			Set(o, "", "");
		}

		return ;
	}


};


class MHttp{
public:
	VString head, post, method, path, http;
	ILink ilink;
	httpval *input, *iget, *ipost, *cookies, *headers; httpfile *ifile;
	HLString data;
	MCookie cookie;

	MHttp(){
		input=0; iget=0; ipost=0; cookies=0; headers=0; ifile=0;
	}

	void Clean(){
		input=0; iget=0; ipost=0; cookies=0; headers=0; ifile=0;
		head.Clean(); post.Clean(); method.Clean(); path.Clean(); http.Clean(); ilink.Clean(); data.Clear();
	}

	void Set(VString line){
		head=PartLine(line, post, "\r\n\r\n");
		method=PartLineO(head, "\r\n");
		method=PartLine(method, path, " ");
		path=PartLine(path, http, " ");

		MString p=NormalPath(path); ilink.Link(p, 1);
		
	if(method=="GET"){
		for(int i=0; i<ilink.args; i++){
			MString v=HTI(ilink.ilinkq[i].val);
			DataAddKV(iget, ilink.ilinkq[i].key, v);
			v=CleanVal(v);
			DataAddKV(input, ilink.ilinkq[i].key, v);
			//input.insert(ilink.ilinkq[i].key, CleanVal(()));
		}
	}
	if(method=="POST"){
		ilink.SetQuest(post);
		for(int i=0; i<ilink.args; i++){
			MString v=HTInp(ilink.ilinkq[i].val);
			DataAddKV(ipost, ilink.ilinkq[i].key, v);
			v=CleanVal(v);
			DataAddKV(input, ilink.ilinkq[i].key, v);
		//input.insert(ilink.ilinkq[i].key, CleanVal(HTI(ilink.ilinkq[i].val)));
		}
	}

	VString ln=head;
	while(ln){
		VString o, t;
		o=PartLine(ln, ln, "\r\n");
		o=PartLine(o, t, ": ");
		DataAddKV(headers, o, t);
		if(o=="Set-Cookie"){
			VString k, v;
			k=PartLine(t, v, ";");
			DataAddKV(cookies, k, v);
			// new
			cookie.Set(t, "", "");
		}
	}

		return ;
	}


	void* DataAddKV(httpval *&v, const VString &key, const VString &val){
		httpval *d=(httpval*)&data.el(data.addnf(0, sizeof(void*)+sizeof(int)*2+key.size()+val.size()));
		//char *p=d+sizeof(void*);
		d->k=key.size(); d->v=val.size();
		memcpy((char*)&d->data, key, key);
		memcpy(&d->data[key.size()], val, val);
		if(!v){ v=d; return d; }
		httpval *el=(httpval*)v; while(el){ if(!el->n){ el->n=(httpval*)d; break; } el=el->n; }
		return d;
	}

	VString Input(VString key){
		return input->Find(key);
	}

	VString GetHead(VString name){
		return headers->Find(name);
	}

	int GetCode(){
		return stoi(head.str(9, 3));
	}

	MString GetCookies(){
		HLString ls;
		httpval *c=cookies;
		while(c){
			ls+c->key()+"; ";
			c=c->n;
		}
		return ls;
	}

	MString GetCookie(VString name){
		httpval *c=cookies;
		while(c){
			if(PartLineO(c->key(), "=")==name) return PartLineOT(c->key(), "=");
			c=c->n;
		}
		return VString();
	}

};


class GetHttp{
	// ip
	ConIp ip;
	// page
	MString r_method, r_agent, r_accept, r_post, r_boundary_id, r_boundary, r_cookies, page;
	// result: http data
	MHttp http;
	// return
	MString head, data;
	// traffic
	int64 arecv, asend;
	int timeout;
	int repeat;
	int usessl;
	int minimal;

#ifdef USEMSV_OPENSSL
	MySSL ssl;
#endif

public:
	GetHttp(){ timeout=30; repeat=1; usessl=0; minimal=0; arecv =0; asend = 0; }
	void Clean(){ r_method.Clean(); r_agent.Clean(); r_post.Clean(); r_boundary_id.Clean(); r_boundary.Clean(); r_cookies.Clean(); http.Clean(); }

	void SetMethod(VString v){ r_method=v; }
	VString GetMethod(){ return r_method; }

	void SetAgent(VString v){ r_agent=v; }
	VString GetAgent(){ return r_agent; }
	void SetAccept(VString v){ r_accept=v; }
	VString GetAccept(){ return r_accept; }

	void SetPost(VString v){ r_post=v; }
	VString GetPost(){ return r_post; }

	void SetBoundary(VString id, VString v){ r_boundary_id = id; r_boundary = v; }
	//VString GetPost(){ return r_post; }

	void SetMinimal(){ minimal=1; }

	MHttp& GetHead(){ return http; }
	VString GetHead(VString name){ return http.GetHead(name); }
	VString GetData(){ return data; }

	void SetCookies(VString v){ r_cookies=v; }
	VString GetCookies(){ return r_cookies; }

	void SetUseSSL(int v){ usessl=v; }
	int SetUseSSL(){ return usessl; }

	void SetTimeout(int tm){ timeout=tm; }
	void SetRepeat(int val){ repeat=val; }

	static MString EncodeUrl(VString line){
		return Replace(Replace(Replace(Replace(line,
			" ", "%20"),
			"!", "%21"),
			"<", "%3C"),
			">", "%3E")
			;		
	}

	static MString EncodePar(VString line){
		return Replace(Replace(Replace(Replace(Replace(Replace(Replace(line,
			" ", "%20"),
			"!", "%21"),
			"<", "%3C"),
			">", "%3E"),
			"&", "%26"),
			"+", "%2B"),
			"#", "%23")
			;		
	}

	void AddBoundary(HLString &ls, VString bnid, VString key, VString val){
		ls + "--" + bnid + "\r\n" + "Content-Disposition: form-data; name=\"" + key + "\"" "\r\n" "\r\n" + val + "\r\n";
	}

	void AddBoundaryFile(HLString &ls, VString bnid, VString key, VString name, VString val){
		ls + "--" +  bnid + "\r\n" + "Content-Disposition: form-data; name=\"" + key + "\"; " +  "filename=\"" + name + "\"" "\r\n" + "Content-Type: application/octet-stream" "\r\n" "\r\n" + val + "\r\n";
	}

	void AddBoundaryEnd(HLString &ls, VString bnid){
		ls + "--" + bnid + "--" "\r\n";
	}

	int Request(VString url){
		int ret = 0, r = repeat + 1;
		while(r){
			ret=DoRequest(url);
			if(ret)
				break;
			r--;

			if(r)
				print("[!!!!!] Repeat\r\n");
		}
		return ret;
	}

	int GetRecvSz(){ return arecv; }
	int GetSendSz(){ return asend; }
	
protected:
	int DoRequest(VString url){
		HLString ls; ILink il; int snd;
		arecv = 0; asend = 0;

		il.Link(url);
#ifdef USEMSV_OPENSSL
		if(il.proto=="https://"){
			if(!il.port)
				il.port=443;
			usessl=1;
		}
#endif
		// ls+ r_method ? r_method : ( r_post ? "POST" : "GET" );
		if(r_method)
			ls + r_method;
		else
			if(r_post || r_boundary)
				ls + "POST";
			else
				ls + "GET";
		//ls+" "+EncodeUrl(il.GetPathFileQuest())+" HTTP/1.0\r\n";
		ls+" "+il.GetPathFileQuest()+" HTTP/1.0\r\n";
		
			ls+"Host: "+il.domain+"\r\n";
		if(!minimal){			
			//ls+"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20100101 Firefox/21.0\r\n";
			
			ls + "User-Agent: ";
			if(r_agent)
				ls + r_agent;
			else
				ls +"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20100101 Firefox/22.0";
			ls+"\r\n";

			ls + "Accept: ";
			if(r_accept)
				ls + r_accept;
			else
				ls + "*/*";
			ls+"\r\n";

			//if(r_boundary_id && r_boundary){
			//	ls + "Content-Type: multipart/form-data; boundary=" + r_boundary_id + "\r\n";
			//}

			//ls+"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
			//ls+"Accept: */*\r\n";
			//ls+"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\r\n";
			ls+"Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3\r\n";
			//ls+"Accept-Encoding: gzip, deflate\r\n";
			ls+"Accept-Encoding: none\r\n";
			ls+"Connection: close\r\n";
		}

		if(r_cookies){
			ls+"Cookie: "+r_cookies+"\r\n";
		}
		
		if(r_post){
			ls+"Content-Length: " + r_post.size() + "\r\n";
			ls+"Content-Type: application/x-www-form-urlencoded\r\n";
		}

		if(r_boundary){
			ls + "Content-Length: " + r_boundary.size() + "\r\n";
			ls + "Content-Type: multipart/form-data; boundary=" + r_boundary_id + "\r\n";
		}


		ls+"\r\n";

//		print(VString(ls.oneline(), ls.size()));
//		print(r_post, "\r\n");

		ip.Ip(il.domain); ip.port= il.port ? il.port : 80;
		SOCKET sock=ip.Connect();
		if(sock<=0)
			return 0;

#ifdef USEMSV_OPENSSL
		if(usessl){
			usessl=ssl.Connect(sock);
		}
#endif

		snd=send(sock, ls.oneline(), ls.size());
		if(!snd)
			return 0;

		asend += snd;

		if(r_post){
			snd = send(sock, r_post, r_post);
			asend += snd;
		}

		if(r_boundary){
			snd = send(sock, r_boundary, r_boundary);
			asend += snd;
		}

		if(!snd)
			return 0;

		int ret=Recv(sock);

#ifdef USEMSV_OPENSSL
			ssl.close();
#endif
		closesocket(sock);

		return ret;
	}

protected:
	int Recv(SOCKET sock){
		unsigned char buf[S8K]; int bufs=0, bufms=sizeof(buf);
		HLString ls; int rdata=0, cl=0;
		int tm=time(), ltm=tm;

		while(1){
			if(bufs==bufms){
//				print("Buffer full\r\n");
				return 0;
			}

			// timeout
			ltm=time();
			if(ltm-tm>timeout) return 0;

			// recv
			if(!ifrecv(sock, 300)) continue;
			int rcv=recv(sock, (char*)buf+bufs, bufms-bufs);
			if(rcv<0)
				return 0;

			arecv += rcv;
			tm=ltm;

			if(rcv==0){
				if(rdata && (!cl || ls.size()>=cl)){
					http.Set(VString(buf, bufs));
					ls.String(this->data);
//					print("\r\n\r\n", this->data);
					return 1;
				}
				return 0;
			}
			
			if(!rdata)
				bufs+=rcv;
			else{
				ls+VString((char*)buf+bufs, rcv);
				if(cl && ls.size()>=cl){
					http.Set(VString(buf, bufs));
					ls.String(this->data);
//					print("\r\n\r\n", this->data);
					return 1;
				}
				continue;
			}

			VString data(buf, bufs);
			if(!PartLineOT(data, "\r\n\r\n").data) continue;

			// Cl
			cl=PartLineDouble(data, "Content-Length: ", "\r\n").toi();
			VString d=PartLineOT(data, "\r\n\r\n");
			if(d.sz<=cl || !cl){
				ls+d; bufs-=d.sz; rdata=1;				
				
				if(cl && ls.size()>=cl){
					http.Set(VString(buf, bufs));
					ls.String(this->data);
					return 1;
				}		
				continue;
			}
			
			return 0;
		}

		return 0;
	}


	int send(SOCKET sock, const char *buf, int sz){
#ifdef USEMSV_OPENSSL
			if(usessl)
				return ssl.send(buf, sz);
#endif
			return ::send(sock, buf, sz, 0);
	}

	int recv(SOCKET sock, char *buf, int sz){
#ifdef USEMSV_OPENSSL
			if(usessl)
				return ssl.recv(buf, sz);
#endif
		return ::recv(sock, buf, sz, 0);
	}

};



class GetHttp2{
	// ip
	ConIp ip;
	// page
	MString r_method, r_agent, r_accept, r_post, r_cookies, r_referrer, page;
	// result: http data
	MHttp http;
	MCookie cookie;
	// return
	MString head, data;
	// traffic
	int timeout;
	int repeat;
	int usessl;

#ifdef USEMSV_OPENSSL
	MySSL ssl;
#endif

public:
	GetHttp2(){ timeout=30; repeat=1; usessl=0; }
	void Clean(){ timeout=30; repeat=1; usessl=0; r_method.Clean(); r_agent.Clean(); r_post.Clean(); r_cookies.Clean(); r_referrer.Clean(); http.Clean(); }
	void CleanCookie(){ cookie.Clear(); }

	void SetMethod(VString v){ r_method=v; }
	VString GetMethod(){ return r_method; }

	void SetAgent(VString v){ r_agent=v; }
	VString GetAgent(){ return r_agent; }
	void SetAccept(VString v){ r_accept=v; }
	VString GetAccept(){ return r_accept; }

	void SetPost(VString v){ r_post=v; }
	VString GetPost(){ return r_post; }

	MHttp& GetHead(){ return http; }
	VString GetData(){ return data; }
	VString GetHeadData(){ return head; }

	void SetCookies(VString v){ r_cookies=v; }
	VString GetCookies(){ return r_cookies; }

	MCookie& GetCookie(){ return cookie; }
	void SetCookie(MCookie &cook){ cookie=cook; }

	void SetReferrer(VString v){ r_referrer=v; }

	void SetUseSSL(int v){ usessl=v; }
	int SetUseSSL(){ return usessl; }

	void SetTimeout(int tm){ timeout=tm; }

	static MString EncodeUrl(VString line){
		return Replace(Replace(Replace(Replace(line,
			" ", "%20"),
			"!", "%21"),
			"<", "%3C"),
			">", "%3E")
			;		
	}

	static MString EncodePar(VString line){
		return Replace(Replace(Replace(Replace(Replace(Replace(line,
			" ", "%20"),
			"!", "%21"),
			"<", "%3C"),
			">", "%3E"),
			"&", "%26"),
			"+", "%2B")
			;		
	}

	void AnalysHead(VString line, VString domain, VString path){
		http.Clean();
		http.Set(line);

		VString o, t;
		while(line){
			o=PartLine(line, line, "\r\n");
			o=PartLine(o, t, ": ");

			if(o=="Set-Cookie")
				cookie.Set(t, domain, path);
		}		
	}

	int Request(VString url){
		int ret = 0, r = repeat + 1;
		while(r){
			ret=DoRequest(url);
			if(ret)
				break;
			print("[!!!!!] Repeat\r\n");
			r--;
		}
		return ret;
	}
	
protected:
	int DoRequest(VString url){
		HLString ls; ILink il; int snd;

		il.Link(url);
#ifdef USEMSV_OPENSSL
		if(il.proto=="https://"){
			usessl=1;
		}
		if(!il.port && usessl)
			il.port=443;
#endif
		// ls+ r_method ? r_method : ( r_post ? "POST" : "GET" );
		if(r_method) ls+r_method;
		else ls+ (r_post ? "POST" : "GET");
		//ls+" "+EncodeUrl(il.GetPathFileQuest())+" HTTP/1.0\r\n";
		ls+" "+il.GetPathFileQuest()+" HTTP/1.0\r\n";
		ls+"Host: "+il.domain+"\r\n";
		//ls+"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20100101 Firefox/21.0\r\n";
		ls+"User-Agent: "+ (r_agent ? r_agent : "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:31.0) Gecko/20100101 Firefox/22.0"); ls+"\r\n";
		ls+"Accept: "+ (r_accept ? r_accept : "*/*"); ls+"\r\n";
		//ls+"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
		//ls+"Accept: */*\r\n";
		//ls+"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\r\n";
		ls+"Accept-Language: ru-ru,ru;q=0.8,en-us;q=0.5,en;q=0.3\r\n";
		//ls+"Accept-Encoding: gzip, deflate\r\n";
		ls+"Accept-Encoding: none\r\n";
		ls+"Connection: close\r\n";

		if(r_cookies){
			ls+"Cookie: "+r_cookies+"\r\n";
		}else{
			ls+"Cookie: "+cookie.Get(il.domain, il.path)+"\r\n";
		}

		if(r_referrer)
			ls+"Referrer: "+r_referrer+"\r\n";

		if(r_post){
			ls+"Content-Length: " + r_post.size() + "\r\n";
			ls+"Content-Type: application/x-www-form-urlencoded\r\n";
		}

		ls+"\r\n";

//		print(VString(ls.oneline(), ls.size()));
//		print(r_post, "\r\n");

		ip.Ip(il.domain); ip.port= il.port ? il.port : 80;
		SOCKET sock=ip.Connect();
		if(sock<=0)
			return 0;

#ifdef USEMSV_OPENSSL
		if(usessl){
			usessl=ssl.Connect(sock);
		}
#endif

		snd=send(sock, ls.oneline(), ls.size());
		if(!snd)
			return 0;
		if(r_post)
			snd=send(sock, r_post, r_post);
		if(!snd)
			return 0;

		int ret=Recv(sock);
		if(ret){
			AnalysHead(head, il.domain, il.path);
		}

#ifdef USEMSV_OPENSSL
			ssl.close();
#endif
		closesocket(sock);

		return ret;
	}

protected:
	int Recv(SOCKET sock){
		unsigned char buf[S8K]; int bufs=0, bufms=sizeof(buf);
		HLString ls; int rdata=0, cl=0;
		int tm=time(), ltm=tm;
		int hread=0;

		while(1){
			if(bufs==bufms){
//				print("Buffer full\r\n");
				return 0;
			}

			// timeout
			ltm=time();
			if(ltm-tm>timeout)
				return 0;

			// recv
			if(!ifrecv(sock, 300)) continue;
			int rcv=recv(sock, (char*)buf+bufs, bufms-bufs);
			if(rcv<0)
				return 0;
			bufs+=rcv;

			tm=ltm;

			if(!hread){
				if(!rcv){
					head.Clean();
					ls.add((char*)buf, bufs);
					ls.String(data);
					return data;
				}

				if(!PartLineOT(VString(buf, bufs), "\r\n\r\n").data)
					continue;

				hread=1;
				head=PartLineO(VString(buf, bufs), "\r\n\r\n");
				
				memcpy(buf, buf+head.sz+4, bufs-(head.sz+4));
				bufs-=(head.sz+4);

				VString c=PartLineDouble(head, "Content-Length: ", "\r\n");
				cl= c.data ? c.toi() : -1;
			}

			ls.add((char*)buf, bufs);
			bufs=0;

			if(cl>=0 && ls.size()>=cl){
				ls.String(data);
				return 1;
			}

			if(!rcv)
				if(cl<0){
					ls.String(data);
					return 1;
				}
				else
					return 0;
		}

		return 0;
	}


	int send(SOCKET sock, const char *buf, int sz){
#ifdef USEMSV_OPENSSL
			if(usessl)
				return ssl.send(buf, sz);
#endif
			return ::send(sock, buf, sz, 0);
	}

	int recv(SOCKET sock, char *buf, int sz){
#ifdef USEMSV_OPENSSL
			if(usessl)
				return ssl.recv(buf, sz);
#endif
		return ::recv(sock, buf, sz, 0);
	}

};