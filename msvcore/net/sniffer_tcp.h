#define SNIFFER_TCPT_MC		1
#define SNIFFER_TCPT_R		2

class sniffer_tcp_d{
public:
	sniffer_tcp_d *_p, *_n;
	// 
	sniffer_tcp_d(){ snd=0; rcv=0; sndb=0; rcvb=0; type=0; }

	// info
	unsigned int seq, ack_seq, rseq, rack_seq; unsigned int ltime, type;

	// data
	unsigned int sip, dip; unsigned short sport, dport;
	int64 snd, rcv; int sndb, rcvb;
	SendDataL data;

	int r_type(){ return type&SNIFFER_TCPT_R; }
	void r_type(int i){ if(i) type|=SNIFFER_TCPT_R; else type&=~SNIFFER_TCPT_R; }

};

GLock sniffer_tcp_lock;

class sniffer_tcp: public OMatrixT<sniffer_tcp_d>{
protected:
	sniffer sniff;
	unsigned int fip;
	int isprint;

public:
	sniffer_tcp(){ isprint=0; fip=0; sniff.Start(); }

	int IsPrint(){ return isprint; }
	int IsPrint(int n){ return isprint=n; }

	int IsFilter(){ return fip; }
	int IsFilter(unsigned int n){ return fip=n; }

	sniffer_tcp_d* New(){
		UGLOCK(sniffer_tcp_lock);
		sniffer_tcp_d *p=new sniffer_tcp_d;
		p->ltime=time();
		OMAdd(p);
		return p;
	}

	sniffer_tcp_d* Find(int &r, unsigned int sip, unsigned int dip, unsigned short sport, unsigned short dport){
		UGLOCK(sniffer_tcp_lock);
		sniffer_tcp_d *p=_a; r=0;

		while(p){
			if(p->sip==sip && p->dip==dip && p->sport==sport && p->dport==dport){
				return p;
			}

			if(p->sip==dip && p->sip==dip && p->sport==dport && p->dport==sport){
				r=1; return p;

			}
			
			p=p->_n;
		}
		return 0;
	}

	int Size(){
		UGLOCK(sniffer_tcp_lock);
		sniffer_tcp_d *p=_a; int sz=0;

		while(p){				
			sz++; p=p->_n;
		}
		return sz;
	}

