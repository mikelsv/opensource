// nest api server
class _nest_api_sess{
public:
	int ltime, rtime;
	unsigned int id;
	unsigned char hash[32];
};
MString nest_api_key;

MList<_nest_api_sess> nest_api_sess;

int nest_api_sess_find(_nest_api_sess &sess, int &id){
	return sess.id==id;
}

int nest_api_sess_find_lt(_nest_api_sess &sess, int &id){
	return sess.ltime<id;
}

int nest_api_sess_test(int sid, char *hash){
	UGLOCK(nest_api_sess);
	_nest_api_sess *s;

	if(s=nest_api_sess.Find((void*)nest_api_sess_find, sid)){
		memcpy(hash, s->hash, 32);
		return 1;
	}

	return 0;
}

int nest_api_sess_test(int sid, VString shash, VString rand, VString crc, VString rtime){
	MString tmp;
	_nest_api_sess *s;

	if(!sid || !shash || rand.sz != 32 || !rtime)
		return 0;

	UGLOCK(nest_api_sess);

	if(s = nest_api_sess.Find((void*)nest_api_sess_find, sid)){
		tmp.Add(rtime, VString(s->hash, 32), rand, nest_api_key, rand, VString(s->hash, 32), crc, rtime);

		if(shash == md5h(tmp) && s->rtime <= rtime.toi()){
			s->ltime = time() + 120;
			s->rtime = rtime.toi();
			return 1;
		} else{
			s->ltime = time() - 100;
		}
	}

	return 0;
}

// 
//int nest_api_request(int sid, VString shash, VString rand, VString crc, VString rtime){
//	nest_api_sess_del();
//
//	if(!sid || !shash || rand.sz != 32 || !rtime || !nest_api_sess_test(sid, shash, rand, crc, rtime))
//		return 0;
//
//	return 1;
//}

int nest_api_sess_new(char *hash){
	if(nest_api_sess.Size()>S4K)
		return 0;

	UGLOCK(nest_api_sess);
	_nest_api_sess *s=nest_api_sess.New();

	int sid; unsigned char h[16];
	md5_state_t state;

	while(1){
		sid=rand();
		
		md5_init(&state);
		md5_append(&state, (const md5_byte_t *)&sid, sizeof(sid));
		md5_append(&state, (const md5_byte_t *)&s, sizeof(s));
		md5_append(&state, (const md5_byte_t *)&state, sizeof(state));
		md5_finish(&state, (md5_byte_t*)h);

		if(sid && !nest_api_sess.Find((void*)nest_api_sess_find, sid)){
			s->ltime=time();
			s->rtime = 0;
			s->id=sid;
			prmf_btoh(s->hash, 32, h, 16);
			memcpy(hash, s->hash, 32);
			
			return sid;
		}
	}

	return 0;
}

int nest_api_sess_del(){
	UGLOCK(nest_api_sess);

	int tm=time()-10;
	_nest_api_sess *s;

	while(s=nest_api_sess.Find((void*)nest_api_sess_find_lt, tm)){
		nest_api_sess.Del(s);
	}

	return 0;
}



// nest api client
class nest_api_cli{
public:
	VString url;
	VString pass;

	nest_api_cli(VString u, VString p){
		url = u;
		pass = p;
	}

};

void nest_api_request(nest_api_cli &cli, VString req, TString &ret){
	GetHttp gp;

	ret.Add("data=", req, "&crc=", md5h(req));

	gp.SetPost(ret);
	gp.Request(cli.url);

	VString data = gp.GetData(), crc;
	crc = PartLine(data, data, "$");

	if(crc.sz > 32)
		crc = crc.str(-32);

	if(crc == md5h(data)){
		ret = data;
		return ;
	}

	ret = "{error:CRCFAIL,msg:'Crc fail'}";

	return ;
}