// Nest Api Two.
// RSA keys, Pub keys server, hmac-md5(password), random line, cjx.

// Request:
// CJX: data, crc, time, rand, srand, pass

// 0.0.0.1 27.10.2015 19:03

#ifndef USEMSV_OPENSSL
	#error "Use #define USEMSV_OPENSSL"
#endif

// Nest Api Defines
#ifndef USEMSV_NESTAPI_CONFPATH
	#define USEMSV_NESTAPI_CONFPATH "nestapi.conf"
#endif

#ifndef USEMSV_NESTAPI_PUB
	#define USEMSV_NESTAPI_PUB ""
#endif

#ifndef USEMSV_NESTAPI_KEY
	#define USEMSV_NESTAPI_KEY ""
#endif

VString NestApiIVek = "-----BEGINRSAPRIVATEKEY-----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-----ENDRSAPRIVATEKEY-----";

// Nest Api Key
class NestApiKey{
public:
	NestApiKey *next;
	VString name;
	VString key;
	VString srand;
	unsigned int datasz;
	//unsigned char data[];
};

// Nest Api Keys
class NestApiKeys{
public:
	MString data, odata;
	NestApiKey *key;
	unsigned int ltime;

	NestApiKeys(){
		key = 0;
		ltime = 0;
	}

	int LoadGetSize(VString line){
		// format: name key.
		unsigned int sz = 0;

		while(line){
			VString o = PartLine(line, line, "\n"), t;

			while(o.endo() == '\r' || o.endo() == ' ' || o.endo() == '\t')
				o.sz --;

			o = PartLineST(o, t);
			sz += sizeof(NestApiKey) + o.sz + t.sz + 32;
		}

		return sz;
	}

	void LoadGetData(VString line){
		NestApiKey *key, *lkey = 0;
		unsigned int sz = LoadGetSize(line);		
		unsigned char *ln;
		
		odata -= data;
		data.Reserv(sz);
		ln = data;

		while(line){
			VString o = PartLine(line, line, "\n"), t;

			while(o.endo() == '\r' || o.endo() == ' ' || o.endo() == '\t')
				o.sz --;

			o = PartLineST(o, t);
			key = (NestApiKey*)ln;
			ln += sizeof(NestApiKey);

			memcpy(ln, o, o);
			key->name.setu(ln, o);
			ln += o;

			memcpy(ln, t, t);
			key->name.setu(ln, t);
			ln += t;

			//memcpy(ln, t, t);
			//key->srand.setu(ln, t);
			ln += 32;
			
			if(lkey)
				lkey->next = key;
			lkey = key;
		}

		key = (NestApiKey*)data.data;
		ltime = time();

		return ;
	}

};

// Nest Api Base Class
class NestApi : public TLock{
	// config
	MString keys_path;
	NestApiKeys keys_data;

	// keys
	VString keys_pub;
	VString keys_key;

	// server
	unsigned int serv_ltime; // last time
	unsigned int serv_mtime; // mod time
	unsigned int serv_rtime; // rand time uid
	unsigned int serv_utime; // update time (sec)
	// server rand string = (serv_ltime - serv_mtime) + md5-hmac($1 + rtime).str(0, 12);

public:

	NestApi(VString path, VString pub, VString key){
		keys_path = path;
		keys_data.LoadGetData(LoadFile(path));

		keys_pub = pub;
		keys_key = key;

		// New keys
		if(!keys_pub || !keys_key){
			TString pub, key;

			print("Error! Nest Api keys not found!\r\n");
			print("Generate new keys...");

			RsaCreateKeys(pub, key);
			print("OK\r\n");

			HLString ls;

			ls + "#define USEMSV_NESTAPI_PUB \"" + Replace(pub, "\n", "\\n") + "\"";
			ls + "\r\n";
			ls + "#define USEMSV_NESTAPI_KEY \"" + Replace(key, "\n", "\\n") + "\"";

			SaveFile("_msv_nestapi_keys.pem", ls.String());
			print("Keys saved to _msv_nestapi_keys.pem\r\n");

			Sleep(100 * S1K);
			exit(0);
			return ;
		}

		// Test Keys
		unsigned int tm = msrand();
		TString tmp;

		VString vtmp = VString((unsigned char*)&tm, sizeof(tm));

		tmp = RsaPublicEncode(keys_pub, vtmp);
		tmp = RsaPrivateDecode(keys_key, tmp);

		if(tmp != vtmp){
			print("Error! Nest Api keys is fail! Generate new keys!\r\n");

			Sleep(100 * S1K);
			exit(0);
			return ;
		}

		serv_ltime = 0;
		serv_utime = 3600;

		GetSRand();

		return ;
	}

