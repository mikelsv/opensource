#ifndef USEMSV_MLIST_USED
#error Please define: #define USEMSV_MLIST
#endif

#ifndef USEMSV_XDATACONT_USED
#error Please define: #define USEMSV_XDATACONT
#endif


#define AMF3_UNDEFINED   0x00
#define AMF3_NULL        0x01
#define AMF3_FALSE       0x02
#define AMF3_TRUE        0x03
#define AMF3_INTEGER     0x04
#define AMF3_DOUBLE      0x05
#define AMF3_STRING      0x06
#define AMF3_XMLDOC      0x07
#define AMF3_DATE        0x08
#define AMF3_ARRAY       0x09
#define AMF3_OBJECT      0x0a
#define AMF3_XML         0x0b
#define AMF3_BYTEARRAY   0x0c

#define AMF3_SWITCHTOAMF3  0x11

#define AMF3_MAX_INT     268435455 /*  (2^28)-1 */
#define AMF3_MIN_INT    -268435456 /* -(2^28)   */

// Encoding options
#define AMF3_FORCE_OBJECT      0x01

// Decoding options
#define AMF3_CLASS_MAP         0x01
#define AMF3_CLASS_AUTOLOAD    0x02
#define AMF3_CLASS_CONSTRUCT   0x04


class amfe{
	// http://en.wikipedia.org/wiki/Action_Message_Format
	HLString res, mres;
public:

	VString Get(){ return VString(res.oneline(), res.size()); }

	void EncodePackage(){
		// amf-packet-structure
		char c[2]={0, 0x3};
		// version
		res+VString((char*)c, 2);
		// count
		c[1]=0;
		res+VString((char*)c, 2);
		// header-count*56+ bits	

		// message-count
		c[1]=1;
		res+VString((char*)c, 2);
		return ;
	}

	void EncodeMessageHeader(VString s){
		// message-type-structure
		// target-uri-length
		char c[2]={0, s.sz};
		res+VString(c, 2);
		// target-uri-string
		res+s;
		// response-uri-length
		c[1]=2;
		res+VString(c, 2);
		// response-uri-string
		res+"/1";
		return ;
	}

	void End(){
		char c[4]={0, 0, 0, mres.size()+7};
		// message-length
		res+VString(c, 4);
		// Strict Array - 0x0a (32-bit entry count)
		c[3]=0x0A;
		res+VString(&c[3], 1);
		// Object size
		c[3]=1;
		res+VString(c, 4);
		// Switch to AMF3 - 0x11
		c[3]=0x11;
		res+VString(&c[3], 1);
		// Object - 0x0A
		//EncodeObject();
		c[3]=1;
		mres+VString(&c[3], 1);

		res+mres;

		//unsigned char a[70];
		//memcpy(a, res.oneline(), res.size());

		return ;
	}

	void EncodeObject(VString k, VString v){
		char c[4]={0xa, 0xb, 0x1};
		mres+VString((char*)c, 3);

		EncodeMessage(k, v);
		return ;
	}

	void EncodeMessage(VString k, VString v){
		EncodeStr(k, k.sz);

		char c[4]={0x6};
		mres+VString((char*)c, 1);

		EncodeStr(v, v);
		return ;
	}

	void EncodeU29(int val) {
		char buf[4];
		int size;
		val &= 0x1fffffff;
		if (val <= 0x7f) {
			buf[0] = val;
			size = 1;
		} else if (val <= 0x3fff) {
			buf[1] = val & 0x7f;
			val >>= 7;
			buf[0] = val | 0x80;
			size = 2;
		} else if (val <= 0x1fffff) {
			buf[2] = val & 0x7f;
			val >>= 7;
			buf[1] = val | 0x80;
			val >>= 7;
			buf[0] = val | 0x80;
			size = 3;
		} else {
			buf[3] = val;
			val >>= 8;
			buf[2] = val | 0x80;
			val >>= 7;
			buf[1] = val | 0x80;
			val >>= 7;
			buf[0] = val | 0x80;
			size = 4;
		}
		mres+VString(buf, size);
	}

	void EncodeStr(VString str, int len) {
		EncodeU29((len << 1) | 1);
		mres+str;
		return ;
	}

};


class amfd_index_d{
public:
	VString text; int id;
};

int amfd_index_find(amfd_index_d &el, int &i){
	return el.id==i;
}

class amfd_index{
	MList<amfd_index_d> list;
	int lid;
public:

	amfd_index(){ lid = 0; }
	~amfd_index(){ Clean(); }

