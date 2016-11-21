class StormCoreVirtual;
struct storm_socket_data;
class storm_item;
class storm_socket;

//#define STORM_RECV_WAIT	(-2147483647)

// work element
struct storm_work_el{
	StormCoreVirtual *core;
	storm_socket_data *data;
	storm_item *item;

	int s_eread; // 1 - enable, -1 - disable;
	int s_read;
	int s_timeout;
	int s_close;
	int stat;

	// init
	storm_work_el(){
		s_read = 0;
		s_timeout = -1;
		s_close = 0;
		s_eread = 0;
	}

	// send
	void send(VString line);
	void send(const char*, unsigned int sz);

	// readed
	void readed(int sz){
		if(sz > 0)
			s_read += sz;
	}

	void readed(VString &read, unsigned int sz){
		if(sz > 0){
			s_read += sz;
			read.data += sz;

			if(sz <= read.sz)
				read.sz -= sz;
			else
				read.sz = 0;
		}
	}

	// enable read
	void eread(){
		s_eread = 1;
	}

	// disable read
	void dread(){
		s_eread = -1;
	}

	// timeout
	void timeout(int v){
		s_timeout = v;
	}

	// close
	void close(){
		s_close = 1;
	}

	// get ip
	void gettip(unsigned int &ip, unsigned short &port);
	void getcip(unsigned int &ip, unsigned short &port);
	SOCKET getsock();
};


class storm_item{
protected:
	//RainCore *core;
public:

	// Core
	//virtual void rain_setcore(RainCore *c){ core=c; }
	virtual void storm_init(){}
	virtual void storm_start(){}
	virtual void storm_stop(){}

	// Socket
	//virtual void storm_setcip(unsigned int ip, unsigned short port){}
	//virtual void storm_settip(unsigned int ip, unsigned short port){}
	//virtual void storm_setsock(SOCKET s){}
	virtual void storm_setsock(StormCoreVirtual *core, storm_socket_data *data){}
	//virtual void storm_setksock(rain3_socket_data *sock){}

	// Socket Work
	virtual int storm_socket_recv(SOCKET sock, char *data, int sz, int flag){
		return recv(sock, data, sz, flag);
	}
	virtual int storm_socket_send(SOCKET sock, char *data, int sz, int flag){
		return send(sock, data, sz, flag);
	}

	// Work
	virtual void storm_work(storm_work_el &wel){};
	//virtual void rain_recv(rain_work_el &wel, unsigned char*rdata, int rsz){}
	virtual void storm_recv(storm_work_el &wel, VString read){}
	virtual int storm_send(storm_work_el &wel, VString send){ return 0; }
	virtual void storm_timeout(storm_work_el &wel){ wel.close(); }
	virtual void storm_signal(storm_socket *ssock, int sid, void *data){ return ; }

	// New / Delete
	virtual storm_item* storm_new() = 0;
	virtual void storm_delete(storm_item *el) = 0;
};

#define STORM_DEFINE_NEW_DELETE(cls) static OList<cls> storm_items_list; \
	virtual storm_item* storm_new(){ return storm_items_list.New(); return 0; } \
	virtual void storm_delete(storm_item *el){ storm_items_list.Del((cls*)el); } \
	public: static storm_item* static_storm_new(){ return storm_items_list.New(); return 0; }

#define STORM_DEFINE_NEW_DELETE_OK(cls) OList<cls> cls::storm_items_list;

// socket data
struct storm_socket_data{
	SOCKET sock;
	storm_item *item;
	storm_socket_data *prev, *next;

	unsigned char rd, wr; // rd: 0 - no recv(), 1 - recv(), 2 - stop recv; wr: 0 - no send(), 1 - send();  read [--, x-, xx], Write [--, x-, xx].
	unsigned char stat, cl; // stat: AIFOP_ERR, AIFOP_TIME, AIFOP_LU. cl: close - close & del
	unsigned short wtime; // wait time.   [ADD] ltime to short.
	unsigned char unkid;
	unsigned int ltime; // last time, wait time.

	// recv, send
	RecvDataD rv;
	SendDataL sd;

	void eread(){
		rd &= ~2;
	}

	void send(VString line){
		sd.Set(line);
	}

	void close(){
		cl |= AIFOP_CLOSE;
	}

	void clean(){
		sock = 0;
		item = 0;
		next = 0;
		rd = 0;
		wr = 0;
		stat = 0;
		cl = 0;
		wtime = 0;
		ltime = 0;

		rv.Clear();
		sd.Clear();
		return ;
	}