	TString GetSRand(){
		unsigned int tm = time();
		if(serv_ltime + serv_utime < tm){
			serv_ltime = tm;
			serv_mtime = msrand();
			serv_rtime = msrand();
		}

		tm -= serv_mtime;

		TString ret;
		ret.Add(VString((unsigned char*)&tm, sizeof(tm)), VString((unsigned char*)&serv_rtime, sizeof(serv_rtime)));

		ret = md5(ret);
		
		memcpy(ret.data + 0, &tm, sizeof(tm));
		memcpy(ret.data + sizeof(tm), &serv_rtime, sizeof(serv_rtime));

		return ret;
	}

	int TestSRand(VString line){
		if(line.sz != 32)
			return 0;

		unsigned int &tm = *(unsigned int*)line.data;
		unsigned int &r = *(unsigned int*)(line.data + sizeof(tm));

		if(serv_ltime <= tm && serv_ltime + serv_utime > tm)
			return 1;

		return 0;
	}

} NestApiCli(USEMSV_NESTAPI_CONFPATH, USEMSV_NESTAPI_PUB, USEMSV_NESTAPI_KEY);


class NestApiSslRequest{
public:
	// head free
	unsigned int rand;
	unsigned int ver:2;
	unsigned int unk:30;

	// head rsa
	unsigned int crc; // crc(text)
	unsigned int size; // Rsa(text).size
	unsigned char akey[256]; // 92b key for aes

	// data aes
	// unsigned char data[];

	void Init(){
		rand = msrand();
		ver = 1;
		unk = msrand();
		
		TString key = RandGetUniqLine();
		memcpy(akey, key, 256);

		return ;
	}

	void InitEncode(VString line){
		Init();
		crc = crc32(line, line);
	}

	int GetSizeDecode(){
		return sizeof(*this);
	}

	int GetSizeEncode(){
		return 0;
	}

	TString Encode(VString line, VString key){
		TString res;

		InitEncode(line);
		res = AesEncode(line, VString(akey, 92), NestApiIVek);
		size = res;

		TString hres = RsaPublicEncode(key, VString((unsigned char *)&crc, 100));
		memcpy(akey, hres, hres);

		res.Add(VString((unsigned char*)this, sizeof(*this)), res);

		// res ^ rand
		//unsigned char *ln = res.data + 8, *to = res.endu();
		//while(ln + 3 < to){
		//	(*(unsigned int*)ln) ^= rand;
		//	ln += 4;
		//}

		//for(int i = 0; ln < to; i++)
		//	*ln++ ^= rand & ((1 << i) - 1);

		return res;
	}

	TString Decode(VString line, VString key){
		if(line.size() < GetSizeDecode())
			return 0;

		NestApiSslRequest *req = (NestApiSslRequest*)line.data;

		rand = req->rand;
		ver = req->ver;
		unk = req->unk;

		// res ^ rand
		//unsigned char *ln = line.data + 8, *to = line.endu();
		//while(ln + 3 < to){
		//	(*(unsigned int*)ln) ^= rand;
		//	ln += 4;
		//}

		//for(int i = 0; ln < to; i++)
		//	*ln++ ^= rand & ((1 << i) - 1);

		TString hres = RsaPrivateDecode(key, VString(req->akey, 256));
		TString res = AesDecode(line.str(GetSizeDecode()), hres.str(8), NestApiIVek);

		if(hres > 8){
			crc = *(unsigned int*)hres.data;
			size = *((unsigned int*)hres.data + 1);
		}

		return res;
	}


	int Test(){
		VString line = NestApiIVek;

		TString edata = Encode(line, USEMSV_NESTAPI_PUB);
		TString data = Decode(edata, USEMSV_NESTAPI_KEY);

		if(edata && data == NestApiIVek){
			print("NestApiSslRequest::Test() is OK.\r\n");
			return 1;
		}
		else
			print("NestApiSslRequest::Test() is FAIL.\r\n");

		return 0;
	}

};

TString Tmp_GetRequest(VString data, VString key){
	unsigned int crc = crc32(data, data);

	TString rline = RandGetUniqLine();

	NestApiSslRequest req;
	req.Init();





	return 0;
}

// Client
void NestApiRequest(VString req){
	// CJX data;

	GetHttp gp;


};

//void NestApiRequest(VString data){
//	// CJX data;
//
//
//}

#ifdef USEMSV_MSL_FL
template<class B>
void NestApiRequest(VString to, msl_value_template<B> &val){
	TString data = CjxGetLine(val);


	return ;
}
#endif



// Server
void NestApisRequest(VString data){
	// CJX data;





}