	void Add(VString text, int i=-1){
		UGLOCK(list);
		if(i==-1) i=lid++;
		amfd_index_d *el=list.New();
		el->text=text; el->id=i;
	}

	amfd_index_d* Get(int i){
		amfd_index_d* el=list.Find(amfd_index_find, i);
		return el;
	}

	void Clean(){
		list.Clear();
		lid = 0;
	}
	
};

struct amfd_opt{
	// str
	VString *str; int strs;
	// parent el
	XDataEl *el;

	amfd_opt(){ str=0; strs=0; el=0; }

	amfd_opt& operator=(const amfd_opt &opt){
		str=0; strs=0; el=opt.el;
		return *this;
	}

};

class amfd : public XDataCon{

	XDataLine data;
	HLString sdata;
	amfd_index index;
	int amf3;

	//VString *vstri; int vstris;



public:

	amfd(){ amf3=0; }
	~amfd(){ Clean(); }

	void Clean(){
		data.Clear();
		sdata.Clear();
		index.Clean();
		amf3=0;
	}


	// Get Data
	operator XDataEl*(){ if(data) return &data.el(0); return 0; }
	XDataEl* GetData(){ if(data) return &data.el(0); return 0; }


	int Decode(VString line){
		if(line.sz<8)
			return 0;
		
		unsigned char *ln=line, *to=line.endu();

		// amf-packet-structure
		// version
		unsigned short ver=htons(*(unsigned short*)ln); ln+=2;
		// header-count
		unsigned short count=htons(*(unsigned short*)ln); ln+=2;
		// message-count
		unsigned short mcount=htons(*(unsigned short*)ln); ln+=2;

		// header-type-structure
		for(int i=0; i<count; i++){
			if(to-ln<6)
				return 0;

			// header-name-length
			unsigned short len=htons(*(unsigned short*)ln); ln+=2;

			// header-name-string
			VString str(ln, len); ln+=len;

			ln++;

			if(to-ln<2)
				return 0;

			// header-length
			unsigned int mlen=htonl(*(unsigned int*)ln); ln+=4;
			
			ln+=mlen;

			// DecodeHeader(ln, to);
			//printf("amfd DecodeHeader() not create.\r\n");
			//return 0;
		}

		XDataEl *parent=XDataAdd(data);

		// message-type-structure
		for(int i=0; i<mcount; i++){			
			if(to-ln<6)
				return 0;

			// target-uri-length
			unsigned short len=htons(*(unsigned short*)ln); ln+=2;
			// target-uri-string
			VString str(ln, len); ln+=len;

			if(to-ln<2)
				return 0;

			// response-uri-length
			unsigned short rlen=htons(*(unsigned short*)ln); ln+=2;
			// response-uri-string
			VString rstr(ln, rlen); ln+=rlen;

			if(to-ln<2)
				return 0;

			// message-length
			unsigned int mlen=htonl(*(unsigned int*)ln); ln+=4;
			if(mlen==0xFFFFFFFF)
				mlen=to-ln;

			if(ln+mlen>to)
				return 0;

			XDataEl *el=XDataAdd(data, parent);
			el->key="message";


			amfd_opt opt; opt.el=el;
			if(!DecodeValue(opt, ln, ln+mlen))
				return 0;
		}
		
		//XDataPrint(GetData());

		return 1;
	}

	int DecodeT(VString line, int fel = 0){
		if(line.sz<8)
			return -1;
		
		unsigned char *ln=line, *to=line.endu();

		// wtf packet structure
		unsigned short ver = htons(*(unsigned short*)ln); ln+=2;
		unsigned short size = htons(*(unsigned short*)ln); ln+=2;
		unsigned short ext = htons(*(unsigned short*)ln); ln+=2;
		unsigned short wft2 = htons(*(unsigned short*)ln); ln+=2;

		if(ver)
			print("AMF VER DECODE FAIL!\r\n");
		
		unsigned short arr[8];

		if(!ver && ext){
			//if(line.sz != ln - line.data + size)
			for(int i=0; i<8; i++){
				arr[i] = htons(*(unsigned short*)ln);
				ln += 2;
			}
			//ln += 16;
			int ty=678;
			// 256, 0, 16b
			// 255, 255.0, 1442b

			// 42223, 63458, 24958, 32289, 46891, 2836, 42656, 10581
			// 16817, 36196, 53588, 22487, 8164, 29661, 63748, 19157
			// 52693, 11083, 20130, 16310, 45588, 64668, 20170, 22023
			// 30876, 50522, 31576, 21717, 5791, 37181, 59528, 2326 - 255
		}

		if(ext && wft2 && line.sz != size + 8){
			char fd[] = { 0xa, 0xb, 0x01 };
			VString l = PartLineOT(line, VString(fd, 3));
			if(l){
				ln = l.data - 3;
			}
		}

		if(ln + size > to)
			return -1;

		XDataEl *parent=XDataAdd(data);
//		XDataEl *el=XDataAdd(data, parent);
		parent->key="data";

		amf3 = 1;

		int fsz = ln - line.data + size;

		amfd_opt opt; opt.el=parent;
		if(!DecodeValue(opt, ln, ln+size)){
			print("DECODE FAIL!\r\n");
			return line;
			return fsz;
			return 0;
		}
		
		//XDataPrint(GetData());

		return fsz;
	}

