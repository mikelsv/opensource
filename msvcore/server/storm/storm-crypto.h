//#ifdef USEMSV_OPENSSL
#ifdef IAM_YOU_CODE___NOOOOOOO

class storm_ssl{
	SSL *ssl;
	SSL_CTX *ctx;
	int work;

public:

	storm_ssl(){
		ssl = 0;
		ctx = 0;
		work = 0;
	}

	int IsWork(int isrecv = 0){
		if(isrecv && work == 1){

			int r = 0; // SSL_accept(ssl);

			if(r<0){
				ERR_print_errors_fp(stderr);
				Release();
				return 0;
			}

			work++;
		}

		return work>1;
	}

	int Connect(SOCKET sock){
		Release();

		ctx = SSL_CTX_new(SSLv23_method());
		if(!ctx){
			Release();
			return 0;
		}

		ssl = SSL_new(ctx);
		if(!ssl){
			Release();
			return 0;
		}

		if(!SSL_set_fd(ssl, sock)){
			Release();
			return 0;
		}

		SSL_CTX_set_timeout(ctx, 60);

		int err=SSL_connect(ssl);

		work = 1+1;
		return err;
	}

	int Accept(SOCKET sock, VString cert, VString key){
		Release();

		ctx = SSL_CTX_new(SSLv23_server_method());
		if(!ctx){
			Release();
			return 0;
		}

		if(!LoadCert(ctx, cert, key)){
			return 0;
		}

		ssl = SSL_new(ctx);
		if(!ssl){
			Release();
			return 0;
		}

		if(!SSL_set_fd(ssl, sock)){
			Release();
			return 0;
		}

		if(SSL_accept(ssl) < 0){
			ERR_print_errors_fp(stderr);
			Release();
			return 0;
		}

		SSL_CTX_set_timeout(ctx, 60);

		work = 1;

		return 1;
	}

	int AcceptFile(SOCKET sock, VString cert, VString key){
		Release();

		int r = 0;

		ctx = SSL_CTX_new(SSLv23_server_method());
		if(!ctx){
			Release();
			return 0;
		}

		if(!LoadCertFile(ctx, cert, key)){
			return 0;
		}

		ssl = SSL_new(ctx);
		if(!ssl){
			Release();
			return 0;
		}

		if(!SSL_set_fd(ssl, sock)){
			Release();
			return 0;
		}

		if(SSL_accept(ssl) < 0){
			ERR_print_errors_fp(stderr);
			Release();
			return 0;
		}

		SSL_CTX_set_timeout(ctx, 60);

//char buf[8*1024];
//    char reply[8*1024];
//    int sd, bytes;
//    const char* HTMLecho="<html><body><pre>%s</pre></body></html>\n\n";
//
	//sd = SSL_get_fd(ssl);
	//SSL_accept(ssl);

	//bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
	//
	//memcpy(buf, "HTTP/1.1 200 OK\r\n\r\n", 19);
	//bytes = SSL_write(sd, buf, 19); /* get request */

    //if ( SSL_accept(ssl) == -1 ){     /* do SSL-protocol accept */
	//	int err = ERR_get_error();
    //    ERR_print_errors_fp(stderr);
	//}
 //   else
 //   {
 //       //ShowCerts(ssl);        /* get any certificates */
 //       bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
 //       if ( bytes > 0 )
 //       {
 //           buf[bytes] = 0;
 //           printf("Client msg: \"%s\"\n", buf);
 //           sprintf(reply, HTMLecho, buf);   /* construct reply */
 //           SSL_write(ssl, reply, strlen(reply)); /* send reply */
 //       }
 //       else
 //           ERR_print_errors_fp(stderr);
 //   }




		work = 1;

		return 1;
	}

