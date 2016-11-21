 void USEMSV_STORMSERVER_HTTPJSON_CALL(VString head, VString data, TString &ret);

class listen_http_json : public listen_http{

	 virtual int AnalysSend(storm_work_el &wel){
		 VString data = post;
		 TString ret;

		 USEMSV_STORMSERVER_HTTPJSON_CALL(head, data, ret);

		 CloseConnection(wel, ret);
		 return 0;
	 }

 	STORM_DEFINE_NEW_DELETE(listen_http_json);
};

STORM_DEFINE_NEW_DELETE_OK(listen_http_json);

//listen_http *listen_http::static_con = 0;