	int DecodeD(VString line){
		Clean();
		amf3 = 1;

		XDataEl *parent=XDataAdd(data);
		parent->key="data";

		unsigned char *ln = line;

		amfd_opt opt; opt.el=parent;
		if(!DecodeValue(opt, ln, line.endu())){
			return 0;
		}

		return line;
	}

	int DecodeValue(amfd_opt &opt, unsigned char *&ln, unsigned char *to, int one=0){
		VString str; double d; unsigned int i;

		if(!amf3){
			while(ln<to && !amf3){
				switch(*ln){
					case AMF3_OBJECT:{
						ln++;
						if(ln+4>to)
							return 0;
						unsigned int count=htonl(*(unsigned int*)ln); ln+=4;

						for(int i=0; i<count; i++){
							amfd_opt nopt=opt;
							if(!DecodeValue(nopt, ln, to))
								return 0;
						}
					break;}

					case AMF3_SWITCHTOAMF3:
						ln++; amf3=1;
					break;

					default:
						return 0;
					break;
				}
			}

			if(!amf3)
				return 1;
		}

		if(opt.el->key=="id")
			int rt=67;

		while(ln<to){
			switch(*ln++){

			case AMF3_UNDEFINED:
				return 0;
			break;

			case AMF3_NULL:
				//ln++;
			break;

			case AMF3_FALSE:
				opt.el->val=sdata.addnfr("0");
			break;

			case AMF3_TRUE:
				//ln++;
				opt.el->val=sdata.addnfr("1");
			break;

			case AMF3_INTEGER:
				if(!DecodeU29(i, ln, to)) return 0;
				opt.el->val=sdata.addnfr(itos(i));	
			break;

			case AMF3_DOUBLE:
				//ln++;
				DecodeDouble(d, ln, to);
				opt.el->val=sdata.addnfr(dtos(d));				
			break;

			case AMF3_XML:
			//	if(!DecodeStr(str, ln, to)) return 0;
			//	opt.el->val=str;
			//break;

			case AMF3_XMLDOC:

			//break;

			case AMF3_BYTEARRAY:
			//	i=6;
			//	if(!DecodeByteArray(ln, to)) return 0;

			//break;
			case AMF3_STRING:
				//ln++;
				if(!DecodeStr(str, ln, to)) return 0;
				opt.el->val=str;
			break;

			case AMF3_ARRAY:
				if(!DecodeArray(opt, ln, to)) return 0;
			break;

			// Object
			case AMF3_OBJECT:
				//ln++;
				if(!DecodeObject(opt, ln, to)) return 0;
			break;

//			case AMF3_SWITCHTOAMF3:
//				amf3=1;
//			break;

			// no way
			default:
				return 0;
			break;
			}

			if(one)
				return 1;
		}

		return 1;
	}


	int DecodeU29(unsigned int &res, unsigned char *&ln, unsigned char *to){
		unsigned char *lto=min(ln+4, to);
		int cycle=0, tmp; res=0;

		if(ln>=to) return 0;

		do{
			tmp=*ln;

			if(cycle == 3){
				res <<= 8;
				res |= tmp & 0xff;
				ln++;
				break;
			}else{
				res <<= 7;
				res |= tmp & 0x7f;
			}
		} while((ln++ < lto) && ++cycle && (tmp & 0x80));

		return 1;
	}


	int DecodeStr(VString &str, unsigned char *&ln, unsigned char *to){
		unsigned int ofs, pfx, def;
		if(!DecodeU29(pfx, ln, to)) return 0;
		def = pfx & 1;
		pfx >>= 1;

		if(def){
			if(ln+pfx>to)
				return 0;

			str.setu(ln, pfx);
			ln+=pfx;
			// add to hash
			if(str) index.Add(str);
		}else{
			// get from hash
			amfd_index_d *el=index.Get(pfx);
			if(el)
				str=el->text;
			else
			//	return 0;

			str="get_from_hashe";
		}

		return 1;
	}

