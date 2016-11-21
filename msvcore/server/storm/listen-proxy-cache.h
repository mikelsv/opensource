#include "../../special/fse3.h"

#define STORM_PROXY_CACHE_VER (10 | 0xDEADF00D)

struct proxy_cache_file{
	MString file;
	HFILE fl;
};

struct proxy_cache_file_data{
	unsigned int ver; // = 0xDEADF00D | version. ;)
	unsigned int last_time;
	unsigned int link_size;
	unsigned int data_size;
	// link + data
};



class proxy_cache_control{
	Fse3 fse;
	IList<proxy_cache_file> files;

public:

	proxy_cache_control(){
		fse.SetDataSize(sizeof(void*));

		LoadCache();
		return ;
	}

protected:

	void LoadCache(){
		Readdir rd;
		MkDir("proxy-cache.dat");

		rd.ReadDir("proxy-cache.dat/*.*");
		for(unsigned int i = 0; i<rd.size(); i++){
			LoadCache(rd[i].key);
		}

		return ;
	}

	int LoadCache(VString fname){
		MString file;
		file.Add("proxy-cache.dat/", fname);

		HFILE fl = CreateFile(file, O_RDWR, S_IREAD|S_IWRITE);

		if(!IsHandle(fl))
			return 0;

		unsigned char buf[S4K];
		int bufs = 0;
		proxy_cache_file_data *data = (proxy_cache_file_data*)buf;

		int rd = ReadFile(fl, buf, sizeof(proxy_cache_file_data));
		if(rd != sizeof(proxy_cache_file_data) || data->ver != STORM_PROXY_CACHE_VER){
			CloseHandle(fl);
			return 0;
		}

		// add to list
		files.A();
		files.n().file = file;
		files.n().fl = fl;
		files.Added();

		int64 fpos = SetFilePointer(fl, 0), npos;

		while(1){
			// read header
			rd = ReadFile(fl, buf, sizeof(proxy_cache_file_data));
			if(!rd)
				break;

			if(rd != sizeof(proxy_cache_file_data) || data->link_size > sizeof(buf) - sizeof(proxy_cache_file_data)){
				// error
				SetFilePointer(fl, fpos);
				SetEndOfFile(fl);
				return 1;
			}

			// read link
			rd = ReadFile(fl, buf + sizeof(proxy_cache_file_data), data->link_size);
			if(rd != data->link_size){
				// error
				SetFilePointer(fl, fpos);
				SetEndOfFile(fl);
				return 1;
			}

			npos = fpos + sizeof(proxy_cache_file_data) + data->link_size + data->data_size;

			if(SetFilePointer(fl, npos) != npos){
				SetFilePointer(fl, fpos);
				SetEndOfFile(fl);
				return 1;
			}

			// add link
			AddLink(files.size() - 1, fpos, data->last_time, VString(buf + sizeof(proxy_cache_file_data), data->link_size));

			fpos = npos;
		}

		return 1;
	}

	int AddLink(int files_id, int64 pos, int ltime, VString link){

		return 0;
	}

};

class listen_proxy_cache : public listen_proxy{

	static proxy_cache_control proxy_cache;

	virtual int AnalysProxy(storm_work_el &wel, VString &read){
		if(!read)
			return 0;

		//
		if(astat == STORMSERVER_PROXY_SOCK5){
			if(read.incompare("GET ") || read.incompare("HEAD")){
				VString rhead, rpost;
				rhead = PartLine(read, rpost, "\r\n\r\n");
				
				// more data
				if(!rpost.data)
					return 0;

				// cache it


			} else
				astat = STORMSERVER_PROXY_SOCK5_IGN;
		}

		if(!sock.SendData(read))
			wel.close();
		wel.readed(read, read);
		
		return 0;
	}	 

	STORM_DEFINE_NEW_DELETE(listen_proxy_cache);
};

STORM_DEFINE_NEW_DELETE_OK(listen_proxy_cache);