#define USEMSV_OPENSSL
#pragma comment(lib,"../../openssl-1.0.2/out32/ssleay32.lib") 
#pragma comment(lib,"../../openssl-1.0.2/out32/libeay32.lib") 

#include <openssl/ssl.h>
#include <openssl/err.h>

// ZLIB: #include "../../opensource/msvcore/crossplatform-zlib.cpp"

// include path ..\..\openssl-1.0.2\include

// http://developer.covenanteyes.com/building-openssl-for-visual-studio/
// http://phpseclib.sourceforge.net/
// http://stackoverflow.com/questions/4484246/encrypt-and-decrypt-text-with-rsa-in-php


class InitMySSL{
public:
	InitMySSL(){
		SSL_library_init();
		SSL_load_error_strings();		/* load all error messages */
		OpenSSL_add_all_algorithms();	/* load & register cryptos */
		return ;
	}

	~InitMySSL(){
		ERR_free_strings();
		EVP_cleanup();
		ERR_remove_state(0);
		sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
		//sk_free(SSL_COMP_get_compression_methods());
		//SSL_COMP_free_compression_methods();
	}


}InitMySSL;

class MySSL{
	SSL *ssl;
	SSL_CTX *ctx;
	int work;

public:
	MySSL(){
		ssl=0;
		ctx=0;
		work=0;
	}

	~MySSL(){
		Release();
	}

	void Release(){
		if(work){
			work = 0;
			SSL_shutdown(ssl);
		}

		if(ssl)
			SSL_free(ssl);
		if(ctx)
			SSL_CTX_free(ctx);

		ssl=0; ctx=0;
	}

	int IsWork(){
		return work;
	}

	int Connect(SOCKET sock){
		Release();

		ctx=SSL_CTX_new(SSLv23_method());
		if(!ctx)
			return 0;

		ssl=SSL_new(ctx);
		if(!ssl)
			return 0;

		if(!SSL_set_fd(ssl, sock))
			return 0;

		SSL_CTX_set_timeout(ctx, 60);
		
		int err=SSL_connect(ssl);

		work=err>=0;
		return work;
	}

	int send(const char *buf, int sz){
		if(!work)
			return 0;

		return SSL_write(ssl, buf, sz);
	}

	int recv(void *buf, int sz){
		if(!work)
			return 0;

		return SSL_read(ssl, buf, sz);
	}

	int close(){
		if(!work)
			return 0;

		work = 0;
		SSL_shutdown(ssl);
		return 1;
	}

};



void RsaCreateKeys(TString &pub, TString &sec){
	// generate keys
	RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
	int sz;

	BIO *bp=BIO_new(BIO_s_mem());
	BIO *bs=BIO_new(BIO_s_mem());

	PEM_write_bio_RSA_PUBKEY(bp, rsa);
	PEM_write_bio_RSAPrivateKey(bs, rsa, 0, 0, 0, 0, 0);

	// pub
	sz=BIO_ctrl_pending(bp);
	pub.Reserv(sz);
	BIO_read(bp, pub, sz);

	// sec
	sz=BIO_ctrl_pending(bs);
	sec.Reserv(sz);
	BIO_read(bs, sec, sz);

	BIO_free(bp);
	BIO_free(bs);

	RSA_free(rsa);

	return ;
}

int RsaSaveCreateKeys(VString pubf, VString secf){
	TString pub, sec;
	int ret=1;

	RsaCreateKeys(pub, sec);
	ret&=SaveFile(pubf, pub)>0;
	ret&=SaveFile(secf, sec)>0;
	return ret;
}