	void gettip(unsigned int &ip, unsigned short &port){
		sockaddr_in from; int fromlen = sizeof(from);

		getsockname(sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

		ip = ntohl(from.sin_addr.s_addr);
		port = htons(from.sin_port);
	}

	void getcip(unsigned int &ip, unsigned short &port){
		sockaddr_in from; int fromlen = sizeof(from);

		getpeername(sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

		ip = ntohl(from.sin_addr.s_addr);
		port = htons(from.sin_port);
	}

};

struct storm_work_data{
	//storm_socket_data *prev;
	storm_socket_data *data;
	SOCKET sock;
};


// storm virtual core
class StormCoreVirtual{
public:
	virtual int StormRunCore() = 0;
	virtual int StormAddSock(storm_item *core, SOCKET sock, int init=1) = 0;
	virtual int StormSockIs(storm_socket_data *sdata, SOCKET sock, unsigned char unkid) = 0;
	virtual int StormEnableRead(storm_socket_data *sdata, SOCKET sock, unsigned char unkid) = 0;
	virtual int StormSendSignal(storm_socket_data *sdata, SOCKET sock, unsigned char unkid, int sid, void *sidata) = 0;
	virtual int StormSendData(storm_socket_data *data, SOCKET sock, unsigned char unkid, VString line) = 0;
	virtual int StormCloseSocket(storm_socket_data *data, SOCKET sock, unsigned char unkid) = 0;
};

class storm_socket{
	StormCoreVirtual *core;
	storm_socket_data *data;
	SOCKET sock;
	unsigned char unkid;

public:

	storm_socket(){
		core = 0;
		data = 0;
		sock = 0;
	}

	void set(StormCoreVirtual *core, storm_socket_data *data){
		if(core && data){
			this->core = core;
			this->data = data;
			this->sock = data->sock;
			this->unkid = data->unkid;
		}

		return ;
	}

	void set(storm_work_el &wel){
		return set(wel.core, wel.data);
	}

	void operator=(storm_work_el &wel){
		return set(wel);
	}

	int GetSock(){
		if(Is())
			return sock;
		return 0;
	}

	int Is(){
		if(!core)
			return 0;

		return core->StormSockIs(data, sock, unkid);
	}

	int GetConIp(unsigned int &cip, unsigned short &cport){
		if(!Is())
			return 0;

		getcip(sock, cip, cport);
		return 1;
	}

	int EnableRead(){
		if(!core)
			return 0;
		return core->StormEnableRead(data, sock, unkid);
	}

	int SendSignal(int sid, void *sdata = 0){
		if(!core)
			return 0;
		return core->StormSendSignal(data, sock, unkid, sid, sdata);
	}

	int SendData(VString line){
		if(!core)
			return 0;
		return core->StormSendData(data, sock, unkid, line);
	}

	int CloseSocket(){
		if(!core)
			return 0;
		return core->StormCloseSocket(data, sock, unkid);
	}

	void Clean(){
		core = 0;
		data = 0;
		sock = 0;
		unkid = 0;
	}
};


struct storm_socket_block{
	storm_socket_data data[STORMSERVER_SOCKETBLOCK_SZ];
	storm_socket_block *next;
};


class storm_work_ring{
	static const int ring_sz=256;
	storm_socket_data *data[ring_sz];
	unsigned int wid, rid;

public:
	storm_work_ring(){
		wid = 0; rid = 0;
	}

	int write(storm_socket_data *d){
		if(((wid+1)&(ring_sz-1)) == (rid&(ring_sz-1)))
			return 0;

		data[wid] = d;
		wid = (wid+1)&(ring_sz-1);
		return 1;
	}

	storm_socket_data *read(){
		if(wid == rid)
			return 0;

		storm_socket_data*d = data[rid];
		rid = (rid+1)&(ring_sz-1);
		return d;
	}

	bool is(){
		return wid != rid;
	}

	int freesz(){
		return (rid - 1 + ring_sz - wid) % ring_sz;
	}

};



// storm core
class StormCore : public StormCoreVirtual, public MSVCOT, public MSVMCOT{
	// sockets block
	storm_socket_block *socket_data;
	// free, select, work
	storm_socket_data *socket_free, *socket_select, *socket_work;
	// rings
	storm_work_ring select_ring, work_ring, select_ring_e;
	// lock
	TLock socket_free_lock, socket_work_lock;//, socket_epoll_lock;
	CLock cond_lock;
	// statistics
	int sockets_count, sockets_count_select;
	int is_socket_poll_work;

	// threads
	TLock threads_lock;
	static const int threads_max = STORMSERVER_THREADS_MAX;
	static const int threads_min = STORMSERVER_THREADS_MIN;
	int threads_cur;
	unsigned int threads[threads_max];

#ifdef STORMSERVER_POLL_EPOLL
	int epoll_fd;	

	int epoll_add_socket(storm_socket_data *data){
		struct epoll_event ev;
		ev.data.fd = data->sock;
        ev.data.ptr = data;
		ev.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
		int s = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, data->sock, &ev);
		return s != -1;
	}

	int epoll_mod_socket(storm_socket_data *sdata, int evs){
		struct epoll_event ev;
		ev.data.fd = sdata->sock;
        ev.data.ptr = sdata;
		ev.events = evs;
		int s = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sdata->sock, &ev);
		return s != -1;
	}