	int DecodeRef(unsigned int &len, unsigned char *&ln, unsigned char *to){
		unsigned int ofs, pfx, def;
		if(!DecodeU29(pfx, ln, to))
			return 0;

		def = pfx & 1;
		pfx >>= 1;
		if(def) len = pfx;
		else{
			return 1;
			// get from hashe
		}
		return 1;
	}

	int DecodeDouble(double &val, unsigned char *&ln, unsigned char *to){
		union { double d; char c[8]; } u;

		if(ln+8>to)
			return 0;

		for(int i = 0; i < 8; ++i) u.c[i] = ln[7 - i];
		val = u.d;
		ln+=8;
		return 1;
	}

	int DecodeObject(amfd_opt &opt, unsigned char *&ln, unsigned char *to, int opts=0){
		unsigned int ofs, pfx, id=0;
		if(!DecodeRef(pfx, ln, to)) return 0;
	
		XDataEl *el=0; VString str;
		int el_cnt=0;
		VString vstr[S1K];

		int def = pfx & 1;
		pfx >>= 1;

		if(def){
			int r=DecodeStr(str, ln, to);
			if(r<1)
				return r;

			if(str){
				el=XDataAdd(data, opt.el);
				el->key="title";
				el->val=str;
			}

			//if(!opt.str){
				//opt.str=vstr;
				//setstr=1;
			//}

			el_cnt=pfx>>2;
			if(el_cnt){
				opt.strs=el_cnt;

				for(int i=0; i<el_cnt; ++i) { /* static member names */
					int r=DecodeStr(str, ln, to);
					if(r<1)
						return r;

					if(str.sz<0){
						break;
					}
					if(opt.str) opt.str[i]=str;
					else
						vstr[i]=str;

					//XDataEl *el=XDataAdd(data, parent);
					//el->key=sdata.addnfr(itos(i));
					//el->val=str;
				}
			}
		}

		if(opt.strs){
			for(int i=0; i<opt.strs; i++){
				el=XDataAdd(data, opt.el);
				if(opt.str)
					el->key=opt.str[i];
				else
					el->key=vstr[i];
			
				amfd_opt nopt; nopt.el=el;
				if(!DecodeValue(nopt, ln, to, 1)) return 0;
			}
			return 1;
		}

		if(pfx&1){ /* externalizable */
			XDataEl *el=XDataAdd(data, opt.el);
			el->key=sdata.addnfr(itos(id++));

			amfd_opt nopt; nopt.el=el;
			if(!DecodeValue(nopt, ln, to, 1)) return 0;
		}
		else
			if(pfx&2 || !def && !pfx){ /* dynamic */
				while(ln<to){
					if(!DecodeStr(str, ln, to)) return 0;
					if(!str) break;

					el=XDataAdd(data, opt.el);
					el->key=str;
					//el_cnt=n;

					amfd_opt nopt; nopt.el=el;
					if(!DecodeValue(nopt, ln, to, 1)) return 0;
				}
			}
		else{
			amfd_index_d *el=index.Get(pfx);
		}

		return 1;
	}

	int DecodeByteArray(unsigned char *&ln, unsigned char *to){
		unsigned int len, id=0; VString str; unsigned char *pln=ln;
		if(!DecodeRef(len, ln, to)) return 0;


		return 1;
	}

	int DecodeArray(amfd_opt &opt, unsigned char *&ln, unsigned char *to){
		unsigned int len, id=0; VString str; unsigned char *pln=ln;
		if(!DecodeRef(len, ln, to)) return 0;

		//if(!len) return 1;

		//XDataEl *pel=XDataAdd(data, parent);

		VString vstr[S1K];
		amfd_opt nopt; nopt.str=vstr;

		if(len>=0){
			for(;;){ /* associative portion */
				if(!DecodeStr(str, ln, to)) return 0;
				if(!str) break;

				XDataEl *el=XDataAdd(data, opt.el);
				el->key=str;
	
				nopt.el=el; 
				int r=DecodeValue(nopt, ln, to, 1);
				if(r<1) return r;
			}

			while(len-- > 0){ /* dense portion */
				XDataEl *el=XDataAdd(data, opt.el);
				el->key=sdata.addnfr(itos(id++));

				nopt.el=el;
				int r=DecodeValue(nopt, ln, to, 1);
				if(r<1)
					return r;
				if(ln >= to)
					return 0;
			}
		}
		return 1;
	}

};