TString RsaOperation(int type, VString key, VString text){
	if(!text)
		return TString();

	TString ret;
	RSA *rsa = RSA_new(), *r;
	BIO *b = BIO_new_mem_buf(key, key);
	int sz = 0;

	if(type<2)
		r = PEM_read_bio_RSA_PUBKEY(b, &rsa, 0, 0);
	else
		r = PEM_read_bio_RSAPrivateKey(b, &rsa, 0, 0);

	if(!r){
		BIO_free(b);
		RSA_free(rsa);
		return TString();
	}
	
	switch(type){
	case 0:
		//r=PEM_read_bio_RSA_PUBKEY(b, &rsa, 0, 0);
		//if(!r)
		//	return MString();
		ret.Reserv(RSA_size(rsa));
		sz=RSA_public_encrypt(text, text, ret, rsa, RSA_PKCS1_PADDING);
	break;
	case 1:
		//PEM_read_bio_RSA_PUBKEY(b, &rsa, 0, 0);
		ret.Reserv(RSA_size(rsa));
		sz=RSA_public_decrypt(text, text, ret, rsa, RSA_PKCS1_PADDING);
	break;
	case 2:
		//PEM_read_bio_RSAPrivateKey(b, &rsa, 0, 0);
		ret.Reserv(RSA_size(rsa));
		sz=RSA_private_encrypt(text, text, ret, rsa, RSA_PKCS1_PADDING);
	break;
	case 3:
		//PEM_read_bio_RSAPrivateKey(b, &rsa, 0, 0);
		ret.Reserv(RSA_size(rsa));
		sz=RSA_private_decrypt(text, text, ret, rsa, RSA_PKCS1_PADDING);
	break;
	}
	
	BIO_free(b);
	RSA_free(rsa);

	if(sz==-1){
		int err=ERR_get_error();
		ERR_error_string(ret, ret);
	}

	ret.Reserv(sz);
	return ret;
}

TString RsaPublicEncode(VString key, VString text){
	return RsaOperation(0, key, text);
}

TString RsaPublicDecode(VString key, VString text){
	return RsaOperation(1, key, text);
}

TString RSAPrivateEncode(VString key, VString text){
	return RsaOperation(2, key, text);
}

TString RsaPrivateDecode(VString key, VString text){
	return RsaOperation(3, key, text);
}


void test_rsa(){
	TString pub, sec, e, d;
	RsaCreateKeys(pub, sec);
//	e=RSAEncode(pub, "The test string for encrypt/decrypt");
//	d=RSADecode(sec, e);
	return ;
}


// AES
TString AesEncode(VString line, VString ckey, VString ivec){
	TString ret;
	ret.Reserv(line.sz*2);
	int retlen=0, retlenf=0;

	if(!ckey || !ivec)
		return TString();

	EVP_CIPHER_CTX ctx;
	EVP_EncryptInit(&ctx, EVP_aes_128_cbc(), ckey, ivec);
	EVP_EncryptUpdate(&ctx, ret, &retlen, line, line);
	EVP_EncryptFinal(&ctx, ret.data+retlen, &retlenf);

	ret.Reserv(retlen+retlenf);
	return ret;
}

TString AesDecode(VString line, VString ckey, VString ivec){
	TString ret;
	ret.Reserv(line.sz+16);
	int retlen=0, retlenf=0;

	if(!ckey || !ivec)
		return TString();

	EVP_CIPHER_CTX ctx;
	EVP_DecryptInit(&ctx, EVP_aes_128_cbc(), ckey, ivec);
	EVP_DecryptUpdate(&ctx, ret, &retlen, line, line);
	EVP_DecryptFinal(&ctx, ret.data+retlen, &retlenf);
	EVP_CIPHER_CTX_cleanup(&ctx);

	ret.Reserv(retlen+retlenf);
	return ret;
}

TString SslGetUniqLine(){
	TString pub, sec;
	RsaCreateKeys(pub, sec);
	return RsaPublicEncode(pub, sec.str(32, 100));
}

TString RandGetUniqLine(){
	TString ret;
	ret.Reserv(256);

	unsigned char *r = ret, *to = ret.endu();

	while(r < to){
		*r++ = mrand(256);
	}

	return ret;
}