	int epoll_mod_socket(storm_socket_data *sdata){
		return epoll_mod_socket(sdata, (sdata->rd ? 0 : EPOLLIN) | (sdata->wr ? 0 : EPOLLOUT) | EPOLLONESHOT);
	}

	int epoll_del_socket(storm_socket_data *sdata){
		struct epoll_event ev;
		ev.data.fd = sdata->sock;
 //       ev.data.ptr = data;
		ev.events = 0;
		int s = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sdata->sock, &ev);
		return s != -1;
	}

	storm_socket_data* epoll_select(int tm = -1){
		 struct epoll_event ev;
		 int r = epoll_wait(epoll_fd, &ev, 1, tm);
		
		 //int stat = 0;
		 if(r != 1)
			 return 0;

		storm_socket_data *data = (storm_socket_data*)ev.data.ptr;
		if(ev.events & EPOLLIN)
			data->rd = 1;
		if(ev.events & EPOLLOUT)
			data->wr = 1;
		return data;
	}

#endif

public:
	StormCore(){
		// sockets
		socket_data = 0;
		socket_free = 0;
		socket_select = 0;
		socket_work = 0;

		// threads
		threads_cur = 0;
		memset(threads, 0, sizeof(threads));

		sockets_count = 0;
		sockets_count_select = 0;
		is_socket_poll_work = 0;

#ifdef STORMSERVER_POLL_EPOLL
		epoll_fd = epoll_create1(0);
#endif
	}

	int GetSocketsCount(){
		return sockets_count;
	}

	virtual int StormAddSock(storm_item *core, SOCKET sock, int init=1){
		if(init)
			core->storm_init();

		if(!NewSocketData(core, sock)){
			core->storm_stop();
			core->storm_delete(core);
			closesocket(sock);
		}

		// set non blocking		
#ifdef WIN32
		unsigned long v = 1;
		ioctlsocket(sock, FIONBIO, &v);
#else
		fcntl(sock, F_SETFL, O_NONBLOCK);
#endif

		return 0;
	}

	//virtual int StormDelSock(storm_core *core, SOCKET sock){
	//	

	//	return 0;
	//}

	// new & delete socket
	storm_socket_data* NewSocketData(storm_item *item, SOCKET sock){
		UGLOCK(socket_free_lock);

		// add free blocks
		if(!socket_free){
			storm_socket_block *d = socket_data;
			while(d && !d->next)
				d = d->next;

			if(!d)
				d = socket_data = new storm_socket_block;
			else
				d = d->next = new storm_socket_block;

			memset(d, 0, sizeof(d));

			for(int i = STORMSERVER_SOCKETBLOCK_SZ - 1; i >= 0; i--){
				d->data[i].next = socket_free;
				socket_free = &d->data[i];
			}

			d->next = 0;
		}

		if(!socket_free)
			return 0;

		storm_socket_data *data = socket_free;
		socket_free = socket_free->next;

		data->clean();
		data->item = item;
		data->sock = sock;
		data->ltime = time();
		data->wtime = 60;
		data->unkid ++;
		data->prev = 0;

		data->item->storm_start();
		data->item->storm_setsock(this, data);
		//d->stat = 0;
		//d->cl = 0; d->rd = 0; d->wr = 0;

#ifdef STORMSERVER_PRINT_ADD
		Itos it;
		print(it.Format("Storm Add(%d). Sockets:%d. Void: 0x%h\r\n", data->sock, sockets_count + 1, (unsigned long)data));
#endif

		//while(!select_ring.write(d))
		//	Sleep(1);

		// set socket in poll
		PollSocket(data);

		sockets_count++;
		return data;
	}

	void DelSocketData(storm_socket_data* sdata){

//#ifdef STORMSERVER_POLL_EPOLL
//		epoll_del_socket(data);
//#endif

#ifndef STORMSERVER_TEST_EMULATE
#ifdef STORMSERVER_POLL_EPOLL
		epoll_del_socket(sdata);
#endif
		// close socket
		closesocket(sdata->sock);
#endif

		socket_free_lock.Lock();

		sockets_count--;

#ifdef STORMSERVER_PRINT_DEL
		Itos it;
		print(it.Format("Storm Del(%d). Sockets:%d. Void: 0x%h\r\n", sdata->sock, sockets_count, (unsigned long)sdata));
#endif

		// stop
		sdata->item->storm_stop();

		// delete
		//data->item->storm_delete(data->item);
		storm_item *item = sdata->item;

		sdata->clean();
		sdata->next = socket_free;
		socket_free = sdata;

		socket_free_lock.UnLock();

		// real delete
		item->storm_delete(item);

		return ;
	}


	virtual int StormSockIs(storm_socket_data *sdata, SOCKET sock, unsigned char unkid){
		if(!sdata || !sock)
			return 0;

		UGLOCK(socket_free_lock);

		if(sdata->sock != sock || sdata->unkid != unkid)
			return 0;

		return 1;
	}

	virtual int StormEnableRead(storm_socket_data *sdata, SOCKET sock, unsigned char unkid){
		if(!sdata || !sock)
			return 0;

		UGLOCK(socket_free_lock);

		if(sdata->sock != sock || sdata->unkid != unkid)
			return 0;

		if(!sdata->item)
			return 0;

		sdata->eread();

		return 1;
	}

	virtual int StormSendSignal(storm_socket_data *sdata, SOCKET sock, unsigned char unkid, int sid, void *sidata){
		if(!sdata || !sock)
			return 0;

		UGLOCK(socket_free_lock);

		if(sdata->sock != sock || sdata->unkid != unkid)
			return 0;

		if(!sdata->item)
			return 0;

		storm_socket ssock;
		ssock.set(this, sdata);

		sdata->item->storm_signal(&ssock, sid, sidata);

		return 1;
	}

	virtual int StormSendData(storm_socket_data *sdata, SOCKET sock, unsigned char unkid, VString line){
		if(!sdata || !sock)
			return 0;

		if(!line)
			return 1;

		UGLOCK(socket_free_lock);

		if(sdata->sock != sock || sdata->unkid != unkid)
			return 0;

//#ifdef STORMSERVER_POLL_EPOLL
		if(!sdata->sd.Size() && ifsend(sdata->sock)){
			int sd = sdata->item->storm_socket_send(sdata->sock, line, line, 0);
			listen_http_modstate.OnSend(sd);
			//print(HLString() + "S(" + data->sock + "): " + sd + "/" + line.sz + "\r\n");
					
#ifdef STORMSERVER_PRINT_SEND
				Itos it;
				print(it.Format("WorkDo(%d) a_send %d / %d\r\n", el->sock, rd, line.sz));
#endif

			if(sd > 0){
				line = line.str(sd);
			}
		}

		if(line){
			sdata->send(line);
			sdata->wr = 0;

#ifdef STORMSERVER_POLL_EPOLL
			while(!select_ring_e.write(sdata))
				Sleep(1);
#endif
		}

		return 1;
	}

	virtual int StormCloseSocket(storm_socket_data *sdata, SOCKET sock, unsigned char unkid){
		if(!sdata || !sock)
			return 0;

		UGLOCK(socket_free_lock);

		if(sdata->sock == sock && sdata->unkid == unkid){
			sdata->close();
			sdata->wr = 0;
			//print(HLString() + "C(" + sdata->sock + ")\r\n");

#ifdef STORMSERVER_POLL_EPOLL
				while(!select_ring_e.write(sdata))
					Sleep(1);
#endif
			//shutdown(data->sock, 2);
			return 1;
		}

		return 0;
	}

	void PollSocket(storm_socket_data *sdata){
#ifdef STORMSERVER_POLL_SELECT
	while(!select_ring.write(sdata))
			Sleep(1);
#endif

#ifdef STORMSERVER_POLL_EPOLL
	//UGLOCK(socket_epoll_lock);
	SocketPollToSelect(sdata);
	epoll_add_socket(sdata);
#endif

		return ;
	}

	void PollSocketAgain(storm_socket_data *sdata){
		UGLOCK(socket_free_lock);

#ifdef STORMSERVER_POLL_SELECT
		while(!select_ring.write(sdata))
			Sleep(1);
#endif

#ifdef STORMSERVER_POLL_EPOLL
		//UGLOCK2(socket_epoll_lock);
		SocketPollToSelect(sdata);
		epoll_mod_socket(sdata);
#endif

		return ;
	}

	storm_socket_data* PollWait(){
//#if defined STORMSERVER_POLL_SELECT // || defined STORMSERVER_POLL_EPOLL

#ifndef STORMSERVER_POLL_SELECT_NOCOND
		cond_lock.Lock();
		if(!work_ring.is()){
			cond_lock.Wait(0);
			//printf("Yes Captain!\r\n");
		}
		cond_lock.UnLock();
#endif

		socket_work_lock.Lock();
		storm_socket_data *d=work_ring.read();
		socket_work_lock.UnLock();

		if(d){
			//printf("Data!\r\n");
			return d;
		}
		//print("No Data\r\n");
		
		Sleep(1);
		return 0;
//#endif

//#ifdef STORMSERVER_POLL_EPOLL
//		return epoll_select();
//#endif

		return 0;
	}

	void SocketPollToSelect(storm_socket_data *sdata){
		if(!sdata)
			return ;

		sdata->prev = 0;
		sdata->next = socket_select;

		if(socket_select)
			socket_select->prev = sdata;

		socket_select = sdata;
		sockets_count_select ++;

		sdata->stat |= AIFOP_POLL;

#ifdef STORMSERVER_POLL_EPOLL
		//epoll_mod_socket(sdata);
#endif
		return ;
	}

	void SocketPollToWork(storm_socket_data *sdata){ //storm_work_data &td, storm_work_data &first){
		storm_socket_data *d;
		//SocketPollWorkDebug();

		UGLOCK(socket_free_lock);

		if(sdata->prev)
			sdata->prev->next = sdata->next;

		if(sdata->next)
			sdata->next->prev = sdata->prev;

		if(sdata == socket_select)
			socket_select = sdata->next;

		sdata->next = 0;
		sdata->stat &= ~AIFOP_POLL;
		sockets_count_select --;

		__l.UnLock();

		while(!work_ring.write(sdata)){
#ifdef STORMSERVER_POLL_SELECT
			while(d = select_ring.read())
				SocketPollToSelect(d);
#endif
			Sleep(1);
		}

#ifndef STORMSERVER_POLL_SELECT_NOCOND
		cond_lock.Signal();
#endif

		//SocketPollWorkDebug();
		return ;
	}

