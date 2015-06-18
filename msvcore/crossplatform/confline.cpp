//class ConfLine{
//	MString data, file;
//
//public:
	ConfLine::ConfLine(){}
	ConfLine::ConfLine(VString file){ Load(file);  }

	int ConfLine::Is(){ return data; }

VString ConfLine::Load(VString fl){
		data=LoadFile(fl); /*if(data) */file=fl; return data;
	}

VString ConfLine::LoadData(VString fl, VString dt){
		data=dt; /*if(data) */file=fl; return data;
	}

	VString ConfLine::Reload(){
		data=LoadFile(file); return data;
	}

	int ConfLine::Save(){
		return SaveFile(file, data);
	}

	int ConfLine::Save(VString fl){
		file=fl;
		return SaveFile(file, data);
	}

	VString ConfLine::operator[](VString line){ return Get(line); }

	VString ConfLine::Get(VString line){ // , VString to="" for features
		unsigned char *pos=data;//, lpos;
		while(int r=rtms(data.endu(), line, line, pos)){
			if((pos==data.data || (*(pos-1)==10 || *(pos-1)==13)) &&
				(pos+line.sz<data.endu() && (*(pos+line.sz)==' ' || *(pos+line.sz)==9 || *(pos+line.sz)==10 || *(pos+line.sz)==13) || pos+line.sz==data.endu())){
					VString ret; ret.data=pos+line.sz;
					while(pos<data.endu() && *pos!=10 && *pos!=13) pos++;
					ret.sz=pos-ret.data;
					//dspacev(ret, 7);
					while(ret.sz && (*ret.data==' ' || *ret.data=='\t')){ ret.data++; ret.sz--; }
					while(ret.sz && (ret.endo()==' ' || ret.endo()=='\t')){ ret.sz--; }
					if(!ret) ret.data=pos;
					return ret;
			}
			pos++;
		}
		return VString();
	}

	VString ConfLine::Set(VString line, VString set){
		VString l=Get(line);
		if(l.data){
			data.Add(VString(data.data, l.data-data.data), (*(l.data-1)!=9 && *(l.data-1)!=32) ? " " : "", set, VString(l.endu(), data.endu()-l.endu()));
		}else{
			data.Add(data, data ? "\r\n" : "", line, " ", set);
		}
		return l;
	}

	VString ConfLine::GetLine(unsigned int &p){ // if return p==0 then EOF.
		unsigned char *ln=data.data+p, *lln=ln, *to=data.endu(); if(p>=data.sz){ p=0; return ""; }
		if(p>data.size()){ p=0; return VString(); }
		while(ln<to){
			if(*ln==10 || *ln==13){
				VString ret; ret.setu(lln, ln-lln);
				while(ln<to && (*ln==10 || *ln==13)) ln++;
				p=ln-data.data;
				return ret;
			}
			ln++;
		}

		//p=0;
		p=ln-data.data;
		VString ret; ret.setu(lln, ln-lln);
		return ret;
	}

	unsigned int ConfLine::GetLine(VString &line, unsigned int p){
		line=GetLine(p); return p;
	}

	VString ConfLine::GetData(){
		return data;
	}

	void ConfLine::Clear(){ data.Clean(); file.Clean(); }
	void ConfLine::ClearData(){ data.Clean(); }

//};