	int LoadCert(SSL_CTX *ctx, VString cert, VString key){
		if(!cert || !key)
			return 0;

		int ret;

		// Read X509
		RSA *rsa = 0;
		X509 *x509 = NULL;
		BIO *b=BIO_new_mem_buf(cert, cert);
		x509 = PEM_read_bio_X509(b, NULL, 0, NULL);

		ret=SSL_CTX_use_certificate(ctx, x509);

		BIO_free(b);
		X509_free(x509);
		
		if(ret<=0){
			ERR_print_errors_fp(stderr);
			return 0;
		}

		// Read Private key
		b=BIO_new_mem_buf(key, key);
		rsa=PEM_read_bio_RSAPrivateKey(b, 0, 0, 0);

		ret=SSL_CTX_use_RSAPrivateKey(ctx, rsa);

		BIO_free(b);
		RSA_free(rsa);

		if(ret<=0){
			ERR_print_errors_fp(stderr);
			return 0;
		}


		/* verify private key */
		if(!SSL_CTX_check_private_key(ctx)){
			fprintf(stderr, "Private key does not match the public certificate\n");
			return 0;
		}

		//New lines - Force the client-side have a certificate
		//SSL_CTX_set_cipher_list(ctx, "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:SRP-DSS-AES-256-CBC-SHA:SRP-RSA-AES-256-CBC-SHA:DHE-DSS-AES256-GCM-SHA384:DHE-RSA-AES256-GCM-SHA384:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA256:DHE-RSA-AES256-SHA:DHE-DSS-AES256-SHA:DHE-RSA-CAMELLIA256-SHA:DHE-DSS-CAMELLIA256-SHA:ECDH-RSA-AES256-GCM-SHA384:ECDH-ECDSA-AES256-GCM-SHA384:ECDH-RSA-AES256-SHA384:ECDH-ECDSA-AES256-SHA384:ECDH-RSA-AES256-SHA:ECDH-ECDSA-AES256-SHA:AES256-GCM-SHA384:AES256-SHA256:AES256-SHA:CAMELLIA256-SHA:PSK-AES256-CBC-SHA:ECDHE-RSA-DES-CBC3-SHA:ECDHE-ECDSA-DES-CBC3-SHA:SRP-DSS-3DES-EDE-CBC-SHA:SRP-RSA-3DES-EDE-CBC-SHA:EDH-RSA-DES-CBC3-SHA:EDH-DSS-DES-CBC3-SHA:ECDH-RSA-DES-CBC3-SHA:ECDH-ECDSA-DES-CBC3-SHA:DES-CBC3-SHA:PSK-3DES-EDE-CBC-SHA:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:SRP-DSS-AES-128-CBC-SHA:SRP-RSA-AES-128-CBC-SHA:DHE-DSS-AES128-GCM-SHA256:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES128-SHA256:DHE-DSS-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA:DHE-RSA-SEED-SHA:DHE-DSS-SEED-SHA:DHE-RSA-CAMELLIA128-SHA:DHE-DSS-CAMELLIA128-SHA:ECDH-RSA-AES128-GCM-SHA256:ECDH-ECDSA-AES128-GCM-SHA256:ECDH-RSA-AES128-SHA256:ECDH-ECDSA-AES128-SHA256:ECDH-RSA-AES128-SHA:ECDH-ECDSA-AES128-SHA:AES128-GCM-SHA256:AES128-SHA256:AES128-SHA:RC4-SHA:SEED-SHA:CAMELLIA128-SHA:IDEA-CBC-SHA:PSK-AES128-CBC-SHA:ECDHE-RSA-RC4-SHA:ECDHE-ECDSA-RC4-SHA:ECDH-RSA-RC4-SHA:ECDH-ECDSA-RC4-SHA:RC4-MD5:PSK-RC4-SHA:EDH-RSA-DES-CBC-SHA:EDH-DSS-DES-CBC-SHA:DES-CBC-SHA:EXP-EDH-RSA-DES-CBC-SHA:EXP-EDH-DSS-DES-CBC-SHA:EXP-DES-CBC-SHA:EXP-RC2-CBC-MD5:EXP-RC4-MD5"); 
		//SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
		//SSL_CTX_set_verify_depth(ctx, 4);
		//End new lines

		return 1;
	}