#ifdef STORMSERVER_POLL_EPOLL
	int SocketEPollTimeout(){
		storm_socket_data *sdata = socket_select, *dl;
		int cnt = 0;

		unsigned int tme = time();

		//UGLOCK(socket_epoll_lock);
		UGLOCK(socket_free_lock);

		while(sdata){
			if(sdata->ltime + sdata->wtime <= tme){
				if(!work_ring.freesz())
					return 0;

				while(!(sdata->stat & AIFOP_POLL)){
					print("FAIL sdata->stat & AIFOP_POLL\r\n");
					Sleep(1000);
				}

				while(!sdata->item){
					print("FAIL !sdata->item\r\n");
					Sleep(1000);
				}

				epoll_mod_socket(sdata, EPOLLONESHOT);
				sdata->stat = AIFOP_TIME | AIFOP_LU;
				
				dl = sdata;
				sdata = sdata->next;

				SocketPollToWork(dl);
			}
			else
				sdata = sdata->next;
		}

		return 1;
	}
#endif


#ifdef STORMSERVER_POLL_EPOLL
	void SocketPollWorkEpoll(){

		// Run once
		socket_work_lock.Lock();
		if(is_socket_poll_work)
			return ;
		is_socket_poll_work = 1;
		socket_work_lock.UnLock();

		int ltime = time();
		storm_socket_data *sdata;

		while(1){
/*
			sdata = select_ring.read();			
			if(sdata){
				SocketPollToSelect(sdata);
				continue;
			}
*/
			while(sdata = select_ring_e.read()){
				UGLOCK(socket_free_lock);
				if(sdata->stat & AIFOP_POLL)
					epoll_mod_socket(sdata, EPOLLOUT | EPOLLONESHOT);
				//SocketPollWorkToSelect2(sdata);
				continue;
			}

			sdata = epoll_select(1000);

			if(sdata)
			while(!(sdata->stat & AIFOP_POLL)){
				print("FAIL epoll_select(); !!!\r\n");
				Sleep(1000);
			}

			if(sdata)
				SocketPollToWork(sdata);

			int tm = time();

			if(ltime + 10 < tm){
				if(SocketEPollTimeout())
					ltime = tm;
				else
					ltime = tm - 9;
			}

			//Sleep(1000);
		}
	}