// prog secure
VString _ssl_init_p="-----BEGIN PUBLIC KEY-----MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA8At66rwd96Ra2eNvKp9KMBW0Ex6Q8lwHep+/D8lWhuwSeU0+CSm9daQS0Te89Gcdo1x5XfPoe77tD0fvrfxGinJumAAVILRa8XME/Ml1BHRTq/cVqDoMWOkax8mYGBHwzW815DqmDqe9xH7LVGGbWzz4JY+ji2vYt2dZk7HZBY+2h3aJIoRB/RrHYdb6USVt325RMDpGsUONXpx3Ep7vnBuWEcTqY45mnDLKPpl64r75j9NXOVe0KlKUMHzPyns1nOYIJjzNAgnxH3jGle11sK535/yt81h7uh1p7ozFNHj88LN0q19ebrwQU2qBQmb/LJL+Z/IvsPDi4yVGTcj3EwIDAQAB-----END PUBLIC KEY-----";
VString _ssl_init_s="-----BEGIN RSA PRIVATE KEY-----MIIEowIBAAKCAQEA8At66rwd96Ra2eNvKp9KMBW0Ex6Q8lwHep+/D8lWhuwSeU0+CSm9daQS0Te89Gcdo1x5XfPoe77tD0fvrfxGinJumAAVILRa8XME/Ml1BHRTq/cVqDoMWOkax8mYGBHwzW815DqmDqe9xH7LVGGbWzz4JY+ji2vYt2dZk7HZBY+2h3aJIoRB/RrHYdb6USVt325RMDpGsUONXpx3Ep7vnBuWEcTqY45mnDLKPpl64r75j9NXOVe0KlKUMHzPyns1nOYIJjzNAgnxH3jGle11sK535/yt81h7uh1p7ozFNHj88LN0q19ebrwQU2qBQmb/LJL+Z/IvsPDi4yVGTcj3EwIDAQABAoIBAAV+fcW5oIX2gQ3Tosu68ZvAI2r8nByv9ovb9CeMWCqmZJXaKr1S8z8qDjQzrmN5qLgAedaoqn2KlSpwXxAAYizxCY75EYK6HgrAszCF++OqNfANq7U5/uZaD37bIlwKrRRGo1kh7oMPW8yPil/YDBI+hv91Fi0uvrsT5nz26RyzbgRjSWtFhsrFvuYH/Y190048x0GX9d+m3RVhfautchha2eS7k04oEDgnAjmXHlUJF6v/9jIQwzzerrAYNTxwXhwhht/m4+zfR+EA5y737RIPXytUqIyTBZAsaNGe+jsrJiEWQLgjTdYN5tglhZr9y5QdVAKOtS0Vp1qvPtR9SsECgYEA/VE3M60/dMkw3BhhHDZ0mBTDPF8U87sQWk5/Wkc3a541gLTCIAG9SjaexK6VEMI+lFMETwqXd3H1eu50KQzmuEadJSIoOAqdwz6BpCoUXURJhgyZ9tSqS8HNP77uGEQGvTqNBsJUq8mFzF3XqExtGhsWcZu23TaIupu5Q/ERrbsCgYEA8pZH5m3weztU0ASzYOxgJPhkgwEQ71vKoD92BWJsrKT9l9LcjgMIryIgPs3pHwHhNqE5Jz1DxPmSbBHAvoYCpYd/q3nhaVD8xr9hmToqU3n436V7/pbnKqVfsl9w9k0NrOXfH62lU4XaooAXqyJzp01oDHnZEEuAqJAGCI0qGokCgYEAmmXLGKNLHxqVvkDZx8FlNWDKUty9P4VQBUr9EoB/xgdrIuSJwcptnnNGOCkU0F8kl3E2GhshkPsTnXcDtBWZ9s2LcJuuGeJWoC9UA0RmnyZFDgBWQ30BwyT/vg/YvU+RL/slA+6kfwRYmeTE0l6qzROfjWVUguZ/VgYeWMvxc4sCgYAHeD0qiUQYZ1FLPnnPiDfFq6hcqFG/LRJmMM4PiGJURphQbnvAG7POwLxBctG01Lf8mKZL4QUPMagij8FhnHrEYhG6U/xu48GE1SvKLhJTk7PoHw5cXoUruP/jXDD8RI8hPcrLDOvEUKDU1CDmch8hja6gg7f9WnXRuu2UodQkEQKBgDyr19w7uUR38irP52IegyQrnalII1yLe6IP7kMJQ9fVd8I8jHwWXBoqUAjhAd/N1Eg++tuALGGHE5VinL6e9Ukh4nWoC5M2e6Z2uG44/XiwBKZN4tQ+2Xuh83nClilfrIR9TJCI4dm7XNbTwxaxbwVK6xbcQyxO8LsS12nE8ddF-----END RSA PRIVATE KEY-----";

MString _ssl_init_0, _ssl_init_1, _ssl_init_2;