	int LoadCertFile(SSL_CTX *ctx, VString cert, VString key){
		TString CertFile = cert, KeyFile = key;

		//New lines 
		if(SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1){
			ERR_print_errors_fp(stderr);
			return 0;
		}

		if (SSL_CTX_set_default_verify_paths(ctx) != 1){
			ERR_print_errors_fp(stderr);
			return 0;
		}
		//End new lines

		/* set the local certificate from CertFile */
		if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0)
		{
			ERR_print_errors_fp(stderr);
			return 0;
		}
		/* set the private key from KeyFile (may be the same as CertFile) */
		if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0)
		{
			ERR_print_errors_fp(stderr);
			return 0;
		}
		/* verify private key */
		if (!SSL_CTX_check_private_key(ctx))
		{
			fprintf(stderr, "Private key does not match the public certificate\n");
			return 0;
		}

		//New lines - Force the client-side have a certificate
		//SSL_CTX_set_cipher_list(ctx, "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:SRP-DSS-AES-256-CBC-SHA:SRP-RSA-AES-256-CBC-SHA:DHE-DSS-AES256-GCM-SHA384:DHE-RSA-AES256-GCM-SHA384:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA256:DHE-RSA-AES256-SHA:DHE-DSS-AES256-SHA:DHE-RSA-CAMELLIA256-SHA:DHE-DSS-CAMELLIA256-SHA:ECDH-RSA-AES256-GCM-SHA384:ECDH-ECDSA-AES256-GCM-SHA384:ECDH-RSA-AES256-SHA384:ECDH-ECDSA-AES256-SHA384:ECDH-RSA-AES256-SHA:ECDH-ECDSA-AES256-SHA:AES256-GCM-SHA384:AES256-SHA256:AES256-SHA:CAMELLIA256-SHA:PSK-AES256-CBC-SHA:ECDHE-RSA-DES-CBC3-SHA:ECDHE-ECDSA-DES-CBC3-SHA:SRP-DSS-3DES-EDE-CBC-SHA:SRP-RSA-3DES-EDE-CBC-SHA:EDH-RSA-DES-CBC3-SHA:EDH-DSS-DES-CBC3-SHA:ECDH-RSA-DES-CBC3-SHA:ECDH-ECDSA-DES-CBC3-SHA:DES-CBC3-SHA:PSK-3DES-EDE-CBC-SHA:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:SRP-DSS-AES-128-CBC-SHA:SRP-RSA-AES-128-CBC-SHA:DHE-DSS-AES128-GCM-SHA256:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES128-SHA256:DHE-DSS-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA:DHE-RSA-SEED-SHA:DHE-DSS-SEED-SHA:DHE-RSA-CAMELLIA128-SHA:DHE-DSS-CAMELLIA128-SHA:ECDH-RSA-AES128-GCM-SHA256:ECDH-ECDSA-AES128-GCM-SHA256:ECDH-RSA-AES128-SHA256:ECDH-ECDSA-AES128-SHA256:ECDH-RSA-AES128-SHA:ECDH-ECDSA-AES128-SHA:AES128-GCM-SHA256:AES128-SHA256:AES128-SHA:RC4-SHA:SEED-SHA:CAMELLIA128-SHA:IDEA-CBC-SHA:PSK-AES128-CBC-SHA:ECDHE-RSA-RC4-SHA:ECDHE-ECDSA-RC4-SHA:ECDH-RSA-RC4-SHA:ECDH-ECDSA-RC4-SHA:RC4-MD5:PSK-RC4-SHA:EDH-RSA-DES-CBC-SHA:EDH-DSS-DES-CBC-SHA:DES-CBC-SHA:EXP-EDH-RSA-DES-CBC-SHA:EXP-EDH-DSS-DES-CBC-SHA:EXP-DES-CBC-SHA:EXP-RC2-CBC-MD5:EXP-RC4-MD5"); 
		//SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
		//SSL_CTX_set_verify_depth(ctx, 4);
		//End new lines

		return 1;
	}

	int recv(void *buf, int sz){
		if(!ssl)
			return 0;

		int ret = SSL_read(ssl, buf, sz), sret;
		if(ret == -1){
			sret = SSL_get_error(ssl, ret);
			if(sret == SSL_ERROR_WANT_READ)
				ret = STORM_RECV_WAIT;
		}		

		return ret;
	}

	int send(const char *buf, int sz){
		if(!ssl)
			return 0;

		return SSL_write(ssl, buf, sz);
	}

	//bool ifrecv(unsigned int wt=10){
	//	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
	//	tm.tv_sec=wt/1000; tm.tv_usec=1000;
	//	return SSL_select(sock+1, &rfds, 0, 0, &tm)!=0;
	//}

	//bool ifsend(unsigned int wt=10){
	//	timeval tm; fd_set rfds; FD_ZERO(&rfds); FD_SET(sock, &rfds);
	//	tm.tv_sec=wt/1000; tm.tv_usec=0;
	//	return SSL_select(sock+1, 0, &rfds, 0, &tm)!=0;
	//}

	void Release(){
		//domain.Clean();

		if(ssl)
			SSL_free(ssl);
		if(ctx)
			SSL_CTX_free(ctx);

		ssl = 0; ctx = 0; work = 0;
	}

	~storm_ssl(){
		Release();

	}


};



#endif
#ifndef USEMSV_OPENSSL

class MySSL{

public:
	int IsWork(int isrecv=0){
		return 0;
	}

	int Connect(SOCKET sock){
		return 0;
	}

	int Accept(SOCKET sock, VString cert, VString key){
		return 0;
	}

	int AcceptFile(SOCKET sock, VString cert, VString key){
		return 0;
	}

	int Recv(void*, int sz){
		return 0;
	}
	
	int Send(void*, int sz){
		return 0;
	}

};
#endif



class storm_core_ssl : public storm_item{
protected:
	// storm_ssl *ssl;
	MySSL *ssl;
	
	virtual void storm_init(){
		ssl = 0;
	}

	virtual void storm_stop(){
		delete ssl;
		ssl = 0;
	}

	~storm_core_ssl(){
		storm_stop();
	}

	// ssl
	virtual int storm_socket_recv(SOCKET sock, char *data, int sz, int flag){
		if(ssl && ssl->IsWork(1))
			return ssl->Recv(data, sz);
		return recv(sock, data, sz, flag);
	}
	virtual int storm_socket_send(SOCKET sock, char *data, int sz, int flag){
		if(ssl && ssl->IsWork())
			return ssl->Send(data, sz);
		return send(sock, data, sz, flag);
	}

	virtual int storm_ssl_connect(SOCKET sock){
		delete ssl;
		ssl = new MySSL;

		return ssl->Connect(sock);
	}

	virtual int storm_ssl_accept(SOCKET sock, VString cert, VString key){
		delete ssl;
		ssl = new MySSL;

		return ssl->Accept(sock, cert, key);
	}

	virtual int storm_ssl_accept_file(SOCKET sock, VString cert, VString key){
		delete ssl;
		ssl = new MySSL;

		return ssl->AcceptFile(sock, cert, key);
	}

};