#endif

#ifdef STORMSERVER_POLL_SELECT
	// Work
	void SocketPollWork(){
		// Run once
		socket_work_lock.Lock();
		if(is_socket_poll_work)
			return ;
		is_socket_poll_work = 1;
		socket_work_lock.UnLock();

		storm_work_data td[32]; int tdi = 0, cnt;

		// no lock
		while(1){
			storm_socket_data *d = select_ring.read(), *prev/*, *lprev*/;

			if(d){
				SocketPollToSelect(d);
				continue;
			}

			if(!socket_select){
				Sleep(7);
				continue;
			}

			d = socket_select;
			cnt=0;			

			while(d){
//				td[tdi].prev=prev;
				td[tdi].data=d;
				td[tdi].sock=d->sock;
				tdi++;

				d = d->next;

				if(tdi == 32 || !d){
					SocketPollWorkSelect(td, tdi, prev);
					tdi = 0;
				}				
				
				cnt++;
			}
		}
		return ;
	}

#endif

	void SocketPollWorkSelect(storm_work_data *td, int tdi, storm_socket_data *&prev){
#ifdef STORMSERVER_TEST_EMULATE
		return SocketPollWorkSelectEmulate(td, tdi, prev);
#endif

		fd_set rfds, wfds, efds; timeval tm; SOCKET maxn=0; int err, mod=0;
		FD_ZERO(&rfds); FD_ZERO(&wfds); FD_ZERO(&efds);
		unsigned int tme = time();

		if(!tdi)
			return ;

		for(int i=0; i<tdi; i++){
			// read
			if(!td[i].data->rd)
				FD_SET(td[i].sock, &rfds);

			// write
			if(!td[i].data->wr)
				FD_SET(td[i].sock, &wfds);
			else
				if(td[i].data->sd.Size())
					mod=1;

			FD_SET(td[i].sock, &efds);

			if(maxn<=td[i].sock)
				maxn=td[i].sock+1;

			// close
			if(td[i].data->cl)
				mod = 1;

			// timeout
			if(!mod && td[i].data->ltime + td[i].data->wtime <= tme)
				mod = 1;
		}

		// Select
		tm.tv_sec = 0; tm.tv_usec = 30000;
		err=select(maxn, &rfds, &wfds, &efds, &tm);

		if(!err && !mod)
			return ;

		//for(int i=0; i<tdi; i++){
		for(int i = tdi - 1; i >= 0; i --){
			mod = 0;

			if(prev == td[i].data){
				prev = i ? td[i - 1].data : 0;
			}

			// recv send
			if(!td[i].data->rd && FD_ISSET(td[i].sock, &rfds)){
				td[i].data->rd = 1; mod = 1;
			}

			if(!td[i].data->wr && FD_ISSET(td[i].sock, &wfds)){
				td[i].data->wr = 1; mod = 1;
			}

			//if(td[i].data->sd.Size())
			//	mod=1;

			// close
			if(td[i].data->cl)
				 mod = 1;

			// stat
			err = FD_ISSET(td[i].sock, &efds) * AIFOP_ERR
				+ ((td[i].data->ltime + td[i].data->wtime <= tme) ? AIFOP_TIME : 0);

			if((td[i].data->stat&AIFOP_LU))
					int gtrh=7;

			if(err || mod){
				td[i].data->stat=err+AIFOP_LU;

				if((td[i].data->stat&AIFOP_LU)==0)
					int gtrh=7;

				SocketPollToWork(td[i].data);
			}
		}

		//if(!prev && td[0].prev)
		//	prev = td[0].prev;

		return ;
	}

	void SocketPollWorkDebug(){
#ifndef STORMSERVER_DEBUG
		return ;
#endif

		// debug
		storm_socket_data *d = socket_select;
		int cnt = 0;

		while(d){
			if(d->stat & AIFOP_LU)
				int erty=7;

			cnt ++;
			d = d->next;
		}

		if(cnt != sockets_count_select)
			int erty=7;

		return ;
	}

	void WorkDoThread(int &thread){
		thread = 2;

		while(thread == 2){
			storm_socket_data *core = PollWait();

			if(core){
				if(WorkDo(core))
					PollSocketAgain(core);
			}			
		}
	}

	int Run(){
		COTS();
		return 1;
	}

	void WorkStartThread(){
		UGLock(threads_lock);
		
		for(int i=0; i<threads_max; i++){
			if(!threads[i]){
				threads_cur++;
				threads[i]=1;
				MCOTS(&threads[i]);
			}
		}

		return ;
	}

	virtual DWORD COT(LPVOID lp){
		if(!lp){
			WorkStartThread();

#ifdef STORMSERVER_POLL_SELECT
			SocketPollWork();
#endif
#ifdef STORMSERVER_POLL_EPOLL
			SocketPollWorkEpoll();
#endif
		} else
			WorkDoThread(*(int*)lp);
		return 0;
	}

	virtual DWORD MCOT(LPVOID lp){
		return COT(lp);
	}

	int WorkDo(storm_socket_data *el){
#ifdef STORMSERVER_TEST_EMULATE
		return WorkDoEmulate(el);
#endif

//		if(!el->item)
//			*el->item = *el->item; // ^_^ please die

#ifdef STORMSERVER_PRINT_DO
			Itos it;
			print(it.Format("WorkDo(%d).\r\n", el->sock));
#endif

		unsigned char buff[STORMSERVER_STACK_BUFF];
		int s_close = 0;
		int s_status;

		storm_work_el wel;
		wel.data = el;
		wel.core = this;
		wel.item = el->item;

		if(!(s_status = WorkDoSend(el, wel, buff)))
			el->cl |= AIFOP_CLOSE;

		// enable read
		if(wel.s_eread > 0)
			el->rd |= 3;

		wel.s_eread = 0;

		if(!s_status)
			s_close = 1;
		else if(s_status < 0)
			s_close = -1;

		if(!(s_status = WorkDoRecv(el, wel, buff)))
			el->cl |= AIFOP_CLOSE;

		// disable read
		if(wel.s_eread < 0)
			el->rd |= 2;

		if(!s_status)
			s_close = 1;
		
		if(el->stat & AIFOP_TIME)
			el->item->storm_timeout(wel);

		if(!s_close && s_status == -1)
			s_close = -1;

		if(el->stat & AIFOP_ERR)
			wel.s_close = 1;

		wel.stat=el->stat | (el->rd==1 ? AIFOP_RECV : 0) | (el->wr==1 ? AIFOP_SEND : 0);
		
		el->ltime = time();

		if(wel.s_close)
			el->cl |= AIFOP_CLOSE;

		if(s_close == -1 && el->cl & AIFOP_CLOSE)
			s_close = 1;

		// close
		//if(el->cl&AIFOP_CLOSE && isnosend){
		if(s_close == 1){
			//StormDelSock(el->item, el->sock);
			DelSocketData(el);
			return 0;
		}
		
		if(wel.s_timeout != -1)
			el->wtime = wel.s_timeout;
		
		el->stat &= ~AIFOP_CLEARMOD; //kio->stat&=~AIFOP_LU;

		return 1;
	}

	int WorkDoRecv(storm_socket_data *el, storm_work_el &wel, unsigned char *buff){
		if(!(el->rd & 1))
			return -1;

		RecvData lot;

		if(lot.Read(el->rv, VString(buff, STORMSERVER_STACK_BUFF), VString())){

			if(!(el->rd & 2)){
				VString fbuf = lot.GetFree();
				int rsz = el->item->storm_socket_recv(el->sock, fbuf, fbuf, 0);
				if(rsz == OPENSSL_RECV_WAIT){
					el->rd = 0;
					return -1;
				}

				listen_http_modstate.OnRecv(rsz);

#ifdef STORMSERVER_PRINT_RECV
				Itos it;
				print(it.Format("WorkDo(%d) a_recv %d\r\n", el->sock, rsz));
#endif
				if(rsz>0)
					lot.Add(rsz);
				else	
					return 0;
			}

			VString read = lot.GetBuffer();
			el->item->storm_recv(wel, read);
			lot.Del(wel.s_read);
			el->rd = 0;
		} else
			return 0;

		return 1;
	}

	int WorkDoSend(storm_socket_data *el, storm_work_el &wel, unsigned char *buff){
		if(el->wr != 1)
			return 1;

		if(el->sd.Size()){
			unsigned int rd = el->sd.Get(buff, STORMSERVER_STACK_BUFF);
			if(rd){
				int sd = el->item->storm_socket_send(el->sock, (char*)buff, rd, 0);
				if(sd == OPENSSL_RECV_WAIT){
					el->wr = 0;
					return 1;
				}

				listen_http_modstate.OnSend(sd);

#ifdef STORMSERVER_PRINT_SEND
				Itos it;
				print(it.Format("WorkDo(%d) a_send %d / %d\r\n", el->sock, sd, rd));
#endif
				if(sd > 0){
					el->sd.Del(sd);
					//if(el->sd.Size())
					//	return 1;
				} else
					return 0;

				el->wr = 0;
				return 1;
			}
			else 
				return 0;
		}

		if(int rd = el->item->storm_send(wel, VString(buff, STORMSERVER_STACK_BUFF / 2))){
			if(rd < 0)
				return -1;

			int sd = el->item->storm_socket_send(el->sock, (char*)buff, rd, 0);
			listen_http_modstate.OnSend(sd);

#ifdef STORMSERVER_PRINT_SEND
				Itos it;
				print(it.Format("WorkDo(%d) a_send2 %d / %d\r\n", el->sock, rd, sd));
#endif

			if(sd <= 0)
				return 0;
			
			if(sd != rd)
				el->sd.Set(buff + sd, rd - sd);

			el->wr = 0;
			return 1;
		}

		return -1;
	}

	virtual int StormRunCore(){
		return Run();
	}

