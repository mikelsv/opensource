// http://www.cs.columbia.edu/sip/drafts/draft-ietf-xmpp-core-03.txt
// http://xmpp.org/extensions/xep-0220.html

#define XMPP_TYPE_SERV	1
#define XMPP_TYPE_CLI	1

VString XmppGetRequest(VString xml);

class listen_xml : public storm_core_ssl{ public:
	int wstat;

	// xmpp
	MString from, to;
	int xmpp_state;

public:

	virtual void storm_recv(storm_work_el &wel, VString read){
		VString line;
		while(line=XmppGetRequest(read)){
			PreAnalysXmlLine(wel, line);
			wel.readed(read, line);
		}

		return ;
	}

	void PreAnalysXmlLine(storm_work_el &wel, VString line){
		XDataCont ct; HLString ls; Itos it;
		ct.Read(line);

		print(HLString() + " ????? (" + xmpp_state + ") " + line + "\r\n\r\n");
		
		XDataEl *el=ct;
		for(el; el; el=el->n()){
			if(xmpp_state)
				AnalysXmlLineCli(wel, el);
			else
				AnalysXmlLineServ(wel, el);
		}

		return ;
	}

	virtual void AnalysXmlLineCli(storm_work_el &wel, XDataEl *el){
		return ;
	}

	virtual void AnalysXmlLineServ(storm_work_el &wel, XDataEl *el){
		return ;
	}

	STORM_DEFINE_NEW_DELETE(listen_xml);
};

STORM_DEFINE_NEW_DELETE_OK(listen_xml);


VString XmppGetRequest(VString xml){
	unsigned char *ln=xml, *to=xml.endu();
	int l=0, q=0, cl=0;
	unsigned char *lln=ln, *dln=0;
	unsigned char*line=ln, *npos=line;

	for(line; line<to; line++){
/// <<<move tab
if(q){
	if(q==1 && *line=='>'){ q=0; if(line>ln && *(line-1)=='/') cl=1; if(line>ln && *(line-1)=='?') cl=2; }
	else if(q==2) {if(*line=='\'') q=1;} // '
	else if(q==3) {if(*line=='"') q=1;} // ;
	else if(*line=='\'') q=2; else if(*line=='"') q=3;
} else{
	if(*line=='<'){ q=1;
		if(cl){
			l--;
			if(!l)
				if(line-ln){ xml=xml.str(line-ln); return VString(ln, line-ln); ln=line; }
				else npos=line;
			cl=0;
		}
		if(line+1<to && *(line+1)=='/') cl=1; else l++;
		if(incompare(line, to-line, (unsigned char*)"<stream:stream", 14)){ cl=1; } // l--; }
		if(l<0) l=0;
	}
}
/// tab move>>>
	}
	if(cl){
		l--;
		if(!l)
			if(line-ln){ xml=xml.str(line-ln); return VString(ln, line-ln); ln=line; }
			else npos=line;
	}

	return VString();
}