	void Del(sniffer_tcp_d *p){
		UGLOCK(sniffer_tcp_lock);
		OMDel(p);
		delete p;
		return ;
	}



// Do
	void Do(){
		sniff.noprint=1;

		while(1){
			if(!sniff.Do()) break;

			u16	ep = ntohs(*(u16*)(sniff.buf + 12));
#ifdef WIN32
			ep=0x0800;
#endif

			if(sniff.ip->protocol==IPPROTO_TCP && ep==0x0800){
				struct tcphdr *tcp=(tcphdr*)(sniff.ip+1); //unsigned int dsz=sniff.ipsz-((char*)tcp-(char*)sniff.buf+sizeof(tcphdr));
				unsigned int dsz=sniff.ipsz-sizeof(iphdr)-tcp->doff*4;

				//if(incompare(VString((char*)tcp+tcp->doff*4, dsz), "GET /")){
				//	print("> ", VString((char*)tcp+tcp->doff*4, dsz), "\r\n");
				//}

				Itos it;
				it.Format("\r\n%ip!:%d - %ip!:%d\r\n", (unsigned int)(htonl(sniff.ip->saddr)), htons(tcp->source),  (unsigned int)(htonl(sniff.ip->daddr)), htons(tcp->dest));
				print(it);

				if(fip && sniff.ip->saddr!=fip && sniff.ip->daddr!=fip) continue;

				//if(sniff.ip->saddr!=GetIPh("192.168.0.253") && sniff.ip->daddr!=GetIPh("192.168.0.253")) continue;

				//print("D!\r\n");
				 it.Format("Flags: syn %d ack %d fin %d rst %d psh %d urg %d ece %d cwr %d\r\n", tcp->syn, tcp->ack, tcp->fin, tcp->rst, tcp->psh, tcp->urg, tcp->ece, tcp->cwr);
				print(it);


				unsigned int seq=htonl(tcp->seq), aseq=htonl(tcp->ack_seq);

				// close connection
				if(tcp->fin){
					int r; sniffer_tcp_d *c=Find(r, sniff.ip->saddr, sniff.ip->daddr, tcp->source, tcp->dest);
					//unsigned int s=htonl(tcp->seq), a=htonl(tcp->ack_seq);
					if(c && c->seq==htonl(tcp->seq) && c->ack_seq==htonl(tcp->ack_seq)){
						close(c);
						Del(c);
					}
					//print("SZ: ", itos(Size()), "\r\n");

					continue;
				}
				 
				// new connection // SYN
				if(tcp->syn && !tcp->ack){
					sniffer_tcp_d *c=New();
					
					c->sip=sniff.ip->saddr; c->dip=sniff.ip->daddr; c->sport=tcp->source; c->dport=tcp->dest;
					c->seq=seq; c->ack_seq=aseq; c->r_type(0); c->ltime=time();
					syn(c);
					continue;
				}

				// SYN ASK
				if(tcp->syn && tcp->ack){
					int r;
					sniffer_tcp_d *c=Find(r, sniff.ip->saddr, sniff.ip->daddr, tcp->source, tcp->dest);
					
					if(c){ c->r_type(r); synask(c); }

					if(c && r && c->seq+1==htonl(tcp->ack_seq)){
						c->ack_seq=htonl(tcp->seq); c->seq++; c->ack_seq++;
						c->r_type(0); connect(c); c->ltime=time();
					}
					else{

					}
				}

				// RECV
				if(!tcp->syn && tcp->ack){
					int r; //unsigned int s=htonl(tcp->seq), a=htonl(tcp->ack_seq);
					sniffer_tcp_d *c=Find(r, sniff.ip->saddr, sniff.ip->daddr, tcp->source, tcp->dest);
					if(c){
						if(c->seq+1==seq && !c->ack_seq && !dsz){ c->ack_seq=aseq; }
						if(c->seq+1==seq && !r && !dsz){ c->seq++; } 

						c->r_type(r); ask(c); c->ltime=time();


						//if(!dsz && r && c->seq+1==htonl(tcp->ack_seq)){
						//	c->seq++;
						//}
					
						if(dsz && tcp->psh){
							c->r_type(r);

							int rs=tcp->res1;
							if(isprint) printf("R: (%d) %d\r\n", dsz, tcp->doff);

							if(r && c->seq==aseq){
								c->ack_seq+=dsz;
								recv(c, ((char*)tcp)+tcp->doff*4, dsz);
							}
							if(!r && c->ack_seq==aseq){
								c->seq+=dsz;
								send(c, ((char*)tcp)+tcp->doff*4, dsz);
							}
						}
					}
				}
			}
		}
		return ;
	}

// Call
	virtual void syn(sniffer_tcp_d *c){
		if(!isprint) return ;
		print("SYN: "); printsdip(c); print("\r\n");
	}
	virtual void synask(sniffer_tcp_d *c){
		if(!isprint) return ;
		print("SYNASK: "); printsdip(c); print("\r\n");
	}
	virtual void ask(sniffer_tcp_d *c){
		if(!isprint) return ;
		print("ACK: "); printsdip(c); print("\r\n");
	}

	virtual void connect(sniffer_tcp_d *c){
		if(!isprint) return ;
		print("Connect: "); printsdip(c); print("\r\n");
	}
	virtual void mconnect(sniffer_tcp_d *c){
		if(!isprint) return ;
		print("May Connect: "); printsdip(c); print("\r\n");
	}

	virtual void send(sniffer_tcp_d *c, char*data, int sz){
		if(!isprint) return ;
		print("Send: "); printsdip(c); print("\r\n");
		print("Data: "); print(VString(data, sz)); print("\r\n");
	}
	virtual void recv(sniffer_tcp_d *c, char*data, int sz){
		if(!isprint) return ;
		print("Recv: "); printsdip(c); print("\r\n");
		print("Data: "); print(VString(data, sz)); print("\r\n");
	}
	virtual void close(sniffer_tcp_d *c){
		if(!isprint) return ;
		print("Close: "); printsdip(c); print("\r\n");
	}

// print
	virtual void printsdip(sniffer_tcp_d *c){
		if(!isprint) return ;
		Itos it;
		if(!c->r_type()) it.Format("%ip!:%d - %ip!:%d", (unsigned int)(htonl(c->sip)), htons(c->sport), (unsigned int)(htonl(c->dip)), htons(c->dport));
					else it.Format("%ip!:%d - %ip!:%d", (unsigned int)(htonl(c->dip)), htons(c->dport), (unsigned int)(htonl(c->sip)), htons(c->sport));
		print(it);
	}


// Destroy
	~sniffer_tcp(){ Clear(); }

	void Clear(){
		UGLOCK(sniffer_tcp_lock);
		sniffer_tcp_d *p=_a, *d=p;
		while(p){ d=p; p=p->_n; delete d; OMDel(d); }
	}

};