#ifdef STORMSERVER_TEST_EMULATE
	int AddSocketEmulate(storm_core *el, SOCKET sock){
		if(!el)
			return 0;
		return StormAddSock(el, sock);
	}

	int WorkDoEmulate(storm_socket_data *el){
		if(!(el->stat & AIFOP_LU))
				int err = 1;

		if(el->rd == 1 && rand() % 2){ el->rd = 0; }
		if(el->wr == 1 && rand() % 2){ el->wr = 0; }

		if(el->cl&AIFOP_CLOSE){
			DelSocketData(el);
			return 0;
		}

		if(!el->rd && el->wr && rand() % 2){
			el->cl |= AIFOP_CLOSE;
		}

		el->stat &= ~AIFOP_CLEARMOD;
		return 1;
	}

	void SocketPollWorkSelectEmulate(storm_work_data *td, int tdi, storm_socket_data *&prev){
		fd_set rfds, wfds, efds; timeval tm; SOCKET maxn=0; int err = 0, mod=0;
		FD_ZERO(&rfds); FD_ZERO(&wfds); FD_ZERO(&efds);
		unsigned int tme=time();

		if(!tdi)
			return ;

		for(int i=tdi-1; i>=0; i--){
			mod = 0;

			if(td[i].data->stat & AIFOP_LU)
				int err = 1;

			// recv send
			if(!td[i].data->rd && rand() % 2){
				td[i].data->rd=1; mod=1;
			}

			if(!td[i].data->wr && rand() % 2){
				td[i].data->wr=1; mod=1;
			}

			// close
			if(td[i].data->cl)
				 mod=1;

			// stat
			err=rand() % 2 * AIFOP_ERR
				+ ((td[i].data->ltime+td[i].data->wtime<=tme) ? AIFOP_TIME : 0);

			if(err || mod){
				//if(prev == td[i].data){
				//	prev = i ? td[i-1].data : 0;
				//}

				td[i].data->stat=err+AIFOP_LU;

				//SocketPollWorkAddDo(td[i], td[0]);
			}
		}

		//if(!prev && td[0].prev)
		//	prev = td[0].prev;

		return ;
	}



#endif

};

void storm_work_el::send(VString line){
	if(!line)
		return ;

	int sd = item->storm_socket_send(data->sock, line, line, 0);
	listen_http_modstate.OnSend(sd);

	if(sd > 0){
		line.data += sd;
		line.sz -= sd;
	}

	if(line)
		data->send(line);

	data->wr = 0;

	return ;
}

void storm_work_el::send(const char *line, unsigned int sz){
	return send(VString(line, sz));
}

void storm_work_el::gettip(unsigned int &ip, unsigned short &port){
	sockaddr_in from; int fromlen = sizeof(from);

	getsockname(data->sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

	ip = ntohl(from.sin_addr.s_addr);
	port = htons(from.sin_port);
}

void storm_work_el::getcip(unsigned int &ip, unsigned short &port){
	sockaddr_in from; int fromlen = sizeof(from);

	getpeername(data->sock, (struct sockaddr*)&from, (socklen_t*)&fromlen);

	ip = ntohl(from.sin_addr.s_addr);
	port = htons(from.sin_port);
}

SOCKET storm_work_el::getsock(){
	return data ? data->sock : 0;
}