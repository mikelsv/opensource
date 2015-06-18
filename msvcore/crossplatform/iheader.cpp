// IHeader
class IHeader{
public:
	TLine head;
	TLine shead;
	MString code;

	static VString _rn;

	void Code(const VString& st);
	void Code(const int st);

	void Header(const VString &line);
	void Header(VString key, VString val);
	void SHeader(const VString &line, bool rep=0);

	void ReadHeader(VString head);

	int Send(int sock, int cl=0);
	int RSend(HLString& ls, int cl=0);
	unsigned int RSend(unsigned char *ret, unsigned int rsz, int cl=0);

	int SendHeader(const VString& method, const VString& path, const VString& http, int sock, int cl=0);
	int RSendHeader(const VString& method, const VString& path, const VString& http, HLString& ls, int cl=0);
	int RSendHeader(const VString& get, HLString& ls, int cl=0);
	int RSendHeaderO(HLString& ls, int cl);

	VString Get(const VString& line);
	VString SGet(const VString& line);

	void Clean();
};

VString IHeader::_rn="\r\n";


	void IHeader::Code(const VString& st) {code=st;}
	void IHeader::Code(const int st) {code=itos(st);}

	void IHeader::Header(const VString &line){
		MString tmp[2]; if(!line) return;
		//int err=explode(tmp, line, ": ", 1, 1);
		VString o; tmp[0]=PartLine(line, o, ": "); tmp[1]=o; int err= o ? 2 : 1;

		if(head.Find(tmp[0])) head.Find(tmp[0])->t=tmp[1];
		else
		if(err==2) head.insert(tmp[0], tmp[1]);
		else head.insert("", line);
	}

	void IHeader::Header(VString key, VString val){
		DTLine *dt=head.Find(key);
		if(dt) dt->t=val;
		else
			head.insert(key, val);
		return ;
	}

	void IHeader::SHeader(const VString &line, bool rep){
		MString tmp[2]; if(!line) return;
		//int err=explode(tmp, line, ": ", 1, 1);
		VString o; tmp[0]=PartLine(line, o, ": "); tmp[1]=o; int err= o ? 2 : 1;

	// modes
		if(tmp[0]=="Location") Code("301");
		if(rep && shead.Find(tmp[0])) shead.Find(tmp[0])->t=tmp[1];
		else
		if(err==2) shead.insert(tmp[0], tmp[1]);
		else shead.insert("", line);
	}

	void IHeader::ReadHeader(VString head){
		VString o, t;
		while(head){
			t=PartLine(head, head, "\r\n");
			if(t) Header(t);
		}
		return ;
	}
	
	int IHeader::Send(int sock, int cl){ HLString ls; RSend(ls, cl); return ls.Send(sock); }

	int IHeader::RSend(HLString& ls, int cl){
		ls+"HTTP/1.1 "+code+"\r\n";
		DTLine*dtl=shead._a;
		while(dtl){
			ls+dtl->s+": "+dtl->t+"\r\n";
			dtl=dtl->_n;
		} if(cl) ls+"\r\n";
		return 1;
	}

	unsigned int IHeader::RSend(unsigned char *ret, unsigned int rsz, int cl){// prmf
		unsigned char *aret=ret, *rt=ret ? ret+rsz : 0; // define

		prmdadd_t("HTTP/1.1 "); prmdadd(code); prmdadd(_rn);
			
		DTLine*dtl=shead._a;
		while(dtl){
			prmdadd(dtl->s); prmdadd_t(": "); prmdadd(dtl->t); prmdadd(_rn);
			dtl=dtl->_n;
		}
		if(cl) prmdadd(_rn);
		return ret-aret;
	}

	int IHeader::SendHeader(const VString& method, const VString& path, const VString& http, int sock, int cl){
		HLString ls; RSendHeader(method, path, http, ls, cl); return ls.Send(sock);
	}

	int IHeader::RSendHeader(const VString& method, const VString& path, const VString& http, HLString& ls, int cl){
		ls+method+" "+path+" "+http+"\r\n";
		RSendHeaderO(ls, cl);
		//DTLine*dtl=head._a;
		//while(dtl){
		//	ls+dtl->s+": "+dtl->t+_er;
		//	dtl=dtl->_n;
		//} if(cl) ls+_er;
		return 1;
	}

	int IHeader::RSendHeaderO(HLString& ls, int cl){
		DTLine*dtl=head._a;
		while(dtl){
			ls+dtl->s+": "+dtl->t+"\r\n";
			dtl=dtl->_n;
		} if(cl) ls+"\r\n";
		return 1;
	}

	int IHeader::RSendHeader(const VString& get, HLString& ls, int cl){
		ls+get+"\r\n";
		RSendHeaderO(ls, cl);
		//DTLine*dtl=head._a;
		//while(dtl){
		//	ls+dtl->s+": "+dtl->t+_er;
		//	dtl=dtl->_n;
		//} if(cl) ls+_er;
		return 1;
	}

	VString IHeader::Get(const VString& line){
		DTLine*dtl=head.Find(line);
		if(dtl) return dtl->t;
		return "";
	}

	VString IHeader::SGet(const VString& line){
		DTLine*dtl=shead.Find(line);
		if(dtl) return dtl->t;
		return "";
	}

	void IHeader::Clean(){ head.delall(); shead.delall(); code.Clean(); }



//#include "HTTP.cpp"