int ProgSecureInit(){
	MkDir("secure");

	// create
	if(!IsFile("secure/_init_0.key")){
		SaveFile("secure/do_not_delete_this_files.txt", "Else all data will be lose!");
		SaveFile("secure/_init_0.key", RandGetUniqLine());
	}

	_ssl_init_0=LoadFile("secure/_init_0.key");

	// create RSA keys
	if(!IsFile("secure/_init_1.key") || !IsFile("secure/_init_2.key")){
		RsaSaveCreateKeys("secure/_init_1.key", "secure/_init_2.key");
		SaveFile("secure/_init_1.key", AesEncode(LoadFile("secure/_init_1.key"), _ssl_init_0, _ssl_init_p));
		SaveFile("secure/_init_2.key", AesEncode(LoadFile("secure/_init_2.key"), _ssl_init_0, _ssl_init_s));
	}

	_ssl_init_1=LoadFile("secure/_init_1.key");
	_ssl_init_2=LoadFile("secure/_init_2.key");
	
	return _ssl_init_0 && _ssl_init_1 && _ssl_init_2;
}


TString AesEncodeMyDataBlock(VString data, VString ckey, VString ivek){
	TString ret;
	
	TString key = RandGetUniqLine();
	TString key2 = RandGetUniqLine();

	key += VString(key2).str(0, 100);
	return ret.Add(VString(key).str(0, 256), RsaPublicEncode(ckey, VString(key2).str(0, 100)), AesEncode(data, key, ivek));
}

TString AESDecodeMyDataBlock(VString data, VString ckey, VString ivek){
	TString key = data.str(0, 256);
	VString key2 = data.str(256, 256);
	VString line = data.str(512);

	key.Add(key, RsaPrivateDecode(ckey, key2));
	return AesDecode(line, key, ivek);
}


TString TEncodeData(VString data){
	TString ret;
	
	TString key = RandGetUniqLine();
	TString key2 = RandGetUniqLine();

	key += VString(key2).str(0, 100);
	return ret.Add(VString(key).str(0, 256), RsaPublicEncode(AesDecode(_ssl_init_1, _ssl_init_0, _ssl_init_p), VString(key2).str(0, 100)), AesEncode(data, key, _ssl_init_1));
}

TString TDecodeData(VString data){
	TString key = data.str(0, 256);
	VString key2 = data.str(256, 256);
	VString line = data.str(512);

	key.Add(key, RsaPrivateDecode(AesDecode(_ssl_init_2, _ssl_init_0, _ssl_init_s), key2));
	return AesDecode(line, key, _ssl_init_1);
}

/*
int DecodeDeflateDo(z_stream &zs, VString data, VString ret){
	memset(&zs, 0, sizeof(zs));
	inflateInit(&zs);
	zs.avail_in=data; zs.next_in=data; zs.avail_out=ret; zs.next_out=ret;
	int r=inflate(&zs, Z_NO_FLUSH);
	r = inflate(&zs, Z_FINISH);
	return r;
}

MString DecodeDeflate(VString data){
	if(!data) return MString();

	unsigned char dbuf[S512K], *rbuf=dbuf; MString b; z_stream zs;

	int r=DecodeDeflateDo(zs, data, VString(dbuf, sizeof(dbuf)));

	if(r==-5){
		b.Reserv(S16M); rbuf=b;
		r=DecodeDeflateDo(zs, data, b);
	}

	if(r==-5){
		print("NO MEMORY!!!!!!!!!!!!!!!!\r\n");
		return MString();
	}

	if(r<0){
		print("INFLATE FAIL\r\n");
//		AddEventDo(ZFNENUM_BADDATA);
		return MString();
	}

	return MString(rbuf, zs.total_out);
}*/

int OpenSSLCreateRSAPrivateKey(TString &r_key, int bits = 2048){
	RSA * rsa;
	rsa = RSA_generate_key(
		bits,   /* number of bits for the key - 2048 is a sensible value */
		RSA_F4, /* exponent - RSA_F4 is defined as 0x10001L */
		NULL,   /* callback - can be NULL if we aren't displaying progress */
		NULL    /* callback argument - not needed in this case */
	);

	//EVP_PKEY_assign_RSA(pkey, rsa);

	// save key
	int sz;
	BIO *bio=BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(bio, rsa, 0, 0, 0, 0, 0);

	sz=BIO_ctrl_pending(bio);
	r_key.Reserv(sz);
	BIO_read(bio, r_key, sz);
	
	BIO_free(bio);
	RSA_free(rsa);

	return sz;
}

