class listen_raw : public storm_item{

	virtual void storm_recv(storm_work_el &wel, VString read){
		wel.send(read); // send data
		wel.readed(read); // readed all data
	}

	STORM_DEFINE_NEW_DELETE(listen_raw);
};

STORM_DEFINE_NEW_DELETE_OK(listen_raw);