int OpenSSLCreateCertificateRequest(VString r_key, TString &r_req, VString ca, VString co, VString cn){
	// http://stackoverflow.com/questions/256405/programmatically-create-x509-certificate-using-openssl
	
	int ret=0;

	EVP_PKEY * pkey;
	pkey = EVP_PKEY_new();

	// read private key
	BIO *b=BIO_new_mem_buf(r_key, r_key), *bio;

	int r=0, sz;
	if(!PEM_read_bio_PrivateKey(b, &pkey, 0, 0)){
		BIO_free(b);
		EVP_PKEY_free(pkey);
		return 0;
	}

	BIO_free(b);

	// create certificate
	X509_REQ * x509;
	x509 = X509_REQ_new();

	X509_REQ_set_version(x509, 2);
	//ASN1_INTEGER_set(X509_REQ_get_serialNumber(x509), 1);
	//X509_gmtime_adj(X509_REQ_get_notBefore(x509), 0);
	//X509_gmtime_adj(X509_REQ_get_notAfter(x509), 31536000L);

	X509_REQ_set_pubkey(x509, pkey);

	X509_NAME * name;
	name = X509_REQ_get_subject_name(x509);

	X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char *)ca, -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)co, -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)cn, -1, -1, 0);

	// X509_set_issuer_name(x509, name);

	if(!X509_REQ_sign(x509, pkey, EVP_sha1()))
		goto free_all;

	// save cert
	bio=BIO_new(BIO_s_mem());
	PEM_write_bio_X509_REQ(bio, x509);
	sz=BIO_ctrl_pending(bio);
	r_req.Reserv(sz);
	BIO_read(bio, r_req, sz);
	BIO_free(bio);

	ret=1;

free_all:
	X509_REQ_free(x509);
	EVP_PKEY_free(pkey);
	//OPENSSL_free(name);
	//RSA_free(rsa);

	return ret;
}

int OpenSSLCreateCACertificate(VString r_key, TString &r_req, VString ca, VString co, VString cn){
	// http://stackoverflow.com/questions/256405/programmatically-create-x509-certificate-using-openssl
	// http://www.codepool.biz/security/how-to-use-openssl-to-sign-certificate.html
	
	int ret=0;

	EVP_PKEY * pkey;
	pkey = EVP_PKEY_new();

	// read private key
	BIO *b=BIO_new_mem_buf(r_key, r_key), *bio;

	int r=0, sz;
	if(!PEM_read_bio_PrivateKey(b, &pkey, 0, 0)){
		BIO_free(b);
		EVP_PKEY_free(pkey);
		return 0;
	}

	BIO_free(b);

	// create certificate
	X509 * x509;
	x509 = X509_new();

	X509_set_version(x509,2);
	ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
	X509_gmtime_adj(X509_get_notBefore(x509), 0);
	X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

	X509_set_pubkey(x509, pkey);

	X509_NAME * name;
	name = X509_get_subject_name(x509);

	X509_NAME_add_entry_by_txt(name, "C",  MBSTRING_ASC, (unsigned char *)ca, -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "O",  MBSTRING_ASC, (unsigned char *)co, -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)cn, -1, -1, 0);

	X509_set_issuer_name(x509, name);

	if(!X509_sign(x509, pkey, EVP_sha1()))
		goto free_all;

	// save cert
	bio=BIO_new(BIO_s_mem());
	PEM_write_bio_X509(bio, x509);
	sz=BIO_ctrl_pending(bio);
	r_req.Reserv(sz);
	BIO_read(bio, r_req, sz);
	BIO_free(bio);

	ret=1;

free_all:
	X509_free(x509);
	EVP_PKEY_free(pkey);
	//OPENSSL_free(name);
	//RSA_free(rsa);

	return ret;
}

bool OpenSSLSignCertificateLoadCA(X509 ** px509, VString ca_cert){
	// read ca
	BIO *b=BIO_new_mem_buf(ca_cert, ca_cert);
	bool ret = (PEM_read_bio_X509(b, px509, 0, 0)!=0);
	BIO_free(b);
    return ret;
}

// load ca private key
bool OpenSSLSignCertificateLoadCAPrivateKey(EVP_PKEY **ppkey, VString ca_key){
    bool ret;
    BIO *in = NULL;
    RSA *r = NULL;
    EVP_PKEY *pkey = NULL;

	in = BIO_new_mem_buf(ca_key, ca_key);
    ret = (PEM_read_bio_RSAPrivateKey(in, &r, NULL, NULL) != NULL);
    if(!ret)
        goto free_all;
 
    pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, r);
    *ppkey = pkey;
    r = NULL;
 
free_all:
    BIO_free(in);
    return ret;
}
 
// load X509 Req
bool OpenSSLSignCertificateLoadX509Req(X509_REQ **ppReq, VString req){
    bool ret;
    BIO *in = NULL;
 
    in = BIO_new_mem_buf(req, req);
    ret = (PEM_read_bio_X509_REQ(in, ppReq, NULL, NULL) != NULL);
 
    BIO_free(in);
    return ret;
}

int OpenSSLSignCertificateDoX509Sign(X509 *cert, EVP_PKEY *pkey, const EVP_MD *md)
{
    int rv;
    EVP_MD_CTX mctx;
    EVP_PKEY_CTX *pkctx = NULL;
 
    EVP_MD_CTX_init(&mctx);
    rv = EVP_DigestSignInit(&mctx, &pkctx, md, NULL, pkey);
 
    if (rv > 0)
        rv = X509_sign_ctx(cert, &mctx);
    EVP_MD_CTX_cleanup(&mctx);
    return rv > 0 ? 1 : 0;
}

int OpenSSLSignCertificate(TString &r_cert, VString ca_cert, VString ca_key, VString r_req, int serial){
	int ret = 0, sz;
	X509 * ca = NULL;
    X509_REQ * req = NULL;
    EVP_PKEY *pkey = NULL, *pktmp = NULL;
 
    X509_NAME *subject = NULL, *tmpname = NULL;
    X509 * cert = NULL;
    BIO *out = NULL;

	//int serial = 1;
    long days = 3650 * 24 * 3600; // 10 years

	if(!OpenSSLSignCertificateLoadCA(&ca, ca_cert))
        goto free_all;
 
    if(!OpenSSLSignCertificateLoadCAPrivateKey(&pkey, ca_key))
        goto free_all;
 
    if(!OpenSSLSignCertificateLoadX509Req(&req, r_req))
        goto free_all;

	    cert = X509_new();
    // set version to X509 v3 certificate
    if (!X509_set_version(cert,2))
        goto free_all;
 
    // set serial
    ASN1_INTEGER_set(X509_get_serialNumber(cert), serial);
 
    // set issuer name frome ca
    if (!X509_set_issuer_name(cert, X509_get_subject_name(ca)))
        goto free_all;
 
    // set time
    X509_gmtime_adj(X509_get_notBefore(cert), 0);
    X509_gmtime_adj(X509_get_notAfter(cert), days);
 
    // set subject from req
    tmpname = X509_REQ_get_subject_name(req);
    subject = X509_NAME_dup(tmpname);
    if (!X509_set_subject_name(cert, subject))
        goto free_all;
 
    // set pubkey from req
    pktmp = X509_REQ_get_pubkey(req);
    ret = X509_set_pubkey(cert, pktmp);
    EVP_PKEY_free(pktmp);
    if (!ret) goto free_all;
 
    // sign cert
    if (!OpenSSLSignCertificateDoX509Sign(cert, pkey, EVP_sha1()))
        goto free_all;
 
    //out = BIO_new_file(szUserCert,"w");
    //ret = PEM_write_bio_X509(out, cert);

	// save cert
	out=BIO_new(BIO_s_mem());
	PEM_write_bio_X509(out, cert);
	sz=BIO_ctrl_pending(out);
	r_cert.Reserv(sz);
	ret=BIO_read(out, r_cert, sz);
	//BIO_free(bio);
 
free_all:
 
    X509_free(cert);
    BIO_free_all(out);
 
    X509_REQ_free(req);
    X509_free(ca);
    EVP_PKEY_free(pkey);

	return (ret == 1);
}
